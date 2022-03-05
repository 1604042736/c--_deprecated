from expr import *

class BinOp(Expr):
    def __init__(self, *args, **kwargs) -> None:
        self.left: Expr = None
        self.op: SyntaxTree = None
        self.right: Expr = None
        super().__init__(*args, **kwargs)

    def gen(self):
        self.left.gen()
        self.right.gen()
        self.op.gen()

    def get_size(self):
        return self.left.get_size()

    def get_type(self):
        self.type=self.left.get_type()
        return self.type