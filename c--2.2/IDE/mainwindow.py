from Forms import *
from PyQt5.QtWidgets import *
from editor import *
import json


class MainWindow(QMainWindow, Ui_MainWindow):
    def __init__(self):
        super().__init__()
        self.__dict__ |= json.load(open('config.json', encoding='utf-8'))

        self.setupUi(self)
        self.new_action.triggered.connect(self.newfile)
        self.open_action.triggered.connect(self.openfile)
        self.save_action.triggered.connect(self.savefile)
        self.run_action.triggered.connect(self.runfile)

    def newfile(self):
        self.tw_main.addTab(Editor(), '未命名')

    def openfile(self):
        filename, filetype = QFileDialog.getOpenFileName(self,
                                                         "打开文件",
                                                         ".",  # 起始路径
                                                         "All Files (*)")
        if filename:
            self.tw_main.addTab(Editor(filename), filename)

    def savefile(self):
        self.tw_main.currentWidget().savefile()

    def runfile(self):
        self.tw_main.currentWidget().runfile()


if __name__ == '__main__':
    app = QApplication(sys.argv)
    mainwindow = MainWindow()
    mainwindow.show()
    sys.exit(app.exec_())
