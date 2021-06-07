import re


class Preprocessor:
    '''预处理器'''
    define = {'True': 1, 'False': 0}  # 定义的宏
    IMPORT_PATH = 'Import'  # import路径
    CONDIT_INS = ['ifdef', 'ifudef', 'elif', 'else', 'endif']  # 条件指令

    def getstringrange(self, code):
        i = 0
        while i < len(code):
            if code[i] == '"' or code[i] == "'":
                startc = code[i]
                start = i
                i += 1
                while i < len(code) and code[i] != startc:
                    if code[i] == '\\':
                        i += 1
                    i += 1
                end = i
                self.stringrange += list(range(start, end + 1))
            i += 1

    def pretreatment(self, code):
        self.stringrange = []  # 出现的字符串范围
        self.getstringrange(code)
        defineflag = False  # 宏判断的标记
        predivs = re.findall('#.*', code)  # 查找所有的预处理指令
        replace = {}  # 要替换的语句
        for i in range(len(predivs)):
            if code.find(predivs[i]) not in self.stringrange:
                args = predivs[i].replace('#', ' ').split()
                if args[0] == 'define':
                    self.define[args[1]] = predivs[i][predivs[i].find(args[2]):]
                    replace[predivs[i]] = ''
                elif args[0] == 'undef':
                    self.define.pop(args[1])
                    replace[predivs[i]] = ''
                elif args[0] == 'ifdef':
                    if args[1] in self.define:
                        defineflag = True
                        replace[predivs[i]] = ''
                    else:
                        next_index = self.get_next_index(i, predivs)
                        if next_index != -1:
                            # 替换掉当前预处理指令到下一个条件预处理之间的代码
                            replace[code[code.find(predivs[i]):code.find(predivs[next_index])]] = ''
                elif args[0] == 'ifudef':
                    if args[1] not in self.define:
                        defineflag = True
                        replace[predivs[i]] = ''
                    else:
                        next_index = self.get_next_index(i, predivs)
                        if next_index != -1:
                            replace[code[code.find(predivs[i]):code.find(predivs[next_index])]] = ''
                elif args[0] == 'elif':
                    if not defineflag and args[1] in self.define:
                        defineflag = True
                        replace[predivs[i]] = ''
                    else:
                        next_index = self.get_next_index(i, predivs)
                        if next_index != -1:
                            replace[code[code.find(predivs[i]):code.find(predivs[next_index])]] = ''
                elif args[0] == 'else':
                    if not defineflag:
                        replace[predivs[i]] = ''
                    else:
                        next_index = self.get_next_index(i, predivs)
                        if next_index != -1:
                            replace[code[code.find(predivs[i]):code.find(predivs[next_index])]] = ''
                elif args[0] == 'endif':
                    defineflag = False
                    replace[predivs[i]] = ''
                elif args[0] == 'import':
                    j, indent, rep = code.find(predivs[i]) - 1, '', ''
                    while j >= 0 and code[j] != '\n':  # 获取当前预处理指令的缩进
                        indent += code[j]
                        j -= 1
                    try:
                        # 先找程序路径
                        with open(f'{args[1]}', encoding='utf-8')as file:
                            filelines = [i + '\n' for i in Preprocessor().pretreatment(file.read()).split('\n')]
                            rep = filelines[0]  # 第一行不用添加缩进
                            filelines.pop(0)
                            for line in filelines:
                                rep = rep + indent + line  # 将每一行添加缩进以适应当前缩进
                    except:
                        # 再找默认路径
                        with open(f'{self.IMPORT_PATH}\\{args[1]}', encoding='utf-8')as file:
                            filelines = [i + '\n' for i in Preprocessor().pretreatment(file.read()).split('\n')]
                            rep = filelines[0]
                            filelines.pop(0)
                            for line in filelines:
                                rep = rep + indent + line
                    replace[predivs[i]] = rep
                else:
                    replace[predivs[i]] = ''
        for key, val in replace.items():
            code = code.replace(key, val)
        for key, val in self.define.items():
            code = code.replace(key, str(val))
        return code

    def get_next_index(self, i, predivs):
        '''找到下一个条件预处理指令的位置'''
        for j in range(i + 1, len(predivs)):
            if predivs[j].replace('#', ' ').split()[0] in self.CONDIT_INS:
                return j
        return -1
