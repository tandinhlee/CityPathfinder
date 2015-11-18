//
//  hogcalculator.hpp
//  City Pathfinder
//
//  Created by Dinh Le on 16/10/15.
//  Copyright © 2015 Dinh Le. All rights reserved.
//

#ifndef hogcalculator_hpp
#define hogcalculator_hpp

#include <stdio.h>
#include "hogcalculator.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>
#include <vector>

cv::Mat histogramRGBCalculator(cv::Mat src);
cv::Mat hogVisualizationCalculator(cv::Mat src);

#endif /* hogcalculator_hpp */
