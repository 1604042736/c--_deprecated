from ast_ import *


class Expr(AST):
    '''表达式'''

    def __init__(self, **kwargs) -> None:
        if 'children' not in self.__dict__:
            self.children = []
        if 'message' not in self.__dict__:
            self.message = []
        self.message.extend(['type'])
        self.type = None
        super().__init__(**kwargs)
