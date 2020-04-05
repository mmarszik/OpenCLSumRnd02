TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++14

LIBS += /usr/lib/x86_64-linux-gnu/libOpenCL.so

QMAKE_CXXFLAGS+= -fopenmp
QMAKE_LFLAGS +=  -fopenmp

SOURCES += main.cpp

DISTFILES += \
    OpenCLSumRnd.cl \
    run.sh \
    make.sh
