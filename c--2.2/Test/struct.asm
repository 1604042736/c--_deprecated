global _main
extern _printf

section .text:
_main:
push ebp
mov ebp,esp
sub esp,4
push 2
mov ecx,ebp
sub ecx,0
mov eax,ecx
push eax
pop ecx
sub ecx,0
pop eax
mov [ecx],eax
push 1
mov ecx,ebp
sub ecx,4
mov eax,ecx
push eax
pop ecx
sub ecx,0
pop eax
mov [ecx],eax
mov ecx,ebp
sub ecx,0
mov eax,ecx
push eax
pop ecx
sub ecx,0
mov eax,[ecx]
push eax
mov ecx,ebp
sub ecx,4
mov eax,ecx
push eax
pop ecx
sub ecx,0
mov eax,[ecx]
push eax
pop ebx
pop eax
add eax,ebx
push eax
push constant0
call _printf
add esp,4
mov esp,ebp
pop ebp
ret 
section .data:
constant0 db '%d'