/*
 * 	Copyright (c) 2013-2016 EasyDarwin.ORG.  All rights reserved.
 * 	Github: https://github.com/EasyDarwin
 * 	WEChat: EasyDarwin
 *	Website: http://www.easydarwin.org
 */

package org.easydarwin.easyplayer.indicator;

import android.app.Activity;
import android.app.Dialog;
import android.view.LayoutInflater;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.TextView;

import org.easydarwin.easyplayer.R;


/**
 * 等待加载
 *
 * @author Ming
 */
public class WaitProgressDialog extends Dialog {

    private LayoutInflater mInflater;
    private Dialog mDialog;
    private TextView mDialog_Meg;
    Activity activity;

    public WaitProgressDialog(Activity activity) {
        super(activity, true, null);
        mInflater = LayoutInflater.from(activity);
        this.activity = activity;
    }

    /**
     * 显示进度
     *
     * @param message
     */
    public void showProgress(final String message) {
        hideProgress();
        if (mDialog == null) {
            final View v1 = mInflater.inflate(R.layout.loding, null);
            mDialog = new Dialog(activity, R.style.WaitProgressDialogStyle);
            mDialog.setCanceledOnTouchOutside(false);
            mDialog.requestWindowFeature(Window.FEATURE_NO_TITLE);
            getWindow().setFlags(WindowManager.LayoutParams.FLAG_BLUR_BEHIND,
                    WindowManager.LayoutParams.FLAG_BLUR_BEHIND);
            mDialog.setContentView(v1);
        }
        mDialog.setCancelable(true);
        mDialog_Meg = ((TextView) mDialog.findViewById(R.id.tv_loding));
        mDialog_Meg.setText(message);
        mDialog.show();
    }

    /**
     * @return the isShowing
     */
    public boolean isShowing() {
        return !(mDialog == null || !mDialog.isShowing());
    }

    /**
     * 取消等待框
     */
    public void hideProgress() {
        if (mDialog != null && mDialog.isShowing()) {
            mDialog.dismiss();
        }
        mDialog = null;
    }
}
