# TrainyardExpress-ZXNext
A port of Matt Rix's Trainyard Express game, for the ZX Spectrum Next / N-Go.
You can download a binary [here](https://davidprograma.itch.io/trainyard-express).

![capture](https://img.itch.zone/aW1hZ2UvMTY4NDgxOC85OTMwMTcyLnBuZw==/347x500/0c2cK2.png)

## Disclaimer

This code is provided as-is, as I wanted to release the source code as there is not a lot of ZX Spectrum Next games released with full sources.

This was part of my development tree when I programmed the port. What I did is to setup this homebrew developing environment, which was good enough for me as a summer project. It may not be suited for other developers' ways of doing things, but it was not the intent. Please try to adapt yourself to the compiling / coding style, as I don't have plans of upgrading / maintaining this project. I will only answer to issues / push requests at my own discretion.

Having said that, here are the instructions for compiling. The more you comply with my enviroment, the easier it will be for you to successfully create an executable.

## Compiling

- The code was developed on Windows machines using the [z88dk](https://github.com/z88dk/z88dk) compiler at version [2.2](https://github.com/z88dk/z88dk/releases/download/v2.2/z88dk-win32-2.2.zip). For compiling with unmodified BATch files, keep in mind that I had z88dk installed under `C:\z88dk\`

- Set the z88dk environment variables with `z88dk_set_path.bat`

- For compiling, execute `compile.bat`, a `trainyard-express.nex` zxnext binary file will (should) be generated on the `build` directory.

- For executing on the [CSpect emulator](https://dailly.blogspot.com/), install CSpect version [2.16.5](http://www.javalemmings.com/public/zxnext/CSpect2_16_5.zip) under 'C:\z88dk\cspect' and execute `run.bat`

## Thanks and acknowledgments

- Marta, for beta testing and level transcription.
- Matt Rix, for creating the original Trainyard, for answering my questions, and giving me his permission for creating this port and releasing the source code.
- ManuFerHi for the N-Go, a great Spectrum Next clone.
- Mike Dailly, for his CSpect emulator, great for developing.
- The Z88DK team for their wonderful compiler.