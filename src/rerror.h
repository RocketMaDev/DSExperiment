#ifndef __ROCKET_ERROR_H__
#define __ROCKET_ERROR_H__
typedef enum {
    RERR_OK = 0,
    RERR_OOM = 1,
    RERR_OVERFLOW = 2,
    RERR_NOTFOUND = 3,
    RERR_UNDERFLOW = 4,
    RERR_NOTIMPLEMENTED = 5,
} RERROR;
#endif
