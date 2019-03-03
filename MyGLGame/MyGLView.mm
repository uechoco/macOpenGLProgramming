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
    NSWindow            *window;
    NSSize              size;
    NSRect              viewportRect;
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
        NSOpenGLPFAStencilSize, 8,
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

    // OpenGLとGLSLのバージョンを確認する
    // OpenGL Version: 4.1 INTEL-10.36.19
    // GLSL Version: 4.10
    const GLubyte *glVersion = glGetString(GL_VERSION);
    const GLubyte *glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
    printf("OpenGL Version: %s\n", glVersion);
    printf("GLSL Version: %s\n", glslVersion);

    // テクスチャのユニット個数と最大サイズを確認する
    // Max Texture Units(シェーダ上でのテクスチャの同時利用可能枚数): 16
    // Max Texture Size (width/height)(テクスチャの最大横幅と縦幅): 16384
    GLint maxTextureUnits, maxTextureSize;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
    printf("Max Texture Units: %d\n", maxTextureUnits);
    printf("Max Texture Size (width/height): %d\n", maxTextureSize);

    // インデックス・リスト関係の情報を確認する
    // Max IndexList Vertices(頂点数): 1048575
    // Max IndexList Count(インデックス数): 150000
    GLint maxIndexListVertices, maxIndexListCount;
    glGetIntegerv(GL_MAX_ELEMENTS_VERTICES, &maxIndexListVertices);
    glGetIntegerv(GL_MAX_ELEMENTS_INDICES, &maxIndexListCount);
    printf("Max IndexList Vertices: %d\n", maxIndexListVertices);
    printf("Max IndexList Count: %d\n", maxIndexListCount);

    window = self.window;
    size = self.bounds.size;
    [window makeFirstResponder:self];

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

- (void)resetViewportSize
{
    NSSize frameSize = [self frame].size;
    frameSize = [self convertSizeToBacking:frameSize];
    const float kBaseAspect = 4.0f / 3.0f;
    if (frameSize.width / frameSize.height > kBaseAspect) {
        int width = int(frameSize.height * kBaseAspect);
        viewportRect.origin.x = (frameSize.width - width) / 2;
        viewportRect.origin.y = 0;
        viewportRect.size.width = width;
        viewportRect.size.height = frameSize.height;
    } else {
        int height = int(frameSize.width / kBaseAspect);
        viewportRect.origin.x = 0;
        viewportRect.origin.y = (frameSize.height - height) / 2;
        viewportRect.size.width = frameSize.width;
        viewportRect.size.height = height;
    }
    size = self.bounds.size;
}

- (void)pauseDisplayLink
{
    CVDisplayLinkStop(displayLink);
}

- (void)restartDisplayLink
{
    CVDisplayLinkStart(displayLink);
}

- (GLKVector2)mousePosition
{
    NSPoint location = [NSEvent mouseLocation];
    NSRect rect = NSMakeRect(location.x, location.y, 0.0f, 0.0f);
    rect = [window convertRectFromScreen:rect];
    location = rect.origin;
    location.x = location.x * 2 / size.width - 1.0f;
    location.y = location.y * 2 / size.height - 1.0f;
    return GLKVector2Make(location.x, location.y);
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

    glViewport(viewportRect.origin.x, viewportRect.origin.y, viewportRect.size.width, viewportRect.size.height);
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
