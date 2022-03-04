from syntaxtree import *

class Pointer(SyntaxTree):
    '''
    指针
    '''

    def __init__(self, *args, **kwargs) -> None:
        self.type: SyntaxTree = None  # 类型
        super().__init__(*args, **kwargs)