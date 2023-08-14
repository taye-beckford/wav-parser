# Simple WAV File Parser
The main.c file reads a .wav file and outputs the header chunks and a specified number of raw data bytes to the console.

# Usage
First clone the repository:

`git clone https://github.com/taye-makes-code/wav-parser.git`

Then add a .wav file to the same directory or use the sample.wav provided.

Lastly compile and run the code using:

`make`

`./parse sample.wav`

You can change sample.wav to whatever .wav file you add to the directory.

Additionally can specify the number of raw bytes you want to see by modifying the `NUMRAWBYTES` variable.