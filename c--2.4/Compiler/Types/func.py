from type_ import *


class Func(Type):
    def __init__(self, **kwargs) -> None:
        self.return_type = None
        self.args_type = []
        self.scope = None
        super().__init__(**kwargs)

    def __str__(self) -> str:
        return f'{self.return_type}({",".join(map(str,self.args_type))})'

    def call(self, name):
        result = []
        result.append(['call', name])
        return result
