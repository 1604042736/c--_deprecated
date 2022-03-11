from symtab import *
from genir import *

class Globals:
    '''全局共享变量'''
    symtab=Symtab()
    genir=GenIR()
    code=[] #代码
    filename='' #文件名