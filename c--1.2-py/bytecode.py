class ByteCode:
    '''字节码'''
    
    def __init__(self):
        self.code = []  # 代码
        self.objects = {}   # ByteCode对象

    def print(self):
        for i in range(len(self.code)):
            print('\t' + str(i) + ' ' + ' '.join([str(self.code[i][j]) for j in range(len(self.code[i])-2)]))
        for key, val in self.objects.items():
            print(key + ':')
            val.print()
