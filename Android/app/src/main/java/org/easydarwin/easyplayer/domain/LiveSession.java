/*
	Copyright (c) 2013-2016 EasyDarwin.ORG.  All rights reserved.
	Github: https://github.com/EasyDarwin
	WEChat: EasyDarwin
	Website: http://www.easydarwin.org
*/
package org.easydarwin.easyplayer.domain;

import android.text.TextUtils;

/**
 * Created by Helong on 16/3/15-22:21.
 */
public class LiveSession {


    private String index;
    private String name;
    private String url;
    private String AudienceNum;

    public String getIndex() {
        return index;
    }

    public void setIndex(String index) {
        this.index = index;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getUrl() {
        return url;
    }

    public void setUrl(String url) {
        this.url = url;
    }

    public String getAudienceNum() {
        if (TextUtils.isEmpty(AudienceNum)) {
            return "0";
        }
        return AudienceNum;
    }

    public void setAudienceNum(String audienceNum) {
        AudienceNum = audienceNum;
    }
}
