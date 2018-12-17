#!/usr/bin/env python

# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'mission.ui'
#
# Created by: PyQt4 UI code generator 4.11.4

# Import required packages
from PyQt5 import QtCore, QtGui,QtWidgets
from PyQt5.uic import loadUi # UIC: UI Package Loader
import sys
# from PyQt4.QtCore import QThread,SIGNAL
# from datetime import datetime

class Ui_RoverControlCenter(QtWidgets.QMainWindow):
    def __init__(self):
        super(Ui_RoverControlCenter,self).__init__()
        loadUi('rovercontrolcenter.ui', self)
        # self.setup()
        # self.getThread = getTileThread()
        # self.connect(self.getThread, SIGNAL("showMap()"),self.showMap)
        # self.getThread.start()

    # def setup(self):


if __name__ == "__main__":
    app = QtWidgets.QApplication(sys.argv)
    ui = Ui_RoverControlCenter()
    ui.show()

    sys.exit(app.exec_())