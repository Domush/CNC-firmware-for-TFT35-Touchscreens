#ifndef _MY_MISC_H_
#define _MY_MISC_H_

// Macros to make a string from a macro
#define STRINGIFY_(M) #M
#define STRINGIFY(M) STRINGIFY_(M)

#define COUNT(n) (sizeof(n) / sizeof(n[0]))

typedef char *my_va_list;
//_ INTSIZEOF (n) macro: align sizeof (n) by sizeof (int)?
#define MY_INTSIZEOF(n) ((sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1))

//Take the format parameter and change the parameter address to 4-byte alignment
#define my_va_start(ap, format) (ap = (my_va_list)&format + MY_INTSIZEOF(format))

//For type data, first get its 4-byte aligned address, then fetch it?
#define my_va_arg(ap, type) (*(type *)((ap += MY_INTSIZEOF(type)) - MY_INTSIZEOF(type)))

#define my_va_end(ap) (ap = (my_va_list)0)

#define ABS(n) ((n) > 0 ? (n) : -(n))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) > (b) ? (b) : (a))

int inRange(int cur, int tag, int range);
int limitValue(int min, int value, int max);
long map(long x, long in_min, long in_max, long out_min, long out_max);
int my_vsprintf(char *buf, const char *fmt, my_va_list args);
void my_sprintf(char *buf, const char *fmt, ...);
char *substr(const char *src, int m, int n);

#endif
