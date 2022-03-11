global _main
extern _printf

section .text:
f:
push ebp
mov ebp,esp
sub esp,12
;->Name b
mov ecx,ebp
add ecx,12
mov eax,[ecx]
push eax
;<-Name b
;->Name a
mov ecx,ebp
add ecx,8
mov eax,[ecx]
push eax
;<-Name a
push constant0
call _printf
add esp,8
mov esp,ebp
pop ebp
ret 
_main:
push ebp
mov ebp,esp
sub esp,0
push 1
push 2
call f
add esp,8
push 2
push 2
call f
add esp,8
mov esp,ebp
pop ebp
ret 
section .data:
constant0 db '%d %d'