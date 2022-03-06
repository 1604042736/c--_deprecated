from syntaxtree import *
from symtab import *
from type import *

class Function(SyntaxTree):
    def __init__(self, **kwargs) -> None:
        self.type:Type=None
        self.name:str=''
        self.args:list=[]
        self.body:list[SyntaxTree]=None
        super().__init__(**kwargs)

    def analyse(self):
        func=FuncSym(self.name,self.type,self.args)
        self.symtab.add_func(func)
        self.symtab.enterfunc(func)
        super().analyse()
        self.symtab.leavefunc()

    def gen(self):
        if self.body:
            func=self.symtab.functions[self.name]
            self.genir.add_label(self.name)
            self.symtab.enterfunc(func)
            self.genir.add_inst('push', 'ebp')
            self.genir.add_inst('mov', 'ebp', 'esp')
            self.genir.add_inst('sub','esp',func.get_deep())
            self.gen_list(self.body)
            self.genir.add_inst('mov', 'esp', 'ebp')
            self.genir.add_inst('pop', 'ebp')
            self.genir.add_inst('ret')
            self.symtab.leavefunc()