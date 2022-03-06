import os
import sys

if __name__=='__main__':
    os.system(' '.join(sys.argv[1:]))
    print('\n'+'-'*16)
    os.system('pause')