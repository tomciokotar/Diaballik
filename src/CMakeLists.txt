set (SOURCES
 graphics.cpp
 main.cpp
 mainWindow.cpp
 ball.cpp
 piece.cpp
 board.cpp
 square.cpp
 movement.cpp
 turn.cpp
)

set (HEADERS
 mainWindow.h
 board.h
 turn.h
)

qt4_wrap_cpp (SOURCES ${HEADERS})
add_executable (Diaballik ${SOURCES})
target_link_libraries (Diaballik ${QT_LIBRARIES})
