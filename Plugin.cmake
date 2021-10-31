# ~~~
# Summary:      Local, non-generic plugin setup
# Copyright (c) 2020-2021 Mike Rossiter
# License:      GPLv3+
# ~~~

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.


# -------- Options ----------

set(OCPN_TEST_REPO
    "opencpn/shipdriver-alpha"
    CACHE STRING "Default repository for untagged builds"
)
set(OCPN_BETA_REPO
    "opencpn/shipdriver-beta"
    CACHE STRING
    "Default repository for tagged builds matching 'beta'"
)
set(OCPN_RELEASE_REPO
    "opencpn/shipdriver-prod"
    CACHE STRING
    "Default repository for tagged builds not matching 'beta'"
)

option(SHIPDRIVER_USE_SVG "Use SVG graphics" ON)

#
#
# -------  Plugin setup --------
#
set(PKG_VERSION  2.8.7.2)
set(PKG_PRERELEASE "")  # Empty, or a tag like 'beta'

set(DISPLAY_NAME ShipDriver)    # Dialogs, installer artifacts, ...
set(PLUGIN_API_NAME ShipDriver) # As of GetCommonName() in plugin API
set(CPACK_PACKAGE_CONTACT "Mike Rossiter")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Simulate ship movements")
set(CPACK_PACKAGE_DESCRIPTION [=[
Simulates navigation of a vessel. Using the sail option and a current
grib file for wind data, simulates how a sailing vessel might react in
those conditions. Using 'Preferences' the simulator is able to record AIS
data from itself. This can be replayed to simulate collision situations.
]=])

set(PKG_AUTHOR "Mike Rossiter")
set(PKG_IS_OPEN_SOURCE "yes")
set(CPACK_PACKAGE_HOMEPAGE_URL https://github.com/Rasbats/shipdriver_pi)
set(PKG_INFO_URL https://opencpn.org/OpenCPN/plugins/shipdriver.html)

set(SRC
    src/ShipDriver_pi.h
    src/ShipDriver_pi.cpp
    src/icons.h
    src/icons.cpp
    src/ShipDrivergui.h
    src/ShipDrivergui.cpp
    src/ShipDrivergui_impl.cpp
    src/ShipDrivergui_impl.h
    src/AisMaker.h
    src/AisMaker.cpp
    src/GribRecord.cpp
    src/GribRecordSet.h
    src/GribRecord.h
)

macro(late_init)
  # Perform initialization after the PACKAGE_NAME library and ocpn::api
  # is available.
  if (SHIPDRIVER_USE_SVG)
    target_compile_definitions(${PACKAGE_NAME} PUBLIC SHIPDRIVER_USE_SVG)
  endif ()
endmacro ()

macro(add_plugin_libraries)
  # Add libraries required by this plugin
  add_subdirectory("libs/tinyxml")
  target_link_libraries(${PACKAGE_NAME} ocpn::tinyxml)

  add_subdirectory("libs/wxJSON")
  target_link_libraries(${PACKAGE_NAME} ocpn::wxjson)

  add_subdirectory("libs/plugingl")
  target_link_libraries(${PACKAGE_NAME} ocpn::plugingl)
endmacro ()
