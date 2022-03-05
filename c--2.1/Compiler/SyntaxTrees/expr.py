from syntaxtree import *
from type import *

class Expr(SyntaxTree):
    def __init__(self, *args, **kwargs) -> None:
        self.type:Type=None
        super().__init__(*args, **kwargs)

    def get_type(self):
        '''
        获取类型
        '''