cmake_minimum_required(VERSION 3.16)

project(Assignment2 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)

set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/code/Forms)

include_directories(
    ${Qt6Widgets_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/code/HeaderFiles
    ${CMAKE_CURRENT_SOURCE_DIR}/code/SourceFiles
)

# ============================================
# MAIN APPLICATION (Qt GUI)
# ============================================
add_executable(assignment2
    code/SourceFiles/main.cpp

    # Backend classes
    code/HeaderFiles/user.h
    code/SourceFiles/user.cpp

    code/HeaderFiles/supplier.h
    code/SourceFiles/supplier.cpp

    code/HeaderFiles/inventory.h
    code/SourceFiles/inventory.cpp

    code/HeaderFiles/rawmaterial.h
    code/SourceFiles/rawmaterial.cpp

    code/HeaderFiles/nonperishable.h
    code/SourceFiles/nonperishable.cpp

    code/HeaderFiles/perishable.h
    code/SourceFiles/perishable.cpp

    code/HeaderFiles/chemical.h
    code/SourceFiles/chemical.cpp

    code/HeaderFiles/food.h
    code/SourceFiles/food.cpp

    code/HeaderFiles/product.h

    # UI classes
    code/HeaderFiles/dialog.h
    code/SourceFiles/dialog.cpp
    code/Forms/dialog.ui

    code/HeaderFiles/logindialog.h
    code/SourceFiles/logindialog.cpp
    code/Forms/logindialog.ui

    code/SourceFiles/admin.cpp
    code/HeaderFiles/admin.h
    code/SourceFiles/productFactory.cpp
    code/HeaderFiles/productFactory.h
    code/HeaderFiles/productType.h
    code/HeaderFiles/fileManager.h
    code/SourceFiles/fileManager.cpp
    code/HeaderFiles/userManager.h
    code/SourceFiles/userManager.cpp
    code/SourceFiles/systemController.cpp
    code/HeaderFiles/systemController.h
)

target_link_libraries(assignment2 PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
)

set_target_properties(assignment2 PROPERTIES
    WIN32_EXECUTABLE TRUE
)

# ============================================
# GOOGLE TEST SETUP
# ============================================
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.14.0
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Enable testing
enable_testing()

# ============================================
# SHARED LIBRARY FOR TESTING (Core Logic Only)
# ============================================
# This library contains ONLY the backend classes (no Qt, no UI)
# so we can test them independently

add_library(inventory_core STATIC
    code/HeaderFiles/product.h

    code/HeaderFiles/perishable.h
    code/SourceFiles/perishable.cpp

    code/HeaderFiles/food.h
    code/SourceFiles/food.cpp

    code/HeaderFiles/supplier.h
    code/SourceFiles/supplier.cpp

    code/HeaderFiles/inventory.h
    code/SourceFiles/inventory.cpp

    code/HeaderFiles/productFactory.h
    code/SourceFiles/productFactory.cpp

    code/HeaderFiles/productType.h
)

target_include_directories(inventory_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/code/HeaderFiles
)

# ============================================
# TEST EXECUTABLE - ALL TESTS
# ============================================
add_executable(inventory_tests
    tests/test_food.cpp
    tests/test_inventory.cpp
)

target_link_libraries(inventory_tests PRIVATE
    inventory_core
    GTest::gtest_main
    GTest::gmock_main
)

# Discover tests for CTest
include(GoogleTest)
gtest_discover_tests(inventory_tests)
