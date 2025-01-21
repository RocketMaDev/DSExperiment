/**
 * Chapter 4 - STRING
 * Experiment 1.4 - String
 */
#ifndef __STR_H__
#define __STR_H__
#ifdef NODE_TYPE
#   undef NODE_TYPE
#endif
#define NODE_TYPE unsigned char
#include "arraylist.h"
#include "rerror.h"
typedef ArrayList String;

/**
 * Initialize the string.
 *
 * @param s An uninitialized string.
 * @see `ArrayListInit`
 */
int StrInit(String *s);
/**
 * Copy literal string into `String`.
 *
 * @param s     An uninitialized string.
 * @param cs    C-style string literal.
 * @see `ArrayListExtend`
 */
int StrAssign(String *s, const char *cs);
/**
 * Compare 2 strings like `strcmp`
 *
 * @param a A string.
 * @param b Another string.
 * @return `strcmp(a, b)`.
 */
int StrCompare(const String *a, const String *b);
/**
 * Get string length like `strlen`.
 *
 * @param s A string.
 * @return Its size.
 * @see `ArrayListSize`
 */
unsigned int StrLength(String *s);

/**
 * Concatenate `dst` with `src`.
 *
 * @param dst   Dest string.
 * @param src   Source string to append to `dst`.
 * @see `ArrayListExtend`
 */
int StrConcat(String *dst, const String *src);
/**
 * Slice `s` and store it in `sub`.
 *
 * @param sub   An uninitialized string.
 * @param s     The string to slice. It will leave unchanged.
 * @param pos   The index to start slice.
 * @param len   `sub`'s length.
 * @return A `-status` code where `status` is one of member in `RERROR`. `0` if no error.
 *
 * @exception RERR_OVERFLOW If `pos` to `pos + size` is not in string range.
 * @exception RERR_OOM      Insufficient memory when slicing into `sub`.
 */
int StrSubstr(String *sub, const String *s, unsigned int pos, unsigned int len);
/**
 * Traverse every char in string with `func`. The ending '\0' will not be traversed.
 *
 * @param s     A string.
 * @param buf   An arbitrary pointer which will be `func`'s third arg when called.
 * @param func  The function to traverse the string.
 * @see `ArrayListTraverse`
 */
void StrTraverse(String *s, void *buf, void (*func)(unsigned int index, NODE_TYPE ch, void *buf));
/**
 * Clone `src` into `dst`.
 *
 * @param dst   An uninitialized string.
 * @param src   The string to copy.
 * @see `ArrayListExtend`
 */
int StrClone(String *dst, const String *src);

/**
 * Locate a substring from `pos` in `s`, and return its index.
 *
 * @param s         The string to search from.
 * @param substr    The substring to search in `s`.
 * @param pos       Where to start searching.
 * @return The index of substr located in `s` if found, or one of `-status`
 *         below.
 *
 * @exception RERR_OVERFLOW `pos` is out of range of `s`.
 * @exception RERR_NOTFOUND `substr` not found in `s`.
 */
int StrIndex(const String *s, const String *substr, unsigned int pos);
/**
 * Replace seen `match` with `replace` in `s` repeatedly.
 *
 * @param s         The string where replacement happens.
 * @param match     The string to search in `s` and replace.
 * @param replace   The string to replace `match`.
 * @return A `-status` code where `status` is one of member in `RERROR`. `0` if no error.
 *
 * @exception RERR_OOM  Insufficient memory when running.
 */
int StrReplace(String *s, const String *match, const String *replace);
/**
 * Insert a substring before `index`.
 *
 * @param s         The string to insert.
 * @param index     Where to insert.
 * @param substr    String to insert.
 *
 * @exception RERR_OVERFLOW If index greater than `size`.
 * @exception RERR_OOM      Insufficient memory when expanding.
 */
int StrInsert(String *s, unsigned int index, const String *substr);
/**
 * Delete a substring of `s`.
 *
 * @param s     The string to be cut.
 * @param pos   Where to start cutting. pos will be cut.
 * @param len   The length to cut.
 * @return A `-status` code where `status` is one of member in `RERROR`. `0` if no error.
 *
 * @exception RERR_OVERFLOW The substr to cut is out of range of `s`.
 */
int StrDelete(String *s, unsigned int pos, unsigned int len);
#endif
