cmake_minimum_required(VERSION 2.8)

add_library(search STATIC
    eval/Evaluator.cpp
    eval/FeatureVector.cpp
    eval/Material.cpp
    mate/Mate.cpp
    see/SEE.cpp
    shek/SCRDetector.cpp
    tree/Measure.cpp
    tree/Node.cpp
    tree/Tree.cpp
    tree/Worker.cpp
    tt/TTElement.cpp
    tt/TTSlots.cpp
    RandomSearcher.cpp
    Searcher.cpp
    SearchHandler.cpp
)
