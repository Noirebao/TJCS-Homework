# This Python file uses the following encoding: utf-8

from Gui import *

if __name__ == "__main__":
    app = QtWidgets.QApplication(sys.argv)
    gui = myUI()
    gui.show()
    sys.exit(app.exec_())