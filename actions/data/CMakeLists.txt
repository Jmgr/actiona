set(PROJECT actionpackdata)

set(SOURCES_PREFIX src)
set(SOURCES
    ${SOURCES_PREFIX}/actions/readenvironmentinstance.cpp
    ${SOURCES_PREFIX}/actions/readinifileinstance.cpp
    ${SOURCES_PREFIX}/actions/readregistryinstance.cpp
    ${SOURCES_PREFIX}/actions/readtextfileinstance.cpp
    ${SOURCES_PREFIX}/actions/sendmailinstance.cpp
    ${SOURCES_PREFIX}/actions/webdownloadinstance.cpp
    ${SOURCES_PREFIX}/actions/writeregistryinstance.cpp
    ${SOURCES_PREFIX}/actions/writetextfileinstance.cpp
    ${SOURCES_PREFIX}/actions/writeinifileinstance.cpp
    ${SOURCES_PREFIX}/code/clipboard.cpp
    ${SOURCES_PREFIX}/code/file.cpp
    ${SOURCES_PREFIX}/code/inifile.cpp
    ${SOURCES_PREFIX}/code/mailattachment.cpp
    ${SOURCES_PREFIX}/code/mail.cpp
    ${SOURCES_PREFIX}/code/mailmessage.cpp
    ${SOURCES_PREFIX}/code/registry.cpp
    ${SOURCES_PREFIX}/code/sql.cpp
    ${SOURCES_PREFIX}/code/tcp.cpp
    ${SOURCES_PREFIX}/code/tcpserver.cpp
    ${SOURCES_PREFIX}/code/udp.cpp
    ${SOURCES_PREFIX}/code/web.cpp
    ${SOURCES_PREFIX}/qxtcore/qxtsignalwaiter.cpp
    ${SOURCES_PREFIX}/qxtsmtp/qxthmac.cpp
    ${SOURCES_PREFIX}/qxtsmtp/qxtmailattachment.cpp
    ${SOURCES_PREFIX}/qxtsmtp/qxtmailmessage.cpp
    ${SOURCES_PREFIX}/qxtsmtp/qxtsmtp.cpp
)

set(HEADERS_PREFIX src)
set(HEADERS
    ${HEADERS_PREFIX}/actionpackdata.hpp
    ${HEADERS_PREFIX}/data_global.hpp
    ${HEADERS_PREFIX}/actions/copyfiledefinition.hpp
    ${HEADERS_PREFIX}/actions/copyfileinstance.hpp
    ${HEADERS_PREFIX}/actions/readbinaryfiledefinition.hpp
    ${HEADERS_PREFIX}/actions/readbinaryfileinstance.hpp
    ${HEADERS_PREFIX}/actions/readclipboarddefinition.hpp
    ${HEADERS_PREFIX}/actions/readclipboardinstance.hpp
    ${HEADERS_PREFIX}/actions/readenvironmentdefinition.hpp
    ${HEADERS_PREFIX}/actions/readenvironmentinstance.hpp
    ${HEADERS_PREFIX}/actions/readinifiledefinition.hpp
    ${HEADERS_PREFIX}/actions/readinifileinstance.hpp
    ${HEADERS_PREFIX}/actions/readregistrydefinition.hpp
    ${HEADERS_PREFIX}/actions/readregistryinstance.hpp
    ${HEADERS_PREFIX}/actions/readtextfiledefinition.hpp
    ${HEADERS_PREFIX}/actions/readtextfileinstance.hpp
    ${HEADERS_PREFIX}/actions/sendmaildefinition.hpp
    ${HEADERS_PREFIX}/actions/sendmailinstance.hpp
    ${HEADERS_PREFIX}/actions/webdownloaddefinition.hpp
    ${HEADERS_PREFIX}/actions/webdownloadinstance.hpp
    ${HEADERS_PREFIX}/actions/writebinaryfiledefinition.hpp
    ${HEADERS_PREFIX}/actions/writebinaryfileinstance.hpp
    ${HEADERS_PREFIX}/actions/writeclipboarddefinition.hpp
    ${HEADERS_PREFIX}/actions/writeclipboardinstance.hpp
    ${HEADERS_PREFIX}/actions/writeinifiledefinition.hpp
    ${HEADERS_PREFIX}/actions/writeinifileinstance.hpp
    ${HEADERS_PREFIX}/actions/writeregistrydefinition.hpp
    ${HEADERS_PREFIX}/actions/writeregistryinstance.hpp
    ${HEADERS_PREFIX}/actions/writetextfiledefinition.hpp
    ${HEADERS_PREFIX}/actions/writetextfileinstance.hpp
    ${HEADERS_PREFIX}/code/clipboard.hpp
    ${HEADERS_PREFIX}/code/file.hpp
    ${HEADERS_PREFIX}/code/inifile.hpp
    ${HEADERS_PREFIX}/code/mailattachment.hpp
    ${HEADERS_PREFIX}/code/mail.hpp
    ${HEADERS_PREFIX}/code/mailmessage.hpp
    ${HEADERS_PREFIX}/code/registry.hpp
    ${HEADERS_PREFIX}/code/sql.hpp
    ${HEADERS_PREFIX}/code/tcp.hpp
    ${HEADERS_PREFIX}/code/tcpserver.hpp
    ${HEADERS_PREFIX}/code/udp.hpp
    ${HEADERS_PREFIX}/code/web.hpp
    ${HEADERS_PREFIX}/qxtcore/qxtsignalwaiter.h
    ${HEADERS_PREFIX}/qxtsmtp/qxthmac.h
    ${HEADERS_PREFIX}/qxtsmtp/qxtmailattachment.h
    ${HEADERS_PREFIX}/qxtsmtp/qxtmailmessage.h
    ${HEADERS_PREFIX}/qxtsmtp/qxtmail_p.h
    ${HEADERS_PREFIX}/qxtsmtp/qxtsmtp.h
    ${HEADERS_PREFIX}/qxtsmtp/qxtsmtp_p.h
)

set(TS_PREFIX translations)
set(TS_FILES
    ${TS_PREFIX}/actionpackdata_fr_FR.ts
    ${TS_PREFIX}/actionpackdata_zh_CN.ts
)

set(QRC ${PROJECT}.qrc)

add_library(${PROJECT} SHARED
    ${SOURCES}
    ${HEADERS}
    ${TS_FILES}
    ${QRC}
)

add_dependencies(${PROJECT} tools actiontools)

setup_target(${PROJECT})

target_compile_definitions(${PROJECT} PRIVATE ACTIONPACKDATA_LIBRARY)

find_package(Qt6 ${ACT_MINIMUM_QT_VERSION} COMPONENTS Qml Network Sql Widgets Core5Compat REQUIRED)

target_link_directories(${PROJECT}
    PRIVATE
        ${CMAKE_BINARY_DIR}/3rdparty/qtjsapi/out
)

target_include_directories(${PROJECT}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/src/3rdparty/mINI/src
        ${CMAKE_CURRENT_SOURCE_DIR}/../../tools/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../../actiontools/include
)

target_link_libraries(${PROJECT} actiontools Qt6::Network Qt6::Sql Qt6::Widgets Qt6::Core5Compat)

compile_translations(${PROJECT})

install(TARGETS ${PROJECT} DESTINATION ${ACT_ACTIONS_INSTALL_DIR})
