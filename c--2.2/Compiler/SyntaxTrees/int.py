from type import *

class Int(Type):
    def __init__(self, **kwargs) -> None:
        super().__init__(**kwargs)
        self.size=4