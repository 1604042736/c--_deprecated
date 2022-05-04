from ast_ import *


class Load(AST):
    def gen(self):
        self._genir.add_code('LOAD_')