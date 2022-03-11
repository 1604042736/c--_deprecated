from expr import *
from mode import *

class Attribute(Expr):
    def __init__(self, **kwargs) -> None:
        self.value:Expr=None
        self.attr:str=''
        self.mode:Mode=None
        super().__init__(**kwargs)

    def analyse_type(self):
        self.value.analyse_type()
        self.type=self.value.get_attr(self.attr).type

    def gen(self):
        self.value.gen()
        attr=self.value.get_attr(self.attr)
        self.genir.add_inst('pop','ecx')
        attr.type.gen(attr)
        self.mode.gen(self.type)