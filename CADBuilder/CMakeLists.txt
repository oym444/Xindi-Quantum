cmake_minimum_required(VERSION 3.5)

project(CADBuilder VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置第三方库路径
set(THIRD_PARTY_PATH "${CMAKE_SOURCE_DIR}/../3rdparty")


find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# 查找VTK包
find_package(VTK REQUIRED  PATHS "${THIRD_PARTY_PATH}/VTK-9.0.1/lib/cmake" COMPONENTS
            vtkCommonColor
            vtkCommonCore
            vtkCommonDataModel
            vtkFiltersSources
            vtkIOGeometry
            vtkInteractionStyle
            vtkRenderingContextOpenGL2
            vtkRenderingCore
            vtkRenderingFreeType
            vtkRenderingGL2PSOpenGL2
            vtkRenderingOpenGL2
            vtkGUISupportQt
            vtkIOImage
            vtkInteractionImage
            QUIET)

# 添加OpenCASCADE配置
set(OCASCADE_ROOT_DIR "${THIRD_PARTY_PATH}/OCCT-7.5.0")  # 假设上级目录有opencascade文件夹
list(APPEND CMAKE_PREFIX_PATH ${OCASCADE_ROOT_DIR})
find_package(OpenCASCADE REQUIRED)
## OpenCascade
#find_package(OpenCASCADE)

# Configure C++ compiler's includes dir
include_directories ( SYSTEM ${OpenCASCADE_INCLUDE_DIR} )


# 查找Google Test
find_package(GTest REQUIRED PATHS "${THIRD_PARTY_PATH}/googletest-1.11.0/lib/cmake")

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        trapezoidmodeler.h
        trapezoidmodeler.cpp
        extruder.h
        extruder.cpp
        vtkconverter.h
        vtkconverter.cpp
        visualizationwindow.h visualizationwindow.cpp
        commandparser.h
        commandhandler.h
        commandinputdialog.h commandinputdialog.cpp
        src.qrc
)


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(CADBuilder
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET CADBuilder APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(CADBuilder SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(CADBuilder
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(CADBuilder PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

#头文件
target_include_directories(CADBuilder PRIVATE ${VTK_INCLUDE_DIRS})
#链接VTK库
target_link_libraries(CADBuilder PRIVATE ${VTK_LIBRARIES})

#链接OpenCASCADE库
foreach(LIB ${OpenCASCADE_LIBRARIES})
  target_link_libraries(CADBuilder PRIVATE
    debug "${OpenCASCADE_LIBRARY_DIR}d/${LIB}.lib"
    optimized "${OpenCASCADE_LIBRARY_DIR}/${LIB}.lib"
  )
endforeach()
set_property(TARGET CADBuilder PROPERTY VS_DEBUGGER_ENVIRONMENT "PATH=$<$<CONFIG:DEBUG>:${OpenCASCADE_BINARY_DIR}d>$<$<NOT:$<CONFIG:DEBUG>>:${OpenCASCADE_BINARY_DIR}>;%PATH%")

# 包含头文件路径
target_include_directories(CADBuilder PRIVATE "${THIRD_PARTY_PATH}/googletest-1.11.0/include")


# 添加测试目标
set(TEST_SOURCES
    unit_tests.cpp
    extruder.h extruder.cpp
    trapezoidmodeler.h trapezoidmodeler.cpp
    vtkconverter.h vtkconverter.cpp
)
add_executable(CADBuilderTests ${TEST_SOURCES})

target_include_directories(CADBuilderTests PRIVATE
    ${VTK_INCLUDE_DIRS}
    ${OpenCASCADE_INCLUDE_DIR}
    "${THIRD_PARTY_PATH}/googletest-1.11.0/include"
)
target_link_libraries(CADBuilderTests PRIVATE Qt${QT_VERSION_MAJOR}::Widgets ${VTK_LIBRARIES})

# 链接OpenCASCADE库
foreach(LIB ${OpenCASCADE_LIBRARIES})
  target_link_libraries(CADBuilderTests PRIVATE
    debug "${OpenCASCADE_LIBRARY_DIR}d/${LIB}.lib"
    optimized "${OpenCASCADE_LIBRARY_DIR}/${LIB}.lib"
  )
endforeach()
set_property(TARGET CADBuilderTests PROPERTY VS_DEBUGGER_ENVIRONMENT "PATH=$<$<CONFIG:DEBUG>:${OpenCASCADE_BINARY_DIR}d>$<$<NOT:$<CONFIG:DEBUG>>:${OpenCASCADE_BINARY_DIR}>;%PATH%")

target_link_libraries(CADBuilderTests PRIVATE GTest::gtest GTest::gmock)

# 启用CTest
enable_testing()
add_test(NAME CADBuilderTests COMMAND CADBuilderTests)



set_target_properties(CADBuilder PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS CADBuilder
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})


if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(CADBuilder)
endif()
