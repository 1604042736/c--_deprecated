from pointer import Pointer
from expr import *
from error import *

class UnpackRef(Expr):
    '''解引用'''
    def __init__(self, **kwargs) -> None:
        self.exp:Expr=None
        self.level:int=0
        super().__init__(**kwargs)

    def analyse_type(self):
        self.exp.analyse_type()
        self.type=self.exp.type
        if isinstance(self.type,Pointer):
            self.type.level-=self.level
            if self.type.level==0:
                self.type=self.exp.type.type
            elif self.type.level<0:
                self.error('无法解这么多引用')
        else:
            self.error(f'{self.type}不能解引用')
        self.type.analyse()

    def gen(self):
        self.exp.type=self.type
        self.exp.gen()