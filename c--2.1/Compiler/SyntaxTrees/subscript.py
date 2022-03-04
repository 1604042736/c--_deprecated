from syntaxtree import *

class Subscript(SyntaxTree):
    def __init__(self, *args, **kwargs) -> None:
        self.value: SyntaxTree = None
        self.slice: SyntaxTree = None
        self.mode: SyntaxTree = None
        super().__init__(*args, **kwargs)

    def set_symtab(self):
        self.value.set_symtab()
        self.slice.set_symtab()

    def gen(self):
        self.genir.comment('->Subscript')
        self.slice.gen()
        self.value.gen()
        self.genir.add_inst('pop','ecx')
        info=self.value.get_array_info()
        i=info.count
        level=info.level
        shift=info.basicsize
        ins=info.ins
        if i==0:
            self.genir.add_inst('pop','eax')
        else:
            self.genir.add_inst('pop','eax')
            self.genir.add_inst('push',level)
            self.genir.add_inst('pop','ebx')
            self.genir.add_inst('mul','ebx')
        self.genir.add_inst('push',shift)
        self.genir.add_inst('pop','ebx')
        self.genir.add_inst('mul','ebx')
        self.genir.add_inst(ins,'ecx','eax')
        self.mode.gen()
        self.genir.comment('<-Subscript')

    def get_array_info(self):
        info= self.value.get_array_info()
        info.next()
        return info

    def get_size(self):
        info= self.value.get_array_info()
        info.next()
        return info.size