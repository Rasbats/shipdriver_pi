# Set ARCH using cmake probes and various heuristics.

if (COMMAND GetArch)
  return()
endif ()

# Based on code from nohal
function (GetArch)
  if (NOT WIN32)
    # default
    set(ARCH "x86_64")
    if (CMAKE_SYSTEM_PROCESSOR MATCHES "arm*")
      if (CMAKE_SIZEOF_VOID_P MATCHES "8")
        set(ARCH "arm64")
      else ()
        set(ARCH "armhf")
      endif ()
    else (CMAKE_SYSTEM_PROCESSOR MATCHES "arm*")
      set(ARCH ${CMAKE_SYSTEM_PROCESSOR})
    endif ()
    if (EXISTS /etc/redhat-release)
      if (ARCH STREQUAL "arm64")
        set(ARCH "aarch64")
      endif ()
    elseif (EXISTS /etc/suse-release OR EXISTS /etc/SuSE-release)
      if (ARCH STREQUAL "arm64")
        set(ARCH "aarch64")
      endif ()
    endif ()
  else (NOT WIN32)
    # Should really be i386 since we are on win32. However, it's x86_64 for now,
    # see #2027
    set(ARCH "x86_64")
  endif (NOT WIN32)
  set(ARCH
      ${ARCH}
      PARENT_SCOPE
  )
endfunction (GetArch)
