//
//  UIImage+OpenCV.h
//  City Pathfinder
//
//  Created by Dinh Le on 03/09/15.
//  Copyright © 2015 Dinh Le. All rights reserved.
//

#import <UIKit/UIKit.h>
#include <opencv2/opencv.hpp>
using namespace cv;

@interface UIImage (OpenCV)
- (cv::Mat)cvMatFromUIImage;
+ (UIImage *)UIImageFromCVMat:(cv::Mat)cvMat;
@end
