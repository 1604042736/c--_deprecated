from PyQt5.QtWidgets import *
from PyQt5.QtCore import *
from PyQt5.QtGui import *


class Highlighter(QSyntaxHighlighter):
    Rules = []
    Formats = {}

    def __init__(self, parent=None):
        super(Highlighter, self).__init__(parent)

        self.initializeFormats()

        KEYWORDS = ["and", "elif", "else", "if", "or",
                    "pass", "return", "while", "int", "void","extern"]

        Highlighter.Rules.append((QRegExp(
            "|".join([r"\b%s\b" % keyword for keyword in KEYWORDS])),
            "keyword"))
        Highlighter.Rules.append((QRegExp(
            r"\b[+-]?[0-9]+[lL]?\b"
            r"|\b[+-]?0[xX][0-9A-Fa-f]+[lL]?\b"
            r"|\b[+-]?[0-9]+(?:\.[0-9]+)?(?:[eE][+-]?[0-9]+)?\b"),
            "number"))
        Highlighter.Rules.append((QRegExp(
            r"\bPyQt4\b|\bQt?[A-Z][a-z]\w+\b"), "pyqt"))
        Highlighter.Rules.append((QRegExp(r"\b@\w+\b"),
                                  "decorator"))
        stringRe = QRegExp(r"""(?:'[^']*'|"[^"]*")""")
        stringRe.setMinimal(True)
        Highlighter.Rules.append((stringRe, "string"))
        self.stringRe = QRegExp(r"""(:?"["]".*"["]"|'''.*''')""")
        self.stringRe.setMinimal(True)
        Highlighter.Rules.append((self.stringRe, "string"))
        self.tripleSingleRe = QRegExp(r"""'''(?!")""")
        self.tripleDoubleRe = QRegExp(r'''"""(?!')''')

    @staticmethod
    def initializeFormats():
        baseFormat = QTextCharFormat()
        baseFormat.setFontFamily("courier")
        baseFormat.setFontPointSize(12)
        for name, color in (("normal", Qt.black),
                            ("keyword", Qt.darkBlue), ("builtin", Qt.darkRed),
                            ("constant", Qt.darkGreen),
                            ("decorator", Qt.darkBlue), ("comment", Qt.darkGreen),
                            ("string", Qt.darkYellow), ("number", Qt.darkMagenta),
                            ("error", Qt.darkRed), ("pyqt", Qt.darkCyan)):
            format = QTextCharFormat(baseFormat)
            format.setForeground(QColor(color))
            if name in ("keyword", "decorator"):
                format.setFontWeight(QFont.Bold)
            if name == "comment":
                format.setFontItalic(True)
            Highlighter.Formats[name] = format

    def highlightBlock(self, text):
        NORMAL, TRIPLESINGLE, TRIPLEDOUBLE, ERROR = range(4)

        textLength = len(text)
        prevState = self.previousBlockState()

        self.setFormat(0, textLength,
                       Highlighter.Formats["normal"])

        if text.startswith("Traceback") or text.startswith("Error: "):
            self.setCurrentBlockState(ERROR)
            self.setFormat(0, textLength,
                           Highlighter.Formats["error"])
            return
        if (prevState == ERROR and
                not (text.startswith(sys.ps1) or text.startswith("#"))):
            self.setCurrentBlockState(ERROR)
            self.setFormat(0, textLength,
                           Highlighter.Formats["error"])
            return

        for regex, format in Highlighter.Rules:
            i = regex.indexIn(text)
            while i >= 0:
                length = regex.matchedLength()
                self.setFormat(i, length,
                               Highlighter.Formats[format])
                i = regex.indexIn(text, i + length)

        # Slow but good quality highlighting for comments. For more
        # speed, comment this out and add the following to __init__:
        # PythonHighlighter.Rules.append((QRegExp(r"#.*"), "comment"))
        if not text:
            pass
        elif text[0] == "#":
            self.setFormat(0, len(text),
                           Highlighter.Formats["comment"])
        else:
            stack = []
            for i, c in enumerate(text):
                if c in ('"', "'"):
                    if stack and stack[-1] == c:
                        stack.pop()
                    else:
                        stack.append(c)
                elif c == "#" and len(stack) == 0:
                    self.setFormat(i, len(text),
                                   Highlighter.Formats["comment"])
                    break

        self.setCurrentBlockState(NORMAL)

        if self.stringRe.indexIn(text) != -1:
            return
        # This is fooled by triple quotes inside single quoted strings
        for i, state in ((self.tripleSingleRe.indexIn(text),
                          TRIPLESINGLE),
                         (self.tripleDoubleRe.indexIn(text),
                          TRIPLEDOUBLE)):
            if self.previousBlockState() == state:
                if i == -1:
                    i = text.length()
                    self.setCurrentBlockState(state)
                self.setFormat(0, i + 3,
                               Highlighter.Formats["string"])
            elif i > -1:
                self.setCurrentBlockState(state)
                self.setFormat(i, text.length(),
                               Highlighter.Formats["string"])

    def rehighlight(self):
        QApplication.setOverrideCursor(QCursor(Qt.WaitCursor))
        QSyntaxHighlighter.rehighlight(self)
        QApplication.restoreOverrideCursor()
