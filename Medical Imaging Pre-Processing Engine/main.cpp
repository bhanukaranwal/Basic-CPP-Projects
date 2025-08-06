/*
  Medical Imaging Pre-Processing Engine (main.cpp)
  --------------------------------------------------
  - Simulates DICOM/medical image pipeline: denoising, edge detection, segmentation
  - Loads example grayscale "image" (matrix)
  - Applies Gaussian blur, Sobel edge detection, Otsu thresholding
  - Console text visualization for all main steps
  - C++17 only, single file, ready for extension

  Note: Replace synthetic test image or add file I/O as needed
*/

#include <iostream>
#include <vector>
#include <array>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <numeric>
using namespace std;

using Image = vector<vector<uint8_t>>;
// Utility: print image as ASCII
void showImg(const Image& img, const string& legend) {
    static const char charset[] = " .:-=+*#%@";
    cout << "\n-- " << legend << " --\n";
    for (const auto& row : img) {
        for (auto px : row)
            cout << charset[px/26] << charset[px/26];
        cout << "\n";
    }
    cout << endl;
}

// Gaussian blur (5x5 kernel)
Image gaussianBlur(const Image& img) {
    vector<vector<int>> kernel = {
        {1, 4, 7, 4, 1},
        {4,16,26,16,4},
        {7,26,41,26,7},
        {4,16,26,16,4},
        {1,4, 7, 4,1}
    };
    int ksum = accumulate(kernel[0].begin(), kernel[0].end(), 0)*5; // 273
    int H = img.size(), W = img[0].size();
    Image out = img;
    for (int i=2; i<H-2; ++i)
        for (int j=2; j<W-2; ++j) {
            int sum=0;
            for (int di=-2; di<=2; ++di)
                for (int dj=-2; dj<=2; ++dj)
                    sum += img[i+di][j+dj]*kernel[di+2][dj+2];
            out[i][j] = sum / ksum;
        }
    return out;
}

// Sobel edge detection
Image sobel(const Image& img) {
    int H = img.size(), W = img[0].size();
    Image out(H, vector<uint8_t>(W, 0));
    vector<vector<int>> GX = {{-1,0,1}, {-2,0,2}, {-1,0,1}};
    vector<vector<int>> GY = {{1,2,1}, {0,0,0}, {-1,-2,-1}};
    for (int i=1; i<H-1; ++i)
        for (int j=1; j<W-1; ++j) {
            int sx=0, sy=0;
            for(int di=-1;di<=1;++di)
                for(int dj=-1;dj<=1;++dj) {
                    sx += img[i+di][j+dj]*GX[di+1][dj+1];
                    sy += img[i+di][j+dj]*GY[di+1][dj+1];
                }
            int v = hypot(sx,sy);
            out[i][j] = min(255,v);
        }
    return out;
}

// Otsu thresholding (simple version)
uint8_t otsuThreshold(const Image& img) {
    int H = img.size(), W = img[0].size();
    array<int,256> hist={};
    for (const auto& row: img)
        for (auto px: row)
            hist[px]++;
    int total = H*W;
    double sum=0, sumB=0; int wB=0, wF=0; double varMax=0; uint8_t t=0;
    for(int i=0;i<256;++i) sum += i*hist[i];
    for(int i=0;i<256;++i) {
        wB+=hist[i]; if(!wB) continue; wF=total-wB; if(!wF) break;
        sumB+=i*hist[i];
        double mB=sumB/wB, mF=(sum-sumB)/wF;
        double var=wB*wF*pow(mB-mF,2);
        if(var>varMax) {varMax=var;t=i;}
    }
    return t;
}
Image threshold(const Image& img, uint8_t th) {
    int H=img.size(), W=img[0].size();
    Image out=img;
    for(int i=0;i<H;++i) for(int j=0;j<W;++j) out[i][j]=img[i][j]>th?255:0;
    return out;
}

// An example (replace for file loader!)
Image makeDemoImg() {
    int H=24, W=40;
    Image img(H, vector<uint8_t>(W,32));
    // Fill "circle" center
    for(int i=0;i<H;++i)
        for(int j=0;j<W;++j) {
            int dx=i-H/2, dy=j-W/2;
            if(dx*dx+dy*dy<40) img[i][j]=220;
            else if(dx*dx+dy*dy<90) img[i][j]=110;
            // Add "anomaly"
            if((i-5)*(i-5)+(j-15)*(j-15)<6) img[i][j]=250;
        }
    return img;
}

int main() {
    cout << "=== Medical Imaging Pre-Processing Engine Simulation ===\n";

    // 1. Load image
    Image orig = makeDemoImg();
    showImg(orig, "Original Image (synthetic MRI slice)");

    // 2. Gaussian Blur
    Image denoised = gaussianBlur(orig);
    showImg(denoised, "After Gaussian Denoising");

    // 3. Sobel Edge Detection
    Image edges = sobel(denoised);
    showImg(edges, "Sobel Edge Detection");

    // 4. Otsu threshold segmentation
    uint8_t th = otsuThreshold(denoised);
    Image segmented = threshold(denoised, th);
    showImg(segmented, "Automated Otsu Segmentation");

    // Summaries
    cout << "Summary:\n"
         << "- Denoising makes structures clearer, suppressing noise.\n"
         << "- Edge detection highlights tissue/boundary transitions.\n"
         << "- Automated Otsu segmentation isolates probable regions of interest.\n"
         << "\nNext steps: integrate with DICOM loader, visualize in GUI, or supply to ML models!\n";
    return 0;
}
