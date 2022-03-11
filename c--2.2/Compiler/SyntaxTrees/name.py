from expr import *
from mode import *

class Name(Expr):
    def __init__(self, **kwargs) -> None:
        self.id:str=''
        self.mode:Mode=None
        super().__init__(**kwargs)

    def analyse_type(self):
        var=self.symtab.get_var(self.id)
        self.type=var.type

    def gen(self):
        self.genir.comment('->Name '+self.id)
        name=self.symtab.get_var(self.id)
        self.genir.add_inst('mov','ecx','ebp')
        name.type.gen(name)
        self.mode.gen(self.type)
        self.genir.comment('<-Name '+self.id)