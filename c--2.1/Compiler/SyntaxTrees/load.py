from syntaxtree import *

class Load(SyntaxTree):
    def gen(self,var=None):
        target='[ecx]'
        if var and var.address:
            target='ecx'
        if not var or var.islocal:
            self.genir.add_inst('mov','eax',target)
            self.genir.add_inst('push','eax')