from expr import *


class Or(Expr):
    def gen(self):
        self._genir.add_code('OR')
