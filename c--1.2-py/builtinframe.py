import sys
import math
import frame

builtin_sys = frame.Frame()
builtin_sys.locals['argv'] = sys.argv
builtin_sys.locals['version'] = 'c-- 1.2'
builtin_sys.locals['keywords'] = ['if', 'while', 'else', 'function', 'class', 'and', 'or', 'try', 'continue', 'break',
                                  'return', 'import', 'except']

builtin_math = frame.Frame()
builtin_math.locals['sin'] = math.sin
builtin_math.locals['cos'] = math.cos
builtin_math.locals['tan'] = math.tan


def builtin_len(obj):
    return len(obj[0])


def builtin_input(obj=''):
    return input(obj)


def builtin_print(*obj):
    for i in obj:
        print(i, end='')


def builtin_str(obj):
    return str(obj)


def builtin_int(obj):
    return int(obj)


def builtin_abs(obj):
    return abs(obj)


def builtin_max(one, other):
    return max(one[0], other[1])


def builtin_min(one, other):
    return min(one[0], other[1])


def builtin_bin(obj):
    return bin(obj)


def builtin_oct(obj):
    return oct(obj)


def builtin_hex(obj):
    return hex(obj)


def builtin_open(filename, mode, encoding):
    return open(filename, mode=mode, encoding=encoding)
