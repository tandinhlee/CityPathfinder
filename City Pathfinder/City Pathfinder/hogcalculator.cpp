//
//  hogcalculator.cpp
//  City Pathfinder
//
//  Created by Dinh Le on 16/10/15.
//  Copyright © 2015 Dinh Le. All rights reserved.
//
#include "hogcalculator.hpp"
#include <array>
#include <strstream>
#define KERNEL_SIZE 31
#define IMG_RESIZE_WIDTH 320
#define IMG_RESIZE_HEIGHT 180
#define ROUND_DIGITS 5
cv::Mat histogramRGBCalculator(cv::Mat src)
{
    cv::Mat dst;
    
    if( src.empty() )
    { return src; }
    
    /// Separate the image in 3 places ( B, G and R )
    std::vector<cv::Mat> bgr_planes;
    split( src, bgr_planes );
    
    /// Establish the number of bins
    int histSize = 256;
    
    /// Set the ranges ( for B,G,R) )
    float range[] = { 0, 256 } ;
    const float* histRange = { range };
    
    bool uniform = true; bool accumulate = false;
    
    cv::Mat b_hist, g_hist, r_hist;
    
    /// Compute the histograms:
    calcHist( &bgr_planes[0], 1, 0, cv::Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
    calcHist( &bgr_planes[1], 1, 0, cv::Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
    calcHist( &bgr_planes[2], 1, 0, cv::Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );
    
    // Draw the histograms for B, G and R
    int hist_w = 512; int hist_h = 400;
    int bin_w = cvRound( (double) hist_w/histSize );
    
    cv::Mat histImage( hist_h, hist_w, CV_8UC3, cv::Scalar( 0,0,0) );
    
    /// Normalize the result to [ 0, histImage.rows ]
    normalize(b_hist, b_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat() );
    normalize(g_hist, g_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat() );
    normalize(r_hist, r_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat() );
    
    /// Draw for each channel
    for( int i = 1; i < histSize; i++ )
    {
        line( histImage, cv::Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ) ,
             cv::Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
             cv::Scalar( 255, 0, 0), 2, 8, 0  );
        line( histImage, cv::Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ) ,
             cv::Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
             cv::Scalar( 0, 255, 0), 2, 8, 0  );
        line( histImage, cv::Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ) ,
             cv::Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
             cv::Scalar( 0, 0, 255), 2, 8, 0  );
    }
    
    return histImage;
    
}

// HOGDescriptor visual_imagealizer
// adapted for arbitrary size of feature sets and training images
cv::Mat get_hogdescriptor_visual_image(cv::Mat& origImg,
                                       std::vector<float>& descriptorValues,
                                       cv::Size winSize,
                                       cv::Size cellSize,
                                       int scaleFactor,
                                       double viz_factor)
{
    cv::Mat visual_image;
    resize(origImg, visual_image, cv::Size(origImg.cols*scaleFactor, origImg.rows*scaleFactor));
    
    int gradientBinSize = 9;
    // dividing 180° into 9 bins, how large (in rad) is one bin?
    float radRangeForOneBin = (float)(CV_PI/(float)gradientBinSize);
    
    // prepare data structure: 9 orientation / gradient strenghts for each cell
    int cells_in_x_dir = winSize.width / cellSize.width;
    int cells_in_y_dir = winSize.height / cellSize.height;
    float*** gradientStrengths = new float**[cells_in_y_dir];
    int** cellUpdateCounter   = new int*[cells_in_y_dir];
    for (int y=0; y<cells_in_y_dir; y++)
    {
        gradientStrengths[y] = new float*[cells_in_x_dir];
        cellUpdateCounter[y] = new int[cells_in_x_dir];
        for (int x=0; x<cells_in_x_dir; x++)
        {
            gradientStrengths[y][x] = new float[gradientBinSize];
            cellUpdateCounter[y][x] = 0;
            
            for (int bin=0; bin<gradientBinSize; bin++)
                gradientStrengths[y][x][bin] = 0.0;
        }
    }
    
    // nr of blocks = nr of cells - 1
    // since there is a new block on each cell (overlapping blocks!) but the last one
    int blocks_in_x_dir = cells_in_x_dir - 1;
    int blocks_in_y_dir = cells_in_y_dir - 1;
    
    // compute gradient strengths per cell
    int descriptorDataIdx = 0;
    for (int blockx=0; blockx<blocks_in_x_dir; blockx++)
    {
        for (int blocky=0; blocky<blocks_in_y_dir; blocky++)
        {
            // 4 cells per block ...
            for (int cellNr=0; cellNr<4; cellNr++)
            {
                // compute corresponding cell nr
                int cellx = blockx;
                int celly = blocky;
                if (cellNr==1) celly++;
                if (cellNr==2) cellx++;
                if (cellNr==3)
                {
                    cellx++;
                    celly++;
                }
                
                for (int bin=0; bin<gradientBinSize; bin++)
                {
                    float gradientStrength = descriptorValues[ descriptorDataIdx ];
                    descriptorDataIdx++;
                    
                    gradientStrengths[celly][cellx][bin] += gradientStrength;
                    
                } // for (all bins)
                
                
                // note: overlapping blocks lead to multiple updates of this sum!
                // we therefore keep track how often a cell was updated,
                // to compute average gradient strengths
                cellUpdateCounter[celly][cellx]++;
                
            } // for (all cells)
            
            
        } // for (all block x pos)
    } // for (all block y pos)
    
    
    // compute average gradient strengths
    for (int celly=0; celly<cells_in_y_dir; celly++)
    {
        for (int cellx=0; cellx<cells_in_x_dir; cellx++)
        {
            
            float NrUpdatesForThisCell = (float)cellUpdateCounter[celly][cellx];
            
            // compute average gradient strenghts for each gradient bin direction
            for (int bin=0; bin<gradientBinSize; bin++)
            {
                gradientStrengths[celly][cellx][bin] /= NrUpdatesForThisCell;
            }
        }
    }
    // draw cells
    for (int celly=0; celly<cells_in_y_dir; celly++)
    {
        for (int cellx=0; cellx<cells_in_x_dir; cellx++)
        {
            int drawX = cellx * cellSize.width;
            int drawY = celly * cellSize.height;
            
            int mx = drawX + cellSize.width/2;
            int my = drawY + cellSize.height/2;
            
            rectangle(visual_image,
                      cv::Point(drawX*scaleFactor,drawY*scaleFactor),
                      cv::Point((drawX+cellSize.width)*scaleFactor,
                                (drawY+cellSize.height)*scaleFactor),
                      CV_RGB(100,100,100),
                      1);
            
            // draw in each cell all 9 gradient strengths
            for (int bin=0; bin<gradientBinSize; bin++)
            {
                float currentGradStrength = gradientStrengths[celly][cellx][bin];
                
                // no line to draw?
                if (currentGradStrength==0)
                    continue;
                
                float currRad = bin * radRangeForOneBin + radRangeForOneBin/2;
                
                float dirVecX = cos( currRad );
                float dirVecY = sin( currRad );
                float maxVecLen = cellSize.width/2;
                float scale = viz_factor; // just a visual_imagealization scale,
                // to see the lines better
                
                // compute line coordinates
                float x1 = mx - dirVecX * currentGradStrength * maxVecLen * scale;
                float y1 = my - dirVecY * currentGradStrength * maxVecLen * scale;
                float x2 = mx + dirVecX * currentGradStrength * maxVecLen * scale;
                float y2 = my + dirVecY * currentGradStrength * maxVecLen * scale;
                
                // draw gradient visual_imagealization
                line(visual_image,
                     cv::Point(x1*scaleFactor,y1*scaleFactor),
                     cv::Point(x2*scaleFactor,y2*scaleFactor),
                     CV_RGB(0,0,255),
                     1);
                
            } // for (all bins)
            
        } // for (cellx)
    } // for (celly)
    // don't forget to free memory allocated by helper data structures!
    for (int y=0; y<cells_in_y_dir; y++)
    {
        for (int x=0; x<cells_in_x_dir; x++)
        {
            delete[] gradientStrengths[y][x];
        }
        delete[] gradientStrengths[y];
        delete[] cellUpdateCounter[y];
    }
    delete[] gradientStrengths;
    delete[] cellUpdateCounter;
    
    return visual_image;
    
}

cv::Mat hogVisualizationCalculator(cv::Mat src){
    if( src.empty() )
    { return src; }
    cv::Mat img;
    cv::cvtColor(src, img, CV_RGB2GRAY);
    cv::HOGDescriptor hog;
    int scaleFactor = 1;
    double viz_factor = 1;
    std::vector<float> descriptorsValues;
    std::vector<cv::Point> locations;
    cv::Size cellSize = cv::Size(8,8);
    hog.compute(img, descriptorsValues,cellSize,cv::Size(0,0),locations);
    //d.computeGradient(img, grad, angleOfs);
    cv::Mat dst;
    dst = get_hogdescriptor_visual_image(img, descriptorsValues,cv::Size(img.cols,img.rows),cellSize, scaleFactor, viz_factor);
    return dst;
}

cv::Mat getGaborKernel ( double w, double theta, double sigma,
                        int filterSize, char const *filterType ) {
    
    cv::Mat kernel  = cv::Mat(filterSize,filterSize, CV_32F);
    double xr = -std::floor(filterSize/2.0)/filterSize;
    double yr = -xr;
    double step = 1.0/filterSize;
    double xrTemp, yrTemp;
    double rotation;
    double bandpass;
    double filterRadius = std::floor(filterSize / 2.0);
    double xg, yg, k, gauss;
    double sigmaScale = sigma * (filterSize / 2.0);
    double A = 1 / (2 * M_PI * sigmaScale * sigmaScale);
    for (int row  = 0; row < filterSize; row++){
        yrTemp = yr - row * step;
        for (int col = 0; col < filterSize; col++){
            xrTemp = xr + col * step;
            rotation = xrTemp * std::cos(theta) + yrTemp * std::sin(theta);
            
            if (std::strcmp(filterType, "even")){
                bandpass = std::cos(2 * M_PI * w * rotation);
            }
            else{
                bandpass = std::sin(2 * M_PI * w * rotation);
            }
            
            xg = filterRadius + row;
            yg = - filterRadius - col;
            
            k = -1 * (( xg * xg + yg * yg) / (2 * sigmaScale * sigmaScale));
            gauss = A * std::exp(k);
            kernel.at<float>(row,col) = gauss*bandpass;
        
        }
    }
    
    return kernel;
}
std::vector<double> extractFeatureCalculator(cv::Mat src) {
    std::vector<double> vector;
    if( src.empty() )
    { return vector; }
    cv::Mat img;
    cv::cvtColor(src, img, CV_RGB2GRAY);
    cv::resize(img, img, CvSize(IMG_RESIZE_WIDTH,IMG_RESIZE_HEIGHT),0,0,CV_INTER_LINEAR);
    std::array<double,4> theta{0,M_PI_4,M_PI_2,(3*M_PI)/4};
    double scaleFactor = IMG_RESIZE_WIDTH / KERNEL_SIZE;
    std::array<double,5> w {
        2.0/scaleFactor
        ,8.0/scaleFactor
        ,16.0/scaleFactor
        ,32.0/scaleFactor
        ,64.0/scaleFactor
    };
    
    for (int l=0; l < w.size(); l++) {
        for (int t =0 ; t < theta.size(); t++) {
            char const *type = "event";
            cv::Mat gaborKernel = getGaborKernel(w[l],theta[t],1/w[l],KERNEL_SIZE,type);
            cv::Mat imgAtKernel = img.clone();
            imgAtKernel.convertTo(imgAtKernel, CV_32F);
            cv::filter2D(imgAtKernel, imgAtKernel, CV_32F, gaborKernel , cv::Point(-1,-1),0,cv::BORDER_REFLECT);
            std::vector<double> mean1;
            std::vector<double> std1;
            cv::meanStdDev(imgAtKernel, mean1, std1);
            double mean = mean1.at(0);
            double std = std1.at(0);
            double variance = std*std;
            vector.push_back(mean);
            vector.push_back(variance);
        }
    }
    return vector;
}


