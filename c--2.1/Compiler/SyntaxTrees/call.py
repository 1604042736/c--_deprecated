from syntaxtree import *

class Call(SyntaxTree):
    def __init__(self, *args, **kwargs) -> None:
        self.func: str = ''
        self.args: list = []
        super().__init__(*args, **kwargs)

    def gen(self):
        if self.func not in self.symtab.functions:
            self.genir.add_extern(self.func)
        self.gen_list(self.args[::-1])
        self.genir.add_inst('call', self.func)
        self.genir.add_inst('add', 'esp', self.get_list_size(self.args))
        if self.func in self.symtab.functions:  #无法确认函数类型或函数类型为void的时侯不push
            self.genir.add_inst('push', 'eax')

    def set_symtab(self):
        self.set_list_symtab(self.args)

    def get_size(self):
        func=self.symtab.functions[self.func]
        return func.return_type.size