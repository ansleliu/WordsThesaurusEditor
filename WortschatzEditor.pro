#-------------------------------------------------
#
# Project created by QtCreator 2013-12-27T09:29:58
#
#-------------------------------------------------

QT       += core gui sql network xml

TARGET = WortschatzEditor
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    editordialog.cpp \
    aboutdialog.cpp \
    startdialog.cpp \
    addnewrecorddialog.cpp \
    importexceldialog.cpp \
    MySQLite.cpp \
    CreatWordListDB.cpp \
    ImportExcel.cpp \
    ImportTxt.cpp \
    ImportTXTDialog.cpp \
    WordListDB.cpp \
    DownloadControl.cpp \
    Download.cpp \
    pathmanage.cpp \
    StandardDB.cpp \
    WordListCreate.cpp \
    DownloadGoogleDialog.cpp \
    DownloadProcess.cpp \
    StandardDialog.cpp \
    ExportExcel.cpp \
    ExportExcelDialog.cpp \
    AutomaticFillingUnit.cpp \
    AutoFillingUnitDialog.cpp \
    TranslatDehelperBackup.cpp \
    XMLParser.cpp \
    TranslatDehelperDialog.cpp

HEADERS  += mainwindow.h \
    editordialog.h \
    aboutdialog.h \
    startdialog.h \
    addnewrecorddialog.h \
    importexceldialog.h \
    MySQLite.h \
    CreatWordListDB.h \
    ImportExcel.h \
    ImportTxt.h \
    ImportTXTDialog.h \
    WordListDB.h \
    DownloadControl.h \
    Download.h \
    pathmanage.h \
    StandardDB.h \
    WordListCreate.h \
    DownloadGoogleDialog.h \
    DownloadProcess.h \
    StandardDialog.h \
    ExportExcel.h \
    ExportExcelDialog.h \
    AutomaticFillingUnit.h \
    AutoFillingUnitDialog.h \
    TranslatDehelperBackup.h \
    XMLParser.h \
    TranslatDehelperDialog.h

FORMS    += mainwindow.ui \
    editordialog.ui \
    aboutdialog.ui \
    startdialog.ui \
    addnewrecorddialog.ui \
    importexceldialog.ui \
    ImportTXTDialog.ui \
    DownloadGoogleDialog.ui \
    StandardDialog.ui \
    ExportExcelDialog.ui \
    AutoFillingUnitDialog.ui \
    TranslatDehelperDialog.ui

UI_DIR = ./ui
MOC_DIR = TEMP/moc
RCC_DIR = TEMP/rcc
OBJECTS_DIR = TEMP/obj

RESOURCES += \
    rc.qrc

OTHER_FILES += \
    myapp.rc

RC_FILE = myapp.rc

CONFIG(debug, debug|release) {
    win32: LIBS += -L$$PWD/VLD/lib/Win32/ -lvld

    INCLUDEPATH += $$PWD/VLD/include
    DEPENDPATH += $$PWD/VLD/include
}
