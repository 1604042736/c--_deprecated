from expr import *
from syntaxtree import *
from store import *


class Assign(SyntaxTree):
    def __init__(self, **kwargs) -> None:
        self.targets: list[Expr] = []
        self.value: Expr = None
        super().__init__(**kwargs)
        for i in self.targets:
            i.mode=Store()

    def gen(self):
        self.value.gen()
        self.gen_list(self.targets)
