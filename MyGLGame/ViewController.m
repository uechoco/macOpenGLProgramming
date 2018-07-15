//
//  ViewController.m
//  MyGLGame
//
//  Created by Yusuke Ueno on 2018/07/15.
//  Copyright © 2018年 uechoco. All rights reserved.
//

#import "ViewController.h"
#import "MyGLView.h"

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];

    // ウィンドウのリサイズに合わせて横方向にも縦方向にも伸縮するビュー
    NSRect viewFrame = [self.view frame];
    MyGLView* glView = [[MyGLView alloc] initWithFrame:viewFrame];
    glView.translatesAutoresizingMaskIntoConstraints = YES;
    glView.autoresizingMask = NSViewWidthSizable | NSViewHeightSizable;
    [self.view addSubview:glView];
    
}


- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];

    // Update the view, if already loaded.
}


@end
