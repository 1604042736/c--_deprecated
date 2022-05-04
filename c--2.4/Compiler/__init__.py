'''编译器'''
from Compiler.compiler_parser import *
from tokenize import *
from pegen.parser import *
from Compiler.symtab import *
from Compiler.genir import *

symtab = None
genir = None


def compile(filename):
    '''编译'''
    global symtab, genir
    with open(filename, encoding='utf-8') as file:
        tokengen = handle_tokens(file.readline)
        tokenizer = Tokenizer(tokengen)
        parser = GeneratedParser(tokenizer)
        symtab = Symtab()
        genir = GenIR()
        tree = parser.start()
        tree.gen()

        tree.print()
        print('='*32)
        for i in Symtab.all_scopes:
            i.print()
        print('='*32)
        genir.print()
        print('='*32)
        genir.toasm(filename+'.asm')


def handle_tokens(readline):
    '''对tokenize生成的token进行一些处理'''
    for token in generate_tokens(readline):
        if token.type in (NEWLINE, INDENT, DEDENT):
            continue
        yield token
