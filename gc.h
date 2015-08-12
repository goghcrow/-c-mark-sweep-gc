// @from http://blog.jobbole.com/53376/

#ifndef _GC_H_
#define _GC_H_

#define INPARAM
#define OUTPARAM

// 栈式虚拟机大小
#define STACK_MAX 256

// 触发GC的阈值
#define INITIAL_GC_THRESHOLD 8

// 变量类型
typedef enum {
	OBJ_INT,
	OBJ_PAIR
} ObjectType;

// 动态类型变量
typedef struct sObject {
	// 用于GC的标记 0未标记，1扫描过标记变量in use
	unsigned char marked;

	ObjectType type;

	union {
		/* OBJ_INT */
		int value;
		/* OBJ_PAIR */
		struct {
			struct sObject* head;
			struct sObject* tail;
		};
	};

	// 对象链表的下一个
	struct sObject* next;
} Object;

typedef struct {
	// 虚拟机的stack
	Object* stack[STACK_MAX];
	// 当前stack大小（“指针”）
	int stackSize;

	// 对象链表，存储所有创建对象，用于GC的扫描
	Object* firstObject;

	// 用于计算GC时机
	// 已经创建了多少对象
	int numObjects;
	// 触发GC的最大对象数目
	int maxObjects;
} VM;


/* public */VM* newVM();
/* public */void push(VM* vm, Object* value);
/* public */Object* pop(VM* vm);
/* public */void pushInt(VM* vm, int value);
/* public */Object* pushPair(VM* vm);
/* private */void markAll(VM* vm);
/* private */void sweep(VM* vm);
/* public */void gc(VM* vm);
/* private */void freeVM(VM *vm);

/* public */Object* newObject(VM* vm, ObjectType type);
/* private */void mark(Object* object);

#include "debug.h"
#include "gc.c"

#endif
