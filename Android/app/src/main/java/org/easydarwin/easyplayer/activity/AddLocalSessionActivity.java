package org.easydarwin.easyplayer.activity;

import android.os.Bundle;
import android.support.v7.app.ActionBar;
import android.text.TextUtils;
import android.view.MenuItem;
import android.view.View;
import android.widget.EditText;
import android.widget.Toast;

import org.easydarwin.easyplayer.R;
import org.easydarwin.easyplayer.config.DarwinConfig;
import org.easydarwin.easyplayer.domain.LocalAddSessionVO;
import org.easydarwin.easyplayer.event.LocalSessionEvent;
import org.greenrobot.eventbus.EventBus;

import java.util.List;
import java.util.UUID;

public class AddLocalSessionActivity extends BaseActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        ActionBar actionBar=getSupportActionBar();
        if(actionBar!=null){
            actionBar.setDisplayShowTitleEnabled(true);
            actionBar.setDisplayHomeAsUpEnabled(true);
            actionBar.setTitle(R.string.title_activity_add_session);
            actionBar.show();
        }

        setContentView(R.layout.activity_add_local_session);
        final List<LocalAddSessionVO> localAddSessions = getMyLocalSession();

        final EditText txtName= (EditText) findViewById(R.id.edt_name);
        txtName.setText(String.valueOf(System.currentTimeMillis()));
        final EditText txtUrl= (EditText) findViewById(R.id.edt_url);
        final LocalAddSessionVO intentSession= (LocalAddSessionVO) getIntent().getSerializableExtra(DarwinConfig.SESSION_DATA);
        if(intentSession!=null){
            txtName.setText(intentSession.getName());
            txtUrl.setText(intentSession.getAddress());
        }
        findViewById(R.id.btn_save).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String name=txtName.getText().toString();
                String url=txtUrl.getText().toString();
                if(TextUtils.isEmpty(url)){
                    Toast.makeText(AddLocalSessionActivity.this,"请输入直播地址",Toast.LENGTH_LONG).show();
                    return;
                }
                if(!url.startsWith("rtsp://") || !url.endsWith(".sdp") || url.equals("rtsp://.sdp")){
                    Toast.makeText(AddLocalSessionActivity.this,"请输入正确的直播地址",Toast.LENGTH_LONG).show();
                    return;
                }

                if(intentSession!=null){
                    intentSession.setName(name);
                    intentSession.setAddress(url);
                    for (int i=0;i<localAddSessions.size();i++){
                        if(intentSession.getId().equals(localAddSessions.get(i).getId())){
                            localAddSessions.set(i,intentSession);
                            break;
                        }
                    }
                }else {
                    LocalAddSessionVO localAddSessionVO=new LocalAddSessionVO();
                    String id= UUID.randomUUID().toString();
                    localAddSessionVO.setAddress(url);
                    localAddSessionVO.setName(name);
                    localAddSessionVO.setId(id);
                    localAddSessions.add(localAddSessionVO);
                }
                saveMyLocalSessions(LocalAddSessionVO.toJson(localAddSessions));
                LocalSessionEvent localSessionEvent = new LocalSessionEvent();
                localSessionEvent.setLocalAddSessionVOs(localAddSessions);
                EventBus.getDefault().post(localSessionEvent);
                finish();
            }
        });
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()){
            case android.R.id.home:
                finish();
                break;
        }
        return super.onOptionsItemSelected(item);
    }
}
