from expr import *

class Mul(Expr):
    def gen(self):
        self.genir.add_inst('pop','ebx')
        self.genir.add_inst('pop','eax')
        self.genir.add_inst('mul','ebx')
        self.genir.add_inst('push','eax')