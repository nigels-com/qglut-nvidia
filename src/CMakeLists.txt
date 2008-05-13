
SET(SRCS
	qglut.cpp
	qglutwidget.h
	qglutwidget.cpp
	qglutapp.h
	qglutapp.cpp)

QT4_WRAP_CPP(MOCS qglutwidget.h qglutapp.h)

#ADD_LIBRARY(qglut SHARED ${SRCS} ${MOCS})
ADD_LIBRARY(qglut ${SRCS} ${MOCS})
TARGET_LINK_LIBRARIES(qglut ${LIBS})

ADD_EXECUTABLE(qgluttest qgluttest.cpp)
TARGET_LINK_LIBRARIES(qgluttest qglut ${LIBS})


