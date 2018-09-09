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
#include <vector>
#include <stdexcept>

//! printf()形式で++文字列を作成する(1023文字まで)
std::string FormatString(const char* format, ...);

//! 指定された名前のファイルに対応したパス文字列を取得
std::string GetFilepath(const std::string& filename);

//! 指定された名前のテキストファイルを文字列として読み込む
std::string ReadTextFile(const std::string& filename);

/*! 与えられた文字列strを、separatorを区切り文字として分割します。（空文字挿入のオプションあり） */
std::vector<std::string> Split(const std::string& str, const std::string& separator, bool ignoreEmptyString = false);

/*! 文字列strが、文字列valueで始まる文字列かどうかをチェックします。（大文字・小文字の区別オプションあり） */
bool StartsWith(const std::string& str, const std::string& value, bool ignoreCase = true);

/*! 文字列strが、文字列valueで終わる文字列かどうかをチェックします。（大文字・小文字の区別オプションあり） */
bool EndsWith(const std::string& str, const std::string& value, bool ignoreCase = true);

/*! 文字列の先頭および末尾にある空白文字（タブ文字, 改行文字, 空白文字）をすべて削除します。 */
std::string Trim(const std::string& str);

/*! 文字列の先頭および末尾から、指定された文字セットをすべて削除します。 */
std::string Trim(const std::string& str, const std::string& trimChars);

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
