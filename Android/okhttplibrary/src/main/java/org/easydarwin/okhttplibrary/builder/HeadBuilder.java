package org.easydarwin.okhttplibrary.builder;

import org.easydarwin.okhttplibrary.OkHttpUtils;
import org.easydarwin.okhttplibrary.request.OtherRequest;
import org.easydarwin.okhttplibrary.request.RequestCall;

/**
 * Created by zhy on 16/3/2.
 */
public class HeadBuilder extends GetBuilder
{
    @Override
    public RequestCall build()
    {
        return new OtherRequest(null, null, OkHttpUtils.METHOD.HEAD, url, tag, params, headers).build();
    }
}
