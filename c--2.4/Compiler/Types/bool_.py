from type_ import *


class Bool(Type):
    def __init__(self, **kwargs) -> None:
        super().__init__(**kwargs)
        self.size = 1
