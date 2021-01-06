#ifndef _DLLENTRYEXITDATACONTROL_H_
#define _DLLENTRYEXITDATACONTROL_H_
//============================================
// 入退室状況印字制御ヘッダーファイル
// DllEntryExitDataControl.h
// 
// 機能概要
// フロアのセキュリティによるドアの入退室ファイルを読み込み、
// テキストもしくは帳票形式で印字する
// 当機能で使用する定数及びAPIのプロトタイプを宣言する
//
// 定義内容
// １．インクルード定義
// ２．外部参照定義
// ３．プロトタイプ宣言
//
// 作成履歴
// 2020/9/25  s-saito   新規作成
// 
// 更新履歴
// 
// 
// バージョン
//                         ver.1.0
// 補足事項
// ・当ヘッダー及びソース情報については入退室状況印字機能で
// 　使用する情報のみとする
// 
//============================================
//☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★
// 各定数・プロトタイプ定義
//☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★
// ◆インクルード定義
#pragma once
#include "CRecordOperations.h"
#include "DllCommonOperation.h"

// ◆外部参照変数
extern ENTRYEXITDATALIBRARY_API int g_iParseArgv;
extern ENTRYEXITDATALIBRARY_API vector<string> g_vecParseArgc;

// ◆プロトタイプ宣言
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
// 入退室状況印字制御メイン処理
ENTRYEXITDATALIBRARY_API int DataControlMain(int, vector<string>);
#ifdef __cplusplus
}
#endif // __cplusplus

// 入退室状況印字制御開始処理
int DataControlStartProc(int, vector<string>, FILE **, FILE **, int*);

// 入退室状況印字制御終了処理
int DataControlEndProc(FILE **, FILE **, vector<string>, int);

// CRTメモリ検知イベントハンドラ処理
int __cdecl MemoryLeakDookProc(int, char *, int *);
#endif // _DLLENTRYEXITDATACONTROL_H_
