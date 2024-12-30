#include <stdbool.h>
typedef struct {
    unsigned key;
    unsigned age;
    char name[0xc];
    bool female;
} Record;

static Record infos[] = {
    {56, 19, "Zhang", true},
    {19, 20, "Wang", true},
    {80, 19, "Zhou", true},
    {5, 20, "Huang", false},
    {21, 20, "Zheng", false},
    {64, 19, "Li", false},
    {89, 18, "Liu", true},
    {13, 19, "Qian", true},
    {38, 20, "Sun", false},
    {75, 20, "Zhao", false},
    {92, 20, "Chen", false},
};
#define ninfo sizeof(infos) / sizeof(Record)

static Record *map[32];
#define MAP(k) map[(k) & 0x1f]
static unsigned keys[ninfo];

__attribute__((constructor))
static void map_init(void) {
    for (unsigned i = 0; i < ninfo; i++) {
        MAP(infos[i].key) = infos + i;
        keys[i] = infos[i].key;
    }
}
