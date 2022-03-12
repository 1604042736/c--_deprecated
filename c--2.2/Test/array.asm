global _main
extern _printf

section .text:
_main:
push ebp
mov ebp,esp
sub esp,84
push 10
mov ecx,ebp
sub ecx,40
pop eax
mov [ecx],eax
push 0
mov ecx,ebp
sub ecx,44
pop eax
mov [ecx],eax
_0:
mov ecx,ebp
sub ecx,44
mov eax,[ecx]
push eax
mov ecx,ebp
sub ecx,40
mov eax,[ecx]
push eax
pop edx
push edx
pop ebx
pop eax
cmp eax,ebx
jge _3
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
sub ecx,44
mov eax,[ecx]
push eax
mov ecx,ebp
sub ecx,44
mov eax,[ecx]
push eax
mov ecx,ebp
sub ecx,0
mov eax,ecx
push eax
pop ecx
pop eax
push 4
pop ebx
mul ebx
sub ecx,eax
pop eax
mov [ecx],eax
mov ecx,ebp
sub ecx,44
mov eax,[ecx]
push eax
push 1
pop ebx
pop eax
add eax,ebx
push eax
mov ecx,ebp
sub ecx,44
pop eax
mov [ecx],eax
jmp _0
_5:
push 0
mov ecx,ebp
sub ecx,44
pop eax
mov [ecx],eax
_6:
mov ecx,ebp
sub ecx,44
mov eax,[ecx]
push eax
mov ecx,ebp
sub ecx,40
mov eax,[ecx]
push eax
pop edx
push edx
pop ebx
pop eax
cmp eax,ebx
jge _9
push 1
jmp _10
_9:
push 0
_10:
pop eax
cmp eax,0
je _8
push 1
jmp _7
_8:
push 0
_7:
pop eax
cmp eax,0
je _11
mov ecx,ebp
sub ecx,44
mov eax,[ecx]
push eax
mov ecx,ebp
sub ecx,0
mov eax,ecx
push eax
pop ecx
pop eax
push 4
pop ebx
mul ebx
sub ecx,eax
mov eax,[ecx]
push eax
push constant0
call _printf
add esp,4
mov ecx,ebp
sub ecx,44
mov eax,[ecx]
push eax
push 1
pop ebx
pop eax
add eax,ebx
push eax
mov ecx,ebp
sub ecx,44
pop eax
mov [ecx],eax
jmp _6
_11:
mov esp,ebp
pop ebp
ret 
section .data:
constant0 db '%d '