from syntaxtree import *
from store import *

class Assign(SyntaxTree):
    def __init__(self, *args, **kwargs) -> None:
        self.targets: list = []
        self.value: SyntaxTree = None
        super().__init__(*args, **kwargs)
        for target in self.targets:
            target.mode = Store()

    def gen(self):
        self.value.gen()
        self.gen_list(self.targets)

    def set_symtab(self):
        self.value.set_symtab()