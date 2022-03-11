import os
import re

__path__=os.path.dirname(__file__)
__include__=os.path.dirname(__path__)+'\Include'

class Preprocessor:
    '''预处理器'''
    ID='([A-Za-z]|_)([A-Za-z]|[0-9]|_)*'
    INCLUDE_FILE='<.*?>|".*?"'
    INCLUDE=r'#\s*include\s*<.*?>|#\s*include\s*".*?"'
    LEX=[INCLUDE]
    def __init__(self,filename) -> None:
        self.filename=filename
        self.replace={} #要替换的
        with open(filename,encoding='utf-8')as file:
            self.content=file.read()
            file.seek(0)

            ins=re.finditer('|'.join(self.LEX),file.read()) #匹配所有指令
            for i in ins:
                self.deal(i)
        for key,val in self.replace.items():
            self.content=self.content.replace(key,val)

    def deal(self,ins:re.Match):
        '''处理指令'''
        if 'include' in ins.group(): #include指令
            include_file=re.findall(self.INCLUDE_FILE,ins.group())[0]
            filename=None   #包含文件名
            if include_file[0]=='<':    #从默认目录包含
                filename=__include__+'\\'+include_file[1:-1]
            elif include_file[0]=='"':  #从当前目录包含
                filename=include_file[1:-1]
            pre=Preprocessor(filename)
            #计算指令所在的缩进
            indent=''
            i,_=ins.span()
            i-=1
            while i>=0 and self.content[i]!='\n':
                indent+=self.content[i]
                i-=1
            replace=pre.content.split('\n')
            for index,code in enumerate(replace):
                if index>0:
                    replace[index]=indent+replace[index]
            self.replace[ins.group()]='\n'.join(replace)

    def write(self,filename):
        with open(filename,mode='w',encoding='utf-8')as file:
            file.write(self.content)