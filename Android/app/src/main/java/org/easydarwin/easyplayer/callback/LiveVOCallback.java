/*
	Copyright (c) 2013-2016 EasyDarwin.ORG.  All rights reserved.
	Github: https://github.com/EasyDarwin
	WEChat: EasyDarwin
	Website: http://www.easydarwin.org
*/
package org.easydarwin.easyplayer.callback;

import com.google.gson.Gson;

import org.easydarwin.easyplayer.domain.LiveVO;
import org.easydarwin.okhttplibrary.callback.Callback;

import java.io.IOException;

import okhttp3.Response;

/**
 * Created by Helong on 16/3/15-22:26.
 */
public abstract class LiveVOCallback extends Callback<LiveVO> {
    @Override
    public LiveVO parseNetworkResponse(Response response) throws IOException
    {
        String string = response.body().string();
        LiveVO liveVO = new Gson().fromJson(string, LiveVO.class);
        return liveVO;
    }
}
