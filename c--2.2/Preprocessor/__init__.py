from logging import root
import os
import sys
sys.path.append(os.path.dirname(__file__))

from preprocessor import *

def preprocess(filename):
    preprocessor=Preprocessor(filename)
    root,ext=os.path.splitext(filename)
    _filename=root+'.i'
    preprocessor.write(_filename)
    return _filename