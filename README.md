## Features 
1. support basic c type and array of basic c type  
2. support struct alias, for instance typedef 
3. all number type treated as signed number
4. support union, union register likes struct, but decode or encode the max size field of union
5. do not support point expert char point, char point is treated as string
6. do not support embeded named struct but un-named struct, un-named struct memebers is  treated as prarent struct member 
7. using cjson to decode and encode
8. support max number of 64 fields in a struct

## usage 

```
/* Create ctx to store C struct infomation, and specify to how decode and encode C struct
* @spec: decode and encode specification, now just support cJSON, it can be NULL.
* return: Return point type of bzstruct_ctx_t if no error.
*/
bzstruct_ctx_t *bzstruct_ctx_new(char *spec);

/* Register C struct
*       
*  @ctx:  bzstrcut ctx created by bzstruct_ctx_new
*  @type: C struct name, format like (foo, foo_t)
*  @__VA_ARGS: fileds, every filed must enclose with parenthese
*  return: return piont type of bzstruct_t if no error  
*/
REGISTER_CSTRUCT(ctx, type, ...)

/* Decode buf to C struct
*/
int bzstruct_decode(bzstruct_ctx_t *ctx, const char *struct_name, void *cstruct, uint8_t *buf, int len);

/*Encode C struct to buf*/
int bzstruct_encode(bzstruct_ctx_t *ctx, const char *struct_name, void *cstruct, uint8_t **buf, int *len);


```


## How to build 
```
Usage: make [Options]
  Options:
     TEST=<1>                 - Build a demo program
     BUILD_DIR=<build_dir>    - Define Build dir, default is ./build
     EXTRA_C_FLAGS=<flag>     - Define extra c compling flags
     INSTALL_LIB_DIR=<dir>    - Define where lib will be installed, default is BUILD_DIR/lib
     clean                    - Clean BUILD_DIR
```
