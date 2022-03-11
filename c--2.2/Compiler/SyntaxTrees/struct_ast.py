from type import *

class Struct(Type):
    def __init__(self, **kwargs) -> None:
        self.name:str=''
        super().__init__(**kwargs)
        
    def analyse(self):
        self.size=0
        struct=self.symtab.get_struct(self.name)
        for key,val in struct.member.items():
            self.size+=val.type.size

    def get_attr(self,name):
        struct=self.symtab.get_struct(self.name)
        return struct.member[name]

    def load(self):
        self.genir.add_inst('mov','eax','ecx')
        self.genir.add_inst('push','eax')

    def store(self):
        self.genir.add_inst('pop','eax')
        self.genir.add_inst('mov','ecx','eax')

    def __str__(self) -> str:
        return f'struct {self.name}'