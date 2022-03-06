import sys
import os
__path__=os.path.dirname(__file__)
sys.path.append(__path__)
__forget__=['__init__.py','__all__.py']
for i in os.listdir(__path__):
    root,ext=os.path.splitext(i)
    if ext=='.ui':
        os.system(f'pyuic5 {__path__}\\{i} -o {__path__}\\{root}_ui.py')
with open(f'{__path__}\\__all__.py',mode='w',encoding='utf-8')as file:
    for i in os.listdir(__path__):
        root,ext=os.path.splitext(i)
        if os.path.isfile(__path__+'\\'+i)and ext=='.py'and i not in __forget__:
            file.write(f'from {root} import *\n')
from __all__ import *