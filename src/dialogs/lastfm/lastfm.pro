TEMPLATE = lib
QT += network xml
CONFIG += static plugin
include (../../../config.pri)
SOURCES = lastfm.cpp lastfmparser.cpp lastfmhandler.cpp valuebar.cpp plugin.cpp
HEADERS = lastfm.h lastfmparser.h lastfmhandler.h valuebar.h plugin.h
