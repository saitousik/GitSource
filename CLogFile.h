#ifndef _CLOGFILE_H_
#define _CLOGFILE_H_
//============================================
// ログ出力クラスヘッダーファイル
// CLogFile.h
// 
// 機能概要
// ログを出力する情報を定義する
// ここでは使用する定数及びAPIのプロトタイプを宣言する
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
#include "DllCommonOperation.h"

// ◆クラス定義
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
class CLogFile
{
// Publicメンバー変数
public:
    // トレースログ出力ファイル名変数
    string m_strTraceLogFileName;

    // データログ出力ファイル名変数
    string m_strDataLogFileName;

    // 日時情報構造体変数
    TIMEDATA m_stTimeData{};

    // ログ出力情報構造体変数
    LOGINFORMATION m_stLogData;

    // ログ出力データ変数
    char m_cLogData[LOGDATA]{};

    // エラーコード変数
    ERRORCODETABLE m_eErrorResultCode{};

    // イベントメッセージ管理テーブル変数
    map<int, string> m_maErrorActionName;

// Publicメンバー関数
public:
	// デフォルトコンストラクタ
    COMMONLIBRARY_API CLogFile();

    // パラメータ情報指定コンストラクタ
    COMMONLIBRARY_API CLogFile(const string, const string);

	// デストラクタ
    COMMONLIBRARY_API virtual ~CLogFile();

    // マシン日時取得処理（オーバーライド）
    COMMONLIBRARY_API virtual TIMEDATA GetDateTimeProc(void);

	// ログ出力制御処理
	void LogOutputControlProc(
        const char *, 
        const char *, 
        int, 
        int, 
        int, 
        const char *, 
        ERRORCODETABLE eErrCd = tagErrorCode::SUCCESS);

// Privateメンバー関数
private:
	// トレースログ出力処理
	void WriteTraceLogProc(
        const char *, 
        const char *, 
        int, 
        const char *, 
        const char *, 
        ERRORCODETABLE eErrCd = tagErrorCode::SUCCESS);

	// データログ出力処理
	void WriteDataLogProc(const char *, const char *, int, const char *);
};
#ifdef __cplusplus
}
#endif // __cplusplus
#endif // _CLOGFILE_H_
