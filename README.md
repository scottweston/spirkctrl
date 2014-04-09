spirkctrl
=========

Simple IR learning and transmitting for SparkCore

### Working Final Breadboard design

![Bread Boarded](spirkctrl-bboard.jpg "Bread Boarded")

### Details

As you can see from the image I'm using a standard IR reciever which doesn't
filter out the carrier, clearly there is enough capacitance in the circuit that
it didn't matter and it got filtered out (perhaps simply by using the A0 pin).
In any case you will get a far better signal read if you use one of the 38kHz
tuned IR receivers - although I've not yet had any problem.

I bought the little IR transmitter board as a kit but as you can see it's a
very simple transistor to switch 5V, I just thought this was neater and is a
little thing I'd reuse quite a lot (and it cost the same as just the IR LED on
its own from my local electronics store).

For what it's worth at some point I'll add a fritzing diagram of the circuit.

### Learning

Due to limitations on how much data can be sent through the SparkCore cloud I
went for the very simple solution of outputting learnt sequences on the serial
port which I then copy&paste into <code>const unsigned int</code> arrays in the
code and then hit <code>flash</code>. To me this seemed the simplest solution,
I have no need to program/send/understand IR codes - it's not like I
install/change my aircon or AV equipment very often (I will be adding
<code>power on</code> and <code>power off</code> for my AV amp soon as I like
the idea of having it turn on when I get home so my Chromecast is ready to rock
and roll so I can move my music from the car to the amp as I walk in the
door).
