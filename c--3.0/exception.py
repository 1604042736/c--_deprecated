class CException(Exception):
    '''
    c--中的异常基类
    '''

    def __init__(self, filename, line, lineno, msg) -> None:
        super().__init__(msg)
        self.filename = filename
        self.line = line
        self.lineno = lineno
        self.msg = msg

    def print(self):
        print(f'文件 "{self.filename}",行 {self.lineno+1}')
        print('\t'+self.line.lstrip().rstrip())
        print(f'{type(self).__name__}: {self.msg}')


class SyntaxException(CException):
    '''
    语法异常
    '''


class NameNotFoundException(CException):
    '''
    无法找到变量
    '''

    def __init__(self, filename, line, lineno, name) -> None:
        super().__init__(filename, line, lineno, f'找不到变量"{name}"')
        self.name = name  # 该变量名称


class JumpException(CException):
    def __init__(self, ast, msg):
        super().__init__(ast.lexer.filename,
                         ast.lexer.lines[ast.lexer.lineno], ast.lexer.lineno, msg)
        self.ast = ast  # 保存语法树


class BreakException(JumpException):
    '''
    循环中断
    '''


class ContinueException(JumpException):
    '''
    循环继续
    '''


class ReturnException(JumpException):
    '''
    返回值
    '''
