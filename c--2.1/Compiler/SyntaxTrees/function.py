from syntaxtree import *

class Function(SyntaxTree):
    def __init__(self, *args, **kwargs) -> None:
        self.type: SyntaxTree = None
        self.name: str = ''
        self.args: list = []
        self.body: list = []
        super().__init__(*args, **kwargs)

    def gen(self):
        if self.body:
            func=self.symtab.functions[self.name]
            self.symtab.enterfunc(func)
            self.genir.add_label(self.name)
            self.genir.add_inst('push', 'ebp')
            self.genir.add_inst('mov', 'ebp', 'esp')
            self.genir.add_inst('sub','esp',func.get_deep())
            self.gen_list(self.body)
            self.genir.add_inst('mov', 'esp', 'ebp')
            self.genir.add_inst('pop', 'ebp')
            self.genir.add_inst('ret')
            self.symtab.leavefunc()

    def set_symtab(self):
        self.symtab.add_func(self.name,self.args,self.type)
        if self.body:
            func=self.symtab.functions[self.name]
            self.symtab.enterfunc(func)
            self.set_list_symtab(self.args)
            self.set_list_symtab(self.body)
            self.symtab.leavefunc()