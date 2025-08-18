**Tea 1690 (created by (tea), (16)-bit console from the (90)'s)**

* language: custom assembly language
* price: free
* platform: windows
* display: 256 x 240 pixels, 15bpp color depth

  * It can be described as a cross between the commodore amiga and gameboy advance in terms of capabilities, while also being similar in some ways to my previous fantasy console called the game blitz, which itself was similar to the 32x, including being hard to program for because of its bitmap based rendering and additionally missing or broken functionality. I aimed to correct this with the tea 1690, providing a strong development platform for assembly coders including myself.
  * One of the goals for this project was for it to be easier to program for than the game blitz, and easy to program for in general. I achieved this by making the hardware itself simple, so that it only takes a few instructions to get something up and running, and by creating a compact instruction set consisting of 34 opcodes that can easily be understood, while providing CISC-like capabilities.
  * Everything rendered to the frame buffer is an object on the tea 1690, which means there's no hard limit on the number of background layers or sprites, up to the 128 object limit. No hard limit but there is a practical limit, around 8 screens max before the 1690's GPU starts to lag. Frame buffers were also a thing back on the commodore amiga and had similar limitations. Of course it functioned differently, mainly because the tea 1690 renders directly to a 15bpp buffer and the commodore amiga used bitplanes but the concept is similar.
  * The limitations of the tea 1690 are intended to encourage optimization and creativity. Not limited to the point of being painful to program for like the original ZX spectrum, but not nearly as powerful as the playstation or nintendo DS.

note: this is only verified to run on windows. Any other platforms may have issues or be incompatible.
The source code was built using visual studio 2022. To access it, open the .sln files.

