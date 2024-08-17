#ifndef __FILECONTEXT_H__
#define __FILECONTEXT_H__

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#define FILECONTEXT_FROMASTCONTEXT(fc, ac)                                     \
    do                                                                         \
    {                                                                          \
        fc.filename = ac.filename;                                             \
        fc.line = ac.line;                                                     \
        fc.row = ac.s_row;                                                     \
        fc.col = ac.s_col;                                                     \
    } while (0)

typedef struct {
    FILE *file;
    char *filename;
    char *line;
    int row, col;
    int l_row; // 逻辑上的行, 用于处理'\'
} FileContext;

void filecontext_init(FileContext *, FILE *, char *, int, int);

#ifdef __cplusplus
}
#endif

#endif