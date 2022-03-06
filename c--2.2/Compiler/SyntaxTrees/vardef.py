from syntaxtree import *
from type import *
from name import *
from store import *


class VarDef(SyntaxTree):
    def __init__(self, **kwargs) -> None:
        self.type: Type = None
        self.names: list[str] = []
        self.values: list[SyntaxTree] = []
        super().__init__(**kwargs)

    def analyse(self):
        for name,value in zip(self.names,self.values):
            self.type.set_val(name)
        return super().analyse()

    def gen(self):
        for name,value in zip(self.names,self.values):
            if value:
                value.gen()
                Name(id=name,mode=Store()).gen()