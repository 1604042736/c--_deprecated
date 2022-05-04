from expr import *


class Call(Expr):
    def __init__(self, **kwargs) -> None:
        self.message = ['func']
        self.children = ['args']
        self.func: str = ''
        self.args: list[Expr] = []
        super().__init__(**kwargs)

    def gen(self):
        varsym = self._symtab.get_varsym(self.func)
        self.type = varsym.type.return_type

        arg_len = 0
        for i in self.args:
            i.gen()
            i.print()
            arg_len += i.type.size
        self._genir.add_code('CALL', self.func, arg_len)
