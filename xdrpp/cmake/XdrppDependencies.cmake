#
# List of XDRPP dependncies
#
set(XDRPP_DEPENDENCIES Threads)

set(XDRPP_ALL_DEPENDENCIES
  ${XDRPP_DEPENDENCIES}
  ${XDRPP_BACKENDS_DB_DEPENDENCIES})

#
# Perform checks
#
colormsg(_HIBLUE_ "Looking for XDRPP dependencies:")

macro(boost_external_report NAME)

  set(VARNAME ${NAME})
  string(TOUPPER ${NAME} VARNAMEU)

  set(VARNAMES ${ARGV})
  list(REMOVE_AT VARNAMES 0)

  # Test both, given original name and uppercase version too
  if(NOT ${VARNAME}_FOUND AND NOT ${VARNAMEU}_FOUND)
    colormsg(_RED_ "WARNING: ${NAME} libraries not found, some features will be disabled.")
  endif()

  foreach(variable ${VARNAMES})
    if(${VARNAMEU}_FOUND)
      boost_report_value(${VARNAMEU}_${variable})
    elseif(${VARNAME}_FOUND)
      boost_report_value(${VARNAME}_${variable})
    endif()
  endforeach()
endmacro()

#
#  Some externals default to OFF
#
option(WITH_VALGRIND "Run tests under valgrind" OFF)


