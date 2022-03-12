global _main
extern _printf

section .text:
f1:
push ebp
mov ebp,esp
sub esp,4
push 1
mov ecx,ebp
add ecx,8
pop eax
mov [ecx],eax
mov esp,ebp
pop ebp
ret 
f2:
push ebp
mov ebp,esp
sub esp,4
push 1
mov ecx,ebp
add ecx,8
pop eax
mov [ecx],eax
mov esp,ebp
pop ebp
ret 
_main:
push ebp
mov ebp,esp
sub esp,0
push 2
mov ecx,ebp
sub ecx,0
pop eax
mov [ecx],eax
mov ecx,ebp
sub ecx,0
mov eax,ecx
push eax
push constant2
call _printf
add esp,4
mov ecx,ebp
sub ecx,0
mov eax,[ecx]
push eax
call f1
add esp,4
mov ecx,ebp
sub ecx,0
mov eax,[ecx]
push eax
push constant2
call _printf
add esp,4
mov ecx,ebp
sub ecx,0
mov eax,ecx
push eax
call f2
add esp,4
mov ecx,ebp
sub ecx,0
mov eax,[ecx]
push eax
push constant2
call _printf
add esp,4
mov esp,ebp
pop ebp
ret 
section .data:
constant2 db '%d '