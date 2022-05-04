f: 
push ebp
mov ebp,esp
sub esp,0
mov eax,[ebp+4]
push eax
mov eax,1
push eax
mov ecx,eax
pop ebx
pop eax
cmp eax,ebx
jz _4
push 1
jmp _3
_4: 
push 0
_3: 
mov eax,[ebp+4]
push eax
mov eax,2
push eax
mov ecx,eax
pop ebx
pop eax
cmp eax,ebx
jz _6
push 1
jmp _5
_6: 
push 0
_5: 
pop ebx
pop eax
cmp eax,0
jne _9
cmp ebx,0
je _8
_9: 
push 1
jmp _7
_8: 
push 0
_7: 
pop eax
cmp eax,0
je _2
mov eax,1
push eax
ret 
jmp _1
_2: 
_1: 
mov eax,[ebp+4]
push eax
mov eax,1
push eax
pop ebx
pop eax
sub eax,ebx
push eax
call f
add esp,4
push eax
mov eax,[ebp+4]
push eax
mov eax,2
push eax
pop ebx
pop eax
sub eax,ebx
push eax
call f
add esp,4
push eax
pop ebx
pop eax
add eax,ebx
push eax
ret 
mov esp,ebp
pop ebp
ret 
main: 
push ebp
mov ebp,esp
sub esp,0
mov eax,10
push eax
call f
add esp,4
push eax
mov esp,ebp
pop ebp
ret 
