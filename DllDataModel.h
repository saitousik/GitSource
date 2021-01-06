#ifndef _DLLDATAMODEL_H_
#define _DLLDATAMODEL_H_
//============================================
// 構造体等定義ヘッダーファイル
// DllDataModel.h
// 
// 機能概要
// 各機能で使用する構造体等のデータモデル定義
// 
// 
// 定義内容
// １．インクルード定義
// ２．名前空間指定
// ３．日時情報桁数定義
// ４．その他桁数・定数定義
// ５．共通マクロ定義
// ６．列挙型宣言
// ７．構造体宣言
//
// 作成履歴
// 2020/7/25  s-saito   新規作成
// 
// 更新履歴
// 
// 
// 
// バージョン
//                         ver.1.0
// 補足事項
// ・当ヘッダー情報については製造する機能に対して
// 　共通として扱う定義を記載する
// 
// 
//============================================
//☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★
// 各列挙型・構造体定義
//☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★
// ◆インクルード定義
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <crtdbg.h>
#include <vector>
#include <deque>
#include <sstream>
#include <map>

// ◆名前空間指定
using namespace std;

// ◆日時情報桁数定義
#define TIMEDATA_YEAR 4
#define TIMEDATA_MONTH 2
#define TIMEDATA_DAY 2
#define TIMEDATA_DAYOFWEEK 3
#define TIMEDATA_HOUR 2
#define TIMEDATA_MINITE 2
#define TIMEDATA_SECOND 2
#define TIMEDATA_MSECOND 6

// ◆その他桁数・定数定義
#define LOGDATA 2048
#define DATASIZE 1024
#define MESSAGESIZE 1024
#define FILENAMESIZE _MAX_PATH + FILENAME_MAX

// ◆共通マクロ定義
#define SAFE_FREE(pMemory) free(pMemory); pMemory = NULL;
#define SAFE_FREE_DEBUG(pMemory) _freea_dbg(pMemory, _NORMAL_BLOCK); pMemory = NULL;
#define SAFE_FREE_NEWALLOC(pMemory) delete pMemory; pMemory = NULL;
#define SAFE_FREE_NEWALLOC_ARRAY(pMemory) delete[] pMemory; pMemory = NULL;

// ◆列挙型宣言
// エラーコード
typedef enum class tagErrorCode
{
    SUCCESS = 0,									// 正常終了
    ENVERROR = 101,									// 環境変数エラー
    NOPARAM,										// パラメータ指定なし
    FAILPARAM,										// パラメータ不正
    FILEOPENERROR,									// ファイルオープンエラー
    FILECLOSEERROR,									// ファイルクローズエラー
    NODATA,											// データなし
    DATAFRAUD,										// データ不正
    DATAERROR,										// データ取得エラー
    MEMORYERROR,									// メモリ領域確保エラー
    COPYERROR,										// ファイルコピーエラー
    RENAMEERROR,									// ファイル名変換エラー
    DELETEERROR,									// 削除エラー
    STATERROR,										// 状態取得エラー
    FAILERROR										// その他エラー
}ERRORCODETABLE;

// エラー戻り値
enum
{
    CM_ENVERROR = 101,								// 環境変数エラー
    CM_NOPARAM,										// パラメータ指定なし
    CM_PARAMERROR,									// パラメータ不正
    CM_FILEOPENERROR,								// ファイルオープンエラー
    CM_FILECLOSEERROR,								// ファイルクローズエラー
    CM_NODATA,										// データなし
    CM_DATAFRAUD,									// データ不正
    CM_DATAERROR,									// データ取得エラー
    CM_MEMORYERROR,									// メモリ領域確保エラー
    CM_FILECOPYERROR,								// ファイルコピーエラー
    CM_RENAMEERROR,									// ファイル名変換エラー
    CM_FILEDELETEERROR,								// 削除エラー
    CM_FILESTATERROR,								// 状態取得エラー
    CM_FAILERROR									// その他エラー
};

// ファイル名変換タイプ
typedef enum class tagFileType
{
    NORMAL,											// 初期値
    FILETYPE1,										// ディレクトリ名指定あり且つファイル名変更なし（実行フォルダと同階層）
    FILETYPE2,										// ディレクトリ名指定あり且つファイル名変更あり（実行フォルダと同階層）
    FILETYPE3,										// ディレクトリ名指定あり且つファイル名変更あり（実行フォルダの１階層上）
    FILETYPE4,										// ディレクトリ名指定あり且つファイル名変更あり（実行フォルダの１階層上）
    FILETYPE5,										// ディレクトリ名指定あり且つファイル名変更なし（実行フォルダの１階層上）
    FILETYPE6,										// ディレクトリ名指定なし且つファイル名変更あり
    FILETYPE7,										// ディレクトリ名指定なし且つファイル名変更なし（ベース絶対パス設定）
    FILETYPE8,										// ディレクトリ名指定なし且つファイル名変更なし（コマンド情報設定）
    FILETYPE9										// ディレクトリ名指定あり且つファイル名変更あり（実行フォルダと同階層）
}FILETYPETABLE;

// 出力ログ識別子
enum
{
    TRACEOUTPUTLOG = 1,								// トレースログ
	DATAOUTPUTLOG									// データログ
};

// トレースログ識別子
enum
{
    NOTRACE,										// データログ情報
    DATAINFORMATION,								// データ情報
    ERRORINFORMATION1,								// エラー情報
    ERRORINFORMATION2,								// エラーメッセージ情報
    SUCCESSINFORMATION,								// 成功情報
    NORMALINFORMATION								// インフォメーション
};

// ◆構造体宣言
// ログ出力情報構造体
typedef struct tagLogInfo
{
    string strDateTime;                                         // ログ出力日時
    string strActionName;				                        // 処理名称／ファイル名称
    char *pcActionName = NULL;				                    // 処理名称／ファイル名称ポインタ
    string strMessage;						                    // ログ出力メッセージ
    char *pcMessage = NULL;						                // ログ出力メッセージポインタ
	ERRORCODETABLE eErrorCode = tagErrorCode::SUCCESS;          // エラーコード
}LOGINFORMATION;

// 日時情報構造体
typedef struct tagTimeData
{
    string strMachDate;				                 // マシン日付
    string strMachTime;				                 // マシン時刻
    string strMachDateTime;		                     // マシン日時（ログ出力用）
    string strMachDatePlus;		                     // マシン日付（記号付き）
    string strMachTimePlus;		                     // マシン時刻（記号付き）
}TIMEDATA;
#endif // _DLLDATAMODEL_H_
