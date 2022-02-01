import argparse
from syntaxtree import _compile

parser = argparse.ArgumentParser(description='c--3.0')
parser.add_argument("filename")
parser.add_argument("-d", "--debug", help="显示调试信息", action='store_true')
parser.add_argument("-v", "--version", help="显示版本信息", action='store_true')
args=parser.parse_args()

def main():
    global args
    if args.version:
        print("c-- 3.0")
        return
    filename = args.filename
    _compile(filename, args.debug)


if __name__ == '__main__':
    main()
