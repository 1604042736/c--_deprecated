extern:
    void _printf(char* format,...)

int _main():
    int[10] a
    int b=10,i=0
    while i<b:
        a[i]=i
        i=i+1
    i=0
    while i<b:
        _printf("%d ",a[i])
        i=i+1