find_package(Qt6 ${ACT_MINIMUM_QT_VERSION} COMPONENTS Widgets Test REQUIRED)

add_executable(screencapturetest screencapturetest.cpp)
target_link_directories(screencapturetest PRIVATE ${CMAKE_BINARY_DIR}/3rdparty/qtjsapi/out)
target_link_libraries(screencapturetest PRIVATE actiontools Qt6::Widgets Qt6::Test)
add_test(NAME screencapture COMMAND screencapturetest)
