#include <fstream>
#include <cmath>
#include <vector>

#define endl "\n"

using namespace std;

const int img_wh=17;
const int img_wh2=img_wh/2;
const double tempImg=255.0/(img_wh2*1.5);
const double Dist=sqrt((img_wh2*img_wh2)+(img_wh2*img_wh2));

struct Pixel{
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

            double dist=sqrt((i-img_wh2)*(i-img_wh2)+(j-img_wh2)*(j-img_wh2));
            int l = tempImg*(Dist-dist);
            img.Image[i][j].r = l%255;
            img.Image[i][j].g = 0;
            img.Image[i][j].b = 0;
        }
    }

    out << "P3\n" << img_wh << " " << img_wh << "\n255" << "\n";

    for (int i = 0; i < img_wh; i++) {
        for (int j = 0; j < img_wh; j++) {
            out << img.Image[i][j].r << " " << img.Image[i][j].g << " " << img.Image[i][j].b << "\n";
        }
        out << "\n";
    }

    return 0;
}
