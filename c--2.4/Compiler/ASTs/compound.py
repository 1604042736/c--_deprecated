from ast_ import *


class Compound(AST):
    '''复合语句'''

    def __init__(self, **kwargs) -> None:
        self.children = ['body']
        self.body = []
        super().__init__(**kwargs)
