import argparse
from interpreter import *


def file(filename, printtree=False, dumptree=False, printbytecode=False, run=True):
    '''以文件模式运行'''
    with open(filename, encoding='utf-8')as f:
        Interpreter().interpret(f.read(), 'File', filename=filename, printtree=printtree, dumptree=dumptree,
                                printbytecode=printbytecode, run=run)


def shell(printtree=False, dumptree=False, printbytecode=False, run=True):
    '''以交互式模式运行'''
    print('Welcome to c--2.0!')
    interpreter = Interpreter()
    while True:
        try:
            interpreter.interpret(input('>>> '), 'Shell', printtree=printtree, dumptree=dumptree,
                              printbytecode=printbytecode, run=run)
        except Exception as e:
            print(e)


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('-v', '--version', action='version', version='c--1.2')
    parser.add_argument('-pt', '--printtree', action="store_true", help='Print Syntaxtree')
    parser.add_argument('-dt', '--dumptree', action="store_true", help='Print Syntaxtree as string')
    parser.add_argument('-pbc', '--printbytecode', action="store_true", help='Print bytecode')
    parser.add_argument('-run', action="store_true", help='Run code')
    parser.add_argument('filename', nargs='?', help='Filename')
    args = parser.parse_args()
    pt, dt, pbc, run = bool(args.printtree), bool(args.dumptree), bool(args.printbytecode), not bool(args.run)
    if not args.filename:
        shell(printtree=pt, dumptree=dt, printbytecode=pbc, run=run)
    else:
        file(args.filename, printtree=pt, dumptree=dt, printbytecode=pbc, run=run)
