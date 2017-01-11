# EasyPlayer #

EasyPlayer是由EasyDarwin开源团队开发和维护的一个RTSP播放器项目，目前支持**Windows**(支持**多窗口**、包含**ActiveX**，**npAPI** Web插件)、**Android**平台，**iOS**平台，视频支持**H.264**，**H.265**，**MPEG4**，**MJPEG**，音频支持**G711A**，**G711U**，**G726**，**AAC**，支持RTSP over TCP/UDP，支持**硬解码**，是一套极佳的RTSP播放组件！

## 编译方法 ##
Android：Android Studio编译；

Windows：vs2010编译；

iOS：xcode编译；


## 最新版本下载 ##

- Windows：[https://github.com/EasyDarwin/EasyPlayer/releases/tag/v1.0.16.0818](https://github.com/EasyDarwin/EasyPlayer/releases/tag/v1.0.16.0818 "EasyPlayer Win") 

- Android：http://fir.im/EasyPlayer

![EasyPlayer Android](http://www.easydarwin.org/github/images/firimeasyplayerdroid.png)

- Android Pro全功能版：http://fir.im/EasyPlayerPro

![EasyPlayer Pro Android](data:image/png;base64,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)

- iOS：https://itunes.apple.com/us/app/easy-player/id1158675416

![EasyPlayer iOS](http://www.easydarwin.org/github/images/firimeasyplayerios.png)

## DEMO效果 ##

![EasyPlayer Win](http://www.easydarwin.org/skin/easydarwin/images/easyplayer20160908171027.png)

![EasyPlayer Android](https://raw.githubusercontent.com/EasyDarwin/EasyPlayer/master/Android/EasyPlayer/screenshot/single_video.jpg?raw=true)

![EasyPlayer iOS](http://www.easydarwin.org/github/images/easyplayeriosdemo20161009.jpg)


##更新记录##

### Android v1.1.17.0110 (2017-01-10) ###

* EasyPlayer Android Pro全功能版本
* 支持RTSP、RTMP、HTTP、HLS多种流媒体协议；

### iOS v1.0.5 (2016-10-07) ###

* EasyPlayer IOS版本；

### Windows v1.2.16.1111 (2016-11-11) ###

* 修复停止模仿慢的问题
* 增加RTSP播放错误返回处理
* 增加播放日志输出

### Android v1.0.16.0818 (2016-08-18) ###

* 增加了保存视频快照功能
* 支持抓拍、录像、实时码率展示
* 支持视频窗口拖拽、放大（参考360小水滴）
* 支持手动输入视频源和获取（EasyDarwin服务器）直播视频源

### Android v1.0.16.0801 (2016-08-01) ###

* 增加了多窗口播放功能

### Windows v1.0.16.0519 (2016-05-19) ###

* 修复G.711播放噪音问题

### Android v1.0.16.0506 (2016-05-06) ###

* 修复G.711播放噪音问题

### Windows v1.2.16.0414 (2016-04-14) ###

* 增加EasyPlayer npAPI通用浏览器插件EasyPlayerPlugin工程FireBreath框架开发，支持Windows平台除了Google Chrome以外的几乎所有浏览器;
* 关于EasyPlayerPlugin的编译，在win32目录下有个text文件“EasyPlayerPlugin 编译说明.txt"专门进行说明;
* 关于FireBreath框架以及EasyPlayerPlugin工程编译的任何问题，请咨询SwordTwelve;

### Windows v1.1.16.0405 (2016-04-05) ###

* 增加EasyRTSPClient库的企业级商业授权，个人用户免费试用;
* 更新libEasyPlayer库，修复单声道声音播放导致程序异常的问题;

### Android v1.0.16.0403 (2016-04-03) ###

* EasyPlayer 安卓初版
* 视频编码支持H.264
* 音频编码支持AAC/G.711A(PCMA)/G.711U(PCMU)
* 仅支持RTSP/RTP协议

### Windows v1.0.16.0309 (2016-3-9) ###

* 增加SkinUI皮肤库，通过xml配置界面资源，将界面和代码独立开来，方便代码阅读和移植;
* 更新libEasyPlayer库，支持声音和视频的播放;
* 增加EasyPlayer IE浏览器插件，测试页面在bin目录下，**Win8及以上系统请使用全路径注册**;


## 商业授权 ##
EasyPlayer是一款非常稳定的RTSP播放器，各平台版本需要经过授权才能商业使用，具体授权方案通过邮件咨询：[support@easydarwin.org](mailto:support@easydarwin.org)

## 获取更多信息 ##

邮件：[support@easydarwin.org](mailto:support@easydarwin.org) 

WEB：[www.EasyDarwin.org](http://www.easydarwin.org)

QQ交流群：[544917793](http://jq.qq.com/?_wv=1027&k=2IDkJId "EasyPlayer")

Copyright &copy; EasyDarwin.org 2012-2016

![EasyDarwin](http://www.easydarwin.org/skin/easydarwin/images/wx_qrcode.jpg)
