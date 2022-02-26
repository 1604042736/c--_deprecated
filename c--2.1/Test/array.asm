global _main
extern _printf

section .text:
_main:
push ebp
mov ebp,esp
sub esp,48
push 10
;->Name
mov ecx,ebp
sub ecx,40
pop eax
mov [ecx],eax
;<-Name
push 0
;->Name
mov ecx,ebp
sub ecx,44
pop eax
mov [ecx],eax
;<-Name
;->While
_0:
;->Name
mov ecx,ebp
sub ecx,44
mov eax,[ecx]
push eax
;<-Name
;->Name
mov ecx,ebp
sub ecx,40
mov eax,[ecx]
push eax
;<-Name
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
;->Name
mov ecx,ebp
sub ecx,44
mov eax,[ecx]
push eax
;<-Name
;->Subscript
;->Name
mov ecx,ebp
sub ecx,44
mov eax,[ecx]
push eax
;<-Name
;->Name
mov ecx,ebp
sub ecx,0
mov eax,ecx
push eax
;<-Name
pop ecx
pop eax
push 4
pop ebx
mul ebx
sub ecx,eax
pop eax
mov [ecx],eax
;<-Subscript
;->Name
mov ecx,ebp
sub ecx,44
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
sub ecx,44
pop eax
mov [ecx],eax
;<-Name
jmp _0
_5:
;<-While
push 0
;->Name
mov ecx,ebp
sub ecx,44
pop eax
mov [ecx],eax
;<-Name
;->While
_6:
;->Name
mov ecx,ebp
sub ecx,44
mov eax,[ecx]
push eax
;<-Name
;->Name
mov ecx,ebp
sub ecx,40
mov eax,[ecx]
push eax
;<-Name
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
;->Subscript
;->Name
mov ecx,ebp
sub ecx,44
mov eax,[ecx]
push eax
;<-Name
;->Name
mov ecx,ebp
sub ecx,0
mov eax,ecx
push eax
;<-Name
pop ecx
pop eax
push 4
pop ebx
mul ebx
sub ecx,eax
mov eax,[ecx]
push eax
;<-Subscript
push constant0
call _printf
add esp,7
;->Name
mov ecx,ebp
sub ecx,44
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
sub ecx,44
pop eax
mov [ecx],eax
;<-Name
jmp _6
_11:
;<-While
mov esp,ebp
pop ebp
ret 
section .data:
constant0 db '%d '