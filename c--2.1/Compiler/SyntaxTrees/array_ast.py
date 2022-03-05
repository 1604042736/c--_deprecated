from type import *

class Array(Type):
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

    def get_type(self):
        return self.type

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