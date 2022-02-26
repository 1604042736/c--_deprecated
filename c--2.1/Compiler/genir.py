class GenIR:
    '''
    生成目标代码
    '''
    lbnum=0 #标签标号
    lbpro='_'   #标签前缀
    globals=['_main']
    extern=[]
    section={'.text':[]}  #段
    cursec='.text'  #当前段
    def get_label(self):
        '''
        获得标签
        '''
        lb=f'{self.lbpro}{self.lbnum}'
        self.lbnum+=1
        return lb

    def add_section(self,section_name):
        if section_name not in self.section:
            self.section[section_name]=[]

    def add_label(self,label):
        self.section[self.cursec].append(label+':')

    def add_globals(self,g):
        self.globals.append(g)

    def add_extern(self,e):
        self.extern.append(e)

    def add_inst(self,op,*args):
        self.section[self.cursec].append(f"{op} {','.join(map(str,args))}")

    def write(self,filename):
        with open(filename,encoding='utf-8',mode='w')as file:
            for i in self.globals:
                file.write(f"global {i}\n")
            for i in self.extern:
                file.write(f"extern {i}\n")
            for section,code in self.section.items():
                file.write(f'\nsection {section}:\n')
                file.write('\n'.join(code))

    def set_cursec(self,sec):
        self.cursec=sec

    def comment(self,msg):
        '''
        注释
        '''
        self.section[self.cursec].append(f';{msg}')