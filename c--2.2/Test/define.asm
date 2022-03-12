global _main
extern _printf

section .text:
_main:
push ebp
mov ebp,esp
sub esp,0
push 3
push constant1
call _printf
add esp,4
push 3
push constant1
call _printf
add esp,4
mov esp,ebp
pop ebp
ret 
section .data:
constant1 db '%d '