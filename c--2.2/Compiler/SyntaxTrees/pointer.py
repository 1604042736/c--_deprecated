from type import *

class Pointer(Type):
    def __init__(self, **kwargs) -> None:
        self.type:Type=None
        self.level:int=0  #指针层数
        super().__init__(**kwargs)

    def analyse(self):
        self.type.analyse()
        self.size=self.level*self.type.size  #TODO 指针长度

    def __str__(self) -> str:
        return f'{self.type}{"*"*self.level}'

    def load(self):
        self.genir.add_inst('mov','eax','ecx')
        self.genir.add_inst('push','eax')

    def store(self):
        self.genir.add_inst('pop','eax')
        self.genir.add_inst('mov','ecx','eax')