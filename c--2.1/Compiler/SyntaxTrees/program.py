from syntaxtree import *

class Program(SyntaxTree):
    '''
    整个程序
    '''

    def __init__(self, *args, **kwargs) -> None:
        self.body: list = []  # 主体
        super().__init__(*args, **kwargs)

    def gen(self):
        self.gen_list(self.body)

    def set_symtab(self):
        self.set_list_symtab(self.body)
