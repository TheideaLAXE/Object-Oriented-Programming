#include <fstream>
#include <cmath>
#include <vector>
#include <iostream>

using namespace std;

const int img_wh=1023;
const int img_hlf=img_wh/2;
const double Dist=sqrt((img_hlf*img_hlf)+(img_hlf*img_hlf));
const double Hue=255.0/(Dist+20);

struct Pixel {
    int r, g, b;
};

struct RGBImage {
    vector<vector<Pixel>> Image = vector<vector<Pixel>>(img_wh, vector<Pixel>(img_wh));
};

int main() {
    RGBImage img;

    ofstream out("Image.ppm");
    //ofstream out("Image.txt");

    for (int i = 0; i < img_wh; i++) {
        for (int j = 0; j < img_wh; j++) {
            double dist=sqrt((i-img_hlf)*(i-img_hlf)+(j-img_hlf)*(j-img_hlf));
            int len = Hue*(Dist-dist);
            //int len2=dist*tempImg;
            img.Image[i][j].r = len%255;
            img.Image[i][j].g = 0;
            img.Image[i][j].b = 0;
        }
    }

    out << "P3\n" << img_wh << " " << img_wh << "\n255\n";
    for (int i = 0; i < img_wh; i++) {
        for (int j = 0; j < img_wh; j++) {
            out << img.Image[i][j].r << " " << img.Image[i][j].g << " " << img.Image[i][j].b << "\n";
        }
        out << "\n";
    }
    cout << "Done printing!";
    return 0;
}
