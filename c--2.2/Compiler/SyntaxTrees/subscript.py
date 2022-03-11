from error import error
from array_ast import *
from expr import *
from mode import *


class Subscript(Expr):
    def __init__(self, **kwargs) -> None:
        self.value: Expr = None
        self.slice: Expr = None
        self.mode: Mode = None
        self.nowdimen=0
        super().__init__(**kwargs)

    def analyse_type(self):
        self.type=self.value.get_type()
        #能进行索引操作的默认是array类型
        if isinstance(self.type,Array):
            #防止被更改
            dimen=self.type.dimen
            self.type=Array(**self.type.__dict__)
            self.type.dimen=dimen[:]

            if len(self.type.dimen)>0:  #维数减一
                self.nowdimen=self.type.dimen.pop(0)
            if len(self.type.dimen)<=0:
                self.type=self.type.type
                self.nowdimen=0
        else:
            error(f'{self.type}不能索引',self.location)

    def gen(self):
        self.slice.gen()
        self.value.gen()
        self.genir.add_inst('pop','ecx')
        if isinstance(self.type,Array):
            self.genir.add_inst('pop','eax')
            self.genir.add_inst('push',self.nowdimen)
            self.genir.add_inst('pop','ebx')
            self.genir.add_inst('mul','ebx')

            self.genir.add_inst('push',self.type.type.size)
            self.genir.add_inst('pop','ebx')
            self.genir.add_inst('mul','ebx')
            self.genir.add_inst('sub','ecx','eax')
        else:
            self.genir.add_inst('pop','eax')

            self.genir.add_inst('push',self.type.size)
            self.genir.add_inst('pop','ebx')
            self.genir.add_inst('mul','ebx')
            self.genir.add_inst('sub','ecx','eax')
        self.mode.gen(self.type)
