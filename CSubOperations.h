#ifndef _CSUBOPERATIONS_H_
#define _CSUBOPERATIONS_H_
//============================================
// 共通操作クラスヘッダーファイル
// CSubOperations.h
// 
// 機能概要
// アプリケーション等を実行するための必要な機能を定義する
// ここではログ出力クラスを継承して使用する定数及びAPIのプロトタイプを宣言する
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
// 各プロトタイプ定義
//☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★
// ◆インクルード定義
#pragma once
#include "CLogFile.h"

// ◆クラス定義
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
class CSubOperations : 
    public CLogFile
{
// Protectedメンバー変数
protected:
    // トレースログ出力ファイル名変数
    string m_strSubOpeTraceLogFileName;

    // データログ出力ファイル名変数
    string m_strSubOpeDataLogFileName;

// Publicメンバー関数
public:
    // デフォルトコンストラクタ
    COMMONLIBRARY_API CSubOperations();

    // パラメータ情報指定コンストラクタ
    COMMONLIBRARY_API CSubOperations(const string, const string);

    // デストラクタ
    COMMONLIBRARY_API virtual ~CSubOperations();

    // 環境変数取得処理
    int GetEnvironmentDataProc(const char *, char *);

    // マシン日時取得処理
    COMMONLIBRARY_API TIMEDATA GetDateTimeProc(void);

    // イベントメッセージ生成処理
    LOGINFORMATION CreateEventMessageProc(int, const char *, ERRORCODETABLE eErr = tagErrorCode::SUCCESS);

    // 指定文字削除処理
    void DeleteDesignationCharProc(const char *, const char, char *);

    // 指定文字列分割処理
    deque<string> SplitDataProc(const string, const char);

    // マルチバイト文字列ワイド文字列変換処理
    COMMONLIBRARY_API wstring MultiToWideProc(string);

    // ワイド文字列マルチバイト文字列変換処理
    COMMONLIBRARY_API string WideToMultiProc(wstring);

    // コマンドライン引数解析処理
    COMMONLIBRARY_API vector<string> ParseArgsProc(string, int*);

// Privateメンバー関数
private:
    // エラーメッセージ取得処理
    const char *GetErrorMessageProc(ERRORCODETABLE);
};
#ifdef __cplusplus
}
#endif // __cplusplus
#endif // _CSUBOPERATIONS_H_

