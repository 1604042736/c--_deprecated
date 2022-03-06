from syntaxtree import *

class While(SyntaxTree):
    def __init__(self, *args, **kwargs) -> None:
        self.exp:SyntaxTree=None
        self.body:list=[]
        super().__init__(*args, **kwargs)

    def gen(self):
        beginlabel=self.genir.get_label()
        self.genir.add_label(beginlabel)
        self.exp.gen()
        endlabel=self.genir.get_label()
        self.genir.add_inst('pop','eax')
        self.genir.add_inst('cmp','eax',0)
        self.genir.add_inst('je',endlabel)
        self.gen_list(self.body)
        self.genir.add_inst('jmp',beginlabel)
        self.genir.add_label(endlabel)