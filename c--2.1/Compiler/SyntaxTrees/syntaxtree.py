from globals import *
from symtab import *

class SyntaxTree:
    '''
    所有语法树的基类
    '''

    def __init__(self, *args, **kwargs) -> None:
        self.__dict__ = dict(self.__dict__, **kwargs)  # 全部属性
        self.genir = Globals.genir
        self.symtab = Globals.symtab

    def print(self, space=0):
        print(f"{'  '*space}{self.__class__.__name__}")
        for key, val in self.__dict__.items():
            if isinstance(val, list):
                print(f"{'  '*(space+1)}{key}")
                for ast in val:
                    if (isinstance(ast, str)
                            or ast == None):
                        print(f"{'  '*(space+2)}{ast}")
                    else:
                        ast.print(space+2)
            elif (isinstance(val, str)
                  or isinstance(val, int)
                  or val == None):
                print(f"{'  '*(space+1)}{key}", end='')
                print(f': {val}')
            elif isinstance(val, SyntaxTree):
                print(f"{'  '*(space+1)}{key}")
                val.print(space+2)

    def gen(self):
        '''
        转换成目标代码
        '''

    def gen_list(self, l):
        '''
        将列表中的语法树转换成目标代码
        '''
        for _, val in enumerate(l):
            if isinstance(val, SyntaxTree):
                val.gen()

    def set_symtab(self):
        '''
        设置符号表
        '''

    def set_list_symtab(self, l):
        for _, val in enumerate(l):
            if isinstance(val, SyntaxTree):
                val.set_symtab()

    def get_size(self):
        '''
        获取大小
        '''
        return None

    def get_list_size(self,l):
        size=0
        for _, val in enumerate(l):
            if isinstance(val, SyntaxTree):
                size+=val.get_size()
        return size

    def get_array_info(self):
        '''
        获取数组信息
        '''