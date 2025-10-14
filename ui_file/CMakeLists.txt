# 收集当前目录下的源文件
aux_source_directory(${CMAKE_CURRENT_LIST_DIR} UI_ROOT_SRCS)

# 将根目录源文件添加到UI_SOURCES变量
list(APPEND UI_SOURCES ${UI_ROOT_SRCS})

# 添加display子目录来收集更多源文件
add_subdirectory(display)

# 将收集到的所有源文件传递给父目录
set(UI_SOURCES ${UI_SOURCES} PARENT_SCOPE)