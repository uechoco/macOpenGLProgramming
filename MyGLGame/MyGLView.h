//
//  MyGLView.h
//  MyGLGame
//
//  Created by Yusuke Ueno on 2018/07/15.
//  Copyright © 2018年 uechoco. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <GLKit/GLKit.h>

@interface MyGLView : NSOpenGLView

+ (MyGLView *)sharedInstance;
- (void)stopDisplayLink;
- (GLKVector2)mousePosition;

@end
