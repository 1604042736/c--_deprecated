extern:
    void _printf(...)

struct A:
    int a

int _main():
    struct A[10] b
    int i=0
    while i<10:
        b[i].a=i
        i=i+1
    i=0
    while i<10:
        _printf("%d ",b[i].a)
        i=i+1