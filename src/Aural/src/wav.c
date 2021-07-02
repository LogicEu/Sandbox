#include "../Aural.h"
#include <string.h>
#include <math.h>

void wav_file_write_sine(const char* path, sine_t sine, int duration_seconds, int sample_rate, int bit_depth)
{
    FILE* file = fopen(path, "wb");
    if (!file) {
        printf("Could not open '%s' file for writing waveform audio\n", path);
        return;
    }

    //Header 
    char header[] = "RIFF----WAVEfmt ";
    fwrite(&header[0], 1, strlen(header), file);

    //WAV Info
    int32_t wavSize = 16, byteRate = sample_rate * bit_depth / 8;
    int16_t compressionCode = 1, channels = 1, blockAllign = bit_depth / 8, bit = bit_depth;

    fwrite(&wavSize, sizeof(int32_t), 1, file);
    fwrite(&compressionCode, sizeof(int16_t), 1, file);
    fwrite(&channels, sizeof(int16_t), 1, file);
    fwrite(&sample_rate, sizeof(int32_t), 1, file);
    fwrite(&byteRate, sizeof(int32_t), 1, file);
    fwrite(&blockAllign, sizeof(int16_t), 1, file);
    fwrite(&bit, sizeof(int16_t), 1, file);

    //Chunk Divider
    char dataDivider[] = "data----";
    fwrite(&dataDivider, 1, strlen(dataDivider), file);

    //Start of Audio Output
    int preAudioPosition = ftell(file);
    float maxAmp = (1 << (bit_depth - 1)) - 1; // pow(2, bit_depth - 1) - 1;
    for (int i = 0; i < sample_rate * duration_seconds; i++) {
        int16_t sample = (int)(sine_process(&sine) * maxAmp);
        fwrite(&sample, sizeof(int16_t), 1, file);
    }
    int postAudioPosition = ftell(file);
    int dif = postAudioPosition - preAudioPosition;
    int final = postAudioPosition - 8;

    // Set duration and data positions
    fseek(file, preAudioPosition - 4, SEEK_SET);
    fwrite(&dif, sizeof(int32_t), 1, file);
    fseek(file, 4, SEEK_SET);
    fwrite(&final, sizeof(int32_t), 1, file);

    fclose(file);
    printf("Succesfully writed WAV file '%s'\n", path);
}