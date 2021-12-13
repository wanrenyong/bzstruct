#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "bzstruct.h"
#include "cJSON.h"


extern bzstruct_ctx_t *_demo_ctx;
#define DEMO_CTX _demo_ctx




struct bar
{
	char *str;
};

struct foo
{
	int a;
	int b;
	struct bar bar;
};

enum demo_enum{
	DEMO_0,
	DEMO_1,
};

union ipaddr
{
	uint32_t ipv4_addr;
	uint8_t ipv6_addr[16]; 
};
typedef union ipaddr ipaddr_t;

struct array
{
	int type;
	char *name;
};

struct demo_s
{
	unsigned int id;
	int age;
	unsigned short port;
	char name[32];
	char *addr;
	uint16_t code[32];
	char c;
	struct foo foo;
	ipaddr_t ip;
	enum demo_enum en;
	struct array array[10];
};

typedef struct demo_s demo_t;



int demo_main(int argc, char *argv[])
{
	
#if 1
	int ret;
	const char *s;
	int i, j;
	void *obj;

	struct demo_s d ={
		.id = 123,
		.age = 32,
		.port = 80,
		.name = "wanry",
		.addr = "shanghai songjiang",
		.c = 'a',
		. foo = {
			.a = 100,
			.b = 200,
			.bar = {
				.str = "Welecom to China",
			},
		},
		.ip = {
			.ipv4_addr = 0xc0c2c3c1,
		},
		.en = DEMO_1,
		.array = {
			[0] = {0, "hello"},
			[1] = {1, "world}"},
			[2] = 2,
			[3] = 3,
		},
	}; 

	
	
	memset(d.code, 0xff, sizeof(d.code));


	
	
	
//	_BZSTRUCT_DUMP(DEMO_CTX, demo_t, &d);
	
	BZSTRUCT_DUMP(demo_t, &d);

	memset(&d, 0, sizeof(d));

	s = "{\"id\":4222,\"age\":55,\"name\":\"HELLO\",\"addr\":\"beijing,china\",\"port\":17,\"code\":[1,2,3,4,6,10,100],\"foo\":{\"a\":22,\"b\":24,\"bar\":{\"str\":\"10.0.0.1\"}}}";

	BZSTRUCT_DECODE(demo_t, &d, (uint8_t *)s, 0);

	printf("id: %d\n", d.id);
	printf("age: %d\n", d.age);
	printf("name: %s\n", d.name);
	printf("addr: %s\n", d.addr);
	printf("port: %d\n", d.port);

	printf("code: ");
	for(i = 0; i < sizeof(d.code)/sizeof(d.code[0]); i++)
		printf("%d ", d.code[i]);

	printf("\n");

	printf("foo.a: %d\n", d.foo.a);
	printf("foo.b: %d\n", d.foo.b);
	printf("foo.bar.str: %s\n", d.foo.bar.str);


	
	printf("---------------------------------\n");

	obj = BZSTRUCT_TO_OBJ(demo_t, &d);
	if(obj != NULL)
	{
		char *str;
		str = cJSON_PrintUnformatted(obj);
		cJSON_Delete(obj);
		if(str != NULL)
		{
			printf("%s\n", str);
			free(str);
		}
		else
		{
			printf("Coudl not print\n");
		}
	}
	

#endif
	
}

#if 0

#define DEMO_STRUCT_CTX_INIT() BZSTRUCT_CTX_INIT(DEMO_CTX, NULL)
#define DEMO_CSTRUCT_REGISTER(type, ...) _BZSTRUCT_REGISTER(DEMO_CTX, type, __VA_ARGS__)

DEMO_STRUCT_CTX_INIT()
DEMO_CSTRUCT_REGISTER((union ipaddr,ipaddr_t), (ipv4_addr), (ipv6_addr))
DEMO_CSTRUCT_REGISTER((struct bar), (str))
DEMO_CSTRUCT_REGISTER((struct foo), (a), (b), (struct bar, bar))
DEMO_CSTRUCT_REGISTER((struct array), (name), (type))
DEMO_CSTRUCT_REGISTER((struct demo_s, demo_t), (id), (age), \
(name), (addr), (c), (port), (code), (struct foo, foo), (union ipaddr, ip), (en), (struct array, array, []))

#else

BZSTRUCT_REGISTER((union ipaddr,ipaddr_t), (ipv4_addr), (ipv6_addr))
BZSTRUCT_REGISTER((struct bar), (str))
BZSTRUCT_REGISTER((struct foo), (a), (b), (struct bar, bar))
BZSTRUCT_REGISTER((struct array), (name), (type))
BZSTRUCT_REGISTER((struct demo_s, demo_t), (id), (age), \
(name), (addr), (c), (port), (code), (struct foo, foo), (union ipaddr, ip), (en), (struct array, array, []))
#endif


