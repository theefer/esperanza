DEPENDPATH += ..
INCLUDEPATH += . /sw/include ../lib 
CONFIG += link_pkgconfig staticlib debug
QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter
PKGCONFIG += xmms2-client xmms2-client-cpp

TEMPLATE = lib
TARGET = xmmsqt
HEADERS += playlistmodel.h xmmsqt4.h  xclient.h browsemodel.h medialibsearchmodel.h xmediainfocache.h
SOURCES += playlistmodel.cpp xmmsqt4.cpp xclient.cpp browsemodel.cpp medialibsearchmodel.cpp xmediainfocache.cpp 
