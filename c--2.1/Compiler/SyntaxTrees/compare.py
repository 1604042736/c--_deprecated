from syntaxtree import *

class Compare(SyntaxTree):
    def __init__(self, *args, **kwargs) -> None:
        self.left: SyntaxTree = None
        self.ops: list = []
        self.comparators: list = []
        super().__init__(*args, **kwargs)

    def gen(self):
        endlabel=self.genir.get_label()
        finallabel=self.genir.get_label()
        self.left.gen()
        for i in range(len(self.ops)):
            op = self.ops[i]
            comparator = self.comparators[i]
            comparator.gen()
            #复制
            self.genir.add_inst('pop','edx')
            self.genir.add_inst('push','edx')
            op.gen()
            self.genir.add_inst('pop','eax')
            self.genir.add_inst('cmp','eax',0)
            self.genir.add_inst('je',finallabel)
            if i < len(self.ops)-1:
                self.genir.add_inst('push','edx')
        self.genir.add_inst('push',1)
        self.genir.add_inst('jmp',endlabel)
        self.genir.add_label(finallabel)
        self.genir.add_inst('push',0)
        self.genir.add_label(endlabel)

    def get_size(self):
        return self.left.get_size()