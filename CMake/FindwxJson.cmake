# 

# - Try to find wxJson
# Once done this will define
#  WXJSON_FOUND - System has wxJson
#  WXJSON_INCLUDE_DIRS - The wxJson include directories
#  WXJSON_LIBRARIES - The libraries needed to use wxJson
#  WXJSON_DEFINITIONS - Compiler switches required for using wxJson

#find_package(PkgConfig)
#pkg_check_modules(PC_WXJSON QUIET )
#set(WXJSON_DEFINITIONS ${PC_WXJSON_CFLAGS_OTHER})

find_path(WXJSON_INCLUDE_DIR wx/json_defs.h
          HINTS ${wxJson_DIR}/include/wxJson
          PATH_SUFFIXES wxJson )

find_library(WXJSON_LIBRARY NAMES libwxJson.so
             HINTS ${wxJson_DIR}/lib/wxJson )

set(WXJSON_LIBRARIES ${WXJSON_LIBRARY} )
set(WXJSON_INCLUDE_DIRS ${WXJSON_INCLUDE_DIR} )

include_directories(${WXJSON_INCLUDE_DIRS})

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set WXJSON_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(wxJson  DEFAULT_MSG
                                  WXJSON_LIBRARY WXJSON_INCLUDE_DIR)
mark_as_advanced(WXJSON_INCLUDE_DIR WXJSON_LIBRARY )
