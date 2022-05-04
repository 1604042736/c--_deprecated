'''类型'''
import os
import sys
__path__ = os.path.dirname(__file__)
sys.path.append(__path__)

__forget__ = ['__init__.py', 'types_all.py']

with open(f'{__path__}\\types_all.py', mode='w')as file:
    for i in os.listdir(__path__):
        if os.path.isfile(__path__+'\\'+i) and i not in __forget__:
            root, ext = os.path.splitext(i)
            file.write(f'from {root} import *\n')

if True:
    from types_all import *
