import Compiler


class Type:
    '''类型'''

    def __init__(self, **kwargs) -> None:
        self.__dict__ |= kwargs
        self.size = 0
        self._symtab = Compiler.symtab
        self._genir = Compiler.genir

    def load(self, name):
        varsym = self._symtab.get_varsym(name)
        result = []
        if varsym.deep >= 0:
            result.append(['mov', 'eax', f'[ebp-{varsym.deep}]'])
        else:
            result.append(['mov', 'eax', f'[ebp+{-varsym.deep}]'])
        return result

    def call(self, name):
        pass

    def eq(self, other):
        final_label = self._genir.get_label()
        false_label = self._genir.get_label()

        result = []
        result.append(['pop', 'ebx'])
        result.append(['pop', 'eax'])
        result.append(['cmp', 'eax', 'ebx'])
        result.append(['jz', false_label])
        result.append(['push', '1'])
        result.append(['jmp', final_label])
        result.append([false_label+':'])
        result.append(['push', '0'])
        result.append([final_label+':'])

        return result

    def or_(self, other):
        final_label = self._genir.get_label()
        false_label = self._genir.get_label()
        true_label = self._genir.get_label()

        result = []
        result.append(['pop', 'ebx'])
        result.append(['pop', 'eax'])
        result.append(['cmp', 'eax', '0'])
        result.append(['jne', true_label])
        result.append(['cmp', 'ebx', '0'])
        result.append(['je', false_label])
        result.append([true_label+':'])
        result.append(['push', '1'])
        result.append(['jmp', final_label])
        result.append([false_label+':'])
        result.append(['push', '0'])
        result.append([final_label+':'])

        return result

    def add(self, other):
        result = []

        result.append(['pop', 'ebx'])
        result.append(['pop', 'eax'])
        result.append(['add', 'eax', 'ebx'])
        result.append(['push', 'eax'])

        return result

    def sub(self, other):
        result = []

        result.append(['pop', 'ebx'])
        result.append(['pop', 'eax'])
        result.append(['sub', 'eax', 'ebx'])
        result.append(['push', 'eax'])

        return result

    def bool(self):
        result=[]
        result.append(['push','1'])
        return result

    def __str__(self) -> str:
        return f'{self.__class__.__name__}'

    __repr__ = __str__
