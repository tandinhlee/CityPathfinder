//
//  CVWrapper.m
//  City Pathfinder
//
//  Created by Dinh Le on 03/09/15.
//  Copyright © 2015 Dinh Le. All rights reserved.
//

#import "CVWrapper.h"
#import "UIImage+OpenCV.h"
#include "hogcalculator.hpp"
using namespace cv;
@implementation CVWrapper
+(UIImage *) getHistogramRGB:(UIImage *)sourceImage{
    UIImage *resultImage = nil;
    //Processing here !
    cv::Mat sourceMat = [sourceImage cvMatFromUIImage];
    cv::Mat desMat = histogramRGBCalculator(sourceMat);
    //cv::Mat greyMat;
    //cv::cvtColor(sourceMat, greyMat, CV_BGR2GRAY);
    resultImage = [UIImage UIImageFromCVMat:desMat];
    return resultImage;
}
+(UIImage *) getHOGVisualization:(UIImage *)sourceImage{
    UIImage *resultImage = nil;
    //Processing here !
    cv::Mat sourceMat = [sourceImage cvMatFromUIImage];
    cv::Mat desMat = hogVisualizationCalculator(sourceMat);
    //cv::Mat greyMat;
    //cv::cvtColor(sourceMat, greyMat, CV_BGR2GRAY);
    resultImage = [UIImage UIImageFromCVMat:desMat];
    return resultImage;
}

+ (NSString *) getExtractFeatureString:(UIImage *) sourceImage {
    NSMutableString *resultString = [[NSMutableString alloc] init];
    cv::Mat sourceMat = [sourceImage cvMatFromUIImage];
    std::vector<double> listValue = extractFeatureCalculator(sourceMat);
    [resultString appendString:@"{features:["];
    for (int i = 0; i < listValue.size(); i++) {
        double currentValue = listValue.at(i);
        [resultString appendString:[NSString stringWithFormat:@"%f",currentValue]];
        if (i+1 < listValue.size()) {
            [resultString appendString:@","];
        }
    }
    [resultString appendString:@"]}"];
    return resultString;
}
@end
