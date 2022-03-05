from expr import *

class BoolOp(Expr):
    def __init__(self, *args, **kwargs) -> None:
        self.op: SyntaxTree = None
        self.values: list = []
        super().__init__(*args, **kwargs)

    def gen(self):
        endlabel=self.genir.get_label()
        finallabel=self.genir.get_label()
        self.values[0].gen()
        for i in range(1,len(self.values)):
            val=self.values[i]
            val.gen()
            self.op.gen()
            self.genir.add_inst('pop','eax')
            self.genir.add_inst('cmp','eax',0)
            self.genir.add_inst('je',finallabel)
            self.genir.add_inst('push',1)
        self.genir.add_inst('jmp',endlabel)
        self.genir.add_label(finallabel)
        self.genir.add_inst('push',0)
        self.genir.add_label(endlabel)

    def get_size(self):
        return self.values[0].get_size()

    def get_type(self):
        self.type=self.values[0].get_type()
        return self.type