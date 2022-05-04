from ast_ import *
from assign import *
from store import *
from name import *


class VarDef(AST):
    def __init__(self, **kwargs) -> None:
        self.message = ['type', 'vars']
        self.children = ['defaults']
        self.type = None
        self.vars = []
        self.defaults = []
        super().__init__(**kwargs)

    def gen(self):
        for i, var in enumerate(self.vars):
            self._symtab.add_varsym(VarSym(var, self.type))
            if self.defaults[i]:
                Assign(targets=[Name(id=var, mode=Store())],
                       value=self.defaults[i]).gen()
