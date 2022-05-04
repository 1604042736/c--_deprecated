from type_ import *


class Pointer(Type):
    def __init__(self, **kwargs) -> None:
        self.base_type: Type = None
        self.level: int = 0
        super().__init__(**kwargs)
        self.size=self.base_type.size

    def __str__(self) -> str:
        return super().__str__() + f'{"*"*self.level}'
