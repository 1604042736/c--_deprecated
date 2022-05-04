from expr import *


class String(Expr):
    def __init__(self, **kwargs) -> None:
        self.children=['str']
        self.str: str = ''
        super().__init__(**kwargs)
