#ifndef _DLLCOMMONOPERATION_H_
#define _DLLCOMMONOPERATION_H_
//============================================
// 共通操作機能ヘッダーファイル
// DllCommonOperation.h
// 
// 機能概要
// 機能をサポートする情報を定義する
// ここでは使用する定数及びメッセージ等を定義する
//
// 定義内容
// １．インクルード定義
// ２．定数定義
// ３．環境変数定義
// ４．メッセージ定義
// ５．メモリリーク定義
// ６．メモリリークの発生場所定義
// ７．外部へのライブラリ呼び出し定義
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
#include "DllDataModel.h"
#include <time.h>
#include <errno.h>
#include <direct.h>
#include <locale.h>

// ◆定数定義
#define CREATE_DATE_NAME 1
#define CREATE_TIME_NAME 2
#define LOG_TYPE_INFORMATION "Information"
#define LOG_TYPE_DATA "Data"
#define LOG_TYPE_ERROR "Error"
#define LOG_TYPE_SUCCESS "Success"
#define LOGARGS __FILE__, __FUNCTION__, __LINE__
#define PATH_SIZE 10
#define FILEEXTENSION_SIZE 6
#define APINAME_SIZE 64
#define LOG_TYPE_SIZE 14
#define PATHCHECK_SIZE_2BYTE 2
#define PATHCHECK_SIZE_3BYTE 3
#define DATA_SIGN 1
#define ERR_OFF 0
#define ERR_ON 1
#define CRLF_CODE 0x0D0A
#define DELIMITER "."
#define ACTIONSET 1
#define ERRORSET 2

// ◆メッセージ定義
// ■異常メッセージ
#define ENVGETERROR "環境変数から情報を取得出来ません。環境変数を設定しているかご確認お願い致します。"
#define NOTPARAMETER "パラメータが指定されておりません。読み込みファイル名は必須です。"
#define PARAMETERERROR "パラメータ指定が不正です。指定内容をご確認お願い致します。"
#define FILECANNOTOPEN "ファイルをオープン出来ません。ファイル格納場所指定のご確認お願い致します。"
#define FILECANNOTCLOSE "ファイルをクローズ出来ません。"
#define NOTDATA "データがありません。入力データのあるファイルをご指定してください。"
#define DATAINJUSTICE "データが不正のため出力処理は出来ません。このレコードは印字対象外になります。"
#define DATAGETERROR "情報取得に失敗しました。データが取得出来なかったと思われます。"
#define MEMORYENSUREERROR "メモリ領域を確保出来ません。"
#define FILECOPYERROR "ファイルのコピーに失敗しました。何らかの原因が発生したと思われます。"
#define FILERENAMEERROR "ファイル名を変換出来ません。"
#define FILEDELETEERROR "ファイルを削除出来ません。ファイルの状態をご確認お願い致します。"
#define FILESTATERROR "ファイル状態を取得出来ません。該当ファイルが移動もしくは削除されている可能性があります。"
#define OPERATIONERROR "操作に失敗しました。"

// ■正常メッセージ
#define ALLREADEND "全てのデータ取得が終わりました。"
#define MAKEDIRSUCCESS "フォルダを生成しました。"
#define ALREADYEXISTS "フォルダは既に生成済みのため引き続きそのフォルダを使用します。"
#define MEMORYCREATESUCCESS "メモリ領域を確保出来ました。"
#define MEMORYRELEASESUCCESS "メモリ領域を解放しました。"
#define FILEOPENSUCCESS "ファイルがオープン出来ました。"
#define FILECLOSESUCCESS "ファイルがクローズ出来ました。"
#define FULLPATHDEFINITION "ファイル名を絶対パスで指定しているため編集せずにそのまま使用します。"
#define GETSTATSUCCESS "ファイルの状態が取得出来ました。"
#define FILEREADSUCCESS "ファイルからの読み込みが出来ました。"
#define FILEWRITESUCCESS "ファイルへの書き込みが出来ました。"
#define FILECOPYSUCCESS "ファイルのコピーが完了しました。"
#define RENAMESUCCESS "ファイル名に変更出来ました。"
#define DELETESUCCESS "ファイルを削除しました。"
#define DATACONTINUED "読み込んだデータが不正でしたが処理を継続します。"
#define ERRORPROCCONTINUED "関数での処理は失敗しましたが機能に対する直接の影響はないため処理を継続します。"
#define GETENVSUCCESS "環境変数が取得出来ましたので環境変数のファイル名で処理を進めます。"
#define GETNOTENVCONTINUED "環境変数が取得出来ませんでしたので機能で定めているファイル名で処理を進めます。"
#define GETPARAMSUCCESS "パラメータのコマンド情報が取得出来ましたのでコマンド情報のファイル名で処理を進めます。"

// ◆メモリリーク定義
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#endif // _DEBUG

// ◆メモリ確保定義
#ifdef _DEBUG
#define DBG_NEW new (_NORMAL_BLOCK, __FILE__, __LINE__)
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif // _DEBUG
#ifdef _DEBUG
#define malloc(MMEMORY) _malloc_dbg(MMEMORY, _NORMAL_BLOCK, __FILE__, __LINE__)
#define calloc(MCOUNT, CMEMORY) _calloc_dbg(MCOUNT, CMEMORY, _NORMAL_BLOCK, __FILE__, __LINE__)
#endif // _DEBUG

// ◆外部へのライブラリ呼び出し定義
#ifndef OperationsSupport_EXPORTS
#define COMMONLIBRARY_API __declspec(dllimport)
#else
#define COMMONLIBRARY_API __declspec(dllexport)
#endif // OperationsSupport_EXPORTS
#endif // _DLLCOMMONOPERATION_H_
