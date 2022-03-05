import sys
import os
sys.path.append(os.path.dirname(__file__))

from add import *
from and_ast import *
from array_ast import *
from assign import *
from attribute import *
from binop import *
from bitand import *
from bitor import *
from bitxor import *
from boolop import *
from call import *
from chartype import *
from compare import *
from div import *
from eq import *
from floattype import *
from function import *
from geq import *
from gt import *
from if_ast import *
from inttype import *
from leq import *
from load import *
from lshift import *
from lt import *
from mod import *
from mul import *
from name import *
from neq import *
from num import *
from or_ast import *
from pointer import *
from program import *
from return_ast import *
from rshift import *
from store import *
from string_ast import *
from sub import *
from subscript import *
from syntaxtree import *
from vardef import *
from while_ast import *
from argdef import *

def get_var_defs(a):
    result = {'names': [], 'values': []}
    for i in a:
        result['names'].append(i[0].string)
        if i[1]:
            result['values'].append(i[1][1])
        else:
            result['values'].append(None)
    return result


def get_args(a):
    if a == None:
        return []
    result = [a[0]]
    b = a[1]
    if b:
        for _, val in b:
            result.append(val)
    return result
