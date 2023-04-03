
This is an aimless exploration into opengl....only to be worked on as a stress reliever.... so far it supports mouse and keyboard and gamepads(tested xbox one) on windows (tested Windows 11)

THIS IS AN ACTIVE PROJECT WITH UNSCREENED COMMITS. THIS IS NOT PRODUCTION CODE AND IT DEFINITELY NEEDS CLEANUP.

(Possible WinRT nuget dependency issues when building in Visual studio versions older than Visual Studio 2022 where it is included by default)

References:

Collision Detection and resolution:
http://www.peroxide.dk/papers/collision/collision.pdf

gtlf loading and opengl pipeline found in the first commit of this project is based on the YouTube tutorial playlist which can be found here: 
https://www.youtube.com/playlist?list=PLPaoO-vpZnumdcb4tZc4x5Q-v7CkrQ6M-.
https://github.com/VictorGordan/opengl-tutorials

netoworking:
https://www.boost.org/doc/libs/1_78_0/doc/html/boost_asio/examples/cpp11_examples.html

orientation to quaterion conversion:
https://gamedev.net/forums/topic/669405-creating-a-quaternion-from-a-direction/5236168/

Networking:

To play PvP on a lan (or configured WAN connection) Change the IP on line 147 of NetworkHandler.cpp (subject to drift with edits)