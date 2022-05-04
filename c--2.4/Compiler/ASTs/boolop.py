from expr import *


class BoolOp(Expr):
    def __init__(self, **kwargs) -> None:
        self.children = ['op', 'values']
        self.op: Expr = None
        self.values: list[Expr] = []
        super().__init__(**kwargs)

    def gen(self):
        self.values[0].gen()
        for i in range(1, len(self.values)):
            value = self.values[i]
            value.gen()
            self.op.gen()
