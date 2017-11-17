/**********************************************************\

  Auto-generated EasyPlayerPluginAPI.cpp

\**********************************************************/

#include "JSObject.h"
#include "variant_list.h"
#include "DOM/Document.h"
#include "global/config.h"

#include "EasyPlayerPluginAPI.h"
#include <list>
#include <boost/assign/list_of.hpp>

///////////////////////////////////////////////////////////////////////////////
/// @fn FB::variant EasyPlayerPluginAPI::echo(const FB::variant& msg)
///
/// @brief  Echos whatever is passed from Javascript.
///         Go ahead and change it. See what happens!
///////////////////////////////////////////////////////////////////////////////
FB::variant EasyPlayerPluginAPI::echo(const FB::variant& msg)
{
    static int n(0);
    fire_echo("So far, you clicked this many times: ", n++);

    // return "foobar";
    return msg;
}

///////////////////////////////////////////////////////////////////////////////
/// @fn EasyPlayerPluginPtr EasyPlayerPluginAPI::getPlugin()
///
/// @brief  Gets a reference to the plugin that was passed in when the object
///         was created.  If the plugin has already been released then this
///         will throw a FB::script_error that will be translated into a
///         javascript exception in the page.
///////////////////////////////////////////////////////////////////////////////
EasyPlayerPluginPtr EasyPlayerPluginAPI::getPlugin()
{
    EasyPlayerPluginPtr plugin(m_plugin.lock());
    if (!plugin) {
        throw FB::script_error("The plugin is invalid");
    }
    return plugin;
}

// Read/Write property testString
std::string EasyPlayerPluginAPI::get_testString()
{
    return m_testString;
}

void EasyPlayerPluginAPI::set_testString(const std::string& val)
{
    m_testString = val;
}

// Read-only property version
std::string EasyPlayerPluginAPI::get_version()
{
    return FBSTRING_PLUGIN_VERSION;
}

void EasyPlayerPluginAPI::testEvent()
{
    fire_test();
}

int EasyPlayerPluginAPI::Start( std::string& sURL, int nRenderFormat,  std::string& sUserName,  std::string& sPassword, int nHardDecode)
{
	if (m_player)
	{
		int nRenderType = nRenderFormat;
		RENDER_FORMAT eRenderFormat = DISPLAY_FORMAT_YV12;
		switch (nRenderType)
		{
		case 0:
			eRenderFormat = DISPLAY_FORMAT_YV12;
			break;
		case 1:
			eRenderFormat = DISPLAY_FORMAT_YUY2;
			break;
		case 2:
			eRenderFormat = DISPLAY_FORMAT_UYVY;
			break;
		case 3:
			eRenderFormat = DISPLAY_FORMAT_A8R8G8B8;
			break;
		case 4:
			eRenderFormat = DISPLAY_FORMAT_X8R8G8B8;
			break;
		case 5:
			eRenderFormat = DISPLAY_FORMAT_RGB565;
			break;
		case 6:
			eRenderFormat = DISPLAY_FORMAT_RGB555;
			break;
		case 7:
			eRenderFormat = DISPLAY_FORMAT_RGB24_GDI;
			break;
		}
		EasyPlayerWnd *pwnd = getPlugin()->GetWindow()->get_as<EasyPlayerWnd>();  
		HWND hwnd = pwnd->getHWND();  

		m_player->Start((char*)sURL.c_str(), hwnd, eRenderFormat, 1, sUserName.c_str() , sPassword.c_str(), nHardDecode);
	}
	return 1;
}
void EasyPlayerPluginAPI::Config(int nFrameCache, int nPlaySound, int nShowToScale, int nShowStatisticInfo)
{
	if (m_player)
	{
		m_player->Config(nFrameCache, nPlaySound, nShowToScale, nShowStatisticInfo );
	}
}
void EasyPlayerPluginAPI::Close(void)
{
	if (m_player)
	{
		m_player->Close();
	}
}