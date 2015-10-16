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
+(UIImage *) testProcessImage:(UIImage *)sourceImage{
    UIImage *resultImage = nil;
    //Processing here !
    cv::Mat sourceMat = [sourceImage cvMatFromUIImage];
    cv::Mat desMat = mycalculator(sourceMat);
    //cv::Mat greyMat;
    //cv::cvtColor(sourceMat, greyMat, CV_BGR2GRAY);
    resultImage = [UIImage UIImageFromCVMat:desMat];
    return resultImage;
}
@end
