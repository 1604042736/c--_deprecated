global _main
extern _printf

section .text:
sum:
push ebp
mov ebp,esp
sub esp,16
push 0
mov ecx,ebp
sub ecx,4
pop eax
mov [ecx],eax
push 1
mov ecx,ebp
sub ecx,8
pop eax
mov [ecx],eax
_0:
mov ecx,ebp
sub ecx,8
mov eax,[ecx]
push eax
mov ecx,ebp
add ecx,8
mov eax,[ecx]
push eax
pop edx
push edx
pop ebx
pop eax
cmp eax,ebx
jg _3
push 1
jmp _4
_3:
push 0
_4:
pop eax
cmp eax,0
je _2
push 1
jmp _1
_2:
push 0
_1:
pop eax
cmp eax,0
je _5
mov ecx,ebp
sub ecx,4
mov eax,[ecx]
push eax
mov ecx,ebp
sub ecx,8
mov eax,[ecx]
push eax
pop ebx
pop eax
add eax,ebx
push eax
mov ecx,ebp
sub ecx,4
pop eax
mov [ecx],eax
mov ecx,ebp
sub ecx,8
mov eax,[ecx]
push eax
push 1
pop ebx
pop eax
add eax,ebx
push eax
mov ecx,ebp
sub ecx,8
pop eax
mov [ecx],eax
jmp _0
_5:
mov ecx,ebp
sub ecx,4
mov eax,[ecx]
push eax
pop eax
mov esp,ebp
pop ebp
ret 
mov esp,ebp
pop ebp
ret 
_main:
push ebp
mov ebp,esp
sub esp,0
push 100
call sum
add esp,4
push eax
push constant0
call _printf
add esp,4
mov esp,ebp
pop ebp
ret 
section .data:
constant0 db '%d'