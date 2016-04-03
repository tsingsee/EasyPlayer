package org.easydarwin.easyplayer;

import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.content.res.Configuration;
import android.os.Bundle;
import android.os.Handler;
import android.os.ResultReceiver;
import android.support.v4.view.GestureDetectorCompat;
import android.support.v4.view.ViewConfigurationCompat;
import android.support.v7.app.AppCompatActivity;
import android.text.TextUtils;
import android.view.GestureDetector;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.ViewConfiguration;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.widget.Toast;

import com.umeng.analytics.MobclickAgent;

import org.easydarwin.video.EasyRTSPClient;
import org.easydarwin.video.RTSPClient;
import org.esaydarwin.rtsp.player.R;

public class PlayActivity extends AppCompatActivity implements SurfaceHolder.Callback {

    private String mRTSPUrl;
    private EasyRTSPClient mStreamRender;
    private ResultReceiver mResultReceiver;
    private GestureDetectorCompat mDetector;
    private int mWidth;
    private int mHeight;


    class MyGestureListener extends GestureDetector.SimpleOnGestureListener {
        private static final String DEBUG_TAG = "Gestures";

        @Override
        public boolean onDown(MotionEvent event) {

            return true;
        }

        @Override
        public boolean onDoubleTap(MotionEvent motionEvent) {
            setRequestedOrientation((getRequestedOrientation() == ActivityInfo.SCREEN_ORIENTATION_PORTRAIT || getRequestedOrientation() == ActivityInfo.SCREEN_ORIENTATION_REVERSE_PORTRAIT) ?
                    ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE : ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
            return true;
        }

    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mRTSPUrl = getIntent().getStringExtra("play_url");
        if (TextUtils.isEmpty(mRTSPUrl)) {
            finish();
            return;
        }
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        if (isLandscape()) {
            getWindow().addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
        }
        setContentView(R.layout.activity_main);

//        getSupportActionBar().setDisplayHomeAsUpEnabled(true);
        final SurfaceView surfaceView = (SurfaceView) findViewById(R.id.surface_view);
        surfaceView.getHolder().addCallback(this);
        mDetector = new GestureDetectorCompat(this, new MyGestureListener());

        mResultReceiver = new ResultReceiver(new Handler()) {
            @Override
            protected void onReceiveResult(int resultCode, Bundle resultData) {
                super.onReceiveResult(resultCode, resultData);
                if (resultCode == EasyRTSPClient.RESULT_VIDEO_DISPLAYED) {
//                    Toast.makeText(PlayActivity.this, "视频正在播放了", Toast.LENGTH_SHORT).show();
                    findViewById(android.R.id.progress).setVisibility(View.GONE);
                } else if (resultCode == EasyRTSPClient.RESULT_VIDEO_SIZE) {
                    mWidth = resultData.getInt(EasyRTSPClient.EXTRA_VIDEO_WIDTH);
                    mHeight = resultData.getInt(EasyRTSPClient.EXTRA_VIDEO_HEIGHT);
                    if (!isLandscape()) {

                        fixPlayerRatio(surfaceView, getResources().getDisplayMetrics().widthPixels, getResources().getDisplayMetrics().heightPixels);
                    }
                } else if (resultCode == EasyRTSPClient.RESULT_TIMEOUT) {
                    Toast.makeText(PlayActivity.this, "试播时间到", Toast.LENGTH_SHORT).show();
                }
            }
        };
    }

    private void fixPlayerRatio(View renderView, int maxWidth, int maxHeight) {

        int widthSize = maxWidth;
        int heightSize = maxHeight;
        int width = mWidth, height = mHeight;
        float aspectRatio = width * 1.0f / height;


        if (widthSize > heightSize * aspectRatio) {
            height = heightSize;
            width = (int) (height * aspectRatio);
        } else {
            width = widthSize;
            height = (int) (width / aspectRatio);
        }
        renderView.getLayoutParams().width = width;
        renderView.getLayoutParams().height = height;
        renderView.requestLayout();
    }

    private boolean isLandscape() {
        return getRequestedOrientation() == ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE || getRequestedOrientation() == ActivityInfo.SCREEN_ORIENTATION_REVERSE_LANDSCAPE;
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        return mDetector.onTouchEvent(event);
    }

    @Override
    public void onConfigurationChanged(Configuration newConfig) {
        super.onConfigurationChanged(newConfig);
        final View render = findViewById(R.id.surface_view);
        if (newConfig.orientation == Configuration.ORIENTATION_LANDSCAPE) {
            getWindow().addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
            setNavVisibility(true);
            final ViewGroup.LayoutParams params = render.getLayoutParams();
            params.height = ViewGroup.LayoutParams.MATCH_PARENT;
            params.width = ViewGroup.LayoutParams.MATCH_PARENT;
            render.requestLayout();
        } else {
            getWindow().clearFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);

            fixPlayerRatio(render, getResources().getDisplayMetrics().widthPixels, getResources().getDisplayMetrics().heightPixels);
            setNavVisibility(false);
        }
    }

    public void setNavVisibility(boolean visible) {
        if (!ViewConfigurationCompat.hasPermanentMenuKey(ViewConfiguration.get(this))) {
            int newVis = View.SYSTEM_UI_FLAG_LAYOUT_STABLE;
            if (!visible) {
                // } else {
                // newVis &= ~(View.SYSTEM_UI_FLAG_LOW_PROFILE |
                // View.SYSTEM_UI_FLAG_FULLSCREEN |
                // View.SYSTEM_UI_FLAG_HIDE_NAVIGATION);
                newVis |= View.SYSTEM_UI_FLAG_LAYOUT_STABLE | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION | View.SYSTEM_UI_FLAG_IMMERSIVE;
            }
            // If we are now visible, schedule a timer for us to go invisible.
            // Set the new desired visibility.
            getWindow().getDecorView().setSystemUiVisibility(newVis);
        }
    }

    public void onResume() {
        super.onResume();
        MobclickAgent.onResume(this);       //统计时长
    }

    public void onPause() {
        super.onPause();
        MobclickAgent.onPause(this);
    }

    @Override
    public void surfaceCreated(final SurfaceHolder surfaceHolder) {
//        mRTSPUrl = "rtsp://www.easydss.com:554/demo.mp4";
//        mRTSPUrl = "rtsp://121.41.73.249:554/SM-N9200.sdp";
//        mRTSPUrl = "rtsp://121.41.73.249:554/SCH-I959.sdp";
//        mRTSPUrl = "rtsp://121.41.73.249/1001_home.sdp";
//        mRTSPUrl = "rtsp://60.173.195.210:10554/live.sdp?token=279e6d95f80111e595ea000c298ba27c766dbbf15c2cf91b76e70d1e6bcaa3b5";


//        if (TextUtils.isEmpty(mRTSPUrl)) {
//            final EditText edit = new EditText(this);
//            final int hori = (int) getResources().getDimension(R.dimen.activity_horizontal_margin);
//            final int verti = (int) getResources().getDimension(R.dimen.activity_vertical_margin);
//            edit.setPadding(hori, verti, hori, verti);
//            edit.setText("rtsp://.sdp");
//            edit.setSelection("rtsp://".length());
//            new AlertDialog.Builder(this).setView(edit).setTitle("请输入播放地址").setPositiveButton("确定", new DialogInterface.OnClickListener() {
//                @Override
//                public void onClick(DialogInterface dialogInterface, int i) {
//                    mRTSPUrl = String.valueOf(edit.getText());
//                    if (TextUtils.isEmpty(mRTSPUrl)) {
//                        if (BuildConfig.DEBUG) {
////                            mRTSPUrl = "rtsp://121.41.73.249/1001_home.sdp";
////                            mRTSPUrl = "rtsp://218.204.223.237:554/live/1/66251FC11353191F/e7ooqwcfbqjoo80j.sdp";
//                        }
//                    }
//                    if (TextUtils.isEmpty(mRTSPUrl)) {
//                        Toast.makeText(PlayActivity.this, "url不合法", Toast.LENGTH_SHORT).show();
//                        return;
//                    }
//                }
//            }).setNegativeButton("取消", new DialogInterface.OnClickListener() {
//                @Override
//                public void onClick(DialogInterface dialogInterface, int i) {
//                    finish();
//                }
//            }).show();
//        } else {
//
//        }

        startRending(surfaceHolder.getSurface());
    }

    private void startRending(Surface surface) {
        mStreamRender = new EasyRTSPClient(this, "F94CAB947C2786773C95DC05244DF8CA", surface, mResultReceiver);
        mStreamRender.start(1, mRTSPUrl, RTSPClient.TRANSTYPE_TCP, RTSPClient.EASY_SDK_VIDEO_FRAME_FLAG | RTSPClient.EASY_SDK_AUDIO_FRAME_FLAG, "admin", "admin");
    }

    @Override
    public void surfaceChanged(SurfaceHolder surfaceHolder, int i, int i1, int i2) {

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder surfaceHolder) {
        if (mStreamRender != null) {
            mStreamRender.stop();
            mStreamRender = null;
        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
//        getMenuInflater().inflate(R.menu.setting, menu);
        return super.onCreateOptionsMenu(menu);
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        if (item.getItemId() == R.id.setting) {
            Intent i = new Intent(this, SettingsActivity.class);
            startActivity(i);
            return true;
        } else if (item.getItemId() == android.R.id.home) {
            finish();
            return true;
        }
        return super.onOptionsItemSelected(item);
    }
}
