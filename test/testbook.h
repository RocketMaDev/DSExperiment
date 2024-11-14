#ifndef __BOOK_H__
#define __BOOK_H__

typedef struct {
    char bookid[16];
    char bookname[32];
    float price;
} Book;
#define NODE_TYPE Book

#endif
