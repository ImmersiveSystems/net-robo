raspivid -t 0 -hf -vf  -h 720  -w 1280 -fps 25 -b 2000000 -o - |
ffmpeg -i - -vcodec copy  -an -f flv rtmp://192.168.1.76/live/test
