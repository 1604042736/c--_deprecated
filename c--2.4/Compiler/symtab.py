class Symtab:
    '''符号表'''
    all_scopes = []  # 所有的作用域

    def __init__(self):
        self.varsyms = {}  # 变量符号
        self.deep = 0  # 深度
        self.scope = [self]
        Symtab.all_scopes.append(self)  # 相当于全局作用域

    def add_varsym(self, var):
        '''添加变量符号'''
        var.deep = self.deep
        self.scope[-1].varsyms[var.name] = var
        self.deep += var.type.size

    def get_varsym(self, name):
        # 从后往前查找作用域
        i = len(self.scope)-1
        while i >= 0:
            if self.scope[i].varsyms.get(name):
                return self.scope[i].varsyms.get(name)
            i -= 1
        return None

    def add_scope(self, scope=None):
        if scope == None:
            scope = Scope()
            self.scope.append(scope)
            Symtab.all_scopes.append(scope)
        else:
            self.scope.append(scope)
            Symtab.all_scopes.append(scope)

    def pop_scope(self):
        self.scope.pop()

    def print(self):
        print(f'Scope: {id(self)},deep={self.deep}')
        attr = ['name', 'type']
        print('\t'.join(attr))
        for key, val in self.varsyms.items():
            for i in attr:
                print(val.__dict__[i], end='\t')
            print()


class VarSym:
    '''变量符号'''

    def __init__(self, name, type, deep=0) -> None:
        self.name = name  # 名称
        self.type = type  # 类型
        self.deep = deep  # 深度


class Scope(Symtab):
    '''作用域'''

    def __init__(self):
        self.varsyms = {}  # 变量符号
        self.deep = 0  # 深度
