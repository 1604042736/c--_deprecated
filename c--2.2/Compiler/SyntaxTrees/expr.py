from syntaxtree import *
from expr import *
from type import *


class Expr(SyntaxTree):
    '''表达式的基类'''

    def __init__(self, **kwargs) -> None:
        self.type: Type = None
        super().__init__(**kwargs)

    def get_type(self):
        '''获得类型'''
        if self.type:
            return self.type
        self.analyse_type()
        return self.type

    def analyse_type(self):
        '''分析类型'''

    def analyse(self):
        self.analyse_type()
        return super().analyse()

    def get_attr(self,name):
        '''获得属性'''
        return self.type.get_attr(name)