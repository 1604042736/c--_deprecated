from expr import *

class Attribute(Expr):
    def __init__(self, *args, **kwargs) -> None:
        self.value: SyntaxTree = None
        self.attr: str = ''
        self.mode: SyntaxTree = None
        super().__init__(*args, **kwargs)

    def get_type(self):
        self.type=self.value.get_type()
        return self.type