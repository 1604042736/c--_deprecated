from type import *


class Array(Type):
    def __init__(self, **kwargs) -> None:
        self.type: Type = None
        self.dimen: list = []  # 维数
        super().__init__(**kwargs)

    def analyse(self):
        self.type.analyse()
        self.size = 1
        for i in self.dimen:
            self.size *= int(i)*self.type.size

    def __str__(self) -> str:
        return f"{self.type}{''.join([f'[{i}]'for i in self.dimen])}"

    def load(self):
        #数组变量是个指针
        self.genir.add_inst('mov','eax','ecx')
        self.genir.add_inst('push','eax')

    def store(self):
        self.genir.add_inst('pop','eax')
        self.genir.add_inst('mov','ecx','eax')