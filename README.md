# Conway's Game of Life for Rainbowduino

**[Conway's Game of Life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life)** is zero-player mathematical simulation based on cellular automata. It has very simple rules (see the Wikipedia link above) and it's often presented as a programming task for beginner developers.

**[Rainbowduino](http://www.seeedstudio.com/wiki/Rainbowduino_v3.0)** is Arduino clone intended for driving of 8*8 RGB LED matrix.

This project brings the two together - implements the Game of Life on Rainbowduino and its many clones.

![Finished project](http://thingiverse-production-new.s3.amazonaws.com/renders/b6/01/27/03/ff/42b803bd1a637658adc5e4fd2be5c1a3_preview_featured.JPG)

I also designed a nice case for this project, which can be 3D printed and you can download it on [Thingiverse](http://www.thingiverse.com/thing:1638743). The entire thing can be seen in action on [YouTube](https://www.youtube.com/watch?v=WKVRDCAMNG4).

## Implementation

My intention was to create some nice aesthetics with the Rainbowduino. The Game of life is preferred over just randomly blinking lights for its specific meaning and general fun. The implementation detects finite states and just resets the field with random values. It does not detect oscillations between two or more states, but will reset to random state every 120 steps (configurable) anyway.

The implementation is very straightforward and intended to be used also as a teaching tool when practical. The code readability is preferred over effectivity, because we update the state only once per second anyway.

### Problems With Randomness

My cheap Chinese clone of Rainbowduino does not have any GPIO ports at all, neither analog or digital ones. The Rainbowduino is generally intended to serve just as a display driver and expects to be run by something better, so this is not generally a big limitation. So there isn't any source of randomness at all, which makes seeding the random number generator impossible. 

I discovered that the `A6` and `A7` inputs do oscillate a bit, so I implemented a method which tries to extract some entropy from them. It's not very good and I don't recommend this approach for anything more important than pretty *blinkenlights*.

## Development Note

I'm using Visual Studio 2015 and the "Visual Micro Arduino IDE" plugin for Arduino projects. You may ignore the additional files (`.sln`, `.vcxproj`, `.vcxproj.filters`) and work only with `.ino` in Arduino IDE.

## License

Copyright (c) Michal Altair Valasek, 2016

Licensed under terms of the MIT license.
