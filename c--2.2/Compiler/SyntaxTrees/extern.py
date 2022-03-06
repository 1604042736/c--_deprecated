from syntaxtree import *

class Extern(SyntaxTree):
    '''外部声明'''
    def __init__(self, **kwargs) -> None:
        self.extern:list=[]
        super().__init__(**kwargs)

    def analyse(self):
        self.symtab.cursym=[]
        super().analyse()
        for i in self.symtab.cursym:
            i.isextern=True