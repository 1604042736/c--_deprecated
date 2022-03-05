from expr import *

class Name(Expr):
    def __init__(self, *args, **kwargs) -> None:
        self.id: SyntaxTree = None
        self.mode: SyntaxTree = None
        super().__init__(*args, **kwargs)

    def gen(self):
        self.genir.comment('->Name')
        name=self.symtab.get_var(self.id)
        self.genir.add_inst('mov','ecx','ebp')
        name.type.gen(name)
        self.mode.gen(name)
        self.genir.comment('<-Name')

    def get_size(self):
        name=self.symtab.get_var(self.id)
        return name.type.get_size()

    def get_array_info(self):
        name=self.symtab.get_var(self.id)
        return name.type.get_array_info()

    def get_type(self):
        name=self.symtab.get_var(self.id)
        self.type=name.type
        return self.type