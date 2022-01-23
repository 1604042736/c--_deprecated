from copy import deepcopy
import operator
from frame import Frame
from exception import ReturnException,BreakException,ContinueException,JumpException

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
    memory=[]
    def __init__(self, *args, **kwargs):
        self.runcount = 0
        self.envir=kwargs['parser'].envir
        self.lineno=kwargs['parser'].lexer.lineno
        self.lexer=kwargs['parser'].lexer
        kwargs.pop('parser')
        self.__dict__ |= kwargs

    def print(self, space=0):
        '''
        输出语法树
        '''
        print('  '*space, end='')
        print(self.__class__.__name__)
        for key, val in self.__dict__.items():
            if key in ('_values','_class','envir','lineno','runcount','attr','lexer'):
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
            try:
                self.envir.curframe.stack.append(self.envir.curframe.globals[a])
            except KeyError:
                self.envir.curframe.stack.append(self.envir.curframe.builtins[a])

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

    def call(self,a):
        '''
        调用函数
        '''
        func = self.pop()
        return_val = None
        if isinstance(func,FunctionDef):    #不是内置函数
            try:
                func.run(a)
            except ReturnException:
                return_val = self.pop()
            self.envir.pop_frame()
        else:   #是内置函数或者类
            args=[]
            for i in range(a):
                args.append(self.pop())
            args=args[::-1]
            return_val=func(*args)
        return return_val

    def load_attr(self,a):
        '''
        获取属性
        '''
        val=self.pop()
        self.load_val(getattr(val,a))

    def store_attr(self,a):
        '''
        存储属性
        '''
        val=self.pop()
        setval=self.pop()
        setattr(val,a,setval)

    def make_list(self,a):
        '''
        创建列表
        '''
        l=[]
        for i in range(a):
            l.insert(0,self.pop)
        self.load_val(l)

    def make_dict(self,a):
        '''
        创建字典
        '''
        d={}
        for i in range(a):
            d[self.pop()]=self.pop()
        self.load_val(d)


class Sentence(SyntaxTree):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.runcount = -2  # 语句在解析时就已运行,设置成-2防止重复运行


class Namespace(SyntaxTree):
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
        self.load_val(self.call(len(self.args)))


class Attribute(SyntaxTree):
    @haverun
    def run(self):
        self.value.run()
        if self.mode=='load':
            self.load_attr(self.attr)
        elif self.mode=='store':
            self.store_attr(self.attr)


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
            self.lexer.lines, filename, startlineno=self.startlineno,endlineno=self.endlineno)
        parser = Parser(lexer, self.envir)
        while parser.token!='<None>' and self.indent==parser.indent:
            global_lineno=lexer.lineno
            try:
                p=parser.sentence()
                if global_lineno not in SyntaxTree.memory:   #没有保存
                    l.insert(i,p)   #将语句添加到语法树对应的列表中
                    SyntaxTree.memory.append(global_lineno)
            except JumpException as e:
                if global_lineno not in SyntaxTree.memory:
                    l.insert(i,e.ast)
                    SyntaxTree.memory.append(global_lineno)
                e.ast.runcount=1    #因为之前已经运行过一次
                if self.startlineno+1>=self.endlineno and isinstance(l[-1],Unkown):    #全部解析完
                    l.pop()
                raise e
            #更新信息
            self.startlineno=self.get_lineno(lexer.lines,lexer.lineno)
            i+=1
            #print(lexer.lineno,self.endlineno)
            #pt.print()
        l.pop(i)    #全部解析完了

    def get_lineno(self,lines,lineno):
        '''
        获得新的行
        '''
        newlineno=lineno
        #print(lines,lineno,self.shift,self.endlineno,list(self.indent))
        while newlineno<len(lines)and self.indent!=self.lexer.get_indent(lines[newlineno]):
            newlineno+=1
        return newlineno


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
    @haverun
    def run(self):
        raise BreakException(self,'break不应该在这里出现')


class Continue(Sentence):
    @haverun
    def run(self):
        raise ContinueException(self,'continue不应该在这里出现')


class FunctionDef(Sentence):
    @haverun
    def run(self,arglen=-1):
        if self.runcount == -2:  # 第一次运行
            self.load_val(self)
            self.store_name(self.name)
            self.runcount = 0
            self._values = self.envir.curframe.globals | self.envir.curframe.locals #函数所在作用域的全局变量和局部变量
        elif arglen!=-1:    #是函数调用
            args = []
            #self.print()
            for i in range(arglen):
                args.append(self.pop())
            args = args[::-1]
            frame = Frame()
            frame.globals = self._values
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
        raise ReturnException(self,'return不应该在这里出现')

class List(SyntaxTree):
    @haverun
    def run(self):
        for i in self.list:
            i.run()
        self.make_list(len(self.list))

class Dict(SyntaxTree):
    @haverun
    def run(self):
        for i in range(len(self.keys)):
            self.keys[i].run()
            self.values[i].run()
        self.make_dict(len(self.keys))

class Try(Sentence):
    @haverun
    def run(self):
        try:
            self.run_list(self.body)
        except Exception as e:
            result=[]   #异常处理情况
            for t in self.handles:
                result.append(t.run(e))
            if not any(result): #有一个处理好了
                raise e

class ExceptionHandle(SyntaxTree):
    @haverun
    def run(self,e):
        if self.type==None:
            self.run_list(self.body)
            return True
        else:
            self.type.run()
            if isinstance(e,self.pop()):
                return True
        return False

class Class(Sentence):
    @haverun
    def run(self):
        #self.print()
        if self.runcount==-2:   #第一次运行(初始化)
            self.runcount=1
            self.load_val(self)
            self.store_name(self.name)
            #运行类,获取类的属性
            frame = Frame()
            frame.globals = self.envir.curframe.globals|self.envir.curframe.locals
            self.envir.add_frame(frame)
            self.run_list(self.body)
            self.attr=frame.locals
            #去除一些不必要的东西
            self.attr.pop('globals')
            self.attr.pop('stack')
            self.attr.pop('builtins')
            self.envir.pop_frame()

    def __call__(self):
        '''
        创建类的实例
        '''
        return self

    def __getattribute__(self, __name: str):
        if __name=='attr':
            try:
                return super().__getattribute__(__name)
            except:
                return {}
        if __name in self.attr:
            return self.attr[__name]
        return super().__getattribute__(__name)

    def __setattr__(self, __name: str, __value) -> None:
        if __name in self.attr:
            self.attr[__name]=__value
        return super().__setattr__(__name, __value)


def compileandrun(func):
    '''
    编译后运行语法树
    '''
    def wrap(*args):
        lineno=args[0].lexer.lineno #保存行
        tree = func(*args)
        if isinstance(tree, Sentence):  # 是语句并且可以运行
            tree.run()
        return tree
    return wrap