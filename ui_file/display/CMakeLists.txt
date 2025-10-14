##
# @file CMakeLists.txt
# @brief 
#/
set(APP_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR})

set(APP_MODULE_SRCS)
list(APPEND APP_MODULE_SRCS
        ${APP_MODULE_PATH}/app_display.c
    )

aux_source_directory(${APP_MODULE_PATH}/ducky DUCKY_SRCS)
aux_source_directory(${APP_MODULE_PATH}/icons ICONS_SRCS)
aux_source_directory(${APP_MODULE_PATH}/logo LOGO_SRCS)
aux_source_directory(${APP_MODULE_PATH}/ui UI_SRCS)


list(APPEND APP_MODULE_SRCS
    ${DUCKY_SRCS}
    ${ICONS_SRCS}
    ${LOGO_SRCS}
    ${UI_SRCS}
)

# 将源文件添加到UI_SOURCES变量，供父目录使用
list(APPEND UI_SOURCES ${APP_MODULE_SRCS})
message(STATUS "Collected UI sources: ${UI_SOURCES}")
set(UI_SOURCES ${UI_SOURCES} PARENT_SCOPE)