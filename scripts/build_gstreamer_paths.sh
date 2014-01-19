################# SET UP GSTREAMER ENVIRONMENT ##########

cd $HOME
echo -e \\n# Gstreamer environment >> .profile
echo export GST_PLUGIN_PATH=/usr/local/lib/gstreamer-1.0/ >> .profile
echo export GST_OMX_CONFIG_DIR=/usr/local/etc/xdg/ >> .profile
echo export LD_LIBRARY_PATH=/usr/local/lib/ >> .profile
. ./.profile

rm ~/.cache/gstreamer-1.0/registry.armv6l.bin

# Install the v4l-utils to be able to use the v4l2-ctl command to configure 
# v4l2 input devices, such as webcams, if required
sudo apt-get install v4l-utils
