TULIPA_BUILD_WITH_UNICODE()

set(CURRENT_PROJ common)

INCLUDE_DIRECTORIES(
)

LINK_DIRECTORIES(
)

set(HEADER_FILES 
  ${TULIPA_HOME_DIR}/common/comdef.h
  ${TULIPA_HOME_DIR}/common/easy_app.h
  ${TULIPA_HOME_DIR}/common/gui.h
  ${TULIPA_HOME_DIR}/common/resource.h
  ${TULIPA_HOME_DIR}/common/Signal.h
  ${TULIPA_HOME_DIR}/common/threadpolicy.h
  ${TULIPA_HOME_DIR}/common/window.h  
)

set(SOURCE_FILES 
  ${TULIPA_HOME_DIR}/common/easy_app.cpp
  ${TULIPA_HOME_DIR}/common/threadpolicy.cpp
)

set(RESOURCE_FILES
)

if(MSVC)
set(HEADER_FILES 
  ${HEADER_FILES}
  ${TULIPA_HOME_DIR}/common/msgui.h
  ${TULIPA_HOME_DIR}/common/mswindow.h
  )

set(SOURCE_FILES 
  ${SOURCE_FILES}
  ${TULIPA_HOME_DIR}/common/msgui.cpp
  ${TULIPA_HOME_DIR}/common/mswindow.cpp
  )
  
set(RESOURCE_FILES 
  ${RESOURCE_FILES}
  ${TULIPA_HOME_DIR}/common/icon.ico
  ${TULIPA_HOME_DIR}/common/appresource.rc
)
endif(MSVC)


SOURCE_GROUP("Header Files" FILES ${HEADER_FILES})
SOURCE_GROUP("Source Files" FILES ${SOURCE_FILES})
SOURCE_GROUP("Resource Files" FILES ${RESOURCE_FILES})

ADD_LIBRARY(${CURRENT_PROJ} STATIC ${HEADER_FILES} ${SOURCE_FILES} ${RESOURCE_FILES})

TULIPA_CONFIG_OUTPUT_PATHS(${CURRENT_PROJ})