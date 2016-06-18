/*
	Copyright (c) 2013-2016 EasyDarwin.ORG.  All rights reserved.
	Github: https://github.com/EasyDarwin
	WEChat: EasyDarwin
	Website: http://www.easydarwin.org
*/
package org.easydarwin.easyplayer.activity;

import android.content.DialogInterface;
import android.content.Intent;
import android.support.v4.widget.SwipeRefreshLayout;
import android.support.v7.app.ActionBar;
import android.os.Bundle;
import android.support.v7.app.AlertDialog;
import android.text.TextUtils;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ListView;
import android.widget.Toast;

import org.easydarwin.easyplayer.R;
import org.easydarwin.easyplayer.adapter.LiveVOAdapter;
import org.easydarwin.easyplayer.callback.LiveVOCallback;
import org.easydarwin.easyplayer.config.DarwinConfig;
import org.easydarwin.easyplayer.domain.LiveSession;
import org.easydarwin.easyplayer.domain.LiveVO;
import org.easydarwin.easyplayer.domain.LocalAddSessionVO;
import org.easydarwin.easyplayer.event.LocalSessionEvent;
import org.easydarwin.okhttplibrary.OkHttpUtils;
import org.greenrobot.eventbus.EventBus;
import org.greenrobot.eventbus.Subscribe;

import java.util.ArrayList;
import java.util.List;

import okhttp3.Call;
import okhttp3.Request;

public class MainActivity extends BaseActivity implements
        AdapterView.OnItemClickListener, SwipeRefreshLayout.OnRefreshListener,AdapterView.OnItemLongClickListener {

    ListView listView;
    LiveVOAdapter liveVOAdapter;
    SwipeRefreshLayout swipeRefreshLayout;
    String serverIp, serverPort;
    List<Object> allSessions = new ArrayList<>();
    List<LocalAddSessionVO> localSessions = new ArrayList<>();
    List<LiveSession> liveSessions = new ArrayList<>();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        EventBus.getDefault().register(this);
        ActionBar actionBar = getSupportActionBar();
        if (actionBar != null) {
            actionBar.setTitle(getString(R.string.app_name));
            actionBar.setDisplayShowHomeEnabled(false);
            actionBar.setDisplayHomeAsUpEnabled(false);
            actionBar.show();
        }
        listView = (ListView) findViewById(R.id.list_live);
        liveVOAdapter = new LiveVOAdapter(allSessions);
        listView.setAdapter(liveVOAdapter);
        listView.setOnItemClickListener(this);
        listView.setOnItemLongClickListener(this);
        swipeRefreshLayout = (SwipeRefreshLayout) findViewById(R.id.swip_refersh);
        swipeRefreshLayout.setOnRefreshListener(this);
        getLocalSession();
        getSeeeion();
    }

    private void getLocalSession(){
        localSessions = getMyLocalSession();
        dataChanged();
    }

    private void getSeeeion() {
        serverIp = mSettingSharedPreference.getString(DarwinConfig.SERVER_IP, DarwinConfig.DEFAULT_SERVER_IP);
        serverPort = mSettingSharedPreference.getString(DarwinConfig.SERVER_PORT, DarwinConfig.DEFAULT_SERVER_PORT);
        getSessions(serverIp, serverPort);
    }

    @Subscribe
    public void onEventMainThread(LocalSessionEvent localSessionEvent){
        localSessions = localSessionEvent.getLocalAddSessionVOs();
        dataChanged();
    }

    @Override
    protected void onResume() {
        super.onResume();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        EventBus.getDefault().unregister(this);
    }

    /**
     * 获取直播地址列表
     *
     * @param ip   服务器地址
     * @param port 服务器端口号
     */
    private void getSessions(String ip, String port) {

        if (TextUtils.isEmpty(ip) || TextUtils.isEmpty(port)) {
            return;
        }

        String url = String.format("http://%s:%s/api/getrtsppushsessions", ip, port);
        OkHttpUtils.post().url(url).build().execute(new LiveVOCallback() {

            @Override
            public void onBefore(Request request) {
                swipeRefreshLayout.setRefreshing(false);
                showWaitProgress("");
            }

            @Override
            public void onAfter() {
                hideWaitProgress();
            }

            @Override
            public void onError(Call call, Exception e) {
                Toast.makeText(MainActivity.this, "onError:" + e.toString(), Toast.LENGTH_LONG).show();
            }

            @Override
            public void onResponse(LiveVO liveVO) {
                liveSessions = liveVO.getEasyDarwin().getBody().getSessions();
                dataChanged();
                if (allSessions.size() == 0) {
                    showToadMessage("暂无直播信息");
                }
            }
        });
    }

    /**
     * 跳转到设置界面
     */
    private void toSetting() {
        startActivity(new Intent(MainActivity.this, SettingActivity.class));
    }

    @Override
    public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
        Object object = parent.getAdapter().getItem(position);
        Intent intent = new Intent(this, EasyPlayerActivity.class);
        if(object instanceof LiveSession){
            LiveSession session = (LiveSession) object;
            intent.putExtra(DarwinConfig.RTSP_ADDRESS, session.getUrl());
        }else {
            LocalAddSessionVO localAddSessionVO = (LocalAddSessionVO) object;
            intent.putExtra(DarwinConfig.RTSP_ADDRESS, localAddSessionVO.getAddress());
        }
        startActivity(intent);
    }

    @Override
    public boolean onItemLongClick(AdapterView<?> parent, View view, int position, long id) {
        Object object = parent.getAdapter().getItem(position);
        if(object instanceof LocalAddSessionVO){
            final LocalAddSessionVO session = (LocalAddSessionVO) object;
            new AlertDialog.Builder(this).setItems(new CharSequence[]{"修改", "删除"}, new DialogInterface.OnClickListener() {
                @Override
                public void onClick(DialogInterface dialog, int which) {
                    if(which == 0){
                        Intent intent = new Intent(MainActivity.this,AddLocalSessionActivity.class);
                        intent.putExtra(DarwinConfig.SESSION_DATA,session);
                        startActivity(intent);
                    }else {
                        new AlertDialog.Builder(MainActivity.this).setMessage("要删除该地址吗？").setPositiveButton("确定", new DialogInterface.OnClickListener() {
                            @Override
                            public void onClick(DialogInterface dialogInterface, int i) {
                                List<LocalAddSessionVO> localAddSessionVOs=getMyLocalSession();
                                localSessions.clear();
                                for (LocalAddSessionVO localAddSessionVO:localAddSessionVOs){
                                    if(localAddSessionVO.getId().equals(session.getId())){
                                        continue;
                                    }
                                    localSessions.add(localAddSessionVO);
                                }
                                saveMyLocalSessions(LocalAddSessionVO.toJson(localSessions));
                                dataChanged();
                            }
                        }).setNegativeButton("取消", null).show();
                    }
                }
            }).show();
        }
        return true;
    }

    private void dataChanged(){
        allSessions.clear();
        allSessions.addAll(liveSessions);
        allSessions.addAll(localSessions);
        liveVOAdapter.notifyDataSetChanged();
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        MenuInflater menuInflater = getMenuInflater();
        menuInflater.inflate(R.menu.main_menu, menu);
        return super.onCreateOptionsMenu(menu);
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case R.id.main_menu_setting:
                toSetting();
                break;
            case R.id.add_url:
                startActivity(new Intent(MainActivity.this,AddLocalSessionActivity.class));
                break;
        }
        return super.onOptionsItemSelected(item);
    }

    @Override
    public void onRefresh() {
        getLocalSession();
        getSeeeion();
    }


}
