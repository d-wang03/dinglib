TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

HEADERS += \
    dobject.h \
    dobject_p.h \
    dobjectbase.h \
    dglobal.h \
    dosdef.h \
    dparam.h \
    dparam_p.h \
    dutility.h

SOURCES += \
    dobject.cpp \
    main.cpp \
    dobjectbase.cpp \
    dparam.cpp
