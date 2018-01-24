
# Gaussian_noise

24bitBMP画像にガウシアンノイズを発生させる

***DEMO:***

![RAW](https://user-images.githubusercontent.com/33972190/35308430-0dff2cf0-00eb-11e8-8253-84917fd9b896.jpg)

![OUT(σ=0.2)](https://user-images.githubusercontent.com/33972190/35308448-2ccf1e4c-00eb-11e8-9476-b99812984af8.jpg)

This picture from <http://coconut.sys.eng.shizuoka.ac.jp/bmp/>

## Build and Run

```
gcc gaussian_noise_bmp.c
```

and

```
$ ./a.out
enter the path of the bmp file.
hoge.bmp
enter the value of sigma.
0.2
```

hoge.bmp need to be smaller than 512x512.



