from expr import *

class Add(Expr):
    def gen(self):
        self.genir.add_inst('pop','ebx')
        self.genir.add_inst('pop','eax')
        self.genir.add_inst('add','eax','ebx')
        self.genir.add_inst('push','eax')