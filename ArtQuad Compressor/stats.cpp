
#include "stats.h"

stats::stats(PNG& im) {
    sumRed.resize(im.width());
    sumGreen.resize(im.width());
    sumBlue.resize(im.width());
    sumsqRed.resize(im.width());
    sumsqGreen.resize(im.width());
    sumsqBlue.resize(im.width());
   

    for(unsigned int x = 0; x < im.width(); x++){
        sumRed[x].resize(im.height());
        sumGreen[x].resize(im.height());
        sumBlue[x].resize(im.height());
        sumsqRed[x].resize(im.height());
        sumsqGreen[x].resize(im.height());
        sumsqBlue[x].resize(im.height());
        for(unsigned int y = 0; y < im.height(); y++){
            RGBAPixel* pixel = im.getPixel(x, y);
            long red = pixel->r;
            long blue = pixel->b;
            long green = pixel->g;

          if (x == 0 && y == 0) {
                sumRed[x][y] = red;
                sumGreen[x][y] = green;
                sumBlue[x][y] = blue;
                sumsqRed[x][y] = red * red;
                sumsqGreen[x][y] = green * green;
                sumsqBlue[x][y] = blue * blue;
            } else if (x == 0) {
                sumRed[x][y] = sumRed[x][y-1] + red;
                sumGreen[x][y] = sumGreen[x][y-1] + green;
                sumBlue[x][y] = sumBlue[x][y-1] + blue;
                sumsqRed[x][y] = sumsqRed[x][y-1] + red*red;
                sumsqGreen[x][y] = sumsqGreen[x][y-1] + green*green;
                sumsqBlue[x][y] = sumsqBlue[x][y-1] + blue*blue;
            } else if (y == 0) {
                sumRed[x][y] = sumRed[x-1][y] + red;
                sumGreen[x][y] = sumGreen[x-1][y] + green;
                sumBlue[x][y] = sumBlue[x-1][y] + blue;
                sumsqRed[x][y] = sumsqRed[x-1][y] + red*red;
                sumsqGreen[x][y] = sumsqGreen[x-1][y] + green*green;
                sumsqBlue[x][y] = sumsqBlue[x-1][y] + blue*blue;
            } else {
                sumRed[x][y] = sumRed[x-1][y] + sumRed[x][y-1] - sumRed[x-1][y-1] + red;
                sumGreen[x][y] = sumGreen[x-1][y] + sumGreen[x][y-1] - sumGreen[x-1][y-1] + green;
                sumBlue[x][y] = sumBlue[x-1][y] + sumBlue[x][y-1] - sumBlue[x-1][y-1] + blue;
                sumsqRed[x][y] = sumsqRed[x-1][y] + sumsqRed[x][y-1] - sumsqRed[x-1][y-1] + red*red;
                sumsqBlue[x][y] = sumsqBlue[x-1][y] + sumsqBlue[x][y-1] - sumsqBlue[x-1][y-1] + blue*blue;
                sumsqGreen[x][y] = sumsqGreen[x-1][y] + sumsqGreen[x][y-1] - sumsqGreen[x-1][y-1] + green*green;
            }   
        }
    }
}

long stats::getSum(char channel, pair<int, int> ul, int dim) {
    int x = ul.first + pow(2,dim) -1;
    int y = ul.second + pow(2,dim) -1;
    long sum = 0;
    if (channel == 'r'){
        sum = sumRed[x][y];
        if (ul.first == 0 && ul.second !=0) sum = sum - sumRed[x][ul.second-1];
        else if (ul.second == 0 && ul.first != 0) sum = sum - sumRed[ul.first-1][y];
        else if (ul.first > 0 && ul.second > 0) sum = sum - sumRed[ul.first-1][y] - sumRed[x][ul.second-1] + sumRed[ul.first-1][ul.second-1];
    } else if (channel == 'g'){
        sum = sumGreen[x][y];
        if (ul.first == 0 && ul.second !=0) sum = sum - sumGreen[x][ul.second-1];
        else if (ul.second == 0 && ul.first != 0) sum = sum - sumGreen[ul.first-1][y];
        else if (ul.first > 0 && ul.second > 0) sum = sum - sumGreen[ul.first-1][y] - sumGreen[x][ul.second-1] + sumGreen[ul.first-1][ul.second-1];
    } else if (channel == 'b'){
        sum = sumBlue[x][y];
        if (ul.first == 0 && ul.second !=0) sum = sum - sumBlue[x][ul.second-1];
        else if (ul.second == 0 && ul.first != 0) sum = sum - sumBlue[ul.first-1][y];
        else if (ul.first > 0 && ul.second > 0) sum = sum - sumBlue[ul.first-1][y] - sumBlue[x][ul.second-1] + sumBlue[ul.first-1][ul.second-1];
    }
    return sum;
}

long stats::getSumSq(char channel, pair<int, int> ul, int dim) {
    int x = ul.first + pow(2,dim) -1;
    int y = ul.second + pow(2,dim) -1;
    long sum = 0;
    if (channel == 'r'){
        sum = sumsqRed[x][y];
        if (ul.first == 0 && ul.second !=0) sum = sum - sumsqRed[x][ul.second-1];
        if (ul.second == 0 && ul.first != 0) sum = sum - sumsqRed[ul.first-1][y];
        if (ul.first > 0 && ul.second > 0) sum = sum - sumsqRed[ul.first-1][y] - sumsqRed[x][ul.second-1] + sumsqRed[ul.first-1][ul.second-1];
    } else if (channel == 'g'){
        sum = sumsqGreen[x][y];
        if (ul.first == 0 && ul.second !=0) sum = sum - sumsqGreen[x][ul.second-1];
        if (ul.second == 0 && ul.first != 0) sum = sum - sumsqGreen[ul.first-1][y];
        if (ul.first > 0 && ul.second > 0) sum = sum - sumsqGreen[ul.first-1][y] - sumsqGreen[x][ul.second-1] + sumsqGreen[ul.first-1][ul.second-1];
    } else if (channel == 'b'){
        sum = sumsqBlue[x][y];
        if (ul.first == 0 && ul.second !=0) sum = sum - sumsqBlue[x][ul.second-1]; 
        if (ul.second == 0 && ul.first != 0) sum = sum - sumsqBlue[ul.first-1][y];
        if (ul.first > 0 && ul.second > 0) sum = sum - sumsqBlue[ul.first-1][y] - sumsqBlue[x][ul.second-1] + sumsqBlue[ul.first-1][ul.second-1];
    }
    return sum;
}

long stats::rectArea(int dim) {
    return (long)(pow(2,dim) * pow(2,dim));
}


/** Given a square, compute its sum of squared deviations from mean, over all color channels.
     * @param ul is (x,y) of the upper left corner of the square
     * @param dim is log of side length of the square*/
double stats::getVar(pair<int, int> ul, int dim) {
    int num_pixels = (int)rectArea(dim);

    double sum_red_sq = (double)getSumSq('r', ul, dim);
    double sum_blue_sq = (double)getSumSq('b', ul, dim);
    double sum_green_sq = (double)getSumSq('g', ul, dim);

    double sq_sum_red_avg = pow((double)getSum('r', ul, dim),2)/num_pixels;
    double sq_sum_blue_avg = pow((double)getSum('b', ul, dim),2)/num_pixels;
    double sq_sum_green_avg = pow((double)getSum('g', ul, dim),2)/num_pixels;
  
    double var_red = sum_red_sq - sq_sum_red_avg;
    double var_blue = sum_blue_sq - sq_sum_blue_avg;
    double var_green = sum_green_sq - sq_sum_green_avg;

    return var_red + var_blue + var_green;
    
}

RGBAPixel stats::getAvg(pair<int, int> ul, int dim) {
    int num_pixels = (int)rectArea(dim);

    int red = (int)(getSum('r', ul, dim)/num_pixels);
    int blue = (int)(getSum('b', ul, dim)/num_pixels);
    int green = (int)(getSum('g', ul, dim)/num_pixels);

    return RGBAPixel(red, green, blue);
}
