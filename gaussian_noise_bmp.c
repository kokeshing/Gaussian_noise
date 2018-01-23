#include <stdio.h>
#include <stdint.h>
#include <math.h>

uint32_t xor128();
double box_muller();

int32_t main(int argc, char *argv[]){
    uint8_t img[512][512][3];
    uint8_t header[14];
    uint32_t bsize;
    int32_t width;
    int32_t height;
    uint8_t infoheader[28];
    FILE *fp;

    fp = fopen(argv[1], "rb");

    fread(&header, sizeof(uint8_t), 14, fp);
    fread(&bsize, sizeof(uint32_t), 1, fp);
    fread(&width, sizeof(int32_t), 1, fp);
    fread(&height, sizeof(int32_t), 1, fp);
    fread(&infoheader, sizeof(uint8_t), 28, fp);
    for(uint32_t i = 0; i < width; i++){
        for(uint32_t j = 0; j < height; j++){
            for(uint32_t c = 0; c < 3; c++){
                fread(&img[i][j][c], sizeof(uint8_t), 1, fp);
            }
        }
    }

    fclose(fp);

    double rand = 0.0;
    double sigma = 0.0;
    
    printf("σを入力してください 推進:1.0以下\n");
    scanf("%lf", &sigma);

    for(uint32_t i = 0; i < width; i++){
        for(uint32_t j = 0; j < height; j++){
            rand = box_muller();
            rand = sigma * rand + 1.0;

            for(uint32_t c = 0; c < 3; c++){
                if(img[i][j][c] * rand > 255){
                    img[i][j][c] = 255;
                }else{
                    img[i][j][c] = img[i][j][c] * rand;
                }
            }
        }
    }

    fp = fopen("out.bmp", "wb");

    fwrite(&header, sizeof(uint8_t), 14, fp);
    fwrite(&bsize, sizeof(uint32_t), 1, fp);
    fwrite(&width, sizeof(int32_t), 1, fp);
    fwrite(&height, sizeof(int32_t), 1, fp);
    fwrite(&infoheader, sizeof(uint8_t), 28, fp);
    for(uint32_t i = 0; i < width; i++){
        for(uint32_t j = 0; j < height; j++){
            for(uint32_t c = 0; c < 3; c++){
                fwrite(&img[i][j][c], sizeof(uint8_t), 1, fp);
            }
        }
    }

    fclose(fp);

    return 0;
}

uint32_t xor128(void) { 
    static uint32_t x = 123456789;
    static uint32_t y = 362436069;
    static uint32_t z = 521288629;
    static uint32_t w = 88675123; 
    uint32_t t;
 
    t = x ^ (x << 11);
    x = y;
    y = z;
    z = w;
    return w = (w ^ (w >> 19)) ^ (t ^ (t >> 8)); 
}

double box_muller(){
    uint32_t x = 0, y = 0;
    x = xor128();
    y = xor128();
    double X, Y, Z;
    
    X = x / (double)UINT32_MAX;
    Y = y / (double)UINT32_MAX;
    
    Z = sqrt(-2 * log(X)) * cos(2 * M_PI * Y);

    return Z;
}



