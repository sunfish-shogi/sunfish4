cmake_minimum_required(VERSION 2.8)

add_library(book STATIC
    Book.cpp
    Book.hpp
    BookGenerator.cpp
    BookGenerator.hpp
    BookUtil.hpp
)
