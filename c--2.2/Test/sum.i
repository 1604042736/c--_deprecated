extern:
    void _printf(...)

int sum(int n):
    int s=0,i=1
    while i<=n:
        s=s+i
        i=i+1
    return s

int _main():
    _printf("%d",sum(100))