import operator
from frame import Frame


def haverun(func):
    '''
    已经运行过
    '''
    def wrap(*args):
        if args[0].runcount == -1:
            args[0].runcount = 1
            return
        try:
            a = func(*args)
        except Exception as e:
            args[0].runcount += 1
            raise e
        args[0].runcount += 1
        return a
    return wrap

class JumpException(Exception):
    def __init__(self,*args):
        super().__init__(*args)
        self.ast=args[0]


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
    state = []
    memory=[]
    def __init__(self, *args, **kwargs):
        self.runcount = 0
        self.__dict__ |= kwargs

    def print(self, space=0):
        '''
        输出语法树
        '''
        print('  '*space, end='')
        print(self.__class__.__name__)
        for key, val in self.__dict__.items():
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
                print(':',val)

    def run(self):
        '''
        运行
        '''

    def run_list(self,l):
        '''
        运行列表
        '''
        for i,t in enumerate(l):
            if isinstance(t,Unkown):
                t.run(self,l,i)
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
        try:
            self.envir.curframe.stack.append(self.envir.curframe.locals[a])
        except KeyError:
            self.envir.curframe.stack.append(self.envir.curframe.globals[a])

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
        slc = self.envir.curframe.stack.pop()
        cls = self.envir.curframe.stack.pop()
        return cls[slc]

    def store_sub(self):
        '''
        存储索引
        '''
        val = self.envir.curframe.stack.pop()
        slc = self.envir.curframe.stack.pop()
        cls = self.envir.curframe.stack.pop()
        cls[slc] = val

    def call(self):
        '''
        调用函数
        '''
        SyntaxTree.state.append('call')
        func = self.pop()
        return_val = None
        #func.print()
        try:
            func.run()
        except ReturnException:
            return_val = self.pop()
        SyntaxTree.state.pop()
        self.envir.pop_frame()
        return return_val


class Sentence(SyntaxTree):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.runcount = -2  # 语句在解析时就已运行,设置成-2防止重复运行


class Namespace(Sentence):
    @haverun
    def run(self):
        for t in self.body:
            t.run()


class Expr(Sentence):
    @haverun
    def run(self):
        self.exp.run()


class Assign(Sentence):
    @haverun
    def run(self):
        self.value.run()
        for t in self.targets:
            t.run()


class BoolOp(SyntaxTree):
    @haverun
    def run(self):
        self.values[0].run()
        for i in range(1, len(self.values)):
            self.values[i].run()
            self.doop(self.op)


class BinOp(SyntaxTree):
    @haverun
    def run(self):
        self.left.run()
        self.right.run()
        self.doop(self.op)


class Constant(SyntaxTree):
    @haverun
    def run(self):
        self.load_const(self.value)


class Name(SyntaxTree):
    @haverun
    def run(self):
        if self.mode == 'load':
            self.load_name(self.id)
        elif self.mode == 'store':
            self.store_name(self.id)


class Compare(SyntaxTree):
    @haverun
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
    @haverun
    def run(self):
        self.slice.run()
        self.value.run()
        if self.mode == 'load':
            self.load_sub()
        elif self.mode == 'store':
            self.store_sub()


class Call(SyntaxTree):
    @haverun
    def run(self):
        for t in self.args:
            t.run()
        self.func.run()
        self.load_val(self.call())


class Attribute(SyntaxTree):
    pass


class If(Sentence):
    @haverun
    def run(self):
        self.exp.run()
        if self.is_true():
            # 用到了该代码块再解析
            self.run_list(self.body)
        else:
            self.run_list(self.elses)


class Unkown(SyntaxTree):
    @haverun
    def run(self,pt,l,i):
        from lexer import Lexer
        from parser import Parser
        filename = self.lexer.filename
        lexer = Lexer(
            self.lexer.lines[self.startlineno:self.endlineno], filename, shift=self.shift)
        parser = Parser(lexer, self.envir)
        while parser.token!='<None>' and self.indent==parser.indent:
            try:
                p=parser.sentence()
                if lexer.lineno+lexer.shift not in SyntaxTree.memory:
                    l.insert(i,p)   #将语句添加到语法树对应的列表中
                    SyntaxTree.memory.append(lexer.lineno+lexer.shift)
            except JumpException as e:
                if lexer.lineno+lexer.shift not in SyntaxTree.memory:
                    l.insert(i,e.ast)
                    SyntaxTree.memory.append(lexer.lineno+lexer.shift)
                e.ast.runcount=1    #因为之前已经运行过一次
                #lexer.lineno相对于新的lines
                #self.endlineno相对于原来的
                if lexer.lineno+lexer.shift>=self.endlineno and isinstance(l[-1],Unkown):    #全部解析完
                    l.pop()
                raise e
            #更新信息
            self.startlineno=self.get_lineno(lexer.lines,lexer.lineno)
            self.shift=self.startlineno+lexer.shift
            i+=1
        l.pop(i)    #全部解析完了

    def get_lineno(self,lines,lineno):
        '''
        获得新的行
        '''
        #lineno是相对于新的lines
        #newlineno是相对于原来的lines
        #所以之后要加上偏移
        newlineno=lineno
        while newlineno<len(lines)and self.indent!=self.lexer.get_indent(lines[newlineno]):
            newlineno+=1
        return newlineno+self.shift


class While(Sentence):
    @haverun
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


class Break(Sentence):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.runcount = -2

    @haverun
    def run(self):
        raise BreakException(self)


class Continue(Sentence):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.runcount = -2

    @haverun
    def run(self):
        raise ContinueException(self)


class FunctionDef(Sentence):
    @haverun
    def run(self):
        if self.runcount == -2:  # 第一次运行
            self.load_val(self)
            self.store_name(self.name)
            self.runcount = 0
            self.values = self.envir.curframe.globals | self.envir.curframe.locals
        elif 'call' in SyntaxTree.state:
            arglen = len(self.args)
            args = []
            for i in range(arglen):
                args.append(self.pop())
            args = args[::-1]
            frame = Frame()
            frame.globals = self.values
            self.envir.add_frame(frame)
            for i, arg in enumerate(self.args):
                if isinstance(arg, Name):
                    self.load_val(args[i])
                    self.store_name(arg.id)
            self.run_list(self.body)


class Return(Sentence):
    @haverun
    def run(self):
        self.value.run()
        raise ReturnException(self)


def compileandrun(func):
    '''
    编译后运行语法树
    '''
    def wrap(*args):
        tree = func(*args)
        tree.__dict__['envir'] = args[0].envir
        if 'donotrun' not in SyntaxTree.state and isinstance(tree, Sentence):  # 是语句并且可以运行
            tree.run()
        return tree
    return wrap