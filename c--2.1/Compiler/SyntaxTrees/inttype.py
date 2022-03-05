from type import *

class IntType(Type):
    '''
    整型
    '''
    size=4
    def set_symtab(self,name):
        islocal=bool(self.symtab.curfunc)
        var=VarSym(name,self,offset=self.symtab.get_offset(),islocal=islocal)
        self.symtab.add_var(var)
        self.symtab.add_offset(self.size)

    def gen(self,var):
        if var.offset>=0:
            self.genir.add_inst('sub','ecx',var.offset)
        else:
            self.genir.add_inst('add','ecx',-var.offset+4)

    def get_size(self):
        return self.size