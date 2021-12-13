#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <linux/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/un.h>


#include "bzstruct.h"


static inline uint32_t string_hash(const char *str)
{
	uint32_t hash = 0;
	char c;
	if(str == NULL)
		return 0;
	while( (c = *str++) != 0)
		hash += c;
	return hash;
}


const char *bzstruct_dtype_string(bzstruct_dtype_t type)
{

#define DO_CASE(x) \
	case x: \
		return #x;
	
	switch(type)
	{
		DO_CASE(BZSTRUCT_DTYPE_UNKNOWN)
		DO_CASE(BZSTRUCT_DTYPE_CHAR)	
		DO_CASE(BZSTRUCT_DTYPE_STRING)
		DO_CASE(BZSTRUCT_DTYPE_CHAR_ARRAY)
		DO_CASE(BZSTRUCT_DTYPE_INT8)
		DO_CASE(BZSTRUCT_DTYPE_INT8_ARRAY)
		DO_CASE(BZSTRUCT_DTYPE_INT8_POINT)
		DO_CASE(BZSTRUCT_DTYPE_UINT8)
		DO_CASE(BZSTRUCT_DTYPE_UINT8_ARRAY)
		DO_CASE(BZSTRUCT_DTYPE_UINT8_POINT)
		DO_CASE(BZSTRUCT_DTYPE_INT16)
		DO_CASE(BZSTRUCT_DTYPE_INT16_ARRAY)
		DO_CASE(BZSTRUCT_DTYPE_INT16_POINT)
		DO_CASE(BZSTRUCT_DTYPE_UINT16)
		DO_CASE(BZSTRUCT_DTYPE_UINT16_ARRAY)
		DO_CASE(BZSTRUCT_DTYPE_UINT16_POINT)
		DO_CASE(BZSTRUCT_DTYPE_INT32)
		DO_CASE(BZSTRUCT_DTYPE_INT32_ARRAY)
		DO_CASE(BZSTRUCT_DTYPE_INT32_POINT)
		DO_CASE(BZSTRUCT_DTYPE_UINT32)
		DO_CASE(BZSTRUCT_DTYPE_UINT32_ARRAY)
		DO_CASE(BZSTRUCT_DTYPE_UINT32_POINT)
		DO_CASE(BZSTRUCT_DTYPE_INT64)
		DO_CASE(BZSTRUCT_DTYPE_INT64_ARRAY)
		DO_CASE(BZSTRUCT_DTYPE_INT64_POINT)
		DO_CASE(BZSTRUCT_DTYPE_UINT64)
		DO_CASE(BZSTRUCT_DTYPE_UINT64_ARRAY)
		DO_CASE(BZSTRUCT_DTYPE_UINT64_POINT)
		DO_CASE(BZSTRUCT_DTYPE_CSTRUCT)
		DO_CASE(BZSTRUCT_DTYPE_CSTRUCT_ARRAY)
		DO_CASE(BZSTRUCT_DTYPE_CSTRUCT_POINT)
		DO_CASE(BZSTRUCT_DTYPE_UNION)
		default:
			break;
	}
#undef DO_CASE
	return "Unkonwn";
}


static struct bzstruct *_bzstruct_lookup(bzstruct_ctx_t *ctx, const char *name)
{
	struct bzstruct *cstruct;
	uint32_t index;

	index = string_hash(name) % BZSTRUCT_HASH_SIZE;
	
	TAILQ_FOREACH(cstruct, &ctx->cstruct[index], entry)
	{
		if(strcmp(cstruct->name , name) == 0)
			return cstruct;
#if 0
		if(cstruct->aliase != NULL)
		{
			if(strcmp(cstruct->aliase , name) == 0)
				return cstruct;
		}
#endif
	}
	return NULL;
}

struct bzstruct *bzstruct_lookup(bzstruct_ctx_t *ctx, const char *name)
{
	struct bzstruct *p;

	p = _bzstruct_lookup(ctx, name);
	if(p != NULL)
		return (p->ref == NULL ? p : p->ref);
	return NULL;
}


int bzstruct_size_get(bzstruct_ctx_t *ctx, const char *name)
{
	struct bzstruct *stru;

	if(ctx == NULL || name == NULL)
		return 0;
	
	stru = bzstruct_lookup(ctx, name);
	if(stru != NULL)
		return stru->size;
	return 0;
}

struct bzstruct_field *bzstruct_field_lookup(struct bzstruct *bzstruct, const char *name)
{
	struct bzstruct_field *field;

	TAILQ_FOREACH(field, &bzstruct->field, entry)
	{
		if(strcmp(field->name , name) == 0)
			return field;
	}
	return NULL;
}

struct bzstruct_field *bzstruct_maxsize_field_lookup(struct bzstruct *bzstruct)
{
	struct bzstruct_field *field, *p = NULL;
	int size = 0;

	TAILQ_FOREACH(field, &bzstruct->field, entry)
	{
		if(field->size > size)
		{
			p = field;
			size = field->size;
		}
	}
	return p;
}




int bzstruct_encode(bzstruct_ctx_t *ctx, const char *struct_name, void *cstruct, uint8_t **buf, int *len)
{
	if(ctx == NULL || ctx->spec == NULL || struct_name == NULL || buf == NULL)
	{
		BZSTRUCT_LOG_ERR("Bad parameter");
		return BZSTRUCT_E_BADPARAM;
	}

	return ctx->spec->encode(ctx, struct_name, cstruct, buf, len);
}

int bzstruct_decode(bzstruct_ctx_t *ctx, const char *struct_name, void *cstruct, uint8_t *buf, int len)
{
	if(ctx == NULL || ctx->spec == NULL || struct_name == NULL || buf == NULL)
	{
		BZSTRUCT_LOG_ERR("Bad parameter");
		return BZSTRUCT_E_BADPARAM;
	}

	return ctx->spec->decode(ctx, struct_name, cstruct, buf, len);
}

int bzstruct_to_obj(bzstruct_ctx_t *ctx, const char *struct_name, void *cstruct, void **obj)
{
	if(ctx == NULL || ctx->spec == NULL || struct_name == NULL || obj == NULL)
	{
		BZSTRUCT_LOG_ERR("Bad parameter");
		return BZSTRUCT_E_BADPARAM;
	}

	return ctx->spec->to_obj(ctx, struct_name, cstruct, obj);
}

int bzstruct_from_obj(bzstruct_ctx_t *ctx, const char *struct_name, void *cstruct, void *obj)
{
	void *p;

	if(ctx == NULL || ctx->spec == NULL || struct_name == NULL || cstruct == NULL || obj == NULL)
	{
		BZSTRUCT_LOG_ERR("Bad parameter");
		return BZSTRUCT_E_BADPARAM;
	}

	return ctx->spec->from_obj(ctx, struct_name, cstruct, obj);
}

int bzstruct_dump_obj(bzstruct_ctx_t *ctx, const char *struct_name, void *cstruct)
{
	void *p;

	if(ctx == NULL || ctx->spec == NULL || struct_name == NULL)
	{
		BZSTRUCT_LOG_ERR("Bad parameter");
		return BZSTRUCT_E_BADPARAM;
	}

	return ctx->spec->dump_obj(ctx, struct_name, cstruct);
}



void bzstruct_field_destroy(struct bzstruct_field *field)
{
	if(field != NULL)
		free(field);
}

/*get number of element if field is array*/
static int _bzstruct_field_element_count(bzstruct_field_t *field)
{
	switch(field->type)
	{
		case BZSTRUCT_DTYPE_INT8_ARRAY:
		case BZSTRUCT_DTYPE_UINT8_ARRAY:
			return field->size/sizeof(int8_t);
		case BZSTRUCT_DTYPE_INT16_ARRAY:
		case BZSTRUCT_DTYPE_UINT16_ARRAY:
			return field->size/sizeof(int16_t);
		case BZSTRUCT_DTYPE_INT32_ARRAY:
		case BZSTRUCT_DTYPE_UINT32_ARRAY:
			return field->size/sizeof(int32_t);
		case BZSTRUCT_DTYPE_INT64_ARRAY:
		case BZSTRUCT_DTYPE_UINT64_ARRAY:
			return field->size/sizeof(int64_t);
#if 0
		case BZSTRUCT_DTYPE_CSTRUCT_ARRAY:
		{
			int sz = bzstruct_size_get(field->container->ctx, field->type_name);
			return sz == 0 ? 0 : field->size/sz;
		}
#endif
		default:
			break;
	}
	return 1;
}

int bzstruct_field_element_count(bzstruct_field_t *field)
{
	return _bzstruct_field_element_count(field);
}

struct bzstruct_field * bzstruct_field_new(const char *name, int size, int offset, bzstruct_dtype_t type, const char *type_name)
{
	struct bzstruct_field  *p;

	if(name == NULL)
	{
		BZSTRUCT_LOG_ERR("Bad parameter!");
		return NULL;
	}

	p = calloc(sizeof(struct bzstruct_field), 1);
	if(p == NULL)
	{
		BZSTRUCT_LOG_ERR("Could not alloc memory");
		return NULL;
	}

	p->name = name;
	p->size = size;
	p->type = type;
	p->offset = offset;
	p->type_name = type_name;
	
	return p;
}


struct bzstruct_field *bzstruct_register_field(struct bzstruct *bzstruct,
                     const char *field_name, int size, int offset, bzstruct_dtype_t type, const char *type_name)
{
	struct bzstruct_field  *p;

	BZCTRUCT_LOG_DEBUG("Register field: %s->%s, size:%d, offset:%d, dtype:%s (%d), typename=%s", 
		bzstruct->name, field_name, size, offset, DTYPE_STR(type), type, type_name);

	p = bzstruct_field_lookup(bzstruct, field_name);
	if(p != NULL)
	{
		BZSTRUCT_LOG_WARN("Struct field has existed: %s.%s", bzstruct->name, field_name);
		return p;
	}

	p = bzstruct_field_new(field_name, size, offset, type, type_name);
	if(p == NULL)
	{
		BZSTRUCT_LOG_ERR("Could not alloc memory for bzstruct_field");
		return NULL;
	}

	p->container = bzstruct;

	p->elem_count = _bzstruct_field_element_count(p);
	
	TAILQ_INSERT_TAIL(&bzstruct->field, p, entry);
	return p;
}

struct bzstruct_field *bzstruct_field_register(bzstruct_ctx_t *ctx, const char *struct_name, 
							const char *field_name, int size, int offset, bzstruct_dtype_t type, const char *type_name)
{
	struct bzstruct_field  *p;
	struct bzstruct *bzstruct;

	if(ctx == NULL || struct_name == NULL || field_name == NULL)
	{
		BZSTRUCT_LOG_ERR("Bad parameter");
		return NULL;
	}

	bzstruct = bzstruct_lookup(ctx, struct_name);
	if(bzstruct == NULL)
	{
		BZSTRUCT_LOG_ERR("Could not find C struct: %s", struct_name);
		return NULL;
	}
	
	return bzstruct_register_field(bzstruct, field_name, size, offset, type, type_name);
}


struct bzstruct *bzstruct_new(const char *struct_name)
{

	struct bzstruct *p;
	
	p = calloc(sizeof(struct bzstruct), 1);
	if(p == NULL)
	{
		BZSTRUCT_LOG_ERR("Could not alloc memory");
		return NULL;
	}
	
	p->name = struct_name;
	TAILQ_INIT(&p->field);

	return p;
}


void bzstruct_clean(bzstruct_ctx_t *ctx, char *stru_name, void *cstruct)
{
	if(ctx != NULL && stru_name != NULL && cstruct != NULL)
	{
		if(ctx->spec != NULL &&  ctx->spec->clean != NULL)
			ctx->spec->clean(ctx, stru_name, cstruct);
	}
}


void bzstruct_destroy(struct bzstruct *bzstruct)
{
	struct bzstruct_field *field, *n;
	
	if(bzstruct == NULL)
		return;

	if(bzstruct->ref == NULL) /*just destroy fields of non-aliase struct*/
	{
		field = TAILQ_FIRST(&bzstruct->field);
		while(field != NULL)
		{
			n = TAILQ_NEXT(field, entry);
			bzstruct_field_destroy(field);
			field = n;
		}
	}
	free(bzstruct);
}

extern bzstruct_spec_t cjson_cstruct_spec;
bzstruct_ctx_t *bzstruct_ctx_new(char *spec)
{
	bzstruct_ctx_t *ctx;
	int i;

	ctx = calloc(sizeof(bzstruct_ctx_t), 1);
	if(ctx == NULL)
	{
		BZSTRUCT_LOG_ERR("Could not alloc memory!");
		return NULL;
	}

	ctx->spec = &cjson_cstruct_spec;

	for(i = 0; i < BZSTRUCT_HASH_SIZE; i++)
		TAILQ_INIT(&ctx->cstruct[i]);
	return ctx;
}

void bzstruct_ctx_destroy(bzstruct_ctx_t *ctx)
{
	struct bzstruct *p, *n;
	int i;
	
	if(ctx == NULL)
		return;

	for(i = 0; i < BZSTRUCT_HASH_SIZE; i++)
	{
		p = TAILQ_FIRST(&ctx->cstruct[i]);
		while(p != NULL)
		{
			n = TAILQ_NEXT(p, entry);
			bzstruct_destroy(p);
			p = n;
		}
	}
	free(ctx);
}

static int _bzstruct_aliase_register(bzstruct_ctx_t *ctx, struct bzstruct *bzstruct, const char *struct_aliase)
{
	struct bzstruct *aliase;
	uint32_t index;
	
	if(bzstruct_lookup(ctx, struct_aliase) != NULL)
	{
		BZSTRUCT_LOG_ERR("Could struct aliase has been existed");
		return BZSTRUCT_E_EXIST;
	}

	aliase = bzstruct_new(struct_aliase);
	if(aliase == NULL)
	{
		BZSTRUCT_LOG_ERR("Could not alloc bzstruct: %s", struct_aliase);
		return BZSTRUCT_E_NOMEM;
	}
	aliase->ref = bzstruct;
	index = string_hash(struct_aliase) % BZSTRUCT_HASH_SIZE;
	TAILQ_INSERT_TAIL(&ctx->cstruct[index], aliase, entry);
	return 0;
}

static int bzstruct_aliase_register(bzstruct_ctx_t *ctx, const char *struct_name, const char *struct_aliase)
{
	struct bzstruct *bzstruct, *aliase;
	
	if(ctx == NULL || struct_name == NULL || struct_aliase == NULL)
	{
		BZSTRUCT_LOG_ERR("Bad parameter");
		return BZSTRUCT_E_BADPARAM;
	}
	bzstruct = bzstruct_lookup(ctx, struct_name);
	if(bzstruct == NULL)
	{
		BZSTRUCT_LOG_WARN("Could not find C struct: %s", struct_name);
		return BZSTRUCT_E_NOTFOUND;
	}
	return _bzstruct_aliase_register(ctx, bzstruct, struct_aliase);
}

struct bzstruct *bzstruct_register(bzstruct_ctx_t *ctx, const char *struct_name, const char *struct_aliase, int is_union, int size)
{
	struct bzstruct *bzstruct, *aliase;
	uint32_t index;
	int ret;

	if(ctx == NULL || struct_name == NULL)
	{
		BZSTRUCT_LOG_ERR("Bad parameter");
		return NULL;
	}

	BZCTRUCT_LOG_DEBUG("Register struct:%s, alias:%s, is_union:%d, size:%d", struct_name, 
		struct_aliase != NULL ? struct_aliase: "NULL", is_union, size);

	bzstruct = bzstruct_lookup(ctx, struct_name);
	if(bzstruct != NULL)
	{
		BZSTRUCT_LOG_WARN("C struct has existed: %s", struct_name);
		return NULL;
	}
	
	bzstruct = bzstruct_new(struct_name);
	if(bzstruct == NULL)
	{
		BZSTRUCT_LOG_ERR("Could not alloc bzstruct: %s", struct_name);
		return NULL;
	}
	bzstruct->ctx = ctx;
	bzstruct->size = size;
	bzstruct->aliase = struct_aliase;
	bzstruct->is_union = is_union;
	index = string_hash(struct_name) % BZSTRUCT_HASH_SIZE;
	TAILQ_INSERT_TAIL(&ctx->cstruct[index], bzstruct, entry);

	if(struct_aliase != NULL)
	{
		ret = _bzstruct_aliase_register(ctx, bzstruct, struct_aliase);
		if(ret)
		{
			BZSTRUCT_LOG_ERR("Could not register struct aliase: %s, ret=%d", struct_aliase, ret);
			bzstruct_destroy(bzstruct);
			return NULL;
		}
	}

	return bzstruct;
}




BZSTRUCT_CTX_INIT(BZSTRUCT_DEF_CTX, NULL)


#if 0
__attribute__((constructor(101))) void bzstruct_ctx_init(void)
{
	char *spec = getenv((char *)"BZSTRUCT_SPEC");
	bzctx = bzstruct_ctx_new(spec);
	if(bzctx == NULL)
	{
		BZSTRUCT_LOG_ERR("Could not init bzctx\n");
	}
}


bzstruct_ctx_t *bzstruct_ctx_get(void)
{
	return bzctx;
}
#endif



