project(memlab)

set(CMAKE_CXX_FLAGS "-std=c++14 -I.")
set(SRC "main.cpp"
        "process_memory.cpp"
   )

set(TST_SRC "test.cpp")

add_executable(memlab ${SRC})
add_executable(test ${TST_SRC})
