from bytecode import *


class Compiler:
    '''编译器, 将语法树编译成字节码'''

    def compile(self, ast):
        self.bytecode = ByteCode()
        self._compile(ast)
        return self.bytecode

    def _compile(self, ast):
        if type(ast).__name__ == 'File':
            for t in ast.body:
                self._compile(t)
        elif type(ast).__name__ == 'Shell':
            for t in ast.body:
                self._compile(t)
        elif type(ast).__name__ == 'String':
            self.addargs(['LOAD_STRING', ast.content], ast)
        elif type(ast).__name__ == 'Num':
            self.addargs(['LOAD_NUM', int(ast.content)], ast)
        elif type(ast).__name__ == 'Name':
            if type(ast.mode).__name__ == 'Load':
                self.addargs(['LOAD_NAME', ast.content], ast)
            elif type(ast.mode).__name__ == 'Store':
                self.addargs(['STORE_NAME', ast.content], ast)
        elif type(ast).__name__ == 'Assign':
            self._compile(ast.expr)
            self._compile(ast.name)
        elif type(ast).__name__ == 'List':
            for t in ast.list:
                self._compile(t)
            self.bytecode.code.append(['LIST', len(ast.list)])
        elif type(ast).__name__ in ['Add', 'Sub', 'Mul', 'Div', 'Mod', 'And', 'Or', 'Eq', 'Neq', 'Lt', 'Leq', 'Gt',
                                    'Geq', 'RShift', 'LShift']:
            self._compile(ast.right)
            self._compile(ast.left)
            self.addargs([type(ast).__name__.upper()], ast)
        elif type(ast).__name__ == 'If':
            self._compile(ast.expr)
            back_index = len(self.bytecode.code)
            for t in ast.body:
                self._compile(t)
            self.insertargs(back_index, ['CMP', len(self.bytecode.code) - back_index + 1], ast)  # 不符合条件就跳过if的body
            back_index = len(self.bytecode.code)
            if ast.elses:
                for t in ast.elses:
                    self._compile(t)
            self.insertargs(back_index, ['JMP', len(self.bytecode.code) - back_index], ast)  # 跳过else的body
        elif type(ast).__name__ == 'While':
            jmp_index = len(self.bytecode.code)
            self._compile(ast.expr)
            back_index = len(self.bytecode.code)
            for t in ast.body:
                self._compile(t)
            self.insertargs(back_index, ['CMP', len(self.bytecode.code) - back_index + 1], ast)
            self.addargs(['JMP', jmp_index - len(self.bytecode.code) - 1], ast)
        elif type(ast).__name__ == 'Index':
            self._compile(ast.expr)
            self._compile(ast.name)
            if type(ast.mode).__name__ == 'Load':
                self.addargs(['LOAD_INDEX'], ast)
            elif type(ast.mode).__name__ == 'Store':
                self.addargs(['STORE_INDEX'], ast)
        elif type(ast).__name__ == 'Function':
            argbytecode = []
            for t in ast.args:
                argbytecode.append(['LOAD_ARG', t.content] + ast.token)
            self.bytecode.objects[ast.name] = ByteCode()
            bodycode = Compiler().compile(ast.body)
            self.bytecode.objects[ast.name].code = argbytecode + bodycode.code
            self.bytecode.objects[ast.name].objects = bodycode.objects
            self.addargs(['FUNCTION', ast.name], ast)
        elif type(ast).__name__ == 'list':
            for t in ast:
                self._compile(t)
        elif type(ast).__name__ == 'Return':
            self._compile(ast.expr)
            self.addargs(['RETURN'], ast)
        elif type(ast).__name__ == 'Call':
            self._compile(ast.args)
            self._compile(ast.name)
            self.addargs(['CALL', len(ast.args)], ast)
        elif type(ast).__name__ == 'Try':
            back_index = len(self.bytecode.code)
            self._compile(ast.body)
            self.insertargs(back_index, ['SET_TRY', len(self.bytecode.code) - back_index + 1], ast)
            jmp_index = len(self.bytecode.code)
            self._compile(ast.handlers)
            self.insertargs(jmp_index, ['JMP', len(self.bytecode.code) - jmp_index], ast)
            self.addargs(['CLEAR_TRY'], ast)
        elif type(ast).__name__ == 'Except':
            self._compile(ast.body)
        elif type(ast).__name__ == 'Dict':
            i = len(ast.vals) - 1
            while i >= 0:
                self._compile(ast.vals[i])
                self._compile(ast.keys[i])
                i -= 1
            self.addargs(['DICT', len(ast.vals)], ast)
        elif type(ast).__name__ == 'Class':
            self.bytecode.objects[ast.name] = Compiler().compile(ast.body)
            for t in ast.bases:
                self.bytecode.objects[ast.name].code.append(['LOAD_NAME', t.content] + t.token)
                self.bytecode.objects[ast.name].code.append(['CALL', 0] + t.token)  # 调用父类
                self.bytecode.objects[ast.name].code.append(['MERGE'] + t.token)  # 将父类的局部变量合并到子类的局部变量中
            self.addargs(['CLASS', ast.name], ast)
            self.bytecode.objects[ast.name].code.append(['RETURN_SELF'] + ast.token)
        elif type(ast).__name__ == 'Attr':
            self._compile(ast.name)
            if type(ast.mode).__name__ == 'Load':
                self.addargs(['LOAD_ATTR', ast.attr.content], ast)
            elif type(ast.mode).__name__ == 'Store':
                self.addargs(['STORE_ATTR', ast.attr.content], ast)
        elif type(ast).__name__ == 'Import':
            self.addargs(['IMPORT', ast.name], ast)

    def addargs(self, args, ast):
        self.bytecode.code.append(args + ast.token)

    def insertargs(self, index, args, ast):
        self.bytecode.code.insert(index, args + ast.token)
