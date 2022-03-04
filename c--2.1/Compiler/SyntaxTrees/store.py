from syntaxtree import *

class Store(SyntaxTree):
    def gen(self,var=None):
        self.genir.add_inst('pop','eax')
        target='[ecx]'
        if var and var.address:
            target='ecx'
        if not var or var.islocal:
            self.genir.add_inst('mov',target,'eax')