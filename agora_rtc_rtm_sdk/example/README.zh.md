## Overview

**Agora SDK Samples** are in subdirectories.  They are source code samples for **Agora RTC SDK**, including receiving and sending audio and video media streamings in various formats. Each sample can run independently.
These samples are provided for developers to study and experience Agora's **RTC SDK API**.  With the understanding of the APIs, developer should be able to integrate desired SDK features to their own Apps easily

## Build Agora SDK Samples 

```
$ ./build-<ARCH>.sh
$ cd out/<ARCH>
$ ../../sync-data.sh

After a successful build, in subdir out/x86-64, there are **sample_xxx** executables.
Additionally, you can run ./sync-data.sh so the necessary media data file will be made available in test_data directory.
  
## Run Agora SDK Samples 运行

// Pull stream
sample_receive_yuv_pcm
# join channel 'test_cname', use default parameters to receive h264 video and auido streams. replace `XXXXXX' with your own appID.

$ ./sample_receive_h264_pcm --token XXXXXX --channelId test_cname

## Parameters:

* **--token ：** User's appId or token, no default, reqired. 
* **--channelId ：** Channel name, no default, required. 
* **--userId ：** User ID。default is **0**，SDK randomly assign
* **--remoteUserId ：** Remote user ID，specify which remote user's stream is subscribed to, no default, if this parameter is not used, all remote streams will be subscribed.
* **--audioFile ：** audio file name to save remote **PCM** audio stream。this is file's path name, default is **received_audio.pcm**
* **--sampleRate ：** audio sampling rate, default is **48000**
* **--numOfChannels ：** received audio channel number 1 or 2 (stereo) default is **1**（mono）

>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
README in Chinese
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
## 概述


**Agora SDK Samples** 位于本目录中的各个子文件夹中，是 **Agora RTC SDK** 的基本示例代码。其中包含了对各种音频、视频格式的媒体流的发送和接收。每个示例代码可以单独运行。
提供这一系列示例代码的目的，是供开发者体验和学习声网的**RTC SDK API**，并且在理解API的基础上，更轻松的集成将功能到自己的应用程序中。
强烈建议你仔细阅读**h264_pcm**文件夹中的两个sample文件，文件头中附有大量的注释帮助你快速理解本SDK的音视频收发逻辑

## Agora SDK Samples 编译

```
$ ./build-<ARCH>.sh
$ cd out/<ARCH>
$ ../../sync-data.sh

编译完成后，在out目录里会产生一系列 **sample_xxx** 可执行文件。
另外，执行sync-data.sh的作用是把实例代码中依赖的一些测试媒体文件同步到本地的test_data目录。
```

## 如果你是license用户，在体验、测试阶段：
SDK 下载包中包含了 deviceID.bin 和 certificate.bin 文件，因此你无需购买商用 License 即可免费体验 RTSA Lite SDK。免费体验存在并发限制且时效为 90 天。

## 如果你是license用户，在集成上线阶段，你需要进行如下工作：
1. 登录 [Agora 控制台](https://console.agora.io/)，点击页面右上角的用户名，在下拉列表中打开 RESTful API 页面。
2. 点击 **添加密钥** 按钮。在下方的页面中会生成新的客户 ID 和客户密钥，在右边的操作栏点击 **提交** 按钮。
3. 页面显示 **创建成功** 提示信息后，在相应的客户密钥栏点击 **下载** 按钮。
4. 保存下载下来的 **key_and_secret.txt** ，里面包含客户 ID 和客户密钥，稍后激活 License 时会用到。
5. 联系 sales@agora.io 或填写[问卷](https://www.wjx.cn/m/96954268.aspx)，购买商用的License
6. 运行 $ ./license_query --appId YOUR_APPID  --customerKey YOUR_KEY --customerSecret YOUR_SECRET
```
该程序会帮你查询你的appid下已经激活和未激活的所有licenseKey,其参数如下:
- 参数 `YOUR_APPID` 需要替换为你创建的 App ID.
- 参数 `YOUR_KEY` 需要替换为你刚才保存的 `key_and_secret.txt` 文件里的 key值
- 参数 `YOUR_SECRET` 需要替换为你刚才保存的 `key_and_secret.txt` 文件里的 secret值

选择一个未激活的key
然后运行
$ ./license_activator --appId YOUR_APPID --customerKey YOUR_KEY --customerSecret YOUR_SECRET --licenseKey YOUR_LICENSE --certOutputDir .
该程序激活成功后，默认在当前目录生成 **deviceID.bin** 和 **certificate.bin** ，启动应用时会用到它们。其参数如下：
- 参数 `YOUR_APPID` 需要替换为你创建的 App ID.
- 参数 `YOUR_KEY` 需要替换为你刚才保存的 `key_and_secret.txt` 文件里的 key值
- 参数 `YOUR_SECRET` 需要替换为你刚才保存的 `key_and_secret.txt` 文件里的 secret值
- 参数 `YOUR_LICENSE` 需要替换为你购买的商用 license 序列号（也就是license_query查询到的某个未激活的License key）


#### 参数

**sample_send_h264_pcm** 示例程序用来展示 **H264** 和 **PCM** 推流的相关功能，支持的参数选项如下：

* **--token ：** 用于指定用户的appId或token。无默认值，必填
* **--channelId ：** 用于指定加入频道的名称。无默认值，必填
* **--userId ：** 用于指定用户ID。默认值为 **0**，代表SDK随机指定
* **--audioFile ：** 用于指定发送的 **PCM** 音频文件。参数为文件路径，默认值为 **test_data/send_audio_16k_1ch.pcm**
* **--videoFile ：** 用于指定发送的 **H264** 视频文件。参数为文件路径，默认值为 **test_data/send_video.h264**
* **--sampleRate ：** 用于指定发送的音频的采样率。默认值为 **48000**
* **--numOfChannels ：** 用于指定发送的音频的通道数。默认值为 **1**（单通道）
* **--fps ：** 用于指定发送的视频的帧率。默认值为 **30**

**sample_send_h264_opus** 示例程序用来展示 **H264** 和 **Opus** 推流的相关功能，支持的参数选项如下：

* **--token ：** 用于指定用户的appId或token。无默认值，必填
* **--channelId ：** 用于指定加入频道的名称。无默认值，必填
* **--userId ：** 用于指定用户ID。默认值为 **0**，代表SDK随机指定
* **--audioFile ：** 用于指定发送的 **Opus** 音频文件。参数为文件路径，默认值为 **test_data/send_audio.opus**
* **--videoFile ：** 用于指定发送的 **H264** 视频文件。参数为文件路径，默认值为 **test_data/send_video.h264**
* **--fps ：** 用于指定发送的视频的帧率。默认值为 **30**

**sample_receive_h264_pcm** 示例程序用来展示 **H264** 和 **PCM** 拉流的相关功能，支持的参数选项如下：

* **--token ：** 用于指定用户的appId或token。无默认值，必填
* **--channelId ：** 用于指定加入频道的名称。无默认值，必填
* **--userId ：** 用于指定用户ID。默认值为 **0**，代表SDK随机指定
* **--remoteUserId ：** 用于指定远端用户ID，意思是仅订阅该远端用户发送的媒体流。无默认值，如不指定该参数，表示订阅所有远端用户发送的媒体流。
* **--audioFile ：** 用于指定音频文件名，该文件保存接收到的 **PCM** 音频流。参数为文件路径，默认值为 **received_audio.pcm**
* **--videoFile ：** 用于指定视频文件名，该文件保存接收到的 **h264** 视频流。参数为文件路径，默认值为 **received_video.h264**
* **--sampleRate ：** 用于指定接收的音频的采样率。默认值为 **48000**
* **--numOfChannels ：** 用于指定接收的音频的通道数。默认值为 **1**（单通道）
* **--streamtype ：** 用于指定接收的视频的大小流。默认值为**high**（大流），小流为**low**

**sample_send_h264_multi_inst** 示例程序用来展示单进程多实例 **H264** 推流的相关功能，支持的参数选项如下：

* **--token ：** 用于指定用户的appId或token。无默认值，必填
* **--channelId ：** 用于指定加入频道的名称。无默认值，必填
* **--userId ：** 用于指定用户ID。默认值为 **0**，代表SDK随机指定
* **--videoFile ：** 用于指定发送的 **H264** 视频文件。参数为文件路径，默认值为 **test_data/send_video.h264**
* **--fps ：** 用于指定发送的视频的帧率。默认值为 **30**
* **--numOfInstances ：** 用于指定并行发送视频流的实例数量（线程数）。默认值为 **1**

**sample_receive_decrypted_h264** 示例程序用来展示（加密） **decryted_H264** 拉流的相关功能，支持的参数选项如下：

* **--token ：** 用于指定用户的appId或token。无默认值，必填
* **--channelId ：** 用于指定加入频道的名称。无默认值，必填
* **--userId ：** 用于指定用户ID。默认值为 **0**，代表SDK随机指定
* **--remoteUserId ：** 用于指定远端用户ID，意思是仅订阅该远端用户发送的媒体流。无默认值，如不指定该参数，表示订阅所有远端用户发送的媒体流。
* **--videoFile ：** 用于指定视频文件名，该文件保存接收到的 **H264** 视频流。参数为文件路径，默认值为 **received_video.h264**
* **--streamtype ：** 用于指定接收的视频的大小流。默认值为**high**（大流），小流为**low**
* **--encrypt ：** 用于开启或关闭加密功能。默认值为**false**（关闭）。如需开启设置为**1**
* **--encryptionKey ：** 用于设置加密的密钥。可以为任意字符串。注意：如果开启加密功能却未设置密钥，程序会报错退出

**sample_send_encrypted_h264** 示例程序用来展示（加密） **decryted_H264** 推流的相关功能，支持的参数选项如下：

* **--token ：** 用于指定用户的appId或token。无默认值，必填
* **--channelId ：** 用于指定加入频道的名称。无默认值，必填
* **--userId ：** 用于指定用户ID。默认值为 **0**，代表SDK随机指定
* **--remoteUserId ：** 用于指定远端用户ID，意思是仅订阅该远端用户发送的媒体流。无默认值，如不指定该参数，表示订阅所有远端用户发送的媒体流。
* **--videoFile ：** 用于指定发送的 **H264** 视频文件。参数为文件路径，默认值为 **test_data/send_video.h264**
* **--encrypt ：** 用于开启或关闭加密功能。默认值为**false**（关闭）。如需开启设置为**1**
* **--encryptionKey ：** 用于设置加密的密钥。可以为任意字符串。注意：如果开启加密功能却未设置密钥，程序会报错退出

注意::
如果解密失败（如，某一端加密模块未开启或者双方的密钥不一致），则接收端无法接收到h264文件。

**sample_receive_mixed_audio** 示例程序用来展示接受**mixAuido**的相关功能 ，支持的参数选项如下：
* **--token ：** 用于指定用户的appId或token。无默认值，必填
* **--channelId ：** 用于指定加入频道的名称。无默认值，必填
* **--userId ：** 用于指定用户ID。默认值为 **0**，代表SDK随机指定
* **--audioFile ：** 用于指定发送的 **PCM** 音频文件。参数为文件路径，默认值为 **test_data/send_audio_16k_1ch.pcm**
* **--sampleRate ：** 用于指定接收的音频的采样率。默认值为 **48000**
* **--numOfChannels ：** 用于指定接收的音频的通道数。默认值为 **1**（单通道）

**sample_stringuid_send** **sample_stringuid_receive** 示例程序用来展示stringuid功能，支持的参数选项和**sample_send/receive_h264_pcm** 保持一致，需要注意的如下：

* 需要保证加入频道的所有用户的stringUid保持不同，否则SDK会产生预期外的行为
* 需要频道内所有用户均开启stringuid功能，否则开启stringUid与未开启stringUid的用户无法互通

#### 例子

```
// 推流

# 加入名为`test_cname`的频道，用默认参数发送H264视频和PCM音频，参数`XXXXXX`需要替换为用户自己的appId或token
$ ./sample_send_h264_pcm --token XXXXXX --channelId test_cname

# 加入名为`test_cname`的频道，用默认参数发送H264视频和Opus音频，参数`XXXXXX`需要替换为用户自己的appId或token
$ ./sample_send_h264_opus --token XXXXXX --channelId test_cname

# 加入名为`test_cname`的频道，用默认参数，分8路并行发送H264视频，参数`XXXXXX`需要替换为用户自己的appId或token
$ ./sample_send_h264_multi_inst --token XXXXXX --channelId test_cname --numOfInstances 8

// 拉流
# 加入名为`test_cname`的频道，用默认参数接收h264视频和PCM音频，参数`XXXXXX`需要替换为用户自己的appId或token
$ ./sample_receive_h264_pcm --token XXXXXX --channelId test_cname
