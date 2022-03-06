from expr import *


class BinOp(Expr):
    def __init__(self, **kwargs) -> None:
        self.left: Expr = None
        self.op: SyntaxTree = None
        self.right: Expr = None
        super().__init__(**kwargs)

    def analyse_type(self):
        self.type=self.left.get_type()

    def gen(self):
        self.left.gen()
        self.right.gen()
        self.op.gen()