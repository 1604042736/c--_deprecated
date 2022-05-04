from expr import *


class Compare(Expr):
    def __init__(self, **kwargs) -> None:
        self.children = ['left', 'ops', 'values']
        self.left: Expr = None
        self.ops: list[Expr] = []
        self.values: list[Expr] = []
        super().__init__(**kwargs)

    def gen(self):
        self.left.gen()
        for i, value in enumerate(self.values):
            if i > 0:  # 上一个操作数
                self._genir.add_code("LOAD_TEMP")
            value.gen()
            self._genir.add_code("STORE_TEMP")
            self.ops[i].gen()
