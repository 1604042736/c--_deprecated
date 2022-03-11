extern:
    void _printf(...)

struct A:
    int a

int _main():
    struct A b,c
    b.a=2
    c.a=1
    _printf("%d",b.a+c.a)