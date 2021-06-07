class SyntaxTree:
    '''语法树'''

    def __init__(self, token):
        self.token = token  # 文件信息

    def print(self, spaces=0, step='  '):
        print(f"{step * spaces}{type(self).__name__}")
        for key, val in self.__dict__.items():
            if key not in ['token']:
                print(f"{step * (spaces + 1)}{key}", end="")
                if val:
                    if isinstance(val, list):
                        print()
                        for t in val:
                            t.print(spaces + 2)
                    elif isinstance(val, str):
                        print(': ' + val)
                    else:
                        print()
                        val.print(spaces + 2)
                else:
                    print()

    def __str__(self):
        s, i = f'{type(self).__name__}(', 1
        for key, val in self.__dict__.items():
            if key not in ['token']:
                s += key + '='
                s += str(val)
                if i < len(self.__dict__) - 1:  # 还有未输出的参数
                    s += ', '
                i += 1
        s += ')'
        return s

    __repr__ = __str__
