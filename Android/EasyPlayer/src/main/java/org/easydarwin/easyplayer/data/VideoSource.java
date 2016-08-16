package org.easydarwin.easyplayer.data;

import android.database.sqlite.SQLiteDatabase;
import android.provider.BaseColumns;

/**
 * Created by afd on 8/13/16.
 */
public class VideoSource implements BaseColumns {
    public static final String URL = "url";
    public static final String TABLE_NAME = "video_source";
    /**
     * -1 refers to manual added, otherwise pulled from server.
     */
    public static final String INDEX = "_index";
    public static final String NAME = "name";
    public static final String AUDIENCE_NUMBER = "audience_number";

    public static void createTable(SQLiteDatabase db) {
        db.execSQL(String.format("CREATE TABLE IF NOT EXISTS %s (" +
                        "%s integer primary key autoincrement, " +
                        "%s integer default -1, " +
                        "%s VARCHAR(256) NOT NULL DEFAULT '', " +
                        "%s VARCHAR(256) NOT NULL DEFAULT '', " +
                        "%s integer DEFAULT 0 " +
                        ")",
                TABLE_NAME,
                _ID,
                INDEX,
                URL,
                NAME,
                AUDIENCE_NUMBER));
    }

}
