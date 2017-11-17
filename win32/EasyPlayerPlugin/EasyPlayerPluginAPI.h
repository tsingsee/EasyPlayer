/**********************************************************\

  Auto-generated EasyPlayerPluginAPI.h

\**********************************************************/

#include <string>
#include <sstream>
#include <boost/weak_ptr.hpp>
#include "JSAPIAuto.h"
#include "BrowserHost.h"
#include "EasyPlayerPlugin.h"

#ifndef H_EasyPlayerPluginAPI
#define H_EasyPlayerPluginAPI

#include "EasyPlayerManager.h"

#include "EasyPlayerWnd.h"

class EasyPlayerPluginAPI : public FB::JSAPIAuto
{
public:
    ////////////////////////////////////////////////////////////////////////////
    /// @fn EasyPlayerPluginAPI::EasyPlayerPluginAPI(const EasyPlayerPluginPtr& plugin, const FB::BrowserHostPtr host)
    ///
    /// @brief  Constructor for your JSAPI object.
    ///         You should register your methods, properties, and events
    ///         that should be accessible to Javascript from here.
    ///
    /// @see FB::JSAPIAuto::registerMethod
    /// @see FB::JSAPIAuto::registerProperty
    /// @see FB::JSAPIAuto::registerEvent
    ////////////////////////////////////////////////////////////////////////////
    EasyPlayerPluginAPI(const EasyPlayerPluginPtr& plugin, const FB::BrowserHostPtr& host) :
        m_plugin(plugin), m_host(host)
    {
        registerMethod("echo",      make_method(this, &EasyPlayerPluginAPI::echo));
        registerMethod("testEvent", make_method(this, &EasyPlayerPluginAPI::testEvent));
		registerMethod("Start",      make_method(this, &EasyPlayerPluginAPI::Start));
		registerMethod("Config",      make_method(this, &EasyPlayerPluginAPI::Config));
		registerMethod("Close",      make_method(this, &EasyPlayerPluginAPI::Close));
  
        // Read-write property
        registerProperty("testString",
                         make_property(this,
                                       &EasyPlayerPluginAPI::get_testString,
                                       &EasyPlayerPluginAPI::set_testString));
        
        // Read-only property
        registerProperty("version",
                         make_property(this,
                                       &EasyPlayerPluginAPI::get_version));

		//创建播放器实例
		try 
		{
			m_player  = EasyPlayerPtr(new EasyPlayerManager);
			m_valid   = true;
		} 
		catch(...) 
		{
			m_host->htmlLog("failed to initialize easy rtsp player");
		}
    }

    ///////////////////////////////////////////////////////////////////////////////
    /// @fn EasyPlayerPluginAPI::~EasyPlayerPluginAPI()
    ///
    /// @brief  Destructor.  Remember that this object will not be released until
    ///         the browser is done with it; this will almost definitely be after
    ///         the plugin is released.
    ///////////////////////////////////////////////////////////////////////////////
    virtual ~EasyPlayerPluginAPI() {};

    EasyPlayerPluginPtr getPlugin();

    // Read/Write property ${PROPERTY.ident}
    std::string get_testString();
    void set_testString(const std::string& val);

    // Read-only property ${PROPERTY.ident}
    std::string get_version();
	// 自定义插件接口函数 [4/8/2016 Dingshuai]
	int Start( std::string& sURL, int nRenderFormat,  std::string& sUserName,  std::string& sPassword, int nHardDecode);
	void Config(int nFrameCache, int nPlaySound, int nShowToScale, int nShowStatisticInfo);
	void Close(void);

    // Method echo
    FB::variant echo(const FB::variant& msg);
    
    // Event helpers
    FB_JSAPI_EVENT(test, 0, ());
    FB_JSAPI_EVENT(echo, 2, (const FB::variant&, const int));

    // Method test-event
    void testEvent();

private:
	typedef boost::shared_ptr<EasyPlayerManager>EasyPlayerPtr;
	EasyPlayerPtr m_player;

    EasyPlayerPluginWeakPtr m_plugin;
    FB::BrowserHostPtr m_host;

    std::string m_testString;
};

#endif // H_EasyPlayerPluginAPI

