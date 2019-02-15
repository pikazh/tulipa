macro(TULIPA_BUILD_WITH_UNICODE)
  ADD_DEFINITIONS(
    -DUNICODE
    -D_UNICODE
  )
endmacro(TULIPA_BUILD_WITH_UNICODE)

macro(TULIPA_CONFIG_OUTPUT_PATHS TARGETNAME)
  set_target_properties(${TARGETNAME}
    PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY "${TULIPA_RUNTIME_OUTPUT_DIRECTORY}"
      ARCHIVE_OUTPUT_DIRECTORY "${TULIPA_LIBRARY_OUTPUT_DIRECTORY}"
      LIBRARY_OUTPUT_DIRECTORY "${TULIPA_RUNTIME_OUTPUT_DIRECTORY}"
      DEBUG_POSTFIX	"_d"
  )
endmacro(TULIPA_CONFIG_OUTPUT_PATHS)