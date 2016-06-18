package org.easydarwin.easyplayer.domain;

import com.google.gson.Gson;
import com.google.gson.reflect.TypeToken;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;

/**
 * Created by dell on 2016/6/18 018.
 */
public class LocalAddSessionVO implements Serializable{

    private String id;
    private String name;
    private String address;

    public String getAddress() {
        return address;
    }

    public void setAddress(String address) {
        this.address = address;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public static List<LocalAddSessionVO> parse(String json){
        return new Gson().fromJson(json, new TypeToken<List<LocalAddSessionVO>>(){}.getType());
    }

    public static String toJson(List<LocalAddSessionVO> sessions){
        if(sessions == null){
            sessions = new ArrayList<>();
        }
       return new Gson().toJson(sessions);
    }
}

