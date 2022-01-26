import operator
import sys

class Instruction:
    def __init__(ins):
        ins.iop="opHALT"
        ins.iarg1=0
        ins.iarg2=0
        ins.iarg3=0
        
class Opcode:
    def __init__(opcode):
        opcode.code=[\
                        ["opHALT",0],\
                        ["opIN",1],\
                        ["opOUT",2],\
                        ["opADD",3],\
                        ["opSUB",4],\
                        ["opMUL",5],\
                        ["opDIV",6],\
                        ["opRDE",7],\
                        ["opRRLim",8],\
                        ["opLD",9],\
                        ["opST",10],\
                        ["opRMLim",11],\
                        ["opLDA",12],\
                        ["opLDC",13],\
                        ["opJLT",14],\
                        ["opJLE",15],\
                        ["opJGT",16],\
                        ["opJGE",17],\
                        ["opJEQ",18],\
                        ["opJNE",19],\
                        ["opAND",20],\
                        ["opOR",21],\
                        ["opRET",22],\
                        ["opCALL",23],\
                        ["opSTP",24],\
                        ["opLDP",25],\
                        ["opASTP",26],\
                        ["opRALim",27]\
                    ]
        opcode.op="opHALT"
        opcode.oplen=0

    def getopnum(opcode):
        i=0
        while(i<len(opcode.code)):
            if(operator.eq(opcode.op,opcode.code[i][0])==True):
                return opcode.code[i][1]
            i+=1

    def getop(opcode):
        return opcode.code[opcode.oplen][0]

    def nextop(opcode):
        opcode.oplen+=1
        if(opcode.oplen==28):
            opcode.oplen=0
        opcode.op=opcode.getop()

opCodeTab=["HALT","IN","OUT","ADD","SUB","MUL","DIV","RDE","????",\
           "LD","ST","????",\
           "LDA","LDC","JLT","JLE","JGT","JGE","JEQ","JNE","AND","OR",\
           "RET","CALL","STP","LDP","ASTP","????"\
          ]

reg=[0 for i in range(8)]
dMem=[0 for i in range(1024)]
dMem[0]=1023
iMem=[Instruction() for i in range(1024)]
pMem=[]
iloc=0
dloc=0
traceflag=False
icountflag=True
inCol=0
lineLen=0
in_Line=['' for i in range(121)]
num=0
word=['' for i in range(20)]
ch=''
ret=[]

def getCh():
    global inCol
    global lineLen
    global ch
    global in_Line
    inCol+=1
    if(inCol<lineLen):
        ch=in_Line[inCol]
    else:
        ch=' '

def skipCh(c):
    global ch
    temp=False
    if(nonBlank()and(ch==c)):
        getCh()
        temp=True
    return temp

def getWord():
    global ch
    global word
    temp=False
    length=0
    if(nonBlank()):
        while(str.isalnum(ch)):
            if(length<19):
                word[length]=ch
                length+=1
            getCh()
        word[length]='\0'
        temp=bool(length!=0)
    return temp

def getNum():
    global num
    global ch
    temp=False
    num=0
    sign=1
    while(nonBlank()and((ch=='+')or(ch=='-'))):
        temp=False
        if(ch=='-'):
            sign=-sign
        getCh()
    term=0
    nonBlank()
    while(str.isdigit(ch)):
        temp=True
        term=term*10+int(ch)
        getCh()
    num=num+(term*sign)
    while((nonBlank())and((ch=='+')or(ch=='-'))):
        sign=1
        while(nonBlank()and((ch=='+')or(ch=='-'))):
            temp=False
            if(ch=='-'):
                sign=-sign
            getCh()
        term=0
        nonBlank()
        while(str.isdigit(ch)):
            temp=True
            term=term*10+int(ch)
            getCh()
        num=num+(term*sign)
    return temp
        

def nonBlank():
    global inCol
    global lineLen
    global in_Line
    global ch
    while((inCol<lineLen)and(in_Line[inCol]==' ')):
        inCol+=1
    if(inCol<lineLen):
        ch=in_Line[inCol]
        return True
    else:
        ch=' '
        return False

def opClass(c):
    if(c<=8):
        return "opclRR"
    elif(c<=11):
        return "opclRM"
    else:
        return "opclRA"

def error(msg,lineNo,instNo):
    print("Line %d"%(lineNo),end="")
    if(instNo>=0):
        print(" (Instruction %d)"%(instNo),end="")
    print("   %s\n"%(msg))
    return False

def getString(st):
    s=""
    i=0
    while(i<len(st)and st[i]!='\0'):
        s+=st[i]
        i+=1
    return s

def readInstructions(pgm):
    global inCol
    global lineLen
    global in_Line
    global num
    global word
    global ch
    global iMem
    global reg
    opcode=Opcode()
    arg1=arg2=arg3=loc=regNo=lineNo=0
    
    in_Line=list(pgm.readline())
    while(in_Line):
        inCol=0
        lineNo+=1
        lineLen=len(in_Line)-1
        if(in_Line[lineLen]=='\n'):
            in_Line[lineLen]='\0'
        else:
            lineLen+=1
            in_Line[lineLen]='\0'
        if((nonBlank())and(in_Line[inCol]!='*')):
            if(not getNum()):
                return error("Bad location",lineNo,-1)
            loc=num
            if(loc>1024):
                return error("Location too large",lineNo,loc)
            if(not skipCh(':')):
                return error("Missing colon",lineNo,loc)
            if(not getWord()):
                return error("Missing opcode",lineNo,loc)
            opcode.op="opHALT"
            opcode.oplen=0
            while((opcode.getopnum()<27)and(operator.eq(opCodeTab[opcode.getopnum()],getString(word))!=True)):
                opcode.nextop()
            if(operator.eq(opCodeTab[opcode.getopnum()],getString(word))!=True):
                return error("Illegal opcode",lineNo,loc)
            a=opClass(opcode.getopnum())
            if(a=="opclRR"):
                if((not getNum())or(num<0)or(num>=8)):
                    return error("Bad first register",lineNo,loc)
                arg1=num
                if(not skipCh(',')):
                    return error("Missing comma",lineNo,loc)
                if((not getNum())or(num<0)or(num>=8)):
                    return error("Bad second register",lineNo,loc)
                arg2=num
                if(not skipCh(',')):
                    return error("Missing comma",lineNo,loc)
                if((not getNum())or(num<0)or(num>=8)):
                    return error("Bad third register",lineNo,loc)
                arg3=num
            elif(a=="opclRM"or a=="opclRA"):
                if((not getNum())or(num<0)or(num>=8)):
                    return error("Bad first register",lineNo,loc)
                arg1=num
                if(not skipCh(',')):
                    return error("Missing comma",lineNo,loc)
                if(not getNum()):
                    return error("Bad displacement",lineNo,loc)
                arg2=num
                if(not skipCh('(')and (not skipCh(','))):
                    return error("Missing LParen",lineNo,loc)
                if((not getNum())or(num<0)or(num>=8)):
                    return error("Bad second register",lineNo,loc)
                arg3=num
            iMem[loc].iop=opcode.op
            iMem[loc].iarg1=arg1
            iMem[loc].iarg2=arg2
            iMem[loc].iarg3=arg3
        in_Line=list(pgm.readline())
    return True

def stepTM():
    global reg
    global iMem
    global in_Line
    global lineLen
    global inCol
    global ret
    global pMem
    r=s=t=m=ok=0
    pcc=reg[7]
    if((pcc<0)or(pcc>1024)):
        return "srIMEM_ERR"
    reg[7]=pcc+1
    currentinstruction=iMem[pcc]
    opcode=Opcode()
    opcode.op=currentinstruction.iop
    opcode.oplen=opcode.getopnum()
    a=opClass(opcode.getopnum())
    if(a=="opclRR"):
        r=currentinstruction.iarg1
        s=currentinstruction.iarg2
        t=currentinstruction.iarg3
    elif(a=="opclRM"):
        r=currentinstruction.iarg1
        s=currentinstruction.iarg3
        m=currentinstruction.iarg2+reg[s]
        if((m<0)or(m>1024)):
            return "srDMEM_ERR"
    elif(a=="opclRA"):
        r=currentinstruction.iarg1
        s=currentinstruction.iarg3
        m=currentinstruction.iarg2+reg[s]
    a=currentinstruction.iop
    if(a=="opHALT"):
        return "srHALT"
    elif(a=="opIN"):
        in_Line=input()
        lineLen=len(in_Line)
        inCol=0
        ok=getNum()
        if(ok==True):
            reg[r]=num
        while(not ok):
            in_Line=input()
            lineLen=len(in_Line)
            inCol=0
            ok=getNum()
            if(ok==True):
                reg[r]=num
    elif(a=="opOUT"):
        print(reg[r])
    elif(a=="opADD"):
        reg[r]=reg[s]+reg[t]
    elif(a=="opSUB"):
        reg[r]=reg[s]-reg[t]
    elif(a=="opMUL"):
        reg[r]=reg[s]*reg[t]
    elif(a=="opDIV"):
        if(reg[t]!=0):
            reg[r]=reg[s]/reg[t]
        else:
            return "srZERODIVIDE"
    elif(a=="opRDE"):
        if(reg[t]!=0):
            reg[r]=reg[s]%reg[t]
        else:
            return "srZERODIVIDE"
    elif(a=="opLD"):
        reg[r]=dMem[m]
    elif(a=="opST"):
        dMem[m]=reg[r]
    elif(a=="opSTP"):
        pMem.insert(0,reg[r])
    elif(a=="opLDP"):
        reg[r]=pMem[currentinstruction.iarg2]
    elif(a=="opASTP"):
        pMem[currentinstruction.iarg2]=reg[r]
    elif(a=="opLDA"):
        reg[r]=m
    elif(a=="opLDC"):
        reg[r]=currentinstruction.iarg2
    elif(a=="opCALL"):
        reg[r]=currentinstruction.iarg2
        ret.append(pcc)
    elif(a=="opRET"):
        for i in range(currentinstruction.iarg2):
            del pMem[0]
        reg[r]=ret.pop()+1
    elif(a=="opJLT"):
        if(reg[r]<0):
            reg[7]=m
    elif(a=="opJLE"):
        if(reg[r]<=0):
            reg[7]=m
    elif(a=="opJGT"):
        if(reg[r]>0):
            reg[7]=m
    elif(a=="opJGE"):
        if(reg[r]>=0):
            reg[7]=m
    elif(a=="opJEQ"):
        if(reg[r]==0):
            reg[7]=m
    elif(a=="opJNE"):
        if(reg[r]!=0):
            reg[7]=m
    elif(a=="opAND"):
        if(reg[r]and 0):
            reg[7]=m
    elif(a=="opOR"):
        if(reg[r]or 0):
            reg[7]=m
    return "srOKAY"

def doCommand():
    global iloc
    global reg
    global traceflag
    stepResult = "srOKAY"
    while(stepResult == "srOKAY"):
        iloc=reg[7]
        stepResult = stepTM()

def main(filename):
    try:
        pgm=open(filename,"r")
    except IOError:
        return id(IOError)
    a=readInstructions(pgm)
    if(not a):
        return id(a)
    doCommand()
    pgm.close()
    return 0

if(__name__=="__main__"):
    a=main("in.cqqbc")
