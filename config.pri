DEPENDPATH += ../../ui ../../lib ../ui ../lib
INCLUDEPATH += ../../ui ../../lib ../ui ../lib
QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter
QT += network xml

macx {
	INCLUDEPATH += /opt/local/include # ugly
	DEFINES += HAVE_DNSSD
	LIBS += -lboost_signals
}


!win32 {
	PKGCONFIG += xmms2-client xmms2-client-cpp
	CONFIG += link_pkgconfig debug
}

win32 {
	INCLUDEPATH += /c/temp/include/xmms2
	INCLUDEPATH += /include
	LIBPATH += /c/temp/lib
	LIBPATH += /lib
	LIBS += -lxmmsclient -lxmmsclient++ -lboost_signals -lws2_32
	RC_FILE = esperanza.rc
}