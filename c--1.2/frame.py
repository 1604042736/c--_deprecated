import interpreter


class Frame:
    '''栈帧, 负责运行字节码'''
    # 运算符操作
    operator = {
        'ADD': lambda a, b: a + b,
        'SUB': lambda a, b: a - b,
        'MUL': lambda a, b: a * b,
        'DIV': lambda a, b: a / b,
        'MOD': lambda a, b: a % b,
        'AND': lambda a, b: a and b,
        'OR': lambda a, b: a or b,
        'RSHIFT': lambda a, b: a >> b,
        'LSHIFT': lambda a, b: a << b,
        'EQ': lambda a, b: a == b,
        'NEQ': lambda a, b: a != b,
        'LT': lambda a, b: a < b,
        'LEQ': lambda a, b: a <= b,
        'GT': lambda a, b: a > b,
        'GEQ': lambda a, b: a >= b
    }
    IMPORT_PATH = 'Import'

    def __init__(self):
        self.globals = {}  # 全局变量
        self.locals = {}  # 局部变量
        self.callarg = {}  # 函数参数
        self.stack = []  # 运行时栈
        self.exception = []  # 异常信息
        self.tryop = []  # 异常处理的位置

    def loadbuiltins(self):
        '''加载内置函数或模块'''
        import builtinframe, os
        self.globals['sys'] = builtinframe.builtin_sys
        self.globals['len'] = builtinframe.builtin_len
        self.globals['input'] = builtinframe.builtin_input
        self.globals['print'] = builtinframe.builtin_print
        self.globals['int'] = builtinframe.builtin_int
        self.globals['str'] = builtinframe.builtin_str
        self.globals['math'] = builtinframe.builtin_math
        self.globals['abs'] = builtinframe.builtin_abs
        self.globals['max'] = builtinframe.builtin_max
        self.globals['min'] = builtinframe.builtin_min
        self.globals['bin'] = builtinframe.builtin_bin
        self.globals['oct'] = builtinframe.builtin_oct
        self.globals['hex'] = builtinframe.builtin_hex
        self.globals['open'] = builtinframe.builtin_open
        self.globals['os'] = os

    def eval(self, bytecode):
        try:
            self.loadbuiltins()
            return self._eval(bytecode)
        except:
            # 输出异常
            print(
                f"File {self.exception[1]['filename']}, Lineno {self.exception[1]['lineno']}, Linepos {self.exception[1]['linepos']}")
            print(f"\t{self.exception[1]['line'][:-1].strip()}")
            print(self.exception[0])

    def _eval(self, bytecode):
        op = 0
        nowarg = 0
        while op < len(bytecode.code):
            args = bytecode.code[op]
            try:
                if args[0] == 'LOAD_STRING':
                    self.stack.append(args[1])
                elif args[0] == 'LOAD_NUM':
                    self.stack.append(args[1])
                elif args[0] == 'LOAD_NAME':
                    # 先找局部变量, 再找全局变量, 都找不到引发错误
                    if args[1] in self.locals:
                        self.stack.append(self.locals[args[1]])
                    elif args[1] in self.globals:
                        self.stack.append(self.globals[args[1]])
                    else:
                        raise NameError(f"Name '{args[1]}' is not defined")
                elif args[0] == 'STORE_NAME':
                    self.locals[args[1]] = self.stack.pop()
                elif args[0] == 'LIST':
                    l = []
                    for i in range(args[1]):
                        l.insert(0, self.stack.pop())
                    self.stack.append(l)
                elif args[0] == 'PRINT':
                    print(self.stack.pop(), end='')
                elif args[0] in self.operator:
                    self.stack.append(self.operator[args[0]](self.stack.pop(), self.stack.pop()))
                elif args[0] == 'CMP':  # args[0]是op的偏移量
                    if not self.stack.pop():
                        op += args[1]
                elif args[0] == 'JMP':  # args[0]是op的偏移量
                    op += args[1]
                elif args[0] == 'LOAD_INDEX':
                    self.stack.append(self.stack.pop()[self.stack.pop()])
                elif args[0] == 'STORE_INDEX':
                    l = self.stack.pop()
                    index = self.stack.pop()
                    val = self.stack.pop()
                    l[index] = val
                elif args[0] == 'RETURN':
                    return self.stack.pop()
                elif args[0] == 'FUNCTION':
                    self.locals[args[1]] = bytecode.objects[args[1]]
                elif args[0] == 'CALL':
                    argslen = args[1]
                    func = self.stack.pop()
                    callarg = []
                    for i in range(argslen):  # 加载参数
                        callarg.insert(0, self.stack.pop())
                    if type(func).__name__ == 'ByteCode':
                        callframe = Frame()
                        callframe.globals = dict(self.globals,**self.locals)  # 被调用函数的globals为调用方的globals和locals的合集
                        callframe.callarg = callarg
                        self.stack.append(callframe.eval(func))
                    else:  # 如果是内置函数
                        self.stack.append(func(*callarg))
                elif args[0] == 'LOAD_ARG':
                    self.locals[args[1]] = self.callarg[nowarg]
                    nowarg += 1
                elif args[0] == 'INPUT':
                    if args[1]:
                        print(self.stack.pop(), end='')
                    self.stack.append(input())
                elif args[0] == 'INT':
                    self.stack.append(int(self.stack.pop()))
                elif args[0] == 'STR':
                    self.stack.append(str(self.stack.pop()))
                elif args[0] == 'SET_TRY':
                    self.tryop.append(op + args[1])  # 设置异常处理语句的位置
                elif args[0] == 'CLEAR_TRY':
                    self.tryop = []
                elif args[0] == 'DICT':
                    dictlen = args[1]
                    d = {}
                    for i in range(dictlen):
                        key = self.stack.pop()
                        val = self.stack.pop()
                        d[key] = val
                    self.stack.append(d)
                elif args[0] == 'CLASS':
                    self.locals[args[1]] = bytecode.objects[args[1]]
                elif args[0] == 'RETURN_SELF':
                    return self
                elif args[0] == 'LOAD_ATTR':
                    name = self.stack.pop()
                    if type(name).__name__ == 'Frame':
                        self.stack.append(name.locals[args[1]])
                    else:
                        self.stack.append(getattr(name, args[1]))
                elif args[0] == 'STORE_ATTR':
                    name = self.stack.pop()
                    if type(name).__name__ == 'Frame':
                        name.locals[args[1]] = self.stack.pop()
                elif args[0] == 'IMPORT':
                    filename = args[1] + '.c--'
                    inter = interpreter.Interpreter()
                    try:
                        # 先从程序路径找
                        with open(f'{filename}', encoding='utf-8')as f:
                            inter.interpret(f.read(), 'File', filename=filename)
                    except:
                        # 再从默认路径找
                        with open(f'{self.IMPORT_PATH}\\{filename}', encoding='utf-8')as f:
                            inter.interpret(f.read(), 'File', filename=filename)
                    self.locals[args[1]] = inter.frame
                elif args[0] == 'LEN':
                    self.stack.append(len(self.stack.pop()))
                elif args[0] == 'MERGE':  # 合并
                    self.locals = dict(self.locals,**self.stack.pop().locals)
            except Exception as e:
                self.exception = [type(e).__name__ + ': ' + str(e), args[-1]]
                if self.tryop:  # 有对应的异常处理语句
                    op = self.tryop[0]
                else:
                    raise Exception
            op += 1
