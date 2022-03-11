from expr import *
from pointer import *

class Address(Expr):
    '''取地址'''

    def __init__(self, **kwargs) -> None:
        self.exp: Expr = None
        super().__init__(**kwargs)

    def analyse_type(self):
        self.exp.analyse_type()
        self.type=self.exp.type
        if isinstance(self.type,Pointer):
            self.type.level+=1
        else:
            self.type=Pointer(type=self.exp.type,level=1)
        self.type.analyse()

    def gen(self):
        #不知为何这里必须重新设置
        self.exp.type=self.type
        self.exp.gen()