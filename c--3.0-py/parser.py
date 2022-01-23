from ast import ExceptHandler
from syntaxtree import Assign, Attribute, BinOp, BoolOp, Break, Call, Class, Compare, Constant, Continue, Dict, ExceptionHandle, Expr, FunctionDef, If, List, Name, Namespace, Return, Sentence, Subscript, SyntaxTree, Try, Unkown, While, compileandrun


class Parser:
    '''
    语法分析
    '''

    def __init__(self, lexer, envir):
        self.envir = envir
        self.lexer = lexer
        self.token = self.lexer.token
        self.indent = self.lexer.indent

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
        t = Namespace(body=self.body(),parser=self)
        return t

    def body(self, indent=''):
        t = []
        while self.token != '<None>' and self.indent == indent:
            t.append(self.sentence())
        return t

    @compileandrun
    def sentence(self):
        t = None
        if self.token == 'if':
            t = self.if_sentence()
        elif self.token == 'while':
            t = self.while_sentence()
        elif self.token == 'break':
            t = Break(parser=self)
        elif self.token == 'continue':
            t = Continue(parser=self)
        elif self.token == 'def':
            t = self.funcdef_sentence()
        elif self.token == 'return':
            t = self.return_sentence()
        elif self.token=='try':
            t=self.try_sentence()
        elif self.token=='class':
            t=self.class_sentence()
        else:
            t = self.expression_sentence()
        return t

    def unkown(self):
        startline, endline, indent = self.skip_block()
        # 先不解析代码块
        t = Unkown(parser=self, indent=indent, startlineno=startline, endlineno=endline)
        return t

    @compileandrun
    def class_sentence(self):
        t=Class(parser=self,name='',body=[])
        self.expect('class')
        t.name=self.token.name
        self.expect(self.token.name)
        self.expect(':')
        t.body=[self.unkown()]
        return t

    @compileandrun
    def try_sentence(self):
        t=Try(parser=self,body=[],handles=[])
        self.expect('try')
        self.expect(':')
        t.body=[self.unkown()]
        while self.token=='except':
            t.handles.append(self.except_sentence())
        return t

    def except_sentence(self):
        t=ExceptionHandle(parser=self,type=None,body=[])
        self.expect('except')
        if self.token==':':
            self.expect(':')
            t.body=[self.unkown()]
        else:
            t.type=self.expression()
            self.expect(':')
            t.body=[self.unkown()]
        return t

    @compileandrun
    def if_sentence(self):
        t=If(exp=None,body=[],elses=[],parser=self)
        self.expect('if')
        t.exp = self.expression()
        self.expect(':')
        t.body = [self.unkown()]
        t.elses = []
        if self.token == 'else':
            self.expect('else')
            self.expect(':')
            p = self.unkown()
            t.elses.append(p)
        return t

    @compileandrun
    def while_sentence(self):
        t = While(exp=None, body=[],parser=self)
        self.expect('while')
        t.exp = self.expression()
        self.expect(':')
        t.body = [self.unkown()]
        return t

    @compileandrun
    def funcdef_sentence(self):
        t = FunctionDef(name=None, args=None, body=[],parser=self)
        self.expect('def')
        t.name = self.token.name
        self.expect(self.token.name)
        self.expect('(')
        t.args = self.argument_expression_list()
        self.expect(')')
        self.expect(':')
        t.body = [self.unkown()]
        return t

    @compileandrun
    def return_sentence(self):
        t = Return(value=None,parser=self)
        self.expect('return')
        t.value = self.expression()
        return t

    @compileandrun
    def expression_sentence(self):
        t = self.expression()
        if not isinstance(t, Sentence):
            t = Expr(exp=t,parser=self)
        return t

    @compileandrun
    def expression(self):
        t = self.conditional_expression()
        if self.token == '=':
            self.expect('=')
            t.mode = 'store'
            p = Assign(targets=[t], value=self.expression(),parser=self)
            t = p
        return t

    @compileandrun
    def conditional_expression(self):
        return self.logical_or_expression()

    @compileandrun
    def logical_or_expression(self):
        t = self.logical_and_expression()
        if self.token == 'or':
            p = BoolOp(op='or', values=[t],parser=self)
            t = p
            while self.token == 'or':
                self.expect('or')
                t.values.append(self.logical_and_expression())
        return t

    @compileandrun
    def logical_and_expression(self):
        t = self.inclusive_or_expression()
        if self.token == 'and':
            p = BoolOp(op='and', values=[t],parser=self)
            t = p
            while self.token == 'and':
                self.expect('and')
                t.values.append(self.inclusive_or_expression())
        return t

    @compileandrun
    def inclusive_or_expression(self):
        t = self.exclusive_or_expression()
        while self.token == '|':
            self.expect('|')
            p = BinOp(left=t, op='|', right=self.inclusive_or_expression(),parser=self)
            t = p
        return t

    @compileandrun
    def exclusive_or_expression(self):
        t = self.and_expression()
        while self.token == '^':
            self.expect('^')
            p = BinOp(left=t, op='^', right=self.and_expression(),parser=self)
            t = p
        return t

    @compileandrun
    def and_expression(self):
        t = self.relational_expression()
        while self.token == '&':
            self.expect('&')
            p = BinOp(left=t, op='&', right=self.relational_expression(),parser=self)
            t = p
        return t

    @compileandrun
    def relational_expression(self):
        t = self.shift_expression()
        if self.token.name in ('>=', '<=', '==', '!=', '>', '<'):
            b = self.compares()
            p = Compare(left=t, ops=[i for i, j in b],
                        comparators=[j for i, j in b],parser=self)
            t = p
        return t

    def compares(self):
        result = []
        while self.token.name in ('>=', '<=', '==', '!=', '>', '<'):
            name = self.token.name
            self.expect(self.token.name)
            result.append((name, self.shift_expression()))
        return result

    @compileandrun
    def shift_expression(self):
        t = self.additive_expression()
        while self.token == '>>' or self.token == '<<':
            p = BinOp(left=t, op=self.token.name,parser=self)
            self.expect(self.token.name)
            p.__dict__['right'] = self.additive_expression()
            t = p
        return t

    @compileandrun
    def additive_expression(self):
        t = self.multiplicative_expression()
        while self.token.name in ('+', '-'):
            p = BinOp(left=t, op=self.token.name,parser=self)
            self.expect(self.token.name)
            p.__dict__['right'] = self.multiplicative_expression()
            t = p
        return t

    @compileandrun
    def multiplicative_expression(self):
        t = self.cast_expression()
        while self.token.name in ('*', '/', '%'):
            p = BinOp(left=t, op=self.token.name,parser=self)
            self.expect(self.token.name)
            p.__dict__['right'] = self.cast_expression()
            t = p
        return t

    def cast_expression(self):
        return self.unary_expression()

    def unary_expression(self):
        return self.postfix_expression()

    @compileandrun
    def postfix_expression(self):
        t = self.primary_expression()
        while self.token.name in ('[', '(', '.'):
            if self.token == '[':
                self.expect('[')
                p = Subscript(value=t, slice=self.expression(), mode='load',parser=self)
                self.expect(']')
                t = p
            elif self.token == '(':
                self.expect('(')
                p = Call(func=t, args=self.argument_expression_list(),parser=self)
                self.expect(')')
                t = p
            elif self.token == '.':
                self.expect('.')
                p = Attribute(value=t, attr=self.token.name, mode='load',parser=self)
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

    @compileandrun
    def primary_expression(self):
        t = None
        if self.token.name.isalpha():
            t = Name(id=self.token.name, mode='load',parser=self)
            self.expect(self.token.name)
        elif self.token.name.isalnum():
            t = Constant(value=int(self.token.name),parser=self)
            self.expect(self.token.name)
        elif self.token.name[0] in ('"', "'"):
            t = Constant(value=self.token.name[1:-1],parser=self)
            self.expect(self.token.name)
        elif self.token == '(':
            self.expect('(')
            if self.token != ')':
                t = self.expression()
            self.expect(')')
        elif self.token == '[':
            t = List(list=[],parser=self)
            self.expect('[')
            if self.token != ']':
                val = self.expression()
                t.list.append(val)
                while self.token == ',':
                    self.expect(',')
                    t.list.append(self.expression())
            self.expect(']')
        elif self.token == '{':
            t = Dict(keys=[], values=[],parser=self)
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
