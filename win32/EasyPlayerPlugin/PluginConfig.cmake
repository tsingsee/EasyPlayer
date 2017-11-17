#/**********************************************************\ 
#
# Auto-Generated Plugin Configuration file
# for EasyPlayer Plugin
#
#\**********************************************************/

set(PLUGIN_NAME "EasyPlayerPlugin")
set(PLUGIN_PREFIX "EPP")
set(COMPANY_NAME "EasyDarwin")

# ActiveX constants:
set(FBTYPELIB_NAME EasyPlayerPluginLib)
set(FBTYPELIB_DESC "EasyPlayerPlugin 1.0 Type Library")
set(IFBControl_DESC "EasyPlayerPlugin Control Interface")
set(FBControl_DESC "EasyPlayerPlugin Control Class")
set(IFBComJavascriptObject_DESC "EasyPlayerPlugin IComJavascriptObject Interface")
set(FBComJavascriptObject_DESC "EasyPlayerPlugin ComJavascriptObject Class")
set(IFBComEventSource_DESC "EasyPlayerPlugin IFBComEventSource Interface")
set(AXVERSION_NUM "1")

# NOTE: THESE GUIDS *MUST* BE UNIQUE TO YOUR PLUGIN/ACTIVEX CONTROL!  YES, ALL OF THEM!
set(FBTYPELIB_GUID 40a70b3b-6713-5aca-8489-f3a1c2ed58e9)
set(IFBControl_GUID 7e664238-331c-5837-8577-d6457f0c09e9)
set(FBControl_GUID 7251445a-71de-504c-828a-031a83fef117)
set(IFBComJavascriptObject_GUID 42dd5229-2e52-5850-b48f-dd5c0a56010d)
set(FBComJavascriptObject_GUID eac2ede4-17f2-5b95-a66f-b13979a1c4a2)
set(IFBComEventSource_GUID e9df3573-dffc-5279-acbe-3bff6c0d7b8b)
if ( FB_PLATFORM_ARCH_32 )
    set(FBControl_WixUpgradeCode_GUID d8e6e9ba-8002-5c71-a250-cadba4572cb2)
else ( FB_PLATFORM_ARCH_32 )
    set(FBControl_WixUpgradeCode_GUID 70540ee3-634c-5363-a366-79b46645fdd5)
endif ( FB_PLATFORM_ARCH_32 )

# these are the pieces that are relevant to using it from Javascript
set(ACTIVEX_PROGID "EasyDarwin.EasyPlayerPlugin")
if ( FB_PLATFORM_ARCH_32 )
    set(MOZILLA_PLUGINID "www.easydarwin.org/EasyPlayerPlugin")  # No 32bit postfix to maintain backward compatability.
else ( FB_PLATFORM_ARCH_32 )
    set(MOZILLA_PLUGINID "www.easydarwin.org/EasyPlayerPlugin_${FB_PLATFORM_ARCH_NAME}")
endif ( FB_PLATFORM_ARCH_32 )

# strings
set(FBSTRING_CompanyName "EasyDarwin")
set(FBSTRING_PluginDescription "EasyPlayer Plugin")
set(FBSTRING_PLUGIN_VERSION "1.0.0.0")
set(FBSTRING_LegalCopyright "Copyright 2017 EasyDarwin")
set(FBSTRING_PluginFileName "np${PLUGIN_NAME}")
if (APPLE)
    # On apple, np is not needed
    set(FBSTRING_PluginFileName "${PLUGIN_NAME}")
endif()
set(FBSTRING_ProductName "EasyPlayer Plugin")
set(FBSTRING_FileExtents "")
if ( FB_PLATFORM_ARCH_32 )
    set(FBSTRING_PluginName "EasyPlayer Plugin")  # No 32bit postfix to maintain backward compatability.
else ( FB_PLATFORM_ARCH_32 )
    set(FBSTRING_PluginName "EasyPlayer Plugin_${FB_PLATFORM_ARCH_NAME}")
endif ( FB_PLATFORM_ARCH_32 )
set(FBSTRING_MIMEType "application/x-easyplayerplugin")

# Uncomment this next line if you're not planning on your plugin doing
# any drawing:

#set (FB_GUI_DISABLED 1)

# Mac plugin settings. If your plugin does not draw, set these all to 0
set(FBMAC_USE_QUICKDRAW 0)
set(FBMAC_USE_CARBON 1)
set(FBMAC_USE_COCOA 1)
set(FBMAC_USE_COREGRAPHICS 1)
set(FBMAC_USE_COREANIMATION 0)
set(FBMAC_USE_INVALIDATINGCOREANIMATION 0)

# If you want to register per-machine on Windows, uncomment this line
#set (FB_ATLREG_MACHINEWIDE 1)
