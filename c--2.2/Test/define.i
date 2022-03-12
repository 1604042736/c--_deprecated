extern:
    void _printf(char* format,...)
#define PI 3
#undef PI

#define 1 1

int _main():
    _printf("%d ",PI)
#else
    _printf("%d ",3)
#endif