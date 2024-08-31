project(Laboratorio_listaspesa)
cmake_minimum_required(VERSION 3.28)
aux_source_directory(. SRC_LIST)
set(CMAKE_CXX_STANDARD 17++)
# Controllo della piattaforma per configurazioni specifiche
if(WIN32)
    set(wxWidgets_CONFIGURATION mswu)
elseif(APPLE)
    # Se necessario, aggiungere flag o configurazioni specifiche per macOS
    # ad esempio, set(wxWidgets_CONFIGURATION osx_cocoa) se necessario
endif()

find_package(wxWidgets COMPONENTS core base REQUIRED)
include(${wxWidgets_USE_FILE})

add_executable(${PROJECT_NAME} ${SRC_LIST}
        App.cpp
        App.h
        Mainframe.cpp
        Mainframe.h
        Item.h)

# Impostazioni specifiche di compilazione per macOS
if(APPLE)
    target_compile_definitions(${PROJECT_NAME} PRIVATE __WXMAC__)
endif()

target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})

