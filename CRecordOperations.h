#ifndef _CRECORDOPERATIONS_H_
#define _CRECORDOPERATIONS_H_
//============================================
// 入退室レコード操作クラスヘッダーファイル
// CRecordOperations.h
// 
// 機能概要
// 入退室状況印字に対するデータ編集及び出力情報を定義する
// ここでは使用する定数及びAPIのプロトタイプを宣言する
//
// 定義内容
// １．インクルード定義
// ２．ライブラリ呼び出し
// ３．クラス定義
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
// ・当ヘッダー及びソース情報については入退室状況印字機能で
// 　使用する情報のみとする
// 
//============================================
//☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★
// 各定数・プロトタイプ定義
//☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★
//◆ インクルード定義
#pragma once
#include "CFileOperations.h"
#include "DllEntryExitDataDefineStruct.h"

//◆ 外部参照変数定義
extern FILENAMEINFORMATION g_stFileNameInfo;

// ◆クラス定義
class CRecordOperations : 
	public CFileOperations
{
// Privateメンバー変数
private:
	// トレースログ出力ファイル名変数
	string m_strRecOpeTraceLogFileName;

	// データログ出力ファイル名変数
	string m_strRecOpeDataLogFileName;

// Publicメンバー変数
public:
	// 現在のドアレベル
	int m_iNowDoorLevel = 0;

	// STAT構造体変数
	struct stat m_stStat;

	// 入退室総合情報構造体変数
	ENTRYEXITINFORMATION m_stEntryExitInfo;

// Publicメンバー関数
public:
	// デフォルトコンストラクタ
	CRecordOperations();

	// パラメータ情報指定コンストラクタ
	CRecordOperations(const string, const string);

	// デストラクタ
	~CRecordOperations();

	// 入退室データ編集情報取得処理
	ENTRYEXITDATA *GetEditDataProc(FILE *, FILE *, bool *, int *, int *, long *);

	// 入退室データ編集情報出力指示処理
	void PutEditDataProc(FILE *, ENTRYEXITDATA *, DOORLEVELINFORMATION *);

	// 制限エリアレベルクリア処理
	void ClearLevelProc(FILE *, const ENTRYEXITDATA, const int, DOORLEVELINFORMATION *);

	// 入退室状況印字見出し設定処理
	void SetEntryExitDataTitleProc(FILE *, ERRORCODETABLE eResult = tagErrorCode::SUCCESS);

// Privateメンバー関数
private:
	// 入退室データ編集情報生成処理
	ENTRYEXITDATA CreateEditDataProc(const ENTRYEXITRECORD);

	// 制限エリアレベルチェック処理
	int CheckLevelProc(const ENTRYEXITDATA, DOORLEVELINFORMATION *);

	// 入退室データ編集情報出力制御処理
	void PrintDataControlProc(
		int,
		FILE *,
		const char *,
		const char *,
		const ENTRYEXITDATA,
		const char *,
		PRINTTYPETABLE eType = tagPrintType::DUMMY,
		int iLevel1 = 0,
		int iLevel2 = 0);

	// 入退室不正データファイル出力処理
	void PrintFraudDataFileProc(const char *, const char *);

	// 入退室データファイル出力処理
	void PrintReportFileProc(FILE *, const char *, const char *, const ENTRYEXITDATA, const char *, PRINTTYPETABLE, int, int);

	// 入退室データPC閲覧画面出力処理
	void PrintScreenProc(const char *, const char *, const ENTRYEXITDATA, const char *, PRINTTYPETABLE, int, int);
};
#endif // _CRECORDOPERATIONS_H_
