# EasyPlayer #

EasyPlayer是由EasyDarwin开源团队开发和维护的一个流媒体播放器项目，目前有RTSP版和Pro版两个版本，RTSP支持**Windows**(支持**多窗口**、包含**ActiveX**，**npAPI** Web插件)、**Android**平台，**iOS**平台，视频支持**H.264**，**H.265**，**MPEG4**，**MJPEG**，音频支持**G711A**，**G711U**，**G726**，**AAC**，支持RTSP over TCP/UDP，支持**硬解码**，是一套极佳的RTSP流播放组件；

EasyPlayer Pro版支持RTSP、RTMP、HTTP、HLS等各种流媒体协议播放，稳定、高效、可靠，支持Windows、Android、iOS！

## 代码及编译方法 ##
Android：代码地址：https://github.com/EasyDarwin/EasyPlayer_Android    编译工具：Android Studio；

iOS：代码地址：https://github.com/EasyDarwin/EasyPlayer_iOS	 编译工具：xcode;

Windows：vs2010编译；


## 最新版本下载 ##

- Windows RTSP播放器：[https://github.com/EasyDarwin/EasyPlayer/releases](https://github.com/EasyDarwin/EasyPlayer/releases "EasyPlayer_Win")

- Windows Pro全功能播放器：[https://github.com/EasyDarwin/EasyPlayer/releases/tag/v1.0.17.0315](https://github.com/EasyDarwin/EasyPlayer/releases/tag/v1.0.17.0315 "EasyPlayPro_win_v1.0.17.0315")

- Android RTSP版：http://fir.im/EasyPlayer

![EasyPlayer Android](http://www.easydarwin.org/github/images/easyplayer/firimeasyplayerdroid.png)

- Android Pro全功能版：http://fir.im/EasyPlayerPro

![EasyPlayer Pro 版](http://www.easydarwin.org/github/images/easyplayerprofirim20170205.png)

- iOS RTSP版：[https://itunes.apple.com/us/app/easy-player/id1158675416?l=zh&ls=1&mt=8](https://itunes.apple.com/us/app/easy-player/id1158675416?l=zh&ls=1&mt=8)

![EasyPlayer](http://www.easydarwin.org/github/images/easyplayer/easyplayerios20170313.png)

- iOS Pro全功能版：[https://itunes.apple.com/us/app/easyplayerpro/id1205087581?l=zh&ls=1&mt=8](https://itunes.apple.com/us/app/easyplayerpro/id1205087581?l=zh&ls=1&mt=8)

![EasyPlayer Pro 版](http://www.easydarwin.org/github/images/easyplayer/easyplayerproios20170313.png)

## DEMO效果 ##

![EasyPlayer Win](http://www.easydarwin.org/github/images/easyplayer20160908171027.png)

![EasyPlayer Android](https://raw.githubusercontent.com/EasyDarwin/EasyPlayer_Android/master/EasyPlayer/screenshot/single_video.jpg?raw=true)

![EasyPlayer iOS](http://www.easydarwin.org/github/images/easyplayeriosdemo20170310.jpg)


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
EasyPlayer是一款非常稳定的RTSP播放器，各平台版本需要经过授权才能商业使用，商业授权方案可以邮件发送至[support@easydarwin.org](mailto:support@easydarwin.org "EasyDarwin support mail")或者通过电话Tel：**13718530929** 进行更深入的技术与合作咨询；

## 获取更多信息 ##

邮件：[support@easydarwin.org](mailto:support@easydarwin.org) 

WEB：[www.EasyDarwin.org](http://www.easydarwin.org)

QQ交流群：[544917793](http://jq.qq.com/?_wv=1027&k=2IDkJId "EasyPlayer")

Copyright &copy; EasyDarwin.org 2012-2017

![EasyDarwin](http://www.easydarwin.org/skin/easydarwin/images/wx_qrcode.jpg)
