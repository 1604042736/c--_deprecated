from syntaxtree import *
from inttype import *

class Num(SyntaxTree):
    def __init__(self, *args, **kwargs) -> None:
        self.value = 0
        super().__init__(*args, **kwargs)

    def gen(self):
        self.genir.add_inst('push',self.value)

    def get_size(self):
        return IntType.size