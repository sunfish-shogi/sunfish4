cmake_minimum_required(VERSION 2.8)

add_library(search STATIC
    eval/Evaluator.cpp
    eval/Material.cpp
    see/SEE.cpp
    tree/Node.cpp
    tree/Tree.cpp
    tree/Worker.cpp
    tt/TTElement.cpp
    tt/TTSlots.cpp
    RandomSearcher.cpp
    Searcher.cpp
    SearchInfo.cpp
)
