from asyncio import constants


class Symtab:
    '''
    符号表
    '''
    def __init__(self) -> None:
        self.constant={} #常数
        self.constnum=0   #计数
        self.functions={}    #函数
        self.curfunc=[]    #当前函数
        self.offset=[]   #偏移
        self.globals={}  #全局变量

    def add_constant(self,const,sym,type=None):
        '''
        添加常数
        '''
        if type:
            self.constant[const]=sym(const,'constant'+str(self.constnum),type)
        else:
            self.constant[const]=sym(const,'constant'+str(self.constnum))
        self.constnum+=1

    def get_constant(self,const):
        '''
        获得常数对应的名称
        '''
        return self.constant[const].id

    def add_func(self,name,*args):
        if name not in self.functions:
            self.functions[name]=FunctionSym(name,*args)

    def gen_data(self):
        '''
        生成data段
        '''
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

    def enterfunc(self,func):
        '''
        进入函数
        '''
        self.curfunc.append(func)
        self.offset.append(0)

    def leavefunc(self):
        '''
        离开函数
        '''
        self.curfunc.pop()
        self.offset.pop()

    def add_offset(self,a):
        self.offset[-1]+=a

    def add_var(self,var):
        '''
        添加变量
        '''
        if self.curfunc:
            self.curfunc[-1].add_var(var)
        else:
            self.globals[var.name]=var

    def get_offset(self):
        if self.offset:
            return self.offset[-1]
        else:
            return 0

    def print(self):
        for key,val in self.__dict__.items():
            print(key,': ',val)

    def get_var(self,name):
        '''
        获取变量
        '''
        if self.curfunc:
            return self.curfunc[-1].get_var(name)
        else:
            return self.globals[name]

class FunctionSym:
    '''
    函数符号表
    '''
    def __init__(self,name,args,return_type) -> None:
        self.name=name
        self.args=args
        self.return_type=return_type
        self.locals={}  #局部变量表
        self.argsize=self.get_arg_size()

    def add_arg(self):
        import SyntaxTrees
        import globals  #warning
        symtab=globals.Globals.symtab
        for i in self.args:
            if isinstance(i,SyntaxTrees.VarDef):
                c=0
                for name in i.names:
                    if not c:
                        self.add_var(VarSym(name,i.type,offset=symtab.get_offset()+4,flag=-1))
                    else:
                        self.add_var(VarSym(name,i.type,offset=symtab.get_offset(),flag=-1))
                    symtab.add_offset(i.type.size)
                    c+=1

    def get_arg_size(self):
        import SyntaxTrees
        size=0
        for i in self.args:
            if isinstance(i,SyntaxTrees.VarDef):
                size+=(i.type.size)*len(i.names)
        return size

    def add_var(self,var):
        if var.name not in self.locals:
            self.locals[var.name]=var

    def __repr__(self) -> str:
        return f"{self.__class__.__name__}({str(self.__dict__)[1:-1]})"

    def get_deep(self):
        '''
        获取深度
        '''
        return sum([val.type.size for _,val in self.locals.items()])

    def get_var(self,name):
        return self.locals[name]

class StringSym:
    '''
    字符串符号表
    '''
    def __init__(self,str,id) -> None:
        self.val=str
        self.id=id

    def __eq__(self, __o: object) -> bool:
        return __o==self.val

    def __repr__(self) -> str:
        return f"{self.__class__.__name__}({str(self.__dict__)[1:-1]})"

class NumSym:
    def __init__(self,num,id,type) -> None:
        self.val=num
        self.id=id
        self.type=type

    def __eq__(self, __o: object) -> bool:
        return __o==self.val

    def __repr__(self) -> str:
        return f"{self.__class__.__name__}({str(self.__dict__)[1:-1]})"

class VarSym:
    '''
    变量
    '''
    def __init__(self,name,type,offset=0,islocal=True,flag=1,address=False) -> None:
        self.name=name
        self.offset=offset   #偏移量
        self.islocal=islocal    #是否为局部变量
        self.type=type  #类型
        self.address=address
        self.flag=flag

    def __repr__(self) -> str:
        return f"{self.__class__.__name__}({str(self.__dict__)[1:-1]})"