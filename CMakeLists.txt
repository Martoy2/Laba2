cmake_minimum_required(VERSION 4.0)
project(2C_Laba2)

set(CMAKE_CXX_STANDARD 20)

add_executable(laba2 main.cpp
        structures/container.h
        structures/serialization.h
        structures/serialization.cpp
        structures/array.h
        structures/array.cpp
        structures/node.h
        structures/forward_list.h
        structures/forward_list.cpp
        structures/doubly_list.h
        structures/doubly_list.cpp
        structures/stack.h
        structures/stack.cpp
        structures/queue.h
        structures/queue.cpp
        structures/avl_tree.h
        structures/avl_tree.cpp
        structures/hash_set.h
        structures/hash_set.cpp
        structures/hash_table.h
        structures/utf8.h
        structures/utf8.cpp
        engine/storage.h
        engine/storage.cpp
        engine/query.h
        engine/query.cpp
        engine/executor.h
        engine/executor.cpp
        first/first.h
        first/first.cpp
        first/xml.h
        first/xml.cpp
        second/second.h
        second/second.cpp
        third/third.h
        third/third.cpp
        fourth/fourth.h
        fourth/fourth.cpp
        fifth/fifth.h
        fifth/fifth.cpp
        fifth/huffman.h
        fifth/huffman.cpp
        sixth/sixth.h
        sixth/sixth.cpp
        seventh/seventh.h
        seventh/seventh.cpp
        seventh/lru_cache.h
        seventh/lru_cache.cpp)

set_target_properties(laba2 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
