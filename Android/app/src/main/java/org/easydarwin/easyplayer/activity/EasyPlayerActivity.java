package org.easydarwin.easyplayer.activity;

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

import org.easydarwin.easyplayer.R;
import org.easydarwin.easyplayer.config.DarwinConfig;
import org.easydarwin.video.EasyRTSPClient;
import org.easydarwin.video.RTSPClient;

public class EasyPlayerActivity extends AppCompatActivity implements SurfaceHolder.Callback {


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
        if (getSupportActionBar() != null) {
            getSupportActionBar().hide();
        }
        mRTSPUrl = getIntent().getStringExtra(DarwinConfig.RTSP_ADDRESS);
        if (TextUtils.isEmpty(mRTSPUrl)) {
            finish();
            return;
        }
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        if (isLandscape()) {
            getWindow().addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
        }
        setContentView(R.layout.activity_easyplayer);

        final SurfaceView surfaceView = (SurfaceView) findViewById(R.id.surface_view);
        surfaceView.getHolder().addCallback(this);
        mDetector = new GestureDetectorCompat(this, new MyGestureListener());

        mResultReceiver = new ResultReceiver(new Handler()) {
            @Override
            protected void onReceiveResult(int resultCode, Bundle resultData) {
                super.onReceiveResult(resultCode, resultData);
                if (resultCode == EasyRTSPClient.RESULT_VIDEO_DISPLAYED) {
                    findViewById(android.R.id.progress).setVisibility(View.GONE);
                } else if (resultCode == EasyRTSPClient.RESULT_VIDEO_SIZE) {
                    mWidth = resultData.getInt(EasyRTSPClient.EXTRA_VIDEO_WIDTH);
                    mHeight = resultData.getInt(EasyRTSPClient.EXTRA_VIDEO_HEIGHT);
                    if (!isLandscape()) {

                        fixPlayerRatio(surfaceView, getResources().getDisplayMetrics().widthPixels, getResources().getDisplayMetrics().heightPixels);
                    }
                } else if (resultCode == EasyRTSPClient.RESULT_TIMEOUT) {
                    Toast.makeText(EasyPlayerActivity.this, "试播时间到", Toast.LENGTH_SHORT).show();
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
                newVis |= View.SYSTEM_UI_FLAG_LAYOUT_STABLE | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION | View.SYSTEM_UI_FLAG_IMMERSIVE;
            }
            getWindow().getDecorView().setSystemUiVisibility(newVis);
        }
    }

    public void onResume() {
        super.onResume();
    }

    public void onPause() {
        super.onPause();
    }

    @Override
    public void surfaceCreated(final SurfaceHolder surfaceHolder) {
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
        return super.onCreateOptionsMenu(menu);
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        if (item.getItemId() == android.R.id.home) {
            finish();
            return true;
        }
        return super.onOptionsItemSelected(item);
    }
}
