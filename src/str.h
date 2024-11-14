#ifndef __STR_H__
#define __STR_H__
#ifdef NODE_TYPE
#   undef NODE_TYPE
#endif
#define NODE_TYPE unsigned char
#include "arraylist.h"
#include "rerror.h"
typedef ArrayList String;

#define __init_here

int StrInit(__init_here String *s);
int StrAssign(__init_here String *s, const char *cs);
int StrCompare(const String *a, const String *b);
unsigned int StrLength(String *s);
int StrConcat(String *dst, const String *src);
int StrSubstr(__init_here String *sub, const String *s, unsigned int pos, unsigned int len);
void StrTraverse(String *s, void (*func)(unsigned int index, NODE_TYPE ch));
int StrClone(__init_here String *dst, const String *src);

int StrIndex(const String *s, const String *substr, unsigned int pos);
int StrReplace(String *s, const String *match, const String *replace);
int StrInsert(String *s, unsigned int index, const String *substr);
int StrDelete(String *s, unsigned int pos, unsigned int len);
#undef __init_here
#endif
