package org.easydarwin.easyplayer;

import android.app.Application;
import android.database.sqlite.SQLiteDatabase;
import android.os.Environment;

import org.easydarwin.easyplayer.data.EasyDBHelper;

/**
 * Created by afd on 8/13/16.
 */
public class TheApp extends Application {

    public static SQLiteDatabase sDB;
    public static final String sPicturePath = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_PICTURES) + "/EasyPlayer";
    public static final String sMoviePath = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_MOVIES) + "/EasyPlayer";

    @Override
    public void onCreate() {
        super.onCreate();
        sDB = new EasyDBHelper(this).getWritableDatabase();
    }
}
