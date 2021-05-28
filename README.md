# arduino-lfo-adsr
LittleBits Arduino LFO ADSR implementation

Inspired this project:
https://classroom.littlebits.com/inventions/adsr-lfo-super-modulator-for-arduino-bit.
Kudos to blindbento for imagining this project! (Especially the creative use of the keyboard as parameter selector).
Unfortunately the source code was removed. So this is an attempt to recreate this.

Youtube demo: https://www.youtube.com/watch?v=FogWQbIRJKc

This is my first Arduino project, and it is implemented very naïvely. I'm sure this can be optimized quite a bit, but it is working suprisingly well!

See the project mentioned above for setup.

- D0 = Trigger input
- A0 = Parameter value input (use dimmer or slider)
- A1 = Parameter choice input (with keyboard, use lowest octave!)

* D5 = (analog) LFO
* D9 = (analog) ADSR

Controls (by name of notes, set keyboard to lowest octave):

### LFO
* C#: Rate
* D#: Shape (0 = SINE, others = shift from saw to triangle to inverse saw)
* F#: Offset (ie. minimum value)
* G#: Width (ie. range of oscillation)
* A#: (not used yet)

### ADSR:
* C: Attack
* D: Decay
* E: Sustain level
* F: Release
* G: Offset (ie. minimum value)
* A: Width (ie. range of oscillation)
* B: Invert (HIGH = true)
* C: Repeat (HIGH = true)

Drawbacks:
* Pretty simple, naive implementation (my first Arduino project)
* LFO is always on right now, but can easily be rigged to trigger
* Resolution and noise is not really good, this can greatly be optimised, might need some extra hardware though

Github project:
https://github.com/DieterVDW/ardiuno-lfo-adsr
Duration: 10 minutes

Credits:

Inspired by this project: https://classroom.littlebits.com/inventions/adsr-lfo-super-modulator-for-arduino-bit. Kudos to blindbento for imagining this project!
