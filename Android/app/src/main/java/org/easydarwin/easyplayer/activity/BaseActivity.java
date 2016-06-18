/*
	Copyright (c) 2013-2016 EasyDarwin.ORG.  All rights reserved.
	Github: https://github.com/EasyDarwin
	WEChat: EasyDarwin
	Website: http://www.easydarwin.org
*/

package org.easydarwin.easyplayer.activity;

import android.content.SharedPreferences;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.widget.Toast;

import org.easydarwin.easyplayer.config.DarwinConfig;
import org.easydarwin.easyplayer.domain.LocalAddSessionVO;
import org.easydarwin.easyplayer.indicator.WaitProgressDialog;

import java.util.List;

/**
 * 类BaseActivity的实现描述：//TODO 类实现描述
 *
 * @author HELONG 2016/3/16 18:06
 */
public class BaseActivity extends AppCompatActivity {

    static SharedPreferences mSettingSharedPreference;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if(mSettingSharedPreference==null){
            mSettingSharedPreference=getSharedPreferences(DarwinConfig.SETTING_PREF_NAME,MODE_PRIVATE);
        }
    }

    protected void showToadMessage(String message){
        Toast.makeText(this,message,Toast.LENGTH_LONG).show();
    }

    private WaitProgressDialog waitProgressDialog;

    // 显示等待框
    public void showWaitProgress(String message) {
        if (waitProgressDialog == null) {
            waitProgressDialog = new WaitProgressDialog(this);
        }
        waitProgressDialog.showProgress(message);
    }

    // 隐藏等待框
    public void hideWaitProgress() {
        if (waitProgressDialog != null) {
            waitProgressDialog.hideProgress();
        }
    }

    protected List<LocalAddSessionVO> getMyLocalSession(){
        String localString=mSettingSharedPreference.getString(DarwinConfig.LOCAL_SESSION,DarwinConfig.LOCAL_SESSION_DEFAUT);
        return LocalAddSessionVO.parse(localString);
    }

    protected void saveMyLocalSessions(String json){
        SharedPreferences.Editor editor = mSettingSharedPreference.edit();
        editor.putString(DarwinConfig.LOCAL_SESSION,json);
        editor.commit();
    }

}
