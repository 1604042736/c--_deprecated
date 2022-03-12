from syntaxtree import *
from type import *
from expr import *


class ArgDef(SyntaxTree):
    def __init__(self, **kwargs) -> None:
        self.type:Type=None
        self.name:str=''
        self.default:Expr=None
        super().__init__(**kwargs)

    def gen(self,arg):
        if arg: #传参数
            arg.gen()
            return arg.type.size
        elif self.default:  #没有参数传默认值
            self.default.gen()
            return self.default.type.size
        else:   #都没有说明缺少位置参数
            self.error('缺少参数')

    def analyse(self):
        self.type.set_val(self.name)
        var=self.symtab.get_var(self.name)
        var.offset=-(var.offset+4)
        super().analyse()