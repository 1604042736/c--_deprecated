from syntaxtree import *

class BinOp(SyntaxTree):
    def __init__(self, *args, **kwargs) -> None:
        self.left: SyntaxTree = None
        self.op: SyntaxTree = None
        self.right: SyntaxTree = None
        super().__init__(*args, **kwargs)

    def gen(self):
        self.left.gen()
        self.right.gen()
        self.op.gen()

    def get_size(self):
        return self.left.get_size()