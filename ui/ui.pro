DEPENDPATH += ..
INCLUDEPATH += . /sw/include ../lib 
CONFIG += link_pkgconfig debug
QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter
PKGCONFIG += xmms2-client xmms2-client-cpp
LIBS += -lxmmsqt
LIBPATH += ../lib

TEMPLATE=app
TARGET=simpleqt
RESOURCES += simpleqt.qrc

# Input
HEADERS += playlistview.h playerwidget.h progressframe.h browsedialog.h playerbutton.h filedialog.h medialibdialog.h medialibview.h serverdialog.h
SOURCES += main.cpp playlistview.cpp playerwidget.cpp progressframe.cpp browsedialog.cpp playerbutton.cpp filedialog.cpp medialibdialog.cpp medialibview.cpp serverdialog.cpp
