import os
import sys
__path__ = os.path.dirname(__file__)
sys.path.append(__path__)

__forget__ = ['__init__.py', '__all__.py']

with open(f'{__path__}\\__all__.py', mode='w')as file:
    for i in os.listdir(__path__):
        if os.path.isfile(__path__+'\\'+i) and i not in __forget__:
            root, ext = os.path.splitext(i)
            file.write(f'from {root} import *\n')
from __all__ import *


def printast(cur, back=None, step="  ", space=0):
    '''输出语法树'''
    if isinstance(cur,SyntaxTree):
        if isinstance(back,SyntaxTree):
            print()
        print(f'{step*space}{cur.__class__.__name__}')
        for key, val in cur.astattr.items():
            print(f'{step*(space+1)}{key}', end='')
            printast(val, cur, space=space+2)
    elif isinstance(cur,list):
        print()
        for i in cur:
            printast(i, cur, space=space)
    elif (isinstance(cur,str)
            or isinstance(cur,int)
            or cur==None):
        if isinstance(back,list):
            print(f'{step*space}{cur}')
        else:
            print(f': {cur}')
