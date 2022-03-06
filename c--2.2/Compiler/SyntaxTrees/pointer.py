from type import *

class Pointer(Type):
    def __init__(self, **kwargs) -> None:
        self.type:Type=None
        self.level:int=0  #指针层数
        super().__init__(**kwargs)

    def __str__(self) -> str:
        return f'{self.type}{"*"*self.level}'