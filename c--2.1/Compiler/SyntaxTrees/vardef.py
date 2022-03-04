from syntaxtree import *
from name import *
from store import *

class VarDef(SyntaxTree):
    '''
    变量声明
    '''

    def __init__(self, *args, **kwargs) -> None:
        self.type: SyntaxTree = None  # 类型
        self.names: str = []  # 名称
        self.values: SyntaxTree = []  # 初始值
        super().__init__(*args, **kwargs)

    def set_symtab(self):
        for name,value in zip(self.names,self.values):
            self.type.set_symtab(name)

    def gen(self):
        for name,value in zip(self.names,self.values):
            if value:
                value.gen()
                Name(id=name,mode=Store()).gen()