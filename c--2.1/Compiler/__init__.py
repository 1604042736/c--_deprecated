from pegen.parser import *

import sys
import os
sys.path.append(os.path.dirname(__file__))

from compiler_paresr import *
from globals import *

def compile(filename):
    with open(filename,encoding='utf-8')as file:
        tokengen = tokenize.generate_tokens(file.readline)
        tokenizer = Tokenizer(tokengen)
        parser = GeneratedParser(tokenizer)
        tree = parser.start()
        tree.print()
        tree.set_symtab()
        tree.gen()
        Globals.symtab.gen_data()
        print(Globals.genir.section)
        Globals.symtab.print()
        Globals.genir.write('.'.join(filename.split('.')[:-1])+'.asm')