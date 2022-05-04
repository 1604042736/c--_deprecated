from expr import *


class Assign(Expr):
    def __init__(self, **kwargs) -> None:
        self.children = ['targets', 'value']
        self.targets = []
        self.value = None
        super().__init__(**kwargs)

    def gen(self):
        self.value.gen()
        for i in self.targets:
            i.gen()
