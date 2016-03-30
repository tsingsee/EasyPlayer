package org.easydarwin.easyplayer;

import android.content.DialogInterface;
import android.os.Bundle;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.text.TextUtils;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import org.esaydarwin.rtsp.player.R;
import org.easydarwin.video.RTSPClient;
import org.easydarwin.video.StreamRender;


public class MainActivity extends AppCompatActivity implements SurfaceHolder.Callback ,View.OnClickListener{

    private String mRTSPUrl;
    private StreamRender mStreamRender;
    private Surface mSurface;

    EditText edtUrl;
    Button btnPlay;

    boolean play=false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        this.requestWindowFeature(Window.FEATURE_NO_TITLE);
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        if(getSupportActionBar()!=null){
            getSupportActionBar().hide();
        }
        SurfaceView surfaceView = (SurfaceView) findViewById(R.id.surface_view);
        surfaceView.getHolder().addCallback(this);
        edtUrl= (EditText) findViewById(R.id.edt_url);
        btnPlay= (Button) findViewById(R.id.btn_play);
        btnPlay.setOnClickListener(this);
    }

    @Override
    public void surfaceCreated(final SurfaceHolder surfaceHolder) {
        mSurface=surfaceHolder.getSurface();
    }

    private void startRending(Surface surface) {
        // 创建（RTSP）码流渲染对象
        // 参数分别为：context对象、API key、播放视频的surface
        mStreamRender = new StreamRender(this, "DC68C5811EBF60AFF6AC299E5F1228F4", surface);
        // 开始播放
        mStreamRender.start(1, mRTSPUrl, RTSPClient.TRANSTYPE_TCP, RTSPClient.EASY_SDK_VIDEO_FRAME_FLAG | RTSPClient.EASY_SDK_AUDIO_FRAME_FLAG, "admin", "admin");
    }

    @Override
    public void surfaceChanged(SurfaceHolder surfaceHolder, int i, int i1, int i2) {

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder surfaceHolder) {
        if (mStreamRender != null&&play) {
            // 停止播放
            mStreamRender.stop();
            btnPlay.setText("PLAY");
            play=false;
//            mStreamRender = null;
        }
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()){
            case R.id.btn_play:
                if(play){
                    mStreamRender.stop();
                    btnPlay.setText("PLAY");
                    play=false;
                }else {
                    mRTSPUrl=edtUrl.getText().toString();
                    if(!TextUtils.isEmpty(mRTSPUrl)&&mSurface!=null){
                        startRending(mSurface);
                        btnPlay.setText("STOP");
                        play=true;
                    }
                }
                break;
        }
    }
}
