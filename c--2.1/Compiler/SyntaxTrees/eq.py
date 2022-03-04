from syntaxtree import *

class Eq(SyntaxTree):
    def gen(self):
        self.genir.add_inst('pop','ebx')
        self.genir.add_inst('pop','eax')
        self.genir.add_inst('cmp','eax','ebx')
        falselabel=self.genir.get_label()
        endlabel=self.genir.get_label()
        self.genir.add_inst('jne',falselabel)
        self.genir.add_inst('push',1)
        self.genir.add_inst('jmp',endlabel)
        self.genir.add_label(falselabel)
        self.genir.add_inst('push',0)
        self.genir.add_label(endlabel)