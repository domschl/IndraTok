add_executable (indraAtomTest indra_atom_test.c)

target_link_libraries (indraAtomTest LINK_PUBLIC IndraTok m)

add_test(NAME IndraAtomTest
    COMMAND indraAtomTest
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
