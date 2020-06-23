set(ZEPHA_TEST
        Main.cpp tests/TestSpace.cpp tests/TestBlockChunk.cpp tests/TestSerializer.cpp tests/TestBlockChunkSerialization.cpp)

add_library (Zepha_Test ${ZEPHA_TEST})