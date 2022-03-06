global _main
extern _printf

section .text:
_main:
push ebp
mov ebp,esp
sub esp,0
push constant0
call _printf
add esp,11
mov esp,ebp
pop ebp
ret 
section .data:
constant0 db 'Hello,World'