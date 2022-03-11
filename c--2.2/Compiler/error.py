from globals import *

def error(msg,location):
    '''输出错误信息'''
    lineno,col_offset,end_lineno,end_col_offset=location
    print(f'文件"{Globals.filename}",第{lineno}行,第{col_offset}列:')
    print(f'\t{Globals.code[lineno-1].strip()}')
    print(f'错误: {msg}')
    exit()