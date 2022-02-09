from os import times
from syntaxtree import *


class Parser:
    '''
    语法分析
    '''
    justrun=True    #是否立刻运行
    def __init__(self, lexer, envir):
        self.envir = envir
        self.lexer = lexer
        self.token = self.lexer.token
        self.indent = self.lexer.indent

        Parser.sentences=self.sentences = {  # token对应的语句
            'if': self.if_sentence,
            'while': self.while_sentence,
            'try': self.try_sentence,
            'def': self.def_sentence,
            'class': self.class_sentence,
            'break': self.break_sentence,
            'continue': self.continue_sentence,
            'import': self.import_sentence,
            'return': self.return_sentence,
        }
        Parser.priority=self.priority = [  # 运算符优先级
            self.expression,
            self.conditional_expression,
            self.logical_or_expression,
            self.logical_and_expression,
            self.inclusive_or_expression,
            self.exclusive_or_expression,
            self.and_expression,
            self.relational_expression,
            self.shift_expression,
            self.additive_expression,
            self.multiplicative_expression,
            self.cast_expression,
            self.unary_expression,
            self.postfix_expression,
            self.primary_expression,
        ]

    def expect(self, expected):
        self.token = self.lexer.expect(expected)
        self.indent = self.lexer.indent
        return self.token

    def skip_block(self):
        line = self.lexer.skip_block()
        self.indent = self.lexer.indent
        self.token = self.lexer.token
        return line

    def start(self):
        t = Namespace(body=self.unkown(), parser=self)
        return t

    def body(self, indent=''):
        t = []
        while self.token != '<None>' and self.indent == indent:
            t.append(self.sentence())
        return t
        
    def sentence(self,run=None):
        if run==None:
            run=Parser.justrun
        t = None
        if self.token in self.sentences:
            func=self.sentences[self.token]
            if isinstance(func,FunctionDef):
                Parser.justrun=False
                func.run_list(func.body,False) #先解析好
                Parser.justrun=True
                t=func(self)
            else:
                t = func()
        else:
            t = self.expression_sentence()
        if run and t:
            t.run()
        return t

    def break_sentence(self):
        return Break(parser=self)

    def continue_sentence(self):
        return Continue(parser=self)

    def unkown(self):
        if self.justrun:
            startline, endline, indent = self.skip_block()
            # 先不解析代码块
            t = [Unkown(parser=self, indent=indent,
                    startlineno=startline, endlineno=endline)]
        else:
            t=self.body(indent=self.indent)
        return t

    def import_sentence(self):
        self.expect('import')
        t = Import(name=self.expression(), parser=self)
        return t

    def class_sentence(self):
        t = Class(parser=self, name='', bases=[], body=[])
        self.expect('class')
        t.name = self.token.name
        self.expect(self.token.name)
        if self.token == '(':
            self.expect('(')
            t.bases.append(self.expression())
            while self.token == ',':
                self.expect(',')
                t.bases.append(self.expression())
            self.expect(')')
        self.expect(':')
        t.body = self.unkown()
        return t

    def try_sentence(self):
        t = Try(parser=self, body=[], handles=[])
        self.expect('try')
        self.expect(':')
        t.body = self.unkown()
        while self.token == 'except':
            t.handles.append(self.except_sentence())
        return t

    def except_sentence(self):
        t = ExceptionHandle(parser=self, type=None, body=[])
        self.expect('except')
        if self.token == ':':
            self.expect(':')
            t.body = self.unkown()
        else:
            t.type = self.expression()
            self.expect(':')
            t.body = self.unkown()
        return t

    def if_sentence(self):
        t = If(exp=None, body=[], elses=[], parser=self)
        self.expect('if')
        t.exp = self.expression()
        self.expect(':')
        t.body = self.unkown()
        t.elses = []
        if self.token == 'else':
            self.expect('else')
            self.expect(':')
            p = self.unkown()[0]
            t.elses.append(p)
        return t

    def while_sentence(self):
        t = While(exp=None, body=[], parser=self)
        self.expect('while')
        t.exp = self.expression()
        self.expect(':')
        t.body = self.unkown()
        return t

    def def_sentence(self):
        t = FunctionDef(name=None, prefix=[],args=None, body=[], parser=self)
        self.expect('def')
        t.name = self.token.name
        self.expect(self.token.name)
        self.expect('(')
        t.args = self.argument_expression_list()
        self.expect(')')
        self.expect(':')
        t.body = self.unkown()
        return t

    def return_sentence(self):
        t = Return(value=None, parser=self)
        self.expect('return')
        t.value = self.expression()
        return t

    def expression_sentence(self):
        t = Expr(exp=self.expression(), parser=self)
        return t

    def expression(self, n=0):
        t = self.priority[n+1](n+1)
        if self.token == '=':
            self.expect('=')
            t.mode = 'store'
            p = Assign(targets=[t], value=self.expression(), parser=self)
            t = p
        return t

    def conditional_expression(self, n=0):
        return self.priority[n+1](n+1)

    def logical_or_expression(self, n=0):
        t = self.priority[n+1](n+1)
        if self.token == 'or':
            p = BoolOp(op='or', values=[t], parser=self)
            t = p
            while self.token == 'or':
                self.expect('or')
                t.values.append(self.priority[n+1](n+1))
        return t

    def logical_and_expression(self, n=0):
        t = self.priority[n+1](n+1)
        if self.token == 'and':
            p = BoolOp(op='and', values=[t], parser=self)
            t = p
            while self.token == 'and':
                self.expect('and')
                t.values.append(self.priority[n+1](n+1))
        return t

    def inclusive_or_expression(self, n=0):
        t = self.priority[n+1](n+1)
        while self.token == '|':
            self.expect('|')
            p = BinOp(left=t, op='|',
                      right=self.inclusive_or_expression(), parser=self)
            t = p
        return t

    def exclusive_or_expression(self, n=0):
        t = self.priority[n+1](n+1)
        while self.token == '^':
            self.expect('^')
            p = BinOp(left=t, op='^',
                      right=self.priority[n+1](n+1), parser=self)
            t = p
        return t

    def and_expression(self, n=0):
        t = self.priority[n+1](n+1)
        while self.token == '&':
            self.expect('&')
            p = BinOp(left=t, op='&',
                      right=self.priority[n+1](n+1), parser=self)
            t = p
        return t

    def relational_expression(self, n=0):
        t = self.priority[n+1](n+1)
        if self.token.name in ('>=', '<=', '==', '!=', '>', '<'):
            b = self.compares(n)
            p = Compare(left=t, ops=[i for i, j in b],
                        comparators=[j for i, j in b], parser=self)
            t = p
        return t

    def compares(self, n=0):
        result = []
        while self.token.name in ('>=', '<=', '==', '!=', '>', '<'):
            name = self.token.name
            self.expect(self.token.name)
            result.append((name, self.shift_expression(n)))
        return result

    def shift_expression(self, n=0):
        t = self.priority[n+1](n+1)
        while self.token == '>>' or self.token == '<<':
            p = BinOp(left=t, op=self.token.name, parser=self)
            self.expect(self.token.name)
            p.__dict__['right'] = self.priority[n+1](n+1)
            t = p
        return t

    def additive_expression(self, n=0):
        t = self.priority[n+1](n+1)
        while self.token.name in ('+', '-'):
            p = BinOp(left=t, op=self.token.name, parser=self)
            self.expect(self.token.name)
            p.__dict__['right'] = self.priority[n+1](n+1)
            t = p
        return t

    def multiplicative_expression(self, n=0):
        t = self.priority[n+1](n+1)
        while self.token.name in ('*', '/', '%'):
            p = BinOp(left=t, op=self.token.name, parser=self)
            self.expect(self.token.name)
            p.__dict__['right'] = self.priority[n+1](n+1)
            t = p
        return t

    def cast_expression(self, n=0):
        return self.priority[n+1](n+1)

    def unary_expression(self, n=0):
        return self.priority[n+1](n+1)

    def postfix_expression(self, n=0):
        t = self.priority[n+1](n+1)
        while self.token.name in ('[', '(', '.'):
            if self.token == '[':
                self.expect('[')
                p = Subscript(value=t, slice=self.expression(),
                              mode='load', parser=self)
                self.expect(']')
                t = p
            elif self.token == '(':
                self.expect('(')
                p = Call(func=t, args=self.argument_expression_list(), parser=self)
                self.expect(')')
                t = p
            elif self.token == '.':
                self.expect('.')
                p = Attribute(value=t, attr=self.token.name,
                              mode='load', parser=self)
                self.expect(self.token.name)
                t = p
        return t

    def argument_expression_list(self):
        t = []
        if self.token != ')':
            p = self.expression()
            t.append(p)
            while self.token == ',':
                self.expect(',')
                p = self.expression()
                t.append(p)
        return t

    def primary_expression(self, n=0):
        t = None
        if self.is_name(self.token.name):
            t = Name(id=self.token.name, mode='load', parser=self)
            self.expect(self.token.name)
        elif self.token.name.isalnum():
            t = Constant(value=int(self.token.name), parser=self)
            self.expect(self.token.name)
        elif self.token.name[0] in ('"', "'"):
            t = Constant(value=self.token.name[1:-1], parser=self)
            self.expect(self.token.name)
        elif self.token == '(':
            self.expect('(')
            if self.token != ')':
                t = self.expression()
            self.expect(')')
        elif self.token == '[':
            t = List(list=[], parser=self)
            self.expect('[')
            if self.token != ']':
                val = self.expression()
                t.list.append(val)
                while self.token == ',':
                    self.expect(',')
                    t.list.append(self.expression())
            self.expect(']')
        elif self.token == '{':
            t = Dict(keys=[], values=[], parser=self)
            self.expect('{')
            if self.token != '}':
                t.keys.append(self.expression())
                self.expect(':')
                t.values.append(self.expression())
                while self.token == ',':
                    self.expect(',')
                    t.keys.append(self.expression())
                    self.expect(':')
                    t.values.append(self.expression())
            self.expect('}')
        return t

    def is_name(self, s):
        '''
        是标识符
        '''
        if not s[0].isalpha():
            return False
        for i in range(1, len(s)):
            if not(s[i].isalpha() or s[i].isalnum() or s[i] == '_'):
                return False
        return True
