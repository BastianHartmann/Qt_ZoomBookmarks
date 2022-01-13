QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addmeetingdialog.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    addmeetingdialog.h \
    mainwindow.h

FORMS += \
    addmeetingdialog.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    .gitignore \
    Add_Icon_zoom_bookmarks.png \
    App_icon.ico \
    App_icon.png \
    LICENSE \
    Meeting_icon_zoom_bookmarks.png \
    README.md \
    Trash_Icon_zoom_bookmarks.png \
    ZoomBookmarks_install.zip \
    meetings.json

RC_ICONS = App_icon.ico

RESOURCES += \
    Icons.qrc
