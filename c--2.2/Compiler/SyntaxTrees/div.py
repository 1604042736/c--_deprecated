from syntaxtree import *

class Div(SyntaxTree):
    def gen(self):
        self.genir.add_inst('pop','ebx')
        self.genir.add_inst('pop','eax')
        self.genir.add_inst('div','ebx')
        self.genir.add_inst('push','eax')