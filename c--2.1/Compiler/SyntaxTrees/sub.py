from expr import *

class Sub(Expr):
    def gen(self):
        self.genir.add_inst('pop','ebx')
        self.genir.add_inst('pop','eax')
        self.genir.add_inst('sub','eax','ebx')
        self.genir.add_inst('push','eax')