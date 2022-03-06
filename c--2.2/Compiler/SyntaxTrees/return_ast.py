from syntaxtree import *

class Return(SyntaxTree):
    def __init__(self, *args, **kwargs) -> None:
        self.value:SyntaxTree=None
        super().__init__(*args, **kwargs)

    def gen(self):
        self.value.gen()
        self.genir.add_inst('pop', 'eax')
        self.genir.add_inst('mov', 'esp', 'ebp')
        self.genir.add_inst('pop', 'ebp')
        self.genir.add_inst('ret')