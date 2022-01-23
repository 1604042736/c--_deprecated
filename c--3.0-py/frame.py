class Frame:
    '''
    栈帧
    '''

    def __init__(self):
        self.globals = {}  # 全局变量
        self.builtins=__builtins__    #内置变量
        self.stack = []  # 栈
        self.locals = {'globals':self.globals,
                        'stack':self.stack,
                        'builtins':self.builtins}  # 局部变量