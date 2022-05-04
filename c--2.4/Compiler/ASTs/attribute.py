from expr import *


class Attribute(Expr):
    def __init__(self, **kwargs) -> None:
        self.children=['value']
        self.value: Expr = None
        self.attr: str = ''
        self.mode = None
        super().__init__(**kwargs)
