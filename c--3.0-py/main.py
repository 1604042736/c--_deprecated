import sys
from environment import Environment
from exception import CException
from parser import Parser
from lexer import Lexer
import argparse

parser = argparse.ArgumentParser(description='c--')
parser.add_argument("filename")
parser.add_argument("-d", "--debug", help="显示调试信息",action='store_true')

def main():
    global parser
    args = parser.parse_args()
    filename = args.filename
    with open(filename, encoding='utf-8')as file:
        lines = file.readlines()
        envir = Environment()
        lexer = Lexer(lines, filename)
        parser = Parser(lexer, envir)
        try:
            t = parser.start()
            if args.debug:
                t.print()
        except CException as e:
            e.print()


if __name__ == '__main__':
    main()
