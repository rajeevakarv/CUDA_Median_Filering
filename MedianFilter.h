/************************************************************************/
// The purpose of this program is to perform Median Filtering on the GPU
// and CPU.
//
// Author: Rajeev Verma
// Course: 0306-724 - High Performance Architectures
//
// File: MedianFilter.h
/************************************************************************/
#ifndef MEDIAN_FILTER_H
#define MEDIAN_FILTER_H

#include "Bitmap.h"

//Define the size of the window that will be used for filtering.
#ifndef WINDOW_SIZE
    #define WINDOW_SIZE (3)
#endif

//CPU Median Filtering
void MedianFilterCPU( Bitmap* image, Bitmap* outputImage );

//GPU Median Filtering
bool MedianFilterGPU( Bitmap* image, Bitmap* outputImage, bool sharedMemoryUse );

#endif
