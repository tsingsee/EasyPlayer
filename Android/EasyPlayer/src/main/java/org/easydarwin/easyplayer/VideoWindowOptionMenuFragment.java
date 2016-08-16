package org.easydarwin.easyplayer;


import android.animation.Animator;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.animation.AccelerateDecelerateInterpolator;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;

import org.easydarwin.easyplayer.fragments.PlayFragment;
import org.esaydarwin.rtsp.player.R;


/**
 * A simple {@link Fragment} subclass.
 * Use the {@link VideoWindowOptionMenuFragment#newInstance} factory method to
 * create an instance of this fragment.
 */
public class VideoWindowOptionMenuFragment extends Fragment {
    // TODO: Rename parameter arguments, choose names that match
    // the fragment initialization parameters, e.g. ARG_ITEM_NUMBER
    private static final String ARG_PARAM1 = "param1";
    private static final String TAG = "OptionMenuFragment";

    // TODO: Rename and change types of parameters
    private int mAttachedPlayFragmentId;
    private View remove, add, rotate;

    public int getAttachedPlayFragmentId() {
        return mAttachedPlayFragmentId;
    }

    private class MyAnimatorListener implements Animator.AnimatorListener {

        @Override
        public void onAnimationStart(Animator animation) {
            remove.setEnabled(false);
            add.setEnabled(false);
            rotate.setEnabled(false);
        }

        @Override
        public void onAnimationEnd(Animator animation) {
            remove.setEnabled(true);
            add.setEnabled(true);
            rotate.setEnabled(true);
        }

        @Override
        public void onAnimationCancel(Animator animation) {

        }

        @Override
        public void onAnimationRepeat(Animator animation) {

        }
    }

    public Animation onCreateAnimation(int transit, boolean enter, int nextAnim) {
        doAnimation(!enter, null);
        return AnimationUtils.loadAnimation(getContext(), enter ? android.R.anim.fade_in : android.R.anim.fade_out);
    }

    public VideoWindowOptionMenuFragment() {
        // Required empty public constructor
    }

    /**
     * Use this factory method to create a new instance of
     * this fragment using the provided parameters.
     *
     * @param attachedPlayFragmentId 当前OptionMenu绑定的PlayFragment的ID.
     * @return A new instance of fragment VideoWindowOptionMenuFragment.
     */
    public static VideoWindowOptionMenuFragment newInstance(int attachedPlayFragmentId) {
        VideoWindowOptionMenuFragment fragment = new VideoWindowOptionMenuFragment();
        Bundle args = new Bundle();
        args.putInt(ARG_PARAM1, attachedPlayFragmentId);
        fragment.setArguments(args);
        return fragment;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if (getArguments() != null) {
            mAttachedPlayFragmentId = getArguments().getInt(ARG_PARAM1);
        }
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup parent,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        final View view = inflater.inflate(R.layout.fragment_video_window_option_menu, parent, false);
        view.measure(View.MeasureSpec.makeMeasureSpec(parent.getWidth(), View.MeasureSpec.AT_MOST), View.MeasureSpec.makeMeasureSpec(parent.getHeight(), View.MeasureSpec.AT_MOST));
        Log.d(TAG, String.format("width:%d,height:%d", view.getMeasuredWidth(), view.getMeasuredHeight()));
        View container = view.findViewById(R.id.option_container);
        container.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                PlayActivity activity = (PlayActivity) getActivity();
                activity.onRemoveOptionMenu(VideoWindowOptionMenuFragment.this);
            }
        });
        remove = container.findViewById(R.id.action_remove_window);
        add = container.findViewById(R.id.action_add_window);
        rotate = container.findViewById(R.id.action_rotate_screen);
        add.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                PlayActivity activity = (PlayActivity) getActivity();
                activity.onRemoveOptionMenu(VideoWindowOptionMenuFragment.this);
                activity.onAddWindow();
            }
        });
        remove.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                final PlayActivity activity = (PlayActivity) getActivity();
                activity.onRemoveOptionMenu(VideoWindowOptionMenuFragment.this);
                activity.onRemoveVideoFragment(mAttachedPlayFragmentId);
            }
        });
        rotate.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                final PlayActivity activity = (PlayActivity) getActivity();
                activity.onToggleOrientation();
                activity.onRemoveOptionMenu(VideoWindowOptionMenuFragment.this);
            }
        });
        return view;
    }

    @Override
    public void onViewCreated(View view, @Nullable Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);
    }

    private void doAnimation(boolean reverse, Animator.AnimatorListener listener) {
        if (mAttachedPlayFragmentId != 0) {
            remove.animate().setInterpolator(reverse ? new PlayFragment.ReverseInterpolator() : new AccelerateDecelerateInterpolator());
            remove.setTranslationX(-getView().getMeasuredWidth() * 1.0f / 3);
            remove.animate().setListener(listener);
            remove.animate().rotationBy(180);
            remove.animate().setDuration(200);
            remove.animate().translationX(0f);


            add.animate().setInterpolator(reverse ? new PlayFragment.ReverseInterpolator() : new AccelerateDecelerateInterpolator());
            add.setTranslationX(getView().getMeasuredWidth() * 1.0f / 3);
            add.animate().rotationBy(-180);
            add.animate().setDuration(200);
            add.animate().translationX(0f);

            rotate.setAlpha(0.0f);
            rotate.animate().setInterpolator(reverse ? new PlayFragment.ReverseInterpolator() : new AccelerateDecelerateInterpolator());
            rotate.animate().setDuration(200);
            rotate.animate().alpha(1.0f);
        } else {
            remove.setVisibility(View.GONE);
            add.animate().setInterpolator(reverse ? new PlayFragment.ReverseInterpolator() : new AccelerateDecelerateInterpolator());
            add.setTranslationX(getView().getMeasuredWidth() * 0.25f);
            add.animate().rotationBy(-180);
            add.animate().setDuration(200);
            add.animate().translationX(0f);

            rotate.animate().setInterpolator(reverse ? new PlayFragment.ReverseInterpolator() : new AccelerateDecelerateInterpolator());
            rotate.setTranslationX(-getView().getMeasuredWidth() * 0.25f);
            rotate.animate().setListener(listener);
            rotate.animate().rotationBy(180);
            rotate.animate().setDuration(200);
            rotate.animate().translationX(0f);
        }
    }

}
