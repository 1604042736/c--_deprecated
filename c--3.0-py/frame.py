class Frame:
    '''
    栈帧
    '''

    def __init__(self):
        self.locals = {}  # 局部变量
        self.globals = {}  # 全局变量
        self.stack = []  # 栈
