class Scanner:
    '''词法分析'''

    def __init__(self, filename, code):
        self.filename = filename
        self.tokens = []
        self.scan([i + '\n' for i in code.split('\n')])  # 以行为单位切割, 并在最后加上换行符

    def scan(self, filelines):
        for lineno in range(len(filelines)):
            fileline, linepos = filelines[lineno], 0
            while linepos < len(fileline):
                if fileline[linepos].isalpha() or fileline[linepos] == '_':  # 变量以字母或_开头
                    linepos, add = self.get_name(linepos, fileline)  # 获取变量名, 并更新当前位置
                elif fileline[linepos].isdigit():
                    linepos, add = self.get_num(linepos, fileline)
                elif fileline[linepos] == '=':
                    if fileline[linepos + 1] == '=':
                        add, linepos = '==', linepos + 1
                    else:
                        add = '='
                elif fileline[linepos] == '>':
                    if fileline[linepos + 1] == '=':
                        add, linepos = '>=', linepos + 1
                    elif fileline[linepos + 1] == '>':
                        add, linepos = '>>', linepos + 1
                    else:
                        add = '>'
                elif fileline[linepos] == '<':
                    if fileline[linepos + 1] == '=':
                        add, linepos = '<=', linepos + 1
                    elif fileline[linepos + 1] == '<':
                        add, linepos = '<<', linepos + 1
                    else:
                        add = '<'
                elif fileline[linepos] == '!':
                    if fileline[linepos + 1] == '=':
                        add, linepos = '!=', linepos + 1
                    else:
                        add = '!'
                elif fileline[linepos] == "'" or fileline[linepos] == '"':
                    linepos, add = self.get_string(linepos, fileline)
                else:
                    add = fileline[linepos]
                self.tokens.append(
                    [add,
                     {'lineno': lineno, 'linepos': linepos, 'line': fileline, 'filename': self.filename}])  # 保存对应的文件信息
                linepos += 1

    def get_name(self, i, line):
        res = ""
        while i < len(line) and (line[i].isalpha() or line[i].isdigit() or line[i] == '_'):  # 变量中允许有数字
            res += line[i]
            i += 1
        return i - 1, res  # 返回已经读取到的位置, 和变量名

    def get_num(self, i, line):
        res = ''
        while i < len(line) and line[i].isdigit():
            res += line[i]
            i += 1
        return i - 1, res

    def get_string(self, i, line):
        start, i, res = line[i], i + 1, line[i]
        while i < len(line) and line[i] != start:
            add = line[i]
            if add == '\\':  # 转义字符处理
                i += 1
                add = line[i]
                if add == '\\':
                    add = '\\'
                elif add == 'n':
                    add = '\n'
                elif add == 't':
                    add = '\t'
                elif add == 'r':
                    add = '\r'
                elif add == "'":
                    add = "'"
                elif add == '"':
                    add = '"'
                elif add == 'v':
                    add = '\v'
                elif add == 'f':
                    add = '\f'
            res += add
            i += 1
        res += start
        return i, res
