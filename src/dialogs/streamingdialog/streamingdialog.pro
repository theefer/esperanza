TEMPLATE = lib
CONFIG += static plugin
QT += network xml
include (../../../config.pri)
SOURCES = plugin.cpp streamingdialog.cpp icecasthandler.cpp
HEADERS = plugin.h streamingdialog.h icecasthandler.h
