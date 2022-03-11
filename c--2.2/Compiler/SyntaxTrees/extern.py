from syntaxtree import *

class Extern(SyntaxTree):
    '''外部声明'''
    def __init__(self, **kwargs) -> None:
        self.extern:list=[]
        super().__init__(**kwargs)

    def analyse(self):
        self.symtab.cursym.append([])
        super().analyse()
        for i in self.symtab.cursym[-1]:
            i.isextern=True
        self.symtab.cursym.pop()