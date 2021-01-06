#ifndef _DLLENTRYEXITDATADEFINESTRUCT_H_
#define _DLLENTRYEXITDATADEFINESTRUCT_H_
//============================================
// 入退室状況印字定数・構造体・共用体定義ヘッダーファイル
// DllEntryExitDataDefineStruct.h
// 
// 機能概要
// 入退室状況印字機能で使用する定数及び構造体・共用体を提供する
//
// 定義内容
// １．インクルード定義
// ２．レコード桁数定義
// ３．ディレクトリ名定数定義
// ４．その他定数定義
// ５．メッセージ定義
// ６．列挙型宣言
// ７．構造体・共用体宣言
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
#pragma once
// ◆外部参照定義
#ifdef DLLENTRYEXITDATAOUTPUT_EXPORTS
#define ENTRYEXITDATALIBRARY_API __declspec(dllexport)
#else
#define ENTRYEXITDATALIBRARY_API __declspec(dllimport)
#endif // DLLENTRYEXITDATAOUTPUT_EXPORTS

// ◆ライブラリ呼び出し
// DLLファイルのみの場合はLoadModule関数・LoadLibrary関数でライブラリを呼び出す。
#ifdef NDEBUG
#ifdef WIN64
#pragma comment(lib, "x64\\Release\\OperationsSupport.lib")
#else
#pragma comment(lib, "Release\\OperationsSupport.lib")
#endif // WIN64
#else
#ifdef WIN64
#pragma comment(lib, "x64\\Debug\\OperationsSupport.lib")
#else
#pragma comment(lib, "Debug\\OperationsSupport.lib")
#endif // WIN64
#endif // NDEBUG

// ◆レコード桁数定義
#define RECORD_CARDID 4
#define RECORD_DATE 8
#define RECORD_TIME 6
#define RECORD_DOOR 2
#define RECORD_DIR 1
#define RECORD_ACT 1
#define RECORD_NAME 10

// ◆ディレクトリ名・ファイル名定数定義
#define READPATH "Data\\"
#define WRITEPATH "Output\\"
#define LOGFILEPATH "Log\\"
#define COPYPATH "Backup\\"
#define DEFAULTFILENAME "AccessOutput.txt"
#define LOGFILENAME "TraceInfo.log"
#define DATALOGFILENAME "DataInfo.log"
#define FRAUDDATAFILENAME "FraudDataInfo.txt"

// ◆環境変数定義
#define READ_FILE "READFILE"
#define WRITE_FILE "WRITEFILE"

// ◆その他定数定義
#define TITLE "ID    名前        レベル１             レベル２             レベル３             "
#define NYUSHITSU "I"
#define TAISHITSU "O"
#define KYOHI "R"
#define READDATA 33
#define FLOORMAX 3
#define FILEEND_OFF 0
#define FILEEND_ON 1

// ◆メッセージ定義
// ■異常メッセージ
#define INITFAILED "開始処理に失敗しました。"
#define ENDFAILED "終了処理に失敗しました。"

// ■正常メッセージ
#define READEND "入退室読み込みデータファイルの読み込み終わりました。"
#define GETRECORDSUCCESS "入退室レコードが取得出来ました。"
#define INITSUCCESS "開始処理が完了しました。"
#define ENDSUCCESS "終了処理が完了しました。"

// ◆列挙型宣言
// データ出力タイプ
typedef enum struct tagPrintType
{
	DUMMY,											// 初期値
	DOOR,											// ドア番号
	CARDID,											// カードID及びユーザー名
	ACT,											// 可否情報
	LEVELNG1,										// 制限エリアレベルNG情報１
	LEVELNG2,										// 制限エリアレベルNG情報２
	LEVELNG3,										// 制限エリアレベルNG情報３
	LEVELNG4,										// 制限エリアレベルNG情報４
	LEVELNG5,										// 制限エリアレベルNG情報５
	LEVELNG6,										// 制限エリアレベルNG情報６
	DUMMYDATA										// ダミーデータ
}PRINTTYPETABLE;

// データ出力先識別子
enum
{
	FILEPRINT = 1,									// 入退室書き込みデータファイルのみ出力
	SCREENPRINT,									// PC閲覧画面のみ出力
	BOTHPRINT										// 両方に出力
};

// ◆構造体宣言
// ファイル名情報管理構造体
typedef struct tagFileNameInfo
{
	string strReadName;				                // 入退室読み込みデータファイル名
	string strWriteName;				            // 入退室書き込みデータファイル名
	string strTraceLogName;				            // トレースログ出力ファイル名
	string strDataLogName;			                // データログ出力ファイル名
	string strBackupName;			                // バックアップファイル名
	string strChangeName;				            // 名称変更対象ファイル名
	string strDeleteName;				            // 削除対象ファイル名
	string strModuleName;				            // 実行アプリケーション名
	string strFraudDataName;						// 不正データファイル名
}FILENAMEINFORMATION;

// 入退室情報レコード
typedef struct tagEntryExitRecordInfo
{
	char cCardID[RECORD_CARDID];					// カードID
	char cDate[RECORD_DATE];						// 日付
	char cTime[RECORD_TIME];						// 時刻
	char cDoor[RECORD_DOOR];						// ドア番号
	char cDir[RECORD_DIR];							// 入退出情報
	char cAct[RECORD_ACT];							// 可否情報
	char cName[RECORD_NAME];						// ユーザー名
}ENTRYEXITRECORDINFO;

// 入退室情報データ編集用構造体
typedef struct tagEntryExitData
{
	char cCardID[RECORD_CARDID + 1];				// カードID
	char cName[RECORD_NAME + 1];					// ユーザー名
	char cDate[RECORD_DATE + 1];					// 日付
	char cTime[RECORD_TIME + 1];					// 時刻
	char cDoor[RECORD_DOOR + 1];					// ドア番号
	char cDir[RECORD_DIR + 1];						// 入退出情報
	char cAct[RECORD_ACT + 1];						// 可否情報
}ENTRYEXITDATA;

// 入退室情報共用体
typedef union tagEntryExitRecord
{
	ENTRYEXITRECORDINFO stEntryExitRecord;			// 入退室データ
	char cEntryExitData[DATASIZE];					// 入退室データ（文字列型）
}ENTRYEXITRECORD;

// ドアレベル情報構造体
typedef struct tagDoorLevelInfo
{
	unsigned int iDoorLevel : 2;					// 通過ドアレベル
	unsigned int iBeforeLevel : 2;					// 通過前ドアレベル
	unsigned int iAfterLevel : 2;					// 通過後ドアレベル
	unsigned int iLevel : 2;						// 現在の通過ドアレベル
}DOORLEVELINFORMATION;

// 入退室総合情報構造体
typedef struct tagEntryExitInfo
{
	ENTRYEXITRECORD unEntryExitRecord;				// 入退室レコード
	deque<ENTRYEXITDATA> deqEntryExitEditData;		// 入退室情報データ編集用構造体（dequeコンテナクラス）
	DOORLEVELINFORMATION stDoorLevelData{};			// ドアレベル情報構造体
	char cLastID[RECORD_CARDID + 1];				// 退避カードID
}ENTRYEXITINFORMATION;
#endif // _DLLENTRYEXITDATADEFINESTRUCT_H_
