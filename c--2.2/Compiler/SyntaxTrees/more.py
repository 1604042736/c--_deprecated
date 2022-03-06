from syntaxtree import *

class More(SyntaxTree):
    def gen(self,var):
        var.gen()
        return var.type.size