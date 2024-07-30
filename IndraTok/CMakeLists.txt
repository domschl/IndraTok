add_library (IndraTok SHARED indra_tok.c crc_crypt_hash.c)

target_include_directories (IndraTok PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
