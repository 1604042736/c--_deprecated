import sys,os
sys.path.append(os.path.dirname(__file__))

try:
    from build_parser import *
except:
    pass

from Compiler import *
from Preprocessor import *

import argparse

parser = argparse.ArgumentParser(description='c--2.1')
parser.add_argument("filename")
parser.add_argument("-v", "--version", help="显示版本信息", action='store_true')
args=parser.parse_args()

def main():
    if args.version:
        print("c-- 2.2")
        return
    filename = args.filename
    filename=preprocess(filename)
    compile(filename)

if __name__=='__main__':
    main()