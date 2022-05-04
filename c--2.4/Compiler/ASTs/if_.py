from ast_ import *


class If(AST):
    def __init__(self, **kwargs) -> None:
        self.children = ['exp', 'body', 'orelse']
        self.exp: AST = None
        self.body: AST = None
        self.orelse: AST = None
        super().__init__(**kwargs)

    def gen(self):
        finallabel = self._genir.get_label()
        elselabel = self._genir.get_label()
        self.exp.gen()
        self._genir.add_code('JUMP_IF_FALSE', elselabel)
        self.body.gen()
        self._genir.add_code('JUMP', finallabel)
        self._genir.add_label(elselabel)
        if self.orelse:
            self.orelse.gen()
        self._genir.add_label(finallabel)
