from syntaxtree import *
from symtab import *

class Type(SyntaxTree):
    '''类型的基类'''
    def __init__(self, **kwargs) -> None:
        self.size:int=0 #长度
        super().__init__(**kwargs)

    def set_val(self,name):
        '''为变量设置符号表信息'''
        islocal=True if self.symtab.curfunc else False
        var=VarSym(name,self,offset=self.symtab.get_offset(),islocal=islocal)
        self.symtab.add_var(var)
        self.symtab.add_offset(int(self.size))

    def __str__(self) -> str:
        return self.__class__.__name__.lower()

    def gen(self,var):
        if var.offset>=0:
            self.genir.add_inst('sub','ecx',var.offset)
        else:
            self.genir.add_inst('add','ecx',-var.offset+4)

    def load(self):
        '''加载变量'''
        self.genir.add_inst('mov','eax','[ecx]')
        self.genir.add_inst('push','eax')

    def store(self):
        '''存储变量'''
        self.genir.add_inst('pop','eax')
        self.genir.add_inst('mov','[ecx]','eax')