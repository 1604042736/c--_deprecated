from expr import *


class UnpackRef(Expr):
    def __init__(self, **kwargs) -> None:
        self.children=['exp']
        self.exp: Expr = None
        self.level: int = 0
        super().__init__(**kwargs)
