#ifndef _CFILEOPERATION_H_
#define _CFILEOPERATION_H_
//============================================
// ファイル操作クラスヘッダーファイル
// CFileOperations.h
// 
// 機能概要
// ファイルを操作する情報を定義する
// ここでは共通操作クラスを継承して使用する定数及びAPIのプロトタイプを宣言する
//
// 定義内容
// １．インクルード定義
// ２．クラス定義
//
// 作成履歴
// 2020/9/25  s-saito   新規作成
// 
// 更新履歴
// 
// 
// 
// バージョン
//                         ver.1.0
// 補足事項
// ・当ヘッダー及びソース情報についてはその他の機能でも
// 　サポートを可能とする
// 
//============================================
//☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★
// 各定数・プロトタイプ定義
//☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★
// ◆インクルード定義
#pragma once
#include "CSubOperations.h"

// ◆クラス定義
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
class CFileOperations : 
    public CSubOperations
{
// Protectedメンバー変数
protected:
    // トレースログ出力ファイル名変数
    string m_strFileOpeTraceLogFileName;

    // データログ出力ファイル名変数
    string m_strFileOpeDataLogFileName;

// Publicメンバー変数
public:
    // コピーファイル名変数
    string m_strFileOpeCopyFileName;

// Publicメンバー関数
public:
    // デフォルトコンストラクタ
    COMMONLIBRARY_API CFileOperations();

    // パラメータ情報指定コンストラクタ
    COMMONLIBRARY_API CFileOperations(const string, const string, const string);

    // デストラクタ
    COMMONLIBRARY_API virtual ~CFileOperations();

    // ファイルコピー処理
    int CopyFileProc(const char *, const char *);

    // ファイル名変換処理
    int ChangeFileNameProc(const char *, const char *);

    // ファイル削除処理
    int DeleteFileProc(const char *);

    // ファイル状態チェック処理
    int CheckFileStatProc(const char *, struct stat *);

    // 絶対パス変換ファイル名取得処理
    string GetConvertFileNameProc(
        FILETYPETABLE,
        const char *,
        const char *,
        char *pcDirectoryName = (char *)"", 
        char *pcDefaultName = (char *)"", 
        int iBaseResult = EXIT_SUCCESS);

// Privateメンバー関数
private:
    // ファイル名生成処理
    string CreateFileNameProc(int, const char *, const char *, int iResult = EXIT_SUCCESS);

    // 絶対パス取得処理
    string GetFullPathProc(const char, const char *, const char *);
};
#ifdef __cplusplus
}
#endif // __cplusplus
#endif // _CFILEOPERATION_H_

