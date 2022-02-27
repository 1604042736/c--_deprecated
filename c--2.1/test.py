import os
from colorama import *

init(autoreset = True)

def order(o):
    print(f'\033[1;37;43m===={o}====\033[0m')
    os.system(o)
    print()

def main():
    for fullname in os.listdir('Test'):
        if fullname.split('.')[-1]=='txt':
            filename='.'.join(fullname.split('.')[:-1])
            asmname=filename+'.asm'
            objname=filename+'.o'
            exename=filename+'.exe'
            order(f'main.py Test\\{fullname}')
            order(f'nasm Test\\{asmname} -f elf32')
            order(f'gcc Test\\{objname} -o Test\\{exename}')
            order(f'Test\\{exename}')

if __name__=='__main__':
    main()