package org.easydarwin.easyplayer.fragments;


import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.os.Handler;
import android.os.ResultReceiver;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.SurfaceHolder;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.Toast;

import com.bumptech.glide.Glide;
import com.bumptech.glide.signature.StringSignature;

import org.easydarwin.easyplayer.PlayActivity;
import org.easydarwin.easyplayer.PlaylistActivity;
import org.easydarwin.video.EasyRTSPClient;
import org.esaydarwin.rtsp.player.R;

import java.util.UUID;


/**
 * A simple {@link Fragment} subclass.
 * Use the {@link PlayFragment2#newInstance} factory method to
 * create an instance of this fragment.
 */
public class PlayFragment2 extends PlayFragment implements SurfaceHolder.Callback {
    private GLSurfaceView surfaceView;
    // TODO: Rename parameter arguments, choose names that match
    // the fragment initialization parameters, e.g. ARG_ITEM_NUMBER

    public void setSelected(boolean selected) {
        surfaceView.animate().scaleX(selected ? 0.9f : 1.0f);
        surfaceView.animate().scaleY(selected ? 0.9f : 1.0f);
        surfaceView.animate().alpha(selected ? 0.7f : 1.0f);
    }

    /**
     * Use this factory method to create a new instance of
     * this fragment using the provided parameters.
     *
     * @param url  Parameter 1.
     * @param type Parameter 2.
     * @return A new instance of fragment PlayFragment.
     */
    // TODO: Rename and change types and number of parameters
    public static PlayFragment2 newInstance(String url, int type) {
        PlayFragment2 fragment = new PlayFragment2();
        Bundle args = new Bundle();
        args.putString(ARG_PARAM1, url);
        args.putInt(ARG_PARAM2, type);
        fragment.setArguments(args);
        return fragment;
    }

    @Override
    public View onCreateView(LayoutInflater inflater, final ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        final View view = inflater.inflate(R.layout.fragment_play_glsurfaceview, container, false);
        view.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                PlayActivity activity = (PlayActivity) getActivity();
                activity.onPlayFragmentClicked(PlayFragment2.this);
            }
        });

        cover = (ImageView) view.findViewById(R.id.surface_cover);
//        Glide.with(this).load(PlaylistActivity.url2localPosterFile(getActivity(), mUrl)).diskCacheStrategy(DiskCacheStrategy.NONE).placeholder(R.drawable.placeholder).into(new ImageViewTarget<GlideDrawable>(cover) {
//            @Override
//            protected void setResource(GlideDrawable resource) {
//                int width = resource.getIntrinsicWidth();
//                int height = resource.getIntrinsicHeight();
//                fixPlayerRatio(view, container.getWidth(), container.getHeight(), width, height);
//                cover.setImageDrawable(resource);
//            }
//        });

        Glide.with(this).load(PlaylistActivity.url2localPosterFile(getActivity(), mUrl)).signature(new StringSignature(UUID.randomUUID().toString())).fitCenter().into(cover);
        return view;
    }

    @Override
    public void onViewCreated(final View view, @Nullable Bundle savedInstanceState) {
        surfaceView = (GLSurfaceView) view.findViewById(R.id.surface_view);
        surfaceView.getHolder().removeCallback(surfaceView);
        surfaceView.getHolder().addCallback(this);
        mResultReceiver = new ResultReceiver(new Handler()) {
            @Override
            protected void onReceiveResult(int resultCode, Bundle resultData) {
                super.onReceiveResult(resultCode, resultData);
                if (resultCode == EasyRTSPClient.RESULT_VIDEO_DISPLAYED) {
//                    Toast.makeText(PlayActivity.this, "视频正在播放了", Toast.LENGTH_SHORT).show();
                    view.findViewById(android.R.id.progress).setVisibility(View.GONE);
//                    surfaceView.post(new Runnable() {
//                        @Override
//                        public void run() {
//                            if (mWidth != 0 && mHeight != 0) {
//                                Bitmap e = Bitmap.createBitmap(mWidth, mHeight, Bitmap.Config.ARGB_8888);
//                                surfaceView.getBitmap(e);
//                                File f = PlaylistActivity.url2localPosterFile(surfaceView.getContext(), mUrl);
//                                saveBitmapInFile(f.getPath(), e);
//                                e.recycle();
//                            }
//                        }
//                    });
                    cover.setVisibility(View.GONE);
                } else if (resultCode == EasyRTSPClient.RESULT_VIDEO_SIZE) {
                    mWidth = resultData.getInt(EasyRTSPClient.EXTRA_VIDEO_WIDTH);
                    mHeight = resultData.getInt(EasyRTSPClient.EXTRA_VIDEO_HEIGHT);
                    if (!isLandscape()) {

                        ViewGroup parent = (ViewGroup) view.getParent();
                        parent.addOnLayoutChangeListener(listener);
                        fixPlayerRatio(view, parent.getWidth(), parent.getHeight());
                    }
                } else if (resultCode == EasyRTSPClient.RESULT_TIMEOUT) {
                    Toast.makeText(getActivity(), "试播时间到", Toast.LENGTH_SHORT).show();
                }
            }
        };

        listener = new View.OnLayoutChangeListener() {
            @Override
            public void onLayoutChange(View v, int left, int top, int right, int bottom, int oldLeft, int oldTop, int oldRight, int oldBottom) {
                Log.d(TAG, String.format("onLayoutChange left:%d,top:%d,right:%d,bottom:%d->oldLeft:%d,oldTop:%d,oldRight:%d,oldBottom:%d", left, top, right, bottom, oldLeft, oldTop, oldRight, oldBottom));
                if (right - left != oldRight - oldLeft || bottom - top != oldBottom - oldTop) {
                    if (!isLandscape()) {
                        fixPlayerRatio(view, right - left, bottom - top);
                    } else {
                        PlayActivity activity = (PlayActivity) getActivity();
                        if (!activity.multiWindows()) {
                            view.getLayoutParams().width = ViewGroup.LayoutParams.MATCH_PARENT;
                            view.getLayoutParams().height = ViewGroup.LayoutParams.MATCH_PARENT;
                            view.requestLayout();
                        } else {
                            fixPlayerRatio(view, right - left, bottom - top);
                        }
                    }
                }
            }
        };
        ViewGroup parent = (ViewGroup) view.getParent();
        parent.addOnLayoutChangeListener(listener);
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
//        startRending(holder.getSurface());
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        if (mStreamRender != null) {
            mStreamRender.stop();
            mStreamRender = null;
        }
    }
}
