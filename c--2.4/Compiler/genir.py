import Compiler
from bool_ import *
from int_ import Int


class GenIR:
    '''中间代码'''
    label = 0  # 标签

    def __init__(self) -> None:
        self.code = []  # 代码
        self._symtab = Compiler.symtab

    def add_code(self, *code):
        self.code.append(list(code))

    def get_label(self):
        self.label += 1
        return f'_{self.label}'

    def print(self):
        for i in self.code:
            print(' '.join(map(str, i)))

    def add_label(self, label):
        self.code.append([label+':'])

    def toasm(self, filename):
        '''转换成汇编'''
        funcount = None  # 函数
        type_stack = []
        with open(filename, mode='w', encoding='utf-8')as file:
            for i in self.code:
                ins = i[0]
                args = i[1:]
                if ins[-1] == ':':
                    if varsym := self._symtab.get_varsym(ins[:-1]):  # 是函数
                        if funcount:  # 给上一个函数写结尾
                            self.write_ins(file, 'mov', 'esp', 'ebp')
                            self.write_ins(file, 'pop', 'ebp')
                            self.write_ins(file, 'ret')
                            self._symtab.pop_scope()
                        self._symtab.add_scope(varsym.type.scope)
                        self.write_ins(file, ins)
                        self.write_ins(file, 'push', 'ebp')
                        self.write_ins(file, 'mov', 'ebp', 'esp')
                        self.write_ins(file, 'sub', 'esp', varsym.type.size)
                        funcount = varsym
                    else:
                        self.write_ins(file, ins)
                elif ins == 'LOAD_NAME':
                    name = args[0]
                    varsym = self._symtab.get_varsym(name)
                    self.write_ins(file, varsym.type.load(name))
                    self.write_ins(file, 'push', 'eax')
                    type_stack.append(varsym.type)
                elif ins == 'LOAD_VALUE':
                    # TODO 处理string
                    self.write_ins(file, 'mov', 'eax', args[0])
                    self.write_ins(file, 'push', 'eax')
                    type_stack.append(Int())
                elif ins == 'CALL':
                    varsym = self._symtab.get_varsym(args[0])
                    self.write_ins(file, varsym.type.call(args[0]))
                    self.write_ins(file,'add','esp',args[1])
                    self.write_ins(file, 'push', 'eax')
                    type_stack.append(varsym.type.return_type)
                elif ins == 'JUMP':
                    self.write_ins(file, 'jmp', args[0])
                elif ins == 'EQ':
                    type1 = type_stack.pop()
                    type2 = type_stack.pop()
                    self.write_ins(file, type2.eq(type1))
                    type_stack.append(Bool())
                elif ins == 'OR':
                    type1 = type_stack.pop()
                    if not isinstance(type1, Bool):
                        self.write_ins(file, type1.bool())
                    type2 = type_stack.pop()
                    if not isinstance(type2, Bool):
                        self.write_ins(file, type2.bool())
                    self.write_ins(file, type2.or_(type1))
                    type_stack.append(type2)
                elif ins == 'ADD':
                    type1 = type_stack.pop()
                    type2 = type_stack.pop()
                    self.write_ins(file, type2.add(type1))
                    type_stack.append(type2)
                elif ins == 'RETURN':
                    self.write_ins(file, 'ret')
                elif ins == 'SUB':
                    type1 = type_stack.pop()
                    type2 = type_stack.pop()
                    self.write_ins(file, type2.sub(type1))
                    type_stack.append(type2)
                elif ins == 'JUMP_IF_FALSE':
                    self.write_ins(file, 'pop', 'eax')
                    self.write_ins(file, 'cmp', 'eax', '0')
                    self.write_ins(file, 'je', args[0])
                elif ins == 'STORE_TEMP':
                    self.write_ins(file, 'mov', 'ecx', 'eax')
                elif ins == 'LOAD_TEMP':
                    self.write_ins(file, 'push', 'ecx')
            if funcount:  # 给上一个函数写结尾
                self.write_ins(file, 'mov', 'esp', 'ebp')
                self.write_ins(file, 'pop', 'ebp')
                self.write_ins(file, 'ret')
                self._symtab.pop_scope()

    def write_ins(self, file, ins, *args):
        '''写入指令'''
        if isinstance(ins, list):
            for i in ins:
                self.write_ins(file, *i)
        else:
            print(ins+' '+','.join(map(str, args)))
            file.write(ins+' '+','.join(map(str, args))+'\n')
