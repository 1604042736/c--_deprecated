import argparse
from syntaxtree import _compile

parser = argparse.ArgumentParser(description='c--')
parser.add_argument("filename")
parser.add_argument("-d", "--debug", help="显示调试信息",action='store_true')

def main():
    global parser
    args = parser.parse_args()
    filename = args.filename
    _compile(filename,args.debug)


if __name__ == '__main__':
    main()
