from globals import *
from symtab import *

class SyntaxTree:
    '''
    所有语法树的基类
    '''

    def __init__(self, *args, **kwargs) -> None:
        self.__dict__ = dict(self.__dict__, **kwargs)  # 全部属性
        self.genir = Globals.genir
        self.symtab = Globals.symtab

    def print(self, space=0):
        print(f"{'  '*space}{self.__class__.__name__}")
        for key, val in self.__dict__.items():
            if isinstance(val, list):
                print(f"{'  '*(space+1)}{key}")
                for ast in val:
                    if (isinstance(ast, str)
                            or ast == None):
                        print(f"{'  '*(space+2)}{ast}")
                    else:
                        ast.print(space+2)
            elif (isinstance(val, str)
                  or isinstance(val, int)
                  or val == None):
                print(f"{'  '*(space+1)}{key}", end='')
                print(f': {val}')
            elif isinstance(val, SyntaxTree):
                print(f"{'  '*(space+1)}{key}")
                val.print(space+2)

    def gen(self):
        '''
        转换成目标代码
        '''

    def gen_list(self, l):
        '''
        将列表中的语法树转换成目标代码
        '''
        for _, val in enumerate(l):
            if isinstance(val, SyntaxTree):
                val.gen()

    def set_symtab(self):
        '''
        设置符号表
        '''

    def set_list_symtab(self, l):
        for _, val in enumerate(l):
            if isinstance(val, SyntaxTree):
                val.set_symtab()

    def get_size(self):
        '''
        获取大小
        '''
        return None

    def get_list_size(self,l):
        size=0
        for _, val in enumerate(l):
            if isinstance(val, SyntaxTree):
                size+=val.get_size()
        return size

    def get_array_info(self):
        '''
        获取数组信息
        '''

class Program(SyntaxTree):
    '''
    整个程序
    '''

    def __init__(self, *args, **kwargs) -> None:
        self.body: list = []  # 主体
        super().__init__(*args, **kwargs)

    def gen(self):
        self.gen_list(self.body)

    def set_symtab(self):
        self.set_list_symtab(self.body)


class IntType(SyntaxTree):
    '''
    整型
    '''
    size=4
    def set_symtab(self,name):
        islocal=bool(self.symtab.curfunc)
        var=VarSym(name,self,offset=self.symtab.get_offset(),islocal=islocal)
        self.symtab.add_var(var)
        self.symtab.add_offset(self.size)

    def gen(self,var):
        if var.flag==1:
            self.genir.add_inst('sub','ecx',var.offset)
        elif var.flag==-1:
            self.genir.add_inst('add','ecx',var.offset+4)

    def get_size(self):
        return self.size


class CharType(SyntaxTree):
    '''
    字符型
    '''
    size=1


class FloatType(SyntaxTree):
    '''
    浮点型
    '''


class Pointer(SyntaxTree):
    '''
    指针
    '''

    def __init__(self, *args, **kwargs) -> None:
        self.type: SyntaxTree = None  # 类型
        super().__init__(*args, **kwargs)


class Array(SyntaxTree):
    '''
    数组
    '''

    def __init__(self, *args, **kwargs) -> None:
        self.type: SyntaxTree = None  # 类型
        self.level:list=[]
        super().__init__(*args, **kwargs)
        self.size=1
        self.basesize=self.type.size
        for i in self.level:
            self.size*=int(i)
        self.size*=self.type.size

    def set_symtab(self,name):
        islocal=bool(self.symtab.curfunc)
        var=VarSym(name,self,offset=self.symtab.get_offset(),islocal=islocal,address=True)
        self.var=var
        self.symtab.add_var(var)
        self.symtab.add_offset(self.size)

    def get_array_info(self):
        ins='sub'
        if self.var.offset<0:
            ins='add'
        return ArrayInfo(levels=self.level,size=self.size,basicsize=self.basesize,count=len(self.level)-1,ins=ins)

    def gen(self,var):
        if var.flag==1:
            self.genir.add_inst('sub','ecx',var.offset)
        elif var.flag==-1:
            self.genir.add_inst('add','ecx',var.offset+4)

    def get_size(self):
        return self.basesize

class ArrayInfo:
    '''
    数组信息
    '''
    def __init__(self,**kwargs) -> None:
        self.levels=[]    #维数
        self.level=0
        self.index=0
        self.basicsize=0    #数组单位大小
        self.size=0
        self.count=0
        self.ins='' #指令
        self.__dict__|=kwargs
        assert self.basicsize
        assert self.size
        self.level=self.levels[self.index]

    def next(self):
        self.index+=1
        if self.index<len(self.levels):
            self.level=self.levels[self.index]
            self.count-=1
            leftlevel=self.levels[self.index+1:]    #剩下的level
            self.size=1
            for i in leftlevel:
                self.size*=i
            self.size*=self.basicsize
        else:
            self.level=0
            self.size=self.basicsize


class VarDef(SyntaxTree):
    '''
    变量声明
    '''

    def __init__(self, *args, **kwargs) -> None:
        self.type: SyntaxTree = None  # 类型
        self.names: str = []  # 名称
        self.values: SyntaxTree = []  # 初始值
        super().__init__(*args, **kwargs)

    def set_symtab(self):
        for name,value in zip(self.names,self.values):
            self.type.set_symtab(name)

    def gen(self):
        for name,value in zip(self.names,self.values):
            if value:
                value.gen()
                Name(id=name,mode=Store()).gen()


class Or(SyntaxTree):
    '''
    或
    '''
    def gen(self):
        self.genir.comment('->Or')
        truelabel=self.genir.get_label()
        falselabel=self.genir.get_label()
        endlabel=self.genir.get_label()

        self.genir.add_inst('pop','ebx')
        self.genir.add_inst('pop','eax')
        self.genir.add_inst('cmp','eax','0')
        self.genir.add_inst('jne',truelabel)
        self.genir.add_inst('cmp','ebx','0')
        self.genir.add_inst('je',falselabel)
        self.genir.add_label(truelabel)
        self.genir.add_inst('push',1)
        self.genir.add_inst('jmp',endlabel)
        self.genir.add_label(falselabel)
        self.genir.add_inst('push',0)
        self.genir.add_label(endlabel)
        self.genir.comment('<-Or')

class And(SyntaxTree):
    '''
    与
    '''
    def gen(self):
        truelabel=self.genir.get_label()
        falselabel=self.genir.get_label()
        endlabel=self.genir.get_label()

        self.genir.add_inst('pop','ebx')
        self.genir.add_inst('pop','eax')
        self.genir.add_inst('cmp','eax','0')
        self.genir.add_inst('je',falselabel)
        self.genir.add_inst('cmp','ebx','0')
        self.genir.add_inst('je',falselabel)
        self.genir.add_label(truelabel)
        self.genir.add_inst('push',1)
        self.genir.add_inst('jmp',endlabel)
        self.genir.add_label(falselabel)
        self.genir.add_inst('push',0)
        self.genir.add_label(endlabel)


class BitOr(SyntaxTree):
    pass


class BitXor(SyntaxTree):
    pass


class BitAnd(SyntaxTree):
    pass


class Gt(SyntaxTree):
    def gen(self):
        self.genir.add_inst('pop','ebx')
        self.genir.add_inst('pop','eax')
        self.genir.add_inst('cmp','eax','ebx')
        falselabel=self.genir.get_label()
        endlabel=self.genir.get_label()
        self.genir.add_inst('jle',falselabel)
        self.genir.add_inst('push',1)
        self.genir.add_inst('jmp',endlabel)
        self.genir.add_label(falselabel)
        self.genir.add_inst('push',0)
        self.genir.add_label(endlabel)


class Geq(SyntaxTree):
    def gen(self):
        self.genir.add_inst('pop','ebx')
        self.genir.add_inst('pop','eax')
        self.genir.add_inst('cmp','eax','ebx')
        falselabel=self.genir.get_label()
        endlabel=self.genir.get_label()
        self.genir.add_inst('jle',falselabel)
        self.genir.add_inst('push',1)
        self.genir.add_inst('jmp',endlabel)
        self.genir.add_label(falselabel)
        self.genir.add_inst('push',0)
        self.genir.add_label(endlabel)


class Lt(SyntaxTree):
    def gen(self):
        self.genir.add_inst('pop','ebx')
        self.genir.add_inst('pop','eax')
        self.genir.add_inst('cmp','eax','ebx')
        falselabel=self.genir.get_label()
        endlabel=self.genir.get_label()
        self.genir.add_inst('jge',falselabel)
        self.genir.add_inst('push',1)
        self.genir.add_inst('jmp',endlabel)
        self.genir.add_label(falselabel)
        self.genir.add_inst('push',0)
        self.genir.add_label(endlabel)


class Leq(SyntaxTree):
    def gen(self):
        self.genir.add_inst('pop','ebx')
        self.genir.add_inst('pop','eax')
        self.genir.add_inst('cmp','eax','ebx')
        falselabel=self.genir.get_label()
        endlabel=self.genir.get_label()
        self.genir.add_inst('jg',falselabel)
        self.genir.add_inst('push',1)
        self.genir.add_inst('jmp',endlabel)
        self.genir.add_label(falselabel)
        self.genir.add_inst('push',0)
        self.genir.add_label(endlabel)


class Eq(SyntaxTree):
    def gen(self):
        self.genir.add_inst('pop','ebx')
        self.genir.add_inst('pop','eax')
        self.genir.add_inst('cmp','eax','ebx')
        falselabel=self.genir.get_label()
        endlabel=self.genir.get_label()
        self.genir.add_inst('jne',falselabel)
        self.genir.add_inst('push',1)
        self.genir.add_inst('jmp',endlabel)
        self.genir.add_label(falselabel)
        self.genir.add_inst('push',0)
        self.genir.add_label(endlabel)


class Neq(SyntaxTree):
    def gen(self):
        self.genir.add_inst('pop','ebx')
        self.genir.add_inst('pop','eax')
        self.genir.add_inst('cmp','eax','ebx')
        falselabel=self.genir.get_label()
        endlabel=self.genir.get_label()
        self.genir.add_inst('je',falselabel)
        self.genir.add_inst('push',1)
        self.genir.add_inst('jmp',endlabel)
        self.genir.add_label(falselabel)
        self.genir.add_inst('push',0)
        self.genir.add_label(endlabel)


class LShift(SyntaxTree):
    pass


class RShift(SyntaxTree):
    pass


class Add(SyntaxTree):
    def gen(self):
        self.genir.add_inst('pop','ebx')
        self.genir.add_inst('pop','eax')
        self.genir.add_inst('add','eax','ebx')
        self.genir.add_inst('push','eax')


class Sub(SyntaxTree):
    def gen(self):
        self.genir.add_inst('pop','ebx')
        self.genir.add_inst('pop','eax')
        self.genir.add_inst('sub','eax','ebx')
        self.genir.add_inst('push','eax')


class Mul(SyntaxTree):
    def gen(self):
        self.genir.add_inst('pop','ebx')
        self.genir.add_inst('pop','eax')
        self.genir.add_inst('mul','ebx')
        self.genir.add_inst('push','eax')


class Div(SyntaxTree):
    def gen(self):
        self.genir.add_inst('pop','ebx')
        self.genir.add_inst('pop','eax')
        self.genir.add_inst('div','ebx')
        self.genir.add_inst('push','eax')


class Mod(SyntaxTree):
    pass


class BoolOp(SyntaxTree):
    def __init__(self, *args, **kwargs) -> None:
        self.op: SyntaxTree = None
        self.values: list = []
        super().__init__(*args, **kwargs)

    def gen(self):
        endlabel=self.genir.get_label()
        finallabel=self.genir.get_label()
        self.values[0].gen()
        for i in range(1,len(self.values)):
            val=self.values[i]
            val.gen()
            self.op.gen()
            self.genir.add_inst('pop','eax')
            self.genir.add_inst('cmp','eax',0)
            self.genir.add_inst('je',finallabel)
            self.genir.add_inst('push',1)
        self.genir.add_inst('jmp',endlabel)
        self.genir.add_label(finallabel)
        self.genir.add_inst('push',0)
        self.genir.add_label(endlabel)

    def get_size(self):
        return self.values[0].get_size()

class BinOp(SyntaxTree):
    def __init__(self, *args, **kwargs) -> None:
        self.left: SyntaxTree = None
        self.op: SyntaxTree = None
        self.right: SyntaxTree = None
        super().__init__(*args, **kwargs)

    def gen(self):
        self.left.gen()
        self.right.gen()
        self.op.gen()

    def get_size(self):
        return self.left.get_size()


class Compare(SyntaxTree):
    def __init__(self, *args, **kwargs) -> None:
        self.left: SyntaxTree = None
        self.ops: list = []
        self.comparators: list = []
        super().__init__(*args, **kwargs)

    def gen(self):
        endlabel=self.genir.get_label()
        finallabel=self.genir.get_label()
        self.left.gen()
        for i in range(len(self.ops)):
            op = self.ops[i]
            comparator = self.comparators[i]
            comparator.gen()
            #复制
            self.genir.add_inst('pop','edx')
            self.genir.add_inst('push','edx')
            op.gen()
            self.genir.add_inst('pop','eax')
            self.genir.add_inst('cmp','eax',0)
            self.genir.add_inst('je',finallabel)
            if i < len(self.ops)-1:
                self.genir.add_inst('push','edx')
        self.genir.add_inst('push',1)
        self.genir.add_inst('jmp',endlabel)
        self.genir.add_label(finallabel)
        self.genir.add_inst('push',0)
        self.genir.add_label(endlabel)

    def get_size(self):
        return self.left.get_size()


class Load(SyntaxTree):
    def gen(self,var=None):
        target='[ecx]'
        if var and var.address:
            target='ecx'
        if not var or var.islocal:
            self.genir.add_inst('mov','eax',target)
            self.genir.add_inst('push','eax')

class Store(SyntaxTree):
    def gen(self,var=None):
        self.genir.add_inst('pop','eax')
        target='[ecx]'
        if var and var.address:
            target='ecx'
        if not var or var.islocal:
            self.genir.add_inst('mov',target,'eax')


class Subscript(SyntaxTree):
    def __init__(self, *args, **kwargs) -> None:
        self.value: SyntaxTree = None
        self.slice: SyntaxTree = None
        self.mode: SyntaxTree = None
        super().__init__(*args, **kwargs)

    def set_symtab(self):
        self.value.set_symtab()
        self.slice.set_symtab()

    def gen(self):
        self.genir.comment('->Subscript')
        self.slice.gen()
        self.value.gen()
        self.genir.add_inst('pop','ecx')
        info=self.value.get_array_info()
        i=info.count
        level=info.level
        shift=info.basicsize
        ins=info.ins
        if i==0:
            self.genir.add_inst('pop','eax')
        else:
            self.genir.add_inst('pop','eax')
            self.genir.add_inst('push',level)
            self.genir.add_inst('pop','ebx')
            self.genir.add_inst('mul','ebx')
        self.genir.add_inst('push',shift)
        self.genir.add_inst('pop','ebx')
        self.genir.add_inst('mul','ebx')
        self.genir.add_inst(ins,'ecx','eax')
        self.mode.gen()
        self.genir.comment('<-Subscript')

    def get_array_info(self):
        info= self.value.get_array_info()
        info.next()
        return info

    def get_size(self):
        info= self.value.get_array_info()
        info.next()
        return info.size

class Call(SyntaxTree):
    def __init__(self, *args, **kwargs) -> None:
        self.func: str = ''
        self.args: list = []
        super().__init__(*args, **kwargs)

    def gen(self):
        if self.func not in self.symtab.functions:
            self.genir.add_extern(self.func)
        self.gen_list(self.args[::-1])
        self.genir.add_inst('call', self.func)
        self.genir.add_inst('add', 'esp', self.get_list_size(self.args))
        if self.func in self.symtab.functions:  #无法确认函数类型或函数类型为void的时侯不push
            self.genir.add_inst('push', 'eax')

    def set_symtab(self):
        self.set_list_symtab(self.args)

    def get_size(self):
        func=self.symtab.functions[self.func]
        return func.return_type.size

class Attribute(SyntaxTree):
    def __init__(self, *args, **kwargs) -> None:
        self.value: SyntaxTree = None
        self.attr: str = ''
        self.mode: SyntaxTree = None
        super().__init__(*args, **kwargs)


class Name(SyntaxTree):
    def __init__(self, *args, **kwargs) -> None:
        self.id: SyntaxTree = None
        self.mode: SyntaxTree = None
        super().__init__(*args, **kwargs)

    def gen(self):
        self.genir.comment('->Name')
        name=self.symtab.get_var(self.id)
        self.genir.add_inst('mov','ecx','ebp')
        name.type.gen(name)
        self.mode.gen(name)
        self.genir.comment('<-Name')

    def get_size(self):
        name=self.symtab.get_var(self.id)
        return name.type.get_size()

    def get_array_info(self):
        name=self.symtab.get_var(self.id)
        return name.type.get_array_info()


class String(SyntaxTree):
    def __init__(self, *args, **kwargs) -> None:
        self.value = ''
        super().__init__(*args, **kwargs)
        self.value=self.value[1:-1]

    def gen(self):
        name = self.symtab.get_constant(self.value)
        self.genir.add_inst('push', name)

    def set_symtab(self):
        if self.value not in self.symtab.constant:
            self.symtab.add_constant(self.value,StringSym)

    def get_size(self):
        return len(self.value)


class Num(SyntaxTree):
    def __init__(self, *args, **kwargs) -> None:
        self.value = 0
        super().__init__(*args, **kwargs)

    def gen(self):
        self.genir.add_inst('push',self.value)

    def get_size(self):
        return IntType.size


class Assign(SyntaxTree):
    def __init__(self, *args, **kwargs) -> None:
        self.targets: list = []
        self.value: SyntaxTree = None
        super().__init__(*args, **kwargs)
        for target in self.targets:
            target.mode = Store()

    def gen(self):
        self.value.gen()
        self.gen_list(self.targets)

    def set_symtab(self):
        self.value.set_symtab()


class If(SyntaxTree):
    def __init__(self, *args, **kwargs) -> None:
        self.exp: SyntaxTree = None
        self.body: list = []
        self.elses: list = []
        super().__init__(*args, **kwargs)

    def gen(self):
        self.genir.comment('->If')
        self.exp.gen()
        jmplabel=self.genir.get_label()
        endlabel=self.genir.get_label()
        self.genir.add_inst('pop','eax')
        self.genir.add_inst('cmp','eax',0)
        self.genir.add_inst('je',jmplabel)
        self.gen_list(self.body)
        self.genir.add_inst('jmp',endlabel)
        self.genir.add_label(jmplabel)
        self.gen_list(self.elses)
        self.genir.add_label(endlabel)
        self.genir.comment('<-If')

    def set_symtab(self):
        self.exp.set_symtab()
        self.set_list_symtab(self.body)


class Function(SyntaxTree):
    def __init__(self, *args, **kwargs) -> None:
        self.type: SyntaxTree = None
        self.name: str = ''
        self.args: list = []
        self.body: list = []
        super().__init__(*args, **kwargs)

    def gen(self):
        if self.body:
            func=self.symtab.functions[self.name]
            self.symtab.enterfunc(func)
            self.genir.add_label(self.name)
            self.genir.add_inst('push', 'ebp')
            self.genir.add_inst('mov', 'ebp', 'esp')
            self.genir.add_inst('sub','esp',func.get_deep())
            self.gen_list(self.body)
            self.genir.add_inst('mov', 'esp', 'ebp')
            self.genir.add_inst('pop', 'ebp')
            self.genir.add_inst('ret')
            self.symtab.leavefunc()

    def set_symtab(self):
        self.symtab.add_func(self.name,self.args,self.type)
        if self.body:
            func=self.symtab.functions[self.name]
            self.symtab.enterfunc(func)
            func.add_arg()
            self.set_list_symtab(self.body)
            self.symtab.leavefunc()

class Return(SyntaxTree):
    def __init__(self, *args, **kwargs) -> None:
        self.value:SyntaxTree=None
        super().__init__(*args, **kwargs)

    def gen(self):
        self.value.gen()
        self.genir.add_inst('pop', 'eax')
        self.genir.add_inst('mov', 'esp', 'ebp')
        self.genir.add_inst('pop', 'ebp')
        self.genir.add_inst('ret')

class While(SyntaxTree):
    def __init__(self, *args, **kwargs) -> None:
        self.exp:SyntaxTree=None
        self.body:list=[]
        super().__init__(*args, **kwargs)

    def set_symtab(self):
        self.exp.set_symtab()
        self.set_list_symtab(self.body)

    def gen(self):
        self.genir.comment('->While')
        beginlabel=self.genir.get_label()
        self.genir.add_label(beginlabel)
        self.exp.gen()
        endlabel=self.genir.get_label()
        self.genir.add_inst('pop','eax')
        self.genir.add_inst('cmp','eax',0)
        self.genir.add_inst('je',endlabel)
        self.gen_list(self.body)
        self.genir.add_inst('jmp',beginlabel)
        self.genir.add_label(endlabel)
        self.genir.comment('<-While')


def get_var_defs(a):
    result = {'names': [], 'values': []}
    for i in a:
        result['names'].append(i[0].string)
        if i[1]:
            result['values'].append(i[1][1])
        else:
            result['values'].append(None)
    return result


def get_args(a):
    if a == None:
        return []
    result = [a[0]]
    b = a[1]
    if b:
        for _, val in b:
            result.append(val)
    return result
