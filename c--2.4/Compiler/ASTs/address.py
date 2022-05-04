from expr import *


class Address(Expr):
    def __init__(self, **kwargs) -> None:
        self.children = ['exp']
        self.exp: Expr = None
        super().__init__(**kwargs)
