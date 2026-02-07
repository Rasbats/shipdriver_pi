Update plugin API
=================

Over time the API has evolved and the plugin needs to adapted to this.
These changes are typically too complex to be handled by update-templates
This document is basically an API checklist describing the manual steps
required to use APIs beyond 1.16.


Fix plugin information
----------------------

Changes in main OpenCPN 5.12 makes it important to provide correct information
in the plugin API. In this context, "correct" means using the info defined
in Plugin.cmake and hence in the plugin catalog.

In order to achieve this, first add these two lines to the top of
_config.h.in_:

    #ifndef CONFIG_H__
    #define CONFIG_H__

Before the end, add these

    #define PLUGIN_API_NAME "@PLUGIN_API_NAME@"
    #define PKG_SUMMARY _("@PKG_SUMMARY@")
    static const char* const PKG_DESCRIPTION = _(R"""(@PKG_DESCRIPTION@)""");

At the very end, add

    #endif    // CONFIG_H__

In the next step redefine the API functions `GetCommonName()`,
`GetShortDescription()` and `GetLongDescription()`. In the
shipdriver case these lives in _src/Shipdriver_pi.cpp_ and becomes:

    wxString ShipDriver_pi::GetCommonName() { return PLUGIN_API_NAME; }
    wxString ShipDriver_pi::GetShortDescription() { return PKG_SUMMARY; }
    wxString ShipDriver_pi::GetLongDescription() { return PKG_DESCRIPTION; }

This is boilerplate code common for all plugins.


Shipdriver update to plugin API 118
-----------------------------------

Since version 3.3.0 the shipdriver template supports using API 118. Compared
to the commonly used API 116 this offers some new capabilities:

  - Better version handling, plugin carries the complete version info. This
    avoids the needs for various hacks in main openpcn used for older API
    levels.

  - The new communication interface described in
    https://opencpn-manuals.github.io/main/opencpn-dev/plugin-messaging.html.
    This can handle NMEA2000 messages and also offers more performance
    and flexibility compared to the old interfaces.

  - Improved scalable interface support

  - Enhanced waypoint and route support.

The update process:
-------------------

1. Ensure that the template is updated to 3.3.0 or higher

2. In Plugin.cmake, change `set(PKG_API_LIB api-16)` to `set(PKG_API_LIB api-18)`

3. Add the following snippet to config.h.in:
   ```
   #define PLUGIN_VERSION_TWEAK @PROJECT_VERSION_TWEAK@

   #define PKG_PRERELEASE "@PKG_PRERELEASE@"
   #define PKG_BUILD_INFO "@pkg_vers_build_info@"

   #define PLUGIN
   ```

4. In the main header file for the plugin (Shipdriver_pi.h for shipdriver)
   find the snippet like
   ```
   class ShipDriver_pi : public opencpn_plugin_116 {
   ```
   Here, update to use `public opencpn_plugin_118` instead.

5. In the file which implements the constructor, find the snippet like
   ```
   ShipDriver_pi::ShipDriver_pi(void* ppimgr) : opencpn_plugin_116(ppimgr)
   ```
   Likewise, update this to `opencpn_plugin_118(ppimgr)`.  For shipdriver,
   this is in src/ShipDriver_pi.cpp.

6. Implement new methods, typically in the file which implements
   `GetAPIVersionMajor()`. In the shipdriver case, this is src/ShipDriver_pi.cpp
   ```
   int GetPlugInVersionPatch() { return PLUGIN_VERSION_PATCH; }
   int GetPlugInVersionPost() { return PLUGIN_VERSION_TWEAK; }
   const char *GetPlugInVersionPre() { return PKG_PRERELEASE; }
   const char *GetPlugInVersionBuild() { return PKG_BUILD_INFO; }
   ```
   This is boilerplate code, same for all plugins.

7. Build and test plugin.
