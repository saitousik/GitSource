//============================================
// 入退室レコード操作クラスソースファイル
// CRecordOperations.cpp
// 
// 機能概要
// 入退室レコード読み込み及び入退室レコードチェック、
// PC閲覧画面及び入退室書き込みデータファイルへの印字等データ操作を行う
// 
// 機能内容
// １．入退室データ編集情報取得処理
// ２．入退室データ編集情報出力指示処理
// ３．制限エリアレベルクリア処理
// ４．入退室入退室状況印字見出し設定処理
// ５．入退室データ編集情報生成処理
// ６．制限エリアレベルチェック処理
// ７．入退室データファイル出力処理
// ８．入退室データPC閲覧画面出力処理
// 
// 補足事項
// ・当機能の詳細や状態については共通機能を呼び出しログ情報としてログ出力ファイルに出力する
// ・当機能では共通機能を呼び出し改行コード削除等も行う
// 
// 作成履歴
// 2020/9/25  s-saito   新規作成
// 
// 更新履歴
// 
// 
// バージョン
//                     ver.1.0
//============================================
// セキュア対応
#define _CRT_SECURE_NO_WARNINGS

//☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★
// 各ヘッダーインクルード定義
//☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★
// インクルード定義
#include "CRecordOperations.h"

// クラス定義
CLogFile g_clsRecOpeLogFile;

// マクロ定義
#define LOGOUTPUT(iOutputType, iTraceOutputLogType, cMessageInfo) \
    g_clsRecOpeLogFile.LogOutputControlProc(LOGARGS, iOutputType, iTraceOutputLogType, cMessageInfo)
#define LOGOUTPUTERROR(iOutputType, iTraceOutputLogType, cErrorMessageInfo, eErrorInfoCode) \
    g_clsRecOpeLogFile.LogOutputControlProc(LOGARGS, iOutputType, iTraceOutputLogType, cErrorMessageInfo, eErrorInfoCode)

//☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★
// 各機能（API）定義
//☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★
//********************************************
// 関数名：デフォルトコンストラクタ
// Function Name：CRecordOperations
// 
// 処理概要：
// クラス生成に伴い初期化内容を定義する
// 
// 
// 補足事項
// 
// 
// 作成履歴
// 2020/9/25  s-saito   新規作成
// 
// 更新履歴
// 
// 
//********************************************
CRecordOperations::CRecordOperations()
{
    // STAT構造体変数初期化
    memset(&m_stStat, 0x00, sizeof(struct stat));
}

//********************************************
// 関数名：パラメータ情報付きコンストラクタ
// Function Name：CRecordOperations
// 
// 処理概要：
// パラメータ情報に基づいてクラスを生成する
// 
// 
// 補足事項
// 
// 
// 作成履歴
// 2020/9/25  s-saito   新規作成
// 
// 更新履歴
// 
// 
//********************************************
CRecordOperations::CRecordOperations(
    const string strTraceLogFileName, 
    const string strDataLogFileName)
{
    // ログ出力クラス生成
    g_clsRecOpeLogFile = CLogFile(strTraceLogFileName, strDataLogFileName);

    // 呼び出し元で指定されたログ出力ファイル名設定
    m_strRecOpeTraceLogFileName = strTraceLogFileName;
    m_strRecOpeDataLogFileName = strDataLogFileName;
    m_strFileOpeTraceLogFileName = strTraceLogFileName;
    m_strFileOpeDataLogFileName = strDataLogFileName;
    m_strSubOpeTraceLogFileName = strTraceLogFileName;
    m_strSubOpeDataLogFileName = strDataLogFileName;

    // 入退室総合情報構造体変数初期化
    memset(&m_stEntryExitInfo, 0x00, sizeof(ENTRYEXITINFORMATION));
    memset(&m_stEntryExitInfo.unEntryExitRecord.stEntryExitRecord, 
        0x00, 
        sizeof(ENTRYEXITRECORDINFO));
    memset(&m_stEntryExitInfo.unEntryExitRecord.cEntryExitData, 
        0x00, 
        sizeof(m_stEntryExitInfo.unEntryExitRecord.cEntryExitData));
}

//********************************************
// 関数名：デストラクタ
// Function Name：~CRecordOperations
// 
// 処理概要：
// クラス終了に伴いメモリ解放等を定義する
// 
// 
// 補足事項
// 
// 
// 作成履歴
// 2020/9/25  s-saito   新規作成
// 
// 更新履歴
// 
// 
//********************************************
CRecordOperations::~CRecordOperations()
{
    // マップテーブル情報を削除
    m_maErrorActionName.clear();

    // ログ出力ファイル名変数クリア
    m_strRecOpeTraceLogFileName.clear();
    m_strRecOpeDataLogFileName.clear();
    m_strFileOpeTraceLogFileName.clear();
    m_strFileOpeDataLogFileName.clear();
    m_strFileOpeCopyFileName.clear();
    m_strSubOpeTraceLogFileName.clear();
    m_strSubOpeDataLogFileName.clear();

    // 各メンバ変数クリア
    m_iNowDoorLevel = 0;
    memset(&m_stStat, 0x00, sizeof(struct stat));
    memset(&m_stEntryExitInfo, 0x00, sizeof(ENTRYEXITINFORMATION));
    memset(&m_stEntryExitInfo.unEntryExitRecord.stEntryExitRecord, 
        0x00, 
        sizeof(ENTRYEXITRECORDINFO));
    memset(&m_stEntryExitInfo.unEntryExitRecord.cEntryExitData, 
        0x00, 
        sizeof(m_stEntryExitInfo.unEntryExitRecord.cEntryExitData));
}

//********************************************
// 関数名：入退室データ編集情報取得処理
// Function Name：GetEditDataProc
// 
// 処理概要：
// 入退室読み込みデータファイルから1レコードを読み込む
// 
// 処理手順：
//   １．レコード読み込み
//   ２．ファイル終端チェック（レコード取得出来なかった場合）
//   ３．入退室データ編集情報生成処理（レコード取得成功且つレコード長が33バイトの場合）
// 
// 引数：    入退室読み込みデータファイルポインタ
//           入退室書き込みデータファイルポインタ
//           スキップ制御フラグ
//           ファイル終了フラグ
//           実行結果
//           入退室レコード件数
// 戻り値：  入退室データ編集用構造体（動的構造体）
// 
// 補足事項
// ・レコード読み込みはレコード長が33バイトの1レコード単位で読み込む
// ・入退室データ編集用構造体生成処理はレコード長が33バイトの場合のみ行う
// ・実行結果は以下の情報を設定する
//         正常終了：EXIT_SUCCESS（0）（正常終了はstdlib.hで定義している内容を利用）
//         異常終了：レコードなし：CM_NODATA（106）
//                   レコード不正：CM_DATAFRAUD（107）
//                   レコード取得エラー：CM_DATAERROR（108）
// 
// 
// 作成履歴
// 2020/9/25  s-saito   新規作成
// 
// 更新履歴
// 
// 
//********************************************
ENTRYEXITDATA *CRecordOperations::GetEditDataProc(
	FILE *pfDataFile, 
    FILE *pfOutputFile,
	bool *pbSkipFlag, 
	int *piFileEndFlag, 
	int *piGetResult, 
    long *plRecCnt)
{
    // 変数宣言
    ENTRYEXITDATA *pstEditData = NULL;
    const ENTRYEXITDATA stInitInfoData = { 
        "----", 
        "----------", 
        "--------", 
        "------", 
        "--", 
        "-", 
        "-" };
    char *pcReadResult = NULL;
    int iDataEOF = EXIT_SUCCESS;
    static long lDataCnt = 0;
    long lDatalen = 0;
    char cReadResult[DATASIZE + 1];
    char cReadResultLogout[DATASIZE + 1];
    string strDummyDate;
    string strDummyTime;
    string strDummyDateTime;
    string strDummyData;
    string strDummyDataInfo;
    string strDummyDataInfoLog;
    PRINTTYPETABLE ePrintType = tagPrintType::DUMMY;
    char cFraudData[DATASIZE + 1];
    char cFraudOutputData[DATASIZE + 1];

    // 初期化
    memset(cReadResult, 0x00, sizeof(cReadResult));
    memset(cReadResultLogout, 0x00, sizeof(cReadResultLogout));
    memset(cFraudData, 0x00, sizeof(cFraudData));
    memset(cFraudOutputData, 0x00, sizeof(cFraudOutputData));

    // 標準出力に日本語が表示されるようにロケールを日本に設定
    setlocale(LC_ALL, "japanese");

    // 前処理
    // 日時情報取得処理
    // 入退室データ編集情報取得処理開始日時取得
    m_stTimeData = g_clsRecOpeLogFile.GetDateTimeProc();
    sprintf_s(m_cLogData, 
        sizeof(m_cLogData), 
        "入退室データ編集情報取得処理開始日時：%s %s", 
        (char *)m_stTimeData.strMachDatePlus.c_str(), 
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
    sprintf_s(m_cLogData,
        sizeof(m_cLogData),
        "***** 入退室データ編集情報取得処理開始（%ld回目） *****",
        *plRecCnt + 1);
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);

    try
    {
        // 入退室情報データ編集用構造体メモリ領域の確保
        pstEditData = (ENTRYEXITDATA *)malloc(sizeof(ENTRYEXITDATA));
        m_stLogData = CreateEventMessageProc(
            ACTIONSET,
            "入退室データ編集情報の");

        // メモリ領域が確保出来たかチェック
        if (NULL == pstEditData)
        {
            // メモリ領域の確保失敗
            // 例外処理へスロー
            throw tagErrorCode::MEMORYERROR;
        }

        // メモリの初期化
        memset(pstEditData, 0x00, sizeof(ENTRYEXITDATA));

        // 構造体に初期値を設定
        *pstEditData = stInitInfoData;
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, "***** 入退室データ編集情報の初期状態 *****");
        sprintf_s(
            m_cLogData,
            sizeof(m_cLogData),
            "カードID：%s",
            pstEditData->cCardID);
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
        sprintf_s(
            m_cLogData,
            sizeof(m_cLogData),
            "退避カードID：%s",
            m_stEntryExitInfo.cLastID);
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
        sprintf_s(
            m_cLogData,
            sizeof(m_cLogData),
            "ユーザー名：%s",
            pstEditData->cName);
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
        sprintf_s(
            m_cLogData,
            sizeof(m_cLogData),
            "日付：%s",
            pstEditData->cDate);
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
        sprintf_s(
            m_cLogData,
            sizeof(m_cLogData),
            "時刻：%s",
            pstEditData->cTime);
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
        sprintf_s(
            m_cLogData,
            sizeof(m_cLogData),
            "ドア番号：%s",
            pstEditData->cDoor);
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
        sprintf_s(
            m_cLogData,
            sizeof(m_cLogData),
            "入退室状態：%s",
            pstEditData->cDir);
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
        sprintf_s(
            m_cLogData,
            sizeof(m_cLogData),
            "可否情報：%s",
            pstEditData->cAct);
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

        // メモリ領域確保成功のメッセージをログに出力
        LOGOUTPUT(
            TRACEOUTPUTLOG,
            SUCCESSINFORMATION,
            "***** 入退室情報データ編集用構造体メモリ領域確保正常終了 *****");
        sprintf_s(
            m_cLogData,
            sizeof(m_cLogData),
            "%s%s",
            m_stLogData.pcActionName,
            MEMORYCREATESUCCESS);
        LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, m_cLogData);

        // 主処理
        // ファイルからレコードを取得
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 入退室レコード取得開始 *****");
        pcReadResult = fgets(m_stEntryExitInfo.unEntryExitRecord.cEntryExitData, DATASIZE, pfDataFile);
        lDatalen = (long)strlen(m_stEntryExitInfo.unEntryExitRecord.cEntryExitData);
        sprintf_s(m_cLogData, sizeof(m_cLogData), "レコード長：%ld", lDatalen);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 入退室レコード取得終了 *****");

        // 読み込み結果をチェック
        if (NULL == pcReadResult)
        {
            // レコードが取得出来なかった
            // 終端情報をチェック
            LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 入退室レコードの終端値チェック *****");
            iDataEOF = feof(pfDataFile);
            sprintf_s(m_cLogData, sizeof(m_cLogData), "終端値は：%d", iDataEOF);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
            if (EXIT_SUCCESS != iDataEOF)
            {
                // 終端情報あり
                // 前回にレコードを取得していたかチェック
                if (0 == lDataCnt)
                {
                    // レコードなしエラーとして例外を発生して例外処理へスロー
                    throw tagErrorCode::NODATA;
                }
                else
                {
                    // ファイル終了フラグをON
                    LOGOUTPUT(
                        TRACEOUTPUTLOG,
                        SUCCESSINFORMATION,
                        "***** 入退室データ編集情報取得正常終了 *****");
                    *piFileEndFlag = FILEEND_ON;
                    LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, READEND);

                    // レコードが取得出来ていないためスキップ制御フラグをON
                    *pbSkipFlag = true;
                    sprintf_s(
                        m_cLogData,
                        sizeof(m_cLogData),
                        "スキップ制御フラグ：%s",
                        *pbSkipFlag ? "true" : "false");
                    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
                }
            }
            else
            {
                // 終端情報なし
                // レコード取得エラーとして例外を発生して例外処理へスロー
                throw tagErrorCode::DATAERROR;
            }
        }
        else
        {
            // レコードが存在していたのでカウントアップ
            (*plRecCnt)++;

            // 指定文字削除処理
            // 改行コードを削除して取得レコードを出力
            strncpy_s(cReadResult, sizeof(cReadResult), m_stEntryExitInfo.unEntryExitRecord.cEntryExitData, lDatalen);
            DeleteDesignationCharProc(cReadResult, (const char)CRLF_CODE, cReadResultLogout);
            sprintf_s(m_cLogData, sizeof(m_cLogData), "取得レコード：%s", cReadResultLogout);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

            // レコード長をチェック
            if (READDATA == lDatalen)
            {
                // レコード長正常
                // 入退室データ編集情報生成処理
                // レコードを入退室情報データ編集用構造体で生成
                LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 入退室データ編集情報生成関数呼び出し *****");
                *pstEditData = CreateEditDataProc(m_stEntryExitInfo.unEntryExitRecord);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "取得したカードIDは：%s",
                    pstEditData->cCardID);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                // 生成した入退室情報データ編集用構造体をログ出力
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, "***** 入退室データ編集情報の生成状態 *****");
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "カードID：%s", pstEditData->cCardID);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "退避カードID：%s",
                    m_stEntryExitInfo.cLastID);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "ユーザー名：%s",
                    pstEditData->cName);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "日付：%s",
                    pstEditData->cDate);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "時刻：%s",
                    pstEditData->cTime);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "ドア番号：%s",
                    pstEditData->cDoor);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "入退室状態：%s",
                    pstEditData->cDir);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "可否情報：%s",
                    pstEditData->cAct);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                // データ編集及び印字可能件数をカウント
                lDataCnt++;
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "印字可能なレコード件数：%ld",
                    lDataCnt);
                LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

                // 印字対象レコードのためスキップ制御フラグをOFF
                *piGetResult = EXIT_SUCCESS;
                *pbSkipFlag = false;
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "スキップ制御フラグ：%s",
                    *pbSkipFlag ? "true" : "false");
                LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
                LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, "***** 入退室データ編集情報取得正常終了 *****");
            }
            else
            {
                // レコード長が不正
                // 指定文字削除処理
                // 改行コードを削除した文字列を出力
                strncpy_s(cFraudData, sizeof(cFraudData), pcReadResult, sizeof(cFraudData) - 1);
                DeleteDesignationCharProc(cFraudData, (const char)CRLF_CODE, cFraudOutputData);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "出力データは：%s",
                    cFraudOutputData);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                // 入退室不正データファイル出力処理
                PrintFraudDataFileProc(cFraudOutputData, g_stFileNameInfo.strFraudDataName.c_str());

                // 印字対象外レコードのためスキップ制御フラグをON
                *pbSkipFlag = true;
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "スキップ制御フラグ：%s",
                    *pbSkipFlag ? "true" : "false");
                LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
                LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** 入退室データ編集情報取得異常終了 *****");

                // レコード不正エラーとして例外を発生して例外処理へスロー
                throw tagErrorCode::DATAFRAUD;
            }
        }
    }

    // 例外処理
    catch (const ERRORCODETABLE eDataErr)
    {
        // 例外コードをチェック
        if (tagErrorCode::MEMORYERROR == eDataErr)
        {
            // メモリ領域確保エラー
            // エラーコードを設定
            LOGOUTPUT(
                TRACEOUTPUTLOG,
                ERRORINFORMATION2,
                "***** 入退室情報データ編集用構造体メモリ領域確保異常終了 *****");
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
            pstEditData = NULL;
            *piGetResult = CM_MEMORYERROR;
            m_maErrorActionName.insert(
                make_pair(
                    *piGetResult,
                    m_stLogData.pcActionName));
        }
        else if (tagErrorCode::NODATA == eDataErr)
        {
            // レコードがない
            LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, "***** レコードがない。。。( *´・ω)/(；д； ) *****");
            LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, "***** ダミーデータを使用・・・(；´Д｀) *****");
            LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, "***** 入退室データ編集情報ダミー状態 *****");
            sprintf_s(
                m_cLogData,
                sizeof(m_cLogData),
                "カードID：%s",
                pstEditData->cCardID);
            LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
            sprintf_s(
                m_cLogData,
                sizeof(m_cLogData),
                "退避カードID：%s",
                m_stEntryExitInfo.cLastID);
            LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
            sprintf_s(
                m_cLogData,
                sizeof(m_cLogData),
                "ユーザー名：%s",
                pstEditData->cName);
            LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
            sprintf_s(
                m_cLogData,
                sizeof(m_cLogData),
                "日付：%s",
                pstEditData->cDate);
            LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
            sprintf_s(
                m_cLogData,
                sizeof(m_cLogData),
                "時刻：%s",
                pstEditData->cTime);
            LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
            sprintf_s(
                m_cLogData,
                sizeof(m_cLogData),
                "ドア番号：%s",
                pstEditData->cDoor);
            LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
            sprintf_s(
                m_cLogData,
                sizeof(m_cLogData),
                "入退室状態：%s",
                pstEditData->cDir);
            LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
            sprintf_s(
                m_cLogData,
                sizeof(m_cLogData),
                "可否情報：%s",
                pstEditData->cAct);
            LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

            // PC閲覧画面に見出しを出力（入退室書き込みデータファイルへは出力しない）
            LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 入退室状況印字見出し設定関数呼び出し *****");
            SetEntryExitDataTitleProc(pfOutputFile, eDataErr);

            // 初期値をダミーデータとして生成
            // ユーザーID・ユーザー名
            strDummyData = pstEditData->cCardID;
            strDummyData += "  ";
            strDummyData += pstEditData->cName;
            strDummyData += "  ";

            // 日付・時刻・ドア番号
            strDummyDate = pstEditData->cDate;
            strDummyTime = pstEditData->cTime;
            strDummyDateTime = strDummyDate.substr(4, 2);
            strDummyDateTime += "/";
            strDummyDateTime += strDummyDate.substr(6, 2);
            strDummyDateTime += " ";
            strDummyDateTime += strDummyTime.substr(0, 2);
            strDummyDateTime += ":";
            strDummyDateTime += strDummyTime.substr(2, 2);
            strDummyDateTime += " ";
            strDummyDateTime += pstEditData->cDoor;

            // ダミーデータ
            strDummyDataInfo = strDummyData;
            strDummyDataInfo += strDummyDateTime;
            strDummyDataInfo += "       ";
            strDummyDataInfo += strDummyDateTime;
            strDummyDataInfo += "       ";
            strDummyDataInfo += strDummyDateTime;

            // 一旦ログ出力用に格納して改行コードを設定
            strDummyDataInfoLog = strDummyDataInfo;
            strDummyDataInfo += "\n\n";

            // 入退室データ編集情報出力制御処理
            // ダミーデータ出力を指示（ここでは入退室書き込みデータファイルポインタは使用しない）
            ePrintType = tagPrintType::DUMMYDATA;
            LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 入退室データ編集情報出力制御関数呼び出し *****");
            PrintDataControlProc(
                SCREENPRINT,
                pfOutputFile,
                "%s",
                "",
                *pstEditData,
                strDummyDataInfo.c_str(), 
                ePrintType);
            LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ダミーデータとして初期値を出力 *****");
            sprintf_s(m_cLogData, sizeof(m_cLogData), "ダミーデータ：%s", strDummyDataInfoLog.c_str());
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

            // 入退室情報データ編集用構造体メモリ領域を解放
            LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 入退室情報データ編集用構造体メモリ領域解放 *****");
#ifdef _DEBUG
            SAFE_FREE_DEBUG(pstEditData);
#else
            SAFE_FREE(pstEditData);
#endif // _DEBUG

            // メモリ領域解放のメッセージをログに出力
            LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 入退室情報データ編集用構造体メモリ領域解放終了 *****");
            sprintf_s(
                m_cLogData,
                sizeof(m_cLogData),
                "%s%s",
                m_stLogData.pcActionName,
                MEMORYRELEASESUCCESS);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

            // ファイル終了フラグをON（戻り値がレコードなし）
            LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** 入退室データ編集情報取得異常終了 *****");
            m_stLogData = CreateEventMessageProc(ACTIONSET, "入退室データ編集情報の");
            *piFileEndFlag = FILEEND_ON;
            *piGetResult = CM_NODATA;
            g_clsRecOpeLogFile.m_eErrorResultCode = eDataErr;
            g_clsRecOpeLogFile.m_maErrorActionName.insert(make_pair(*piGetResult, m_stLogData.pcActionName));

            // 日時情報取得処理
            // 入退室データ編集情報取得終了日時取得
            m_stTimeData = g_clsRecOpeLogFile.GetDateTimeProc();
            sprintf_s(
                m_cLogData,
                sizeof(m_cLogData),
                "入退室データ編集情報取得終了日時：%s %s",
                (char *)m_stTimeData.strMachDatePlus.c_str(),
                (char *)m_stTimeData.strMachTimePlus.c_str());
            LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);

            // 例外をリスロー
            throw eDataErr;
        }
        else if (tagErrorCode::DATAERROR == eDataErr)
        {
            // レコード取得エラー
            // ファイル終了フラグをON
            LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** 入退室データ編集情報取得異常終了 *****");
            *piFileEndFlag = FILEEND_ON;

            // レコードが取得出来ていないためスキップ制御フラグをON
            *pbSkipFlag = true;
            sprintf_s(
                m_cLogData,
                sizeof(m_cLogData),
                "スキップ制御フラグ：%s",
                *pbSkipFlag ? "true" : "false");
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

            // 入退室情報データ編集用構造体メモリ領域を解放
            LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 入退室情報データ編集用構造体メモリ領域解放 *****");
#ifdef _DEBUG
            SAFE_FREE_DEBUG(pstEditData);
#else
            SAFE_FREE(pstEditData);
#endif // _DEBUG

            // メモリ領域解放のメッセージをログに出力
            LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 入退室情報データ編集用構造体メモリ領域解放終了 *****");
            sprintf_s(
                m_cLogData,
                sizeof(m_cLogData),
                "%s%s",
                m_stLogData.pcActionName,
                MEMORYRELEASESUCCESS);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

            // エラーコードを設定
            m_stLogData = CreateEventMessageProc(ACTIONSET, "入退室データ編集情報の");
            *piGetResult = CM_DATAERROR;
            g_clsRecOpeLogFile.m_eErrorResultCode = eDataErr;
            g_clsRecOpeLogFile.m_maErrorActionName.insert(make_pair(*piGetResult, m_stLogData.pcActionName));

            // 日時情報取得処理
            // 入退室データ編集情報取得終了日時取得
            m_stTimeData = g_clsRecOpeLogFile.GetDateTimeProc();
            sprintf_s(
                m_cLogData,
                sizeof(m_cLogData),
                "入退室データ編集情報取得終了日時：%s %s",
                (char *)m_stTimeData.strMachDatePlus.c_str(),
                (char *)m_stTimeData.strMachTimePlus.c_str());
            LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);

            // 例外をリスロー
            throw eDataErr;
        }
        else
        {
            // レコード不正
            // メッセージを生成
            m_stLogData = CreateEventMessageProc(ACTIONSET, "入退室データ編集情報の");

            // 2回目以降かチェック
            if (2 <= *plRecCnt)
            {
                // 2回目以降
                // エラーメッセージ生成処理
                // エラーコードを設定及びメッセージをログに出力
                *piGetResult = CM_DATAFRAUD;
                m_maErrorActionName.insert(
                    make_pair(
                        *piGetResult, m_stLogData.pcActionName));
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "%d回目のレコード長：%d",
                    *plRecCnt,
                    lDatalen);
                LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
                LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** エラーメッセージ生成関数呼び出し *****");
                m_eErrorResultCode = eDataErr;
                m_stLogData = CreateEventMessageProc(
                    ERRORSET,
                    m_stLogData.pcActionName,
                    m_eErrorResultCode);
                LOGOUTPUTERROR(
                    TRACEOUTPUTLOG,
                    ERRORINFORMATION1,
                    m_stLogData.pcMessage,
                    m_stLogData.eErrorCode);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "戻り値は：%d",
                    *piGetResult);
                LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

                // 処理継続メッセージをログに出力
                LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, DATACONTINUED);
            }
            else
            {
                // 1回目
                // エラーコードのみ設定
                *piGetResult = CM_DATAFRAUD;
                m_eErrorResultCode = eDataErr;
                m_maErrorActionName.insert(
                    make_pair(
                        *piGetResult, m_stLogData.pcActionName));
            }

            // 入退室情報データ編集用構造体メモリ領域を解放
            LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 入退室情報データ編集用構造体メモリ領域解放 *****");
#ifdef _DEBUG
            SAFE_FREE_DEBUG(pstEditData);
#else
            SAFE_FREE(pstEditData);
#endif // _DEBUG

            // メモリ領域解放のメッセージをログに出力
            LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 入退室情報データ編集用構造体メモリ領域解放終了 *****");
            sprintf_s(
                m_cLogData,
                sizeof(m_cLogData),
                "%s%s",
                m_stLogData.pcActionName,
                MEMORYRELEASESUCCESS);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
        }
    }

    // 後処理
    // 日時情報取得処理
    // 入退室データ編集情報取得終了日時取得
    m_stTimeData = g_clsRecOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "入退室データ編集情報取得終了日時：%s %s", 
        (char *)m_stTimeData.strMachDatePlus.c_str(), 
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);

    // 取得したレコード情報を返却
    return pstEditData;
}

//********************************************
// 関数名：入退室データ編集情報出力指示処理
// Function Name：PutEditDataProc
// 
// 処理概要：
// 入退室データ編集情報取得処理で取得したレコードを帳票形式に編集して
// PC閲覧画面及び入退室書き込みデータファイルへの印字を指示する
// 
// 処理手順：
//   １．カードID及びユーザー名出力指示
//   ２．制限エリアレベルチェック処理
//   ３．不整合に対するデータ出力指示（不整合の場合のみ）
//   ４．制限エリアレベル更新
//   ５．読み込みデータの制限エリアレベル算出
//   ６．取得データ出力指示
//   ７．可否情報チェック
//   ８．可否情報出力指示
//   ９．入退室状態チェック
//   １０．入退室状態出力指示
// 
// 引数：     入退室書き込みデータファイルポインタ
//            入退室情報データ編集用構造体ポインタ
//            ドアレベル情報構造体ポインタ
// 戻り値：   なし
// 
// 補足事項
// 
// 
// 
// 作成履歴
// 2020/9/25  s-saito   新規作成
// 
// 更新履歴
// 
// 
//********************************************
void CRecordOperations::PutEditDataProc(
	FILE *pfOutputFile, 
	ENTRYEXITDATA *pstEData, 
	DOORLEVELINFORMATION *pstDoorInfo)
{
    // 変数宣言
    int iPutSpace = 0;
    int iChkResult = ERR_OFF;
    PRINTTYPETABLE ePrintType = tagPrintType::DUMMY;

    // 前処理
    // 日時情報取得処理
    // 入退室データ編集情報出力指示開始日時取得
    m_stTimeData = g_clsRecOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "入退室データ編集情報出力指示開始日時：%s %s", 
        (char *)m_stTimeData.strMachDatePlus.c_str(), 
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 入退室データ編集情報出力指示開始 *****");

    // カードID情報をログに出力
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "読み込んだレコードのカードIDは：%s", 
        pstEData->cCardID);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "退避分のカードIDは：%s", 
        m_stEntryExitInfo.cLastID);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    // 主処理
    // カードIDが同一の場合
    if (0 == strncmp(pstEData->cCardID, m_stEntryExitInfo.cLastID, RECORD_CARDID))
    {
        // 入退室データ編集情報出力制御処理
        // 半角空白文字出力指示（カードIDと名前を出力しない代わりに半角空白文字で補正）
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 入退室データ編集情報出力制御関数呼び出し *****");
        PrintDataControlProc(BOTHPRINT, pfOutputFile, "%18s", "", *pstEData, " ");
    }
    else
    {
        // 入退室データ編集情報出力制御処理
        // カードIDとユーザー名の出力を指示
        ePrintType = tagPrintType::CARDID;
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 入退室データ編集情報出力制御関数呼び出し *****");
        PrintDataControlProc(BOTHPRINT, pfOutputFile, "%4s  %10s  ", "", *pstEData, "", ePrintType);

        // カードIDを退避
        strncpy_s(m_stEntryExitInfo.cLastID, sizeof(m_stEntryExitInfo.cLastID), pstEData->cCardID, RECORD_CARDID);
        sprintf_s(
            m_cLogData, 
            sizeof(m_cLogData), 
            "退避カードIDは：%s", 
            m_stEntryExitInfo.cLastID);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

        // 入退室データ編集情報をログ出力
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, "***** 入退室データ編集情報の更新状態 *****");
        sprintf_s(
            m_cLogData, 
            sizeof(m_cLogData), 
            "カードID：%s", 
            pstEData->cCardID);
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
        sprintf_s(
            m_cLogData, 
            sizeof(m_cLogData), 
            "退避カードID：%s", 
            m_stEntryExitInfo.cLastID);
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
        sprintf_s(
            m_cLogData, 
            sizeof(m_cLogData), 
            "ユーザー名：%s", 
            pstEData->cName);
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
        sprintf_s(
            m_cLogData, 
            sizeof(m_cLogData), 
            "日付：%s", 
            pstEData->cDate);
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
        sprintf_s(
            m_cLogData, 
            sizeof(m_cLogData), 
            "時刻：%s", 
            pstEData->cTime);
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
        sprintf_s(
            m_cLogData, 
            sizeof(m_cLogData), 
            "ドア番号：%s", 
            pstEData->cDoor);
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
        sprintf_s(
            m_cLogData, 
            sizeof(m_cLogData), 
            "入退室状態：%s", 
            pstEData->cDir);
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
        sprintf_s(
            m_cLogData, 
            sizeof(m_cLogData), 
            "可否情報：%s", 
            pstEData->cAct);
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
    }

    // 制限エリアレベルチェック処理
    // ドアの通過状況をチェック
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 制限エリアレベルチェック関数呼び出し *****");
    iChkResult = CheckLevelProc(*pstEData, pstDoorInfo);
    sprintf_s(m_cLogData, sizeof(m_cLogData), "チェック結果は：%d", iChkResult);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    // チェック結果を確認
    // ここでの判定値『ERR_ON』はエラーではなく不整合として判定する
    if (ERR_ON == iChkResult)
    {
        // 不整合の場合
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 現在の制限エリアレベル→不整合 *****");
        sprintf_s(
            m_cLogData, 
            sizeof(m_cLogData), 
            "入退出状態は：%s", 
            pstEData->cDir);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

        // 現在の通過ドアレベルと通過前ドアレベルをログ出力
        sprintf_s(
            m_cLogData, 
            sizeof(m_cLogData), 
            "現在の通過ドアレベルは：%d", 
            pstDoorInfo->iLevel);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
        sprintf_s(
            m_cLogData, 
            sizeof(m_cLogData), 
            "通過前ドアレベルは：%d", 
            pstDoorInfo->iBeforeLevel);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

        // 入退出状態をチェック
        if (0 == strncmp(pstEData->cDir, NYUSHITSU, RECORD_DIR))
        {
            // 入退室状態が『I』
            // 現在のドアレベルを算出
            m_iNowDoorLevel = pstDoorInfo->iLevel - pstDoorInfo->iBeforeLevel;
            sprintf_s(
                m_cLogData, 
                sizeof(m_cLogData), 
                "現在のドアレベルは：%d", 
                m_iNowDoorLevel);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

            // 現在のドアレベルをチェック
            if (1 < m_iNowDoorLevel || 1 < m_iNowDoorLevel * (-1))
            {
                // 記録していない情報が複数あり
                // 入退室データ編集情報出力制御処理
                // 本来入退室の記録があるべき行への出力を指示
                ePrintType = tagPrintType::LEVELNG2;
                LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 記録していない情報が複数あり *****");
                LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 入退室データ編集情報出力制御関数呼び出し *****");
                PrintDataControlProc(
                    BOTHPRINT, 
                    pfOutputFile,
                    "***** Level %d-->%d\n%18s",
                    "",
                    *pstEData,
                    " ",
                    ePrintType,
                    pstDoorInfo->iLevel,
                    pstDoorInfo->iBeforeLevel);
            }
            else
            {
                // 記録していない情報が複数なし
                // 入退室データ編集情報出力制御処理
                // 本来入退室の記録があるべき行への出力を指示
                ePrintType = tagPrintType::LEVELNG1;
                LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 記録していない情報が複数なし *****");
                LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 入退室データ編集情報出力制御関数呼び出し *****");
                PrintDataControlProc(
                    BOTHPRINT, 
                    pfOutputFile,
                    "***** Level %d-->%d\n%18s",
                    "",
                    *pstEData,
                    " ",
                    ePrintType,
                    pstDoorInfo->iLevel,
                    pstDoorInfo->iBeforeLevel);
            }
        }
        else if (0 == strncmp(pstEData->cDir, TAISHITSU, RECORD_DIR))
        {
            // 入退室状態が『O』
            // 現在のドアレベルを算出
            m_iNowDoorLevel = pstDoorInfo->iBeforeLevel - pstDoorInfo->iLevel;
            sprintf_s(
                m_cLogData, 
                sizeof(m_cLogData), 
                "現在のドアレベルは：%d", 
                m_iNowDoorLevel);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

            // 現在のドアレベルをチェック
            if (1 < m_iNowDoorLevel)
            {
                // 記録していない情報が複数あり
                // 現在のドアレベルが最大かチェック
                if (FLOORMAX == m_iNowDoorLevel)
                {
                    // 現在のドアレベルが最大（入室記録なし）
                    // 入退室データ編集情報出力制御処理
                    // 本来入退室の記録があるべき行への出力を指示
                    ePrintType = tagPrintType::LEVELNG3;
                    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 入室記録なし *****");
                    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 入退室データ編集情報出力制御関数呼び出し *****");
                    PrintDataControlProc(
                        BOTHPRINT, 
                        pfOutputFile,
                        "***** Level %d-->%d\n%18s",
                        "",
                        *pstEData,
                        " ",
                        ePrintType,
                        m_iNowDoorLevel);
                }
                else
                {
                    // 現在のドアレベルが最大ではない（入室記録あり）
                    // 入退室データ編集情報出力制御処理
                    // 本来入退室の記録があるべき行への出力を指示
                    ePrintType = tagPrintType::LEVELNG2;
                    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 記録していない情報が複数あり *****");
                    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 入退室データ編集情報出力制御関数呼び出し *****");
                    PrintDataControlProc(
                        BOTHPRINT, 
                        pfOutputFile,
                        "***** Level %d-->%d\n%18s",
                        "",
                        *pstEData,
                        " ",
                        ePrintType,
                        pstDoorInfo->iLevel,
                        pstDoorInfo->iBeforeLevel);
                }
            }
            else
            {
                // 記録していない情報が複数なし
                // 入退室データ編集情報出力制御処理
                // 本来入退室の記録があるべき行への出力を指示
                ePrintType = tagPrintType::LEVELNG1;
                LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 記録していない情報が複数なし *****");
                LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 入退室データ編集情報出力制御関数呼び出し *****");
                PrintDataControlProc(
                    BOTHPRINT, 
                    pfOutputFile,
                    "***** Level %d-->%d\n%18s",
                    "",
                    *pstEData,
                    " ",
                    ePrintType,
                    pstDoorInfo->iLevel,
                    pstDoorInfo->iBeforeLevel);
            }
        }

        // 現在の通過ドアレベルを更新
        pstDoorInfo->iLevel = pstDoorInfo->iAfterLevel;
        sprintf_s(
            m_cLogData, 
            sizeof(m_cLogData), 
            "現在の通過ドアレベルは：%d", 
            pstDoorInfo->iLevel);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
    }
    else
    {
        // 整合の場合
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 現在の制限エリアレベル→整合 *****");
        sprintf_s(
            m_cLogData, 
            sizeof(m_cLogData), 
            "入退出状態は：%s", 
            pstEData->cDir);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
    }

    // 後処理
    // ドア番号からレベルを算出
    iPutSpace = pstEData->cDoor[0] - '0' - 1;
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "算出後のレベルは：%d", 
        iPutSpace);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    // レベル値に基づいてループ
    while (0 < iPutSpace--)
    {
        // 入退室データ編集情報出力制御処理
        // 半角空白文字出力を指示
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 入退室データ編集情報出力制御関数呼び出し *****");
        PrintDataControlProc(
            BOTHPRINT, 
            pfOutputFile,
            "%21s",
            "",
            *pstEData,
            " ");
    }

    // 入退室データ編集情報出力制御処理
    // 取得したデータの出力を指示
    ePrintType = tagPrintType::DOOR;
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 入退室データ編集情報出力制御関数呼び出し *****");
    PrintDataControlProc(
        BOTHPRINT, 
        pfOutputFile,
        "%.2s/%.2s %.2s:%.2s %2s ",
        "",
        *pstEData,
        "",
        ePrintType);

    // 可否情報をログに出力
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "可否情報は：%s", 
        pstEData->cAct);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    // 可否情報をチェック
    if (0 == strncmp(pstEData->cAct, KYOHI, RECORD_ACT))
    {
        // 可否情報が『拒否』の場合
        // 入退室データ編集情報出力制御処理
        // 『拒否』の情報出力を指示
        ePrintType = tagPrintType::ACT;
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 入退室データ編集情報出力制御関数呼び出し *****");
        PrintDataControlProc(
            BOTHPRINT, 
            pfOutputFile,
            "%s",
            "(R)",
            *pstEData,
            "",
            ePrintType);
    }

    // 入退室状態をログに出力
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "入退室状態は：%s", 
        pstEData->cDir);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    // 入退室状態をチェック
    if (0 == strncmp(pstEData->cDir, NYUSHITSU, RECORD_DIR))
    {
        // 入退室状態が『I』
        // 入退室データ編集情報出力制御処理
        // 入退室状態に対して『IN』での出力を指示
        ePrintType = tagPrintType::ACT;
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 入退室データ編集情報出力制御関数呼び出し *****");
        PrintDataControlProc(
            BOTHPRINT, 
            pfOutputFile,
            "%s\n",
            "IN",
            *pstEData,
            "",
            ePrintType);
    }
    else if (0 == strncmp(pstEData->cDir, TAISHITSU, RECORD_DIR))
    {
        // 入退室情報が『O』
        // 入退室データ編集情報出力制御処理
        // 入退室情報に対して『OUT』での出力を指示
        ePrintType = tagPrintType::ACT;
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 入退室データ編集情報出力制御関数呼び出し *****");
        PrintDataControlProc(
            BOTHPRINT, 
            pfOutputFile,
            "%s\n",
            "OUT",
            *pstEData,
            "",
            ePrintType);
    }

    // 日時情報取得処理
    // 入退室データ編集情報出力指示終了日時取得
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 入退室データ編集情報出力指示終了 *****");
    m_stTimeData = g_clsRecOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "入退室データ編集情報出力指示終了日時：%s %s", 
        (char *)m_stTimeData.strMachDatePlus.c_str(), 
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
}

//********************************************
// 関数名：制限エリアレベルクリア処理
// Function Name：ClearLevelProc
// 
// 処理概要：
// ファイル終了フラグ及びカードIDと退避カードIDに基づいて
// ドアレベル情報構造体の前レコード通過後ドアレベルをクリアする
// 
// 処理手順：
//   １．カードIDチェック
//   ２．現在の制限エリアレベルチェック
//   ３．不整合に対するデータ出力指示
//   ４．制限エリアレベルクリア
// 
// 引数：     入退室書き込みデータファイルポインタ
//            入退室情報データ編集用構造体
//            ファイル終了フラグ
//            ドアレベル情報構造体ポインタ
// 戻り値：   なし
// 
// 補足事項
// ・制限エリアレベルが0より大きい場合はレコードなしメッセージを
//     PC等閲覧画面及び入退室書き込みデータファイルへ印字する
// 
// 
// 作成履歴
// 2020/9/25  s-saito   新規作成
// 
// 更新履歴
// 
// 
//********************************************
void CRecordOperations::ClearLevelProc(
	FILE *pfOutputFile, 
	const ENTRYEXITDATA stEData, 
	const int iFileEndFlg, 
	DOORLEVELINFORMATION *pstDoorData)
{
    // 変数宣言
    PRINTTYPETABLE ePrintType = tagPrintType::DUMMY;

    // 前処理
    // 日時情報取得処理
    // 制限エリアレベルクリア開始日時取得
    m_stTimeData = g_clsRecOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "制限エリアレベルクリア開始日時：%s %s", 
        (char *)m_stTimeData.strMachDatePlus.c_str(), 
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 制限エリアレベルクリア開始 *****");

    // データ状態をログに出力
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "ファイル終端値は：%d", 
        iFileEndFlg);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "入退室データ編集情報のカードIDは：%4s", 
        stEData.cCardID);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "退避カードIDは：%4s", 
        m_stEntryExitInfo.cLastID);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    // 主処理
    // カードIDをチェック
    if ((FILEEND_ON == iFileEndFlg) ||
        (0 != strncmp(stEData.cCardID, m_stEntryExitInfo.cLastID, RECORD_CARDID)))
    {
        // ファイル終了フラグがONもしくはカードIDが退避分と異なる場合
        // 現在の通過ドアレベルが0より大きいかチェック
        if (0 < pstDoorData->iLevel)
        {
            // 現在の通過ドアレベルが0より大きい場合
            sprintf_s(
                m_cLogData, 
                sizeof(m_cLogData), 
                "現在のエリアレベルは：%d", 
                pstDoorData->iLevel);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

            // 現在の通過ドアレベルをチェック
            // 記録していない情報が複数あり
            switch (pstDoorData->iLevel)
            {
                case 3:
                {
                    // 現在の通過ドアレベルが『3』（退室記録なし）
                    // 入退室データ編集情報出力制御処理
                    // 本来退室の記録があるべき行への3行出力を指示
                    ePrintType = tagPrintType::LEVELNG4;
                    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 退室記録なし *****");
                    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 入退室データ編集情報出力制御関数呼び出し *****");
                    PrintDataControlProc(
                        BOTHPRINT, 
                        pfOutputFile,
                        "%18s***** Level %d-->%d\n",
                        "",
                        stEData,
                        " ",
                        ePrintType,
                        pstDoorData->iLevel);

                    // 分岐抜け
                    break;
                }
                case 2:
                {
                    // 現在の通過ドアレベルが『2』
                    // 入退室データ編集情報出力制御処理
                    // 本来退室の記録があるべき行への2行出力を指示
                    ePrintType = tagPrintType::LEVELNG5;
                    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 入退室データ編集情報出力制御関数呼び出し *****");
                    PrintDataControlProc(
                        BOTHPRINT, 
                        pfOutputFile,
                        "%18s***** Level %d-->%d\n",
                        "",
                        stEData,
                        " ",
                        ePrintType,
                        pstDoorData->iLevel);

                    // 分岐抜け
                    break;
                }
                default:
                {
                    // 記録していない情報が複数なし
                    // 入退室データ編集情報出力制御処理
                    // 本来退室の記録があるべき行への1行出力を指示
                    ePrintType = tagPrintType::LEVELNG6;
                    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 入退室データ編集情報出力制御関数呼び出し *****");
                    PrintDataControlProc(
                        BOTHPRINT, 
                        pfOutputFile,
                        "%18s***** Level %d-->0\n",
                        "",
                        stEData,
                        " ",
                        ePrintType,
                        pstDoorData->iLevel);

                    // 分岐抜け
                    break;
                }
            }

            // 現在の通過ドアレベルをクリア
            pstDoorData->iLevel = 0;
            sprintf_s(
                m_cLogData, 
                sizeof(m_cLogData), 
                "クリア後の現在の通過ドアレベルは：%d", 
                pstDoorData->iLevel);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
        }
    }

    // 後処理
    // 日時情報取得処理
    // 制限エリアレベルクリア終了日時取得
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 制限エリアレベルクリア終了 *****");
    m_stTimeData = g_clsRecOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "制限エリアレベルクリア終了日時：%s %s", 
        (char *)m_stTimeData.strMachDatePlus.c_str(), 
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
}

//********************************************
// 関数名：入退室状況印字見出し設定処理
// Function Name：SetEntryExitDataTitleProc
// 
// 処理概要：
// 入退室書き込みデータファイル及びPC閲覧画面に見出しを設定する
// 
// 処理手順：
//   １．カードIDチェック
//   ２．現在の制限エリアレベルチェック
//   ３．不整合に対するデータ出力指示
//   ４．制限エリアレベルクリア
// 
// 引数：     入退室書き込みデータファイルポインタ
// 　　　　   呼び出し元実行結果
// 戻り値：   なし
// 
// 補足事項
// ・入退室書き込みデータファイルへはファイルに書き込みがない且つ
//   入退室レコードが取得出来ている場合のみ印字する
// 
// 
// 作成履歴
// 2020/9/25  s-saito   新規作成
// 
// 更新履歴
// 
// 
//********************************************
void CRecordOperations::SetEntryExitDataTitleProc(
    FILE *pfOutputFile, 
    ERRORCODETABLE eResult)
{
    // 変数宣言
    string strTitle;

    // 前処理
    // 日時情報取得処理
    // 入退室状況印字見出し設定開始日時取得
    m_stTimeData = g_clsRecOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData,
        sizeof(m_cLogData),
        "入退室状況印字見出し設定開始日時：%s %s",
        (char *)m_stTimeData.strMachDatePlus.c_str(),
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 入退室状況印字見出し設定開始 *****");

    // 主処理
    // 文字列を生成
    strTitle = TITLE;

    // ファイルサイズをログに出力
    sprintf_s(
        m_cLogData,
        sizeof(m_cLogData),
        "ファイルサイズは：%d",
        m_stStat.st_size);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    // ファイルサイズをチェック
    if (0 == m_stStat.st_size && tagErrorCode::SUCCESS == eResult)
    {
        // ファイルの中がない（新規作成）且つレコード取得成功
        // 見出しをファイルへ出力
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ファイルへ書き込み *****");
        fprintf_s(pfOutputFile, "%s\n", strTitle.c_str());
        fprintf_s(pfOutputFile, "\n");

        // 出力した見出しをログに出力
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ファイルへの見出し出力 *****");
        sprintf_s(
            m_cLogData,
            sizeof(m_cLogData),
            "ファイルへの出力データは：%s",
            strTitle.c_str());
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
    }

#ifdef SCREEN
    // 見出しをPC閲覧画面へ出力
    printf_s("%s\n", strTitle.c_str());
    printf_s("\n");

    // 出力した見出しをログに出力
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** PC閲覧画面への見出し出力 *****");
    sprintf_s(
        m_cLogData,
        sizeof(m_cLogData),
        "PC閲覧画面のデータは：%s",
        strTitle.c_str());
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
#endif // SCREEN

    // 後処理
    // 日時情報取得処理
    // 入退室状況印字見出し設定終了日時取得
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 入退室状況印字見出し設定終了 *****");
    m_stTimeData = g_clsRecOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData,
        sizeof(m_cLogData),
        "入退室状況印字見出し設定終了日時：%s %s",
        (char *)m_stTimeData.strMachDatePlus.c_str(),
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
}

//********************************************
// 関数名：入退室データ編集情報生成処理
// Function Name：CreateEditDataProc
// 
// 処理概要：
// 入退室に対するデータ編集情報として
// 入退室レコードを取得する
// 
// 処理手順：
//      １．カードID取得
//      ２．ユーザー名取得
//      ３：日付取得
//      ４：時刻取得
//      ５：ドア番号取得
//      ６：入退室状態取得
//      ７：可否情報取得
// 
// 引数：     入退室情報レコード
// 戻り値：   入退室情報データ編集用構造体
// 
// 補足事項
// ・当処理はレコード長が33バイトの場合のみ行う事とする
// 
// 
// 作成履歴
// 2020/9/25  s-saito   新規作成
// 
// 更新履歴
// 
// 
//********************************************
ENTRYEXITDATA CRecordOperations::CreateEditDataProc(
	const ENTRYEXITRECORD unRecData)
{
    // 変数宣言
    ENTRYEXITDATA stEditData;

    // 初期化
    memset(&stEditData, 0x00, sizeof(stEditData));

    // 前処理
    // 日時情報取得処理
    // 入退室情報データ編集情報生成開始日時取得
    m_stTimeData = g_clsRecOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "入退室情報データ編集情報生成開始日時：%s %s", 
        (char *)m_stTimeData.strMachDatePlus.c_str(), 
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 入退室情報データ編集情報生成開始 *****");

    // 主処理
    // 入退室情報データ編集用構造体に対して入退室データレコードから情報を取得
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, "***** 入退室情報データ編集用構造体の状態 *****");

    // カードID
    strncpy_s(stEditData.cCardID, sizeof(stEditData.cCardID), unRecData.stEntryExitRecord.cCardID, RECORD_CARDID);
    sprintf_s(m_cLogData, sizeof(m_cLogData), "カードID：%s", stEditData.cCardID);
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

    // 名前
    strncpy_s(stEditData.cName, sizeof(stEditData.cName), unRecData.stEntryExitRecord.cName, RECORD_NAME);
    sprintf_s(m_cLogData, sizeof(m_cLogData), "ユーザー名：%s", stEditData.cName);
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

    // 日付
    strncpy_s(stEditData.cDate, sizeof(stEditData.cDate), unRecData.stEntryExitRecord.cDate, RECORD_DATE);
    sprintf_s(m_cLogData, sizeof(m_cLogData), "日付：%s", stEditData.cDate);
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

    // 時刻
    strncpy_s(stEditData.cTime, sizeof(stEditData.cTime), unRecData.stEntryExitRecord.cTime, RECORD_TIME);
    sprintf_s(m_cLogData, sizeof(m_cLogData), "時刻：%s", stEditData.cTime);
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

    // ドア番号
    strncpy_s(stEditData.cDoor, sizeof(stEditData.cDoor), unRecData.stEntryExitRecord.cDoor, RECORD_DOOR);
    sprintf_s(m_cLogData, sizeof(m_cLogData), "ドア番号：%s", stEditData.cDoor);
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

    // 入退室状態
    strncpy_s(stEditData.cDir, sizeof(stEditData.cDir), unRecData.stEntryExitRecord.cDir, RECORD_DIR);
    sprintf_s(m_cLogData, sizeof(m_cLogData), "入退室状態：%s", stEditData.cDir);
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

    // 可否情報
    strncpy_s(stEditData.cAct, sizeof(stEditData.cAct), unRecData.stEntryExitRecord.cAct, RECORD_ACT);
    sprintf_s(m_cLogData, sizeof(m_cLogData), "可否情報：%s", stEditData.cAct);
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

    // 後処理
    // 日時情報取得処理
    // 入退室情報データ編集情報生成終了日時取得
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 入退室情報データ編集情報生成終了 *****");
    m_stTimeData = g_clsRecOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "入退室情報データ編集情報生成終了日時：%s %s", 
        (char *)m_stTimeData.strMachDatePlus.c_str(), 
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);

    // 入退室情報データ編集用構造体を返却
    return stEditData;
}

//********************************************
// 関数名：制限エリアレベルチェック処理
// Function Name：CheckLevelProc
// 
// 処理概要：
// 可否情報をチェック及び入退室状態と
// 制限エリアレベルの整合状態をチェックする
// 
// 処理手順：
//   １．入退出状態チェック
//   ２．可否情報チェック
//   ３．制限エリアレベルチェック
//   ４．制限エリアレベル更新（整合の場合のみ）
// 
// 引数：     入退室情報データ編集用構造体
//            ドアレベル情報構造体ポインタ
// 戻り値：   照合結果
//              整合    ：ERR_OFF（0）
//              不整合  ：ERR_ON（1）
// 
// 補足事項
// ・ここでの定数『ERR_OFF』『ERR_ON』はエラーではなく
//     整合・不整合の判定値として使用する
// 
// 
// 作成履歴
// 2020/9/25  s-saito   新規作成
// 
// 更新履歴
// 
// 
//********************************************
int CRecordOperations::CheckLevelProc(
	const ENTRYEXITDATA stEData, 
	DOORLEVELINFORMATION *pstDoorData)
{
    // 変数宣言
    int iConsistencyFlag = 0;
    int iCheckResult = ERR_OFF;

    // 前処理
    // 日時情報取得処理
    // 制限エリアレベルチェック開始日時取得
    m_stTimeData = g_clsRecOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "制限エリアレベルチェック開始日時：%s %s", 
        (char *)m_stTimeData.strMachDatePlus.c_str(), 
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 制限エリアレベルチェック開始 *****");

    // 初期値を設定
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, "***** 制限エリアレベル初期値 *****");
    pstDoorData->iDoorLevel = stEData.cDoor[0] - '0';
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "通過ドアレベル：%d", 
        pstDoorData->iDoorLevel);
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
    pstDoorData->iBeforeLevel = pstDoorData->iDoorLevel;
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "通過前ドアレベル：%d", 
        pstDoorData->iBeforeLevel);
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
    pstDoorData->iAfterLevel = pstDoorData->iDoorLevel;
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "通過後ドアレベル：%d", 
        pstDoorData->iAfterLevel);
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "現在の不整合フラグは：%d", 
        iConsistencyFlag);
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

    // 主処理
    // 入退出状態をログに出力
    sprintf_s(m_cLogData, sizeof(m_cLogData), "入退出状態は：%s", stEData.cDir);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    // 入退出状態をチェック
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, "***** 更新後制限エリアレベル値 *****");
    if (0 == strncmp(stEData.cDir, NYUSHITSU, RECORD_DIR))
    {
        // 入退出状態が『I』（入室）の場合
        // 通過前ドアレベルを設定
        pstDoorData->iBeforeLevel = pstDoorData->iDoorLevel - 1;
        sprintf_s(
            m_cLogData, 
            sizeof(m_cLogData), 
            "通過前ドアレベルは：%d", 
            pstDoorData->iBeforeLevel);
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
    }
    else if (0 == strncmp(stEData.cDir, TAISHITSU, RECORD_DIR))
    {
        // 入退出状態が『O』（退室）の場合
        // 通過後ドアレベルを設定
        pstDoorData->iAfterLevel = pstDoorData->iDoorLevel - 1;
        sprintf_s(
            m_cLogData, 
            sizeof(m_cLogData), 
            "通過後エリアレベルは：%d", pstDoorData->iAfterLevel);
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
    }

    // 可否情報をログに出力
    sprintf_s(m_cLogData, sizeof(m_cLogData), "可否情報は：%s", stEData.cAct);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    // 可否情報をチェック
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, "***** 更新後制限エリアレベル値 *****");
    if (0 == strncmp(stEData.cAct, KYOHI, RECORD_ACT))
    {
        // 可否情報が『R』（拒否）の場合
        // 通過後ドアレベルに通過前ドアレベルを設定
        pstDoorData->iAfterLevel = pstDoorData->iBeforeLevel;
        sprintf_s(
            m_cLogData, 
            sizeof(m_cLogData), 
            "通過後ドアレベルは：%d", pstDoorData->iAfterLevel);
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
    }

    // 現在の通過ドアレベルと通過前ドアレベルが同一かチェック
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, "***** 更新後制限エリアレベル値 *****");
    if (pstDoorData->iLevel != pstDoorData->iBeforeLevel)
    {
        // 不整合
        // 現在の通過ドアレベルが通過前ドアレベルと異なる場合
        // 不整合フラグをON
        iConsistencyFlag = 1;
        sprintf_s(
            m_cLogData, 
            sizeof(m_cLogData), 
            "不整合フラグは：%d", 
            iConsistencyFlag);
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
    }
    else
    {
        // 整合
        // 現在の通過ドアレベルが通過前ドアレベルと同一の場合
        sprintf_s(
            m_cLogData, 
            sizeof(m_cLogData), 
            "不整合フラグは：%d", 
            iConsistencyFlag);
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

        // 現在の通過ドアレベルを通過後ドアレベルで更新
        pstDoorData->iLevel = pstDoorData->iAfterLevel;
        sprintf_s(
            m_cLogData, 
            sizeof(m_cLogData), 
            "通過ドアレベルは：%d", 
            pstDoorData->iDoorLevel);
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
        sprintf_s(
            m_cLogData, 
            sizeof(m_cLogData), 
            "現在の通過ドアレベルは：%d", 
            pstDoorData->iLevel);
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
    }

    // 後処理
    // 不整合フラグを見て戻り値を設定
    iCheckResult = (1 == iConsistencyFlag) ? ERR_ON : ERR_OFF;
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "照合結果は：%d", iCheckResult);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 制限エリアレベルチェック終了 *****");

    // 日時情報取得処理
    // 制限エリアレベルチェック終了日時取得
    m_stTimeData = g_clsRecOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "制限エリアレベルチェック終了日時：%s %s", 
        (char *)m_stTimeData.strMachDatePlus.c_str(), 
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);

    // 照合結果を返却
    return iCheckResult;
}

//********************************************
// 関数名：入退室データ編集情報出力制御処理
// Function Name：PrintDataControlProc
// 
// 処理概要：
// 処理状況及び実行環境に基づいて入退室書き込みデータファイル及び
// PC閲覧画面への出力を切り分ける
// 
// 処理手順：
//   １．データ出力先タイプの分岐
//   ２．入退室書き込みデータファイルのみの出力
//   ３．PC閲覧画面のみの出力
//   ４．両方への出力
// 
// 引数：　 データ出力先タイプ
// 　　　　 入退室書き込みデータファイルポインタ
// 　　　　 出力形式
// 　　　　 入退出状態
// 　　　　 入退室情報データ編集用構造体
//          出力文字列（空白等）
// 　　　　 データ出力タイプ
// 　　　　 制限エリアレベル情報１
// 　　　　 制限エリアレベル情報２
// 戻り値： なし
// 
// 補足事項
// 
// 
// 作成履歴
// 2020/9/25  s-saito   新規作成
// 
// 更新履歴
// 
// 
//********************************************
void CRecordOperations::PrintDataControlProc(
    int iDataPrintType,
    FILE* pfOutputFile,
    const char *pcOutputType,
    const char *pcAct,
    const ENTRYEXITDATA stEData,
    const char *pcEtcChar,
    PRINTTYPETABLE eType,
    int iLevel1,
    int iLevel2)
{
    // 前処理
    // 日時情報取得処理
    // 入退室データ編集情報出力制御開始日時取得
    m_stTimeData = g_clsRecOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData,
        sizeof(m_cLogData),
        "入退室データ編集情報出力制御開始日時：%s %s",
        (char *)m_stTimeData.strMachDatePlus.c_str(),
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 入退室データ編集情報出力制御開始 *****");

    // データ出力先識別子をログに出力
    sprintf_s(
        m_cLogData,
        sizeof(m_cLogData),
        "データ出力先識別子は：%d",
        iDataPrintType);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    // 主処理
    // データ出力先識別子に基づいて出力先を分岐
    switch (iDataPrintType)
    {
    case BOTHPRINT:
    {
        // ファイル・画面両方に出力
        // 入退室データPC閲覧画面出力処理及び入退室データファイル出力処理
        // PC閲覧画面及び入退室書き込みデータファイルへ出力
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 入退室データファイル出力関数呼び出し *****");
        PrintReportFileProc(
            pfOutputFile,
            pcOutputType,
            pcAct,
            stEData,
            pcEtcChar,
            eType,
            iLevel1,
            iLevel2);
#ifdef SCREEN
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 入退室データPC閲覧画面出力関数呼び出し *****");
        PrintScreenProc(
            pcOutputType,
            pcAct,
            stEData,
            pcEtcChar,
            eType,
            iLevel1,
            iLevel2);
#endif // SCREEN

        // 分岐抜け
        break;
    }
    case SCREENPRINT:
    {
        // PC閲覧画面のみ出力
        // 入退室データPC閲覧画面出力処理
        // PC閲覧画面へ出力
#ifdef SCREEN
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 入退室データPC閲覧画面出力関数呼び出し *****");
        PrintScreenProc(
            pcOutputType,
            pcAct,
            stEData,
            pcEtcChar,
            eType,
            iLevel1,
            iLevel2);
#endif // SCREEN

        // 分岐抜け
        break;
    }
    default:
    {
        // 入退室書き込みデータファイルのみ出力
        // 入退室データファイル出力処理
        // 入退室書き込みデータファイルへ出力
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 入退室データファイル出力関数呼び出し *****");
        PrintReportFileProc(
            pfOutputFile,
            pcOutputType,
            pcAct,
            stEData,
            pcEtcChar,
            eType,
            iLevel1,
            iLevel2);

        // 分岐抜け
        break;
    }
    }

    // 後処理
    // 日時情報取得処理
    // 入退室データ編集情報出力制御終了日時取得
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 入退室データ編集情報出力制御終了 *****");
    m_stTimeData = g_clsRecOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData,
        sizeof(m_cLogData),
        "入退室データ編集情報出力制御終了日時：%s %s",
        (char *)m_stTimeData.strMachDatePlus.c_str(),
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
}

//********************************************
// 関数名：入退室不正データファイル出力処理
// Function Name：PrintFraudDataFileProc
// 
// 処理概要：
// 入退室書き込みデータファイルへ
// 出力出来なかった不正入退室データをレコード形式で出力する
// 
// 処理手順：
//   １．入退室不正データファイルオープン
//   ２．入退室不正データファイルへの出力
// 
// 引数：　 入退室レコード不正データ
// 　　　　 入退室レコード不正データファイル
// 戻り値： なし
// 
// 補足事項
// 
// 
// 作成履歴
// 2020/9/25  s-saito   新規作成
// 
// 更新履歴
// 
// 
//********************************************
void CRecordOperations::PrintFraudDataFileProc(
    const char *pcFraudData, 
    const char *pcFraudDataFileName)
{
    // 変数宣言
    FILE *pfFraudDataFile = NULL;
    errno_t eLogErrNo = 0;
    int iCloseResult = EXIT_SUCCESS;

    // 前処理
    // 日時情報取得処理
    // 入退室不正データファイル出力開始日時取得
    m_stTimeData = g_clsRecOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData,
        sizeof(m_cLogData),
        "入退室不正データファイル出力開始日時：%s %s",
        (char *)m_stTimeData.strMachDatePlus.c_str(),
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 入退室不正データファイル出力開始 *****");

    // 主処理
    // 入退室不正データファイル（更新追記型）をオープン
    // 他の処理に影響はない為入退室不正データファイルはオープンに失敗した場合出力を諦めて処理を継続
    eLogErrNo = fopen_s(&pfFraudDataFile, pcFraudDataFileName, "a+");

    // 入退室不正データファイルをオープン出来たかチェック
    if (NULL != pfFraudDataFile)
    {
        // 入退室不正データファイルオープンに成功している場合
        // 入退室不正データファイルに出力
        fprintf(pfFraudDataFile, "%s\n", pcFraudData);
        sprintf_s(
            m_cLogData,
            sizeof(m_cLogData),
            "入退室不正データは：%s", pcFraudData);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

        // 入退室不正データファイルをクローズ
        // 入退室不正データファイルがクローズ出来なくてもメッセージ等出さずに処理終了
        iCloseResult = fclose(pfFraudDataFile);
    }

    // 後処理
    // 日時情報取得処理
    // 入退室不正データファイル出力終了日時取得
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 入退室不正データファイル出力終了 *****");
    m_stTimeData = g_clsRecOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData,
        sizeof(m_cLogData),
        "入退室不正データファイル出力終了日時：%s %s",
        (char *)m_stTimeData.strMachDatePlus.c_str(),
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
}

//********************************************
// 関数名：入退室データファイル出力処理
// Function Name：PrintReportFileProc
// 
// 処理概要：
// 入退室書き込みデータファイルへ
// 出力タイプに基づいて入退室データを出力する
//      タイプ1：ドア情報
//      タイプ2：カードID及びユーザー名
//      タイプ3：可否情報
//      タイプ4：不整合による制限エリアレベルNG情報１
//      タイプ5：不整合による制限エリアレベルNG情報２
//      タイプ6：不整合による制限エリアレベルNG情報３
//      タイプ7：不整合による制限エリアレベルNG情報４
//      タイプ8：不整合による制限エリアレベルNG情報５
//      タイプ9：不整合による制限エリアレベルNG情報６
//      タイプ10：上記以外の出力情報（空白文字等）
// 
// 処理手順：
//   １．データ出力メモリ領域確保
//   ２．入退室書き込みデータファイルへの出力
//   ３．データ出力メモリ領域解放
// 
// 引数：　 入退室書き込みデータファイルポインタ
// 　　　　 出力形式
// 　　　　 入退出状態
// 　　　　 入退室情報データ編集用構造体
//          出力文字列（空白等）
// 　　　　 データ出力タイプ
// 　　　　 制限エリアレベル情報１
// 　　　　 制限エリアレベル情報２
// 戻り値： なし
// 
// 補足事項
// ・データ出力メモリ領域確保に失敗した場合は
//     メモリ領域確保エラー（CM_MEMORYERROR（109））としてログに出力する
// 
// 
// 作成履歴
// 2020/9/25  s-saito   新規作成
// 
// 更新履歴
// 
// 
//********************************************
void CRecordOperations::PrintReportFileProc(
    FILE *pfOutputFile,
    const char *pcOutputType,
    const char *pcAct,
    const ENTRYEXITDATA stEData,
    const char *pcEtcChar,
    PRINTTYPETABLE eType,
    int iLevel1,
    int iLevel2)
{
    // 変数宣言
    char cFileOutputData[DATASIZE + 1];
    char cFileOutputNGDataLogout[DATASIZE + 1];
    char *pcFileOutputData = NULL;

    // 初期化
    memset(cFileOutputData, 0x00, sizeof(cFileOutputData));
    memset(cFileOutputNGDataLogout, 0x00, sizeof(cFileOutputNGDataLogout));

    // 標準出力に日本語が表示されるようにロケールを日本に設定
    setlocale(LC_ALL, "japanese");

    // メモリ領域確保に失敗した場合のNULL設定
    // この設定により確保に失敗した場合はNULLを返すようにする
    set_new_handler(NULL);

    // 前処理
    // 日時情報取得処理
    // 入退室データファイル出力開始日時取得
    m_stTimeData = g_clsRecOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData,
        sizeof(m_cLogData),
        "入退室データファイル出力開始日時：%s %s",
        (char *)m_stTimeData.strMachDatePlus.c_str(),
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 入退室データファイル出力開始 *****");

    try
    {
        // データ出力メモリ領域確保
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** データ出力メモリ領域確保 *****");
        pcFileOutputData = DBG_NEW char[DATASIZE + 1];
        m_stLogData = CreateEventMessageProc(
            ACTIONSET,
            "入退室データファイル出力");

        // メモリ領域が確保出来たかチェック
        if (NULL == pcFileOutputData)
        {
            // メモリ領域の確保失敗
            // 例外処理へスロー
            throw tagErrorCode::MEMORYERROR;
        }

        // メモリの初期化
        memset(pcFileOutputData, 0x00, DATASIZE + 1);

        // メモリ領域確保成功のメッセージをログに出力
        LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, "***** データ出力メモリ領域確保正常終了 *****");
        sprintf_s(
            m_cLogData,
            sizeof(m_cLogData),
            "%sの%s",
            m_stLogData.pcActionName,
            MEMORYCREATESUCCESS);
        LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, m_cLogData);

        // データ出力タイプをログに出力
        sprintf_s(
            m_cLogData,
            sizeof(m_cLogData),
            "データ出力タイプは：%d",
            eType);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

        // 主処理
        // ファイルへのデータ出力
        // 出力タイプにて出力方法を分岐
        switch (eType)
        {
            case tagPrintType::DOOR:
            {
                // 入退出記録
                // 取得した入退出記録を出力
                sprintf_s(
                    pcFileOutputData,
                    DATASIZE,
                    pcOutputType,
                    stEData.cDate + 4,
                    stEData.cDate + 6,
                    stEData.cTime,
                    stEData.cTime + 2,
                    stEData.cDoor);
                fprintf_s(pfOutputFile, "%s", pcFileOutputData);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "出力データは：%s",
                    pcFileOutputData);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                // 分岐抜け
                break;
            }
            case tagPrintType::CARDID:
            {
                // カードIDとユーザー名
                // 取得したカードIDとユーザー名を出力
                sprintf_s(cFileOutputData, sizeof(cFileOutputData), pcOutputType, stEData.cCardID,
                    stEData.cName);
                sprintf_s(pcFileOutputData, DATASIZE, "%s", cFileOutputData);
                fprintf_s(pfOutputFile, "%s", pcFileOutputData);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "出力データは：%s",
                    cFileOutputData);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                // 分岐抜け
                break;
            }
            case tagPrintType::ACT:
            {
                // 入退出状態・可否情報
                // 取得した入退出情報・可否情報を出力
                sprintf_s(pcFileOutputData, DATASIZE, pcOutputType, pcAct);
                fprintf_s(pfOutputFile, "%s", pcFileOutputData);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "出力データは：%s",
                    pcAct);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                // 分岐抜け
                break;
            }
            case tagPrintType::LEVELNG1:
            {
                // 制限エリアレベル出力（連続して記録なし）
                // 本来入退室の記録があるべき行に出力
                sprintf_s(pcFileOutputData, DATASIZE, pcOutputType, iLevel1, iLevel2, pcEtcChar);
                fprintf_s(pfOutputFile, "%s", pcFileOutputData);

                // 指定文字削除処理
                // 改行コードを削除した文字列をログに出力
                strncpy_s(cFileOutputData, sizeof(cFileOutputData), pcFileOutputData, sizeof(cFileOutputData) - 1);
                DeleteDesignationCharProc(cFileOutputData, (const char)CRLF_CODE, cFileOutputNGDataLogout);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "出力データは：%s",
                    cFileOutputNGDataLogout);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                // 分岐抜け
                break;
            }
            case tagPrintType::LEVELNG2:
            {
                // 制限エリアレベル出力（連続して記録あり）
                // 入退出状態をログに出力
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "現在の入退出状態は：%s",
                    stEData.cDir);
                LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

                // 入退出状態をチェック
                if (0 == strncmp(stEData.cDir, NYUSHITSU, RECORD_DIR))
                {
                    // 入退出状態が『I』（入室）の場合
                    // 現在のドアレベル値がマイナスかチェック
                    if (0 > m_iNowDoorLevel)
                    {
                        // 現在のドアレベル値がマイナス値
                        // 本来入退室の記録があるべき行に2行昇順出力
                        // 1行目
                        sprintf_s(pcFileOutputData, DATASIZE, pcOutputType, iLevel1, iLevel2 - 1, pcEtcChar);
                        fprintf_s(pfOutputFile, "%s", pcFileOutputData);

                        // 指定文字削除処理
                        // 改行コードを削除した文字列をログに出力
                        strncpy_s(cFileOutputData, sizeof(cFileOutputData), pcFileOutputData, sizeof(cFileOutputData) - 1);
                        DeleteDesignationCharProc(cFileOutputData, (const char)CRLF_CODE, cFileOutputNGDataLogout);
                        sprintf_s(
                            m_cLogData,
                            sizeof(m_cLogData),
                            "出力データは：%s",
                            cFileOutputNGDataLogout);
                        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                        // 2行目
                        sprintf_s(pcFileOutputData, DATASIZE, pcOutputType, iLevel1 + 1, iLevel2, pcEtcChar);
                        fprintf_s(pfOutputFile, "%s", pcFileOutputData);

                        // 指定文字削除処理
                        // 改行コードを削除した文字列をログに出力
                        strncpy_s(cFileOutputData, sizeof(cFileOutputData), pcFileOutputData, sizeof(cFileOutputData) - 1);
                        DeleteDesignationCharProc(cFileOutputData, (const char)CRLF_CODE, cFileOutputNGDataLogout);
                        sprintf_s(
                            m_cLogData,
                            sizeof(m_cLogData),
                            "出力データは：%s",
                            cFileOutputNGDataLogout);
                        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
                    }
                    else
                    {
                        // 現在のドアレベル値がマイナス値ではない
                        // 本来入退室の記録があるべき行に2行昇順出力
                        // 1行目
                        sprintf_s(pcFileOutputData, DATASIZE, pcOutputType, iLevel1, iLevel2 + 1, pcEtcChar);
                        fprintf_s(pfOutputFile, "%s", pcFileOutputData);

                        // 指定文字削除処理
                        // 改行コードを削除した文字列をログに出力
                        strncpy_s(cFileOutputData, sizeof(cFileOutputData), pcFileOutputData, sizeof(cFileOutputData) - 1);
                        DeleteDesignationCharProc(cFileOutputData, (const char)CRLF_CODE, cFileOutputNGDataLogout);
                        sprintf_s(
                            m_cLogData,
                            sizeof(m_cLogData),
                            "出力データは：%s",
                            cFileOutputNGDataLogout);
                        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                        // 2行目
                        sprintf_s(pcFileOutputData, DATASIZE, pcOutputType, iLevel1 - 1, iLevel2, pcEtcChar);
                        fprintf_s(pfOutputFile, "%s", pcFileOutputData);

                        // 指定文字削除処理
                        // 改行コードを削除した文字列をログに出力
                        strncpy_s(cFileOutputData, sizeof(cFileOutputData), pcFileOutputData, sizeof(cFileOutputData) - 1);
                        DeleteDesignationCharProc(cFileOutputData, (const char)CRLF_CODE, cFileOutputNGDataLogout);
                        sprintf_s(
                            m_cLogData,
                            sizeof(m_cLogData),
                            "出力データは：%s",
                            cFileOutputNGDataLogout);
                        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
                    }
                }
                else if (0 == strncmp(stEData.cDir, TAISHITSU, RECORD_DIR))
                {
                    // 入退出状態が『O』（退室）の場合
                    // 本来入退室の記録があるべき行に2行降順出力
                    // 1行目
                    sprintf_s(pcFileOutputData, DATASIZE, pcOutputType, iLevel1, iLevel2 - 1, pcEtcChar);
                    fprintf_s(pfOutputFile, "%s", pcFileOutputData);

                    // 指定文字削除処理
                    // 改行コードを削除した文字列をログに出力
                    strncpy_s(cFileOutputData, sizeof(cFileOutputData), pcFileOutputData, sizeof(cFileOutputData) - 1);
                    DeleteDesignationCharProc(cFileOutputData, (const char)CRLF_CODE, cFileOutputNGDataLogout);
                    sprintf_s(
                        m_cLogData,
                        sizeof(m_cLogData),
                        "出力データは：%s",
                        cFileOutputNGDataLogout);
                    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                    // 2行目
                    sprintf_s(pcFileOutputData, DATASIZE, pcOutputType, iLevel1 + 1, iLevel2, pcEtcChar);
                    fprintf_s(pfOutputFile, "%s", pcFileOutputData);

                    // 指定文字削除処理
                    // 改行コードを削除した文字列をログに出力
                    strncpy_s(cFileOutputData, sizeof(cFileOutputData), pcFileOutputData, sizeof(cFileOutputData) - 1);
                    DeleteDesignationCharProc(cFileOutputData, (const char)CRLF_CODE, cFileOutputNGDataLogout);
                    sprintf_s(
                        m_cLogData,
                        sizeof(m_cLogData),
                        "出力データは：%s",
                        cFileOutputNGDataLogout);
                    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
                }

                // 分岐抜け
                break;
            }
            case tagPrintType::LEVELNG3:
            {
                // 制限エリアレベル出力（入室記録なし）
                // 本来入退室の記録があるべき行に3行昇順出力
                // 1行目
                sprintf_s(pcFileOutputData, DATASIZE, "***** Level 0-->%d\n%18s", iLevel1 - 2, pcEtcChar);
                fprintf_s(pfOutputFile, "%s", pcFileOutputData);

                // 指定文字削除処理
                // 改行コードを削除した文字列をログに出力
                strncpy_s(cFileOutputData, sizeof(cFileOutputData), pcFileOutputData, sizeof(cFileOutputData) - 1);
                DeleteDesignationCharProc(cFileOutputData, (const char)CRLF_CODE, cFileOutputNGDataLogout);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "出力データは：%s",
                    cFileOutputNGDataLogout);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                // 2行目
                sprintf_s(pcFileOutputData, DATASIZE, pcOutputType, iLevel1 - 2, iLevel1 - 1, pcEtcChar);
                fprintf_s(pfOutputFile, "%s", pcFileOutputData);

                // 指定文字削除処理
                // 改行コードを削除した文字列をログに出力
                strncpy_s(cFileOutputData, sizeof(cFileOutputData), pcFileOutputData, sizeof(cFileOutputData) - 1);
                DeleteDesignationCharProc(cFileOutputData, (const char)CRLF_CODE, cFileOutputNGDataLogout);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "出力データは：%s",
                    cFileOutputNGDataLogout);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                // 3行目
                sprintf_s(pcFileOutputData, DATASIZE, pcOutputType, iLevel1 - 1, iLevel1, pcEtcChar);
                fprintf_s(pfOutputFile, "%s", pcFileOutputData);

                // 指定文字削除処理
                // 改行コードを削除した文字列をログに出力
                strncpy_s(cFileOutputData, sizeof(cFileOutputData), pcFileOutputData, sizeof(cFileOutputData) - 1);
                DeleteDesignationCharProc(cFileOutputData, (const char)CRLF_CODE, cFileOutputNGDataLogout);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "出力データは：%s",
                    cFileOutputNGDataLogout);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                // 分岐抜け
                break;
            }
            case tagPrintType::LEVELNG4:
            {
                // 制限エリアレベル出力（退室記録なし）
                // 本来入退室の記録があるべき行に3行降順出力
                // 1行目
                sprintf_s(pcFileOutputData, DATASIZE, pcOutputType, pcEtcChar, iLevel1, iLevel1 - 1);
                fprintf_s(pfOutputFile, "%s", pcFileOutputData);

                // 指定文字削除処理
                // 改行コードを削除した文字列をログに出力
                strncpy_s(cFileOutputData, sizeof(cFileOutputData), pcFileOutputData, sizeof(cFileOutputData) - 1);
                DeleteDesignationCharProc(cFileOutputData, (const char)CRLF_CODE, cFileOutputNGDataLogout);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "出力データは：%s",
                    cFileOutputNGDataLogout);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                // 2行目
                sprintf_s(pcFileOutputData, DATASIZE, pcOutputType, pcEtcChar, iLevel1 - 1, iLevel1 - 2);
                fprintf_s(pfOutputFile, "%s", pcFileOutputData);

                // 指定文字削除処理
                // 改行コードを削除した文字列をログに出力
                strncpy_s(cFileOutputData, sizeof(cFileOutputData), pcFileOutputData, sizeof(cFileOutputData) - 1);
                DeleteDesignationCharProc(cFileOutputData, (const char)CRLF_CODE, cFileOutputNGDataLogout);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "出力データは：%s",
                    cFileOutputNGDataLogout);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                // 3行目
                sprintf_s(pcFileOutputData, DATASIZE, "%18s***** Level %d-->0\n", pcEtcChar, iLevel1 - 2);
                fprintf_s(pfOutputFile, "%s", pcFileOutputData);

                // 指定文字削除処理
                // 改行コードを削除した文字列をログに出力
                strncpy_s(cFileOutputData, sizeof(cFileOutputData), pcFileOutputData, sizeof(cFileOutputData) - 1);
                DeleteDesignationCharProc(cFileOutputData, (const char)CRLF_CODE, cFileOutputNGDataLogout);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "出力データは：%s",
                    cFileOutputNGDataLogout);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                // 分岐抜け
                break;
            }
            case tagPrintType::LEVELNG5:
            {
                // 制限エリアレベル出力
                // 本来入退室の記録があるべき行に2行降順出力
                // 1行目
                sprintf_s(pcFileOutputData, DATASIZE, pcOutputType, pcEtcChar, iLevel1, iLevel1 - 1);
                fprintf_s(pfOutputFile, "%s", pcFileOutputData);

                // 指定文字削除処理
                // 改行コードを削除した文字列をログに出力
                strncpy_s(cFileOutputData, sizeof(cFileOutputData), pcFileOutputData, sizeof(cFileOutputData) - 1);
                DeleteDesignationCharProc(cFileOutputData, (const char)CRLF_CODE, cFileOutputNGDataLogout);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "出力データは：%s",
                    cFileOutputNGDataLogout);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                // 2行目
                sprintf_s(pcFileOutputData, DATASIZE, "%18s***** Level %d-->0\n", pcEtcChar, iLevel1 - 1);
                fprintf_s(pfOutputFile, "%s", pcFileOutputData);

                // 指定文字削除処理
                // 改行コードを削除した文字列をログに出力
                strncpy_s(cFileOutputData, sizeof(cFileOutputData), pcFileOutputData, sizeof(cFileOutputData) - 1);
                DeleteDesignationCharProc(cFileOutputData, (const char)CRLF_CODE, cFileOutputNGDataLogout);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "出力データは：%s",
                    cFileOutputNGDataLogout);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                // 分岐抜け
                break;
            }
            case tagPrintType::LEVELNG6:
            {
                // 制限エリアレベル『0』出力
                // 本来入退室の記録があるべき行に出力
                sprintf_s(pcFileOutputData, DATASIZE, pcOutputType, pcEtcChar, iLevel1);
                fprintf_s(pfOutputFile, "%s", pcFileOutputData);

                // 指定文字削除処理
                // 改行コードを削除した文字列をログに出力
                strncpy_s(cFileOutputData, sizeof(cFileOutputData), pcFileOutputData, sizeof(cFileOutputData) - 1);
                DeleteDesignationCharProc(cFileOutputData, (const char)CRLF_CODE, cFileOutputNGDataLogout);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "出力データは：%s",
                    cFileOutputNGDataLogout);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                // 分岐抜け
                break;
            }
            default:
            {
                // 上記以外
                // 文字列を出力
                sprintf_s(pcFileOutputData, DATASIZE, pcOutputType, pcEtcChar);
                fprintf_s(pfOutputFile, pcOutputType, pcFileOutputData);

                // 半角空白文字かチェック
                if (0 == strncmp(pcEtcChar, " ", 1))
                {
                    // 半角空白文字
                    // ログに『半角Space』と設定
                    sprintf_s(
                        m_cLogData,
                        sizeof(m_cLogData),
                        "出力データは：半角Space");
                }
                else
                {
                    // 取得した文字列を設定
                    sprintf_s(
                        m_cLogData,
                        sizeof(m_cLogData),
                        "出力データは：%s",
                        pcFileOutputData);
                }

                // データをログに出力
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                // 分岐抜け
                break;
            }
        }
    }

    // 例外処理
    catch (const ERRORCODETABLE eAllocErr)
    {
        // エラーメッセージ生成処理
            // エラーメッセージを出力
        pcFileOutputData = NULL;
        LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** データ出力メモリ領域確保異常終了 *****");
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** エラーメッセージ生成関数呼び出し *****");
        m_eErrorResultCode = eAllocErr;
        m_stLogData = CreateEventMessageProc(
            ERRORSET,
            m_stLogData.pcActionName,
            m_eErrorResultCode);
        LOGOUTPUTERROR(
            TRACEOUTPUTLOG,
            ERRORINFORMATION1,
            m_stLogData.pcMessage,
            m_stLogData.eErrorCode);
        sprintf_s(
            m_cLogData,
            sizeof(m_cLogData),
            "戻り値は：%d",
            m_stLogData.eErrorCode);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
    }

    // 後処理
    // データ出力メモリ領域を解放
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** データ出力メモリ領域解放 *****");
    SAFE_FREE_NEWALLOC_ARRAY(pcFileOutputData);

    // メモリ領域解放のメッセージをログに出力
    LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, "***** データ出力メモリ領域解放終了 *****");
    sprintf_s(
        m_cLogData,
        sizeof(m_cLogData),
        "%sの%s",
        m_stLogData.pcActionName,
        MEMORYRELEASESUCCESS);
    LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, m_cLogData);

    // 日時情報取得処理
    // 入退室データファイル出力終了日時取得
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 入退室データファイル出力終了 *****");
    m_stTimeData = g_clsRecOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData,
        sizeof(m_cLogData),
        "入退室データファイル出力終了日時：%s %s",
        (char *)m_stTimeData.strMachDatePlus.c_str(),
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
}

//********************************************
// 関数名：入退室データPC閲覧画面出力処理
// Function Name：PrintScreenProc
// 
// 処理概要：
// PC等閲覧画面へ出力タイプに基づいて入退室データを出力する
//      タイプ1：ドア情報
//      タイプ2：カードID及びユーザー名
//      タイプ3：可否情報
//      タイプ4：不整合による制限エリアレベルNG情報１
//      タイプ5：不整合による制限エリアレベルNG情報２
//      タイプ6：不整合による制限エリアレベルNG情報３
//      タイプ7：不整合による制限エリアレベルNG情報４
//      タイプ8：不整合による制限エリアレベルNG情報５
//      タイプ9：不整合による制限エリアレベルNG情報６
//      タイプ10：上記以外の出力情報（空白文字等）
// 
// 処理手順：
//   １．データ出力メモリ領域確保
//   ２．PC閲覧画面への出力
//   ３．データ出力メモリ領域解放
// 
// 引数：　 出力形式
// 　　　　 入退出状態
// 　　　　 入退室情報データ編集用構造体
//          出力文字列（空白等）
// 　　　　 データ出力タイプ
// 　　　　 制限エリアレベル情報１
// 　　　　 制限エリアレベル情報２
// 戻り値： なし
// 
// 補足事項
// ・データ出力メモリ領域確保に失敗した場合は
//     メモリ領域確保エラー（CM_MEMORYERROR（109））としてログに出力する
// 
// 
// 作成履歴
// 2020/9/25  s-saito   新規作成
// 
// 更新履歴
// 
// 
//********************************************
void CRecordOperations::PrintScreenProc(
    const char *pcOutputType,
    const char *pcAct,
    ENTRYEXITDATA stEData,
    const char *pcEtcChar,
    PRINTTYPETABLE eType,
    int iLevel1,
    int iLevel2)
{
    // 変数宣言
    char cScreenOutputData[DATASIZE + 1];
    char cScreenOutputNGDataLogout[DATASIZE + 1];
    char cScreenOutputNODataLogout[DATASIZE + 1];
    char *pcScreenOutputData = NULL;

    // 初期化
    memset(cScreenOutputData, 0x00, sizeof(cScreenOutputData));
    memset(cScreenOutputNGDataLogout, 0x00, sizeof(cScreenOutputNGDataLogout));
    memset(cScreenOutputNODataLogout, 0x00, sizeof(cScreenOutputNODataLogout));

    // 標準出力に日本語が表示されるようにロケールを日本に設定
    setlocale(LC_ALL, "japanese");

    // 前処理
    // 日時情報取得処理
    // 入退室データPC閲覧画面出力開始日時取得
    m_stTimeData = g_clsRecOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData,
        sizeof(m_cLogData),
        "入退室データPC閲覧画面出力開始日時：%s %s",
        (char *)m_stTimeData.strMachDatePlus.c_str(),
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 入退室データPC閲覧画面出力開始 *****");

    try
    {
        // データ出力メモリ領域確保
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** データ出力メモリ領域確保 *****");
        pcScreenOutputData = (char *)calloc(DATASIZE + 1, sizeof(char));
        m_stLogData = CreateEventMessageProc(
            ACTIONSET,
            "入退室データPC閲覧画面出力");

        // メモリ領域が確保出来たかチェック
        if (NULL == pcScreenOutputData)
        {
            // メモリ領域の確保失敗
            // 例外処理へスロー
            throw tagErrorCode::MEMORYERROR;
        }

        // メモリ領域確保成功のメッセージをログに出力
        LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, "***** データ出力メモリ領域確保正常終了 *****");
        sprintf_s(
            m_cLogData,
            sizeof(m_cLogData),
            "%sの%s",
            m_stLogData.pcActionName,
            MEMORYCREATESUCCESS);
        LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, m_cLogData);

        // データ出力タイプをログに出力
        sprintf_s(m_cLogData, sizeof(m_cLogData), "データ出力タイプは：%d", eType);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

        // 主処理
        // コンソール・ファイルへのデータ出力
        // 出力タイプにて出力方法を分岐
        switch (eType)
        {
            case tagPrintType::DOOR:
            {
                // 入退出記録
                // 取得した入退出記録を出力
                sprintf_s(
                    pcScreenOutputData,
                    DATASIZE,
                    pcOutputType,
                    stEData.cDate + 4,
                    stEData.cDate + 6,
                    stEData.cTime,
                    stEData.cTime + 2,
                    stEData.cDoor);
                printf_s("%s", pcScreenOutputData);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "PC閲覧画面のデータは：%s",
                    pcScreenOutputData);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                // 分岐抜け
                break;
            }
            case tagPrintType::CARDID:
            {
                // カードIDとユーザー名
                // 取得したカードIDとユーザー名を出力
                sprintf_s(
                    cScreenOutputData,
                    sizeof(cScreenOutputData),
                    pcOutputType,
                    stEData.cCardID,
                    stEData.cName);
                sprintf_s(pcScreenOutputData, DATASIZE, "%s", cScreenOutputData);
                printf_s("%s", pcScreenOutputData);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "PC閲覧画面のデータは：%s",
                    cScreenOutputData);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                // 分岐抜け
                break;
            }
            case tagPrintType::ACT:
            {
                // 入退出情報・可否情報
                // 取得した入退出情報・可否情報を出力
                sprintf_s(pcScreenOutputData, DATASIZE, pcOutputType, pcAct);
                printf_s("%s", pcScreenOutputData);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "PC閲覧画面のデータは：%s",
                    pcAct);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                // 分岐抜け
                break;
            }
            case tagPrintType::LEVELNG1:
            {
                // 制限エリアレベルNG出力
                // 本来入退室の記録があるべき行に出力
                sprintf_s(pcScreenOutputData, DATASIZE, pcOutputType, iLevel1, iLevel2, pcEtcChar);
                printf_s("%s", pcScreenOutputData);

                // 指定文字削除処理
                // 改行コードを削除した文字列をログに出力
                strncpy_s(
                    cScreenOutputData,
                    sizeof(cScreenOutputData),
                    pcScreenOutputData,
                    sizeof(cScreenOutputData) - 1);
                DeleteDesignationCharProc(cScreenOutputData, (const char)CRLF_CODE, cScreenOutputNGDataLogout);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "PC閲覧画面のデータは：%s",
                    cScreenOutputNGDataLogout);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                // 分岐抜け
                break;
            }
            case tagPrintType::LEVELNG2:
            {
                // 制限エリアレベルNG出力（連続して記録なし）
                // 入退出状態をログに出力
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "現在の入退出状態は：%s",
                    stEData.cDir);
                LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

                // 入退出状態をチェック
                if (0 == strncmp(stEData.cDir, NYUSHITSU, RECORD_DIR))
                {
                    // 入退出状態が『I』（入室）の場合
                    // 現在のドアレベル値がマイナスかチェック
                    if (0 > m_iNowDoorLevel)
                    {
                        // 現在のドアレベル値がマイナス値
                        // 本来入退室の記録があるべき行に2行昇順出力
                        // 1行目
                        sprintf_s(pcScreenOutputData, DATASIZE, pcOutputType, iLevel1, iLevel2 - 1, pcEtcChar);
                        printf_s("%s", pcScreenOutputData);

                        // 指定文字削除処理
                        // 改行コードを削除した文字列をログに出力
                        strncpy_s(
                            cScreenOutputData,
                            sizeof(cScreenOutputData),
                            pcScreenOutputData,
                            sizeof(cScreenOutputData) - 1);
                        DeleteDesignationCharProc(cScreenOutputData, (const char)CRLF_CODE, cScreenOutputNGDataLogout);
                        sprintf_s(
                            m_cLogData,
                            sizeof(m_cLogData),
                            "PC閲覧画面のデータは：%s",
                            cScreenOutputNGDataLogout);
                        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                        // 2行目
                        sprintf_s(pcScreenOutputData, DATASIZE, pcOutputType, iLevel1 + 1, iLevel2, pcEtcChar);
                        printf_s("%s", pcScreenOutputData);

                        // 指定文字削除処理
                        // 改行コードを削除した文字列をログに出力
                        strncpy_s(
                            cScreenOutputData,
                            sizeof(cScreenOutputData),
                            pcScreenOutputData,
                            sizeof(cScreenOutputData) - 1);
                        DeleteDesignationCharProc(cScreenOutputData, (const char)CRLF_CODE, cScreenOutputNGDataLogout);
                        sprintf_s(
                            m_cLogData,
                            sizeof(m_cLogData),
                            "PC閲覧画面のデータは：%s",
                            cScreenOutputNGDataLogout);
                        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
                    }
                    else
                    {
                        // 現在のドアレベル値がマイナス値ではない
                        // 本来入退室の記録があるべき行に2行昇順出力
                        // 1行目
                        sprintf_s(pcScreenOutputData, DATASIZE, pcOutputType, iLevel1, iLevel2 + 1, pcEtcChar);
                        printf_s("%s", pcScreenOutputData);

                        // 指定文字削除処理
                        // 改行コードを削除した文字列をログに出力
                        strncpy_s(
                            cScreenOutputData,
                            sizeof(cScreenOutputData),
                            pcScreenOutputData,
                            sizeof(cScreenOutputData) - 1);
                        DeleteDesignationCharProc(cScreenOutputData, (const char)CRLF_CODE, cScreenOutputNGDataLogout);
                        sprintf_s(
                            m_cLogData,
                            sizeof(m_cLogData),
                            "PC閲覧画面のデータは：%s",
                            cScreenOutputNGDataLogout);
                        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                        // 2行目
                        sprintf_s(pcScreenOutputData, DATASIZE, pcOutputType, iLevel1 - 1, iLevel2, pcEtcChar);
                        printf_s("%s", pcScreenOutputData);

                        // 指定文字削除処理
                        // 改行コードを削除した文字列をログに出力
                        strncpy_s(
                            cScreenOutputData,
                            sizeof(cScreenOutputData),
                            pcScreenOutputData,
                            sizeof(cScreenOutputData) - 1);
                        DeleteDesignationCharProc(cScreenOutputData, (const char)CRLF_CODE, cScreenOutputNGDataLogout);
                        sprintf_s(
                            m_cLogData,
                            sizeof(m_cLogData),
                            "PC閲覧画面のデータは：%s",
                            cScreenOutputNGDataLogout);
                        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
                    }
                }
                else if (0 == strncmp(stEData.cDir, TAISHITSU, RECORD_DIR))
                {
                    // 入退出状態が『O』（退室）の場合
                    // 本来入退室の記録があるべき行に2行降順出力
                    // 1行目
                    sprintf_s(pcScreenOutputData, DATASIZE, pcOutputType, iLevel1, iLevel2 - 1, pcEtcChar);
                    printf_s("%s", pcScreenOutputData);

                    // 指定文字削除処理
                    // 改行コードを削除した文字列をログに出力
                    strncpy_s(
                        cScreenOutputData,
                        sizeof(cScreenOutputData),
                        pcScreenOutputData,
                        sizeof(cScreenOutputData) - 1);
                    DeleteDesignationCharProc(cScreenOutputData, (const char)CRLF_CODE, cScreenOutputNGDataLogout);
                    sprintf_s(
                        m_cLogData,
                        sizeof(m_cLogData),
                        "PC閲覧画面のデータは：%s",
                        cScreenOutputNGDataLogout);
                    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                    // 2行目
                    sprintf_s(pcScreenOutputData, DATASIZE, pcOutputType, iLevel1 + 1, iLevel2, pcEtcChar);
                    printf_s("%s", pcScreenOutputData);

                    // 指定文字削除処理
                    // 改行コードを削除した文字列をログに出力
                    strncpy_s(
                        cScreenOutputData,
                        sizeof(cScreenOutputData),
                        pcScreenOutputData,
                        sizeof(cScreenOutputData) - 1);
                    DeleteDesignationCharProc(cScreenOutputData, (const char)CRLF_CODE, cScreenOutputNGDataLogout);
                    sprintf_s(
                        m_cLogData,
                        sizeof(m_cLogData),
                        "PC閲覧画面のデータは：%s",
                        cScreenOutputNGDataLogout);
                    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
                }

                // 分岐抜け
                break;
            }
            case tagPrintType::LEVELNG3:
            {
                // 制限エリアレベルNG出力（入室記録なし）
                // 本来入退室の記録があるべき行に3行昇順出力
                // 1行目
                sprintf_s(pcScreenOutputData, DATASIZE, "***** Level 0-->%d\n%18s", iLevel1 - 2, pcEtcChar);
                printf_s("%s", pcScreenOutputData);

                // 指定文字削除処理
                // 改行コードを削除した文字列をログに出力
                strncpy_s(
                    cScreenOutputData,
                    sizeof(cScreenOutputData),
                    pcScreenOutputData,
                    sizeof(cScreenOutputData) - 1);
                DeleteDesignationCharProc(cScreenOutputData, (const char)CRLF_CODE, cScreenOutputNGDataLogout);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "PC閲覧画面のデータは：%s",
                    cScreenOutputNGDataLogout);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                // 2行目
                sprintf_s(pcScreenOutputData, DATASIZE, pcOutputType, iLevel1 - 2, iLevel1 - 1, pcEtcChar);
                printf_s("%s", pcScreenOutputData);

                // 指定文字削除処理
                // 改行コードを削除した文字列をログに出力
                strncpy_s(
                    cScreenOutputData,
                    sizeof(cScreenOutputData),
                    pcScreenOutputData,
                    sizeof(cScreenOutputData) - 1);
                DeleteDesignationCharProc(cScreenOutputData, (const char)CRLF_CODE, cScreenOutputNGDataLogout);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "PC閲覧画面のデータは：%s",
                    cScreenOutputNGDataLogout);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                // 3行目
                sprintf_s(pcScreenOutputData, DATASIZE, pcOutputType, iLevel1 - 1, iLevel1, pcEtcChar);
                printf_s("%s", pcScreenOutputData);

                // 指定文字削除処理
                // 改行コードを削除した文字列をログに出力
                strncpy_s(
                    cScreenOutputData,
                    sizeof(cScreenOutputData),
                    pcScreenOutputData,
                    sizeof(cScreenOutputData) - 1);
                DeleteDesignationCharProc(cScreenOutputData, (const char)CRLF_CODE, cScreenOutputNGDataLogout);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "PC閲覧画面のデータは：%s",
                    cScreenOutputNGDataLogout);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                // 分岐抜け
                break;
            }
            case tagPrintType::LEVELNG4:
            {
                // 制限エリアレベルNG出力（退室記録なし）
                // 本来入退室の記録があるべき行に3行降順出力
                // 1行目
                sprintf_s(pcScreenOutputData, DATASIZE, pcOutputType, pcEtcChar, iLevel1, iLevel1 - 1);
                printf_s("%s", pcScreenOutputData);

                // 指定文字削除処理
                // 改行コードを削除した文字列をログに出力
                strncpy_s(
                    cScreenOutputData,
                    sizeof(cScreenOutputData),
                    pcScreenOutputData,
                    sizeof(cScreenOutputData) - 1);
                DeleteDesignationCharProc(cScreenOutputData, (const char)CRLF_CODE, cScreenOutputNGDataLogout);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "PC閲覧画面のデータは：%s",
                    cScreenOutputNGDataLogout);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                // 2行目
                sprintf_s(pcScreenOutputData, DATASIZE, pcOutputType, pcEtcChar, iLevel1 - 1, iLevel1 - 2);
                printf_s("%s", pcScreenOutputData);

                // 指定文字削除処理
                // 改行コードを削除した文字列をログに出力
                strncpy_s(
                    cScreenOutputData,
                    sizeof(cScreenOutputData),
                    pcScreenOutputData,
                    sizeof(cScreenOutputData) - 1);
                DeleteDesignationCharProc(cScreenOutputData, (const char)CRLF_CODE, cScreenOutputNGDataLogout);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "PC閲覧画面のデータは：%s",
                    cScreenOutputNGDataLogout);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                // 3行目
                sprintf_s(pcScreenOutputData, DATASIZE, "%18s***** Level %d-->0\n", pcEtcChar, iLevel1 - 2);
                printf_s("%s", pcScreenOutputData);

                // 指定文字削除処理
                // 改行コードを削除した文字列をログに出力
                strncpy_s(
                    cScreenOutputData,
                    sizeof(cScreenOutputData),
                    pcScreenOutputData,
                    sizeof(cScreenOutputData) - 1);
                DeleteDesignationCharProc(cScreenOutputData, (const char)CRLF_CODE, cScreenOutputNGDataLogout);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "PC閲覧画面のデータは：%s",
                    cScreenOutputNGDataLogout);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                // 分岐抜け
                break;
            }
            case tagPrintType::LEVELNG5:
            {
                // 制限エリアレベルNG出力
                // 本来入退室の記録があるべき行に2行降順出力
                // 1行目
                sprintf_s(pcScreenOutputData, DATASIZE, pcOutputType, pcEtcChar, iLevel1, iLevel1 - 1);
                printf_s("%s", pcScreenOutputData);

                // 指定文字削除処理
                // 改行コードを削除した文字列をログに出力
                strncpy_s(
                    cScreenOutputData,
                    sizeof(cScreenOutputData),
                    pcScreenOutputData,
                    sizeof(cScreenOutputData) - 1);
                DeleteDesignationCharProc(cScreenOutputData, (const char)CRLF_CODE, cScreenOutputNGDataLogout);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "PC閲覧画面のデータは：%s",
                    cScreenOutputNGDataLogout);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                // 2行目
                sprintf_s(pcScreenOutputData, DATASIZE, "%18s***** Level %d-->0\n", pcEtcChar, iLevel1 - 1);
                printf_s("%s", pcScreenOutputData);

                // 指定文字削除処理
                // 改行コードを削除した文字列をログに出力
                strncpy_s(
                    cScreenOutputData,
                    sizeof(cScreenOutputData),
                    pcScreenOutputData,
                    sizeof(cScreenOutputData) - 1);
                DeleteDesignationCharProc(cScreenOutputData, (const char)CRLF_CODE, cScreenOutputNGDataLogout);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "PC閲覧画面のデータは：%s",
                    cScreenOutputNGDataLogout);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                // 分岐抜け
                break;
            }
            case tagPrintType::LEVELNG6:
            {
                // 制限エリアレベルNG出力
                // 本来入退室の記録があるべき行に出力
                sprintf_s(pcScreenOutputData, DATASIZE, pcOutputType, pcEtcChar, iLevel1);
                printf_s("%s", pcScreenOutputData);

                // 指定文字削除処理
                // 改行コードを削除した文字列をログに出力
                strncpy_s(
                    cScreenOutputData,
                    sizeof(cScreenOutputData),
                    pcScreenOutputData,
                    sizeof(cScreenOutputData) - 1);
                DeleteDesignationCharProc(cScreenOutputData, (const char)CRLF_CODE, cScreenOutputNGDataLogout);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "PC閲覧画面のデータは：%s",
                    cScreenOutputNGDataLogout);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                // 分岐抜け
                break;
            }
            case tagPrintType::DUMMYDATA:
            {
                // ダミーデータ
                // 文字列を出力
                sprintf_s(pcScreenOutputData, DATASIZE, pcOutputType, pcEtcChar);
                printf_s(pcOutputType, pcScreenOutputData);

                // 指定文字削除処理
                // 改行コードを削除した文字列をログに出力
                strncpy_s(
                    cScreenOutputData,
                    sizeof(cScreenOutputData),
                    pcScreenOutputData,
                    sizeof(cScreenOutputData) - 1);
                DeleteDesignationCharProc(cScreenOutputData, (const char)CRLF_CODE, cScreenOutputNODataLogout);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "PC閲覧画面のデータは：%s",
                    cScreenOutputNODataLogout);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                // 分岐抜け
                break;
            }
            default:
            {
                // 上記以外
                // 文字列を出力
                sprintf_s(pcScreenOutputData, DATASIZE, pcOutputType, pcEtcChar);
                printf_s(pcOutputType, pcScreenOutputData);

                // 半角空白文字かチェック
                if (0 == strncmp(pcEtcChar, " ", 1))
                {
                    // 半角空白文字
                    // ログに『半角Space』と設定
                    sprintf_s(
                        m_cLogData,
                        sizeof(m_cLogData),
                        "PC閲覧画面のデータは：半角Space");
                }
                else
                {
                    // 取得した文字列を設定
                    sprintf_s(
                        m_cLogData,
                        sizeof(m_cLogData),
                        "PC閲覧画面のデータは：%s",
                        pcScreenOutputData);
                }

                // データをログに出力
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                // 分岐抜け
                break;
            }
        }
    }

    // 例外処理
    catch (const ERRORCODETABLE eAllocErr)
    {
        // エラーメッセージ生成処理
        // エラーメッセージを出力
        pcScreenOutputData = NULL;
        LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** データ出力メモリ領域確保異常終了 *****");
        LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** エラーメッセージ生成関数呼び出し *****");
        m_eErrorResultCode = eAllocErr;
        m_stLogData = CreateEventMessageProc(
            ERRORSET,
            m_stLogData.pcActionName,
            m_eErrorResultCode);
        LOGOUTPUTERROR(
            TRACEOUTPUTLOG,
            ERRORINFORMATION1,
            m_stLogData.pcMessage,
            m_stLogData.eErrorCode);
        sprintf_s(
            m_cLogData,
            sizeof(m_cLogData),
            "戻り値は：%d",
            m_stLogData.eErrorCode);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
    }

    // 後処理
    // データ出力メモリ領域を解放
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** データ出力メモリ領域解放 *****");
#ifdef _DEBUG
    SAFE_FREE_DEBUG(pcScreenOutputData);
#else
    SAFE_FREE(pcScreenOutputData);
#endif // _DEBUG

    // メモリ領域解放のメッセージをログに出力
    LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, "***** データ出力メモリ領域解放終了 *****");
    sprintf_s(
        m_cLogData,
        sizeof(m_cLogData),
        "%sの%s", m_stLogData.pcActionName,
        MEMORYRELEASESUCCESS);
    LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, m_cLogData);

    // 日時情報取得処理
    // 入退室データPC閲覧画面出力終了日時取得
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 入退室データPC閲覧画面出力終了 *****");
    m_stTimeData = g_clsRecOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData,
        sizeof(m_cLogData),
        "入退室データPC閲覧画面出力終了日時：%s %s",
        (char *)m_stTimeData.strMachDatePlus.c_str(),
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
}
