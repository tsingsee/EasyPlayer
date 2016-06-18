package org.easydarwin.easyplayer.event;

import org.easydarwin.easyplayer.domain.LocalAddSessionVO;

import java.util.List;

/**
 * Created by dell on 2016/6/18 018.
 */
public class LocalSessionEvent {

    List<LocalAddSessionVO> localAddSessionVOs;

    public List<LocalAddSessionVO> getLocalAddSessionVOs() {
        return localAddSessionVOs;
    }

    public void setLocalAddSessionVOs(List<LocalAddSessionVO> localAddSessionVOs) {
        this.localAddSessionVOs = localAddSessionVOs;
    }
}
