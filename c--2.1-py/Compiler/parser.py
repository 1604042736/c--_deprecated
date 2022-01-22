#!/usr/bin/env python3.8
# @generated by pegen from Grammar\c--.gram

import ast
import sys
import tokenize

from typing import Any, Optional

from pegen.parser import memoize, memoize_left_rec, logger, Parser

from syntaxtree import *
import itertools

# Keywords and soft keywords are listed at the end of the parser definition.
class GeneratedParser(Parser):

    @memoize
    def start(self) -> Optional[Any]:
        # start: sentences? $
        mark = self._mark()
        if (
            (a := self.sentences(),)
            and
            (_endmarker := self.expect('ENDMARKER'))
        ):
            return Module ( body = a or [] )
        self._reset(mark)
        return None

    @memoize
    def block(self) -> Optional[Any]:
        # block: NEWLINE INDENT sentences DEDENT
        mark = self._mark()
        if (
            (_newline := self.expect('NEWLINE'))
            and
            (_indent := self.expect('INDENT'))
            and
            (a := self.sentences())
            and
            (_dedent := self.expect('DEDENT'))
        ):
            return a
        self._reset(mark)
        return None

    @memoize
    def sentences(self) -> Optional[Any]:
        # sentences: sentence*
        # nullable=True
        mark = self._mark()
        if (
            (_loop0_1 := self._loop0_1(),)
        ):
            return _loop0_1
        self._reset(mark)
        return None

    @memoize
    def sentence(self) -> Optional[Any]:
        # sentence: statement NEWLINE | statement
        mark = self._mark()
        if (
            (a := self.statement())
            and
            (_newline := self.expect('NEWLINE'))
        ):
            return a
        self._reset(mark)
        if (
            (statement := self.statement())
        ):
            return statement
        self._reset(mark)
        return None

    @memoize
    def statement(self) -> Optional[Any]:
        # statement: &'if' if_statement | &'while' while_statement | &'def' functiondef_statement | &'return' return_statement | &'break' break_statement | &'continue' continue_statement | expression
        mark = self._mark()
        if (
            self.positive_lookahead(self.expect, 'if')
            and
            (if_statement := self.if_statement())
        ):
            return if_statement
        self._reset(mark)
        if (
            self.positive_lookahead(self.expect, 'while')
            and
            (while_statement := self.while_statement())
        ):
            return while_statement
        self._reset(mark)
        if (
            self.positive_lookahead(self.expect, 'def')
            and
            (functiondef_statement := self.functiondef_statement())
        ):
            return functiondef_statement
        self._reset(mark)
        if (
            self.positive_lookahead(self.expect, 'return')
            and
            (return_statement := self.return_statement())
        ):
            return return_statement
        self._reset(mark)
        if (
            self.positive_lookahead(self.expect, 'break')
            and
            (break_statement := self.break_statement())
        ):
            return break_statement
        self._reset(mark)
        if (
            self.positive_lookahead(self.expect, 'continue')
            and
            (continue_statement := self.continue_statement())
        ):
            return continue_statement
        self._reset(mark)
        if (
            (expression := self.expression())
        ):
            return expression
        self._reset(mark)
        return None

    @memoize
    def expression(self) -> Optional[Any]:
        # expression: assignment_expression
        mark = self._mark()
        if (
            (assignment_expression := self.assignment_expression())
        ):
            return assignment_expression
        self._reset(mark)
        return None

    @memoize
    def assignment_expression(self) -> Optional[Any]:
        # assignment_expression: conditional_expression '=' expression | conditional_expression
        mark = self._mark()
        if (
            (a := self.conditional_expression())
            and
            (literal := self.expect('='))
            and
            (b := self.expression())
        ):
            return Assign ( targets = a , value = b )
        self._reset(mark)
        if (
            (conditional_expression := self.conditional_expression())
        ):
            return conditional_expression
        self._reset(mark)
        return None

    @memoize
    def conditional_expression(self) -> Optional[Any]:
        # conditional_expression: logical_or_expression
        mark = self._mark()
        if (
            (logical_or_expression := self.logical_or_expression())
        ):
            return logical_or_expression
        self._reset(mark)
        return None

    @memoize
    def logical_or_expression(self) -> Optional[Any]:
        # logical_or_expression: logical_and_expression (('or' logical_and_expression))+ | logical_and_expression
        mark = self._mark()
        if (
            (a := self.logical_and_expression())
            and
            (b := self._loop1_2())
        ):
            return BoolOp ( op = Or ( ) , values = [a] + [i [1] for i in b] )
        self._reset(mark)
        if (
            (logical_and_expression := self.logical_and_expression())
        ):
            return logical_and_expression
        self._reset(mark)
        return None

    @memoize
    def logical_and_expression(self) -> Optional[Any]:
        # logical_and_expression: inclusive_or_expression (('and' inclusive_or_expression))+ | inclusive_or_expression
        mark = self._mark()
        if (
            (a := self.inclusive_or_expression())
            and
            (b := self._loop1_3())
        ):
            return BoolOp ( op = And ( ) , values = [a] + [i [1] for i in b] )
        self._reset(mark)
        if (
            (inclusive_or_expression := self.inclusive_or_expression())
        ):
            return inclusive_or_expression
        self._reset(mark)
        return None

    @memoize_left_rec
    def inclusive_or_expression(self) -> Optional[Any]:
        # inclusive_or_expression: inclusive_or_expression '|' exclusive_or_expression | exclusive_or_expression
        mark = self._mark()
        if (
            (a := self.inclusive_or_expression())
            and
            (literal := self.expect('|'))
            and
            (b := self.exclusive_or_expression())
        ):
            return BinOp ( left = a , op = BitOr ( ) , right = b )
        self._reset(mark)
        if (
            (exclusive_or_expression := self.exclusive_or_expression())
        ):
            return exclusive_or_expression
        self._reset(mark)
        return None

    @memoize_left_rec
    def exclusive_or_expression(self) -> Optional[Any]:
        # exclusive_or_expression: exclusive_or_expression '^' and_expression | and_expression
        mark = self._mark()
        if (
            (a := self.exclusive_or_expression())
            and
            (literal := self.expect('^'))
            and
            (b := self.and_expression())
        ):
            return BinOp ( left = a , op = BitXor ( ) , right = b )
        self._reset(mark)
        if (
            (and_expression := self.and_expression())
        ):
            return and_expression
        self._reset(mark)
        return None

    @memoize_left_rec
    def and_expression(self) -> Optional[Any]:
        # and_expression: and_expression '&' relational_expression | relational_expression
        mark = self._mark()
        if (
            (a := self.and_expression())
            and
            (literal := self.expect('&'))
            and
            (b := self.relational_expression())
        ):
            return BinOp ( left = a , op = BitAnd ( ) , right = b )
        self._reset(mark)
        if (
            (relational_expression := self.relational_expression())
        ):
            return relational_expression
        self._reset(mark)
        return None

    @memoize
    def relational_expression(self) -> Optional[Any]:
        # relational_expression: shift_expression compares | shift_expression
        mark = self._mark()
        if (
            (a := self.shift_expression())
            and
            (b := self.compares())
        ):
            return Compare ( left = a , ops = [i for i , j in b] , comparators = [j for i , j in b] )
        self._reset(mark)
        if (
            (shift_expression := self.shift_expression())
        ):
            return shift_expression
        self._reset(mark)
        return None

    @memoize
    def compares(self) -> Optional[Any]:
        # compares: compare+
        mark = self._mark()
        if (
            (_loop1_4 := self._loop1_4())
        ):
            return _loop1_4
        self._reset(mark)
        return None

    @memoize
    def compare(self) -> Optional[Any]:
        # compare: '<' shift_expression | '<=' shift_expression | '>' shift_expression | '>=' shift_expression | '==' shift_expression | '!=' shift_expression
        mark = self._mark()
        if (
            (literal := self.expect('<'))
            and
            (a := self.shift_expression())
        ):
            return ( Lt ( ) , a )
        self._reset(mark)
        if (
            (literal := self.expect('<='))
            and
            (a := self.shift_expression())
        ):
            return ( LtE ( ) , a )
        self._reset(mark)
        if (
            (literal := self.expect('>'))
            and
            (a := self.shift_expression())
        ):
            return ( Gt ( ) , a )
        self._reset(mark)
        if (
            (literal := self.expect('>='))
            and
            (a := self.shift_expression())
        ):
            return ( GtE ( ) , a )
        self._reset(mark)
        if (
            (literal := self.expect('=='))
            and
            (a := self.shift_expression())
        ):
            return ( Eq ( ) , a )
        self._reset(mark)
        if (
            (literal := self.expect('!='))
            and
            (a := self.shift_expression())
        ):
            return ( NotEq ( ) , a )
        self._reset(mark)
        return None

    @memoize_left_rec
    def shift_expression(self) -> Optional[Any]:
        # shift_expression: shift_expression '>>' additive_expression | shift_expression '<<' additive_expression | additive_expression
        mark = self._mark()
        if (
            (a := self.shift_expression())
            and
            (literal := self.expect('>>'))
            and
            (b := self.additive_expression())
        ):
            return BinOp ( left = a , op = RShift ( ) , right = b )
        self._reset(mark)
        if (
            (a := self.shift_expression())
            and
            (literal := self.expect('<<'))
            and
            (b := self.additive_expression())
        ):
            return BinOp ( left = a , op = LShift ( ) , right = b )
        self._reset(mark)
        if (
            (additive_expression := self.additive_expression())
        ):
            return additive_expression
        self._reset(mark)
        return None

    @memoize_left_rec
    def additive_expression(self) -> Optional[Any]:
        # additive_expression: additive_expression '+' multiplicative_expression | additive_expression '-' multiplicative_expression | multiplicative_expression
        mark = self._mark()
        if (
            (a := self.additive_expression())
            and
            (literal := self.expect('+'))
            and
            (b := self.multiplicative_expression())
        ):
            return BinOp ( left = a , op = Add ( ) , right = b )
        self._reset(mark)
        if (
            (a := self.additive_expression())
            and
            (literal := self.expect('-'))
            and
            (b := self.multiplicative_expression())
        ):
            return BinOp ( left = a , op = Sub ( ) , right = b )
        self._reset(mark)
        if (
            (multiplicative_expression := self.multiplicative_expression())
        ):
            return multiplicative_expression
        self._reset(mark)
        return None

    @memoize_left_rec
    def multiplicative_expression(self) -> Optional[Any]:
        # multiplicative_expression: multiplicative_expression '*' cast_expression | multiplicative_expression '/' cast_expression | multiplicative_expression '%' cast_expression | cast_expression
        mark = self._mark()
        if (
            (a := self.multiplicative_expression())
            and
            (literal := self.expect('*'))
            and
            (b := self.cast_expression())
        ):
            return BinOp ( left = a , op = Mult ( ) , right = b )
        self._reset(mark)
        if (
            (a := self.multiplicative_expression())
            and
            (literal := self.expect('/'))
            and
            (b := self.cast_expression())
        ):
            return BinOp ( left = a , op = Div ( ) , right = b )
        self._reset(mark)
        if (
            (a := self.multiplicative_expression())
            and
            (literal := self.expect('%'))
            and
            (b := self.cast_expression())
        ):
            return BinOp ( left = a , op = Mod ( ) , right = b )
        self._reset(mark)
        if (
            (cast_expression := self.cast_expression())
        ):
            return cast_expression
        self._reset(mark)
        return None

    @memoize
    def cast_expression(self) -> Optional[Any]:
        # cast_expression: unary_expression
        mark = self._mark()
        if (
            (unary_expression := self.unary_expression())
        ):
            return unary_expression
        self._reset(mark)
        return None

    @memoize
    def unary_expression(self) -> Optional[Any]:
        # unary_expression: postfix_expression
        mark = self._mark()
        if (
            (postfix_expression := self.postfix_expression())
        ):
            return postfix_expression
        self._reset(mark)
        return None

    @memoize_left_rec
    def postfix_expression(self) -> Optional[Any]:
        # postfix_expression: postfix_expression '[' expression ']' | postfix_expression '(' ')' | postfix_expression '(' argument_expression_list ')' | postfix_expression NAME | primary_expression
        mark = self._mark()
        if (
            (a := self.postfix_expression())
            and
            (literal := self.expect('['))
            and
            (b := self.expression())
            and
            (literal_1 := self.expect(']'))
        ):
            return Subscript ( value = a , slice = b , ctx = Load ( ) )
        self._reset(mark)
        if (
            (a := self.postfix_expression())
            and
            (literal := self.expect('('))
            and
            (literal_1 := self.expect(')'))
        ):
            return Call ( func = a , args = [] )
        self._reset(mark)
        if (
            (a := self.postfix_expression())
            and
            (literal := self.expect('('))
            and
            (b := self.argument_expression_list())
            and
            (literal_1 := self.expect(')'))
        ):
            return Call ( func = a , args = b )
        self._reset(mark)
        if (
            (a := self.postfix_expression())
            and
            (b := self.name())
        ):
            return Attribute ( value = a , attr = b . string , ctx = Load ( ) )
        self._reset(mark)
        if (
            (primary_expression := self.primary_expression())
        ):
            return primary_expression
        self._reset(mark)
        return None

    @memoize
    def primary_expression(self) -> Optional[Any]:
        # primary_expression: NAME | NUMBER | STRING | '(' expression ')'
        mark = self._mark()
        if (
            (name := self.name())
        ):
            return Name ( id = name . string , ctx = Load ( ) )
        self._reset(mark)
        if (
            (number := self.number())
        ):
            return Constant ( value = number . string )
        self._reset(mark)
        if (
            (string := self.string())
        ):
            return Constant ( value = string . string )
        self._reset(mark)
        if (
            (literal := self.expect('('))
            and
            (a := self.expression())
            and
            (literal_1 := self.expect(')'))
        ):
            return a
        self._reset(mark)
        return None

    @memoize
    def argument_expression_list(self) -> Optional[Any]:
        # argument_expression_list: assignment_expression ((',' assignment_expression))*
        mark = self._mark()
        if (
            (a := self.assignment_expression())
            and
            (b := self._loop0_5(),)
        ):
            return [a] + b
        self._reset(mark)
        return None

    @memoize
    def if_statement(self) -> Optional[Any]:
        # if_statement: 'if' expression ':' block else_statement | 'if' expression ':' block
        mark = self._mark()
        if (
            (literal := self.expect('if'))
            and
            (a := self.expression())
            and
            (literal_1 := self.expect(':'))
            and
            (b := self.block())
            and
            (c := self.else_statement())
        ):
            return If ( test = a , body = b , orelse = [c] if not isinstance ( c , list ) else c )
        self._reset(mark)
        if (
            (literal := self.expect('if'))
            and
            (a := self.expression())
            and
            (literal_1 := self.expect(':'))
            and
            (b := self.block())
        ):
            return If ( test = a , body = b , orelse = [] )
        self._reset(mark)
        return None

    @memoize
    def else_statement(self) -> Optional[Any]:
        # else_statement: 'elif' expression ':' block else_statement | 'elif' expression ':' block | 'else' ':' block
        mark = self._mark()
        if (
            (literal := self.expect('elif'))
            and
            (a := self.expression())
            and
            (literal_1 := self.expect(':'))
            and
            (b := self.block())
            and
            (c := self.else_statement())
        ):
            return If ( test = a , body = b , orelse = [c] if not isinstance ( c , list ) else c )
        self._reset(mark)
        if (
            (literal := self.expect('elif'))
            and
            (a := self.expression())
            and
            (literal_1 := self.expect(':'))
            and
            (b := self.block())
        ):
            return If ( test = a , body = b , orelse = [] )
        self._reset(mark)
        if (
            (literal := self.expect('else'))
            and
            (literal_1 := self.expect(':'))
            and
            (a := self.block())
        ):
            return a
        self._reset(mark)
        return None

    @memoize
    def while_statement(self) -> Optional[Any]:
        # while_statement: 'while' expression ':' block 'else' ':' block | 'while' expression ':' block
        mark = self._mark()
        if (
            (literal := self.expect('while'))
            and
            (a := self.expression())
            and
            (literal_1 := self.expect(':'))
            and
            (b := self.block())
            and
            (literal_2 := self.expect('else'))
            and
            (literal_3 := self.expect(':'))
            and
            (c := self.block())
        ):
            return While ( test = a , body = b , orelse = c )
        self._reset(mark)
        if (
            (literal := self.expect('while'))
            and
            (a := self.expression())
            and
            (literal_1 := self.expect(':'))
            and
            (b := self.block())
        ):
            return While ( test = a , block = b , orelse = [] )
        self._reset(mark)
        return None

    @memoize
    def functiondef_statement(self) -> Optional[Any]:
        # functiondef_statement: 'def' NAME '(' ')' ':' block | 'def' NAME '(' argument_expression_list ')' ':' block
        mark = self._mark()
        if (
            (literal := self.expect('def'))
            and
            (name := self.name())
            and
            (literal_1 := self.expect('('))
            and
            (literal_2 := self.expect(')'))
            and
            (literal_3 := self.expect(':'))
            and
            (b := self.block())
        ):
            return FunctionDef ( name = name . string , args = [] , body = b )
        self._reset(mark)
        if (
            (literal := self.expect('def'))
            and
            (name := self.name())
            and
            (literal_1 := self.expect('('))
            and
            (b := self.argument_expression_list())
            and
            (literal_2 := self.expect(')'))
            and
            (literal_3 := self.expect(':'))
            and
            (c := self.block())
        ):
            return FunctionDef ( name = name . string , args = b , body = c )
        self._reset(mark)
        return None

    @memoize
    def return_statement(self) -> Optional[Any]:
        # return_statement: 'return' expression
        mark = self._mark()
        if (
            (literal := self.expect('return'))
            and
            (a := self.expression())
        ):
            return Return ( value = a )
        self._reset(mark)
        return None

    @memoize
    def break_statement(self) -> Optional[Any]:
        # break_statement: 'break'
        mark = self._mark()
        if (
            (literal := self.expect('break'))
        ):
            return Break ( )
        self._reset(mark)
        return None

    @memoize
    def continue_statement(self) -> Optional[Any]:
        # continue_statement: 'continue'
        mark = self._mark()
        if (
            (literal := self.expect('continue'))
        ):
            return Continue ( )
        self._reset(mark)
        return None

    @memoize
    def _loop0_1(self) -> Optional[Any]:
        # _loop0_1: sentence
        mark = self._mark()
        children = []
        while (
            (sentence := self.sentence())
        ):
            children.append(sentence)
            mark = self._mark()
        self._reset(mark)
        return children

    @memoize
    def _loop1_2(self) -> Optional[Any]:
        # _loop1_2: ('or' logical_and_expression)
        mark = self._mark()
        children = []
        while (
            (_tmp_6 := self._tmp_6())
        ):
            children.append(_tmp_6)
            mark = self._mark()
        self._reset(mark)
        return children

    @memoize
    def _loop1_3(self) -> Optional[Any]:
        # _loop1_3: ('and' inclusive_or_expression)
        mark = self._mark()
        children = []
        while (
            (_tmp_7 := self._tmp_7())
        ):
            children.append(_tmp_7)
            mark = self._mark()
        self._reset(mark)
        return children

    @memoize
    def _loop1_4(self) -> Optional[Any]:
        # _loop1_4: compare
        mark = self._mark()
        children = []
        while (
            (compare := self.compare())
        ):
            children.append(compare)
            mark = self._mark()
        self._reset(mark)
        return children

    @memoize
    def _loop0_5(self) -> Optional[Any]:
        # _loop0_5: (',' assignment_expression)
        mark = self._mark()
        children = []
        while (
            (_tmp_8 := self._tmp_8())
        ):
            children.append(_tmp_8)
            mark = self._mark()
        self._reset(mark)
        return children

    @memoize
    def _tmp_6(self) -> Optional[Any]:
        # _tmp_6: 'or' logical_and_expression
        mark = self._mark()
        if (
            (literal := self.expect('or'))
            and
            (logical_and_expression := self.logical_and_expression())
        ):
            return [literal, logical_and_expression]
        self._reset(mark)
        return None

    @memoize
    def _tmp_7(self) -> Optional[Any]:
        # _tmp_7: 'and' inclusive_or_expression
        mark = self._mark()
        if (
            (literal := self.expect('and'))
            and
            (inclusive_or_expression := self.inclusive_or_expression())
        ):
            return [literal, inclusive_or_expression]
        self._reset(mark)
        return None

    @memoize
    def _tmp_8(self) -> Optional[Any]:
        # _tmp_8: ',' assignment_expression
        mark = self._mark()
        if (
            (literal := self.expect(','))
            and
            (assignment_expression := self.assignment_expression())
        ):
            return [literal, assignment_expression]
        self._reset(mark)
        return None

    KEYWORDS = ('else', 'and', 'break', 'while', 'continue', 'elif', 'return', 'def', 'if', 'or')
    SOFT_KEYWORDS = ()


if __name__ == '__main__':
    from pegen.parser import simple_parser_main
    simple_parser_main(GeneratedParser)
