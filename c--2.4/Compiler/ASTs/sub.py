from expr import *


class Sub(Expr):
    def gen(self):
        self._genir.add_code('SUB')
