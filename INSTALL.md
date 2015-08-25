NDNlive: NDN Live Streaming Video Project based on Consumer/Producer API
==

Prerequisites
==
These are prerequisites to build NDNlive.

**Required:**
* [boost](http://www.boost.org/)
* [consumer-producer-api](https://github.com/iliamo/Consumer-Producer-API)
* [NFD](https://github.com/named-data/NFD.git)
* [gstreamer-1.0](http://gstreamer.freedesktop.org/data/pkg/osx/1.4.3/)

Prerequisites build instructions
==

Mac OS build considerations
-

boost
--
sudo port install boost

consumer-producer-api
--
First build and install this API.

NFD
--
First install consumer-producer-api, then install NFD. please from Source, otherwise will cover the installed con-pro-api

gstreamer-1.0
--
downlaod from http://gstreamer.freedesktop.org/data/pkg/osx/1.4.3/

**Installation: please follow the order listed below. Only tested on the 1.4.3, use exactly this version.**
* gstreamer-1.0-1.4.3-universal.pkg  --- the basic gstreamer lib
* gstreamer-1.0-devel-1.4.3-universal.pkg  --- the develpoer gstreamer lib
* gstreamer-1.0-1.4.3-universal-packages.dmg --- some useful plugins, please install all of them except the last one

**Config:**
* Please add /Library/Frameworks/GStreamer.framework/Versions/1.0/lib/pkgconfig to your $PKG_CONFIG_PATH
* e.g. add "export PKG_CONFIG_PATH=/Library/Frameworks/GStreamer.framework/Versions/1.0/lib/pkgconfig:$PKG_CONFIG_PATH" to your .bashrc

Build instructions
==
<pre>
$ ./waf configure
$ ./waf
</pre>
