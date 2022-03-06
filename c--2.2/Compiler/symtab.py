class Symtab:
    '''符号表'''

    def __init__(self) -> None:
        self.curfunc = []  # 当前所在的函数
        self.offset = []  # 偏移
        self.globals = {}  # 全局变量
        self.constant = {}  # 常数
        self.constnum = 0  # 计数
        self.functions={}   #函数
        self.cursym=[]    #操作过的符号

    def add_func(self,func):
        self.functions[func.name]=func
        self.cursym.append(func)

    def get_func(self,name):
        return self.functions[name]

    def enterfunc(self, func):
        '''进入函数'''
        self.curfunc.append(func)
        self.offset.append(0)

    def leavefunc(self):
        '''离开函数'''
        self.curfunc.pop()
        self.offset.pop()

    def add_offset(self, a):
        self.offset[-1] += a

    def add_var(self, var):
        '''添加变量'''
        if self.curfunc:
            self.curfunc[-1].add_var(var)
        else:
            self.globals[var.name] = var
        self.cursym.append(var)

    def get_var(self, name):
        '''获得变量'''
        if self.curfunc:
            return self.curfunc[-1].get_var(name)
        else:
            return self.globals[name]

    def add_constant(self, const):
        '''添加常数(字符串)'''
        self.constant[const] = StringSym(const, 'constant'+str(self.constnum))
        self.constnum += 1

    def get_constant(self, const):
        '''获得常数对应的名称'''
        return self.constant[const].id

    def gen_data(self):
        '''生成data段'''
        import globals
        Globals=globals.Globals
        Globals.genir.add_section('.data')
        sec=Globals.genir.cursec
        Globals.genir.set_cursec('.data')
        for key,const in self.constant.items():
            id=const.id
            val=const.val
            if isinstance(const,StringSym):
                Globals.genir.add_inst(f'{id} db',f"'{val}'")
            else:
                if const.type.size==4:
                    Globals.genir.add_inst(f'{id} dd',val)
        Globals.genir.set_cursec(sec)

    def print(self):
        for key,val in self.__dict__.items():
            print(f'{key}={val}')

    def get_offset(self):
        if self.offset:
            return self.offset[-1]
        else:
            return 0


class StringSym:
    '''字符串符号'''

    def __init__(self, val, id) -> None:
        self.val = val
        self.id = id

    def __repr__(self) -> str:
        return f'StringSym({",".join([f"{key}={val}"for key,val in self.__dict__.items()])})'


class FuncSym:
    '''函数符号'''

    def __init__(self, name, return_type,args,isextern=False) -> None:
        self.name = name  # 函数名
        self.return_type = return_type  # 返回类型
        self.args=tuple(args)   #防止更改
        self.locals = {}  # 局部变量表
        self.isextern=isextern

    def add_var(self, var):
        self.locals[var.name] = var

    def get_var(self, name):
        return self.locals[name]

    def get_deep(self):
        '''获取深度'''
        deep=0
        for key,val in self.locals.items():
            deep+=abs(val.offset)
        return deep

    def __repr__(self) -> str:
        return f'FuncSym({",".join([f"{key}={val}"for key,val in self.__dict__.items()])})'

class VarSym:
    '''变量符号'''

    def __init__(self, name, type, offset=0, islocal=True,isextern=False) -> None:
        self.name = name  # 变量名
        self.type = type  # 类型
        self.offset = offset  # 偏移
        self.islocal = islocal  # 是否是局部变量
        self.isextern=isextern  #是否是外部声明

    def __repr__(self) -> str:
        return f'VarSym({",".join([f"{key}={val}"for key,val in self.__dict__.items()])})'