#ifndef __ROCKET_ERROR_H__
#define __ROCKET_ERROR_H__
typedef enum {
    RERR_OK = 0,
    RERR_OOM = 1,
    RERR_OVERFLOW = 2,
    RERR_NOTFOUND = 3,
    RERR_UNDERFLOW = 4,
    RERR_NOTIMPLEMENTED = 5,
    RERR_EMPTY = 6,
    RERR_CORRUPTED = 7,
    RERR_EXISTED = 8,
} RERROR;

#define OUT_OF_ENUM(msg) \
    do { \
        fprintf(stderr, msg \
                " @%s#L%d, exit\n", __FILE__, __LINE__); \
        exit(1); \
    } while (0)
#endif
