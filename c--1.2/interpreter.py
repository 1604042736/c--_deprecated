from preprocessor import *
from scanner import *
from analyzer import *
from compiler import *
from frame import *


class Interpreter:
    '''解释器'''

    def __init__(self):
        self.frame = Frame()

    def interpret(self, code, mode, filename=None, printtree=False, dumptree=False, printbytecode=False, run=True):
        precode = Preprocessor().pretreatment(code)
        scanner = Scanner(filename, precode)
        parser = Parser(scanner, mode)
        if printtree:
            parser.ast.print()
        if dumptree:
            print(parser.ast)
        bytecode = Compiler().compile(parser.ast)
        if printbytecode:
            bytecode.print()
        if run:
            self.frame.eval(bytecode)
