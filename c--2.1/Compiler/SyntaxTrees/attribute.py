from syntaxtree import *

class Attribute(SyntaxTree):
    def __init__(self, *args, **kwargs) -> None:
        self.value: SyntaxTree = None
        self.attr: str = ''
        self.mode: SyntaxTree = None
        super().__init__(*args, **kwargs)