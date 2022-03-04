from syntaxtree import *

class If(SyntaxTree):
    def __init__(self, *args, **kwargs) -> None:
        self.exp: SyntaxTree = None
        self.body: list = []
        self.elses: list = []
        super().__init__(*args, **kwargs)

    def gen(self):
        self.genir.comment('->If')
        self.exp.gen()
        jmplabel=self.genir.get_label()
        endlabel=self.genir.get_label()
        self.genir.add_inst('pop','eax')
        self.genir.add_inst('cmp','eax',0)
        self.genir.add_inst('je',jmplabel)
        self.gen_list(self.body)
        self.genir.add_inst('jmp',endlabel)
        self.genir.add_label(jmplabel)
        self.gen_list(self.elses)
        self.genir.add_label(endlabel)
        self.genir.comment('<-If')

    def set_symtab(self):
        self.exp.set_symtab()
        self.set_list_symtab(self.body)