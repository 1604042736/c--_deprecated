import colorama
import Compiler
from Compiler.symtab import *
colorama.init(True)


class AST:
    '''语法树'''
    prefix = ''  # 输出的前缀

    def __init__(self, **kwargs) -> None:
        if 'children' not in self.__dict__:
            self.children: list[str] = []  # 要输出的子节点
        if 'message' not in self.__dict__:
            self.message: list[str] = []
        self.message.extend(['children'])
        self.__dict__ |= kwargs
        self._symtab = Compiler.symtab
        self._genir = Compiler.genir

    def reduce_list(self, l):
        '''展开列表'''
        result = []
        for i in l:
            if isinstance(i, list):
                result.extend(self.reduce_list(i))
            else:
                result.append(i)
        return result

    def print_children(self):
        '''输出子节点'''
        _prefix = AST.prefix
        AST.prefix += '|'
        l = [self.__dict__[i] for i in self.children]
        self.print_list(self.reduce_list(l))
        AST.prefix = _prefix

    def print_list(self, l):
        '''输出列表'''
        for i, t in enumerate(l):
            if isinstance(t, list):
                self.print_list(t)
                continue

            if i == len(l)-1:
                AST.prefix = AST.prefix[:-1]+' '
                print(AST.prefix[:-1]+'`-', end='')
            else:
                print(AST.prefix+'-', end='')

            _prefix = AST.prefix
            AST.prefix += ' '

            if t == None:
                print('\033[0;33;40mNone\033[0m')
            else:
                t.print()

            AST.prefix = _prefix

    def print(self):
        '''输出'''
        print(self)
        self.print_children()

    def gen(self):
        '''生成中间代码'''
        for key, val in self.__dict__.items():
            if isinstance(val, AST):
                val.gen()
            elif isinstance(val, list):
                for i in val:
                    if isinstance(i, AST):
                        i.gen()

    def __str__(self) -> str:
        result = f'\033[0;32;40m{self.__class__.__name__}\033[0m('
        message = []
        for i in self.message:
            message.append(
                f'\033[0;36;40m{i}\033[0m=\033[0;31;40m{repr(self.__dict__[i])}\033[0m')
        result += ",".join(message)
        return result+')'

    __repr__ = __str__
