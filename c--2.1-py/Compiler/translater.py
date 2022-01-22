from syntaxtree import *

def translater(t):
    if isinstance(t,list):
        for i in t:
            translater(i)
    elif isinstance(t,AST):
        if type(t).__name__=='Module':
            translater(t.body)
        elif type(t).__name__=='Assign':
            pass