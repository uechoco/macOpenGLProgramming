//
//  StringSupport.hpp
//  MyGLGame
//
//  Created by uechoco on 2018/07/15.
//  Copyright © 2018年 uechoco. All rights reserved.
//

#ifndef StringSupport_hpp
#define StringSupport_hpp

#include <string>
#include <stdexcept>

//! printf()形式で++文字列を作成する(1023文字まで)
std::string FormatString(const char* format, ...);

//! 指定された名前のファイルに対応したパス文字列を取得
std::string GetFilepath(const std::string& filename);

//! 指定された名前のテキストファイルを文字列として読み込む
std::string ReadTextFile(const std::string& filename);

//! ゲーム実行中に起きるエラーを表す例外クラス
class GameError : public std::runtime_error
{
public:
    template<typename... T>
    GameError(const char *format, T... args)
    : std::runtime_error(FormatString(format, args...))
    {
    }

    GameError(const std::string& message)
    : std::runtime_error(message)
    {
    }
};

#endif /* StringSupport_hpp */
