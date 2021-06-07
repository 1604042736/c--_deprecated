from syntaxtree import *


class Parser:
    '''语法分析'''

    def __init__(self, scanner, mode):
        self.scanner = scanner  # 词法分析器
        self.mode = mode  # 模式
        self.parse()

    def newast(self, nodetype):
        '''
        动态创建语法树类, 类名为语法树类型
        '''
        if self.tokenlen < len(self.scanner.tokens):
            return type(nodetype, (SyntaxTree,), {})(self.scanner.tokens[self.tokenlen])
        else:
            return type(nodetype, (SyntaxTree,), {})(self.scanner.tokens[-1])

    def parse(self):
        self.tokenlen = -1
        self.token = ''
        self.indent = 0
        self.match(self.token)
        self.ast = self.newast('File')
        self.ast.__dict__['body'] = self.body()

    def body(self, indent=0):
        t = []
        while self.indent == indent and self.token != '':  # 当前缩进与要求的缩进符合
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
        elif self.token == 'import':
            t = self.import_sentence()
        elif self.token == 'try':
            t = self.try_sentence()
        elif self.token == 'class':
            t = self.class_sentence()
        elif self.isval(self.token):
            t = self.assign_sentence()
        else:
            self.error(f"Unknow token '{self.token}'")
        return t

    def len_sentence(self):
        '''len(args)'''
        t = self.newast('Len')
        self.match('len')
        t.__dict__['args'] = self.args()
        return t

    def class_sentence(self):
        '''
        class name(bases):
            body
        '''
        t = self.newast('Class')
        self.match('class')
        t.__dict__['name'] = self.token
        self.match(self.token)
        t.__dict__['bases'] = []
        if self.token == '(':
            t.__dict__['bases'] = self.args()
        self.match(':')
        t.__dict__['body'] = self.body(indent=self.indent)
        return t

    def import_sentence(self):
        '''import name'''
        t = self.newast('Import')
        self.match(self.token)
        t.__dict__['name'] = self.token
        self.match(self.token)
        return t

    def if_sentence(self):
        '''
        if expr:
            body:
        else:
            body
        '''
        t = self.newast('If')
        self.match('if')
        t.__dict__['expr'] = self.expr()
        self.match(':')
        t.__dict__['body'] = self.body(indent=self.indent)
        t.__dict__['elses'] = None
        if self.token == 'else':
            self.match('else')
            self.match(':')
            t.__dict__['elses'] = self.body(indent=self.indent)
        return t

    def function_sentence(self):
        '''
        function name(args):
            body
        '''
        t = self.newast('Function')
        self.match(self.token)
        t.__dict__['name'] = self.token
        self.match(self.token)
        t.__dict__['args'] = self.args()
        self.match(':')
        t.__dict__['body'] = self.body(indent=self.indent)
        return t

    def while_sentence(self):
        '''
        while expr:
            body
        '''
        t = self.newast('While')
        self.match('while')
        t.__dict__['expr'] = self.expr()
        self.match(':')
        t.__dict__['body'] = self.body(indent=self.indent)
        return t

    def try_sentence(self):
        '''
        try:
            body
        except:
            body
        '''
        t = self.newast('Try')
        self.match(self.token)
        self.match(':')
        t.__dict__['body'] = self.body(indent=self.indent)
        t.__dict__['handlers'] = self.handlers()
        return t

    def handlers(self):
        '''异常处理方法'''
        t = []
        while self.token == 'except':
            t.append(self.except_sentence())
        return t

    def except_sentence(self):
        '''
        except:
            body
        '''
        t = self.newast('Except')
        self.match(self.token)
        self.match(':')
        t.__dict__['body'] = self.body(indent=self.indent)
        return t

    def break_sentence(self):
        t = self.newast('Break')
        self.match(self.token)
        return t

    def continue_sentence(self):
        t = self.newast('Continue')
        self.match(self.token)
        return t

    def return_sentence(self):
        t = self.newast('Return')
        self.match(self.token)
        t.__dict__['expr'] = self.expr()
        return t

    def expr(self):
        return self.or_expr()

    def or_expr(self):
        t = self.and_expr()
        while self.token == 'or':
            self.match(self.token)
            p = self.newast('Or')
            p.__dict__['left'] = t
            p.__dict__['right'] = self.and_expr()
            t = p
        return t

    def and_expr(self):
        t = self.compare_expr()
        while self.token == 'and':
            self.match('and')
            p = self.newast('And')
            p.__dict__['left'] = t
            p.__dict__['right'] = self.compare_expr()
            t = p
        return t

    def compare_expr(self):
        t = self.shift_expr()
        if self.token in ['>', '>=', '<', '<=', '==', '!=']:
            if self.token == '>':
                self.match(self.token)
                p = self.newast('Gt')
            elif self.token == '>=':
                self.match(self.token)
                p = self.newast('Geq')
            elif self.token == '<':
                self.match(self.token)
                p = self.newast('Lt')
            elif self.token == '<=':
                self.match(self.token)
                p = self.newast('Leq')
            elif self.token == '==':
                self.match(self.token)
                p = self.newast('Eq')
            elif self.token == '!=':
                self.match(self.token)
                p = self.newast('Neq')
            p.__dict__['left'] = t
            p.__dict__['right'] = self.shift_expr()
            t = p
        return t

    def shift_expr(self):
        t = self.add_expr()
        while self.token in ['>>', '<<']:
            if self.token == '>>':
                self.match(self.token)
                p = self.newast('RShift')
                p.__dict__['left'] = t
                p.__dict__['right'] = self.add_expr()
                t = p
            elif self.token == '<<':
                self.match(self.token)
                p = self.newast('LShift')
                p.__dict__['left'] = t
                p.__dict__['right'] = self.add_expr()
                t = p
        return t

    def add_expr(self):
        t = self.mul_expr()
        while self.token in ['+', '-']:
            if self.token == '+':
                self.match(self.token)
                p = self.newast('Add')
                p.__dict__['left'] = t
                p.__dict__['right'] = self.mul_expr()
                t = p
            elif self.token == '-':
                self.match(self.token)
                p = self.newast('Sub')
                p.__dict__['left'] = t
                p.__dict__['right'] = self.mul_expr()
                t = p
        return t

    def mul_expr(self):
        t = self.primary_expr()
        while self.token in ['*', '/', '%']:
            if self.token == '*':
                self.match(self.token)
                p = self.newast('Mul')
                p.__dict__['left'] = t
                p.__dict__['right'] = self.primary_expr()
                t = p
            elif self.token == '/':
                self.match(self.token)
                p = self.newast('Div')
                p.__dict__['left'] = t
                p.__dict__['right'] = self.primary_expr()
                t = p
            elif self.token == '%':
                self.match(self.token)
                p = self.newast('Mod')
                p.__dict__['left'] = t
                p.__dict__['right'] = self.primary_expr()
                t = p
        return t

    def primary_expr(self):
        t = None
        if self.token.isdigit():
            t = self.newast('Num')
            t.__dict__['content'] = self.token
            self.match(self.token)
        elif self.token[0] in ['"', "'"]:
            t = self.newast('String')
            t.__dict__['content'] = self.token[1:-1]
            self.match(self.token)
        elif self.isval(self.token):
            t = self.newast('Name')
            t.__dict__['content'] = self.token
            t.__dict__['mode'] = self.newast('Load')
            self.match(self.token)
            if self.token == '[':  # name[...]
                while self.token == '[':
                    self.match('[')
                    p = self.newast('Index')
                    p.__dict__['name'] = t
                    p.__dict__['expr'] = self.expr()
                    p.__dict__['mode'] = self.newast('Load')
                    t = p
                    self.match(']')
            elif self.token == '(':  # name(args)
                p = self.newast('Call')
                p.__dict__['name'] = t
                p.__dict__['args'] = self.args()
                t = p
            while self.token == '.':  # name.attr
                self.match('.')
                p = self.newast('Attr')
                p.__dict__['name'] = t
                p.__dict__['attr'] = self.newast('Name')
                p.__dict__['attr'].__dict__['content'] = self.token
                p.__dict__['attr'].__dict__['mode'] = self.newast('Load')
                self.match(self.token)
                p.__dict__['mode'] = self.newast('Load')
                t = p
                if self.token == '[':  # name[...]
                    while self.token == '[':
                        self.match('[')
                        p = self.newast('Index')
                        p.__dict__['name'] = t
                        p.__dict__['expr'] = self.expr()
                        p.__dict__['mode'] = self.newast('Load')
                        t = p
                        self.match(']')
                elif self.token == '(':
                    p = self.newast('Call')
                    p.__dict__['name'] = t
                    p.__dict__['args'] = self.args()
                    t = p
        elif self.token == '(':
            self.match('(')
            if self.token != ')':
                t = self.expr()
            self.match(')')
        elif self.token == '[':  # [expr]
            t = self.newast('List')
            self.match('[')
            t.__dict__['list'] = []
            if self.token != ']':
                t.__dict__['list'].append(self.expr())
                while self.token == ',':
                    self.match(',')
                    t.__dict__['list'].append(self.expr())
            self.match(']')
        elif self.token == '{':  # {key:val}
            t = self.newast('Dict')
            self.match('{')
            keys, vals = [], []
            if self.token != '}':
                keys.append(self.expr())
                self.match(':')
                vals.append(self.expr())
                while self.token == ',':
                    self.match(',')
                    keys.append(self.expr())
                    self.match(':')
                    vals.append(self.expr())
            self.match('}')
            t.__dict__['keys'] = keys
            t.__dict__['vals'] = vals
        return t

    def args(self):
        self.match('(')
        t = []
        if self.token != ')':
            t.append(self.expr())
            while self.token == ',':
                self.match(',')
                t.append(self.expr())
        self.match(')')
        return t

    def assign_sentence(self):
        t = self.newast('Name')
        t.__dict__['content'] = self.token
        t.__dict__['mode'] = self.newast('Load')
        self.match(self.token)
        if self.token == '[':  # name[...]
            while self.token == '[':
                self.match('[')
                p = self.newast('Index')
                p.__dict__['name'] = t
                p.__dict__['expr'] = self.expr()
                p.__dict__['mode'] = self.newast('Load')
                t = p
                self.match(']')
        if self.token == '(':  # name(args)
            p = self.newast('Call')
            p.__dict__['name'] = t
            p.__dict__['args'] = self.args()
            t = p
        while self.token == '.':  # name.attr
            self.match('.')
            p = self.newast('Attr')
            p.__dict__['name'] = t
            p.__dict__['attr'] = self.newast('Name')
            p.__dict__['attr'].__dict__['content'] = self.token
            p.__dict__['attr'].__dict__['mode'] = self.newast('Load')
            self.match(self.token)
            p.__dict__['mode'] = self.newast('Load')
            t = p
            if self.token == '[':  # name[...]
                while self.token == '[':
                    self.match('[')
                    p = self.newast('Index')
                    p.__dict__['name'] = t
                    p.__dict__['expr'] = self.expr()
                    p.__dict__['mode'] = self.newast('Load')
                    t = p
                    self.match(']')
            if self.token == '(':  # name(args)
                p = self.newast('Call')
                p.__dict__['name'] = t
                p.__dict__['args'] = self.args()
                t = p
        if self.token == '=':  # name=expr
            t.__dict__['mode'] = self.newast('Store')
            self.match('=')
            p = self.newast('Assign')
            p.__dict__['name'] = t
            p.__dict__['expr'] = self.expr()
            t = p
        return t

    def isval(self, s):
        if not s[0].isalpha:
            return False
        for i in s:
            if i != '_' and not i.isalpha() and not i.isdigit():
                return False
        return True

    def match(self, expected):
        if self.token != expected:
            self.error(f"Unexpected token '{self.token}', should be '{expected}'")
        self.tokenlen += 1
        # 忽略空白字符
        while self.tokenlen < len(self.scanner.tokens) and (self.scanner.tokens[self.tokenlen][0] in ['\t', ' ', '\n']):
            # 遇到换行符统计缩进
            while self.tokenlen < len(self.scanner.tokens) and self.scanner.tokens[self.tokenlen][0] == '\n':
                self.get_indent()
            # 忽略空白符, 由于该循环执行完后token可能为换行符, 所以又在外面套了一层循环
            while self.tokenlen < len(self.scanner.tokens) and (self.scanner.tokens[self.tokenlen][0] in ['\t', ' ']):
                self.tokenlen += 1
        if self.tokenlen < len(self.scanner.tokens):
            self.token = self.scanner.tokens[self.tokenlen][0]
        else:
            self.token = ''

    def get_indent(self):
        self.indent = 0
        self.tokenlen += 1
        while self.tokenlen < len(self.scanner.tokens) and (self.scanner.tokens[self.tokenlen][0] in ['\t', ' ']):
            self.indent += 1
            self.tokenlen += 1

    def error(self, msg):
        tokeninfo = self.scanner.tokens[self.tokenlen][1]
        raise Exception(f"File {self.scanner.filename}, Lineno {tokeninfo['lineno']}, Linepos {tokeninfo['linepos']}\n\t{tokeninfo['line'].strip()}\n{msg}")
