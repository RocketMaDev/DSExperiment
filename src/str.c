#include "str.h"
#include "arraylist.h"
#include <string.h>

#define IFERR(stat) \
    if ((err = stat)) \
        return err

int StrInit(String *s) {
    int err;
    IFERR(ArrayListInit(s, 0));
    s->arr[0] = '\0';
    return -RERR_OK;
}

int StrAssign(String *s, const char *cs) {
    unsigned int len = strlen(cs);
    int err;
    IFERR(ArrayListInit(s, len + 1));
    ArrayList buf = {(unsigned char *)cs, len, len + 1};
    ArrayListExtend(s, &buf);
    s->arr[len] = '\0';
    return -RERR_OK;
}

int StrClone(String *dst, const String *src) {
    int err;
    IFERR(ArrayListInit(dst, src->capacity + 1));
    ArrayListExtend(dst, src);
    dst->arr[src->size] = '\0';
    return -RERR_OK;
}

unsigned int StrLength(String *s) {
    return s->size;
}

int StrCompare(const String *a, const String *b) {
    int cmp = strcmp((const char *)a->arr, (const char *)b->arr);
    if (cmp < 0)
        return -1;
    if (cmp > 0)
        return 1;
    return 0;
}

int StrConcat(String *dst, const String *src) {
    int err;
    IFERR(ArrayListExtend(dst, src));
    IFERR(ArrayListAppend(dst, '\0'));
    ArrayListDelete(dst, dst->size - 1, NULL);
    return -RERR_OK;
}

int StrSubstr(String *sub, const String *s, unsigned int pos, unsigned int len) {
    if (pos >= s->size || pos + len > s->size)
        return -RERR_OVERFLOW;
    int err;
    IFERR(ArrayListInit(sub, len + 1));
    if (len == 0) {
        sub->arr[0] = '\0';
        return -RERR_OK;
    }
    ArrayList buf = {s->arr + pos, len, len + 1};
    ArrayListExtend(sub, &buf); // not possible to error
    sub->arr[sub->size] = '\0';
    return -RERR_OK;
}

void StrTraverse(String *s, void *buf, void (*func)(unsigned int index, NODE_TYPE ch, void *buf)) {
    ArrayListTraverse(s, buf, func);
}

int StrIndex(const String *s, const String *substr, unsigned int pos) {
    if (pos >= s->size)
        return -RERR_OVERFLOW;
    char *sub = strstr((char *)s->arr + pos, (char *)substr->arr);
    if (!sub)
        return -RERR_NOTFOUND;
    return sub - (char *)(s->arr + pos);
}

int StrReplace(String *s, const String *match, const String *replace) {
    int err, idx;
    while ((idx = StrIndex(s, match, 0)) >= 0) {
        StrDelete(s, idx, match->size);
        IFERR(StrInsert(s, idx, replace));
    }
    return -RERR_OK;
}

int StrInsert(String *s, unsigned int index, const String *substr) {
    int err;
    if (index > s->size)
        return -RERR_OVERFLOW;
    if (index == s->size) {
        IFERR(ArrayListExtend(s, substr));
        IFERR(ArrayListAppend(s, '\0'));
        ArrayListDelete(s, s->size - 1, NULL);
        return -RERR_OK;
    }
    String remaining;
    IFERR(StrSubstr(&remaining, s, index, s->size - index));
    StrDelete(s, index, s->size - index);
    IFERR(ArrayListExtend(s, substr));
    IFERR(ArrayListExtend(s, &remaining));
    IFERR(ArrayListAppend(s, '\0'));
    ArrayListDelete(s, s->size - 1, NULL);
    return -RERR_OK;
}

int StrDelete(String *s, unsigned int pos, unsigned int len) {
    if (pos >= s->size || pos + len > s->size)
        return -RERR_OVERFLOW;
    if (len == 0)
        return -RERR_OK; // do nothing
    if (pos + len != s->size - 1) // do memmove only when not cutting off the rest of str
        memmove(s->arr + pos, s->arr + pos + len, (s->size - pos - len) * sizeof(NODE_TYPE));
    s->size -= len;
    ArrayListAppend(s, '\0'); // shrinking, not possible to fail
    ArrayListDelete(s, s->size - 1, NULL);
    return -RERR_OK;
}

