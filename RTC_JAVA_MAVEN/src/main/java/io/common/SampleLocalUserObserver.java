package common;

import io.agora.rtc.AgoraLocalUser;
import io.agora.rtc.IAudioFrameObserver;
import io.agora.rtc.DefaultLocalUserObserver;

public class SampleLocalUserObserver extends DefaultLocalUserObserver {

    public SampleLocalUserObserver(AgoraLocalUser localUser) {
        localUser_ = localUser;
        localUser_.registerObserver(this);
        audioFrameObserver_ = null;
    }

    public void setAudioFrameObserver(IAudioFrameObserver observer) {
        audioFrameObserver_ = observer;
        System.out.println("registerAudioFrameObserver");
        localUser_.registerAudioFrameObserver(observer);
    }

    public void unsetAudioFrameObserver() {
        if (audioFrameObserver_ != null) {
            localUser_.unregisterAudioFrameObserver();
        }
        audioFrameObserver_ = null;
    }
    
    private AgoraLocalUser localUser_;
    private IAudioFrameObserver audioFrameObserver_;
}