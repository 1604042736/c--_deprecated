from ast_ import *


class ExpStmt(AST):
    def __init__(self, **kwargs) -> None:
        self.children = ['exp']
        self.exp = None
        super().__init__(**kwargs)
