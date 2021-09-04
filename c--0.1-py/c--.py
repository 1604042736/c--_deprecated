import operator
import sys

class Lexer:
    def __init__(lexer,source):
        lexer.lineno=1
        lexer.linebuf=source.read()
        lexer.bufsize=len(lexer.linebuf)
        lexer.linepos=0
        lexer.EOF_FLAG=False
        lexer.source=source
        lexer.tokenString=['' for i in range(41)]

    def getchar(lexer):
        if(lexer.linepos<lexer.bufsize):
            return lexer.linebuf[lexer.linepos]
        else:
            lexer.EOP_FLAG=True
            return "ENDFILE"

    def ungetchar(lexer):
        if(not lexer.EOF_FLAG):
            lexer.linepos-=1

    def lookup(lexer):
        s=lexer.tokenString
        c=""
        i=0
        while(i<len(s)):
            c+=s[i]
            i+=1
        lp= [\
                ["if","IF"],\
                ["while","WHILE"],\
                ["input","INPUT"],\
                ["print","PRINT"],\
                ["return","RETURN"],\
                ["func","FUNC"],\
                ["include","INCLUDE"],\
                ["array","ARRAY"],\
                ["do","DO"],\
                ["else","ELSE"]
            ]
        j=0
        while(j<len(lp)):
            if(c.find(lp[j][0])!=-1):
                return lp[j][1]
            j+=1
        return "ID"

    def gettoken(lexer):
        lexer.tokenString=['' for i in range(41)]
        tokenStringlen=0
        nowtoken=""
        state="START"
        sace=False
        while(state!="DONE"):
            c=lexer.getchar()
            lexer.linepos+=1
            save=True
            if(state=="START"):
                if(str.isdigit(c)):
                    state="INNUM"
                elif(str.isalpha(c) and c!="ENDFILE"):
                    state="INID"
                elif(c=='='or c=='<'or c=='>'or c=="!"):
                    state="INASSIGN"
                elif(c==' 'or c=='\t'or c=="#"):
                    save=False
                elif(c=='\n'):
                    lexer.lineno+=1
                    save=False
                elif(c=='$'):
                    save=False
                    state="INCOMMENT"
                elif(c=="~"):
                    save=False
                    state="INMORE"
                elif(c=='"'):
                    save=False
                    state="INSTRING"
                elif(c=="|"):
                    state="INOR"
                elif(c=="&"):
                    state="INAND"
                else:
                    state = "DONE"
                    nowtoken=c
            elif(state=="INCOMMENT"):
                save=False
                if(c=="ENDFILE"):
                    state="DONE"
                    nowtoken="ENDFILE"
                elif(c=='\n'):
                    state="START"
            elif(state=="INMORE"):
                save=False
                if(c=="ENDFILE"):
                    state="DONE"
                    nowtoken="ENDFILE"
                elif(c=='~'):
                    state="START"
            elif(state=="INNUM"):
                if(not str.isdigit(c)):
                    lexer.ungetchar()
                    save=False
                    state="DONE"
                    nowtoken="NUM"
            elif(state=="INID"):
                if(not str.isalpha(c)):
                    lexer.ungetchar()
                    save=False
                    state="DONE"
                    nowtoken="ID"
            elif(state=="INSTRING"):
                if(c=='"'):
                    nowtoken="STRING"
                    save=False
                    state="DONE"
            elif(state=="INOR"):
                if(c=="|"):
                    state="DONE"
                    nowtoken=lexer.tokenString[tokenStringlen-1]+c
                else:
                    lexer.ungetchar()
                    save=False
                    state="DONE"
                    nowtoken=lexer.tokenString[tokenStringlen-1]
            elif(state=="INAND"):
                if(c=="&"):
                    state="DONE"
                    nowtoken=lexer.tokenString[tokenStringlen-1]+c
                else:
                    lexer.ungetchar()
                    save=False
                    state="DONE"
                    nowtoken=lexer.tokenString[tokenStringlen-1]
            elif(state=="INASSIGN"):
                if(c=='='):
                    state="DONE"
                    nowtoken=lexer.tokenString[tokenStringlen-1]+c
                else:
                    lexer.ungetchar()
                    save=False
                    state="DONE"
                    nowtoken=lexer.tokenString[tokenStringlen-1]
                    
            if(save==True and tokenStringlen<=40):
                lexer.tokenString[tokenStringlen]=c
                tokenStringlen+=1
            if(state=="DONE"):
                lexer.tokenString[tokenStringlen]='\0'
                if(nowtoken=="ID"):
                    nowtoken=lexer.lookup()
                        
        return nowtoken

class Parser:
    def __init__(parser,lexer):
        parser.lexer=lexer
        parser.error=False
        parser.token=""
        parser.lasttoken=""
        parser.tab=0

    def syntaxError(parser,message):
        print("\n>>> ",end="")
        print("Syntax error at line %d : %s"%(parser.lexer.lineno,message),end="")
        parser.error=True

    def advance(parser):
        parser.lasttoken=parser.token
        parser.token=parser.lexer.gettoken()

    def match(parser,expected):
        if(parser.token.find(expected)!=-1):
            parser.advance()
        else:
            parser.syntaxError("unexpected token -> ")
            parser.printToken()

    def if_stmt(parser):
        t=treenode(parser.lexer.lineno,"IfK")
        parser.match("IF")
        parser.match("(")
        if(t!=None):
            t.child[0]=parser.exp()
        parser.match(")")
        parser.match("{")
        if(t!=None and parser.token!="}"):
            t.child[1]=parser.buildmain()
        parser.match("}")
        if(parser.token=="ELSE"):
            p=treenode(parser.lexer.lineno,"ElseK")
            parser.match("ELSE")
            parser.match("{")
            if(t!=None and parser.token!="}"):
                p.child[0]=parser.buildmain()
            t.child[2]=p
            parser.match("}")
        return t
    
    def while_stmt(parser):
        t=treenode(parser.lexer.lineno,"WhileK")
        parser.match("WHILE")
        parser.match("(")
        if(t!=None):
            t.child[0]=parser.exp()
        parser.match(")")
        parser.match("{")
        if(t!=None and parser.token!="}"):
            t.child[1]=parser.buildmain()
        parser.match("}")
        return t

    def dowhile_stmt(parser):
        t=treenode(parser.lexer.lineno,"DoWhileK")
        parser.match("DO")
        parser.match("{")
        if(t!=None and parser.token!="}"):
            t.child[0]=parser.buildmain()
        parser.match("}")
        parser.match("WHILE")
        parser.match("(")
        if(t!=None):
            t.child[1]=parser.exp()
        parser.match(")")
        return t

    def call(parser):
        t=parser.callparam()
        p=t
        while(parser.token==','):
            q=None
            parser.match(",")
            q=parser.callparam()
            if(q!=None):
                if(t==None):
                    t=p=q
                else:
                    p.sibling=q
                    p=q
        return t

    def callparam(parser):
        t=treenode(parser.lexer.lineno,"CallParamK")
        t.child[0]=parser.exp()
        return t

    def assign(parser):
        t=treenode(parser.lexer.lineno,"AssignK")
        if(t!=None and parser.token=="ID"):
            t.name=parser.copyString()
        parser.match("ID")
        if(parser.token!="("and parser.token!="["):
            parser.match("=")
            if(t!=None):
                t.child[0]=parser.exp()
        elif(parser.token!="["):
            p=treenode(parser.lexer.lineno,"CallK")
            p.name=t.name
            t=p
            parser.match("(")
            if(t!=None and parser.token!=")"):
                t.child[0]=parser.call()
            parser.match(")")
        else:
            p=treenode(parser.lexer.lineno,"ArrayAssignK")
            p.name=t.name
            t=p
            parser.match("[")
            q=treenode(parser.lexer.lineno,"ArraynumK")
            q.child[0]=parser.exp()
            t.child[0]=q
            parser.match("]")
            parser.match("=")
            t.child[1]=parser.exp()
        return t

    def func_stmt(parser):
        t=treenode(parser.lexer.lineno,"FuncK")
        parser.match("FUNC")
        if(t!=None and parser.token=="ID"):
            t.name=parser.copyString()
        parser.match("ID")
        parser.match("(")
        if(t!=None and parser.token!=")"):
            t.child[0]=parser.params()
        parser.match(")")
        parser.match("{")
        if(t!=None and parser.token!="}"):
            t.child[1]=parser.buildmain()
        parser.match("}")
        return t

    def params(parser):
        t=parser.param()
        p=t
        parser.match("ID")
        while(parser.token==","):
            q=None
            parser.match(",")
            q=parser.param()
            parser.match("ID")
            if(q!=None):
                if(t==None):
                    t=p=q
                else:
                    p.sibling=q
                    p=q
        return t

    def param(parser):
        t=treenode(parser.lexer.lineno,"ParamsK")
        t.name=parser.copyString()
        return t
    
    def return_stmt(parser):
        t=treenode(parser.lexer.lineno,"ReturnK")
        parser.match("RETURN")
        if(t!=None):
            t.child[0]=parser.exp()
        return t

    def print_stmt(parser):
        t=treenode(parser.lexer.lineno,"PrintK")
        parser.match("PRINT")
        parser.match("(")
        if(t!=None and parser.token!=")"):
            t.child[0]=parser.exp()
        parser.match(")")
        return t

    def input_stmt(parser):
        t=treenode(parser.lexer.lineno,"InputK")
        parser.match("INPUT")
        parser.match("(")
        if(t!=None and parser.token!=")"):
            t.name=parser.copyString()
            parser.match("ID")
        parser.match(")")
        return t

    def include(parser):
        t=treenode(parser.lexer.lineno,"IncludeK")
        parser.match("INCLUDE")
        if(parser.token=="STRING"):
            t.name=parser.copyString()
            parser.match("STRING")
            t.child[0]=complie(t.name)
        else:
            parser.match("<")
            t.name=parser.copyString()
            parser.match("ID")
            parser.match(">")
            t.child[0]=complie(sys.path[0]+'\\include\\'+t.name+".cqq")
        return t
        
    def printToken(parser):
        if(parser.token=="IF"):
            print("reserved word: %s"%(parser.copyString()))
        elif(parser.token=="WHILE"):
            print("reserved word: %s"%(parser.copyString()))
        else:
            print(parser.copyString())

    def statement(parser):
        if(parser.token=="IF"):
            t=parser.if_stmt()
            return t
        elif(parser.token=="WHILE"):
            t=parser.while_stmt()
            return t
        elif(parser.token=="ID"):
            t=parser.assign()
            return t
        elif(parser.token=="PRINT"):
            t=parser.print_stmt()
            return t
        elif(parser.token=="INPUT"):
            t=parser.input_stmt()
            return t
        elif(parser.token=="RETURN"):
            t=parser.return_stmt()
            return t
        elif(parser.token=="FUNC"):
            t=parser.func_stmt()
            return t
        elif(parser.token=="INCLUDE"):
            t=parser.include()
            return t
        elif(parser.token=="DO"):
            t=parser.dowhile_stmt()
            return t
        else:
            parser.syntaxError("unexpected token -> ")
            parser.printToken()
            parser.advance()
        return None

    def exp(parser):
        t=parser.exp_and()
        if(parser.token=="||"):
            p=treenode(parser.lexer.lineno,"OpK")
            if(p!=None):
                p.child[0]=t
                p.op=parser.token
                t=p
            parser.match(parser.token)
            if(t!=None):
                t.child[1]=parser.exp_and()
        return t

    def exp_and(parser):
        t=parser.exp_stmt()
        if(parser.token=="&&"):
            p=treenode(parser.lexer.lineno,"OpK")
            if(p!=None):
                p.child[0]=t
                p.op=parser.token
                t=p
            parser.match(parser.token)
            if(t!=None):
                t.child[1]=parser.exp_stmt()
        return t

    def exp_stmt(parser):
        t=parser.simple_exp()
        if(parser.token=="<"or parser.token==">"or parser.token=="=="or parser.token=="<="or parser.token==">="or parser.token=="!="):
            p=treenode(parser.lexer.lineno,"OpK")
            if(p!=None):
                p.child[0]=t
                p.op=parser.token
                t=p
            parser.match(parser.token)
            if(t!=None):
                t.child[1]=parser.simple_exp()
        return t

    def simple_exp(parser):
        t=parser.term()
        while(parser.token=="+"or parser.token=="-"):
            p=treenode(parser.lexer.lineno,"OpK")
            if(p!=None):
                p.child[0]=t
                p.op=parser.token
                t=p
                parser.match(parser.token)
                t.child[1]=parser.term()

        return t

    def term(parser):
        t=parser.factor()
        while(parser.token=="*"or parser.token=="/"or parser.token=="%"):
            p=treenode(parser.lexer.lineno,"OpK")
            if(p!=None):
                p.child[0]=t
                p.op=parser.token
                t=p
                parser.match(parser.token)
                p.child[1]=parser.factor()
        return t

    def copyString(parser):
        s=parser.lexer.tokenString
        c=""
        i=0
        while(i<len(s)):
            c+=s[i]
            i+=1
        if(c==None):
            return None
        return c

    def atoi(parser):
        s=parser.lexer.tokenString
        c=""
        i=0
        while(i<len(s)):
            if(s[i]!='\0'):
                c+=s[i]
            i+=1
        if(c==None):
            return None
        return int(c)

    def factor(parser):
        if(parser.token=="NUM"):
            t=treenode(parser.lexer.lineno,"ConstK")
            if(t!=None and parser.token=="NUM"):
                t.val=parser.atoi()
            parser.match("NUM")
            return t
        elif(parser.token=="ID"):
            t=treenode(parser.lexer.lineno,"IdK")
            if(t!=None and parser.token=="ID"):
                t.name=parser.copyString()
            parser.match("ID")
            if(parser.token=="("):
                p=treenode(parser.lexer.lineno,"CallK")
                p.name=t.name
                t=p
                parser.match("(")
                if(t!=None and parser.token!=")"):
                    t.child[0]=parser.call()
                parser.match(")")
            elif(parser.token=="["):
                p=treenode(parser.lexer.lineno,"ArrayK")
                p.name=t.name
                t=p
                parser.match("[")
                q=treenode(parser.lexer.lineno,"ArraynumK")
                q.child[0]=parser.exp()
                t.child[0]=q
                parser.match("]")
            return t
        elif(parser.token=="STRING"):
            t=treenode(parser.lexer.lineno,"StringK")
            if(t!=None and parser.token=="STRING"):
                t.name=parser.copyString()
            parser.match("STRING")
            return t
        elif(parser.token=="("):
            parser.match("(")
            t=parser.exp()
            parser.match(")")
            return t
        elif(parser.token=="ARRAY"):
            t=treenode(parser.lexer.lineno,"NewArrayK")
            parser.match("ARRAY")
            parser.match("[")
            if(t!=None and parser.token!="]"):
                t.child[0]=parser.exp()
            parser.match("]")
            return t
        else:
            parser.syntaxError("unexpected token -> ")
            parser.printToken()
            parser.advance()
        return None

    def buildmain(parser):
        t=parser.statement()
        p=t
        while(parser.token!="ENDFILE"):
            if(parser.lasttoken!="}"and p.kind!="IncludeK"):
                parser.match(";")
            if(parser.token=="ENDFILE" or parser.token=="}"):
                break
            q=parser.statement()
            if(q!=None):
                if(t==None):
                    t=p=q
                else:
                    p.sibling=q
                    p=q
        return t

    def buildtree(parser):
        parser.advance()
        t=parser.buildmain()
        if(parser.token!="ENDFILE"):
            parser.syntaxError("code ends before file")
        return t

class treenode:
    def __init__(tree,lineno,kind):
        tree.child=[None,None,None]
        tree.sibling=None
        tree.kind=kind
        tree.lineno=lineno
        tree.name=""
        tree.op=""
        tree.val=0
        tree.type="Void"

    def printtree(tree,n):
        while(tree!=None):
            for i in range(n):
                print("  ",end="")
            if(tree.kind=="IfK"):
                print("If")
            elif(tree.kind=="WhileK"):
                print("While")
            elif(tree.kind=="ReturnK"):
                print("Return")
            elif(tree.kind=="AssignK"):
                print("Assign to: %s"%(str(tree.name)))
            elif(tree.kind=="InputK"):
                print("Input: %s"%(str(tree.name)))
            elif(tree.kind=="PrintK"):
                print("Print")
            elif(tree.kind=="FuncK"):
                print("Func: %s"%(str(tree.name)))
            elif(tree.kind=="ParamsK"):
                print("Param: %s"%(str(tree.name)))
            elif(tree.kind=="OpK"):
                print("Op: %s"%(str(tree.op)))
            elif(tree.kind=="ConstK"):
                print("Const: %s"%(str(tree.val)))
            elif(tree.kind=="IdK"):
                print("Id: %s"%(str(tree.name)))
            elif(tree.kind=="CallParamK"):
                print("Param")
            elif(tree.kind=="IncludeK"):
                print("Include: %s"%(tree.name))
            elif(tree.kind=="ArrayAssignK"):
                print("Array Assign to: "+str(tree.name))
            elif(tree.kind=="ArraynumK"):
                print("Array number")
            elif(tree.kind=="ArrayK"):
                print("Array: "+str(tree.name))
            elif(tree.kind=="NewArrayK"):
                print("New Array")
            elif(tree.kind=="CallK"):
                print("Call: %s"%(tree.name))
            elif(tree.kind=="StringK"):
                print("String: %s"%(str(tree.name)))
            elif(tree.kind=="DoWhileK"):
                print("DoWhile")
            elif(tree.kind=="ElseK"):
                print("Else")
            for i in range(3):
                if(tree.child[i]!=None):
                    tree.child[i].printtree(n+1)
            tree=tree.sibling

class LineList:
    def __init__(l):
        l.lineno=0
        l.next=None

class BucketList:
    def __init__(l):
        l.name=""
        l.lines=None
        l.memloc=0
        l.next=None

class Symtab:
    def __init__(symtab):
        symtab.hashTable=[None for i in range(211)]

    def hash(symtab,key):
        temp=0
        i=0
        while(key[i]!="\0"):
            a=(temp<<4)+ord(key[i])
            temp=a-(a//211)*211
            i+=1
        return temp

    def st_lookup(symtab,name):
        h=symtab.hash(name)
        l=symtab.hashTable[h]
        while(l!=None and operator.ne(name,l.name)):
            l=l.next
        if(l==None):
            return -1
        else:
            return l.memloc

    def st_insert(symtab,name,lineno,loc):
        h=symtab.hash(name)
        l=symtab.hashTable[h]
        while(l!=None and operator.ne(name,l.name)):
            l=l.next
        if(l==None):
            l=BucketList()
            l.name=name
            l.lines=LineList()
            l.lines.lineno=lineno
            l.memloc=loc
            l.lines.next=None
            l.next=symtab.hashTable[h]
            symtab.hashTable[h]=l
        else:
            t=l.lines
            while(t.next!=None):
                t=t.next
            t.next=LineList()
            t.next.lineno=lineno
            t.next.next=None

    def printSymtab(symtab):
        print("Variable Name  Location  Line Numbers\n")
        for i in range(211):
            if(symtab.hashTable[i]!=None):
                l=symtab.hashTable[i]
                while(l!=None):
                    t=l.lines
                    print("%-14s "%(l.name),end="")
                    print("%8d "%(l.memloc),end="")
                    while(t!=None):
                        print("%4d "%(t.lineno),end="")
                        t=t.next
                    print()
                    l=l.next
            
class Analyzer:
    def __init__(analyzer,symtab):
        analyzer.symtab=symtab
        analyzer.location=0
        analyzer.error=False

    def nullProc(analyzer,t):
        return

    def insertNode(analyzer,t):
        if(t.kind=="AssignK"or t.kind=="InputK"or t.kind=="FuncK"or t.kind=="CallK"or t.kind=="IdK"or t.kind=="ArrayK"or t.kind=="ArrayAssignK"):
            if(analyzer.symtab.st_lookup(t.name)==-1):
                analyzer.symtab.st_insert(t.name,t.lineno,analyzer.location)
                analyzer.location+=1
            else:
                analyzer.symtab.st_insert(t.name,t.lineno,0)

    def buildSymtab(analyzer,tree):
        analyzer.traverse(tree, analyzer.insertNode, analyzer.nullProc)
        print("\nSymbol table:\n")
        analyzer.symtab.printSymtab()

    def traverse(analyzer,t,preProc,postProc):
        if(t!=None):
            preProc(t)
            for i in range(3):
                analyzer.traverse(t.child[i],preProc,postProc)
            postProc(t)
            analyzer.traverse(t.sibling,preProc,postProc)

    def typeCheck(analyzer,t):
        analyzer.traverse(t,analyzer.nullProc,analyzer.checkNode)

    def checkNode(analyzer,t):
        if(t.kind=="OpK"):
            if((t.child[0].type!="Integer" or t.child[1].type!="Integer")and(t.child[0].type!="Boolean" or t.child[1].type!="Boolean")):
                analyzer.typeError(t,"Op applied to non-integer")
            if(t.op=="=="or t.op=="<="or t.op==">="or t.op=="<"or t.op==">"):
                t.type="Boolean"
            else:
                t.type="Integer"
        elif(t.kind=="ConstK"or "IdK"):
            t.type="Integer"
        elif(t.kind=="StringK"):
            t.type="String"
        elif(t.child[0].kind=="NewArrayK"or t.kind=="ArrayK"or t.kind=="ArrayAssignK"):
            t.type="Array"
        elif(t.kind=="FuncK"):
            t.type="Func"
        elif(t.kind=="WhileK"):
            if(t.child[0].type=="Integer"):
                analyzer.typeError(t.child[0],"while test is not Boolean")
        elif(t.kind=="IfK"):
            if(t.child[0].type=="Integer"):
                analyzer.typeError(t.child[0],"if test is not Boolean")
        elif(t.kind=="DoWhileK"):
            if(t.child[1].type=="Integer"):
                analyzer.typeError(t.child[1],"do-while test is not Boolean")
        
    def typeError(analyzer,t,message):
        print("Type error at line %d: %s"%(t.lineno,message))
        analyzer.error=True

class Coder:
    def __init__(coder,out,symtab,traceCode):
        coder.emitLoc=0
        coder.highEmitLoc=0
        coder.out=out
        coder.traceCode=traceCode
        coder.tmpOffset=0
        coder.symtab=symtab
        coder.pc=7
        coder.mp=6
        coder.gp=5
        coder.ac=0
        coder.ac1=1
        coder.func=[]
        coder.call=[]
        coder.param=[]
        coder.params=0
        coder.nowfunc=""

    def emitComment(coder,c):
        if(coder.traceCode==True):
            coder.out.write("* %s\n"%(c))

    def emitRM(coder,op,r,d,s,c):
        coder.out.write("%3d:  %5s  %d,%d(%d) "%(coder.emitLoc,op,r,d,s))
        coder.emitLoc+=1
        if(coder.traceCode==True):
            coder.out.write("\t%s"%(c))
        coder.out.write("\n")
        if(coder.highEmitLoc<coder.emitLoc):
            coder.highEmitLoc=coder.emitLoc

    def emitRO(coder,op,r,s,t,c):
        coder.out.write("%3d:  %5s  %d,%d,%d "%(coder.emitLoc,op,r,s,t))
        coder.emitLoc+=1
        if(coder.traceCode==True):
            coder.out.write("\t%s"%(c))
        coder.out.write("\n")
        if(coder.highEmitLoc<coder.emitLoc):
            coder.highEmitLoc=coder.emitLoc

    def emitSkip(coder,howMany):
        i=coder.emitLoc
        coder.emitLoc+=howMany
        if(coder.highEmitLoc<coder.emitLoc):
            coder.highEmitLoc=coder.emitLoc
        return i

    def emitBackup(coder,loc):
        if(loc>coder.highEmitLoc):
            coder.emitComment("BUG in emitBackup")
        coder.emitLoc=loc

    def emitRM_Abs(coder,op,r,a,c):
        coder.out.write("%3d:  %5s  %d,%d(%d) "%(coder.emitLoc,op,r,a-(coder.emitLoc+1),coder.pc))
        coder.emitLoc+=1
        if(coder.traceCode==True):
            coder.out.write("\t%s"%(c))
        coder.out.write("\n")
        if(coder.highEmitLoc<coder.emitLoc):
            coder.highEmitLoc=coder.emitLoc

    def emitRestore(coder):
        coder.emitLoc=coder.highEmitLoc

    def func_lookup(coder,name):
        i=0
        while(i<len(coder.func)):
            if(operator.eq(name,coder.func[i][0])):
                return i
            i+=1
        return -1

    def param_lookup(coder,name):
        i=len(coder.param)-1
        while(i>=0):
            if(operator.eq(name,coder.param[i][0])):
                return coder.param[i][1]
            i-=1
        return -1

    def param_set(coder):
        i=0
        while(i<(coder.params)):
              coder.param[i][1]=coder.params-coder.param[i][1]-1
              i+=1

    def gen(coder,tree):
        p1=p2=p3=None
        savedLoc1=savedLoc2=currentLoc=loc=0
        if(tree.kind=="IfK"):
            if(coder.traceCode==True):
                coder.emitComment("-> if")
            p1=tree.child[0]
            p2=tree.child[1]
            p3=tree.child[2]
            coder.cGen(p1)
            savedLoc1=coder.emitSkip(1)
            coder.emitComment("if: jump to else belongs here")
            coder.cGen(p2)
            savedLoc2=coder.emitSkip(1)
            coder.emitComment("if: jump to end belongs here")
            currentLoc=coder.emitSkip(0)
            coder.emitBackup(savedLoc1)
            coder.emitRM_Abs("JEQ",coder.ac,currentLoc,"if: jump to else")
            coder.emitRestore()
            coder.cGen(p3)
            currentLoc=coder.emitSkip(0)
            coder.emitBackup(savedLoc2)
            coder.emitRM_Abs("LDA",coder.pc,currentLoc,"jmp to end")
            coder.emitRestore()
            if(coder.traceCode==True):
                coder.emitComment("<- if")
        elif(tree.kind=="ElseK"):
            if(coder.traceCode==True):
                coder.emitComment("-> else")
            coder.cGen(tree.child[0])
            if(coder.traceCode==True):
                coder.emitComment("<- else")
        elif(tree.kind=="WhileK"):
            if(coder.traceCode==True):
                coder.emitComment("-> while")
            p1=tree.child[0]
            p2=tree.child[1]
            savedLoc1=coder.emitSkip(0)
            coder.cGen(p1)
            savedLoc2=coder.emitSkip(1)
            coder.emitComment("while: jump to end belongs here")
            coder.cGen(p2)
            currentLoc=coder.emitSkip(0)
            coder.emitRM_Abs("LDA",coder.pc,savedLoc1,"jmp back to loop")
            coder.emitBackup(savedLoc2)
            coder.emitRM_Abs("JEQ",coder.ac,currentLoc+1,"while: jmp to end")
            coder.emitRestore()
            if(coder.traceCode==True):
                coder.emitComment("<- while")
        elif(tree.kind=="AssignK"):
            if(coder.traceCode==True):
                coder.emitComment("-> assign")
            coder.cGen(tree.child[0])
            loc=coder.param_lookup(tree.name)
            if(loc!=-1):
                coder.emitRM("ASTP",coder.ac,loc,coder.gp,"assign: store value")
            else:
                loc=coder.symtab.st_lookup(tree.name)
                coder.emitRM("ST",coder.ac,loc,coder.gp,"assign: store value")
            if(coder.traceCode==True):
                coder.emitComment("<- assign")
        elif(tree.kind=="InputK"):
            coder.emitRO("IN",coder.ac,0,0,"input integer value")
            loc=coder.symtab.st_lookup(tree.name)
            coder.emitRM("ST",coder.ac,loc,coder.gp,"input: store value")
        elif(tree.kind=="PrintK"):
            coder.cGen(tree.child[0])
            coder.emitRO("OUT",coder.ac,0,0,"print ac")
        elif(tree.kind=="FuncK"):
            if(coder.traceCode==True):
                coder.emitComment("-> func")
            p1=tree.child[0]
            p2=tree.child[1]
            savedLoc1=coder.emitSkip(1)
            coder.cGen(p1)
            coder.param_set()
            if(coder.func_lookup(tree.name)==-1):
                coder.func.append([tree.name,savedLoc1,coder.params])
                coder.params=0
            coder.nowfunc=tree.name
            savedLoc2=coder.emitSkip(0)
            coder.cGen(p2)
            i=coder.func_lookup(tree.name)
            coder.emitRM("RET",coder.pc,coder.func[i][2],0,"return func")
            currentLoc=coder.emitSkip(0)
            coder.emitBackup(savedLoc1)
            coder.emitRM_Abs("LDA",coder.pc,currentLoc,"jmp to end")
            coder.emitRestore()
            for i in range(coder.func[i][2]):
                if(coder.param!=[]):
                    coder.param.pop()
            if(coder.traceCode==True):
                coder.emitComment("<- func")
        elif(tree.kind=="ReturnK"):
            if(coder.traceCode==True):
                coder.emitComment("-> return")
            coder.cGen(tree.child[0])
            a=coder.func_lookup(coder.nowfunc)
            coder.emitRM("RET",coder.pc,coder.func[a][2],0,"return")
            if(coder.traceCode==True):
                coder.emitComment("<- return")
        elif(tree.kind=="ParamsK"):
            if(coder.traceCode==True):
                coder.emitComment("-> Param")
            coder.param.append([tree.name,coder.params])
            coder.emitRM("LDP",coder.ac,coder.params,coder.gp,"ld param")
            coder.params+=1
            if(coder.traceCode==True):
                coder.emitComment("<- Param")
        elif(tree.kind=="CallParamK"):
            if(coder.traceCode==True):
                coder.emitComment("-> CallParam")
            coder.cGen(tree.child[0])
            coder.emitRM("STP",coder.ac,0,0,"store param")
            if(coder.traceCode==True):
                coder.emitComment("<- CallParam")
        elif(tree.kind=="CallK"):
            if(coder.traceCode==True):
                coder.emitComment("-> call")
            coder.cGen(tree.child[0])
            currentLoc=coder.func_lookup(tree.name)
            if(currentLoc==-1):
                savedLoc1=coder.emitSkip(1)
                coder.call.append([tree.name,savedLoc1])
            else:
                coder.emitRM("CALL",coder.pc,coder.func[currentLoc][1]+1,coder.pc,"jmp to func")
            coder.emitRestore()
            if(coder.traceCode==True):
                coder.emitComment("<- call")
        if(tree.kind=="IncludeK"):
            if(coder.traceCode==True):
                coder.emitComment("-> include")
            coder.cGen(tree.child[0])
            if(coder.traceCode==True):
                coder.emitComment("<- include")
        elif(tree.kind=="ConstK"):
            if(coder.traceCode==True):
                coder.emitComment("-> Const")
            coder.emitRM("LDC",coder.ac,tree.val,0,"load const")
            if(coder.traceCode==True):
                coder.emitComment("<- Const")
        elif(tree.kind=="IdK"):
            if(coder.traceCode==True):
                coder.emitComment("-> Id")
            loc=coder.param_lookup(tree.name)
            if(loc!=-1):
                coder.emitRM("LDP",coder.ac,loc,coder.gp,"ld param")
            else:
                loc=coder.symtab.st_lookup(tree.name)
                coder.emitRM("LD",coder.ac,loc,coder.gp,"load id value")
            if(coder.traceCode==True):
                coder.emitComment("<- Id")
        elif(tree.kind=="OpK"):
            if(coder.traceCode==True):
                coder.emitComment("-> Op")
                p1=tree.child[0]
                p2=tree.child[1]
                coder.cGen(p1)
                coder.emitRM("ST",coder.ac,coder.tmpOffset,coder.mp,"op: push left")
                coder.tmpOffset-=1
                coder.cGen(p2)
                coder.tmpOffset+=1
                coder.emitRM("LD",coder.ac1,coder.tmpOffset,coder.mp,"op: load left")
                if(tree.op=="+"):
                    coder.emitRO("ADD",coder.ac,coder.ac1,coder.ac,"op +")
                elif(tree.op=="-"):
                    coder.emitRO("SUB",coder.ac,coder.ac1,coder.ac,"op -")
                elif(tree.op=="*"):
                    coder.emitRO("MUL",coder.ac,coder.ac1,coder.ac,"op *")
                elif(tree.op=="/"):
                    coder.emitRO("DIV",coder.ac,coder.ac1,coder.ac,"op /")
                elif(tree.op=="%"):
                    coder.emitRO("RDE",coder.ac,coder.ac1,coder.ac,"op %")
                elif(tree.op=="<"):
                    coder.emitRO("SUB",coder.ac,coder.ac1,coder.ac,"op <")
                    coder.emitRM("JLT",coder.ac,2,coder.pc,"br if true")
                    coder.emitRM("LDC",coder.ac,0,coder.ac,"false case")
                    coder.emitRM("LDA",coder.pc,1,coder.pc,"unconditional jmp")
                    coder.emitRM("LDC",coder.ac,1,coder.ac,"true case")
                elif(tree.op=="<="):
                    coder.emitRO("SUB",coder.ac,coder.ac1,coder.ac,"op <=")
                    coder.emitRM("JLE",coder.ac,2,coder.pc,"br if true")
                    coder.emitRM("LDC",coder.ac,0,coder.ac,"false case")
                    coder.emitRM("LDA",coder.pc,1,coder.pc,"unconditional jmp")
                    coder.emitRM("LDC",coder.ac,1,coder.ac,"true case")
                elif(tree.op==">="):
                    coder.emitRO("SUB",coder.ac,coder.ac1,coder.ac,"op >=")
                    coder.emitRM("JGE",coder.ac,2,coder.pc,"br if true")
                    coder.emitRM("LDC",coder.ac,0,coder.ac,"false case")
                    coder.emitRM("LDA",coder.pc,1,coder.pc,"unconditional jmp")
                    coder.emitRM("LDC",coder.ac,1,coder.ac,"true case")
                elif(tree.op=="!="):
                    coder.emitRO("SUB",coder.ac,coder.ac1,coder.ac,"op !=")
                    coder.emitRM("JNE",coder.ac,2,coder.pc,"br if true")
                    coder.emitRM("LDC",coder.ac,0,coder.ac,"false case")
                    coder.emitRM("LDA",coder.pc,1,coder.pc,"unconditional jmp")
                    coder.emitRM("LDC",coder.ac,1,coder.ac,"true case")
                elif(tree.op=="&&"):
                    coder.emitRO("SUB",coder.ac,coder.ac1,coder.ac,"op &&")
                    coder.emitRM("AND",coder.ac,2,coder.pc,"br if true")
                    coder.emitRM("LDC",coder.ac,0,coder.ac,"false case")
                    coder.emitRM("LDA",coder.pc,1,coder.pc,"unconditional jmp")
                    coder.emitRM("LDC",coder.ac,1,coder.ac,"true case")
                elif(tree.op=="||"):
                    coder.emitRO("SUB",coder.ac,coder.ac1,coder.ac,"op ||")
                    coder.emitRM("OR",coder.ac,2,coder.pc,"br if true")
                    coder.emitRM("LDC",coder.ac,0,coder.ac,"false case")
                    coder.emitRM("LDA",coder.pc,1,coder.pc,"unconditional jmp")
                    coder.emitRM("LDC",coder.ac,1,coder.ac,"true case")
                elif(tree.op==">"):
                    coder.emitRO("SUB",coder.ac,coder.ac1,coder.ac,"op >")
                    coder.emitRM("JGT",coder.ac,2,coder.pc,"br if true")
                    coder.emitRM("LDC",coder.ac,0,coder.ac,"false case")
                    coder.emitRM("LDA",coder.pc,1,coder.pc,"unconditional jmp")
                    coder.emitRM("LDC",coder.ac,1,coder.ac,"true case")
                elif(tree.op=="=="):
                    coder.emitRO("SUB",coder.ac,coder.ac1,coder.ac,"op ==")
                    coder.emitRM("JEQ",coder.ac,2,coder.pc,"br if true")
                    coder.emitRM("LDC",coder.ac,0,coder.ac,"false case")
                    coder.emitRM("LDA",coder.pc,1,coder.pc,"unconditional jmp")
                    coder.emitRM("LDC",coder.ac,1,coder.ac,"true case")
            if(coder.traceCode==True):
                coder.emitComment("<- Op")

    def call_lookup(coder,name):
        i=0
        while(i<len(coder.call)):
            if(operator.eq(coder.call[i][0],name)==True):
                return i
            i+=1
        return -1

    def cGen(coder,tree):
        if(tree!=None):
            a=coder.call_lookup(tree.name)
            if(tree.kind=="FuncK"and a!=-1):
                currentLoc=coder.emitLoc
                coder.emitBackup(coder.call[a][1])
                coder.emitRM("CALL",coder.pc,currentLoc+1,coder.pc,"jmp to func")
                coder.emitRestore()
                del coder.call[a]
            coder.gen(tree)
            coder.cGen(tree.sibling)

    def codeGen(coder,tree,codefile):
        coder.emitComment("TINY Compilation to TM Code")
        coder.emitComment("File: "+str(codefile))
        coder.emitComment("Standard prelude:")
        coder.emitRM("LD",coder.mp,0,coder.ac,"load maxaddress from location 0")
        coder.emitRM("ST",coder.ac,0,coder.ac,"clear location 0")
        coder.emitComment("End of standard prelude.")
        coder.cGen(tree)

        currentLoc=coder.func_lookup("main\0")
        coder.emitRM("CALL",coder.pc,coder.func[currentLoc][1]+1,coder.pc,"jmp to func")
        coder.emitRestore()
        
        coder.emitComment("End of execution.")
        coder.emitRO("HALT",0,0,0,"")
	
def complie(filename):
    file=""
    for i in range(len(filename)):
        if(filename[i]!="\0"):
            file+=filename[i]

    try:
        source = open(file,"r")
    except IOError:
        print("file '%s' not found"%(file))
        exit()

    lexer=Lexer(source)
    parser=Parser(lexer)
    tree=parser.buildtree()
    source.close()
    
    return tree

def main(filename):
    try:
        source = open(filename,"r")
    except IOError:
        print("file '%s' not found"%(filename))
        return
    
    lexer=Lexer(source)
    parser=Parser(lexer)
    tree=parser.buildtree()
    tree.printtree(1)
    source.close()

    symtab=Symtab()
    analyzer=Analyzer(symtab)
    print("\n Building Symbol Table... ")
    analyzer.buildSymtab(tree)
    print("\n Checking Types...")
    analyzer.typeCheck(tree)

    print("\n Type Checking Finished")

    out=open(filename+"bc","w")
    coder=Coder(out,symtab,True)
    coder.codeGen(tree,filename+"bc")
    out.close()
if(__name__=="__main__"):
    main("in.cqq")
