from globals import *

class SyntaxTree:
    '''语法树,也是其他语法树的基类'''

    def __init__(self, **kwargs) -> None:
        self.__dict__ |= kwargs
        self.astattr=kwargs #语法树属性
        self.symtab=Globals.symtab
        self.genir=Globals.genir

    def analyse(self):
        '''分析'''
        for key,val in self.astattr.items():
            if isinstance(val,SyntaxTree):
                val.analyse()
            elif isinstance(val,list):
                for i in val:
                    if isinstance(i,SyntaxTree):
                        i.analyse()

    def gen(self):
        '''生成目标代码'''
        for key,val in self.astattr.items():
            if isinstance(val,SyntaxTree):
                val.gen()
            elif isinstance(val,list):
                self.gen_list(val)

    def gen_list(self,l):
        for i in l:
            if isinstance(i,SyntaxTree):
                i.gen()