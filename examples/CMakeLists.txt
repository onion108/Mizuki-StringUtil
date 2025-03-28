
function(build_exmpl target)
    add_executable(${target} ${target}.c)
    target_link_libraries(${target} mizukistr)
endfunction(build_exmpl)

build_exmpl(basic)
build_exmpl(view)
build_exmpl(find_and_replace)
build_exmpl(insert_and_delete)
