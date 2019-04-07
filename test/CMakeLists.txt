set(ZEUS_TEST_FILES
        tests/BlockChunk.cpp
        tests/NetHandler.cpp
        tests/Serializer.cpp
        tests/LibNoise.cpp
        tests/Dimension.cpp)

add_library (zeusTest ${ZEUS_TEST_FILES})