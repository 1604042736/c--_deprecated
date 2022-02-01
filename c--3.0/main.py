import argparse
from ast import arg
import sys
from syntaxtree import _compile

parser = argparse.ArgumentParser(description='c--3.0')
parser.add_argument("filename")
parser.add_argument("-d", "--debug", help="显示调试信息", action='store_true')
parser.add_argument("-v", "--version", help="显示版本信息", action='store_true')


def main():
    global parser
    args = parser.parse_args()
    if args.version:
        print("c-- 3.0")
        return
    filename = args.filename
    _compile(filename, args.debug)


if __name__ == '__main__':
    main()
