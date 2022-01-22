import sys
from environment import Environment
from parser import Parser
from lexer import Lexer


def main():
    filename = sys.argv[1]
    with open(filename, encoding='utf-8')as file:
        lines = file.readlines()
        envir = Environment()
        lexer = Lexer(lines, filename)
        parser = Parser(lexer, envir)
        t = parser.start()
        t.print()
        print(parser.envir.curframe.locals)
        print(parser.envir.curframe.stack)


if __name__ == '__main__':
    main()
