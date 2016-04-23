package org.easydarwin.easyplayer;

import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.support.v7.widget.Toolbar;
import android.text.TextUtils;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.view.inputmethod.InputMethodManager;
import android.widget.EditText;
import android.widget.TextView;

import com.umeng.analytics.MobclickAgent;

import org.esaydarwin.rtsp.player.R;
import org.json.JSONArray;
import org.json.JSONException;

public class PlaylistActivity extends AppCompatActivity implements View.OnClickListener, View.OnLongClickListener {

    private RecyclerView mRecyclerView;
    private JSONArray mArray;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.content_playlist);
        setSupportActionBar((Toolbar) findViewById(R.id.toolbar));
        final SharedPreferences preferences = getPreferences(MODE_PRIVATE);
        try {
            mArray = new JSONArray(preferences.getString("play_list", "[\"rtsp://121.41.73.249/1001_home.sdp\"]"));
        } catch (JSONException e) {
            e.printStackTrace();
            preferences.edit().putString("play_list", "[\"rtsp://121.41.73.249/1001_home.sdp\"]").apply();
            mArray = new JSONArray();
        }
        mRecyclerView = (RecyclerView) findViewById(R.id.recycler);

        mRecyclerView.setHasFixedSize(true);
        mRecyclerView.setLayoutManager(new LinearLayoutManager(this));
        mRecyclerView.setAdapter(new RecyclerView.Adapter() {
            @Override
            public RecyclerView.ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
                return new PlayListViewHolder(getLayoutInflater().inflate(android.R.layout.simple_list_item_1, parent, false));
            }

            @Override
            public void onBindViewHolder(RecyclerView.ViewHolder holder, int position) {
                PlayListViewHolder plvh = (PlayListViewHolder) holder;
                plvh.mTextView.setText(mArray.optString(position));
            }

            @Override
            public int getItemCount() {
                return mArray.length();
            }
        });

        if (savedInstanceState == null){
            startActivity(new Intent(this, SplashActivity.class));
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
    public boolean onLongClick(View view) {
        PlayListViewHolder holder = (PlayListViewHolder) view.getTag();
        final int pos = holder.getAdapterPosition();
        if (pos != -1) {

            new AlertDialog.Builder(this).setItems(new CharSequence[]{"修改", "删除"}, new DialogInterface.OnClickListener() {
                @Override
                public void onClick(DialogInterface dialogInterface, int i) {
                    if (i == 0) {
                        String playUrl = mArray.optString(pos);
                        final EditText edit = new EditText(PlaylistActivity.this);
                        final int hori = (int) getResources().getDimension(R.dimen.activity_horizontal_margin);
                        final int verti = (int) getResources().getDimension(R.dimen.activity_vertical_margin);
                        edit.setPadding(hori, verti, hori, verti);
                        edit.setText(playUrl);
                        final AlertDialog alertDialog = new AlertDialog.Builder(PlaylistActivity.this).setView(edit).setTitle("请输入播放地址").setPositiveButton("确定", new DialogInterface.OnClickListener() {
                            @Override
                            public void onClick(DialogInterface dialogInterface, int i) {
                                String mRTSPUrl = String.valueOf(edit.getText());
                                if (TextUtils.isEmpty(mRTSPUrl)) {
                                    return;
                                }
                                try {
                                    mArray.put(pos, mRTSPUrl);
                                    final SharedPreferences preferences = getPreferences(MODE_PRIVATE);
                                    preferences.edit().putString("play_list", String.valueOf(mArray)).apply();
                                    mRecyclerView.getAdapter().notifyItemChanged(pos);
                                } catch (JSONException e) {
                                    e.printStackTrace();
                                }
                            }
                        }).setNegativeButton("取消", null).create();
                        alertDialog.setOnShowListener(new DialogInterface.OnShowListener() {
                            @Override
                            public void onShow(DialogInterface dialogInterface) {
                                InputMethodManager imm = (InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE);
                                imm.showSoftInput(edit, InputMethodManager.SHOW_IMPLICIT);
                            }
                        });
                        alertDialog.show();
                    } else {
                        new AlertDialog.Builder(PlaylistActivity.this).setMessage("确定要删除该地址吗？").setPositiveButton("确定", new DialogInterface.OnClickListener() {
                            @Override
                            public void onClick(DialogInterface dialogInterface, int i) {
                                try {
                                    mArray.put(pos, null);
                                    final SharedPreferences preferences = getPreferences(MODE_PRIVATE);
                                    preferences.edit().putString("play_list", String.valueOf(mArray)).apply();
                                    mRecyclerView.getAdapter().notifyItemRemoved(pos);
                                } catch (JSONException e) {
                                    e.printStackTrace();
                                }
                            }
                        }).setNegativeButton("取消", null).show();
                    }
                }
            }).show();
        }
        return true;
    }

    class PlayListViewHolder extends RecyclerView.ViewHolder {

        private final TextView mTextView;

        public PlayListViewHolder(View itemView) {
            super(itemView);
            itemView.setBackgroundResource(android.R.drawable.list_selector_background);
            mTextView = (TextView) itemView.findViewById(android.R.id.text1);
            itemView.setOnClickListener(PlaylistActivity.this);
            itemView.setOnLongClickListener(PlaylistActivity.this);
            itemView.setTag(this);
        }

    }


    @Override
    public void onClick(View view) {
        PlayListViewHolder holder = (PlayListViewHolder) view.getTag();
        int pos = holder.getAdapterPosition();
        if (pos != -1) {
            String playUrl = mArray.optString(pos);
            if (!TextUtils.isEmpty(playUrl)) {
                Intent i = new Intent(PlaylistActivity.this, PlayActivity.class);
                i.putExtra("play_url", playUrl);
                startActivity(i);
            }
        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.add_url, menu);
        return super.onCreateOptionsMenu(menu);
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        if (item.getItemId() == R.id.add_url) {
            final EditText edit = new EditText(this);
            final int hori = (int) getResources().getDimension(R.dimen.activity_horizontal_margin);
            final int verti = (int) getResources().getDimension(R.dimen.activity_vertical_margin);
            edit.setPadding(hori, verti, hori, verti);
            edit.setText("rtsp://.sdp");
            edit.setSelection("rtsp://".length());
            final AlertDialog dlg = new AlertDialog.Builder(this).setView(edit).setTitle("请输入播放地址").setPositiveButton("确定", new DialogInterface.OnClickListener() {
                @Override
                public void onClick(DialogInterface dialogInterface, int i) {
                    String mRTSPUrl = String.valueOf(edit.getText());
                    if (TextUtils.isEmpty(mRTSPUrl)) {
                        return;
                    }
                    mArray.put(mRTSPUrl);

                    final SharedPreferences preferences = getPreferences(MODE_PRIVATE);
                    preferences.edit().putString("play_list", String.valueOf(mArray)).apply();
                    mRecyclerView.getAdapter().notifyItemInserted(mArray.length() - 1);
                }
            }).setNegativeButton("取消", null).create();
            dlg.setOnShowListener(new DialogInterface.OnShowListener() {
                @Override
                public void onShow(DialogInterface dialogInterface) {
                    InputMethodManager imm = (InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE);
                    imm.showSoftInput(edit, InputMethodManager.SHOW_IMPLICIT);
                }
            });
            dlg.show();
        }
        return super.onOptionsItemSelected(item);
    }
}
