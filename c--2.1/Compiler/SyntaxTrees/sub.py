from syntaxtree import *

class Sub(SyntaxTree):
    def gen(self):
        self.genir.add_inst('pop','ebx')
        self.genir.add_inst('pop','eax')
        self.genir.add_inst('sub','eax','ebx')
        self.genir.add_inst('push','eax')