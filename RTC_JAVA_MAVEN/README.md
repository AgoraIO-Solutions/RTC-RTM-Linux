# Agora Java RTC Tutorial

*其他语言版本： [简体中文](README.zh.md)*

The Agora Java sample is an open-source demo that helps you get realtime video/audio integrated into your Java applications using the Agora RTC SDKs.

With this sample app, you can:
- join RTC channel
- register PcmFrameObserver
- save remote users'audio frames to a local file received_audio.pcm

## prerequisite for Running the App
First, create a developer account at [Agora.io](https://dashboard.agora.io/signin/), and obtain an App ID.

## Integration mode
- Step 1: Download the RTC SDK. After decompressing, copy the *.jar and *.so from agora_sdk_java (for RTC) the lib/ of this project.
- Step 2: Install the dependency jar package which demo required into maven local repo, use maven commands:
mvn install:install-file -Dfile=lib/agora_rtc.jar -DgroupId=io.agora.rtc  -DartifactId=agora-rtc-sdk -Dversion=1.0 -Dpackaging=jar
mvn install:install-file -Dfile=lib/commons-cli-1.4.jar -DgroupId=io.agora.commons-cli -DartifactId=commons-cli -Dversion=1.4 -Dpackaging=ja
-step 3: copy source code:
 1. copy directory common from example_java to src/main/java/io/ and SampleReceiveYuvPcm.java  src/main/java/io/agora/
 2. modify the from line of SampleReceiveYuvPcm.java to be: package io.agora.mainClass;

- Step 4: Compile and pack the project to *.jar, use "mvn package" under the directory where pom.xml locates. It may take some time to generate the package.
- Step 5: Run the demo， use 
java -cp target/SampleReceiveYuvPcm-1.0-SNAPSHOT.jar -Dsun.boot.library.path=lib/ io.agora.mainClass.SampleReceiveYuvPcm -channelId your-channelid -token your-token(or App ID) 

## Parameters
* **--token ：** user specific AppId or token, no default value, required
* **--channelId ：** channel name, no default value, required
* **--sampleRate (optional) ：**  sending audio sampling rate default is 48000
* **--numOfChannels (optional) ：** sending audio's channel number, default is 1 (mono)

## Developer Environment Requirements
- Physical or virtual, more than Ubuntu Linux 14.04 LTS 64 bits
- apache-maven-3.6.0

## Connect Us
- You can find full API document at [Document Center](https://docs.agora.io/en/)

## License
The MIT License (MIT).
