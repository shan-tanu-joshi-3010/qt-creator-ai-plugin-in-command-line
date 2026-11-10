cmake_minimum_required(VERSION 3.16)
project(AICreatorPlugin VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Core Widgets REQUIRED)
find_package(QtCreator REQUIRED COMPONENTS Core TextEditor ExtensionSystem)

add_qtc_plugin(AICreator
  PLUGIN_DEPENDS
    QtCreator::Core
    QtCreator::TextEditor
  SOURCES
    aicreatorplugin.cpp
    aicreatorplugin.h
)