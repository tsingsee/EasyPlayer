/*
	Copyright (c) 2013-2016 EasyDarwin.ORG.  All rights reserved.
	Github: https://github.com/EasyDarwin
	WEChat: EasyDarwin
	Website: http://www.easydarwin.org
*/
package org.easydarwin.easyplayer.adapter;

import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;

import org.easydarwin.easyplayer.R;
import org.easydarwin.easyplayer.domain.LiveSession;
import org.easydarwin.easyplayer.domain.LocalAddSessionVO;

import java.util.List;

/**
 * Created by Helong on 16/3/15-22:27.
 */
public class LiveVOAdapter extends BaseAdapter {

    private List<Object> mSessions;

    public LiveVOAdapter(List<Object> mSessions) {
        this.mSessions = mSessions;
    }

    public void add(List<Object> mSessions) {
        this.mSessions.addAll(mSessions);
        notifyDataSetChanged();
    }

    @Override
    public int getCount() {
        return mSessions.size();
    }

    @Override
    public Object getItem(int position) {
        return mSessions.get(position);
    }

    @Override
    public long getItemId(int position) {
        return position;
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        LiveViewHolder viewHolder;
        if (convertView == null) {
            convertView = View.inflate(parent.getContext(), R.layout.list_live_item, null);
            viewHolder = new LiveViewHolder();
            viewHolder.txtName = (TextView) convertView.findViewById(R.id.txt_name);
            viewHolder.txtRtsp = (TextView) convertView.findViewById(R.id.txt_rtsp);
            viewHolder.txtAudienceNum = (TextView) convertView.findViewById(R.id.txt_audience_num);
            convertView.setTag(viewHolder);
        } else {
            viewHolder = (LiveViewHolder) convertView.getTag();
        }
        Object object = mSessions.get(position);

        if(object instanceof LiveSession){
            LiveSession session = (LiveSession) object;
            viewHolder.txtAudienceNum.setVisibility(View.VISIBLE);
            viewHolder.txtName.setVisibility(View.VISIBLE);
            viewHolder.txtName.setText(String.format("直播：%s", session.getName()));
            viewHolder.txtRtsp.setText(String.format("地址：%s", session.getUrl()));
            String audienceNum = String.format("在线：%s人", session.getAudienceNum());
            viewHolder.txtAudienceNum.setText(audienceNum);
        }else {
            LocalAddSessionVO localAddSessionVO = (LocalAddSessionVO) object;
            viewHolder.txtName.setVisibility(View.GONE);
            viewHolder.txtAudienceNum.setVisibility(View.GONE);
            viewHolder.txtRtsp.setText(String.format("播放：%s", localAddSessionVO.getAddress()));
        }

        return convertView;
    }

    static class LiveViewHolder {
        TextView txtName;
        TextView txtRtsp;
        TextView txtAudienceNum;
    }

}
