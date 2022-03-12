from operator import index
import os
import re
from unicodedata import name

__path__ = os.path.dirname(__file__)
__include__ = os.path.dirname(__path__)+'\Include'


class Preprocessor:
    '''预处理器'''
    ID = r'([A-Za-z]|_)([A-Za-z]|[0-9]|_)*'
    INCLUDE_FILE = r'<.*?>|".*?"'
    INCLUDE = r'#\s*include\s*<.*?>|#\s*include\s*".*?"'
    DEFINE = rf'#\s*define\s+{ID}.*?\n'
    UNDEF = rf'#\s*undef\s+{ID}'
    IFDEF = rf'#\s*ifdef\s+{ID}'
    IFUDEF = rf'#\s*ifudef\s+{ID}'
    ELIF = rf'#\s*elif\s+{ID}'
    ELSE = rf'#\s*else'
    ENDIF = rf'#\s*endif'
    LEX = [INCLUDE, DEFINE, UNDEF, IFDEF, IFUDEF, ELIF, ELSE, ENDIF]

    defines = {}  # 所有的宏
    define_flag = []  # 宏标记

    def __init__(self, filename) -> None:
        self.filename = filename
        self.replace = {}  # 要替换的
        self.order = []
        self.index = 0
        with open(filename, encoding='utf-8')as file:
            self.content = file.read()
            file.seek(0)
            ins = re.finditer('|'.join(self.LEX), file.read())  # 匹配所有指令
            for i in ins:
                self.order.append(i)

        while self.index < len(self.order):
            self.deal(self.order[self.index])
            self.index += 1

        self.define_replace()
        for key, val in self.replace.items():
            self.content = self.content.replace(key, val)

    def deal(self, ins: re.Match):
        '''处理指令'''
        if 'include' in ins.group():  # include指令
            include_file = re.findall(self.INCLUDE_FILE, ins.group())[0]
            filename = None  # 包含文件名
            if include_file[0] == '<':  # 从默认目录包含
                filename = __include__+'\\'+include_file[1:-1]
            elif include_file[0] == '"':  # 从当前目录包含
                filename = include_file[1:-1]
            pre = Preprocessor(filename)
            # 计算指令所在的缩进
            indent = ''
            i, _ = ins.span()
            i -= 1
            while i >= 0 and self.content[i] != '\n':
                indent += self.content[i]
                i -= 1
            replace = pre.content.split('\n')
            for index, code in enumerate(replace):
                if index > 0:
                    replace[index] = indent+replace[index]
            self.replace[ins.group()] = '\n'.join(replace)
        elif 'define' in ins.group():  # define命令
            name, *replace = ins.group()[1:].split()[1:]
            replace = " ".join(replace)
            Preprocessor.defines[name] = replace
        elif 'undef' in ins.group():
            name = ins.group()[1:].split()[1:]
            for i in name:
                Preprocessor.defines.pop(i)
        elif 'ifdef' in ins.group():
            name = ins.group()[1:].split()[1]
            if name in Preprocessor.defines:
                self.define_flag.append(True)
            else:
                self.replace_block()
                self.define_flag.append(False)
        elif 'ifudef' in ins.group():
            name = ins.group()[1:].split()[1]
            if name in Preprocessor.defines:
                self.replace_block()
                self.define_flag.append(False)
            else:
                self.define_flag.append(True)
        elif 'elif' in ins.group():
            if self.define_flag[-1]:
                self.replace_block()
            else:
                name = ins.group()[1:].split()[1]
                if name in Preprocessor.defines:
                    self.define_flag[-1] = True
                else:
                    self.replace_block()
        elif 'else' in ins.group():
            if self.define_flag[-1]:
                self.replace_block()
        elif 'endif' in ins.group():
            self.define_flag.pop()

    def replace_block(self):
        '''替换代码块,为条件判断命令服务'''
        startpos, endpos = self.order[self.index].span()  # 开始部分
        self.index += 1
        while self.index < len(self.order):
            ins = self.order[self.index]
            ins_name = ins.group()[1:].split()[0]
            if ins_name in ('elif', 'else', 'endif'):  # 结束
                # 记录结束位置
                endpos, _ = ins.span()
                self.index -= 1
                break
            self.index += 1
        self.replace[self.content[startpos:endpos]] = ''

    def define_replace(self):
        '''将宏添加到self.replace中'''
        for key, val in Preprocessor.defines.items():
            self.replace[key] = val

    def write(self, filename):
        with open(filename, mode='w', encoding='utf-8')as file:
            file.write(self.content)
