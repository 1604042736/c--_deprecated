from expr import *


class Eq(Expr):
    def gen(self):
        self._genir.add_code('EQ')
