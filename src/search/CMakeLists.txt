cmake_minimum_required(VERSION 2.8)

add_library(search STATIC
    eval/Evaluator.cpp
    tree/Node.cpp
    tree/Tree.cpp
    RandomSearcher.cpp
    Searcher.cpp
)
