import os
import sys
from colorama import *

init(autoreset=True)


def order(o):
    print(f'\033[1;37;43m===={o}====\033[0m')
    os.system(o)
    print()


def test(fullname):
    filename = '.'.join(fullname.split('.')[:-1])
    asmname = filename+'.asm'
    objname = filename+'.o'
    exename = filename+'.exe'
    order(f'main.py Test\\{fullname}')
    order(f'nasm Test\\{asmname} -f elf32')
    order(f'gcc Test\\{objname} -o Test\\{exename}')
    order(f'Test\\{exename}')


def main(fullname=None):
    if fullname:
        test(fullname)
    else:
        for fullname in os.listdir('Test'):
            if fullname.split('.')[-1] == 'txt':
                test(fullname)


if __name__ == '__main__':
    if len(sys.argv) > 1:
        main(sys.argv[1])
    else:
        main()
