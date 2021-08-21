#include <Imagine/Imagine.h>

#define PX_AT(b, x, y) (b->pixels + (b->width * y) + x)

bmp_t bmp_channels_expand(bmp_t* bmp, unsigned int channels)
{
    bmp_t ret = bmp_new(bmp->width, bmp->height, channels);
    if (bmp->channels != IMG_BW) {
        printf("Bitmap already has more than one channel\n");
        return ret;
    }

    for (unsigned int y = 0; y < bmp->height; y++) {
        for (unsigned int x = 0; x < bmp->width; x++) {
            int p = (int)*PX_AT(bmp, x, y);
            memset(px_at(&ret, x, y), p, channels);
        }
    }
    return ret;
}

bmp_t bmp_conv2d(bmp_t* bmp)
{
    //PASS ONE CHANNEL TO THIS
    bmp_t ret = bmp_new(bmp->width, bmp->height, 1);
    
    /*int kernel[3][3] = { // Sharpen
        {0, -1, 0},
        {-1, 5, -1},
        {0, -1, 0}
    };*/

    int kernel[3][3] = { // Emboss
        {-2, -1, 0},
        {-1, 1, 1},
        {0, 1, 2}
    };

    /*int kernel[3][3] = { // Outline
        {-1,   -1,     -1},
        {-1,    8,     -1},
        {-1,   -1,     -1}
    };*/

    /*int kernel[3][3] = { // Identity
        {0,   0,     0},
        {0,    1,    0},
        {0,   0,     0}
    };*/

    for (unsigned int y = 1; y < bmp->height - 1; y++) {
        for (unsigned int x = 1; x < bmp->width - 1; x++) {
            int p = 0;
            //int k = *PX_AT(bmp, x, y);

            p += (int)*PX_AT(bmp, x - 1, y - 1) * kernel[0][0];
            p += (int)*PX_AT(bmp, x    , y - 1) * kernel[0][1];
            p += (int)*PX_AT(bmp, x + 1, y - 1) * kernel[0][2];

            p += (int)*PX_AT(bmp, x - 1, y    ) * kernel[1][0];
            p += (int)*PX_AT(bmp, x    , y    ) * kernel[1][1];
            p += (int)*PX_AT(bmp, x + 1, y    ) * kernel[1][2];

            p += (int)*PX_AT(bmp, x - 1, y + 1) * kernel[2][0];
            p += (int)*PX_AT(bmp, x    , y + 1) * kernel[2][1];
            p += (int)*PX_AT(bmp, x + 1, y + 1) * kernel[2][2];
            
            p *= (p > 0);
            p = p * (p <= 255) + 255 * (p > 255);

            //if (k != p) printf("%d %d \n", k, p);
            *px_at(&ret, x, y) = (uint8_t)p;
        }
        //printf("\n");
    }

    bmp_t tmp = bmp_channels_expand(&ret, 3);
    bmp_free(&ret);
    return tmp;
}

bmp_t bmp_maxpooling(bmp_t* bmp)
{
    bmp_t ret = bmp_new(bmp->width / 2, bmp->height / 2, bmp->channels);
    for (unsigned int y = 0; y < ret.height; y++) {
        for (unsigned int x = 0; x < ret.width; x++) {
            int max = 0;

            int p[4] = {
                (int)*PX_AT(bmp, x, y),
                (int)*PX_AT(bmp, x + 1, y),
                (int)*PX_AT(bmp, x, y + 1),
                (int)*PX_AT(bmp, x + 1, y + 1)
            };

            for (int i = 0; i < 4; i++) {
                max = max * (max >= p[i]) + p[i] * (max < p[i]);
            }

            *px_at(&ret, x, y) = (uint8_t)max;
        }
    }
    return ret;
}