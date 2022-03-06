from void import *
from expr import *
from more import *
from argdef import *


class Call(Expr):
    def __init__(self, **kwargs) -> None:
        self.func:str=''
        self.args:list[Expr]=[]
        super().__init__(**kwargs)

    def analyse_type(self):
        if self.func in self.symtab.functions:
            func=self.symtab.get_func(self.func)
            self.type=func.return_type
        else:
            self.type=Void()

    def gen(self):
        func=self.symtab.get_func(self.func)
        if func.isextern:
            #外部函数
            self.genir.add_extern(self.func)
        funcargs=list(func.args)
        if len(self.args)>=len(funcargs):   #实参比形参多
            if not isinstance(funcargs[-1],More):
                pass #TODO 报错
            else:
                self.complete(funcargs,self.args,More())
        else:
            self.complete(self.args,funcargs)
        size=0
        for i,j in zip(funcargs[::-1],self.args[::-1]):
            size+=i.gen(j)  #push参数的同时统计参数长度
        self.genir.add_inst('call',self.func)
        self.genir.add_inst('add', 'esp', size)
        if not isinstance(func.return_type,Void):   #void没有返回类型
            self.genir.add_inst('push','eax')

    def complete(self,a:list,b:list,fill=None):
        '''补齐a列表'''
        for i in range(len(b)-len(a)):
            a.append(fill)