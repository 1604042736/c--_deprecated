from syntaxtree import *

class String(SyntaxTree):
    def __init__(self, *args, **kwargs) -> None:
        self.value = ''
        super().__init__(*args, **kwargs)
        self.value=self.value[1:-1]

    def gen(self):
        name = self.symtab.get_constant(self.value)
        self.genir.add_inst('push', name)

    def set_symtab(self):
        if self.value not in self.symtab.constant:
            self.symtab.add_constant(self.value,StringSym)

    def get_size(self):
        return len(self.value)