from expr import *

class And(Expr):
    '''
    与
    '''
    def gen(self):
        truelabel=self.genir.get_label()
        falselabel=self.genir.get_label()
        endlabel=self.genir.get_label()

        self.genir.add_inst('pop','ebx')
        self.genir.add_inst('pop','eax')
        self.genir.add_inst('cmp','eax','0')
        self.genir.add_inst('je',falselabel)
        self.genir.add_inst('cmp','ebx','0')
        self.genir.add_inst('je',falselabel)
        self.genir.add_label(truelabel)
        self.genir.add_inst('push',1)
        self.genir.add_inst('jmp',endlabel)
        self.genir.add_label(falselabel)
        self.genir.add_inst('push',0)
        self.genir.add_label(endlabel)