from expr import *
from mode import *

class Attribute(Expr):
    def __init__(self, **kwargs) -> None:
        self.name:Expr=None
        self.attr:str=''
        self.mode:Mode=None
        super().__init__(**kwargs)