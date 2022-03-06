from expr import *
from int import *

class Num(Expr):
    def __init__(self, **kwargs) -> None:
        self.value:int=0
        super().__init__(**kwargs)

    def gen(self):
        self.genir.add_inst('push',self.value)

    def analyse_type(self):
        self.type=Int()