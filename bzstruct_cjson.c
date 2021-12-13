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
#include "cJSON.h"


#define CJSON_FOREACH(cjson, item) \
	for(item = (cjson != NULL) ? (cjson)->child : NULL; item != NULL; item = item->next)


static int struct_to_cjson(bzstruct_ctx_t *ctx, const char *struct_name, void *cstruct, void **cjson);
static int struct_from_cjson(bzstruct_ctx_t *ctx, const char *struct_name, void *cstruct, void *cjson);
static int _struct_from_cjson(bzstruct_ctx_t *ctx, struct bzstruct *bzstruct, void *cstruct, void *cjson);
static int _struct_to_cjson(bzstruct_ctx_t *ctx, struct bzstruct *bzstruct, void *cstruct, void **cjson);


static int mumber_arry_from_cjson(bzstruct_ctx_t *ctx, struct bzstruct_field *field, void *cstruct, void *cjson)
{
	cJSON *item, *p;
	void *d;
	int count;
	
	d = CSTRUCT_OFFSET_ADDR(void *, cstruct, field->offset);
	p = (cJSON *)cjson;
	count = field->elem_count;

#define _XXX(T) \
	*((T *)d) = item->valuedouble; \
	d = ((T *)d) + 1; 

	CJSON_FOREACH(p, item)
	{
		if(count-- < 1)
		{
			BZSTRUCT_LOG_WARN("Too many element of jason array: field=%s, type=%s(%d)", field->type_name, \
					DTYPE_STR(field->type), field->type);
			return BZSTRUCT_E_OVERLOAD;
		}
		switch(field->type)
		{
			case BZSTRUCT_DTYPE_INT8_ARRAY:
			case BZSTRUCT_DTYPE_UINT8_ARRAY:
				_XXX(int8_t)
				break;
			case BZSTRUCT_DTYPE_INT16_ARRAY:
			case BZSTRUCT_DTYPE_UINT16_ARRAY:
				_XXX(int16_t)
				break;
			case BZSTRUCT_DTYPE_INT32_ARRAY:
			case BZSTRUCT_DTYPE_UINT32_ARRAY:
				_XXX(int32_t)
				break;
			case BZSTRUCT_DTYPE_INT64_ARRAY:
			case BZSTRUCT_DTYPE_UINT64_ARRAY:
				_XXX(int64_t)
				break;
			default:
				BZSTRUCT_LOG_WARN("Unknow number type: field=%s, type=%s(%d)", field->type_name, \
					DTYPE_STR(field->type), field->type);
				return BZSTRUCT_E_INVALID;
		}
	}
#undef _XXX	
	return 0;
}


static int number_array_to_cjson(bzstruct_ctx_t *ctx, struct bzstruct_field *field, void *cstruct, void **cjson)
{
	cJSON *item, *p;
	void *d;
	int count;

	count = field->elem_count;
	p = cJSON_CreateArray();
	if(p == NULL)
	{
		BZSTRUCT_LOG_ERR("Could not create JSON number array");
		return BZSTRUCT_E_NOMEM;
	}

	d = CSTRUCT_OFFSET_ADDR(void *, cstruct, field->offset);

#define _XXX(T) \
		item = cJSON_CreateNumber(*((T *)d)); \
		if(item == NULL){ \
			BZSTRUCT_LOG_ERR("Could not create JSON number"); \
			cJSON_Delete(p); \
			return BZSTRUCT_E_NOMEM;\
		}\
		cJSON_AddItemToArray(p, item); \
		d = ((T *)d) + 1; 

	while(count--)
	{			
		switch(field->type)
		{
			case BZSTRUCT_DTYPE_INT8_ARRAY:
			case BZSTRUCT_DTYPE_UINT8_ARRAY:
				_XXX(int8_t)
				break;
			case BZSTRUCT_DTYPE_INT16_ARRAY:
			case BZSTRUCT_DTYPE_UINT16_ARRAY:
				_XXX(int16_t)
				break;
			case BZSTRUCT_DTYPE_INT32_ARRAY:
			case BZSTRUCT_DTYPE_UINT32_ARRAY:
				_XXX(int32_t)
				break;
			case BZSTRUCT_DTYPE_INT64_ARRAY:
			case BZSTRUCT_DTYPE_UINT64_ARRAY:
				_XXX(int64_t)
				break;
			default:
				BZSTRUCT_LOG_WARN("Unknow number type: field=%s, type=%s(%d)", field->type_name, \
					DTYPE_STR(field->type), field->type);
				cJSON_Delete(p); 
				return BZSTRUCT_E_INVALID;
		}
	}

#undef 	_XXX

	*cjson = p;
	return 0;
}

static int number_to_cjson(bzstruct_ctx_t *ctx, struct bzstruct_field *field, void *cstruct, void **cjson)
{
	cJSON *item;
	double d;

	switch(field->type)
	{
		case BZSTRUCT_DTYPE_CHAR:
		case BZSTRUCT_DTYPE_INT8:
		case BZSTRUCT_DTYPE_UINT8:
			d = *CSTRUCT_OFFSET_ADDR(int8_t *, cstruct, field->offset);
			break;
		case BZSTRUCT_DTYPE_INT16:
		case BZSTRUCT_DTYPE_UINT16:
			d = *CSTRUCT_OFFSET_ADDR(int16_t *, cstruct, field->offset);
			break;
		case BZSTRUCT_DTYPE_INT32:
		case BZSTRUCT_DTYPE_UINT32:
			d = *CSTRUCT_OFFSET_ADDR(int32_t *, cstruct, field->offset);
			break;
		case BZSTRUCT_DTYPE_INT64:
		case BZSTRUCT_DTYPE_UINT64:
			d = *CSTRUCT_OFFSET_ADDR(int64_t *, cstruct, field->offset);
			break;
		default:
			BZSTRUCT_LOG_WARN("Unknow number type: field=%s, type=%s(%d)", field->type_name, \
					DTYPE_STR(field->type), field->type);
			return 0;
	}

	item = cJSON_CreateNumber(d);
	if(item == NULL)
	{
		BZSTRUCT_LOG_ERR("Could not create JSON number object");
		return BZSTRUCT_E_NOMEM;
	}
	*cjson = item;
	return 0;
}

static int mumber_from_cjson(bzstruct_ctx_t *ctx, struct bzstruct_field *field, void *cstruct, void *cjson)
{
	void *d; 
	cJSON *item;

	item = (cJSON *)cjson;
	if(item->type != cJSON_Number)
	{
		BZSTRUCT_LOG_ERR("Bad type of JSON object: name=%s, type=%d", field->name, item->type);
		return BZSTRUCT_E_BADFORMAT;
	}

	d = CSTRUCT_OFFSET_ADDR(void *, cstruct, field->offset);

	switch(field->type)
	{
		case BZSTRUCT_DTYPE_CHAR:
		case BZSTRUCT_DTYPE_INT8:
		case BZSTRUCT_DTYPE_UINT8:
			*((int8_t *)d) = item->valuedouble;
			break;
		case BZSTRUCT_DTYPE_INT16:
		case BZSTRUCT_DTYPE_UINT16:
			*((int16_t *)d) = item->valuedouble;
			break;
		case BZSTRUCT_DTYPE_INT32:
		case BZSTRUCT_DTYPE_UINT32:
			*((int32_t *)d) = item->valuedouble;
			break;
		case BZSTRUCT_DTYPE_INT64:
		case BZSTRUCT_DTYPE_UINT64:
			*((int64_t *)d) = item->valuedouble;
			break;
		default:
			BZSTRUCT_LOG_WARN("Unknow number type: field=%s, type=%s(%d)", field->type_name, \
					DTYPE_STR(field->type), field->type);
			return BZSTRUCT_E_INVALID;;
	}	
	return 0;
}

/*char * and char array*/
static int string_to_cjson(bzstruct_ctx_t *ctx, struct bzstruct_field *field, void *cstruct, void **cjson)
{
	cJSON *item;
	char *s, *p;
	
	s  = CSTRUCT_OFFSET_ADDR(char *, cstruct, field->offset);
	if(field->type == BZSTRUCT_DTYPE_CHAR_ARRAY)
		p = s;
	else
	{
		p = *((char **)s);
		if(p == NULL)
		{
			*cjson = NULL;
			return BZSTRUCT_E_NONE;
		}
	}
		
	item = cJSON_CreateString(p);
	if(item == NULL)
	{
		BZSTRUCT_LOG_ERR("Could not create JSON string object");
		return BZSTRUCT_E_NOMEM;
	}
	*cjson = item;
	return 0;
}

/*char * and char array*/
static int string_from_cjson(bzstruct_ctx_t *ctx, struct bzstruct_field *field, void *cstruct, void *cjson)
{
	cJSON *item;
	char *s, **p;
	
	s  = CSTRUCT_OFFSET_ADDR(char *, cstruct, field->offset);
	item = (cJSON *)cjson;
	
	if(item->type != cJSON_String)
	{
		BZSTRUCT_LOG_ERR("Bad type of JSON object: name=%s, type=%d", field->name, item->type);
		return BZSTRUCT_E_BADFORMAT;
	}

	if(field->type == BZSTRUCT_DTYPE_CHAR_ARRAY)
	{
		strncpy(s, item->valuestring, field->size);
	}
	else
	{
		p = (char **)s;
		*p = strdup(item->valuestring);
		if(p == NULL)
		{
			BZSTRUCT_LOG_ERR("Could not alloc memory");
			return BZSTRUCT_E_NOMEM;
		}
	}
	
	return 0;
}

static int struct_field_array_from_cjson(bzstruct_ctx_t *ctx, struct bzstruct_field *field, void *cstruct, void *cjson)
{
	void *addr;
	int count;
	int size;
	bzstruct_t *bzstru;
	cJSON *root, *item;
	int ret;

	bzstru = bzstruct_lookup(ctx, field->type_name);
	if(bzstru == NULL)
	{
		BZSTRUCT_LOG_ERR("Could not find field struct, struct=%s, field=%s", field->type_name, field->name);
		return BZSTRUCT_E_NOTFOUND;
	}
	size = bzstru->size;
	if(size <= 0)
	{
		BZSTRUCT_LOG_ERR("struct size is invalid, size:%d", size);
		return BZSTRUCT_E_BADFORMAT;
	}
	count = field->size/size;
	if(count < 1)
	{
		BZSTRUCT_LOG_ERR("Bad field size, struct size:%d, field_size:%d", size, field->size);
		return BZSTRUCT_E_BADFORMAT;
	}
	root = (cJSON *)cjson;
	addr = CSTRUCT_OFFSET_ADDR(void *, cstruct, field->offset);
	CJSON_FOREACH(root, item)
	{
		if(count-- < 1)
		{
			BZSTRUCT_LOG_WARN("Too many element of jason array: field=%s, type=%s(%d)", field->type_name, \
					DTYPE_STR(field->type), field->type);
			return BZSTRUCT_E_OVERLOAD;
		}
		ret = _struct_from_cjson(ctx, bzstru, addr, item);
		if(ret)
		{
			BZSTRUCT_LOG_ERR("Could not covert cJSON to struct, ret=%d", ret);
			return ret;
		}
		addr = ((char *)addr) + size;
	}
	return 0;
}


static int struct_field_array_to_cjson(bzstruct_ctx_t *ctx, struct bzstruct_field *field, void *cstruct, void **cjson)
{
	void *addr;
	int count;
	int size;
	bzstruct_t *bzstru;
	cJSON *root, *item;
	int ret;

	bzstru = bzstruct_lookup(ctx, field->type_name);
	if(bzstru == NULL)
	{
		BZSTRUCT_LOG_ERR("Could not find field struct, struct=%s, field=%s", field->type_name, field->name);
		return BZSTRUCT_E_NOTFOUND;
	}
	size = bzstru->size;
	if(size <= 0)
	{
		BZSTRUCT_LOG_ERR("struct size is invalid, size:%d", size);
		return BZSTRUCT_E_BADFORMAT;
	}
	count = field->size/size;
	if(count < 1)
	{
		BZSTRUCT_LOG_ERR("Bad field size, struct size:%d, field_size:%d", size, field->size);
		return BZSTRUCT_E_BADFORMAT;
	}

	root = cJSON_CreateArray();
	if(root == NULL)
	{
		BZSTRUCT_LOG_ERR("Could not create JSON object");
		return BZSTRUCT_E_NOMEM;
	}
	addr = CSTRUCT_OFFSET_ADDR(void *, cstruct, field->offset);
	while(count--)
	{
		ret = _struct_to_cjson(ctx, bzstru, addr, (void **)&item);
		if(ret)
		{
			cJSON_Delete(root); 
			BZSTRUCT_LOG_ERR("Could not covert struct to cJSON, ret=%d", ret);
			return ret;
		}
		cJSON_AddItemToArray(root, item);
		addr = ((char *)addr) + size;
	}
	*cjson = root;
	return 0;
}


static int struct_field_to_cjson(bzstruct_ctx_t *ctx, struct bzstruct_field *field, void *cstruct, void **cjson)
{
	void *addr;
	addr = CSTRUCT_OFFSET_ADDR(void *, cstruct, field->offset);
	return struct_to_cjson(ctx, field->type_name, addr, cjson);
}

static int struct_field_from_cjson(bzstruct_ctx_t *ctx, struct bzstruct_field *field, void *cstruct, void *cjson)
{
	void *addr;

	addr = CSTRUCT_OFFSET_ADDR(void *, cstruct, field->offset);
	return struct_from_cjson(ctx, field->type_name, addr, cjson);
}




bzcstruct_dtype_ops_t json_dtype_ops[BZSTRUCT_DTYPE_COUNT] = {
	[BZSTRUCT_DTYPE_CHAR] = {number_to_cjson, mumber_from_cjson},
	[BZSTRUCT_DTYPE_STRING] = {string_to_cjson, string_from_cjson},
	[BZSTRUCT_DTYPE_CHAR_ARRAY] = {string_to_cjson, string_from_cjson},
	[BZSTRUCT_DTYPE_INT8_ARRAY] = {number_array_to_cjson, mumber_arry_from_cjson},
	[BZSTRUCT_DTYPE_INT8] = {number_to_cjson, mumber_from_cjson},
	[BZSTRUCT_DTYPE_UINT8_ARRAY] = {number_array_to_cjson, mumber_arry_from_cjson},
	[BZSTRUCT_DTYPE_UINT8] = {number_to_cjson, mumber_from_cjson},
	[BZSTRUCT_DTYPE_INT16_ARRAY] = {number_array_to_cjson, mumber_arry_from_cjson},
	[BZSTRUCT_DTYPE_INT16] = {number_to_cjson, mumber_from_cjson},
	[BZSTRUCT_DTYPE_UINT16_ARRAY] = {number_array_to_cjson, mumber_arry_from_cjson},
	[BZSTRUCT_DTYPE_UINT16] = {number_to_cjson, mumber_from_cjson},
	[BZSTRUCT_DTYPE_INT32_ARRAY] = {number_array_to_cjson, mumber_arry_from_cjson},
	[BZSTRUCT_DTYPE_INT32] = {number_to_cjson, mumber_from_cjson},
	[BZSTRUCT_DTYPE_UINT32_ARRAY] = {number_array_to_cjson, mumber_arry_from_cjson},
	[BZSTRUCT_DTYPE_UINT32] = {number_to_cjson, mumber_from_cjson},
	[BZSTRUCT_DTYPE_INT64_ARRAY] = {number_array_to_cjson, mumber_arry_from_cjson},
	[BZSTRUCT_DTYPE_INT64] = {number_to_cjson, mumber_from_cjson},
	[BZSTRUCT_DTYPE_UINT64_ARRAY] = {number_array_to_cjson, mumber_arry_from_cjson},
	[BZSTRUCT_DTYPE_UINT64] = {number_to_cjson, mumber_from_cjson},
	[BZSTRUCT_DTYPE_CSTRUCT] = {struct_field_to_cjson, struct_field_from_cjson},
	[BZSTRUCT_DTYPE_UNION] = {struct_field_to_cjson, struct_field_from_cjson},
	[BZSTRUCT_DTYPE_CSTRUCT_ARRAY] = {struct_field_array_to_cjson, struct_field_array_from_cjson},
};


static int union_to_cjson(bzstruct_ctx_t *ctx, struct bzstruct *bzstruct, void *cstruct, void **cjson)
{
	int ret;
	struct bzstruct_field *field;
	bzstruct_field_to_t *to;
	cJSON *root, *item;

	field = bzstruct_maxsize_field_lookup(bzstruct);
	if(field == NULL)
	{
		BZSTRUCT_LOG_WARN("Could not find max size field: %s：", bzstruct->name); 
		return BZSTRUCT_E_INVALID;
	}
	to = json_dtype_ops[field->type].to;
	if(to == NULL)
	{
		BZSTRUCT_LOG_WARN("Invalid  data type: field=%s, type=%d", field->name, field->type); 
		return BZSTRUCT_E_INVALID;
	}

	root = cJSON_CreateObject();
	if(root == NULL)
	{
		BZSTRUCT_LOG_ERR("Could not create JSON object");
		return BZSTRUCT_E_NOMEM;
	}

	ret = to(ctx, field, cstruct, (void **)&item);
	if(ret)
	{
		BZSTRUCT_LOG_WARN("Failed to convert field to cjson field: %s", field->name); 
		cJSON_Delete(root);
		return BZSTRUCT_E_BADFORMAT;
	}
	cJSON_AddItemToObject(root, field->name, item);
	*cjson = root;
	return 0;
	
}

static int _struct_to_cjson(bzstruct_ctx_t *ctx, struct bzstruct *bzstruct, void *cstruct, void **cjson)
{
	int ret;
	struct bzstruct_field *field;
	bzstruct_field_to_t *to;
	cJSON *root, *item;

	if(bzstruct->is_union)
		return union_to_cjson(ctx, bzstruct, cstruct, cjson);

	root = cJSON_CreateObject();
	if(root == NULL)
	{
		BZSTRUCT_LOG_ERR("Could not create JSON object");
		return BZSTRUCT_E_NOMEM;
	}

	TAILQ_FOREACH(field, &bzstruct->field, entry)
	{
		to = json_dtype_ops[field->type].to;
		if(to == NULL)
		{
			BZSTRUCT_LOG_WARN("Invalid  data type: field=%s, type=%d", field->name, field->type); 
			continue;
		}
		ret = to(ctx, field, cstruct, (void **)&item);
		if(ret)
		{
			BZSTRUCT_LOG_WARN("Failed to convert field to cjson field: %s", field->name); 
			continue;
		}
		if(item != NULL)
			cJSON_AddItemToObject(root, field->name, item);
	}
	*cjson = root;
	return 0;
}

static int struct_to_cjson(bzstruct_ctx_t *ctx, const char *struct_name, void *cstruct, void **cjson)
{
	struct bzstruct *bzstruct;

	bzstruct = bzstruct_lookup(ctx, struct_name);
	if(bzstruct == NULL)
	{
		BZSTRUCT_LOG_ERR("Could not find cstruct: %s", struct_name); 
		return BZSTRUCT_E_INVALID;
	}

	return _struct_to_cjson(ctx, bzstruct, cstruct, cjson);

}

#if 0
static int union_from_cjson(bzstruct_ctx_t *ctx, struct bzstruct *bzstruct, void *cstruct, cJSON *cjson)
{
	struct bzstruct_field *field;
	bzstruct_field_from_t *from;
	int ret;

	field = bzstruct_maxsize_field_lookup(bzstruct);
	if(field == NULL)
	{
		BZSTRUCT_LOG_WARN("Could not find max size field: %s：", bzstruct->name); 
		return BZSTRUCT_E_INVALID;
	}
	from = json_dtype_ops[field->type].from;
	if(from == NULL)
	{
		BZSTRUCT_LOG_WARN("Invalid  data type: field=%s, type=%d", field->name, field->type); 
		return BZSTRUCT_E_INVALID;
	}
	ret = from(ctx, field, cstruct, cjson);
	if(ret)
	{
		BZSTRUCT_LOG_WARN("Faied to covert cJSON to field: %s(type=%d)", field->name, field->type); 
		return BZSTRUCT_E_BADFORMAT;
	}
	
	return 0;
}
#endif

static int _struct_from_cjson(bzstruct_ctx_t *ctx, struct bzstruct *bzstruct, void *cstruct, void *cjson)
{
	int ret;
	struct bzstruct_field *field;
	bzstruct_field_from_t *from;
	cJSON *root, *item;

	root = (cJSON *)cjson;

	CJSON_FOREACH(root, item)
	{
		field = bzstruct_field_lookup(bzstruct, item->string);
		if(field == NULL)
		{
			BZSTRUCT_LOG_WARN("Could not find field: struct=%s, field=%s", bzstruct->name, item->string);
			continue;
		}
		from = json_dtype_ops[field->type].from;
		if(from == NULL)
		{
			BZSTRUCT_LOG_WARN("Invalid  data type: field=%s, type=%d", field->name, field->type); 
			continue;
		}
		ret = from(ctx, field, cstruct, item);
		if(ret)
		{
			BZSTRUCT_LOG_WARN("Faied to covert cJSON to field: %s(type=%d)", field->name, field->type); 
			continue;
		}
	}
	return 0;
}

static int struct_from_cjson(bzstruct_ctx_t *ctx, const char *struct_name, void *cstruct, void *cjson)
{
	struct bzstruct *bzstruct;

	bzstruct = bzstruct_lookup(ctx, struct_name);
	if(bzstruct == NULL)
	{
		BZSTRUCT_LOG_WARN("Could not find struct: %s", struct_name);
		return BZSTRUCT_E_INVALID;
	}
	return _struct_from_cjson(ctx, bzstruct, cstruct, cjson);
}


static int struct_decode_json(bzstruct_ctx_t *ctx, const char *struct_name, void *cstruct, uint8_t *buf, int len)
{
	int ret;
	cJSON *root;

	root = cJSON_Parse(buf);
	if(root == NULL)
	{
		BZSTRUCT_LOG_ERR("Bad JSON format");
		return BZSTRUCT_E_BADFORMAT;
	}

	ret = struct_from_cjson(ctx, struct_name, cstruct, root);
	if(ret)
		BZSTRUCT_LOG_ERR("Could not covert cJSON to struct");

	cJSON_Delete(root);
	return 0;
}

static int struct_encode_json(bzstruct_ctx_t *ctx, const char *struct_name, void *cstruct, uint8_t **buf, int *len)
{
	char *p;
	int ret;
	cJSON *root;

	ret = struct_to_cjson(ctx, struct_name, cstruct, (void **)&root);
	if(ret)
	{
		BZSTRUCT_LOG_ERR("Could not covert struct to cJSON");
		return ret;
	}

	p = cJSON_PrintUnformatted(root);
	cJSON_Delete(root);
	if(p == NULL)
	{
		BZSTRUCT_LOG_ERR("Could not covert cJSON to string");
		return BZSTRUCT_E_NOMEM;
	}

	*buf = p;
	if(len != NULL)
		*len = strlen(p);
	
	return 0;
}

static int struct_dump_json(bzstruct_ctx_t *ctx, const char *struct_name, void *pstruct)
{
	uint8_t *s;
	int ret;

	ret = struct_encode_json(ctx, struct_name, pstruct, &s, NULL);
	if(ret)
	{
		BZSTRUCT_LOG_ERR("Could not encode struct to JSON, ret=%d\n", ret);
		return ret;
	}
	if(s != NULL)
	{
		printf("%s\n", s);
		free(s);
	}
	return 0;
	
}


static void _struct_clean(bzstruct_ctx_t *ctx, bzstruct_t *bzstru, void *pstruct)
{
	bzstruct_field_t *field;
	void *addr;
	bzstruct_t *stru;

	TAILQ_FOREACH(field, &bzstru->field, entry)
	{
		addr = CSTRUCT_OFFSET_ADDR(void *, pstruct, field->offset);
		switch(field->type)
		{
			case BZSTRUCT_DTYPE_STRING:
				if(*((char **)addr) != NULL)
				{
					free(*((char **)addr));
					*((char **)addr) = NULL;
				}
				break;
			case BZSTRUCT_DTYPE_CSTRUCT:
			case BZSTRUCT_DTYPE_CSTRUCT_ARRAY:
				{
					int count;
					stru = bzstruct_lookup(ctx, field->type_name);
					if(stru == NULL)
						break;
					count = field->size / stru->size;

					while(count--)
					{
						_struct_clean(ctx, stru, addr);
						addr = ((char *)(addr)) + stru->size;
					}
				}
				break;
			default:
				break;
		}
	}
	
}

static void struct_clean(bzstruct_ctx_t *ctx, const char *struct_name, void *pstruct)
{
	bzstruct_t *bzstru = bzstruct_lookup(ctx, struct_name);
	if(bzstru == NULL)
		return;
	_struct_clean(ctx, bzstru, pstruct);
}


bzstruct_spec_t cjson_cstruct_spec = {
	.name = "cjson",
	.clean = struct_clean,
	.dump_obj = struct_dump_json,
	.to_obj = struct_to_cjson,
	.from_obj = struct_from_cjson,
	.encode = struct_encode_json,
	.decode = struct_decode_json,
};


