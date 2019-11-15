#ifndef MACROS_H
#define MACROS_H

#define MAX(x, y) ((x) > (y) ? (x) : (y))

#define MIN(x, y) ((x) < (y) ? (x) : (y))

#define ABS(x) ((x) > 0 ? (x) : (-x))

#define RANGE(curr, min, max) for ((curr) = (min); (curr) <= (max); (curr)++)

#define RANGE_DESC(curr, max, min) for ((curr) = (max); (curr) >= (min); (curr)--)

#define SET(ary, start, count, value) \
{ \
    int curr; \
    for (curr = (start); curr < (start) + (count); curr++) {\
        ary[curr] = (value); \
    } \
} \

#endif /* MACROS_H */

