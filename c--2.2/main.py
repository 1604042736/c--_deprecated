import sys,os
sys.path.append(os.path.dirname(__file__))

try:
    from build_parser import *
except:
    pass

import argparse

parser = argparse.ArgumentParser(description='c--2.1')
parser.add_argument("filename")
parser.add_argument("-v", "--version", help="显示版本信息", action='store_true')
args=parser.parse_args()

def machine(name,target,filename):
    path=tuple(sys.path)    #保存
    modules=dict(sys.modules)

    machine=__import__(name)
    filename=machine.__dict__[target](filename)

    sys.path=list(path) #恢复
    sys.modules=modules
    return filename

def main():
    if args.version:
        print("c-- 2.2")
        return
    filename = args.filename
    filename=machine('Preprocessor','preprocess',filename)
    filename=machine('Compiler','compile',filename)

if __name__=='__main__':
    main()