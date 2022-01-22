import sys
from ast import *

def printast(tree,indent=0):
    '''
    以树状形式输出一棵语法树
    '''
    print('  '*indent+str(type(tree).__name__))
    for key,val in tree.__dict__.items():
        print('  '*(indent+1)+key,end='')
        if val==None or isinstance(val,str) or isinstance(val,int):
            print(':',val)
        elif isinstance(val,list):
            print()
            for i in val:
                if isinstance(i,str):
                    print(i)
                else:
                    printast(i,indent+2)
        elif isinstance(val,AST):
            print()
            printast(val,indent+2)

if __name__=='__main__':
    if len(sys.argv)>1:
        code=open(sys.argv[1],encoding='utf-8').read()
    else:
        code='''
def f(n):
    if n==1 or n==2:
        return 1
    return f(n-1)+f(n-2)
f(30)'''
    a=parse(code)
    printast(a)
