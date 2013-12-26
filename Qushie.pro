#-------------------------------------------------
#
# Project created by QtCreator 2013-12-11T10:43:10
#
#-------------------------------------------------

QT       += core gui network webkit sql

TARGET = Qushie
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    helpers.cpp \
    QTtelnet.cpp \
    QushieFile.cpp \
    zlib-1.2.8/zutil.c \
    zlib-1.2.8/uncompr.c \
    zlib-1.2.8/trees.c \
    zlib-1.2.8/inftrees.c \
    zlib-1.2.8/inflate.c \
    zlib-1.2.8/inffast.c \
    zlib-1.2.8/infback.c \
    zlib-1.2.8/gzwrite.c \
    zlib-1.2.8/gzread.c \
    zlib-1.2.8/gzlib.c \
    zlib-1.2.8/gzclose.c \
    zlib-1.2.8/deflate.c \
    zlib-1.2.8/crc32.c \
    zlib-1.2.8/compress.c \
    zlib-1.2.8/adler32.c \
    QushieDatabase.cpp


HEADERS  += MainWindow.h \
    helpers.h \
    QTelnet.h \
    QushieFile.h \
    zlib-1.2.8/zutil.h \
    zlib-1.2.8/zlib.h \
    zlib-1.2.8/zconf.h \
    zlib-1.2.8/trees.h \
    zlib-1.2.8/inftrees.h \
    zlib-1.2.8/inflate.h \
    zlib-1.2.8/inffixed.h \
    zlib-1.2.8/inffast.h \
    zlib-1.2.8/gzguts.h \
    zlib-1.2.8/deflate.h \
    zlib-1.2.8/crc32.h \
    QushieDatabase.h




LIBS += -lws2_32
LIBS += -lwsock32

INCLUDEPATH += zlib-1.2.8

