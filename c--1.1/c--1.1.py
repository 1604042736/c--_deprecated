import sys
import re


class Preprocessor:
    define = {}
    IMPORT_PATH = 'Import'

    def pretreatment(self, code):
        self.code = code
        self.defineflag = False
        self.prediv()
        return self.code

    def prediv(self):
        divs = re.findall('#.*', self.code)
        replace = {}
        for div in range(len(divs)):
            args = divs[div].replace('#', ' ').split()
            if args[0] == 'define':
                self.define[args[1]] = args[2]
                replace[divs[div]] = ''
            elif args[0] == 'undef':
                self.define.pop(args[1])
                replace[divs[div]] = ''
            elif args[0] == 'ifdef':
                if args[1] in self.define:
                    self.defineflag = True
                    replace[divs[div]] = ''
                else:
                    replace[self.code[self.code.find(divs[div]):self.code.find(divs[div + 1])]] = ''
            elif args[0] == 'ifudef':
                if args[1] not in self.define:
                    self.defineflag = True
                    replace[divs[div]] = ''
                else:
                    replace[self.code[self.code.find(divs[div]):self.code.find(divs[div + 1])]] = ''
            elif args[0] == 'elif':
                if not self.defineflag and args[1] in self.define:
                    self.defineflag = True
                    replace[divs[div]] = ''
                else:
                    replace[self.code[self.code.find(divs[div]):self.code.find(divs[div + 1])]] = ''
            elif args[0] == 'else':
                if not self.defineflag:
                    replace[divs[div]] = ''
                else:
                    replace[self.code[self.code.find(divs[div]):self.code.find(divs[div + 1])]] = ''
            elif args[0] == 'endif':
                self.defineflag = False
            elif args[0] == 'import':
                i = self.code.find(divs[div]) - 1
                s = ''
                while i >= 0 and i != '\n':
                    s += self.code[i]
                    i -= 1
                rep = ''
                with open(f'{self.IMPORT_PATH}\\{args[1]}', encoding='utf-8')as file:
                    for line in file.readlines():
                        rep += s + line
                replace[divs[div]] = rep
        for key, val in replace.items():
            self.code = self.code.replace(key, val)
        for key, val in self.define.items():
            self.code = self.code.replace(key, val)


class Scanner:
    def scan(self, code):
        i, res = 0, []
        while i < len(code):
            if code[i].isalpha():
                i, r = self.get_name(i, code)
            elif code[i].isdigit():
                i, r = self.get_num(i, code)
            elif code[i] == '=':
                if code[i + 1] == '=':
                    r, i = '==', i + 1
                else:
                    r = '='
            elif code[i] == '>':
                if code[i + 1] == '=':
                    r, i = '>=', i + 1
                elif code[i + 1] == '>':
                    r, i = '>>', i + 1
                else:
                    r = '>'
            elif code[i] == '<':
                if code[i + 1] == '=':
                    r, i = '==', i + 1
                elif code[i + 1] == '<':
                    r, i = '<<', i + 1
                else:
                    r = '<'
            elif code[i] == '!':
                if code[i + 1] == '=':
                    r, i = '==', i + 1
                else:
                    r = '!'
            elif code[i] == "'" or code[i] == '"':
                i, r = self.get_string(i, code)
            else:
                r = code[i]
            res.append(r)
            i += 1
        return res

    def get_name(self, i, code):
        res = ""
        while i < len(code) and (code[i].isalpha() or code[i].isdigit() or code[i] == '_'):
            res += code[i]
            i += 1
        return i - 1, res

    def get_num(self, i, code):
        res = ""
        while i < len(code) and code[i].isdigit():
            res += code[i]
            i += 1
        return i - 1, res

    def get_string(self, i, code):
        start, i, s = code[i], i + 1, code[i]
        while i < len(code) and code[i] != start:
            r = code[i]
            if r == '\\':
                i += 1
                r = code[i]
                if r == 'n':
                    r = '\n'
                elif r == 't':
                    r = '\t'
                elif r == 'r':
                    r = '\r'
            s += r
            i += 1
        s += start
        return i, s


class SyntaxTree:
    def __init__(self, nodetype, name=''):
        self.nodetype = nodetype
        self.name = name
        self.child = {}

    def print(self, spaces=0, step='  '):
        print(f"{step * spaces}{self.nodetype}", end='')
        if self.name:
            print(': ' + self.name)
        else:
            print()
        for key, val in self.child.items():
            print(f"{step * (spaces + 1)}{key}")
            if val:
                if isinstance(val, list):
                    for t in val:
                        t.print(spaces + 2)
                else:
                    val.print(spaces + 2)

    def __str__(self):
        s, i = f'{self.nodetype}(', 0
        if self.name:
            s += self.name
            if len(self.child) >= 1:
                s += ','
        for key, val in self.child.items():
            s += key + '='
            s += str(val)
            if i < len(self.child) - 1:
                s += ','
            i += 1
        s += ')'
        return s

    __repr__ = __str__


class Parser:
    def parse(self, code, tokens, mode, filename=None):
        self.tokenlen = -1
        self.token = ''
        self.tokens = tokens
        self.mode = mode
        self.code = code.split('\n')
        self.codeline = 1
        self.filename = filename
        self.indent = 0
        self.match(self.token)
        ast = SyntaxTree(mode)
        ast.child['body'] = self.body()
        return ast

    def match(self, expected):
        if self.token != expected:
            self.error(f"Unexpected token '{self.token}', should be '{expected}'")
        self.tokenlen += 1
        while self.tokenlen < len(self.tokens) and self.tokens[self.tokenlen] == '\n':
            self.get_indent()
        while self.tokenlen < len(self.tokens) and (self.tokens[self.tokenlen] in ['\t', ' ']):
            self.tokenlen += 1
        if self.tokenlen < len(self.tokens):
            self.token = self.tokens[self.tokenlen]
        else:
            self.token = ''

    def get_indent(self):
        self.indent = 0
        self.tokenlen += 1
        while self.tokenlen < len(self.tokens) and (self.tokens[self.tokenlen] in ['\t', ' ']):
            self.indent += 1
            self.tokenlen += 1
        if self.tokenlen < len(self.tokens):
            self.codeline += 1

    def error(self, msg):
        if self.mode == 'File':
            print(f'File "{self.filename}", Line {self.codeline}')
        elif self.mode == 'Shell':
            print(f'Shell, Line {self.codeline}')
        print('\t' + self.code[self.codeline - 1])
        print(msg)
        exit()

    def body(self, indent=0):
        t = []
        while self.indent == indent and self.token != '':
            p = self.sentence()
            if not p:
                break
            t.append(p)
        return t

    def sentence(self):
        t = None
        if self.token == 'if':
            t = self.if_sentence()
        elif self.token == 'while':
            t = self.while_sentence()
        elif self.token == 'break':
            t = self.break_sentence()
        elif self.token == 'continue':
            t = self.continue_sentence()
        elif self.token == 'return':
            t = self.return_sentence()
        elif self.token == 'function':
            t = self.function_sentence()
        elif self.token == 'print':
            t = self.print_sentence()
        elif self.token == 'input':
            t = self.input_sentence()
        elif self.token == 'import':
            t = self.import_sentence()
        elif self.token == 'try':
            t = self.try_sentence()
        elif self.isval(self.token):
            t = self.assign_sentence()
        return t

    def isval(self, s):
        if not s[0].isalpha:
            return False
        for i in s:
            if i != '_' and not i.isalpha() and not i.isdigit():
                return False
        return True

    def try_sentence(self):
        '''
        try:
            body
        except:
            body
        '''
        t = SyntaxTree('Try')
        self.match(self.token)
        self.match(':')
        t.child['body'] = self.body(indent=self.indent)
        t.child['handlers'] = self.handlers()
        return t

    def handlers(self):
        t = []
        while self.token == 'except':
            t.append(self.except_sentence())
        return t

    def except_sentence(self):
        '''
        except:
            body
        '''
        t = SyntaxTree('Except')
        self.match(self.token)
        self.match(':')
        t.child['body'] = self.body(indent=self.indent)
        return t

    def import_sentence(self):
        t = SyntaxTree('Import')
        self.match(self.token)
        t.child['name'] = SyntaxTree('Name', self.token)
        self.match(self.token)
        return t

    def print_sentence(self):
        t = SyntaxTree('Print')
        self.match(self.token)
        t.child['args'] = self.args()
        return t

    def input_sentence(self):
        t = SyntaxTree('Input')
        self.match(self.token)
        t.child['args'] = self.args()
        return t

    def if_sentence(self):
        '''
        if expr:
            body
        else:
            body
        '''
        t = SyntaxTree('If')
        self.match('if')
        t.child['expr'] = self.expr()
        self.match(':')
        t.child['body'] = self.body(indent=self.indent)
        t.child['else'] = None
        if self.token == 'else':
            self.match('else')
            self.match(':')
            t.child['else'] = self.body(indent=self.indent)
        return t

    def while_sentence(self):
        '''
        while expr:
            body
        '''
        t = SyntaxTree('While')
        self.match('while')
        t.child['expr'] = self.expr()
        self.match(':')
        t.child['body'] = self.body(indent=self.indent)
        return t

    def function_sentence(self):
        '''
        function name(args):
            body
        '''
        t = SyntaxTree('Function')
        self.match(self.token)
        t.child['name'] = SyntaxTree('Name', self.token)
        self.match(self.token)
        t.child['args'] = self.args(argtype='FuncArgs')
        self.match(':')
        t.child['body'] = self.body(indent=self.indent)
        return t

    def break_sentence(self):
        t = SyntaxTree('Break')
        self.match(self.token)
        return t

    def continue_sentence(self):
        t = SyntaxTree('Continue')
        self.match(self.token)
        return t

    def return_sentence(self):
        t = SyntaxTree('Return')
        self.match(self.token)
        t.child['expr'] = self.expr()
        return t

    def assign_sentence(self):
        t = SyntaxTree('Name', self.token)
        t.child['mode'] = SyntaxTree('Load')
        self.match(self.token)
        while self.token == '.':  # name.attr
            self.match('.')
            p = SyntaxTree('Attr')
            p.child['name'] = t
            p.child['attr'] = SyntaxTree('Name', self.token)
            p.child['attr'].child['mode'] = SyntaxTree('Load')
            self.match(self.token)
            p.child['mode'] = SyntaxTree('Load')
            t = p
            if self.token == '[':  # name[...]
                while self.token == '[':
                    self.match('[')
                    p = SyntaxTree('Index')
                    p.child['name'] = t
                    p.child['expr'] = self.expr()
                    p.child['mode'] = SyntaxTree('Load')
                    t = p
                    self.match(']')
            if self.token == '(':  # name(args)
                p = SyntaxTree('Call')
                p.child['name'] = t
                p.child['args'] = self.args()
                t = p
            if self.token == '=':  # name=expr
                t.child['mode'].nodetype = 'Store'
                self.match('=')
                p = SyntaxTree('Assign')
                p.child['name'] = t
                p.child['expr'] = self.expr()
                t = p
        if self.token == '[':  # name[...]
            while self.token == '[':
                self.match('[')
                p = SyntaxTree('Index')
                p.child['name'] = t
                p.child['expr'] = self.expr()
                p.child['mode'] = SyntaxTree('Load')
                t = p
                self.match(']')
        if self.token == '(':  # name(args)
            p = SyntaxTree('Call')
            p.child['name'] = t
            p.child['args'] = self.args()
            t = p
        if self.token == '=':  # name=expr
            t.child['mode'].nodetype = 'Store'
            self.match('=')
            p = SyntaxTree('Assign')
            p.child['name'] = t
            p.child['expr'] = self.expr()
            t = p
        return t

    def args(self, argtype='CallArgs'):
        self.match('(')
        t = []
        if self.token != ')':
            t.append(self.expr())
            while self.token == ',':
                self.match(',')
                t.append(self.expr())
        self.match(')')
        return t

    def expr(self):
        return self.or_expr()

    def or_expr(self):
        t = self.and_expr()
        if self.token == 'or':
            self.match('or')
            p = SyntaxTree('Or')
            p.child['left'] = t
            p.child['right'] = self.and_expr()
            t = p
        return t

    def and_expr(self):
        t = self.compare_expr()
        if self.token == 'and':
            self.match('and')
            p = SyntaxTree('And')
            p.child['left'] = t
            p.child['right'] = self.compare_expr()
            t = p
        return t

    def compare_expr(self):
        t = self.shift_expr()
        if self.token in ['>', '>=', '<', '<=', '==', '!=']:
            if self.token == '>':
                self.match(self.token)
                p = SyntaxTree('Gt')
            elif self.token == '>=':
                self.match(self.token)
                p = SyntaxTree('Geq')
            elif self.token == '<':
                self.match(self.token)
                p = SyntaxTree('Lt')
            elif self.token == '<=':
                self.match(self.token)
                p = SyntaxTree('Leq')
            elif self.token == '==':
                self.match(self.token)
                p = SyntaxTree('Eq')
            elif self.token == '!=':
                self.match(self.token)
                p = SyntaxTree('Neq')
            p.child['left'] = t
            p.child['right'] = self.shift_expr()
            t = p
        return t

    def shift_expr(self):
        t = self.add_sub_expr()
        while self.token in ['>>', '<<']:
            if self.token == '>>':
                self.match(self.token)
                p = SyntaxTree('RShift')
                p.child['left'] = t
                p.child['right'] = self.add_sub_expr()
                t = p
            elif self.token == '<<':
                self.match(self.token)
                p = SyntaxTree('LShift')
                p.child['left'] = t
                p.child['right'] = self.add_sub_expr()
                t = p
        return t

    def add_sub_expr(self):
        t = self.mul_div_mod_expr()
        while self.token in ['+', '-']:
            if self.token == '+':
                self.match(self.token)
                p = SyntaxTree('Add')
                p.child['left'] = t
                p.child['right'] = self.add_sub_expr()
                t = p
            elif self.token == '-':
                self.match(self.token)
                p = SyntaxTree('Sub')
                p.child['left'] = t
                p.child['right'] = self.add_sub_expr()
                t = p
        return t

    def mul_div_mod_expr(self):
        t = self.primary_expr()
        while self.token in ['*', '/', '%']:
            if self.token == '*':
                self.match(self.token)
                p = SyntaxTree('Mul')
                p.child['left'] = t
                p.child['right'] = self.primary_expr()
                t = p
            elif self.token == '/':
                self.match(self.token)
                p = SyntaxTree('Div')
                p.child['left'] = t
                p.child['right'] = self.primary_expr()
                t = p
            elif self.token == '%':
                self.match(self.token)
                p = SyntaxTree('Mode')
                p.child['left'] = t
                p.child['right'] = self.primary_expr()
                t = p
        return t

    def primary_expr(self):
        t = None
        if self.token.isdigit():
            t = SyntaxTree('Num', self.token)
            self.match(self.token)
        elif self.token[0] == '"' or self.token[0] == "'":
            t = SyntaxTree('String', self.token[1:-1])
            self.match(self.token)
        elif self.isval(self.token):
            t = SyntaxTree('Name', self.token)
            t.child['mode'] = SyntaxTree('Load')
            self.match(self.token)
            while self.token == '.':  # name.attr
                self.match('.')
                p = SyntaxTree('Attr')
                p.child['name'] = t
                p.child['attr'] = SyntaxTree('Name', self.token)
                p.child['attr'].child['mode'] = SyntaxTree('Load')
                self.match(self.token)
                p.child['mode'] = SyntaxTree('Load')
                t = p
                if self.token == '[':  # name[...]
                    while self.token == '[':
                        self.match('[')
                        p = SyntaxTree('Index')
                        p.child['name'] = t
                        p.child['expr'] = self.expr()
                        p.child['mode'] = SyntaxTree('Load')
                        t = p
                        self.match(']')
                elif self.token == '(':
                    p = SyntaxTree('Call')
                    p.child['name'] = t
                    p.child['args'] = self.args()
                    t = p
            if self.token == '[':  # name[...]
                while self.token == '[':
                    self.match('[')
                    p = SyntaxTree('Index')
                    p.child['name'] = t
                    p.child['expr'] = self.expr()
                    p.child['mode'] = SyntaxTree('Load')
                    t = p
                    self.match(']')
            elif self.token == '(':
                p = SyntaxTree('Call')
                p.child['name'] = t
                p.child['args'] = self.args()
                t = p
        elif self.token == '(':
            self.match('(')
            if self.token != ')':
                t = self.expr()
            self.match(')')
        elif self.token == '[':
            t = SyntaxTree('List')
            self.match('[')
            t.child['list'] = []
            if self.token != ']':
                t.child['list'].append(self.expr())
                while self.token == ',':
                    self.match(',')
                    t.child['list'].append(self.expr())
            self.match(']')
        return t


class Compiler:
    def compile(self, ast):
        self.bytecode = []
        self.function = {}
        self._compile(ast)
        return self.bytecode, self.function

    def _compile(self, ast):
        if not ast:
            return
        if isinstance(ast, list):
            for child in ast:
                self._compile(child)
            return
        nodetype = ast.nodetype
        if nodetype == 'Shell':
            self._compile(ast.child['body'])
        elif nodetype == 'File':
            self._compile(ast.child['body'])
        elif nodetype == 'String':
            self.bytecode.append(['LOAD_STRING', ast.name])
        elif nodetype == 'Print':
            for child in ast.child['args']:
                self._compile(child)
                self.bytecode.append(['PRINT'])
        elif nodetype == 'Assign':
            self._compile(ast.child['expr'])
            self._compile(ast.child['name'])
        elif nodetype == 'Num':
            self.bytecode.append(['LOAD_NUM', int(ast.name)])
        elif nodetype == 'Name':
            if ast.child['mode'].nodetype == 'Load':
                self.bytecode.append(['LOAD_NAME', ast.name])
            elif ast.child['mode'].nodetype == 'Store':
                self.bytecode.append(['STORE_NAME', ast.name])
        elif nodetype == 'Or':
            self._compile(ast.child['right'])
            self._compile(ast.child['left'])
            self.bytecode.append(['OR'])
        elif nodetype == 'And':
            self._compile(ast.child['right'])
            self._compile(ast.child['left'])
            self.bytecode.append(['AND'])
        elif nodetype == 'Add':
            self._compile(ast.child['right'])
            self._compile(ast.child['left'])
            self.bytecode.append(['ADD'])
        elif nodetype == 'Sub':
            self._compile(ast.child['right'])
            self._compile(ast.child['left'])
            self.bytecode.append(['SUB'])
        elif nodetype == 'Mul':
            self._compile(ast.child['right'])
            self._compile(ast.child['left'])
            self.bytecode.append(['MUL'])
        elif nodetype == 'Div':
            self._compile(ast.child['right'])
            self._compile(ast.child['left'])
            self.bytecode.append(['DIV'])
        elif nodetype == 'MOD':
            self._compile(ast.child['right'])
            self._compile(ast.child['left'])
            self.bytecode.append(['MOD'])
        elif nodetype == 'RShift':
            self._compile(ast.child['right'])
            self._compile(ast.child['left'])
            self.bytecode.append(['RSHIFT'])
        elif nodetype == 'LShift':
            self._compile(ast.child['right'])
            self._compile(ast.child['left'])
            self.bytecode.append(['LSHIFT'])
        elif nodetype == 'Gt':
            self._compile(ast.child['right'])
            self._compile(ast.child['left'])
            self.bytecode.append(['GT'])
        elif nodetype == 'Geq':
            self._compile(ast.child['right'])
            self._compile(ast.child['left'])
            self.bytecode.append(['GEQ'])
        elif nodetype == 'Lt':
            self._compile(ast.child['right'])
            self._compile(ast.child['left'])
            self.bytecode.append(['LT'])
        elif nodetype == 'Leq':
            self._compile(ast.child['right'])
            self._compile(ast.child['left'])
            self.bytecode.append(['LEQ'])
        elif nodetype == 'Eq':
            self._compile(ast.child['right'])
            self._compile(ast.child['left'])
            self.bytecode.append(['EQ'])
        elif nodetype == 'Neq':
            self._compile(ast.child['right'])
            self._compile(ast.child['left'])
            self.bytecode.append(['NEQ'])
        elif nodetype == 'If':
            self._compile(ast.child['expr'])
            jmpindex = len(self.bytecode)
            self._compile(ast.child['body'])
            self.bytecode.insert(jmpindex, ['CMP', len(self.bytecode) + 1])
            jmpindex = len(self.bytecode)
            self._compile(ast.child['else'])
            self.bytecode.insert(jmpindex, ['JMP', len(self.bytecode)])
        elif nodetype == 'While':
            backindex = len(self.bytecode) - 1
            self._compile(ast.child['expr'])
            jmpindex = len(self.bytecode)
            self._compile(ast.child['body'])
            self.bytecode.append(['JMP', backindex])
            self.bytecode.insert(jmpindex, ['CMP', len(self.bytecode)])
        elif nodetype == 'Function':
            bc = []
            for child in ast.child['args']:
                bc += Compiler().compile(child)[0]
            for i in range(len(bc)):
                if bc[i][0] == 'LOAD_NAME':
                    bc[i][0] = 'LOAD_ARG'
            self.function[ast.child['name'].name] = (bc, {}), Compiler().compile(ast.child['body'])
            self.bytecode.append(['FUNCTION', ast.child['name'].name])
        elif nodetype == 'Call':
            self._compile(ast.child['args'])
            self._compile(ast.child['name'])
            self.bytecode.append(['CALL'])
        elif nodetype == 'Return':
            self._compile(ast.child['expr'])
            self.bytecode.append(['RETURN'])
        elif nodetype == 'List':
            for child in ast.child['list']:
                self._compile(child)
            self.bytecode.append(['LIST', len(ast.child['list'])])
        elif nodetype == 'Index':
            self._compile(ast.child['expr'])
            self._compile(ast.child['name'])
            if ast.child['mode'].nodetype == 'Load':
                self.bytecode.append(['LOAD_INDEX'])
            elif ast.child['mode'].nodetype == 'Store':
                self.bytecode.append(['STORE_INDEX'])
        elif nodetype == 'Attr':
            self._compile(ast.child['name'])
            if ast.child['mode'].nodetype == 'Load':
                self.bytecode.append(['LOAD_ATTR', ast.child['attr'].name])
            elif ast.child['mode'].nodetype == 'Store':
                self.bytecode.append(['STORE_ATTR', ast.child['attr'].name])


class Evaler:
    def __init__(self):
        self.stack = []
        self.args = []
        self.local = {}

    def eval(self, bytecode, function):
        try:
            return self._eval(bytecode, function)
        except KeyError as e:
            print(f'name {e} is not defined')
        except Exception as e:
            print(e)

    def _eval(self, bytecode, function):
        i = 0
        while i < len(bytecode):
            code = bytecode[i]
            if code[0] == 'LOAD_STRING':
                self.stack.append(code[1])
            elif code[0] == 'LOAD_NUM':
                self.stack.append(code[1])
            elif code[0] == 'LOAD_NAME':
                self.stack.append(self.local[code[1]])
            elif code[0] == 'STORE_NAME':
                self.local[code[1]] = self.stack.pop()
            elif code[0] == 'PRINT':
                print(self.stack.pop(), end='')
            elif code[0] == 'OR':
                self.stack.append(self.stack.pop() or self.stack.pop())
            elif code[0] == 'AND':
                self.stack.append(self.stack.pop() and self.stack.pop())
            elif code[0] == 'ADD':
                self.stack.append(self.stack.pop() + self.stack.pop())
            elif code[0] == 'SUB':
                self.stack.append(self.stack.pop() - self.stack.pop())
            elif code[0] == 'DIV':
                self.stack.append(self.stack.pop() / self.stack.pop())
            elif code[0] == 'MUL':
                self.stack.append(self.stack.pop() * self.stack.pop())
            elif code[0] == 'MOD':
                self.stack.append(self.stack.pop() % self.stack.pop())
            elif code[0] == 'RSHIFT':
                self.stack.append(self.stack.pop() >> self.stack.pop())
            elif code[0] == 'LSHIFT':
                self.stack.append(self.stack.pop() << self.stack.pop())
            elif code[0] == 'GT':
                self.stack.append(self.stack.pop() > self.stack.pop())
            elif code[0] == 'GEQ':
                self.stack.append(self.stack.pop() >= self.stack.pop())
            elif code[0] == 'LT':
                self.stack.append(self.stack.pop() < self.stack.pop())
            elif code[0] == 'LEQ':
                self.stack.append(self.stack.pop() <= self.stack.pop())
            elif code[0] == 'EQ':
                self.stack.append(self.stack.pop() == self.stack.pop())
            elif code[0] == 'NEQ':
                self.stack.append(self.stack.pop() != self.stack.pop())
            elif code[0] == 'CMP':
                if not self.stack.pop():
                    i = code[1]
            elif code[0] == 'JMP':
                i = code[1]
            elif code[0] == 'FUNCTION':
                self.local[code[1]] = function[code[1]]
            elif code[0] == 'CALL':
                funcname = self.stack.pop()
                arglen = len(function[funcname][0][0])
                args = []
                for j in range(arglen):
                    args.append(self.stack.pop())
                evaler = Evaler()
                evaler.args = args
                evaler.eval(function[funcname][0][0], function[funcname][0][1])
                self.stack.append(evaler.eval(function[funcname][1][0], function[funcname][1][1] | function))
            elif code[0] == 'LOAD_ARG':
                self.local[code[1]] = self.args.pop()
            elif code[0] == 'RETURN':
                return self.stack.pop()
            elif code[0] == 'LIST':
                l = []
                for j in range(code[1]):
                    l.insert(0, self.stack.pop())
                self.stack.append(l)
            elif code[0] == 'LOAD_INDEX':
                self.stack.append(self.stack.pop()[self.stack.pop()])
            elif code[0] == 'STORE_INDEX':
                self.stack.pop()[self.stack.pop()] = self.stack.pop()
            elif code[0] == 'LOAD_ATTR':
                attr = code[1]
                name = self.stack.pop()
                self.stack.append(name.__getattribute__(attr))
            elif code[0] == 'STORE_ATTR':
                attr = code[1]
                name = self.stack.pop()
                name.__dict__[attr] = self.stack.pop()
            i += 1


class Interpreter:
    def __init__(self):
        self.evaler = Evaler()

    def interpret(self, code, mode, filename=None):
        pre = Preprocessor().pretreatment(code)
        tokens = Scanner().scan(pre)
        ast = Parser().parse(pre, tokens, mode, filename)
        ast.print()
        print(ast)
        bytecode = Compiler().compile(ast)
        self.printbytecode(bytecode)
        self.evaler.eval(bytecode[0], bytecode[1])

    def printbytecode(self, bytecode):
        for i in range(len(bytecode[0])):
            print('\t' + str(i) + ' ' + ' '.join([str(j) for j in bytecode[0][i]]))
        for key, val in bytecode[-1].items():
            print(key + ':')
            self.printbytecode(val[0])
            self.printbytecode(val[1])


def shell():
    interpreter = Interpreter()
    while True:
        interpreter.interpret(input('>>> '), 'Shell')


if __name__ == '__main__':
    if len(sys.argv) == 1:
        shell()
    else:
        Interpreter().interpret(open(sys.argv[1], encoding='utf-8').read(), 'File', sys.argv[1])
