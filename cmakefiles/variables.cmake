if(MSVC)
  set(TULIPA_RUNTIME_OUTPUT_DIRECTORY "${TULIPA_HOME_DIR}/out/bin/msvc${MSVC_VERSION}/${CMAKE_VS_PLATFORM_NAME}")
  set(TULIPA_LIBRARY_OUTPUT_DIRECTORY "${TULIPA_HOME_DIR}/out/lib/msvc${MSVC_VERSION}/${CMAKE_VS_PLATFORM_NAME}")
endif(MSVC)
