//
//  CVWrapper.h
//  City Pathfinder
//
//  Created by Dinh Le on 03/09/15.
//  Copyright © 2015 Dinh Le. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@interface CVWrapper : NSObject
+(UIImage *) getHistogramRGB:(UIImage *)sourceImage;
+(UIImage *) getHOGVisualization:(UIImage *)sourceImage;
+ (NSString *) getExtractFeatureString:(UIImage *) sourceImage;
@end
