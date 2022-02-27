global _main
extern _printf

section .text:
sum:
push ebp
mov ebp,esp
sub esp,12
push 0
;->Name
mov ecx,ebp
sub ecx,4
pop eax
mov [ecx],eax
;<-Name
push 1
;->Name
mov ecx,ebp
sub ecx,8
pop eax
mov [ecx],eax
;<-Name
;->While
_0:
;->Name
mov ecx,ebp
sub ecx,8
mov eax,[ecx]
push eax
;<-Name
;->Name
mov ecx,ebp
add ecx,8
mov eax,[ecx]
push eax
;<-Name
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
;->Name
mov ecx,ebp
sub ecx,4
mov eax,[ecx]
push eax
;<-Name
;->Name
mov ecx,ebp
sub ecx,8
mov eax,[ecx]
push eax
;<-Name
pop ebx
pop eax
add eax,ebx
push eax
;->Name
mov ecx,ebp
sub ecx,4
pop eax
mov [ecx],eax
;<-Name
;->Name
mov ecx,ebp
sub ecx,8
mov eax,[ecx]
push eax
;<-Name
push 1
pop ebx
pop eax
add eax,ebx
push eax
;->Name
mov ecx,ebp
sub ecx,8
pop eax
mov [ecx],eax
;<-Name
jmp _0
_5:
;<-While
;->Name
mov ecx,ebp
sub ecx,4
mov eax,[ecx]
push eax
;<-Name
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
add esp,6
mov esp,ebp
pop ebp
ret 
section .data:
constant0 db '%d'