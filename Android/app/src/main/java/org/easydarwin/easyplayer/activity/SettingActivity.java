/*
	Copyright (c) 2013-2016 EasyDarwin.ORG.  All rights reserved.
	Github: https://github.com/EasyDarwin
	WEChat: EasyDarwin
	Website: http://www.easydarwin.org
*/
package org.easydarwin.easyplayer.activity;

import android.content.SharedPreferences;
import android.support.v7.app.ActionBar;
import android.os.Bundle;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

import org.easydarwin.easyplayer.R;
import org.easydarwin.easyplayer.config.DarwinConfig;

public class SettingActivity extends BaseActivity implements View.OnClickListener{

    EditText edtServerIp;
    EditText edtServerPort;
    Button btnSave;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_setting);

        ActionBar actionBar=getSupportActionBar();
        if(actionBar!=null){
            actionBar.setDisplayShowTitleEnabled(true);
            actionBar.setDisplayHomeAsUpEnabled(true);
            actionBar.setTitle(R.string.title_activity_setting);
            actionBar.show();
        }

        String serverIP=mSettingSharedPreference.getString(DarwinConfig.SERVER_IP, "");
        String serverPort=mSettingSharedPreference.getString(DarwinConfig.SERVER_PORT,"8080");
        btnSave= (Button) findViewById(R.id.btn_save);
        btnSave.setOnClickListener(this);
        edtServerIp= (EditText) findViewById(R.id.edt_server_ip);
        edtServerPort= (EditText) findViewById(R.id.edt_server_port);
        edtServerIp.setText(serverIP);
        edtServerPort.setText(serverPort);
    }


    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()){
            case android.R.id.home:
                onBackPressed();
                break;
        }
        return super.onOptionsItemSelected(item);
    }

    /**
     * 保存设置信息
     */
    private void saveSetting(){
        String ip=edtServerIp.getText().toString();
        String port=edtServerPort.getText().toString();
        SharedPreferences.Editor editor=mSettingSharedPreference.edit();
        editor.putString(DarwinConfig.SERVER_PORT,port);
        editor.putString(DarwinConfig.SERVER_IP,ip);
        editor.commit();
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()){
            case R.id.btn_save:
                saveSetting();
                onBackPressed();
                break;
        }
    }
}
