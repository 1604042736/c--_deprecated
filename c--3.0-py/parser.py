from syntaxtree import Assign, Attribute, BinOp, BoolOp, Break, Call, Compare, Constant, Continue, Expr, FunctionDef, If, Name, Namespace, Return, Sentence, Subscript, SyntaxTree, Unkown, While, compileandrun

class Parser:
    '''
    语法分析
    '''

    def __init__(self,lexer,envir):
        self.envir=envir
        self.lexer=lexer
        self.token=self.lexer.token
        self.indent=self.lexer.indent

    def expect(self,expected):
        self.token=self.lexer.expect(expected)
        self.indent=self.lexer.indent
        return self.token

    def skip_block(self):
        line= self.lexer.skip_block()
        self.indent=self.lexer.indent
        self.token=self.lexer.token
        return line

    @compileandrun
    def start(self):
        t=Namespace(body=self.body())
        return t

    def body(self,indent=''):
        t=[]
        while self.token!='<None>' and self.indent==indent:
            t.append(self.sentence())
            if 'return'in SyntaxTree.state:
                break
        return t

    @compileandrun
    def sentence(self):
        t=None
        if self.token=='if':
            t=self.if_sentence()
        elif self.token=='while':
            t=self.while_sentence()
        elif self.token=='break':
            t=Break()
        elif self.token=='continue':
            t=Continue()
        elif self.token=='def':
            t=self.funcdef_sentence()
        elif self.token=='return':
            t=self.return_sentence()
        else:
            t=self.expression_sentence()
        return t

    @compileandrun
    def if_sentence(self):
        self.expect('if')
        exp=self.expression()
        self.expect(':')
        startline,endline,indent=self.skip_block()
        #先不解析代码块
        body=Unkown(indent=indent,startlineno=startline,endlineno=endline,shift=self.lexer.shift+startline,lexer=self.lexer,envir=self.envir)
        elses=[]
        if self.token=='else':
            self.expect('else')
            self.expect(':')
            startline,endline,indent=self.skip_block()
            p=Unkown(indent=indent,startlineno=startline,endlineno=endline,shift=self.lexer.shift+startline,lexer=self.lexer,envir=self.envir)
            elses.append(p)
        t=If(exp=exp,body=[body],elses=elses)
        return t

    @compileandrun
    def while_sentence(self):
        self.expect('while')
        exp=self.expression()
        self.expect(':')
        startline,endline,indent=self.skip_block()
        #先不解析代码块
        body=Unkown(indent=indent,startlineno=startline,endlineno=endline,shift=self.lexer.shift+startline,lexer=self.lexer,envir=self.envir)
        t=While(exp=exp,body=[body])
        return t

    @compileandrun
    def funcdef_sentence(self):
        self.expect('def')
        name=self.token.name
        self.expect(self.token.name)
        self.expect('(')
        args=self.argument_expression_list()
        self.expect(')')
        self.expect(':')
        startline,endline,indent=self.skip_block()
        body=Unkown(indent=indent,startlineno=startline,endlineno=endline,shift=self.lexer.shift+startline,lexer=self.lexer,envir=self.envir)
        t=FunctionDef(name=name,args=args,body=[body])
        return t

    @compileandrun
    def return_sentence(self):
        self.expect('return')
        value=self.expression()
        t=Return(value=value)
        return t

    @compileandrun
    def expression_sentence(self):
        t=self.expression()
        if not isinstance(t,Sentence):
            t=Expr(exp=t)
        return t

    @compileandrun
    def expression(self):
        t=self.conditional_expression()
        if self.token=='=':
            self.expect('=')
            t.mode='store'
            p=Assign(targets=[t],value=self.expression())
            t=p
        return t

    @compileandrun
    def conditional_expression(self):
        return self.logical_or_expression()

    @compileandrun
    def logical_or_expression(self):
        t=self.logical_and_expression()
        if self.token=='or':
            p=BoolOp(op='or',values=[t])
            t=p
            while self.token=='or':
                self.expect('or')
                t.values.append(self.logical_and_expression())
        return t

    @compileandrun
    def logical_and_expression(self):
        t=self.inclusive_or_expression()
        if self.token=='and':
            p=BoolOp(op='and',values=[t])
            t=p
            while self.token=='and':
                self.expect('and')
                t.values.append(self.inclusive_or_expression())
        return t

    @compileandrun
    def inclusive_or_expression(self):
        t=self.exclusive_or_expression()
        while self.token=='|':
            self.expect('|')
            p=BinOp(left=t,op='|',right=self.inclusive_or_expression())
            t=p
        return t

    @compileandrun
    def exclusive_or_expression(self):
        t=self.and_expression()
        while self.token=='^':
            self.expect('^')
            p=BinOp(left=t,op='^',right=self.and_expression())
            t=p
        return t

    @compileandrun
    def and_expression(self):
        t=self.relational_expression()
        while self.token=='&':
            self.expect('&')
            p=BinOp(left=t,op='&',right=self.relational_expression())
            t=p
        return t

    @compileandrun
    def relational_expression(self):
        t=self.shift_expression()
        if self.token.name in ('>=','<=','==','!=','>','<'):
            b=self.compares()
            p=Compare(left=t,ops=[i for i,j in b],comparators=[j for i,j in b])
            t=p
        return t

    def compares(self):
        result=[]
        while self.token.name in ('>=','<=','==','!=','>','<'):
            name=self.token.name
            self.expect(self.token.name)
            result.append((name,self.shift_expression()))
        return result

    @compileandrun
    def shift_expression(self):
        t=self.additive_expression()
        while self.token=='>>' or self.token=='<<':
            p=BinOp(left=t,op=self.token.name)
            self.expect(self.token.name)
            p.__dict__['right']=self.additive_expression()
            t=p
        return t

    @compileandrun
    def additive_expression(self):
        t=self.multiplicative_expression()
        while self.token.name in ('+','-'):
            p=BinOp(left=t,op=self.token.name)
            self.expect(self.token.name)
            p.__dict__['right']=self.multiplicative_expression()
            t=p
        return t

    @compileandrun
    def multiplicative_expression(self):
        t=self.cast_expression()
        while self.token.name in ('*','/','%'):
            p=BinOp(left=t,op=self.token.name)
            self.expect(self.token.name)
            p.__dict__['right']=self.cast_expression()
            t=p
        return t

    def cast_expression(self):
        return self.unary_expression()

    def unary_expression(self):
        return self.postfix_expression()

    @compileandrun
    def postfix_expression(self):
        t=self.primary_expression()
        while self.token.name in ('[','(','.'):
            if self.token=='[':
                self.expect('[')
                p=Subscript(value=t,slice=self.expression(),mode='load')
                self.expect(']')
                t=p
            elif self.token=='(':
                self.expect('(')
                p=Call(func=t,args=self.argument_expression_list())
                self.expect(')')
                t=p
            elif self.token=='.':
                self.expect('.')
                p=Attribute(value=t,attr=self.token.name,ctx='load')
                self.expect(self.token.name)
                t=p
        return t

    def argument_expression_list(self):
        t=[]
        if self.token!=')':
            p=self.expression()
            t.append(p)
            while self.token==',':
                self.expect(',')
                p=self.expression()
                t.append(p)
        return t

    @compileandrun
    def primary_expression(self):
        t=None
        if self.token.name.isalpha():
            t=Name(id=self.token.name,mode='load')
            self.expect(self.token.name)
        elif self.token.name.isalnum():
            t=Constant(value=int(self.token.name))
            self.expect(self.token.name)
        elif self.token.name[0] in ('"',"'"):
            t=Constant(value=self.token.name[1:-1])
            self.expect(self.token.name)
        elif self.token=='(':
            self.expect('(')
            if self.token!=')':
                t=self.expression()
            self.expect(')')
        return t