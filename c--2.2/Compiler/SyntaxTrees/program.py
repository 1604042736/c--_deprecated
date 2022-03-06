from syntaxtree import *


class Program(SyntaxTree):
    def __init__(self, **kwargs) -> None:
        self.body: list[SyntaxTree] = []
        super().__init__(**kwargs)
