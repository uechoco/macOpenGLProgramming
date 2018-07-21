//
//  MyGLView.m
//  MyGLGame
//
//  Created by Yusuke Ueno on 2018/07/15.
//  Copyright © 2018年 uechoco. All rights reserved.
//

#import "MyGLView.h"
#include "Game.hpp"
#include "Time.hpp"
#include "Input.hpp"
#import <OpenGL/OpenGL.h>
#import <OpenGL/gl3.h>

static MyGLView* instance = nil;

@implementation MyGLView {
    NSOpenGLContext* glContext;
    CVDisplayLinkRef displayLink;
    bool hasDisplayLinkStopped;
    Game *pGame;
}

+ (MyGLView *)sharedInstance
{
    return instance;
}

static CVReturn DisplayLinkCallback(CVDisplayLinkRef displayLink,
                                    const CVTimeStamp* now,
                                    const CVTimeStamp* outputTime,
                                    CVOptionFlags flagsIn,
                                    CVOptionFlags* flagsOut,
                                    void* displayLinkContext)
{
    @autoreleasepool {
        MyGLView* glView = (__bridge MyGLView*)displayLinkContext;
        [glView render];
        return kCVReturnSuccess;
    }
}

- (instancetype)initWithFrame:(NSRect)frame
{
    NSOpenGLPixelFormatAttribute attrs[] = {
        NSOpenGLPFAAllowOfflineRenderers,
        NSOpenGLPFAAccelerated,
        NSOpenGLPFADoubleBuffer,
        NSOpenGLPFAColorSize, 32,
        NSOpenGLPFADepthSize, 32,
        NSOpenGLPFAMultisample,
        NSOpenGLPFASampleBuffers, 1,
        NSOpenGLPFASamples, 4,
        NSOpenGLPFANoRecovery,
        NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion3_2Core,
        0
    };
    NSOpenGLPixelFormat* pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];
    
    self = [super initWithFrame:frame pixelFormat:pixelFormat];
    if (self)
    {
        instance = self;
        [self setWantsBestResolutionOpenGLSurface:YES];
    }
    return self;
}

- (void)prepareOpenGL
{
    [super prepareOpenGL];

    [self.window makeFirstResponder:self];

    Time::Start();
    
    glContext = [self openGLContext];

    pGame = new Game();


    [glContext flushBuffer];

    // 垂直同期を使用するためには以下2行のコメントアウトを外す
    //GLint swapInt = 1;
    //[glContext setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];


    // OpenGLのコンテキストは、メインスレッド上でしか取得してはいけないため、
    // prepareOpenGLメソッドで取得して保存しておかないと、
    // 後々ディスプレイ・リンクからのコールバックがあった時点で取得しようとすると
    // エラーが出る環境がある
    CGLContextObj cglContext = [[self openGLContext] CGLContextObj];
    CGLPixelFormatObj cglPixelFormat = [[self pixelFormat] CGLPixelFormatObj];
    CVDisplayLinkCreateWithActiveCGDisplays(&displayLink);
    CVDisplayLinkSetOutputCallback(displayLink, &DisplayLinkCallback, (__bridge void*)(self));
    CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(displayLink, cglContext, cglPixelFormat);
    CVDisplayLinkStart(displayLink);
}

- (void)stopDisplayLink
{
    CVDisplayLinkStop(displayLink);
    CVDisplayLinkRelease(displayLink);
}

- (void)drawRect:(NSRect)dirtyRect {
    [super drawRect:dirtyRect];
    
    // Drawing code here.
}

- (void)render
{
    Input::Update();

    [glContext lock];
    [glContext makeCurrentContext];

    pGame->Render();

    [glContext flushBuffer];
    [glContext unlock];

    // 経過時間の更新
    Time::Update();

    // ウィンドウのタイトルを設定するようなGUI操作のコードは、
    // 基本的にメインスレッド上で実行する必要がある
    [[NSOperationQueue mainQueue] addOperationWithBlock:^{
        [self.window setTitle:[NSString stringWithFormat:@"Game (%.2f fps)", Time::fps]];
    }];
}

@end
