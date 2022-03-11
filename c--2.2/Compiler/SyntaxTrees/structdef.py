from syntaxtree import *
from symtab import *

class StructDef(SyntaxTree):
    def __init__(self, **kwargs) -> None:
        self.name:str=''
        self.defs:list=[]
        super().__init__(**kwargs)

    def analyse(self):
        struct=StructSym(self.name)
        self.symtab.add_struct(struct)
        self.symtab.enterfunc(struct)
        super().analyse()
        self.symtab.leavefunc()