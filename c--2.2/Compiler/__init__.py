import os
import sys
from black import err
sys.path.append(os.path.dirname(__file__))

from pegen.parser import *
from compiler_parser import *
from error import *
from globals import *

def compile(filename):
    with open(filename,encoding='utf-8')as file:
        tokengen = tokenize.generate_tokens(file.readline)
        tokenizer = Tokenizer(tokengen)
        parser = GeneratedParser(tokenizer)
        tree = parser.start()
        if tree==None:
            error('语法错误!')
        tree.analyse()
        tree.gen()
        Globals.symtab.gen_data()

        printast(tree)
        Globals.symtab.print()

        root,ext=os.path.splitext(filename)
        Globals.genir.write(root+'.asm')