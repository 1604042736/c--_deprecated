from expr import *


class Name(Expr):
    def __init__(self, **kwargs) -> None:
        self.message = ['id', 'mode']
        self.id: str = ''
        self.mode = None
        super().__init__(**kwargs)

    def gen(self):
        varsym = self._symtab.get_varsym(self.id)
        self.type = varsym.type

        self.mode.gen()
        self._genir.code[-1][0] += 'NAME'
        self._genir.code[-1].append(self.id)
