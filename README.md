NDNlive
----

NDN Live Streaming Video Project based on Consumer/Producer API

For license information see LICENSE.

http://named-data.net/

Description
----

NDN-based Live Streaming Video Project using Consumer/Producer API. 

- The producer keeps capturing the video from the camera and the audio from the microphone then publishing/producing them frame by frame. 
- The consumer consumes the video and audio frame by frame, then play them back together.

Structure
----

The directory structure is as follows:

* **/root**
    * **src/** *-- source code*
    * **waf** *-- binary waf file*
    * **wscript** *-- waf instruction file*
    * **config.ini** *-- config file of this project* 
    * **.waf-tools/** *-- additional waf tools*
    * **LICENSE**
    * **README.md**
    * **INSTALL.md**

Building instructions
----
Please, see the [INSTALL.md](INSTALL.md) for build instructions.

Config
--
- You should first set the "prefix" and "stream_id" value in the config.ini
    - The "prefix" stands for the routable ndn prefix
    - The "stream_id" stands for the specific id for this stream, you can also pass the stream_id as a parameter. If you do so, the parameter will overlap value set here.

Run
--
Please keep NFD running.
Please run these binary files on the toppest path of the project (the same path with config.ini).

- Terminal 1 -- Publishing video & audio 
<pre>
$ build/producer stream_id(optional)
</pre>

- Terminal 2 -- Playing video & audio
<pre>
$ build/consumer stream_id(optional)
</pre>

License
---
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version, with the additional exemption that compiling, linking, and/or using OpenSSL is allowed.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
