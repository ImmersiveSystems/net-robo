v4l2-ctl --set-fmt-video=width=1280,height=720,pixelformat=H264 -d /dev/video2
dd if=/dev/video2 bs=512K | ffmpeg -i - -vcodec copy -an -f flv rtmp://192.168.0.104/live/test
