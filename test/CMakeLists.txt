set(ZEUS_TEST_FILES
        tests/BlockChunk.cpp
        tests/NetHandler.cpp
        tests/Serializer.cpp
        tests/LibNoise.cpp
        tests/Dimension.cpp
        tests/List.cpp)

add_library (zeusTest ${ZEUS_TEST_FILES})