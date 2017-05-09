TEMPLATE = app
QMAKE_CXXFLAGS= -std=c++11
LIBS += -lGL -lGLU -lglut -lGLEW

SOURCES += \
    circle.cpp \
    point.cpp \
    square.cpp \
    line.cpp \
    drawable.cpp \
    InitShader.cpp \
    main.cpp

OTHER_FILES += \
    fshader.glsl \
    vshader.glsl \
    traffic

HEADERS += \
    circle.h \
    square.h \
    point.h \
    line.h \
    drawable.h \
    include/vec.h \
    include/CheckError.h \
    include/Angel.h \
    include/mat.h
