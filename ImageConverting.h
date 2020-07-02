#ifndef IMAGECONVERTING_H
#define IMAGECONVERTING_H

#include <stdint.h>
#include <iostream>
#include <fstream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

long scale(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

uint16_t color_24bit_to_16bit(uint8_t R, uint8_t G, uint8_t B)
{
    uint8_t R_bits = scale(R, 0, 255, 0, 31);
    uint8_t G_bits = scale(G, 0, 255, 0, 63);
    uint8_t B_bits = scale(B, 0, 255, 0, 31);

    //cout << "bits: " << (uint16_t)R_bits << "  " << (uint16_t)G_bits << "  " << (uint16_t)B_bits << endl;

    uint16_t color = 0;
    color = R_bits;
    color = (color << 6) | G_bits;
    color = (color << 5) | B_bits;

    return color;

}

uint16_t byte_reverse(uint16_t in_color)
{
    uint16_t out_color = (in_color << 8) | (in_color >> 8);
    return out_color;
}

bool write_header_file_reversed(ofstream &outputFile, Mat image, string image_name)
{
    outputFile << "const uint16_t " <<  image_name << "[" << image.rows << "*" << image.cols << "] =  {" << endl;

    for(int row = 0; row < image.rows; row++)
    {
        for(int col = 0; col < image.cols; col++)
        {
            Point3_<uint8_t>* p = image.ptr<Point3_<uint8_t> >(row,col);
            outputFile << "0x" << hex << byte_reverse(color_24bit_to_16bit(p->z, p->y, p->x)) << dec;

            if(image.rows == 1)
            {
                if(col != (image.cols-1))
                    outputFile << ", ";
            }
            if(image.cols == 1)
            {
                if(row != (image.rows-1))
                    outputFile << ", ";
            }
            else
            {
                if(row*col != (image.rows-1)*(image.cols-1))
                    outputFile << ", ";
            }

        }
    outputFile << endl;
    }
    outputFile << "};";

    return 1;
}

bool write_header_file_normal(ofstream &outputFile, Mat image, string image_name)
{
    outputFile << "const uint16_t " <<  image_name << "[" << image.rows << "*" << image.cols << "] =  {" << endl;

    for(int row = 0; row < image.rows; row++)
    {
        for(int col = 0; col < image.cols; col++)
        {
            Point3_<uint8_t>* p = image.ptr<Point3_<uint8_t> >(row,col);
            outputFile << "0x" << hex << color_24bit_to_16bit(p->z, p->y, p->x) << dec;

            if(image.rows == 1)
            {
                if(col != (image.cols-1))
                    outputFile << ", ";
            }
            if(image.cols == 1)
            {
                if(row != (image.rows-1))
                    outputFile << ", ";
            }
            else
            {
                if(row*col != (image.rows-1)*(image.cols-1))
                    outputFile << ", ";
            }

        }
    outputFile << endl;
    }
    outputFile << "};";

    return 1;
}


#endif // IMAGECONVERTING_H
