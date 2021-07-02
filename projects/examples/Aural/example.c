#include <Aural/Aural.h>
#include <stdio.h>

int main(int argc, char** argv)
{
    int sample_rate = 44100;
    int bit_depth = 16;

    sine_t sine = sine_create(523.25f, 0.5f, sample_rate);
    wav_file_write_sine("test2.wav", sine, 2, sample_rate, bit_depth);
    return 0;
}
