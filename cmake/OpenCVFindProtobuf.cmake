# If protobuf is found - libprotobuf target is available

set(HAVE_PROTOBUF FALSE)

if(NOT WITH_PROTOBUF)
  return()
endif()

ocv_option(BUILD_PROTOBUF "Force to build libprotobuf runtime from sources" ON)
ocv_option(PROTOBUF_UPDATE_FILES "Force rebuilding .proto files (protoc should be available)" OFF)

# BUILD_PROTOBUF=OFF: Custom manual protobuf configuration (see find_package(Protobuf) for details):
# - Protobuf_INCLUDE_DIR
# - Protobuf_LIBRARY
# - Protobuf_PROTOC_EXECUTABLE


function(get_protobuf_version version include)
  file(STRINGS "${include}/google/protobuf/stubs/common.h" ver REGEX "#define GOOGLE_PROTOBUF_VERSION [0-9]+")
  string(REGEX MATCHALL "[0-9]+" ver ${ver})
  math(EXPR major "${ver} / 1000000")
  math(EXPR minor "${ver} / 1000 % 1000")
  math(EXPR patch "${ver} % 1000")
  set(${version} "${major}.${minor}.${patch}" PARENT_SCOPE)
endfunction()

if(BUILD_PROTOBUF)
  ocv_assert(NOT PROTOBUF_UPDATE_FILES)
  add_subdirectory("${OpenCV_SOURCE_DIR}/3rdparty/protobuf")
  set(Protobuf_LIBRARIES "libprotobuf")
  set(HAVE_PROTOBUF TRUE)
else()
  unset(Protobuf_VERSION CACHE)
  find_package(Protobuf QUIET)

  # Backwards compatibility
  # Define camel case versions of input variables
  foreach(UPPER
      PROTOBUF_FOUND
      PROTOBUF_LIBRARY
      PROTOBUF_INCLUDE_DIR
      PROTOBUF_VERSION
      )
      if (DEFINED ${UPPER})
          string(REPLACE "PROTOBUF_" "Protobuf_" Camel ${UPPER})
          if (NOT DEFINED ${Camel})
              set(${Camel} ${${UPPER}})
          endif()
      endif()
  endforeach()
  # end of compatibility block

  if(Protobuf_FOUND)
    if(TARGET protobuf::libprotobuf)
      set(Protobuf_LIBRARIES "protobuf::libprotobuf")
    else()
      add_library(libprotobuf UNKNOWN IMPORTED)
      set_target_properties(libprotobuf PROPERTIES
        IMPORTED_LOCATION "${Protobuf_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${Protobuf_INCLUDE_DIR}"
        INTERFACE_SYSTEM_INCLUDE_DIRECTORIES "${Protobuf_INCLUDE_DIR}"
      )
      get_protobuf_version(Protobuf_VERSION "${Protobuf_INCLUDE_DIR}")
      set(Protobuf_LIBRARIES "libprotobuf")
    endif()
    set(HAVE_PROTOBUF TRUE)
  endif()
endif()

# See https://github.com/opencv/opencv/issues/24369
# In Protocol Buffers v22.0 and later drops C++11 support and depends abseil-cpp.
#   Details: https://protobuf.dev/news/2022-08-03/
# And if std::text_view is in abseil-cpp requests C++17 and later.

if(HAVE_PROTOBUF)
    if("${Protobuf_VERSION}" MATCHES [[[0-9]+.([0-9]+).[0-9]+]])
        string(COMPARE GREATER "${CMAKE_MATCH_1}" "21" REQUEST_ABSL)  # >=22

        if(REQUEST_ABSL)
            string(COMPARE GREATER "${CMAKE_CXX_STANDARD}" "16" USED_AFTER_CXX17)  # >=17

            if(NOT USED_AFTER_CXX17)
                message("CMAKE_CXX_STANDARD : ${CMAKE_CXX_STANDARD}")
                message("protobuf           : ${Protobuf_VERSION}")
                message(FATAL_ERROR "protobuf(v22 and later) and abseil-cpp request CMAKE_CXX_STANDARD=17 and later.")
            endif()

            ocv_check_modules(ABSL_STRINGS absl_strings)
            if(NOT ABSL_STRINGS_FOUND)
                message(FATAL_ERROR "protobuf(v22 and later) requests abseil-cpp(strings), but missing.")
            endif()

            ocv_check_modules(ABSL_LOG absl_log)
            if(NOT ABSL_LOG_FOUND)
                message(FATAL_ERROR "protobuf(v22 and later) requests abseil-cpp(log), but missing.")
            endif()

        endif()
    else()
        message(FATAL_ERROR "Protobuf version(${Protobuf_VERSION}) is unexpected to split.")
    endif()
endif()

if(HAVE_PROTOBUF AND PROTOBUF_UPDATE_FILES AND NOT COMMAND PROTOBUF_GENERATE_CPP)
  message(FATAL_ERROR "Can't configure protobuf dependency (BUILD_PROTOBUF=${BUILD_PROTOBUF} PROTOBUF_UPDATE_FILES=${PROTOBUF_UPDATE_FILES})")
endif()

if(HAVE_PROTOBUF)
  list(APPEND CUSTOM_STATUS protobuf)
  if(NOT BUILD_PROTOBUF)
    if(TARGET "${Protobuf_LIBRARIES}")
      get_target_property(__location "${Protobuf_LIBRARIES}" IMPORTED_LOCATION_RELEASE)
      if(NOT __location)
        get_target_property(__location "${Protobuf_LIBRARIES}" IMPORTED_LOCATION)
      endif()
    elseif(Protobuf_LIBRARY)
      set(__location "${Protobuf_LIBRARY}")
    else()
      set(__location "${Protobuf_LIBRARIES}")
    endif()
  endif()
  list(APPEND CUSTOM_STATUS_protobuf "    Protobuf:"
    BUILD_PROTOBUF THEN "build (${Protobuf_VERSION})"
    ELSE "${__location} (${Protobuf_VERSION})")
endif()

if(HAVE_ABSL_STRINGS AND HAVE_ABSL_LOG)
  list(APPEND CUSTOM_STATUS absl)
  list(APPEND CUSTOM_STATUS_absl "      abseil-cpp:" "YES (${ABSL_STRINGS_VERSION})" )
endif()
