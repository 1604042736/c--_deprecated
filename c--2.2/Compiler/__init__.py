import os
import sys
sys.path.append(os.path.dirname(__file__))

from pegen.parser import *
from compiler_parser import *
from error import *
from globals import *

def compile(filename):
    with open(filename,encoding='utf-8')as file:
        Globals.code=file.readlines()   #记录代码
        Globals.filename=filename
        file.seek(0)
        tokengen = tokenize.generate_tokens(file.readline)
        tokenizer = Tokenizer(tokengen)
        parser = GeneratedParser(tokenizer)
        tree = parser.start()
        tree.analyse()
        tree.gen()
        Globals.symtab.gen_data()

        printast(tree)
        Globals.symtab.print()

        root,ext=os.path.splitext(filename)
        Globals.genir.write(root+'.asm')