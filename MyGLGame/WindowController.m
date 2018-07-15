//
//  WindowController.m
//  MyGLGame
//
//  Created by Yusuke Ueno on 2018/07/15.
//  Copyright © 2018年 uechoco. All rights reserved.
//

#import "WindowController.h"

@interface WindowController ()

@end

@implementation WindowController

- (void)windowDidLoad {
    [super windowDidLoad];
    
    self.window.delegate = self;
    self.window.frameAutosaveName = @"Main Window";
    [self.window setContentSize:NSMakeSize(640, 480)];
}

- (NSSize)windowWillResize:(NSWindow *)sender toSize:(NSSize)frameSize
{
    if ([self.window styleMask] & NSWindowStyleMaskFullScreen)
    {
        return frameSize;
    }
    else
    {
        NSRect rect = NSMakeRect(0, 0, 640, 480);
        rect = [self.window frameRectForContentRect:rect];
        return rect.size;
    }
}

- (void)windowWillClose:(NSNotification *)notification
{
    [NSApp terminate:self];
}

@end
