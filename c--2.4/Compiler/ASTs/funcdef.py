from vardef import *
from ast_ import *
from func import *


class FuncDef(AST):
    def __init__(self, **kwargs) -> None:
        self.message = ['name', 'return_type']
        self.children = ['args', 'body']
        self.return_type = None
        self.name = ''
        self.args = []
        self.body = None
        super().__init__(**kwargs)

    def gen(self):
        scope = Scope()

        self._symtab.add_scope(scope)
        self._genir.add_label(self.name)
        args_type = []
        arg_deep = 0  # 函数参数长度
        for i in self.args:
            i.gen()
            if isinstance(i, VarDef):  # TODO 改进算法
                for j in i.vars:
                    arg_deep += i.type.size
                    self._symtab.get_varsym(j).deep = -arg_deep
            args_type.append(i.type)
        _type = Func(return_type=self.return_type, args_type=args_type)
        varsym = VarSym(self.name, _type)
        self._symtab.add_varsym(varsym)

        if self.body:
            self.body.gen()

        self._symtab.pop_scope()
        # 函数类型的大小就是函数作用域的深度
        varsym.type.size = scope.deep
        varsym.type.scope = scope
        self._symtab.add_varsym(varsym)
