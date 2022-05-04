from expr import *


class Subscript(Expr):
    def __init__(self, **kwargs) -> None:
        self.children=['value', 'slice']
        self.value: Expr = None
        self.slice: Expr = None
        self.mode = None
        super().__init__(**kwargs)
