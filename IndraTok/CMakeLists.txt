add_library (IndraTok SHARED indra_atom.c indra_tok.c indra_crc_crypt.c)

target_include_directories (IndraTok PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
