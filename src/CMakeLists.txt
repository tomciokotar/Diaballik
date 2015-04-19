set (SOURCES
 grafika.cpp
 main.cpp
 mainwindow.cpp
 pilka.cpp
 pionek.cpp
 plansza.cpp
 pole.cpp
 przes.cpp
 ruch.cpp
)

set (HEADERS
 mainwindow.h
 plansza.h
 ruch.h
)

qt4_wrap_cpp (SOURCES ${HEADERS})
add_executable (Diaballik ${SOURCES})
target_link_libraries (Diaballik ${QT_LIBRARIES})
