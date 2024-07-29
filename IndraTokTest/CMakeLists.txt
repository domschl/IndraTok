add_executable (indraTokTest indra_tok_test.c)

target_link_libraries (indraTokTest LINK_PUBLIC IndraTok m)

add_test(NAME IndraTokTest
    COMMAND indraTokTest
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
