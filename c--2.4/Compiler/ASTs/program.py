from ast_ import *


class Program(AST):
    def __init__(self, **kwargs) -> None:
        self.children = ['body']
        self.body: AST = None
        super().__init__(**kwargs)
