from pegen.parser import *

import sys
import os
sys.path.append(os.path.dirname(__file__))

from parser import *
from translater import *

def compile(filename):
    file=open(filename,encoding='utf-8')
    tokengen = tokenize.generate_tokens(file.readline)
    tokenizer = Tokenizer(tokengen)
    parser = GeneratedParser(tokenizer)
    tree = parser.start()
    file.close()
    printast(tree)
    translater(tree)