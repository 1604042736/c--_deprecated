from expr import *


class Add(Expr):
    def gen(self):
        self._genir.add_code('ADD')
