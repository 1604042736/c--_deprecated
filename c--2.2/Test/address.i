extern:
    void _printf(...)

void f1(int a):
    a=1

void f2(int* a):
    *a=1

int _main():
    int n=2
    _printf("%d ",&n)
    f1(n)
    _printf("%d ",n)
    f2(&n)
    _printf("%d ",n)