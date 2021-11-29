
## Compile Agora SDK Samples（For Linux）

```
$ ./build-<ARCH>.sh
$ cd out/<ARCH>
$ ../../sync-data.sh

Upon successful compilation, there are a couple of **sample_xxx"" excutables in out folder
Also, download the media files by running sync-data.sh script. These audio/video files will be used together with the sample code.
```


## Run Agora SDK Samples

**sample_send_aac** reads from an AAC file and sends the AAC stream to Agora channel

#### Example:
```
To send aac stream to Agora channel named "demo_channel". Note that `xxxxxx` should be replaced with your own App ID or token
$ ./sample_send_aac --token XXXXXX --channelId demo_channel --audioFile test_data/send_audio.aac
```
