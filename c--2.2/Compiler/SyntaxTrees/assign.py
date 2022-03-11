from expr import *
from syntaxtree import *
from store import *


class Assign(SyntaxTree):
    def __init__(self, **kwargs) -> None:
        self.targets: list[Expr] = []
        self.value: Expr = None
        super().__init__(**kwargs)

    def gen(self):
        self.value.gen()
        self.gen_list(self.targets)
