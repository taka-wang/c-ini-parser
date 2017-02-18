# ==== Collect all source files ====
# use aux_source_directory instead of set
#SET(LIBINIPARSER_SRC iniparser.c internal/ini.c)
aux_source_directory(. PARSER_SRCS)
aux_source_directory(internal INTERNAL_SRCS)


# === Conditional build ============
# cmake .. -DENABLE_SHAREDLIB=TRUE

option (ENABLE_SHAREDLIB "Shared or static flag" ON)

if (ENABLE_SHAREDLIB)
    add_library(iniparser SHARED ${PARSER_SRCS} ${INTERNAL_SRCS})
else()
    add_library(iniparser STATIC ${PARSER_SRCS} ${INTERNAL_SRCS})
endif()

# target location
install(TARGETS iniparser DESTINATION lib)
install(FILES iniparser.h DESTINATION include)