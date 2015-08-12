// gc.c
//
VM* newVM() {
	VM* vm = malloc(sizeof(VM));
	vm->firstObject = NULL;
	vm->stackSize = 0;
	vm->numObjects = 0;
	vm->maxObjects = INITIAL_GC_THRESHOLD;
	return vm;
}

void push(VM* vm, Object* object) {
	assert(vm->stackSize < STACK_MAX, "stack overflow!");
	vm->stack[vm->stackSize++] = object;
}

Object* pop(VM* vm) {
	assert(vm->stackSize > 0, "stack underflow!");
	return vm->stack[--vm->stackSize];
}

Object* newObject(VM* vm, ObjectType type) {
	if(vm->numObjects >= vm->maxObjects) {
		gc(vm);
	}

	Object* object = malloc(sizeof(Object));
	object->marked = 0;
	object->type = type;

	// 新元素添加到对象链表
	object->next = vm->firstObject;
	vm->firstObject = object;
	// 虚拟机对象计数+1
	vm->numObjects++;
	return object;
}

void pushInt(VM* vm, int intValue) {
	Object* object = newObject(vm, OBJ_INT);
	object->value = intValue;
	push(vm, object);
}

Object* pushPair(VM* vm) {
	Object* object = newObject(vm, OBJ_PAIR);
	object->head = pop(vm);
	object->tail = pop(vm);
	push(vm, object);
	return object;
}

void mark(Object* object) {
	// 避免循环引用
	if(object->marked) {
		return;
	}

	object->marked = 1;

	if(object->type == OBJ_PAIR) {
		mark(object->head);
		mark(object->tail);
	}
}

void markAll(VM* vm) {
	for (int i = 0; i < vm->stackSize; ++i) {
		mark(vm->stack[i]);
	}
}

void sweep(VM* vm) {
	Object** object = &vm->firstObject;
	while(*object) {
		// 注意语法
		if(!(*object)->marked) {
			// 这一块没看懂
			// 变量不可达，从链表删除，并且释放空间
			Object* unreached = *object;

			// 链表后移
			*object = unreached->next;
			free(unreached);

			// 对象数量计数-1
			vm->numObjects--;
		} else {
			// 对象已经被mark，为了下一次gc，unmark
			(*object)->marked = 0;
			// 链表后移 注意语法
			object = &(*object)->next;
		}
	}
}

void gc(VM* vm) {
	int numObjects = vm->numObjects;

	markAll(vm);
	sweep(vm);

	// 动态调整GC阈值，随堆中活动对象数量增加而增加，减少而减少
	// 2 为调整力度
	vm->maxObjects = numObjects * 2;

	DEBUG_PRINTF(("Collected %d objects, %d remaining.\n", numObjects - vm->numObjects, vm->numObjects));
}

void freeVM(VM *vm) {
	vm->stackSize = 0;
	gc(vm);
	free(vm);
}
