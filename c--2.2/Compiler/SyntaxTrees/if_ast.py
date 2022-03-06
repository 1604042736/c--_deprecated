from syntaxtree import *
from expr import *


class If(SyntaxTree):
    def __init__(self, **kwargs) -> None:
        self.exp: Expr = None
        self.body: list[SyntaxTree] = []
        self.orelse: SyntaxTree = None
        super().__init__(**kwargs)

    def gen(self):
        self.exp.gen()
        jmplabel=self.genir.get_label()
        endlabel=self.genir.get_label()
        self.genir.add_inst('pop','eax')
        self.genir.add_inst('cmp','eax',0)
        self.genir.add_inst('je',jmplabel)
        self.gen_list(self.body)
        self.genir.add_inst('jmp',endlabel)
        self.genir.add_label(jmplabel)
        if self.orelse:
            self.orelse.gen()
        self.genir.add_label(endlabel)