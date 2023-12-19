cmake_minimum_required(VERSION 3.18)
project(strbench C)

set(CMAKE_C_STANDARD 17)

add_executable(strbench main.c
        const.h
        ll.h
        ll.c
        str.h
        str.c
	arena.h
	list.h)
