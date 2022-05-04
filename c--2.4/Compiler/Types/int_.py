from type_ import *


class Int(Type):
    def __init__(self, **kwargs) -> None:
        super().__init__(**kwargs)
        self.size = 4

    def bool(self):
        result = []
        false_label = self._genir.get_label()
        final_label = self._genir.get_label()

        result.append(['pop', 'eax'])
        result.append(['cmp', 'eax', '0'])
        result.append(['je',false_label])
        result.append(['push','1'])
        result.append(['jmp',final_label])
        result.append([false_label+':'])
        result.append(['push','0'])
        result.append([final_label+':'])

        return result