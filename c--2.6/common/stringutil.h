#ifndef __STRINGUTIL_H__
#define __STRINGUTIL_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

char *empty_string();
char *add_char(char *, char);
char *add_string(char *, char *);
char *repr(char *);
char *from_uint64(uint64_t);

char *path_get_basename(char *path);
char *path_join(char *p1, char *p2);

unsigned long long string_hash(char *);

#ifdef __cplusplus
}
#endif

#endif