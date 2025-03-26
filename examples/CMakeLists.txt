
add_executable(basic basic.c)
target_link_libraries(basic mizukistr)

add_executable(view view.c)
target_link_libraries(view mizukistr)

add_executable(find_and_replace find_and_replace.c)
target_link_libraries(find_and_replace mizukistr)

