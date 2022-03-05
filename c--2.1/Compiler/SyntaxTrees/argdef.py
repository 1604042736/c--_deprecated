from syntaxtree import *

class ArgDef(SyntaxTree):
    def __init__(self, *args, **kwargs) -> None:
        self.type=None
        self.name=''
        super().__init__(*args, **kwargs)

    def set_symtab(self):
        self.type.set_symtab(self.name)
        name=self.symtab.get_var(self.name)
        name.offset=-(name.offset+4)