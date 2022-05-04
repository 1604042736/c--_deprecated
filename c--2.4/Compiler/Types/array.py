from type_ import *


class Array(Type):
    def __init__(self, **kwargs) -> None:
        self.base_type: Type = None
        self.dim: list[int] = []  # 维数
        super().__init__(**kwargs)
        self.size = 1
        for i in self.dim:
            self.size *= i
        self.size*=self.base_type.size

    def __str__(self) -> str:
        return super().__str__() + f'{"".join(["["+str(i)+"]" for i in self.dim])}'
