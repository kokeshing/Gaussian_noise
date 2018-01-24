#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <string.h>

typedef struct{
    uint8_t img[512][512][3];
    uint8_t header[14];
    uint32_t bsize;
    int32_t width;
    int32_t height;
    uint8_t infoheader[28];
}bmpdata;

uint32_t xor128();
double box_muller();

int32_t main(int argc, char *argv[]){

    // 対象画像のパスの取得
    uint8_t path[256] = {0};
    uint32_t pathlen;
    puts(u8"enter the path of the bmp file.");
    fgets(path, 255, stdin);
    pathlen = strlen(path);
    path[pathlen-1] = 0; // 改行コードの削除

    // ファイルを開く
    FILE *fp;
    fp = fopen(path, "rb");

    // bmpの構造にしたがってファイルを読み込んでいく
    bmpdata data = {0};
    fread(&data.header, sizeof(uint8_t), 14, fp);
    fread(&data.bsize, sizeof(uint32_t), 1, fp);
    fread(&data.width, sizeof(int32_t), 1, fp);
    fread(&data.height, sizeof(int32_t), 1, fp);
    fread(&data.infoheader, sizeof(uint8_t), 28, fp);
    for(uint32_t i = 0; i < data.width; i++){
        for(uint32_t j = 0; j < data.height; j++){
            for(uint32_t c = 0; c < 3; c++){
                fread(&data.img[i][j][c], sizeof(uint8_t), 1, fp);
            }
        }
    }

    // 入力画像のファイルを閉じる
    fclose(fp);

    // 乱数関係用の変数
    double rand = 0.0;
    double sigma = 0.0;
    
    // σを読み取る
    puts(u8"enter the value of sigma.");
    scanf("%lf", &sigma);

    // 読み取った画素部分のRGBに正規分布乱数をかけていく
    for(uint32_t i = 0; i < data.width; i++){
        for(uint32_t j = 0; j < data.height; j++){

            // 正規分布乱数の発生
            rand = box_muller();
            rand = sigma * rand + 1.0;

            for(uint32_t c = 0; c < 3; c++){
                // 255以上となる場合は255に
                if(data.img[i][j][c] * rand > 255){
                    data.img[i][j][c] = 255;
                }else{
                    data.img[i][j][c] = data.img[i][j][c] * rand;
                }
            }
        }
    }

    // 保存先ファイルを開く
    fp = fopen("out.bmp", "wb");

    // out.bmpに書き込んでいく
    fwrite(&data.header, sizeof(uint8_t), 14, fp);
    fwrite(&data.bsize, sizeof(uint32_t), 1, fp);
    fwrite(&data.width, sizeof(int32_t), 1, fp);
    fwrite(&data.height, sizeof(int32_t), 1, fp);
    fwrite(&data.infoheader, sizeof(uint8_t), 28, fp);
    for(uint32_t i = 0; i < data.width; i++){
        for(uint32_t j = 0; j < data.height; j++){
            for(uint32_t c = 0; c < 3; c++){
                fwrite(&data.img[i][j][c], sizeof(uint8_t), 1, fp);
            }
        }
    }

    // 保存先ファイルを閉じる
    fclose(fp);

    return 0;
}

// 一様乱数としてxorshift法を使用
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

// ボックスミュラー法で一様乱数から正規分布乱数に
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
