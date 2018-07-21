//
//  ViewController.m
//  MyGLGame
//
//  Created by Yusuke Ueno on 2018/07/15.
//  Copyright © 2018年 uechoco. All rights reserved.
//

#import "ViewController.h"
#import "MyGLView.h"
#include "Input.hpp"

@implementation ViewController {
    bool    isShiftOn;
}

- (void)viewDidLoad {
    [super viewDidLoad];

    // ウィンドウのリサイズに合わせて横方向にも縦方向にも伸縮するビュー
    NSRect viewFrame = [self.view frame];
    MyGLView* glView = [[MyGLView alloc] initWithFrame:viewFrame];
    glView.translatesAutoresizingMaskIntoConstraints = YES;
    glView.autoresizingMask = NSViewWidthSizable | NSViewHeightSizable;
    [self.view addSubview:glView];

    isShiftOn = false;
    
}

- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];

    // Update the view, if already loaded.
}

- (void)keyDown:(NSEvent *)theEvent
{
    unsigned short keyCode = [theEvent keyCode];

    if (keyCode == 0x7e) {
        Input::ProcessKeyDown(KeyCode::UpArrow);
    }
    else if (keyCode == 0x7d) {
        Input::ProcessKeyDown(KeyCode::DownArrow);
    }
    else if (keyCode == 0x7b) {
        Input::ProcessKeyDown(KeyCode::LeftArrow);
    }
    else if (keyCode == 0x7c) {
        Input::ProcessKeyDown(KeyCode::RightArrow);
    }
    else if (keyCode == 0x31) {
        Input::ProcessKeyDown(KeyCode::Space);
    }
    else if (keyCode == 0x35) {
        Input::ProcessKeyDown(KeyCode::Escape);
    }
    else if (keyCode == 0x24 || keyCode == 0x4c) {
        Input::ProcessKeyDown(KeyCode::Return);
    }
    else if (keyCode == 0x00) {
        Input::ProcessKeyDown(KeyCode::A);
    }
    else if (keyCode == 0x0b) {
        Input::ProcessKeyDown(KeyCode::B);
    }
    else if (keyCode == 0x08) {
        Input::ProcessKeyDown(KeyCode::C);
    }
    else if (keyCode == 0x02) {
        Input::ProcessKeyDown(KeyCode::D);
    }
    else if (keyCode == 0x0e) {
        Input::ProcessKeyDown(KeyCode::E);
    }
    else if (keyCode == 0x03) {
        Input::ProcessKeyDown(KeyCode::F);
    }
    else if (keyCode == 0x05) {
        Input::ProcessKeyDown(KeyCode::G);
    }
    else if (keyCode == 0x04) {
        Input::ProcessKeyDown(KeyCode::H);
    }
    else if (keyCode == 0x22) {
        Input::ProcessKeyDown(KeyCode::I);
    }
    else if (keyCode == 0x26) {
        Input::ProcessKeyDown(KeyCode::J);
    }
    else if (keyCode == 0x28) {
        Input::ProcessKeyDown(KeyCode::K);
    }
    else if (keyCode == 0x25) {
        Input::ProcessKeyDown(KeyCode::L);
    }
    else if (keyCode == 0x2e) {
        Input::ProcessKeyDown(KeyCode::M);
    }
    else if (keyCode == 0x2d) {
        Input::ProcessKeyDown(KeyCode::N);
    }
    else if (keyCode == 0x1f) {
        Input::ProcessKeyDown(KeyCode::O);
    }
    else if (keyCode == 0x23) {
        Input::ProcessKeyDown(KeyCode::P);
    }
    else if (keyCode == 0x0c) {
        Input::ProcessKeyDown(KeyCode::Q);
    }
    else if (keyCode == 0x0f) {
        Input::ProcessKeyDown(KeyCode::R);
    }
    else if (keyCode == 0x01) {
        Input::ProcessKeyDown(KeyCode::S);
    }
    else if (keyCode == 0x11) {
        Input::ProcessKeyDown(KeyCode::T);
    }
    else if (keyCode == 0x20) {
        Input::ProcessKeyDown(KeyCode::U);
    }
    else if (keyCode == 0x09) {
        Input::ProcessKeyDown(KeyCode::V);
    }
    else if (keyCode == 0x0d) {
        Input::ProcessKeyDown(KeyCode::W);
    }
    else if (keyCode == 0x07) {
        Input::ProcessKeyDown(KeyCode::X);
    }
    else if (keyCode == 0x10) {
        Input::ProcessKeyDown(KeyCode::Y);
    }
    else if (keyCode == 0x06) {
        Input::ProcessKeyDown(KeyCode::Z);
    }
    else if (keyCode == 0x12) {
        Input::ProcessKeyDown(KeyCode::Alpha1);
    }
    else if (keyCode == 0x13) {
        Input::ProcessKeyDown(KeyCode::Alpha2);
    }
    else if (keyCode == 0x14) {
        Input::ProcessKeyDown(KeyCode::Alpha3);
    }
    else if (keyCode == 0x15) {
        Input::ProcessKeyDown(KeyCode::Alpha4);
    }
    else if (keyCode == 0x17) {
        Input::ProcessKeyDown(KeyCode::Alpha5);
    }
    else if (keyCode == 0x16) {
        Input::ProcessKeyDown(KeyCode::Alpha6);
    }
    else if (keyCode == 0x1a) {
        Input::ProcessKeyDown(KeyCode::Alpha7);
    }
    else if (keyCode == 0x1c) {
        Input::ProcessKeyDown(KeyCode::Alpha8);
    }
    else if (keyCode == 0x19) {
        Input::ProcessKeyDown(KeyCode::Alpha9);
    }
    else if (keyCode == 0x1d) {
        Input::ProcessKeyDown(KeyCode::Alpha0);
    }
}

- (void)keyUp:(NSEvent *)theEvent
{
    unsigned short keyCode = [theEvent keyCode];

    if (keyCode == 0x7e) {
        Input::ProcessKeyUp(KeyCode::UpArrow);
    }
    else if (keyCode == 0x7d) {
        Input::ProcessKeyUp(KeyCode::DownArrow);
    }
    else if (keyCode == 0x7b) {
        Input::ProcessKeyUp(KeyCode::LeftArrow);
    }
    else if (keyCode == 0x7c) {
        Input::ProcessKeyUp(KeyCode::RightArrow);
    }
    else if (keyCode == 0x31) {
        Input::ProcessKeyUp(KeyCode::Space);
    }
    else if (keyCode == 0x35) {
        Input::ProcessKeyUp(KeyCode::Escape);
    }
    else if (keyCode == 0x24 || keyCode == 0x4c) {
        Input::ProcessKeyUp(KeyCode::Return);
    }
    else if (keyCode == 0x00) {
        Input::ProcessKeyUp(KeyCode::A);
    }
    else if (keyCode == 0x0b) {
        Input::ProcessKeyUp(KeyCode::B);
    }
    else if (keyCode == 0x08) {
        Input::ProcessKeyUp(KeyCode::C);
    }
    else if (keyCode == 0x02) {
        Input::ProcessKeyUp(KeyCode::D);
    }
    else if (keyCode == 0x0e) {
        Input::ProcessKeyUp(KeyCode::E);
    }
    else if (keyCode == 0x03) {
        Input::ProcessKeyUp(KeyCode::F);
    }
    else if (keyCode == 0x05) {
        Input::ProcessKeyUp(KeyCode::G);
    }
    else if (keyCode == 0x04) {
        Input::ProcessKeyUp(KeyCode::H);
    }
    else if (keyCode == 0x22) {
        Input::ProcessKeyUp(KeyCode::I);
    }
    else if (keyCode == 0x26) {
        Input::ProcessKeyUp(KeyCode::J);
    }
    else if (keyCode == 0x28) {
        Input::ProcessKeyUp(KeyCode::K);
    }
    else if (keyCode == 0x25) {
        Input::ProcessKeyUp(KeyCode::L);
    }
    else if (keyCode == 0x2e) {
        Input::ProcessKeyUp(KeyCode::M);
    }
    else if (keyCode == 0x2d) {
        Input::ProcessKeyUp(KeyCode::N);
    }
    else if (keyCode == 0x1f) {
        Input::ProcessKeyUp(KeyCode::O);
    }
    else if (keyCode == 0x23) {
        Input::ProcessKeyUp(KeyCode::P);
    }
    else if (keyCode == 0x0c) {
        Input::ProcessKeyUp(KeyCode::Q);
    }
    else if (keyCode == 0x0f) {
        Input::ProcessKeyUp(KeyCode::R);
    }
    else if (keyCode == 0x01) {
        Input::ProcessKeyUp(KeyCode::S);
    }
    else if (keyCode == 0x11) {
        Input::ProcessKeyUp(KeyCode::T);
    }
    else if (keyCode == 0x20) {
        Input::ProcessKeyUp(KeyCode::U);
    }
    else if (keyCode == 0x09) {
        Input::ProcessKeyUp(KeyCode::V);
    }
    else if (keyCode == 0x0d) {
        Input::ProcessKeyUp(KeyCode::W);
    }
    else if (keyCode == 0x07) {
        Input::ProcessKeyUp(KeyCode::X);
    }
    else if (keyCode == 0x10) {
        Input::ProcessKeyUp(KeyCode::Y);
    }
    else if (keyCode == 0x06) {
        Input::ProcessKeyUp(KeyCode::Z);
    }
    else if (keyCode == 0x12) {
        Input::ProcessKeyUp(KeyCode::Alpha1);
    }
    else if (keyCode == 0x13) {
        Input::ProcessKeyUp(KeyCode::Alpha2);
    }
    else if (keyCode == 0x14) {
        Input::ProcessKeyUp(KeyCode::Alpha3);
    }
    else if (keyCode == 0x15) {
        Input::ProcessKeyUp(KeyCode::Alpha4);
    }
    else if (keyCode == 0x17) {
        Input::ProcessKeyUp(KeyCode::Alpha5);
    }
    else if (keyCode == 0x16) {
        Input::ProcessKeyUp(KeyCode::Alpha6);
    }
    else if (keyCode == 0x1a) {
        Input::ProcessKeyUp(KeyCode::Alpha7);
    }
    else if (keyCode == 0x1c) {
        Input::ProcessKeyUp(KeyCode::Alpha8);
    }
    else if (keyCode == 0x19) {
        Input::ProcessKeyUp(KeyCode::Alpha9);
    }
    else if (keyCode == 0x1d) {
        Input::ProcessKeyUp(KeyCode::Alpha0);
    }
}

- (void)flagsChanged:(NSEvent *)theEvent
{
    NSUInteger modifierFlags = [theEvent modifierFlags];

    if (modifierFlags & NSEventModifierFlagShift) {
        if (!isShiftOn) {
            Input::ProcessKeyDown(KeyCode::Shift);
            isShiftOn = true;
        }
    } else {
        if (isShiftOn) {
            Input::ProcessKeyUp(KeyCode::Shift);
            isShiftOn = false;
        }
    }
}


@end
