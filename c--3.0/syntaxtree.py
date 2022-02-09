import operator
from frame import Frame
from exception import NameNotFoundException, ReturnException, BreakException, ContinueException, JumpException

class SyntaxTree:
    '''
    语法树的基类
    '''
    opmap = {
        '+': operator.add,
        '-': operator.sub,
        '*': operator.mul,
        '/': operator.truediv,
        '%': operator.mod,
        '&': operator.and_,
        '^': operator.xor,
        '|': operator.or_,
        '==': operator.eq,
        '>=': operator.ge,
        '<=': operator.le,
        '>': operator.gt,
        '<': operator.lt,
        '!=': operator.ne,
        'and': lambda a, b: a and b,
        'or': lambda a, b: a or b
    }
    memory = []

    def __init__(self, *args, **kwargs):
        self.envir = kwargs['parser'].envir
        self.lineno = kwargs['parser'].lexer.lineno
        self.lexer = kwargs['parser'].lexer
        #为了兼容旧版本python所以不使用 | 运算符
        self.__dict__ = dict(self.__dict__, **kwargs)

    def print(self, space=0):
        '''
        输出语法树
        '''
        print('  '*space, end='')
        print(self.__class__.__name__)
        for key, val in self.__dict__.items():
            if key in ('_values', '_class', 'envir', 'lineno', 'runcount', '_attr', 'lexer', 'parser'):
                continue
            print('  '*(space+1), end='')
            print(key, end='')
            if isinstance(val, SyntaxTree):
                print()
                val.print(space+2)
            elif isinstance(val, list):
                print()
                for t in val:
                    if isinstance(t, str):
                        print("  "*(space+2)+t)
                    else:
                        t.print(space+2)
            else:
                #print(':', list(str(val)))
                print(':', val)

    def copy(self):
        '''
        复制
        '''
        # 既能保持原有的属性不变，后续的操作又不会对原来的属性受到干扰
        # 比如,如果用deepcopy,envir的地址会发生改变,但我们不希望它发生改变
        return self.__class__(**self.__dict__)

    def run(self):
        '''
        运行
        '''

    def run_list(self, l):
        '''
        运行列表
        '''
        for i, t in enumerate(l):
            if isinstance(t, Unkown):
                t.run(self, l, i)
            else:
                t.run()

    def load_const(self, a):
        '''
        加载常量
        '''
        self.envir.curframe.stack.append(a)

    def load_name(self, a):
        '''
        加载变量
        '''
        # print(self.envir.curframe)
        try:
            self.envir.curframe.stack.append(self.envir.curframe.locals[a])
        except KeyError:
            try:
                self.envir.curframe.stack.append(
                    self.envir.curframe.globals[a])
            except KeyError:
                try:
                    self.envir.curframe.stack.append(
                        self.envir.curframe.builtins[a])
                except KeyError:
                    raise NameNotFoundException(
                        self.lexer.filename, self.lexer.lines[self.lexer.lineno-1], self.lexer.lineno-1, a)

    def store_name(self, a):
        '''
        将栈顶赋给变量
        '''
        val = self.envir.curframe.stack.pop()
        self.envir.curframe.locals[a] = val

    def doop(self, a):
        '''
        计算
        '''
        right = self.envir.curframe.stack.pop()
        left = self.envir.curframe.stack.pop()
        self.envir.curframe.stack.append(self.opmap[a](left, right))
        # print(a,left,right,self.get_top())

    def is_false(self):
        '''
        栈顶是否为假
        '''
        return not self.envir.curframe.stack.pop()

    def is_true(self):
        '''
        栈顶是否为真
        '''
        return self.envir.curframe.stack.pop()

    def load_val(self, a):
        '''
        直接添加
        '''
        self.envir.curframe.stack.append(a)

    def get_top(self):
        '''
        获取栈顶元素
        '''
        return self.envir.curframe.stack[-1]

    def pop(self):
        return self.envir.curframe.stack.pop()

    def load_sub(self):
        '''
        获取索引
        '''
        cls = self.envir.curframe.stack.pop()
        slc = self.envir.curframe.stack.pop()
        self.load_val(cls[slc])

    def store_sub(self):
        '''
        存储索引
        '''
        cls = self.envir.curframe.stack.pop()
        slc = self.envir.curframe.stack.pop()
        val = self.envir.curframe.stack.pop()
        cls[slc] = val

    def call(self, a):
        '''
        调用函数
        '''
        func = self.pop()
        return_val = None
        # print(self.envir.curframe)
        # print(self.envir)
        if isinstance(func, FunctionDef):  # 不是内置函数
            try:
                args = []
                for i in range(a):
                    args.append(self.pop())
                args = args[::-1]
                func(*args)
            except ReturnException:
                return_val = self.pop()
            self.envir.pop_frame()
        else:  # 是内置函数或者类
            args = []
            for i in range(a):
                args.append(self.pop())
            args = args[::-1]
            return_val = func(*args)
        return return_val

    def load_attr(self, a):
        '''
        获取属性
        '''
        val = self.pop()
        self.load_val(getattr(val, a))

    def store_attr(self, a):
        '''
        存储属性
        '''
        val = self.pop()
        setval = self.pop()
        setattr(val, a, setval)

    def make_list(self, a):
        '''
        创建列表
        '''
        l = []
        for i in range(a):
            l.insert(0, self.pop)
        self.load_val(l)

    def make_dict(self, a):
        '''
        创建字典
        '''
        d = {}
        for i in range(a):
            d[self.pop()] = self.pop()
        self.load_val(d)


class Namespace(SyntaxTree):
    def run(self):
        for t in self.body:
            t.run()


class Expr(SyntaxTree):
    def run(self):
        self.exp.run()
        self.pop()


class Assign(SyntaxTree):
    def run(self):
        self.value.run()
        for t in self.targets:
            t.run()


class BoolOp(SyntaxTree):
    def run(self):
        self.values[0].run()
        for i in range(1, len(self.values)):
            self.values[i].run()
            self.doop(self.op)


class BinOp(SyntaxTree):
    def run(self):
        self.left.run()
        self.right.run()
        self.doop(self.op)


class Constant(SyntaxTree):
    def run(self):
        self.load_const(self.value)


class Name(SyntaxTree):
    def run(self):
        if self.mode == 'load':
            self.load_name(self.id)
        elif self.mode == 'store':
            self.store_name(self.id)


class Compare(SyntaxTree):
    def run(self):
        self.left.run()
        for i in range(len(self.ops)):
            op = self.ops[i]
            comparator = self.comparators[i]
            comparator.run()
            a = self.get_top()
            self.doop(op)
            if self.is_false():
                self.load_const(False)
                return
            if i < len(self.ops)-1:
                self.load_val(a)
        self.load_const(True)


class Subscript(SyntaxTree):
    def run(self):
        self.slice.run()
        self.value.run()
        if self.mode == 'load':
            self.load_sub()
        elif self.mode == 'store':
            self.store_sub()


class Call(SyntaxTree):
    def run(self):
        for t in self.args:
            t.run()
        self.func.run()
        self.load_val(self.call(len(self.args)))


class Attribute(SyntaxTree):
    def run(self):
        self.value.run()
        if self.mode == 'load':
            self.load_attr(self.attr)
        elif self.mode == 'store':
            self.store_attr(self.attr)


class If(SyntaxTree):
    def run(self):
        self.exp.run()
        if self.is_true():
            # 用到了该代码块再解析
            self.run_list(self.body)
        else:
            self.run_list(self.elses)


class Unkown(SyntaxTree):
    def run(self, pt, l, i):
        from lexer import Lexer
        from _parser import Parser
        filename = self.lexer.filename
        lexer = Lexer(
            self.lexer.lines, filename, startlineno=self.startlineno, endlineno=self.endlineno)
        parser = Parser(lexer, self.envir)
        while parser.token != '<None>' and self.indent == parser.indent:
            global_lineno = lexer.lineno
            # 此次的信息
            # 加入lexer.filename以防干扰
            msg = (lexer.filename, global_lineno)
            try:
                p = parser.sentence()
                if msg not in SyntaxTree.memory:  # 没有保存
                    l.insert(i, p)  # 将语句添加到语法树对应的列表中
                    SyntaxTree.memory.append(msg)
            except JumpException as e:
                if msg not in SyntaxTree.memory:
                    l.insert(i, e.ast)
                    SyntaxTree.memory.append(msg)
                e.ast.runcount = 1  # 因为之前已经运行过一次
                # 全部解析完
                if self.startlineno+1 >= self.endlineno and isinstance(l[-1], Unkown):
                    l.pop()
                raise e
            # 更新信息
            self.startlineno = self.get_lineno(lexer.lines, lexer.lineno)
            i += 1
            # print(lexer.lineno,self.endlineno)
            # pt.print()
        l.pop(i)  # 全部解析完了

    def get_lineno(self, lines, lineno):
        '''
        获得新的行
        '''
        newlineno = lineno
        # print(lines,lineno,self.shift,self.endlineno,list(self.indent))
        while newlineno < len(lines) and self.indent != self.lexer.get_indent(lines[newlineno]):
            newlineno += 1
        return newlineno


class While(SyntaxTree):
    def run(self):
        self.exp.run()
        while self.is_true():
            try:
                self.run_list(self.body)
            except BreakException:
                break
            except ContinueException:
                pass
            self.exp.run()


class Break(SyntaxTree):
    def run(self):
        raise BreakException(self, 'break不应该在这里出现')


class Continue(SyntaxTree):
    def run(self):
        raise ContinueException(self, 'continue不应该在这里出现')


class FunctionDef(SyntaxTree):
    def __init__(self, *args, **kwargs):
        self.type = 'normal'  # 函数类型(普通函数,方法)
        super().__init__(*args, **kwargs)

    def run(self):
        self.load_val(self)
        self.store_name(self.name)
        self.runcount = 0
        self._values = dict(self.envir.curframe.globals, **
                            self.envir.curframe.locals)  # 函数所在作用域的全局变量和局部变量

    def __call__(self, *args):
        # print(self.envir,self.envir.frames)
        # self.print()
        # print(self.envir.curframe)
        # print(self.envir)
        frame = Frame()
        frame.globals = self._values
        # print(frame.globals)
        self.envir.add_frame(frame)
        if self.type == 'method':
            self.load_val(self.instance)
            self.store_name(self.args[0].id)
        for i, arg in enumerate(self.args):
            if self.type == 'method' and i == 0:
                continue
            if isinstance(arg, Name):
                if self.type == 'method':  # 如果是方法，实参比形参少1个
                    self.load_val(args[i-1])
                else:
                    self.load_val(args[i])
                self.store_name(arg.id)
        # print(self.envir.curframe)
        self.run_list(self.body)


class Return(SyntaxTree):
    def run(self):
        self.value.run()
        raise ReturnException(self, 'return不应该在这里出现')


class List(SyntaxTree):
    def run(self):
        for i in self.list:
            i.run()
        self.make_list(len(self.list))


class Dict(SyntaxTree):
    def run(self):
        for i in range(len(self.keys)):
            self.keys[i].run()
            self.values[i].run()
        self.make_dict(len(self.keys))


class Try(SyntaxTree):
    def run(self):
        try:
            self.run_list(self.body)
        except Exception as e:
            result = []  # 异常处理情况
            for t in self.handles:
                result.append(t.run(e))
            if not any(result):  # 有一个处理好了
                raise e


class ExceptionHandle(SyntaxTree):
    def run(self, e):
        if self.type == None:
            self.run_list(self.body)
            return True
        else:
            self.type.run()
            if isinstance(e, self.pop()):
                return True
        return False


class Class(SyntaxTree):
    def run(self):
        # self.print()
        self.load_val(self)
        self.store_name(self.name)
        # 运行类,获取类的属性
        self._attr = {}

        # 获取基类属性
        for base in self.bases:
            base.run()
            _class = self.pop()
            self.__dict__ = dict(self.__dict__, **_class.get_attr())

        frame = Frame()
        frame.globals = dict(self.envir.curframe.globals,
                             **self.envir.curframe.locals)
        self.envir.add_frame(frame)
        self.run_list(self.body)
        # 去除一些不必要的东西
        frame.locals.pop('globals')
        frame.locals.pop('stack')
        frame.locals.pop('builtins')
        self.__dict__ = dict(self.__dict__, **frame.locals)
        self.envir.pop_frame()

    def get_attr(self):
        '''
        获取属性,用于基类
        '''
        attr = {}
        for key, val in self.__dict__.items():
            if key not in ('name',):
                attr[key] = val
        return attr

    def __call__(self, *args):
        '''
        创建类的实例
        '''
        instance = Class(parser=self.parser)
        # self.__dict__与instance.__dict__中的内容地址一样
        # 但是他们的地址不能一样,所以不能用deepcopy
        for key, val in self.__dict__.items():
            if isinstance(val, FunctionDef):
                instance.__dict__[key] = val.copy()
                # 绑定函数与实例
                instance.__dict__[key].type = 'method'
                instance.__dict__[key].instance = instance
            else:
                instance.__dict__[key] = val
        return instance


class Import(SyntaxTree):
    def run(self):
        if isinstance(self.name, Name):
            namespace = self.import_namespace(self.name.id)
            self.load_val(namespace)
            self.store_name(self.name.id)
        elif isinstance(self.name, Attribute):  # 形如import a.b.c
            self.import_fromnamespace(self.name)

    def import_fromnamespace(self, name):
        try:
            name.run()
            self.store_name(self.name.attr)
        except NameNotFoundException as e:
            namespace_name = e.name  # 锁定namespace的内容
            # 解析
            namespace = self.import_namespace(namespace_name)
            self.load_val(namespace)
            self.store_name(namespace_name)
            # 加载
            name.run()
            self.store_name(self.name.attr)

    def import_namespace(self, name):
        '''
        包含一个namespace
        '''
        frame = Frame()
        frame.globals = dict(self.envir.curframe.globals,
                             **self.envir.curframe.locals)
        self.envir.add_frame(frame)
        _, namespace = _compile(name+'.txt', _envir=self.envir)
        self.envir.pop_frame()
        # namespace.print()
        # 一下跟class类似
        frame.locals.pop('globals')
        frame.locals.pop('stack')
        frame.locals.pop('builtins')
        namespace.__dict__ = dict(namespace.__dict__, **frame.locals)
        # print(namespace.envir,namespace.envir.frames,envir,envir.frames,self.envir.frames)
        return namespace

def _compile(filename, debug=False, _envir=None):
    from environment import Environment
    from exception import CException
    from lexer import Lexer
    from _parser import Parser
    with open(filename, encoding='utf-8')as file:
        lines = file.readlines()
        envir = _envir if _envir else Environment()
        lexer = Lexer(lines, filename)
        parser = Parser(lexer, envir)
        try:
            t = parser.start()
            if debug:
                t.print()
            return envir, t
        except CException as e:
            e.print()
