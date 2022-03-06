from PyQt5.QtWidgets import *
from highlighter import *
import json
import os

class Editor(QTextEdit):
    def __init__(self,filepath='<unkown>'):
        super().__init__()
        if filepath!='<unkown>':
            self.setText(open(filepath,encoding='utf-8').read())
        self.highlighter = Highlighter(self.document())
        self.filepath=filepath

    def event(self, event):
        if (event.type() == QEvent.KeyPress and
            event.key() == Qt.Key_Tab):
            cursor = self.textCursor()
            cursor.insertText(" "*4)
            return True
        return QTextEdit.event(self, event)

    def savefile(self):
        if self.filepath=='<unkown>':
            self.filepath, filetype = QFileDialog.getSaveFileName(self,  
                                    "文件保存",  
                                    ".", # 起始路径 
                                    "All Files (*);")  
            if self.filepath=="":
                self.filepath='<unkown>'
                return
        open(self.filepath,encoding='utf-8',mode='w').write(self.toPlainText())

    def runfile(self):
        self.savefile()
        config=json.load(open('config.json',encoding='utf-8'))
        home=config['home']
        filename,ext=os.path.splitext(self.filepath)
        asmname=filename+'.asm'
        objname=filename+'.o'
        exename=filename+'.exe'
        os.system(f'python "{home}\\main.py" "{self.filepath}"')
        os.system(f'nasm "{asmname}" -f elf32')
        os.system(f'gcc "{objname}" -o "{exename}"')
        os.system(f'start python runner.py "{exename}"')