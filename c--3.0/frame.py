import builtins


class Frame:
    '''
    栈帧
    '''

    def __init__(self):
        import _parser
        self.globals = {}  # 全局变量
        self.builtins = builtins.__dict__  # 内置变量
        self.stack = []  # 栈
        self.locals = {'globals': self.globals,
                       'stack': self.stack,
                       'builtins': self.builtins,
                       'Parser':_parser.Parser}  # 局部变量
