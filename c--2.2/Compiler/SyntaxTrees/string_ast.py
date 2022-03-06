from expr import *
from array_ast import *
from char import *

class String(Expr):
    def __init__(self, **kwargs) -> None:
        self.value: str = ''
        super().__init__(**kwargs)

    def gen(self):
        name = self.symtab.get_constant(self.value)
        self.genir.add_inst('push', name)

    def analyse(self):
        self.symtab.add_constant(self.value)
        super().analyse()

    def analyse_type(self):
        #字符串实际上是字符数组
        self.type=Array(type=Char(),dimen=[len(self.value)])