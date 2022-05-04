from expr import *
from int_ import *


class Num(Expr):
    def __init__(self, **kwargs) -> None:
        self.message = ['num']
        self.num: int = 0
        super().__init__(**kwargs)

    def gen(self):
        self.type = Int()
        self._genir.add_code('LOAD_VALUE', self.num)
