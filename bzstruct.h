#ifndef _BZSTRUCT_H_
#define _BZSTRUCT_H_

#include <stdint.h>
#include <stddef.h>
#include <sys/queue.h>


#define BZSTRUCT  1
#define BZSTRUCT_VERSION "0.1"


struct bzstruct_spec;
struct bzstruct;

#define BZSTRUCT_HASH_SIZE  256
struct bzstruct_ctx{
	TAILQ_HEAD(, bzstruct) cstruct[BZSTRUCT_HASH_SIZE];
	struct bzstruct_spec *spec;
};
typedef struct bzstruct_ctx  bzstruct_ctx_t;

 struct bzstruct_spec{
 	const char *name;
	void (*clean)(bzstruct_ctx_t *ctx, const char *struct_name, void *pstruct);
	int (*dump_obj)(bzstruct_ctx_t *ctx, const char *struct_name, void *pstruct);
	int  (*to_obj)(bzstruct_ctx_t *ctx, const char *struct_name, void *pstruct, void **obj);
	int (*from_obj)(bzstruct_ctx_t *ctx, const char *struct_name, void*pstruct, void *obj);
	int (*encode)(bzstruct_ctx_t *ctx, const char *struct_name,  void *pstruct, uint8_t **buf, int *len);
	int (*decode)(bzstruct_ctx_t *ctx, const char *struct_name, void *pstruct, uint8_t *buf, int len);
};
typedef struct bzstruct_spec bzstruct_spec_t;



enum bzstruct_errno{
	 BZSTRUCT_E_FAIL =  -1,
	 BZSTRUCT_E_NONE,
	 BZSTRUCT_E_INVALID,
	 BZSTRUCT_E_NOMEM,
	 BZSTRUCT_E_BADPARAM,
	 BZSTRUCT_E_BADFORMAT,
	 BZSTRUCT_E_OVERLOAD,
	 BZSTRUCT_E_NOTFOUND,
	 BZSTRUCT_E_EXIST,
};

/*data type*/
enum bzstruct_dtype{
	BZSTRUCT_DTYPE_UNKNOWN,
	BZSTRUCT_DTYPE_CHAR,	
	BZSTRUCT_DTYPE_STRING,
	BZSTRUCT_DTYPE_CHAR_ARRAY,
	BZSTRUCT_DTYPE_INT8,
	BZSTRUCT_DTYPE_INT8_ARRAY,
	BZSTRUCT_DTYPE_INT8_POINT,
	BZSTRUCT_DTYPE_UINT8,
	BZSTRUCT_DTYPE_UINT8_ARRAY,
	BZSTRUCT_DTYPE_UINT8_POINT,
	BZSTRUCT_DTYPE_INT16,
	BZSTRUCT_DTYPE_INT16_ARRAY,
	BZSTRUCT_DTYPE_INT16_POINT,
	BZSTRUCT_DTYPE_UINT16,
	BZSTRUCT_DTYPE_UINT16_ARRAY,
	BZSTRUCT_DTYPE_UINT16_POINT,
	BZSTRUCT_DTYPE_INT32,
	BZSTRUCT_DTYPE_INT32_ARRAY,
	BZSTRUCT_DTYPE_INT32_POINT,
	BZSTRUCT_DTYPE_UINT32,
	BZSTRUCT_DTYPE_UINT32_ARRAY,
	BZSTRUCT_DTYPE_UINT32_POINT,

	BZSTRUCT_DTYPE_INT64,
	BZSTRUCT_DTYPE_INT64_ARRAY,
	BZSTRUCT_DTYPE_INT64_POINT,
	BZSTRUCT_DTYPE_UINT64,
	BZSTRUCT_DTYPE_UINT64_ARRAY,
	BZSTRUCT_DTYPE_UINT64_POINT,
	
	BZSTRUCT_DTYPE_CSTRUCT,
	BZSTRUCT_DTYPE_CSTRUCT_ARRAY,
	BZSTRUCT_DTYPE_CSTRUCT_POINT,

	BZSTRUCT_DTYPE_UNION,
	
	BZSTRUCT_DTYPE_COUNT,
};
typedef enum bzstruct_dtype bzstruct_dtype_t;

const char *bzstruct_dtype_string(bzstruct_dtype_t type);
#define DTYPE_STR(t) bzstruct_dtype_string(t)



struct bzstruct{
	const char       *name;
	const char       *aliase;
	int              is_union;
	int              size;
	struct bzstruct  *ref;   
	bzstruct_ctx_t  *ctx;
	TAILQ_HEAD(, bzstruct_field) field;
	TAILQ_ENTRY(bzstruct) entry;
};

typedef struct bzstruct bzstruct_t;


struct bzstruct_field{
	const char                   *name;
	int                          size;
	int                          elem_count; /*number of elememnt if dtype is an array*/
	int                          offset;
	bzstruct_dtype_t             type;
	const char                   *type_name;
	bzstruct_t                   *container;
	TAILQ_ENTRY(bzstruct_field)  entry;
};
typedef struct bzstruct_field bzstruct_field_t;



typedef int bzstruct_field_to_t(bzstruct_ctx_t *ctx, struct bzstruct_field *field, void *cstruct, void **data);
typedef int bzstruct_field_from_t(bzstruct_ctx_t *ctx, struct bzstruct_field *field, void *cstruct, void *data);


struct bzcstruct_dtype_ops{
	bzstruct_field_to_t *to;
	bzstruct_field_from_t *from;
};
typedef struct bzcstruct_dtype_ops bzcstruct_dtype_ops_t;


#define BZSTRUCT_LOG_INFO(fmt, ...)        printf("[INFO] "fmt" @[%s:%d]\n", ##__VA_ARGS__, __func__, __LINE__)
#define BZSTRUCT_LOG_ERR(fmt, ...)         printf("[ERR ] "fmt" @[%s:%d]\n", ##__VA_ARGS__, __func__, __LINE__)
#define BZSTRUCT_LOG_WARN(fmt, ...)        printf("[WARN] "fmt" @[%s:%d]\n", ##__VA_ARGS__, __func__, __LINE__)
#define BZCTRUCT_LOG_DEBUG(fmt, ...)       printf("[DBG ] "fmt" @[%s:%d]\n", ##__VA_ARGS__, __func__, __LINE__)



void bzstruct_clean(bzstruct_ctx_t *ctx, char *stru_name, void *cstruct);
bzstruct_ctx_t *bzstruct_ctx_new(char *spec);
struct bzstruct *bzstruct_register(bzstruct_ctx_t *ctx, const char *struct_name, 
				const char *struct_aliase, int is_union, int size);
struct bzstruct *bzstruct_lookup(bzstruct_ctx_t *ctx, const char *name);
struct bzstruct_field *bzstruct_register_field(struct bzstruct *bzstruct,
                     const char *field_name, int size, int offset, bzstruct_dtype_t type, const char *type_name);
struct bzstruct_field *bzstruct_field_lookup(struct bzstruct *bzstruct, const char *name);
struct bzstruct_field *bzstruct_maxsize_field_lookup(struct bzstruct *bzstruct);
int bzstruct_field_element_count(bzstruct_field_t *field);


void bzstruct_destroy(struct bzstruct *bzstruct);
int bzstruct_dump_obj(bzstruct_ctx_t *ctx, const char *struct_name, void *cstruct);

int bzstruct_decode(bzstruct_ctx_t *ctx, const char *struct_name, void *cstruct, uint8_t *buf, int len);
int bzstruct_encode(bzstruct_ctx_t *ctx, const char *struct_name, void *cstruct, uint8_t **buf, int *len);
int bzstruct_to_obj(bzstruct_ctx_t *ctx, const char *struct_name, void *cstruct, void **obj);
int bzstruct_from_obj(bzstruct_ctx_t *ctx, const char *struct_name, void *cstruct, void *obj);

bzstruct_ctx_t *bzstruct_ctx_get(void);

extern bzstruct_ctx_t *bzstruct_def_ctx;
#define BZSTRUCT_DEF_CTX    bzstruct_def_ctx 



#include "bzstruct_macro.h"


#define bzstruct_dump(ctx, stru_name, cstruct) \
	bzstruct_dump_obj(ctx, stru_name, cstruct)



#define BZSTRUCT_DUMP_OBJ(type, pstruct) \
	bzstruct_dump(BZSTRUCT_DEF_CTX, #type, pstruct)


#define _BZSTRUCT_DUMP(ctx, type, pstruct) \
	bzstruct_dump(ctx, #type, pstruct)
	
#define BZSTRUCT_DUMP(type, pstruct) \
	_BZSTRUCT_DUMP(BZSTRUCT_DEF_CTX, type, pstruct)

#define _BZSTRUCT_DECODE(ctx, type, pstruct, data, len) \
	bzstruct_decode(ctx, #type, pstruct, data, len)
	
#define BZSTRUCT_DECODE(type, pstruct, data, len) \
	_BZSTRUCT_DECODE(BZSTRUCT_DEF_CTX, type, pstruct, data, len)


#define _BZSTRUCT_ENCODE(ctx, type, pstruct, data, len) \
	bzstruct_encode(ctx, #type, pstruct, data, len)
	
#define BZSTRUCT_ENCODE(type, pstruct, data, len) \
	_BZSTRUCT_ENCODE(BZSTRUCT_DEF_CTX, type, pstruct, data, len)



#define _BZSTRUCT_TO_OBJ(ctx, type, pstruct) \
	({ \
		void *__obj = NULL; \
		bzstruct_to_obj(ctx, #type, pstruct, &__obj) ? NULL : __obj; \
	})

#define BZSTRUCT_TO_OBJ(type, pstruct) \
	_BZSTRUCT_TO_OBJ(BZSTRUCT_DEF_CTX, type, pstruct)

#define _BZSTRUCT_FROM_OBJ(ctx, type, pstruct, obj) \
	bzstruct_from_obj(ctx, #type, pstruct, obj)

#define BZSTRUCT_FROM_OBJ(type, pstruct, obj) \
		_BZSTRUCT_FROM_OBJ(BZSTRUCT_DEF_CTX, type, pstruct, obj)





#define __GEN_BZSTRUCT_REGISTER_FUNC_(x) __register_bzstruct_##x 
#define _GEN_BZSTRUCT_REGISTER_FUNC(x) __GEN_BZSTRUCT_REGISTER_FUNC_(x)
#define _BZSTRUCT_REGISTER_FUNC _GEN_BZSTRUCT_REGISTER_FUNC(__COUNTER__)

#define _BZSTRUCT_REGISTER(_ctx, type, ...) \
	__attribute__((constructor(102))) static void _BZSTRUCT_REGISTER_FUNC(void) \
	{\
		REGISTER_CSTRUCT(_ctx, type, __VA_ARGS__); \
	}


#define BZSTRUCT_REGISTER(type, ...) _BZSTRUCT_REGISTER(BZSTRUCT_DEF_CTX, type, __VA_ARGS__)
#if 0
	__attribute__((constructor(102))) static void BZSTRUCT_REGISTER_FUNC(void) \
	{\
		bzstruct_ctx_t *ctx = bzstruct_ctx_get(); \
		if(ctx == NULL) return; \
		REGISTER_CSTRUCT(ctx, type, __VA_ARGS__); \
	}
#endif


#define BZSTRUCT_CTX_INIT(_ctx, _spec) \
	bzstruct_ctx_t *_ctx; \
__attribute__((constructor(101))) void bzstruct_ctx_init_##_ctx(void) \
{ \
	_ctx = bzstruct_ctx_new(_spec); \
	if(_ctx == NULL) \
		BZSTRUCT_LOG_ERR("Could not init bzstruct ctx: %s\n", #_ctx); \
}




#endif

