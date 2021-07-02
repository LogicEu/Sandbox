#include "../Aural.h"
#include <math.h>
#define TWO_PI 6.28318530718

//https://katyscode.wordpress.com/2013/02/28/cutting-your-teeth-on-fmod-part-5-real-time-streaming-of-programmatically-generated-audio/

sine_t sine_create(float freq, float amp, int sample_rate)
{
    sine_t sine;
    sine.frequency = freq;
    sine.amplitude = amp;
    sine.angle = 0.0f;
    sine.offset = TWO_PI * freq / sample_rate;
    return sine;
}

float sine_process(sine_t* sine)
{
    float sample = sine->amplitude * sin(sine->angle);
    sine->angle += sine->offset;
    return sample;
}