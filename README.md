About
=====

This is meant to be a more Linux packaging friendly copy of the AudioProcessing
module from the WebRTC[1] project. The ideal case is that we make no changes to
the code to make tracking upstream code easy.

This package currently only includes the AudioProcessing bits, but I am very
open to collaborating with other projects that wish to distribute other bits of
the code and hopefully eventually have a single point of packaging all the
WebRTC code to help people reuse the code and avoid keeping private copies in
several different projects.

[1] https://webrtc.googlesource.com/src

Feedback
========

Patches, suggestions welcome. You can send them to the PulseAudio mailing
list[2] or to me at the address below.

-- Arun Raghavan <mail@arunraghavan.net>

[2] http://lists.freedesktop.org/mailman/listinfo/pulseaudio-discuss

Notes
====

1. Some files need to be patch to avoid pulling in the gtest framework. This
   should ideally be pushed upstream in some way so we're able to just pull
   in what we need without changing anything.

2. It might be nice to try LTO on the library. We build a lot of code as part
   of the main AudioProcessing module deps, and it's possible that this could
   provide significant space savings.
