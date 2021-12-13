#ifndef _BZSTRUCT_MACRO_H_
#define _BZSTRUCT_MACRO_H_


#define __TO_STR(_s) #_s
#define _TO_STR(_s) __TO_STR(_s)


#define ___NUM_ARGS(_64, _63, _62, _61, _60, _59, _58_, _57, _56, _55, _54, _53, _52, _51, _50, _49, _48, _47, _46, _45, _44, _43 ,_42, _41, _40, _39, _38, _37, _36,_35,_34,_33, _32, _31, _30, _29, _28, _27, _26, _25, _24, _23, _22, _21, _20, _19, _18,_17, _16, _15, _14, _13, _12, _11, _10, _9, _8, _7, _6, _5, _4, _3, _2, _1, N, ...) N
#define __NUM_ARGS(...) ___NUM_ARGS(__VA_ARGS__, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define _NUM_ARGS(...) __NUM_ARGS(0, ##__VA_ARGS__)

#if 0
#define _ARGS_TOKEN_0(...)
#define _ARGS_TOKEN_1(a, ...) a
#define _ARGS_TOKEN_2(a, ...) a, _ARGS_TOKEN_1(__VA_ARGS__)
#define _ARGS_TOKEN_3(a, ...) a, _ARGS_TOKEN_2(__VA_ARGS__)
#define _ARGS_TOKEN_4(a, ...) a, _ARGS_TOKEN_3(__VA_ARGS__)

#define ___ARGS_TOKEN(_4,_3,_2,_1,N, ...) _ARGS_TOKEN_ ## N 
#define __ARGS_TOKEN(...) ___ARGS_TOKEN(__VA_ARGS__, 4,3,2,1,0)(__VA_ARGS__)
#define _ARGS_TOKEN(s) __ARGS_TOKEN s
#define ARGS_TOKEN(s) _ARGS_TOKEN(s)

#define _ARGS_TOKEN_STR(s) _TO_STR(_ARGS_TOKEN(s))

#define _STRIP_PARENTHESES(...) __VA_ARGS__ 
#define STRIP_PARENTHESES(x)  _STRIP_PARENTHESES x
#endif

#define CSTRUCT_OFFSET_ADDR(type, pcstruct, field_offset) ((type)(((char *)pcstruct) + (field_offset)))

#if 0
#define _CSTRUCT_FIELD_NAME(type, name) name
#define CSTRUCT_FIELD_NAME(field) _CSTRUCT_FIELD_NAME field
#define _CSTRUCT_FIELD_TYPE(type, name) type
#define CSTRUCT_FIELD_TYPE(field) _CSTRUCT_FIELD_TYPE field
#endif


	
#define TYPE_OF_FIELD(cstru, field) typeof(((cstru *)0)->field)	
#define OFFSET_OF_FIELD(cstru, field) offsetof(cstru, field)
#define SIZE_OF_FIELD(cstru, field) sizeof(TYPE_OF_FIELD(cstru, field))
#define NUM_FIELD_ARGS(f) _NUM_ARGS f



//#define CSTRUCT_FIELD_TYPE(cstru, field) _CSTRUCT_FIELD_TYPE(cstruct, 1, 2)

//#define CSTRUCT_FIELD_TYPE(cstru, field) NUM_FIELD_ARGS(field) > 1 ? _GET_ARGS_0(field) ? 
//#define CSTRUCT_FIELD_NAME(cstru, field) (NUM_FIELD_ARGS(field) > 1 ? _GET_ARGS_1 # field : _GET_ARGS_0(field))

#define ___FIELD_NAME_3(type, name, num) name
#define ___FIELD_NAME_2(type, name) name
#define ___FIELD_NAME_1(name) name 
#if 0
#define ___FIELD_NAME(_1, _2, _3, N, ...) ___FIELD_NAME_##N
#define __FIELD_NAME(...) ___FIELD_NAME(__VA_ARGS__, 3, 2, 1, 0)
#define _FIELD_NAME(field) __FIELD_NAME field field
#else
#define ___FIELD_NAME(x)  ___FIELD_NAME_##x
#define __FIELD_NAME(x) ___FIELD_NAME(x)
#define _FIELD_NAME(field) __FIELD_NAME(NUM_FIELD_ARGS(field)) field
#endif

#define ___FIELD_TYPE_3(type, name, num) type
#define ___FIELD_TYPE_2(type, name) type
#define ___FIELD_TYPE_1(name) NULL 
#if 0
#define ___FIELD_TYPE(_1, _2, _3, N, ...) ___FIELD_TYPE_##N
#define __FIELD_TYPE(...) ___FIELD_TYPE(__VA_ARGS__, 3, 2, 1, 0)
#define _FIELD_TYPE(field) __FIELD_TYPE field field
#else
#define ___FIELD_TYPE(x) ___FIELD_TYPE_##x
#define __FIELD_TYPE(x) ___FIELD_TYPE(x)
#define _FIELD_TYPE(field) __FIELD_TYPE(NUM_FIELD_ARGS(field)) field
#endif

#define ___TYPEOF_FIELD_3(cstru, ...) typeof(_FIELD_TYPE(__VA_ARGS__))
#define ___TYPEOF_FIELD_2(cstru, ...) typeof(_FIELD_TYPE(__VA_ARGS__))
#define ___TYPEOF_FIELD_1(cstru, ...) typeof(((cstru *)0)->_FIELD_NAME(__VA_ARGS__))
#if 0
#define ___TYPEOF_FIELD_1(cstru, ...) TYPE_OF_FIELD(cstru, _FIELD_NAME(__VA_ARGS__))
#define __TYPEOF_FIELD(_1,_2,_3,N, ...) ___TYPEOF_FIELD_##N
#define _TYPEOF_FIELD(...) __TYPEOF_FIELD(__VA_ARGS__, 3,2,1,0)
#define TYPEOF_FIELD(cstru, field) _TYPEOF_FIELD field (cstru, field)
#else
#define __TYPEOF_FIELD(x) ___TYPEOF_FIELD_##x
#define _TYPEOF_FIELD(x) __TYPEOF_FIELD(x)
//#define TYPEOF_FIELD(cstru, field) _TYPEOF_FIELD(NUM_FIELD_ARGS(field))(cstru, field)
#define TYPEOF_FIELD(cstru, field) typeof(((cstru *)0)->_FIELD_NAME(field))

#endif

#define SIZEOF_FIELD(cstru, field) sizeof(TYPEOF_FIELD(cstru, field))
#define OFFSETOF_FIELD(cstru, field) offsetof(cstru, _FIELD_NAME(field))

#define ___CSTRUCT_NAME(_1, ...) _1
#define __CSTRUCT_NAME(...) ___CSTRUCT_NAME(__VA_ARGS__)
#define _CSTRUCT_NAME(cstru) __CSTRUCT_NAME cstru
#define CSTRUCT_NAME(cstru) _CSTRUCT_NAME(cstru)

#define ___CSTRUCT_ALIASE(_1, ...) __VA_ARGS__
#define __CSTRUCT_ALIASE(...) ___CSTRUCT_ALIASE(__VA_ARGS__)
#define _CSTRUCT_ALIASE(cstru) __CSTRUCT_ALIASE cstru
#define CSTRUCT_ALIASE(cstru) _CSTRUCT_ALIASE(cstru)
#define NUM_CSTRUCT_ARGS(cstru) NUM_FIELD_ARGS(cstru)









#if 0
#define FIELD_IS_ARRAY(cstru, field) (\
	__builtin_classify_type(((cstru *)0)->field) == 5 \
	&&  ! __builtin_types_compatible_p(TYPE_OF_FIELD(cstru, field), typeof(&((cstru *)0)->field[0]))) 

#define FILED_IS_POINT(cstru, field) (\
	__builtin_classify_type(((cstru *)0)->field) == 5 \
	&&  __builtin_types_compatible_p(TYPE_OF_FIELD(cstru, field), typeof(&((cstru *)0)->field[0])))
#endif

/*12 if struct type of builtin*/
#define FIELD_IS_CSTRUCT(cstru, field) (__builtin_classify_type(((cstru *)0)->_FIELD_NAME(field)) == 12)
#define FIELD_IS_NUMBER(cstru, field) (__builtin_classify_type(((cstru *)0)->_FIELD_NAME(field)) == 1)
#define FIELD_IS_CSTRUCT_ARRAY(cstru, field) \
	(NUM_FIELD_ARGS(field) > 2 \
	&& (__builtin_classify_type(((cstru *)0)->_FIELD_NAME(field)) == 5))

/*13 if union type of builtin*/
#define FIELD_IS_UNION(cstru, field) (__builtin_classify_type(((cstru *)0)->_FIELD_NAME(field)) == 13)
#define CSTRUCT_IS_UNION(cstru) (__builtin_classify_type(*((cstru *)0)) == 13)
#define CSTRUCT_IS_STRUCT(cstru) (__builtin_classify_type(*((cstru *)0)) == 12)


#if 0
#define BZSTRUCT_DTYPE_GET(cstru, field)  ( \
	0 | (__builtin_types_compatible_p(TYPEOF_FIELD(cstru, field), char) ? BZSTRUCT_DTYPE_CHAR : 0) \
	| (__builtin_types_compatible_p(TYPEOF_FIELD(cstru, field), char []) ? BZSTRUCT_DTYPE_CHAR_ARRAY : 0) \
	| (__builtin_types_compatible_p(TYPEOF_FIELD(cstru, field), char *) ? BZSTRUCT_DTYPE_STRING : 0) \
	| (__builtin_types_compatible_p(TYPEOF_FIELD(cstru, field), uint8_t) ? BZSTRUCT_DTYPE_UINT8: 0) \
	| (__builtin_types_compatible_p(TYPEOF_FIELD(cstru, field), uint8_t []) ? BZSTRUCT_DTYPE_UINT8_ARRAY: 0) \
	| (__builtin_types_compatible_p(TYPEOF_FIELD(cstru, field), int8_t) ? BZSTRUCT_DTYPE_INT8: 0) \
	| (__builtin_types_compatible_p(TYPEOF_FIELD(cstru, field), int8_t []) ? BZSTRUCT_DTYPE_INT8_ARRAY: 0) \
	| (__builtin_types_compatible_p(TYPEOF_FIELD(cstru, field), uint16_t) ? BZSTRUCT_DTYPE_UINT16 : 0) \
	| (__builtin_types_compatible_p(TYPEOF_FIELD(cstru, field), uint16_t []) ? BZSTRUCT_DTYPE_UINT16_ARRAY : 0) \
	| (__builtin_types_compatible_p(TYPEOF_FIELD(cstru, field), int16_t) ? BZSTRUCT_DTYPE_INT16 : 0) \
	| (__builtin_types_compatible_p(TYPEOF_FIELD(cstru, field), int16_t []) ? BZSTRUCT_DTYPE_INT16_ARRAY : 0) \
	| (__builtin_types_compatible_p(TYPEOF_FIELD(cstru, field), uint32_t) ? BZSTRUCT_DTYPE_UINT32 : 0) \
	| (__builtin_types_compatible_p(TYPEOF_FIELD(cstru, field), uint32_t []) ? BZSTRUCT_DTYPE_UINT32_ARRAY : 0) \
	| (__builtin_types_compatible_p(TYPEOF_FIELD(cstru, field), int32_t) ? BZSTRUCT_DTYPE_INT32 : 0) \
	| (__builtin_types_compatible_p(TYPEOF_FIELD(cstru, field), int32_t []) ? BZSTRUCT_DTYPE_INT32_ARRAY : 0) \
	| (__builtin_types_compatible_p(TYPEOF_FIELD(cstru, field), int64_t) ? BZSTRUCT_DTYPE_INT64 : 0) \
	| (__builtin_types_compatible_p(TYPEOF_FIELD(cstru, field), int64_t []) ? BZSTRUCT_DTYPE_INT64_ARRAY : 0) \
	| (FIELD_IS_CSTRUCT(cstru, field) ? BZSTRUCT_DTYPE_CSTRUCT : 0) \
	| (FIELD_IS_UNION(cstru, field) ? BZSTRUCT_DTYPE_UNION : BZSTRUCT_DTYPE_UNKNOWN))

#else
	#define BZSTRUCT_DTYPE_GET(cstru, field)  ( \
	(__builtin_types_compatible_p(TYPEOF_FIELD(cstru, field), char) ? BZSTRUCT_DTYPE_CHAR : \
	(__builtin_types_compatible_p(TYPEOF_FIELD(cstru, field), char []) ? BZSTRUCT_DTYPE_CHAR_ARRAY :  \
	(__builtin_types_compatible_p(TYPEOF_FIELD(cstru, field), char *) ? BZSTRUCT_DTYPE_STRING :  \
	(__builtin_types_compatible_p(TYPEOF_FIELD(cstru, field), uint8_t) ? BZSTRUCT_DTYPE_UINT8: \
	(__builtin_types_compatible_p(TYPEOF_FIELD(cstru, field), uint8_t []) ? BZSTRUCT_DTYPE_UINT8_ARRAY:  \
	(__builtin_types_compatible_p(TYPEOF_FIELD(cstru, field), int8_t) ? BZSTRUCT_DTYPE_INT8: \
	(__builtin_types_compatible_p(TYPEOF_FIELD(cstru, field), int8_t []) ? BZSTRUCT_DTYPE_INT8_ARRAY: \
	(__builtin_types_compatible_p(TYPEOF_FIELD(cstru, field), uint16_t) ? BZSTRUCT_DTYPE_UINT16 : \
	(__builtin_types_compatible_p(TYPEOF_FIELD(cstru, field), uint16_t []) ? BZSTRUCT_DTYPE_UINT16_ARRAY : \
	(__builtin_types_compatible_p(TYPEOF_FIELD(cstru, field), int16_t) ? BZSTRUCT_DTYPE_INT16 : \
	(__builtin_types_compatible_p(TYPEOF_FIELD(cstru, field), int16_t []) ? BZSTRUCT_DTYPE_INT16_ARRAY : \
	(__builtin_types_compatible_p(TYPEOF_FIELD(cstru, field), uint32_t) ? BZSTRUCT_DTYPE_UINT32 :  \
	(__builtin_types_compatible_p(TYPEOF_FIELD(cstru, field), uint32_t []) ? BZSTRUCT_DTYPE_UINT32_ARRAY :  \
	(__builtin_types_compatible_p(TYPEOF_FIELD(cstru, field), int32_t) ? BZSTRUCT_DTYPE_INT32 :  \
	(__builtin_types_compatible_p(TYPEOF_FIELD(cstru, field), int32_t []) ? BZSTRUCT_DTYPE_INT32_ARRAY : \
	(__builtin_types_compatible_p(TYPEOF_FIELD(cstru, field), int64_t) ? BZSTRUCT_DTYPE_INT64 :  \
	(__builtin_types_compatible_p(TYPEOF_FIELD(cstru, field), uint64_t) ? BZSTRUCT_DTYPE_UINT64 :  \
	(__builtin_types_compatible_p(TYPEOF_FIELD(cstru, field), int64_t []) ? BZSTRUCT_DTYPE_INT64_ARRAY :  \
	(FIELD_IS_CSTRUCT(cstru, field) ? BZSTRUCT_DTYPE_CSTRUCT : \
	(FIELD_IS_NUMBER(cstru, field) ? BZSTRUCT_DTYPE_INT32 : \
	(FIELD_IS_UNION(cstru, field) ? BZSTRUCT_DTYPE_UNION : \
	(FIELD_IS_CSTRUCT_ARRAY(cstru, field) ? BZSTRUCT_DTYPE_CSTRUCT_ARRAY : \
	(BZSTRUCT_DTYPE_UNKNOWN))))))))))))))))))))))))
	

#endif

#define REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field) \
	bzstruct_register_field(bzstru, _TO_STR(_FIELD_NAME(field)), \
	SIZEOF_FIELD(cstru, field), OFFSETOF_FIELD(cstru, field), BZSTRUCT_DTYPE_GET(cstru, field), _TO_STR(_FIELD_TYPE(field)))


#if 0
#define REGIST_CSTRUCT_FIELD_X(ctx, bzstru, cstru, field, N, ...) \
	({ \
		bzstruct_field_t *__p = REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field); \
		__p == NULL ? NULL : REGIST_CSTRUCT_FIELD_##N(ctx, bzstru, cstru,__VA_ARGS__); \
	})

#define _REGIST_CSTRUCT_FIELD_3(ctx, bzstru, cstru, field, ...) \
	REGIST_CSTRUCT_FIELD_X(ctx, bzstru, cstru, field, 2, __VA_ARGS__)

#endif

#define REGIST_CSTRUCT_FIELD_0(ctx, bzstru, cstru, ...) NULL

#define REGIST_CSTRUCT_FIELD_1(ctx, bzstru, cstru, field, ...) \
	({ \
		REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field);  \
	})
	
#define REGIST_CSTRUCT_FIELD_2(ctx, bzstru, cstru, field, ...) \
	({ \
		bzstruct_field_t *__p = REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field) ; \
		__p == NULL ? NULL : REGIST_CSTRUCT_FIELD_1(ctx, bzstru, cstru,__VA_ARGS__); \
	})

	
#define REGIST_CSTRUCT_FIELD_3(ctx, bzstru, cstru, field, ...) \
 	({ \
		bzstruct_field_t *__p = REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field) ; \
		__p == NULL ? NULL : REGIST_CSTRUCT_FIELD_2(ctx, bzstru, cstru,__VA_ARGS__); \
	})

 #define REGIST_CSTRUCT_FIELD_4(ctx, bzstru, cstru, field, ...) \
	({ \
		bzstruct_field_t *__p = REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field) ; \
		__p == NULL ? NULL : REGIST_CSTRUCT_FIELD_3(ctx, bzstru, cstru,__VA_ARGS__); \
	})

#define REGIST_CSTRUCT_FIELD_5(ctx, bzstru, cstru, field, ...) \
	({ \
		bzstruct_field_t *__p = REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field) ; \
		__p == NULL ? NULL : REGIST_CSTRUCT_FIELD_4(ctx, bzstru, cstru,__VA_ARGS__); \
	})

#define REGIST_CSTRUCT_FIELD_6(ctx, bzstru, cstru, field, ...) \
	({ \
		bzstruct_field_t *__p = REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field) ; \
		__p == NULL ? NULL : REGIST_CSTRUCT_FIELD_5(ctx, bzstru, cstru,__VA_ARGS__); \
	})

#define REGIST_CSTRUCT_FIELD_7(ctx, bzstru, cstru, field, ...) \
	({ \
		bzstruct_field_t *__p = REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field) ; \
		__p == NULL ? NULL : REGIST_CSTRUCT_FIELD_6(ctx, bzstru, cstru,__VA_ARGS__); \
	})

#define REGIST_CSTRUCT_FIELD_8(ctx, bzstru, cstru, field, ...) \
	({ \
		bzstruct_field_t *__p = REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field) ; \
		__p == NULL ? NULL : REGIST_CSTRUCT_FIELD_7(ctx, bzstru, cstru,__VA_ARGS__); \
	})

#define REGIST_CSTRUCT_FIELD_9(ctx, bzstru, cstru, field, ...) \
	({ \
		bzstruct_field_t *__p = REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field) ; \
		__p == NULL ? NULL : REGIST_CSTRUCT_FIELD_8(ctx, bzstru, cstru,__VA_ARGS__); \
	})

#define REGIST_CSTRUCT_FIELD_9(ctx, bzstru, cstru, field, ...) \
	({ \
		bzstruct_field_t *__p = REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field) ; \
		__p == NULL ? NULL : REGIST_CSTRUCT_FIELD_8(ctx, bzstru, cstru,__VA_ARGS__); \
	})

#define REGIST_CSTRUCT_FIELD_10(ctx, bzstru, cstru, field, ...) \
	({ \
		bzstruct_field_t *__p = REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field) ; \
		__p == NULL ? NULL : REGIST_CSTRUCT_FIELD_9(ctx, bzstru, cstru,__VA_ARGS__); \
	})

#define REGIST_CSTRUCT_FIELD_11(ctx, bzstru, cstru, field, ...) \
	({ \
		bzstruct_field_t *__p = REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field) ; \
		__p == NULL ? NULL : REGIST_CSTRUCT_FIELD_10(ctx, bzstru, cstru,__VA_ARGS__); \
	})

#define REGIST_CSTRUCT_FIELD_12(ctx, bzstru, cstru, field, ...) \
	({ \
		bzstruct_field_t *__p = REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field) ; \
		__p == NULL ? NULL : REGIST_CSTRUCT_FIELD_11(ctx, bzstru, cstru,__VA_ARGS__); \
	})

#define REGIST_CSTRUCT_FIELD_13(ctx, bzstru, cstru, field, ...) \
	({ \
		bzstruct_field_t *__p = REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field) ; \
		__p == NULL ? NULL : REGIST_CSTRUCT_FIELD_12(ctx, bzstru, cstru,__VA_ARGS__); \
	})


#define REGIST_CSTRUCT_FIELD_14(ctx, bzstru, cstru, field, ...) \
	({ \
		bzstruct_field_t *__p = REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field) ; \
		__p == NULL ? NULL : REGIST_CSTRUCT_FIELD_13(ctx, bzstru, cstru,__VA_ARGS__); \
	})

#define REGIST_CSTRUCT_FIELD_15(ctx, bzstru, cstru, field, ...) \
	({ \
		bzstruct_field_t *__p = REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field) ; \
		__p == NULL ? NULL : REGIST_CSTRUCT_FIELD_14(ctx, bzstru, cstru,__VA_ARGS__); \
	})

#define REGIST_CSTRUCT_FIELD_16(ctx, bzstru, cstru, field, ...) \
	({ \
		bzstruct_field_t *__p = REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field) ; \
		__p == NULL ? NULL : REGIST_CSTRUCT_FIELD_15(ctx, bzstru, cstru,__VA_ARGS__); \
	})

	
#define REGIST_CSTRUCT_FIELD_17(ctx, bzstru, cstru, field, ...) \
	({ \
		bzstruct_field_t *__p = REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field) ; \
		__p == NULL ? NULL : REGIST_CSTRUCT_FIELD_16(ctx, bzstru, cstru,__VA_ARGS__); \
	})

	
#define REGIST_CSTRUCT_FIELD_18(ctx, bzstru, cstru, field, ...) \
 	({ \
		bzstruct_field_t *__p = REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field) ; \
		__p == NULL ? NULL : REGIST_CSTRUCT_FIELD_17(ctx, bzstru, cstru,__VA_ARGS__); \
	})

 #define REGIST_CSTRUCT_FIELD_19(ctx, bzstru, cstru, field, ...) \
	({ \
		bzstruct_field_t *__p = REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field) ; \
		__p == NULL ? NULL : REGIST_CSTRUCT_FIELD_18(ctx, bzstru, cstru,__VA_ARGS__); \
	})

#define REGIST_CSTRUCT_FIELD_20(ctx, bzstru, cstru, field, ...) \
	({ \
		bzstruct_field_t *__p = REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field) ; \
		__p == NULL ? NULL : REGIST_CSTRUCT_FIELD_19(ctx, bzstru, cstru,__VA_ARGS__); \
	})

#define REGIST_CSTRUCT_FIELD_21(ctx, bzstru, cstru, field, ...) \
	({ \
		bzstruct_field_t *__p = REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field) ; \
		__p == NULL ? NULL : REGIST_CSTRUCT_FIELD_20(ctx, bzstru, cstru,__VA_ARGS__); \
	})

#define REGIST_CSTRUCT_FIELD_22(ctx, bzstru, cstru, field, ...) \
	({ \
		bzstruct_field_t *__p = REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field) ; \
		__p == NULL ? NULL : REGIST_CSTRUCT_FIELD_21(ctx, bzstru, cstru,__VA_ARGS__); \
	})

#define REGIST_CSTRUCT_FIELD_23(ctx, bzstru, cstru, field, ...) \
	({ \
		bzstruct_field_t *__p = REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field) ; \
		__p == NULL ? NULL : REGIST_CSTRUCT_FIELD_22(ctx, bzstru, cstru,__VA_ARGS__); \
	})

#define REGIST_CSTRUCT_FIELD_24(ctx, bzstru, cstru, field, ...) \
	({ \
		bzstruct_field_t *__p = REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field) ; \
		__p == NULL ? NULL : REGIST_CSTRUCT_FIELD_23(ctx, bzstru, cstru,__VA_ARGS__); \
	})

#define REGIST_CSTRUCT_FIELD_25(ctx, bzstru, cstru, field, ...) \
	({ \
		bzstruct_field_t *__p = REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field) ; \
		__p == NULL ? NULL : REGIST_CSTRUCT_FIELD_24(ctx, bzstru, cstru,__VA_ARGS__); \
	})

#define REGIST_CSTRUCT_FIELD_26(ctx, bzstru, cstru, field, ...) \
	({ \
		bzstruct_field_t *__p = REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field) ; \
		__p == NULL ? NULL : REGIST_CSTRUCT_FIELD_25(ctx, bzstru, cstru,__VA_ARGS__); \
	})

#define REGIST_CSTRUCT_FIELD_27(ctx, bzstru, cstru, field, ...) \
	({ \
		bzstruct_field_t *__p = REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field) ; \
		__p == NULL ? NULL : REGIST_CSTRUCT_FIELD_26(ctx, bzstru, cstru,__VA_ARGS__); \
	})

#define REGIST_CSTRUCT_FIELD_28(ctx, bzstru, cstru, field, ...) \
	({ \
		bzstruct_field_t *__p = REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field) ; \
		__p == NULL ? NULL : REGIST_CSTRUCT_FIELD_27(ctx, bzstru, cstru,__VA_ARGS__); \
	})

#define REGIST_CSTRUCT_FIELD_29(ctx, bzstru, cstru, field, ...) \
	({ \
		bzstruct_field_t *__p = REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field) ; \
		__p == NULL ? NULL : REGIST_CSTRUCT_FIELD_28(ctx, bzstru, cstru,__VA_ARGS__); \
	})


#define REGIST_CSTRUCT_FIELD_30(ctx, bzstru, cstru, field, ...) \
	({ \
		bzstruct_field_t *__p = REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field) ; \
		__p == NULL ? NULL : REGIST_CSTRUCT_FIELD_29(ctx, bzstru, cstru,__VA_ARGS__); \
	})

#define REGIST_CSTRUCT_FIELD_31(ctx, bzstru, cstru, field, ...) \
	({ \
		bzstruct_field_t *__p = REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field) ; \
		__p == NULL ? NULL : REGIST_CSTRUCT_FIELD_30(ctx, bzstru, cstru,__VA_ARGS__); \
	})

#define REGIST_CSTRUCT_FIELD_32(ctx, bzstru, cstru, field, ...) \
	({ \
		bzstruct_field_t *__p = REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field) ; \
		__p == NULL ? NULL : REGIST_CSTRUCT_FIELD_31(ctx, bzstru, cstru,__VA_ARGS__); \
	})

#define REGIST_CSTRUCT_FIELD_33(ctx, bzstru, cstru, field, ...) \
	({ \
		bzstruct_field_t *__p = REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field) ; \
		__p == NULL ? NULL : REGIST_CSTRUCT_FIELD_32(ctx, bzstru, cstru,__VA_ARGS__); \
	})
	
#define REGIST_CSTRUCT_FIELD_34(ctx, bzstru, cstru, field, ...) \
	({ \
		bzstruct_field_t *__p = REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field) ; \
		__p == NULL ? NULL : REGIST_CSTRUCT_FIELD_33(ctx, bzstru, cstru,__VA_ARGS__); \
	})

	
#define REGIST_CSTRUCT_FIELD_35(ctx, bzstru, cstru, field, ...) \
 	({ \
		bzstruct_field_t *__p = REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field) ; \
		__p == NULL ? NULL : REGIST_CSTRUCT_FIELD_34(ctx, bzstru, cstru,__VA_ARGS__); \
	})

 #define REGIST_CSTRUCT_FIELD_36(ctx, bzstru, cstru, field, ...) \
	({ \
		bzstruct_field_t *__p = REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field) ; \
		__p == NULL ? NULL : REGIST_CSTRUCT_FIELD_35(ctx, bzstru, cstru,__VA_ARGS__); \
	})

#define REGIST_CSTRUCT_FIELD_37(ctx, bzstru, cstru, field, ...) \
	({ \
		bzstruct_field_t *__p = REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field) ; \
		__p == NULL ? NULL : REGIST_CSTRUCT_FIELD_36(ctx, bzstru, cstru,__VA_ARGS__); \
	})

#define REGIST_CSTRUCT_FIELD_38(ctx, bzstru, cstru, field, ...) \
	({ \
		bzstruct_field_t *__p = REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field) ; \
		__p == NULL ? NULL : REGIST_CSTRUCT_FIELD_37(ctx, bzstru, cstru,__VA_ARGS__); \
	})

#define REGIST_CSTRUCT_FIELD_39(ctx, bzstru, cstru, field, ...) \
	({ \
		bzstruct_field_t *__p = REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field) ; \
		__p == NULL ? NULL : REGIST_CSTRUCT_FIELD_38(ctx, bzstru, cstru,__VA_ARGS__); \
	})

#define REGIST_CSTRUCT_FIELD_40(ctx, bzstru, cstru, field, ...) \
	({ \
		bzstruct_field_t *__p = REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field) ; \
		__p == NULL ? NULL : REGIST_CSTRUCT_FIELD_39(ctx, bzstru, cstru,__VA_ARGS__); \
	})

#define REGIST_CSTRUCT_FIELD_41(ctx, bzstru, cstru, field, ...) \
	({ \
		bzstruct_field_t *__p = REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field) ; \
		__p == NULL ? NULL : REGIST_CSTRUCT_FIELD_40(ctx, bzstru, cstru,__VA_ARGS__); \
	})

#define REGIST_CSTRUCT_FIELD_42(ctx, bzstru, cstru, field, ...) \
	({ \
		bzstruct_field_t *__p = REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field) ; \
		__p == NULL ? NULL : REGIST_CSTRUCT_FIELD_41(ctx, bzstru, cstru,__VA_ARGS__); \
	})

#define REGIST_CSTRUCT_FIELD_43(ctx, bzstru, cstru, field, ...) \
	({ \
		bzstruct_field_t *__p = REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field) ; \
		__p == NULL ? NULL : REGIST_CSTRUCT_FIELD_42(ctx, bzstru, cstru,__VA_ARGS__); \
	})

#define REGIST_CSTRUCT_FIELD_44(ctx, bzstru, cstru, field, ...) \
	({ \
		bzstruct_field_t *__p = REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field) ; \
		__p == NULL ? NULL : REGIST_CSTRUCT_FIELD_43(ctx, bzstru, cstru,__VA_ARGS__); \
	})

#define REGIST_CSTRUCT_FIELD_45(ctx, bzstru, cstru, field, ...) \
	({ \
		bzstruct_field_t *__p = REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field) ; \
		__p == NULL ? NULL : REGIST_CSTRUCT_FIELD_44(ctx, bzstru, cstru,__VA_ARGS__); \
	})

#define REGIST_CSTRUCT_FIELD_46(ctx, bzstru, cstru, field, ...) \
	({ \
		bzstruct_field_t *__p = REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field) ; \
		__p == NULL ? NULL : REGIST_CSTRUCT_FIELD_45(ctx, bzstru, cstru,__VA_ARGS__); \
	})


#define REGIST_CSTRUCT_FIELD_47(ctx, bzstru, cstru, field, ...) \
	({ \
		bzstruct_field_t *__p = REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field) ; \
		__p == NULL ? NULL : REGIST_CSTRUCT_FIELD_46(ctx, bzstru, cstru,__VA_ARGS__); \
	})

#define REGIST_CSTRUCT_FIELD_48(ctx, bzstru, cstru, field, ...) \
	({ \
		bzstruct_field_t *__p = REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field) ; \
		__p == NULL ? NULL : REGIST_CSTRUCT_FIELD_47(ctx, bzstru, cstru,__VA_ARGS__); \
	})

#define REGIST_CSTRUCT_FIELD_49(ctx, bzstru, cstru, field, ...) \
	({ \
		bzstruct_field_t *__p = REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field) ; \
		__p == NULL ? NULL : REGIST_CSTRUCT_FIELD_48(ctx, bzstru, cstru,__VA_ARGS__); \
	})

	
#define REGIST_CSTRUCT_FIELD_50(ctx, bzstru, cstru, field, ...) \
	({ \
		bzstruct_field_t *__p = REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field) ; \
		__p == NULL ? NULL : REGIST_CSTRUCT_FIELD_49(ctx, bzstru, cstru,__VA_ARGS__); \
	})

	
#define REGIST_CSTRUCT_FIELD_51(ctx, bzstru, cstru, field, ...) \
 	({ \
		bzstruct_field_t *__p = REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field) ; \
		__p == NULL ? NULL : REGIST_CSTRUCT_FIELD_50(ctx, bzstru, cstru,__VA_ARGS__); \
	})

 #define REGIST_CSTRUCT_FIELD_52(ctx, bzstru, cstru, field, ...) \
	({ \
		bzstruct_field_t *__p = REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field) ; \
		__p == NULL ? NULL : REGIST_CSTRUCT_FIELD_51(ctx, bzstru, cstru,__VA_ARGS__); \
	})

#define REGIST_CSTRUCT_FIELD_53(ctx, bzstru, cstru, field, ...) \
	({ \
		bzstruct_field_t *__p = REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field) ; \
		__p == NULL ? NULL : REGIST_CSTRUCT_FIELD_52(ctx, bzstru, cstru,__VA_ARGS__); \
	})

#define REGIST_CSTRUCT_FIELD_54(ctx, bzstru, cstru, field, ...) \
	({ \
		bzstruct_field_t *__p = REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field) ; \
		__p == NULL ? NULL : REGIST_CSTRUCT_FIELD_53(ctx, bzstru, cstru,__VA_ARGS__); \
	})

#define REGIST_CSTRUCT_FIELD_55(ctx, bzstru, cstru, field, ...) \
	({ \
		bzstruct_field_t *__p = REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field) ; \
		__p == NULL ? NULL : REGIST_CSTRUCT_FIELD_54(ctx, bzstru, cstru,__VA_ARGS__); \
	})

#define REGIST_CSTRUCT_FIELD_56(ctx, bzstru, cstru, field, ...) \
	({ \
		bzstruct_field_t *__p = REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field) ; \
		__p == NULL ? NULL : REGIST_CSTRUCT_FIELD_55(ctx, bzstru, cstru,__VA_ARGS__); \
	})

#define REGIST_CSTRUCT_FIELD_57(ctx, bzstru, cstru, field, ...) \
	({ \
		bzstruct_field_t *__p = REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field) ; \
		__p == NULL ? NULL : REGIST_CSTRUCT_FIELD_56(ctx, bzstru, cstru,__VA_ARGS__); \
	})

#define REGIST_CSTRUCT_FIELD_58(ctx, bzstru, cstru, field, ...) \
	({ \
		bzstruct_field_t *__p = REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field) ; \
		__p == NULL ? NULL : REGIST_CSTRUCT_FIELD_57(ctx, bzstru, cstru,__VA_ARGS__); \
	})

#define REGIST_CSTRUCT_FIELD_59(ctx, bzstru, cstru, field, ...) \
	({ \
		bzstruct_field_t *__p = REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field) ; \
		__p == NULL ? NULL : REGIST_CSTRUCT_FIELD_58(ctx, bzstru, cstru,__VA_ARGS__); \
	})

#define REGIST_CSTRUCT_FIELD_60(ctx, bzstru, cstru, field, ...) \
	({ \
		bzstruct_field_t *__p = REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field) ; \
		__p == NULL ? NULL : REGIST_CSTRUCT_FIELD_59(ctx, bzstru, cstru,__VA_ARGS__); \
	})

#define REGIST_CSTRUCT_FIELD_61(ctx, bzstru, cstru, field, ...) \
	({ \
		bzstruct_field_t *__p = REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field) ; \
		__p == NULL ? NULL : REGIST_CSTRUCT_FIELD_60(ctx, bzstru, cstru,__VA_ARGS__); \
	})

#define REGIST_CSTRUCT_FIELD_62(ctx, bzstru, cstru, field, ...) \
	({ \
		bzstruct_field_t *__p = REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field) ; \
		__p == NULL ? NULL : REGIST_CSTRUCT_FIELD_61(ctx, bzstru, cstru,__VA_ARGS__); \
	})


#define REGIST_CSTRUCT_FIELD_63(ctx, bzstru, cstru, field, ...) \
	({ \
		bzstruct_field_t *__p = REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field) ; \
		__p == NULL ? NULL : REGIST_CSTRUCT_FIELD_62(ctx, bzstru, cstru,__VA_ARGS__); \
	})

#define REGIST_CSTRUCT_FIELD_64(ctx, bzstru, cstru, field, ...) \
	({ \
		bzstruct_field_t *__p = REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field) ; \
		__p == NULL ? NULL : REGIST_CSTRUCT_FIELD_63(ctx, bzstru, cstru,__VA_ARGS__); \
	})

#define REGIST_CSTRUCT_FIELD_65(ctx, bzstru, cstru, field, ...) \
	({ \
		bzstruct_field_t *__p = REGIST_CSTRUCT_FIELD_COMMON(ctx, bzstru, cstru, field) ; \
		__p == NULL ? NULL : REGIST_CSTRUCT_FIELD_64(ctx, bzstru, cstru,__VA_ARGS__); \
	})



#if 0
#define __REGISTER_CSTRUCT_FIELD(_64, _63, _62, _61, _60, _59, _58_, _57, _56, _55, _54, _53, _52, _51, _50, _49, _48, _47, _46, _45, _44, _43 ,_42, _41, _40, _39, _38, _37, _36,_35,_34,_33, _32, _31, _30, _29, _28, _27, _26, _25, _24, _23, _22, _21, _20, _19, _18,_17, _16, _15, _14, _13, _12, _11, _10, _9, _8, _7, _6, _5, _4, _3, _2, _1, N, ...) REGIST_CSTRUCT_FIELD##N

#define _REGISTER_CSTRUCT_FIELD(...) \
	__REGISTER_CSTRUCT_FIELD(__VA_ARGS__, _63, _62, _61, _60, _59, _58_, _57, _56, _55, _54, _53, _52, _51, _50, _49, _48, _47, _46, _45, _44, _43 ,_42, _41, _40, _39, _38, _37, _36,_35, _34, _33, _32, _31, _30, _29, _28, _27, _26, _25, _24, _23, _22, _21, _20, _19, _18, _17, _16, _15, _14, _13, _12, _11, _10, _9, _8, _7, _6, _5, _4, _3, _2, _1, _0)

#else

#define ___REGISTER_CSTRUCT_FIELD(N) REGIST_CSTRUCT_FIELD_##N
#define __REGISTER_CSTRUCT_FIELD(N) ___REGISTER_CSTRUCT_FIELD(N)
#define _REGISTER_CSTRUCT_FIELD(...) __REGISTER_CSTRUCT_FIELD(_NUM_ARGS(__VA_ARGS__))

#endif

#define REGISTER_CSTRUCT_FIELD(ctx, bzstru, cstru,  ...) \
	_REGISTER_CSTRUCT_FIELD(__VA_ARGS__)(ctx, bzstru, cstru, __VA_ARGS__)


#if 0
#define REGISTER_CSTRUCT(ctx, type, ...) \
	({ \
		bzstruct_field_t *__fld; \
		bzstruct_t *___stru = bzstruct_register(ctx, #type, sizeof(type)); \
		___stru == NULL ? NULL : \
		((__fld = REGISTER_CSTRUCT_FIELD(ctx, ___stru, type, __VA_ARGS__)) == NULL ? ({bzstruct_destroy(___stru); NULL;}): \
		___stru); \
	})

#else

/*
*  Useage: REGISTER_CSTRUCT(ctx, (cstruct_name[,cstruct_aliase]), fields...)
*   fields format: (field_name) | (struct_name,field_name[,array_num])
*/

#define REGISTER_CSTRUCT(ctx, type, ...) \
	({ \
		bzstruct_field_t *__fld; \
		const char *__type_aliase; \
		const char *__type_name; \
		bzstruct_t *___stru; \
		__type_aliase = (NUM_CSTRUCT_ARGS(type) > 1 ? _TO_STR(CSTRUCT_ALIASE(type)) : NULL ); \
		__type_name = _TO_STR(CSTRUCT_NAME(type)); \
		(CSTRUCT_IS_STRUCT(CSTRUCT_NAME(type)) ||   CSTRUCT_IS_UNION(CSTRUCT_NAME(type))) ? \
		({\
			(___stru = bzstruct_register(ctx, __type_name, __type_aliase, \
							CSTRUCT_IS_UNION(CSTRUCT_NAME(type)), sizeof(CSTRUCT_NAME(type)))) == NULL ? NULL : \
			((__fld = REGISTER_CSTRUCT_FIELD(ctx, ___stru, CSTRUCT_NAME(type), __VA_ARGS__)) == NULL ? \
				({bzstruct_destroy(___stru); NULL;}) : ___stru); \
		}):NULL; \
	})	


#endif


#endif

