from syntaxtree import *

class More(SyntaxTree):
    def gen(self,var):
        if var:
            var.gen()
            return var.type.size
        return 0