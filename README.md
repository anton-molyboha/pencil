Simple-as-a-pencil drawing / notes taking app for a multi-touch display

The goal is to have an intuitive interface, appropriate for quick note-taking.

This is a work-in-progress, and is not usable at this point: you would not even be able to save your work! That said, you are welcome to play with it at your own risk, or check out the code, and leave feedback.

The supported features are:
 * Draw with a finger
 * Erase with two joined fingers
 * Scroll with two separated fingers 

TODO
=====

First priority:
 * Load and save
 * Zoom

Second priority:
 * Proper implementation of erasing. (Current implementation just draws a thick white curve on top)
 * More intuitive gesture detection (don't change the gesture after initial time-out: it is very annoying when the program's view of what I'm doing switches from "erasing" to "scrolling" by the end of a long and carefully crafted "surgical" erasure)
 * Smoothing the curves. Computer touch-screens are quite imprecise, and it would be nice to do some intelligent guess-work to infer what the user actually meant to draw (or erase)
 * Palm detection

Third priority:
 * iOS / Android
 * Colors
 * Settings menu
 * Full-screen mode

Installation
======

You'll have to compile from source.

Dependencies I am aware of:
 * Qt5
 
In the sources directory, do:

 * `qmake`
 * `make`
 * `cp pencil /usr/local/bin/`
