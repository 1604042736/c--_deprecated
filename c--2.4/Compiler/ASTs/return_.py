from ast_ import *


class Return(AST):
    def __init__(self, **kwargs) -> None:
        self.children = ['exp']
        self.exp = None
        super().__init__(**kwargs)

    def gen(self):
        self.exp.gen()
        self._genir.add_code('RETURN')
