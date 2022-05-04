from expr import *


class BinOp(Expr):
    def __init__(self, **kwargs) -> None:
        self.children = ['left', 'op', 'right']
        self.left: Expr = None
        self.op: Expr = None
        self.right: Expr = None
        super().__init__(**kwargs)

    def gen(self):
        self.left.gen()
        self.type = self.left.type

        self.right.gen()
        self.op.gen()
