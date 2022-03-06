from syntaxtree import *

class Mul(SyntaxTree):
    def gen(self):
        self.genir.add_inst('pop','ebx')
        self.genir.add_inst('pop','eax')
        self.genir.add_inst('mul','ebx')
        self.genir.add_inst('push','eax')