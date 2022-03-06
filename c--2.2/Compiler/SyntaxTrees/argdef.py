from syntaxtree import *
from type import *
from expr import *


class ArgDef(SyntaxTree):
    def __init__(self, **kwargs) -> None:
        self.type:Type=None
        self.name:str=''
        self.default:Expr=None
        super().__init__(**kwargs)

    def gen(self,arg):
        if arg:
            arg.gen()
            return arg.type.size
        else:
            self.default.gen()
            return self.default.type.size

    def analyse(self):
        self.type.set_val(self.name)
        var=self.symtab.get_var(self.name)
        var.offset=-(var.offset+4)
        super().analyse()