cmake_minimum_required(VERSION 3.10)
project(PasswordManager)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenSSL REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(PasswordManager
    src/main.cpp
    src/MainWindow.cpp
    src/AuthDialog.cpp
    src/PasswordDatabase.cpp
    src/EncryptionUtil.cpp
    src/PasswordGenerator.cpp
)

target_link_libraries(PasswordManager Qt5::Widgets OpenSSL::SSL OpenSSL::Crypto)
