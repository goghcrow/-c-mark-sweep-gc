#ifndef _DEBUG_H_
#define _DEBUG_H_

// DEBUG MODE
#define DEBUG

// 输出变量地址 参数必须为指针
#define POINT_TO(arg) printf(#arg" pt... %p\n", arg)

// 无函数调用debug 必须如此调用 DEBUG_PRINTF(("fmt", ...)) 注意表达式俩括号，宏调用无括号
#ifdef DEBUG
#define DEBUG_PRINTF(arg) debug_printf arg
#else
#define DEBUG_PRINTF(arg)
#endif

#include <stdarg.h>

// ANSI C 宏不支持不定长度参数
void debug_printf(char *fmt,...);
void assert(int condition, const char* msg);

#include "debug.c"

#endif
