extern:
    void _printf(...)

void f(int a,int b=1):
    _printf("%d %d",a,b)

int _main():
    f(2)
    f(2,2)