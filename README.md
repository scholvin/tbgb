```
XXXXXXXXX XXXXXXXX   XXXXXXXX XXXXXXXX 
XXXXXXXXX XXXXXXXXX XXXXXXXXX XXXXXXXXX
XXXXXXXXX XXXXXXXXX XXXXXXXXX XXXXXXXXX
   XXX    XXX   XXX XXX       XXX   XXX
   XXX    XXX   XXX XXX       XXX   XXX
   XXX    XXX   XXX XXX       XXX   XXX
   XXX    XXX   XXX XXX       XXX   XXX
   XXX    XXXXXXXXX XXX   XXX XXXXXXXXX
   XXX    XXXXXXXX  XXX   XXX XXXXXXXX 
   XXX    XXXXXXXXX XXX   XXX XXXXXXXXX
   XXX    XXX   XXX XXX   XXX XXX   XXX
   XXX    XXX   XXX XXX   XXX XXX   XXX
   XXX    XXX   XXX XXX   XXX XXX   XXX
   XXX    XXX   XXX XXX   XXX XXX   XXX
   XXX    XXXXXXXXX XXXXXXXXX XXXXXXXXX
   XXX    XXXXXXXXX XXXXXXXXX XXXXXXXXX
   XXX    XXXXXXXX   XXXXXXXX XXXXXXXX
```

This is the C++ code that drives the world-famous [Tiny Bit of Giant's Blood](https://www.tinybitofgiantsblood.com/) light display. 

As of late summer 2021, I'm in the process of porting it to run on a raspberry pi that will be colocated
on the control board.

It makes extensive use of the [Open Lighting Architecture](http://docs.openlighting.org/ola/man/index.html)
libraries to communicate with an [LeDMX4 Pro](https://dmxking.com/led-pixel-control/ledmx4-pro) controller
via [Art-Net](https://art-net.org.uk/), though the details of that are not important.

If you want to add an animation, feel free to take a shot and submit a pull request. Here's what
you need to do, roughly:

1. foo
2. bar
3. baz


