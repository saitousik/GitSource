//============================================
// 入退室状況印字制御ソースファイル
// DllEntryExitDataControl.cpp
// 
// 機能概要
// フロアのセキュリティによるドアの入退室読み込みデータファイルを読み込み、
// テキスト等での帳票形式で入退室書き込みデータファイルへ印字する
// 
// 機能内容
// １．入退室状況印字制御メイン処理
// ２．入退室状況印字制御開始処理
// ３．入退室状況印字制御終了処理
// 
// 処理手順
//  １．入退室読み込みデータファイルの読み込み
//  ２．入退室状況のチェック
//  ３．入退室状況に対するPC等閲覧画面への表示
//  ４．入退室状況に対する入退室書き込みデータファイルへの書き込み
// 
// 補足事項
// ・当機能の詳細や状態については共通機能を呼び出しログ情報としてログ出力ファイルに出力する
// ・当機能では上記の他共通機能を呼び出しバックアップファイル生成や絶対パスへの変換等も行う
// ・当機能ではメモリリークに対するCRTメモリ検知イベントハンドラ処理も行う
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
// 各ヘッダーインクルード・クラス定義
//☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★
// インクルード定義
#include "DllEntryExitDataControl.h"

// クラス定義
CLogFile g_clsMainLogFile;
CFileOperations g_clsMainFileOpe;
CSubOperations g_clsMainSubOpe;
CSubOperations *g_pclsMainSubOpe;
CFileOperations *g_pclsMainFileOpe;
CRecordOperations *g_pclsMainRecOpe;

// マクロ定義
#define LOGOUTPUT(iOutputType, iTraceOutputLogType, cMessageInfo) \
    g_clsMainLogFile.LogOutputControlProc(LOGARGS, iOutputType, iTraceOutputLogType, cMessageInfo)
#define LOGOUTPUTERROR(iOutputType, iTraceOutputLogType, cErrorMessageInfo, eErrorInfoCode) \
    g_clsMainLogFile.LogOutputControlProc(LOGARGS, iOutputType, iTraceOutputLogType, cErrorMessageInfo, eErrorInfoCode)

// グローバル変数
static map<int, ERRORCODETABLE> g_maStartEndErrorCode;
int g_iParseArgv;
vector<string> g_vecParseArgc;
FILENAMEINFORMATION g_stFileNameInfo;
map<FILETYPETABLE, string> g_maFileName;

//☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★
// 各機能（API）定義
//☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★
//********************************************
// 関数名：入退室状況印字制御メイン処理
// Function Name：DataControlMain
// 
// 処理概要：
// 入退室読み込みデータファイルを読み込み、
// PC閲覧画面及び入退室書き込みデータファイルへ印字する
// 
// 処理手順：
//   １．入退室状況印字制御開始処理
//   ２．入退室データ編集情報取得処理（１件目）（データがない場合はダミーデータを設定）
//   ３．見出し出力
// （以降ここから全てのデータが取得出来るまでループ）
//   ４．入退室データ編集情報出力処理
//   ５．入退室データ編集情報取得処理（２件目以降）
//   ６．制限エリアレベルクリア処理
// （ここまでループ）
//   ７．入退室状況印字制御終了処理
//   ８．エラーメッセージPC閲覧画面及びトレースログ出力（エラーが発生した場合のみ）
// 
// 引数：      コマンド数
//             コマンド情報
// 戻り値：    実行結果（入退室状況印字制御初期処理／入退室状況印字制御終了処理の戻り値）
//             正常終了：EXIT_SUCCESS（0）
//             異常終了：入退室状況印字制御初期処理／入退室状況印字制御終了処理の戻り値
//                       入退室データ編集情報取得処理の戻り値
//                         レコードなし：CM_NODATA（106）
//                         レコード不正：CM_DATAFRAUD（107）
//                         レコード取得エラー：CM_DATAERROR（108）
// 
// 補足事項
// ・ファイルへの見出し出力は新規作成のみとする
// ・入退室読み込みデータファイルの全てのデータが取得出来るまで４～６を繰り返す
// 
// 作成履歴
// 2020/9/25  s-saito   新規作成
// 
// 更新履歴
// 
// 
//********************************************
int DataControlMain(
    int iArgv,
    vector<string> vecArgc)
{
    // 変数宣言
    int iRtn = EXIT_SUCCESS;
    int iStartResult = EXIT_SUCCESS;
    int iGetDataResult = EXIT_SUCCESS;
    int iEndResult = EXIT_SUCCESS;
    int iFileCopyResult = EXIT_SUCCESS;
    char cInitData[DATASIZE + 1];
    char cInitDataDateTime[DATASIZE + 1];
    FILETYPETABLE eFileType = tagFileType::NORMAL;
    PRINTTYPETABLE ePrintType = tagPrintType::DUMMY;
    ERRORCODETABLE eInitResultCode = tagErrorCode::SUCCESS;
    ERRORCODETABLE eEndResultCode = tagErrorCode::SUCCESS;
    ENTRYEXITDATA* pstEntryExitEditData = NULL;
    bool bSkipFlag = true;
    int iFileEndFlag = FILEEND_OFF;
    size_t siDataPrintCnt = 0;
    int iParamCnt = 0;
    size_t siRecCnt = 0;
    FILE* pfEntryExitFile = NULL;
    FILE* pfOutputFile = NULL;
    int iStatResult = 0;
    long lRecCnt = 0;

    // 初期化
    memset(cInitData, 0x00, sizeof(cInitData));
    memset(cInitDataDateTime, 0x00, sizeof(cInitDataDateTime));
    g_clsMainLogFile.m_eErrorResultCode = tagErrorCode::SUCCESS;

    // 前処理
    try
    {
        // 絶対パス変換ファイル名取得処理
        // トレースログ出力ファイル名を絶対パス付き状態で取得
        eFileType = tagFileType::FILETYPE3;
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 絶対パス変換ファイル名取得関数呼び出し *****");
        g_stFileNameInfo.strTraceLogName = g_clsMainFileOpe.GetConvertFileNameProc(
            eFileType, 
            "", 
            vecArgc.at(0).c_str(), 
            (char *)LOGFILEPATH, 
            (char *)LOGFILENAME);

        // 絶対パス変換ファイル名取得処理
        // データログ出力ファイル名を絶対パス付き状態で取得
        eFileType = tagFileType::FILETYPE4;
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 絶対パス変換ファイル名取得関数呼び出し *****");
        g_stFileNameInfo.strDataLogName = g_clsMainFileOpe.GetConvertFileNameProc(
            eFileType, 
            "", 
            vecArgc.at(0).c_str(), 
            (char *)LOGFILEPATH, 
            (char *)DATALOGFILENAME);

        // 絶対パス変換ファイル名取得処理
        // 不正データファイル名を絶対パス付き状態で取得
        eFileType = tagFileType::FILETYPE9;
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 絶対パス変換ファイル名取得関数呼び出し *****");
        g_stFileNameInfo.strFraudDataName = g_clsMainFileOpe.GetConvertFileNameProc(
            eFileType,
            "",
            vecArgc.at(0).c_str(),
            (char *)WRITEPATH,
            (char *)FRAUDDATAFILENAME);

        // 取得したログ出力ファイル名に基づいて各クラス生成
        // ログ出力クラス
        CLogFile clsMainLogFile(
            g_stFileNameInfo.strTraceLogName, 
            g_stFileNameInfo.strDataLogName);
        g_clsMainLogFile.m_strTraceLogFileName = clsMainLogFile.m_strTraceLogFileName;
        g_clsMainLogFile.m_strDataLogFileName = clsMainLogFile.m_strDataLogFileName;

        // 共通操作クラス
        g_pclsMainSubOpe = DBG_NEW CSubOperations(
            g_stFileNameInfo.strTraceLogName, 
            g_stFileNameInfo.strDataLogName);

        // ファイル操作クラス
        g_pclsMainFileOpe = DBG_NEW CFileOperations(
            g_stFileNameInfo.strTraceLogName, 
            g_stFileNameInfo.strDataLogName, 
            g_stFileNameInfo.strBackupName);

        // 入退室レコード操作クラス
        g_pclsMainRecOpe = DBG_NEW CRecordOperations(
            g_stFileNameInfo.strTraceLogName, 
            g_stFileNameInfo.strDataLogName);

        // 日時情報取得処理
        // 入退室状況印字処理開始日時表示
        g_clsMainLogFile.m_stTimeData = g_clsMainLogFile.GetDateTimeProc();
        sprintf_s(
            g_clsMainLogFile.m_cLogData, 
            sizeof(g_clsMainLogFile.m_cLogData), 
            "入退室状況印字処理開始日時：%s %s", 
            (char *)g_clsMainLogFile.m_stTimeData.strMachDatePlus.c_str(), 
            (char *)g_clsMainLogFile.m_stTimeData.strMachTimePlus.c_str());
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, g_clsMainLogFile.m_cLogData);
#ifdef SCREEN
        printf_s("%s\n", g_clsMainLogFile.m_cLogData);
        printf_s("***** 入退室状況印字処理開始 *****\n");
#endif // SCREEN
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 入退室状況印字処理開始 *****");

        // 日時情報取得処理
        // ここで日付及び時刻の設定状態をデータログに出力
        // ここ以外ではマシン日時の取得のみ行う
        g_clsMainLogFile.m_stTimeData = g_pclsMainSubOpe->GetDateTimeProc();

        // パラメータ情報のコマンド情報をログに出力
        sprintf_s(g_clsMainLogFile.m_cLogData, sizeof(g_clsMainLogFile.m_cLogData), "パラメータ数は： %d", iArgv);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, "***** コマンド情報詳細 *****");
        for (int iCnt = 0; iCnt < iArgv; iCnt++)
        {
            // コマンド情報をログに出力
            sprintf_s(
                g_clsMainLogFile.m_cLogData, 
                sizeof(g_clsMainLogFile.m_cLogData), 
                "%d 個目のコマンド情報は： %s", 
                iCnt + 1, 
                vecArgc.at(iCnt).c_str());
            LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, g_clsMainLogFile.m_cLogData);
        }

        // 絶対パス変換ファイル名取得処理
        // アプリケーション名を絶対パス付き状態で取得
        eFileType = tagFileType::FILETYPE8;
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 絶対パス変換ファイル名取得関数呼び出し *****");

        // パラメータ情報のコマンド情報をチェック
        if (0 != strncmp(
            vecArgc.at(static_cast<size_t>(iArgv) - 1).c_str(), 
            ".", 
            vecArgc.at(static_cast<size_t>(iArgv) - 1).size()) &&
            0 != strncmp(
                vecArgc.at(static_cast<size_t>(iArgv) - 1).c_str(), 
                ".\\", 
                vecArgc.at(static_cast<size_t>(iArgv) - 1).size()))
        {
            // コマンド情報の末尾の変換指定文字列がない
            // 『.』指定にてアプリケーション名を取得
            g_stFileNameInfo.strModuleName = g_pclsMainFileOpe->GetConvertFileNameProc(
                eFileType, 
                ".", 
                vecArgc.at(0).c_str());

            // パラメータ数を1マイナス
            iParamCnt = iArgv - 1;
            sprintf_s(
                g_clsMainLogFile.m_cLogData, 
                sizeof(g_clsMainLogFile.m_cLogData), 
                "パラメータ数は： %d ", 
                iParamCnt);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
        }
        else
        {
            // コマンド情報の末尾の変換指定文字列がある
            // コマンド情報の末尾の変換指定文字列をチェック
            if (0 == strncmp(
                vecArgc.at(static_cast<size_t>(iArgv) - 1).c_str(), 
                ".", 
                vecArgc.at(static_cast<size_t>(iArgv) - 1).size()))
            {
                // 変換指定文字列が『.』
                // 『.』指定にてアプリケーション名を取得
                g_stFileNameInfo.strModuleName = g_pclsMainFileOpe->GetConvertFileNameProc(
                    eFileType, 
                    ".", 
                    vecArgc.at(0).c_str());
            }
            else
            {
                // 変換指定文字列が『.\\』
                // 『.\\』指定にてアプリケーション名を取得
                g_stFileNameInfo.strModuleName = g_pclsMainFileOpe->GetConvertFileNameProc(
                    eFileType, 
                    ".\\", 
                    vecArgc.at(0).c_str());
            }

            // パラメータ数を2マイナス
            iParamCnt = iArgv - 2;
            sprintf_s(
                g_clsMainLogFile.m_cLogData, 
                sizeof(g_clsMainLogFile.m_cLogData), 
                "パラメータ数は： %d ", 
                iParamCnt);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
        }

        // 各ファイル名をPC等閲覧画面出力
#ifdef SCREEN
        // 実行アプリケーション名
        printf_s(
            "%s名：%s\n",
            g_maFileName.at(tagFileType::FILETYPE8).c_str(),
            g_stFileNameInfo.strModuleName.c_str());

        // トレースログ出力ファイル名
        printf_s(
            "%s名：%s\n",
            g_maFileName.at(tagFileType::FILETYPE3).c_str(),
            g_stFileNameInfo.strTraceLogName.c_str());

        // データログ出力ファイル名
        printf_s(
            "%s名：%s\n",
            g_maFileName.at(tagFileType::FILETYPE4).c_str(),
            g_stFileNameInfo.strDataLogName.c_str());
#endif // SCREEN

        // 各ファイル名をログ出力
        // 実行アプリケーション名
        sprintf_s(
            g_clsMainLogFile.m_cLogData,
            sizeof(g_clsMainLogFile.m_cLogData),
            "%s名：%s",
            g_maFileName.at(tagFileType::FILETYPE8).c_str(),
            g_stFileNameInfo.strModuleName.c_str());
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

        // トレースログ出力ファイル名
        sprintf_s(
            g_clsMainLogFile.m_cLogData,
            sizeof(g_clsMainLogFile.m_cLogData),
            "%s名：%s",
            g_maFileName.at(tagFileType::FILETYPE3).c_str(),
            g_stFileNameInfo.strTraceLogName.c_str());
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

        // データログ出力ファイル名
        sprintf_s(
            g_clsMainLogFile.m_cLogData,
            sizeof(g_clsMainLogFile.m_cLogData),
            "%s名：%s",
            g_maFileName.at(tagFileType::FILETYPE4).c_str(),
            g_stFileNameInfo.strDataLogName.c_str());
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

        // 日時情報取得処理
        // 入退室状況印字制御メイン処理開始日時出力
        g_clsMainLogFile.m_stTimeData = g_clsMainLogFile.GetDateTimeProc();
        sprintf_s(
            g_clsMainLogFile.m_cLogData, 
            sizeof(g_clsMainLogFile.m_cLogData), 
            "入退室状況印字制御メイン処理開始日時：%s %s", 
            (char *)g_clsMainLogFile.m_stTimeData.strMachDatePlus.c_str(), 
            (char *)g_clsMainLogFile.m_stTimeData.strMachTimePlus.c_str());
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, g_clsMainLogFile.m_cLogData);
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 入退室状況印字制御メイン処理開始 *****");

        // ドアレベル情報構造体のメンバの値を出力
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, "***** ドアレベル情報の初期状態 *****");
        sprintf_s(
            g_clsMainLogFile.m_cLogData, 
            sizeof(g_clsMainLogFile.m_cLogData), 
            "通過したドアレベル：%d", 
            g_pclsMainRecOpe->m_stEntryExitInfo.stDoorLevelData.iDoorLevel);
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, g_clsMainLogFile.m_cLogData);
        sprintf_s(
            g_clsMainLogFile.m_cLogData, 
            sizeof(g_clsMainLogFile.m_cLogData), 
            "通過前ドアレベル：%d", 
            g_pclsMainRecOpe->m_stEntryExitInfo.stDoorLevelData.iBeforeLevel);
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, g_clsMainLogFile.m_cLogData);
        sprintf_s(
            g_clsMainLogFile.m_cLogData, 
            sizeof(g_clsMainLogFile.m_cLogData), 
            "通過後ドアレベル：%d", 
            g_pclsMainRecOpe->m_stEntryExitInfo.stDoorLevelData.iAfterLevel);
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, g_clsMainLogFile.m_cLogData);
        sprintf_s(
            g_clsMainLogFile.m_cLogData, 
            sizeof(g_clsMainLogFile.m_cLogData), 
            "前レコード通過後ドアレベル：%d", 
            g_pclsMainRecOpe->m_stEntryExitInfo.stDoorLevelData.iLevel);
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, g_clsMainLogFile.m_cLogData);

        // 入退室状況印字制御開始処理
        // ファイルのオープン等を実行
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 入退室状況印字制御開始処理関数呼び出し *****");
        iStartResult = DataControlStartProc(iParamCnt, vecArgc, &pfEntryExitFile, &pfOutputFile, &iFileCopyResult);
        sprintf_s(g_clsMainLogFile.m_cLogData, sizeof(g_clsMainLogFile.m_cLogData), "パラメータ数は：%d", iParamCnt);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
        sprintf_s(g_clsMainLogFile.m_cLogData, sizeof(g_clsMainLogFile.m_cLogData), "戻り値は：%d", iStartResult);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

        // 入退室状況印字制御開始処理に成功したかチェック
        if (EXIT_SUCCESS != iStartResult)
        {
            // 入退室状況印字制御開始処理失敗
            // 入退室状況印字制御開始処理失敗のメッセージをログに出力して後処理へ移動
            g_maStartEndErrorCode.insert(make_pair(iStartResult, g_clsMainLogFile.m_eErrorResultCode));
            LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** 入退室状況印字制御開始処理異常終了 *****");
            LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, INITFAILED);
            goto END;
        }
        else
        {
            // 入退室状況印字制御初期処理成功
            // 入退室状況印字制御初期処理成功のメッセージをログに出力
            LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, "***** 入退室状況印字制御開始処理正常終了 *****");
            LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, INITSUCCESS);
        }

        // 入退室読み込みデータファイル名及び入退室書き込みデータファイル名をPC等閲覧画面出力
#ifdef SCREEN
        // 入退室読み込みデータファイル名
        printf_s(
            "%s名：%s\n",
            g_maFileName.at(tagFileType::FILETYPE1).c_str(),
            g_stFileNameInfo.strReadName.c_str());

        // 入退室書き込みデータファイル名
        printf_s(
            "%s名：%s\n\n",
            g_maFileName.at(tagFileType::FILETYPE2).c_str(),
            g_stFileNameInfo.strWriteName.c_str());
#endif // SCREEN

        // 各ファイル名をログ出力
        // 入退室読み込みデータファイル名
        sprintf_s(
            g_clsMainLogFile.m_cLogData,
            sizeof(g_clsMainLogFile.m_cLogData),
            "%s名：%s",
            g_maFileName.at(tagFileType::FILETYPE1).c_str(),
            g_stFileNameInfo.strReadName.c_str());
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

        // 入退室書き込みデータファイル名
        sprintf_s(
            g_clsMainLogFile.m_cLogData,
            sizeof(g_clsMainLogFile.m_cLogData),
            "%s名：%s",
            g_maFileName.at(tagFileType::FILETYPE2).c_str(),
            g_stFileNameInfo.strWriteName.c_str());
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

        // 主処理
        // 入退室データ編集情報取得処理
        // 1件目のレコードを取得
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 入退室データ編集情報取得関数呼び出し *****");
        g_clsMainLogFile.m_stLogData = g_pclsMainSubOpe->CreateEventMessageProc(ACTIONSET, "入退室データ編集情報の");
        pstEntryExitEditData = g_pclsMainRecOpe->GetEditDataProc(
            pfEntryExitFile,
            pfOutputFile,
            &bSkipFlag,
            &iFileEndFlag,
            &iGetDataResult, 
            &lRecCnt);
        sprintf_s(
            g_clsMainLogFile.m_cLogData, 
            sizeof(g_clsMainLogFile.m_cLogData), 
            "ファイル終了フラグ：%d 入退室レコード取得結果：%d", 
            iFileEndFlag, 
            iGetDataResult);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
        sprintf_s(
            g_clsMainLogFile.m_cLogData, 
            sizeof(g_clsMainLogFile.m_cLogData), 
            "スキップ制御フラグ：%s", 
            bSkipFlag ? "true" : "false");
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

        // 入退室レコードの取得結果をチェック
        if (CM_DATAFRAUD == iGetDataResult)
        {
            // １件目のデータが不正
            // エラーメッセージ生成処理
            // エラーメッセージをログに出力
            LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** エラーメッセージ生成関数呼び出し *****");
            g_clsMainLogFile.m_stLogData = g_pclsMainSubOpe->CreateEventMessageProc(
                ERRORSET, 
                g_clsMainLogFile.m_stLogData.pcActionName, 
                g_clsMainLogFile.m_eErrorResultCode);
            LOGOUTPUTERROR(
                TRACEOUTPUTLOG,
                ERRORINFORMATION1, 
                g_clsMainLogFile.m_stLogData.pcMessage, 
                g_clsMainLogFile.m_stLogData.eErrorCode);
            sprintf_s(g_clsMainLogFile.m_cLogData, sizeof(g_clsMainLogFile.m_cLogData), "戻り値は：%d", iGetDataResult);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

            // 処理継続メッセージをログに出力
            // データ不備というエラーでは行うが当機能に対する直接の影響はないため処理を継続する
            LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** 入退室データ編集情報取得関数呼び出しデータ不備 *****");
            LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, ERRORPROCCONTINUED);
        }
        else
        {
            // 出力対象のレコードがある
            // 取得した1レコード分の入退室データ編集情報を管理用の入退室情報データ編集用構造体へ格納
            // （deqEntryExitEditData変数はC++言語コンテナdeque定義）
            g_pclsMainRecOpe->m_stEntryExitInfo.deqEntryExitEditData.push_back(*pstEntryExitEditData);

            // レコード取得成功のメッセージをログに出力
            LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, "***** 入退室データ編集情報取得関数呼び出し正常終了 *****");
            LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, GETRECORDSUCCESS);
            LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 入退室レコードが取得出来たので処理を継続 *****");
        }

        // 入退室書き込みデータファイルのファイル状態を取得
        sprintf_s(
            g_clsMainLogFile.m_cLogData, 
            sizeof(g_clsMainLogFile.m_cLogData), 
            "***** %s状態取得開始 *****", 
            g_maFileName.at(tagFileType::FILETYPE2).c_str());
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, g_clsMainLogFile.m_cLogData);
        iStatResult = g_pclsMainRecOpe->CheckFileStatProc(
            g_stFileNameInfo.strWriteName.c_str(), 
            &g_pclsMainRecOpe->m_stStat);
        sprintf_s(
            g_clsMainLogFile.m_cLogData, 
            sizeof(g_clsMainLogFile.m_cLogData), 
            "%s名：%s", 
            g_maFileName.at(tagFileType::FILETYPE2).c_str(), 
            g_stFileNameInfo.strWriteName.c_str());
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
        sprintf_s(g_clsMainLogFile.m_cLogData, sizeof(g_clsMainLogFile.m_cLogData), "状態結果は：%d", iStatResult);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
        g_clsMainLogFile.m_stLogData = g_pclsMainSubOpe->CreateEventMessageProc(
            ACTIONSET, 
            g_stFileNameInfo.strWriteName.c_str());

        // 状態結果をチェック
        if (EXIT_SUCCESS != iStatResult)
        {
            // ファイルの状態取得に失敗
            // 入退室情報データ編集用構造体メモリ領域を解放
            LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 入退室情報データ編集用構造体メモリ領域解放 *****");
#ifdef _DEBUG
            SAFE_FREE_DEBUG(pstEntryExitEditData);
#else
            SAFE_FREE(pstEntryExitEditData);
#endif // _DEBUG

            // メモリ領域解放のメッセージをログに出力
            LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 入退室情報データ編集用構造体メモリ領域解放終了 *****");
            sprintf_s(
                g_clsMainLogFile.m_cLogData, 
                sizeof(g_clsMainLogFile.m_cLogData), 
                "%s%s", 
                g_clsMainLogFile.m_stLogData.pcActionName, 
                MEMORYRELEASESUCCESS);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

            // エラーコードを設定して後処理へ移動
            LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** ファイル状態チェック処理異常終了 *****");
            iRtn = iStatResult;
            g_clsMainLogFile.m_maErrorActionName.insert(make_pair(iRtn, g_clsMainLogFile.m_stLogData.pcActionName));
            goto END;
        }
        else
        {
            // ファイルの状態取得成功
            // ファイル名をログに出力
            sprintf_s(
                g_clsMainLogFile.m_cLogData, 
                sizeof(g_clsMainLogFile.m_cLogData), 
                "ファイル名は：%s", 
                g_clsMainLogFile.m_stLogData.pcActionName);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

            // ファイルの状態取得成功のメッセージ及び入退室書き込みデータファイルサイズをログに出力
            sprintf_s(
                g_clsMainLogFile.m_cLogData, 
                sizeof(g_clsMainLogFile.m_cLogData), 
                "***** %s状態取得正常終了 *****", 
                g_maFileName.at(tagFileType::FILETYPE2).c_str());
            LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, g_clsMainLogFile.m_cLogData);
            LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, (char*)GETSTATSUCCESS);
            sprintf_s(
                g_clsMainLogFile.m_cLogData, 
                sizeof(g_clsMainLogFile.m_cLogData), 
                "%sの現在のファイルサイズは：%d", 
                g_maFileName.at(tagFileType::FILETYPE2).c_str(), 
                g_pclsMainRecOpe->m_stStat.st_size);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
        }

        // 入退室状況印字見出し設定処理
        // PC閲覧画面及び入退室書き込みデータファイルへ見出しを出力
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 入退室状況印字見出し設定関数呼び出し *****");
        g_pclsMainRecOpe->SetEntryExitDataTitleProc(pfOutputFile);

        // 読み込みレコードがなくなるまでループ
        do
        {
            // レコード出力対象かチェック
            if (true != bSkipFlag)
            {
                // レコード出力対象
                // 入退室データ編集情報出力処理
                // 取得したレコードを帳票形式に編集して出力
                // （deqEntryExitEditData変数はC++言語コンテナdeque定義）
                LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 入退室データ編集情報出力関数呼び出し *****");
                g_pclsMainRecOpe->PutEditDataProc(
                    pfOutputFile, 
                    &g_pclsMainRecOpe->m_stEntryExitInfo.deqEntryExitEditData[siRecCnt],
                    &g_pclsMainRecOpe->m_stEntryExitInfo.stDoorLevelData);

                // ドアレベル情報構造体の更新状態を出力
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, "***** ドアレベル情報の更新状態 *****");
                sprintf_s(
                    g_clsMainLogFile.m_cLogData, 
                    sizeof(g_clsMainLogFile.m_cLogData), 
                    "通過ドアレベル：%d", 
                    g_pclsMainRecOpe->m_stEntryExitInfo.stDoorLevelData.iDoorLevel);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, g_clsMainLogFile.m_cLogData);
                sprintf_s(
                    g_clsMainLogFile.m_cLogData, 
                    sizeof(g_clsMainLogFile.m_cLogData), 
                    "通過前ドアレベル：%d", 
                    g_pclsMainRecOpe->m_stEntryExitInfo.stDoorLevelData.iBeforeLevel);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, g_clsMainLogFile.m_cLogData);
                sprintf_s(
                    g_clsMainLogFile.m_cLogData, 
                    sizeof(g_clsMainLogFile.m_cLogData), 
                    "通過後ドアレベル：%d", 
                    g_pclsMainRecOpe->m_stEntryExitInfo.stDoorLevelData.iAfterLevel);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, g_clsMainLogFile.m_cLogData);
                sprintf_s(
                    g_clsMainLogFile.m_cLogData, 
                    sizeof(g_clsMainLogFile.m_cLogData), 
                    "現在の通過ドアレベル：%d", 
                    g_pclsMainRecOpe->m_stEntryExitInfo.stDoorLevelData.iLevel);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, g_clsMainLogFile.m_cLogData);

                // 入退室情報データ編集用構造体メモリ領域を解放
                LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 入退室情報データ編集用構造体メモリ領域解放 *****");
#ifdef _DEBUG
                SAFE_FREE_DEBUG(pstEntryExitEditData);
#else
                SAFE_FREE(pstEntryExitEditData);
#endif // _DEBUG

                // メモリ領域解放のメッセージをログに出力
                LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 入退室情報データ編集用構造体メモリ領域解放終了 *****");
                sprintf_s(
                    g_clsMainLogFile.m_cLogData,
                    sizeof(g_clsMainLogFile.m_cLogData),
                    "%s%s",
                    g_clsMainLogFile.m_stLogData.pcActionName,
                    MEMORYRELEASESUCCESS);
                LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
            }

            // 入退室データ編集情報取得処理
            // 2件目以降のレコードを取得
            LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 入退室データ編集情報取得関数呼び出し *****");
            pstEntryExitEditData = g_pclsMainRecOpe->GetEditDataProc(
                pfEntryExitFile,
                pfOutputFile,
                &bSkipFlag,
                &iFileEndFlag,
                &iGetDataResult, 
                &lRecCnt);
            sprintf_s(
                g_clsMainLogFile.m_cLogData, 
                sizeof(g_clsMainLogFile.m_cLogData), 
                "ファイル終了フラグ：%d 入退室レコード取得結果：%d", 
                iFileEndFlag, iGetDataResult);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
            sprintf_s(
                g_clsMainLogFile.m_cLogData, 
                sizeof(g_clsMainLogFile.m_cLogData), 
                "スキップ制御フラグ：%s", 
                bSkipFlag ? "true" : "false");
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
            g_clsMainLogFile.m_stLogData = g_pclsMainSubOpe->CreateEventMessageProc(ACTIONSET, "入退室データ編集情報の");

            // レコード出力対象かチェック
            if (true != bSkipFlag)
            {
                // レコード出力対象
                // 取得した1レコード分の入退室データ編集情報を管理用の入退室情報データ編集用構造体へ格納
                // （deqEntryExitEditData変数はC++言語コンテナdeque定義）
                g_pclsMainRecOpe->m_stEntryExitInfo.deqEntryExitEditData.push_back(*pstEntryExitEditData);

                // 情報カウンターを取得
                // （deqEntryExitEditData変数の数-1）
                siRecCnt = g_pclsMainRecOpe->m_stEntryExitInfo.deqEntryExitEditData.size() - 1;

                // 制限エリアレベルクリア処理
                // 入退室制限エリアレベルをクリア
                // （deqEntryExitEditData変数はC++言語コンテナdeque定義）
                LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 制限エリアレベルクリア関数呼び出し *****");
                g_pclsMainRecOpe->ClearLevelProc(
                    pfOutputFile, 
                    g_pclsMainRecOpe->m_stEntryExitInfo.deqEntryExitEditData[siRecCnt],
                    iFileEndFlag, 
                    &g_pclsMainRecOpe->m_stEntryExitInfo.stDoorLevelData);

                // クリア後のドアレベル情報構造体の更新状態を出力
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, "***** ドアレベル情報のクリア状態 *****");
                sprintf_s(
                    g_clsMainLogFile.m_cLogData, 
                    sizeof(g_clsMainLogFile.m_cLogData), 
                    "通過ドアレベル：%d", 
                    g_pclsMainRecOpe->m_stEntryExitInfo.stDoorLevelData.iDoorLevel);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, g_clsMainLogFile.m_cLogData);
                sprintf_s(
                    g_clsMainLogFile.m_cLogData, 
                    sizeof(g_clsMainLogFile.m_cLogData), 
                    "通過前ドアレベル：%d", 
                    g_pclsMainRecOpe->m_stEntryExitInfo.stDoorLevelData.iBeforeLevel);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, g_clsMainLogFile.m_cLogData);
                sprintf_s(
                    g_clsMainLogFile.m_cLogData, 
                    sizeof(g_clsMainLogFile.m_cLogData), 
                    "通過後ドアレベル：%d", 
                    g_pclsMainRecOpe->m_stEntryExitInfo.stDoorLevelData.iAfterLevel);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, g_clsMainLogFile.m_cLogData);
                sprintf_s(
                    g_clsMainLogFile.m_cLogData, 
                    sizeof(g_clsMainLogFile.m_cLogData), 
                    "現在の通過ドアレベル：%d", 
                    g_pclsMainRecOpe->m_stEntryExitInfo.stDoorLevelData.iLevel);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, g_clsMainLogFile.m_cLogData);
            }

            // ファイル終了フラグ状態をログに出力
            sprintf_s(g_clsMainLogFile.m_cLogData, sizeof(g_clsMainLogFile.m_cLogData), "ファイル終了フラグ状態は：%d", iFileEndFlag);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
        }
        // ファイル終了フラグONであれば終了
        while (FILEEND_OFF == iFileEndFlag);

        // 入退室情報データ編集用構造体メモリ領域を解放
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 入退室情報データ編集用構造体メモリ領域解放 *****");
#ifdef _DEBUG
        SAFE_FREE_DEBUG(pstEntryExitEditData);
#else
        SAFE_FREE(pstEntryExitEditData);
#endif // _DEBUG

        // メモリ領域解放のメッセージをログに出力
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 入退室情報データ編集用構造体メモリ領域解放終了 *****");
        sprintf_s(
            g_clsMainLogFile.m_cLogData,
            sizeof(g_clsMainLogFile.m_cLogData),
            "%s%s",
            g_clsMainLogFile.m_stLogData.pcActionName,
            MEMORYRELEASESUCCESS);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

        // 最終入退室データ出力件数出力
        // （distanceはC++言語定義）
        siDataPrintCnt = distance(
            g_pclsMainRecOpe->m_stEntryExitInfo.deqEntryExitEditData.begin(),
            g_pclsMainRecOpe->m_stEntryExitInfo.deqEntryExitEditData.end());
#ifdef SCREEN
        printf("\n入退室レコード件数は：%ld 件\n", lRecCnt);
        printf("入退室データ印字件数は：%zu 件\n", siDataPrintCnt);
#endif // SCREEN
        LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, ALLREADEND);
        sprintf_s(g_clsMainLogFile.m_cLogData, sizeof(g_clsMainLogFile.m_cLogData), "入退室レコード件数：%ld", lRecCnt);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
        sprintf_s(
            g_clsMainLogFile.m_cLogData, 
            sizeof(g_clsMainLogFile.m_cLogData), 
            "入退室データ印字件数：%zu", 
            siDataPrintCnt);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

        // 件数整合チェック
        if (lRecCnt == siDataPrintCnt)
        {
            // 入退室レコード件数と入退室データ印字件数が一致
            // 一致のメッセージを画面及びログに出力
#ifdef SCREEN
            printf("***** 正常終了！！（OK） *****\n");
#endif // SCREEN
            sprintf_s(
                g_clsMainLogFile.m_cLogData,
                sizeof(g_clsMainLogFile.m_cLogData),
                "***** 正常終了！！（OK） *****");
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

        }
        else
        {
            // 入退室レコード件数と入退室データ印字件数が相違
            // 不一致のメッセージを画面及びログに出力
#ifdef SCREEN
            printf("***** 件数不一致！！（NG） *****\n");
            printf("不正データ件数は：%zu 件\n", lRecCnt - siDataPrintCnt);
#endif // SCREEN
            sprintf_s(
                g_clsMainLogFile.m_cLogData,
                sizeof(g_clsMainLogFile.m_cLogData),
                "***** 件数不一致！！（NG） *****");
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
            sprintf_s(
                g_clsMainLogFile.m_cLogData,
                sizeof(g_clsMainLogFile.m_cLogData),
                "不正データ件数は：%zu 件",
                lRecCnt - siDataPrintCnt);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
        }
    }

    // 例外処理
    catch (const ERRORCODETABLE eRecDataErr)
    {
        // エラーコードを設定
        g_clsMainLogFile.m_eErrorResultCode = eRecDataErr;
        g_clsMainLogFile.m_maErrorActionName.insert(
            make_pair(
                (int)eRecDataErr, 
                g_clsMainLogFile.m_stLogData.pcActionName));
        iRtn = (int)eRecDataErr;

        // PC閲覧画面及びログにエラーメッセージを表示
        g_clsMainLogFile.m_stLogData = g_pclsMainSubOpe->CreateEventMessageProc(
            ERRORSET,
            g_clsMainLogFile.m_maErrorActionName.at((int)eRecDataErr).c_str(),
            g_clsMainLogFile.m_eErrorResultCode);
        LOGOUTPUTERROR(
            TRACEOUTPUTLOG,
            ERRORINFORMATION1,
            g_clsMainLogFile.m_stLogData.pcMessage,
            g_clsMainLogFile.m_stLogData.eErrorCode);
        sprintf_s(g_clsMainLogFile.m_cLogData, sizeof(g_clsMainLogFile.m_cLogData), "戻り値は：%d", eRecDataErr);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 入退室データ編集情報取得関数呼び出し異常終了 *****");
    }

END:
    // 後処理
    // 入退室状況印字制御終了処理
    // ファイルのクローズ等を実行
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 入退室状況印字制御終了処理関数呼び出し *****");
    iEndResult = DataControlEndProc(&pfEntryExitFile, &pfOutputFile, vecArgc, iFileCopyResult);
    sprintf_s(g_clsMainLogFile.m_cLogData, sizeof(g_clsMainLogFile.m_cLogData), "戻り値は：%d", iEndResult);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

    // 入退室状況印字制御終了処理の実行結果をチェック
    if (EXIT_SUCCESS != iEndResult)
    {
        // 入退室状況印字制御終了処理失敗
        // 呼び出しに対する異常終了のメッセージをログに出力
        g_maStartEndErrorCode.insert(make_pair(iEndResult, g_clsMainLogFile.m_eErrorResultCode));
        LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** 入退室状況印字制御終了処理異常終了 *****");
        LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, ENDFAILED);
    }
    else
    {
        // 入退室状況印字制御終了処理成功
        // 呼び出しに対する正常終了のメッセージをログに出力
        LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, "***** 入退室状況印字制御終了処理正常終了 *****");
        LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, ENDSUCCESS);
    }

    // 入退室状況印字制御メイン処理での戻り値をチェック
    if (EXIT_SUCCESS != iRtn)
    {
        // 異常値
        // 入退室状況印字制御メイン処理終了日時出力
        LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** 入退室状況印字制御メイン処理異常終了 *****");
        g_clsMainLogFile.m_stTimeData = g_clsMainLogFile.GetDateTimeProc();
        sprintf_s(
            g_clsMainLogFile.m_cLogData, 
            sizeof(g_clsMainLogFile.m_cLogData), 
            "入退室状況印字制御メイン処理終了日時：%s %s", 
            (char *)g_clsMainLogFile.m_stTimeData.strMachDatePlus.c_str(), 
            (char *)g_clsMainLogFile.m_stTimeData.strMachTimePlus.c_str());
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

        // PC閲覧画面及びログにエラーメッセージを表示
        g_clsMainLogFile.m_stLogData = g_pclsMainSubOpe->CreateEventMessageProc(
            ERRORSET, 
            g_clsMainLogFile.m_maErrorActionName.at(iRtn).c_str(),
            g_clsMainLogFile.m_eErrorResultCode);
        LOGOUTPUTERROR(
            TRACEOUTPUTLOG, 
            ERRORINFORMATION1, 
            g_clsMainLogFile.m_stLogData.pcMessage, 
            g_clsMainLogFile.m_stLogData.eErrorCode);
        sprintf_s(g_clsMainLogFile.m_cLogData, sizeof(g_clsMainLogFile.m_cLogData), "戻り値は：%d", iRtn);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
#ifdef SCREEN
        printf_s("%s\n", g_clsMainLogFile.m_stLogData.pcMessage);
        printf_s("エラーコードは：%d\n", g_clsMainLogFile.m_stLogData.eErrorCode);
        printf_s("戻り値は：%d\n", iRtn);
        printf_s("***** 入退室状況印字処理異常終了 *****\n");
#endif // SCREEN
        LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** 入退室状況印字処理異常終了 *****");
    }
    else
    {
        // 正常値
        // 入退室状況印字制御開始処理／入退室状況印字制御終了処理の結果をチェック
        if (EXIT_SUCCESS == iStartResult && EXIT_SUCCESS == iEndResult)
        {
            // 入退室状況印字制御開始処理及び入退室状況印字制御終了処理共に成功
            // 入退室状況印字制御メイン処理終了日時出力
            LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, "***** 入退室状況印字制御メイン処理正常終了 *****");
            g_clsMainLogFile.m_stTimeData = g_clsMainLogFile.GetDateTimeProc();
            sprintf_s(
                g_clsMainLogFile.m_cLogData, 
                sizeof(g_clsMainLogFile.m_cLogData), 
                "入退室状況印字制御メイン処理終了日時：%s %s", 
                (char *)g_clsMainLogFile.m_stTimeData.strMachDatePlus.c_str(), 
                (char *)g_clsMainLogFile.m_stTimeData.strMachTimePlus.c_str());
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

            // 正常結果を出力
#ifdef SCREEN
            printf_s("入退室状況印字処理実行結果は：%s\n\n", g_stFileNameInfo.strWriteName.c_str());
            printf_s("***** 入退室状況印字処理正常終了 *****\n");
#endif // SCREEN
            sprintf_s(
                g_clsMainLogFile.m_cLogData,
                sizeof(g_clsMainLogFile.m_cLogData),
                "戻り値は：%d",
                iRtn);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
            sprintf_s(
                g_clsMainLogFile.m_cLogData,
                sizeof(g_clsMainLogFile.m_cLogData),
                "入退室状況印字処理実行結果は：%s",
                g_stFileNameInfo.strWriteName.c_str());
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
            LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, "***** 入退室状況印字処理正常終了 *****");
        }
        else
        {
            // 入退室状況印字開始処理もしくは入退室状況印字終了処理どちらか失敗
            // 入退室状況印字開始処理及び入退室状況印字制御終了処理それぞれの戻り値でエラーメッセージを出力する
            // 入退室状況印字開始処理戻り値をチェック
            if (EXIT_SUCCESS != iStartResult)
            {
                // 入退室状況印字制御開始処理で異常終了
                // 戻り値に入退室状況印字制御開始処理の結果コードを設定
                iRtn = iStartResult;

                // エラーメッセージをPC閲覧画面及びログに出力
                LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** エラーメッセージ生成関数呼び出し *****");
                eInitResultCode = tagErrorCode::FAILERROR;
                g_clsMainLogFile.m_stLogData = g_pclsMainSubOpe->CreateEventMessageProc(
                    ACTIONSET, 
                    "入退室状況印字制御開始処理の");
                g_clsMainLogFile.m_stLogData = g_pclsMainSubOpe->CreateEventMessageProc(
                    ERRORSET, 
                    g_clsMainLogFile.m_stLogData.pcActionName, 
                    eInitResultCode);
#ifdef SCREEN
                printf_s("***** %s *****\n", g_clsMainLogFile.m_stLogData.pcMessage);
#endif // SCREEN
                LOGOUTPUTERROR(
                    TRACEOUTPUTLOG, 
                    ERRORINFORMATION1, 
                    g_clsMainLogFile.m_stLogData.pcMessage, 
                    g_clsMainLogFile.m_stLogData.eErrorCode);

                // 入退室状況印字制御開始処理で発生した異常結果をPC等閲覧画面及びログ表示
                g_clsMainLogFile.m_stLogData = g_pclsMainSubOpe->CreateEventMessageProc(
                    ERRORSET, 
                    g_clsMainLogFile.m_maErrorActionName.at(iStartResult).c_str(), 
                    g_maStartEndErrorCode.at(iStartResult));
#ifdef SCREEN
                printf_s("%s\n", g_clsMainLogFile.m_stLogData.pcMessage);
                printf_s("エラーコードは：%d\n", g_clsMainLogFile.m_stLogData.eErrorCode);
                printf_s("戻り値は：%d\n", iRtn);
#endif // SCREEN
                LOGOUTPUTERROR(
                    TRACEOUTPUTLOG, 
                    ERRORINFORMATION1, 
                    g_clsMainLogFile.m_stLogData.pcMessage, 
                    g_clsMainLogFile.m_stLogData.eErrorCode);
                sprintf_s(
                    g_clsMainLogFile.m_cLogData, 
                    sizeof(g_clsMainLogFile.m_cLogData), 
                    "エラーコードは：%d", 
                    g_clsMainLogFile.m_stLogData.eErrorCode);
                LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
                sprintf_s(g_clsMainLogFile.m_cLogData, sizeof(g_clsMainLogFile.m_cLogData), "戻り値は：%d", iRtn);
                LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
            }

            // 入退室状況印字終了処理戻り値をチェック
            if (EXIT_SUCCESS != iEndResult)
            {
                // 入退室状況印字制御終了処理で異常終了
                // 戻り値に入退室状況印字制御終了処理の結果コードを設定
                iRtn = iEndResult;

                // エラーメッセージをPC閲覧画面及びログに出力
                LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** エラーメッセージ生成関数呼び出し *****");
                eEndResultCode = tagErrorCode::FAILERROR;
                g_clsMainLogFile.m_stLogData = g_pclsMainSubOpe->CreateEventMessageProc(
                    ACTIONSET, 
                    "入退室状況印字制御終了処理の");
                g_clsMainLogFile.m_stLogData = g_pclsMainSubOpe->CreateEventMessageProc(
                    ERRORSET, 
                    g_clsMainLogFile.m_stLogData.pcActionName, 
                    eEndResultCode);
#ifdef SCREEN
                printf_s("***** %s *****\n", g_clsMainLogFile.m_stLogData.pcMessage);
#endif // SCREEN
                LOGOUTPUTERROR(
                    TRACEOUTPUTLOG, 
                    ERRORINFORMATION1, 
                    g_clsMainLogFile.m_stLogData.pcMessage, 
                    g_clsMainLogFile.m_stLogData.eErrorCode);

                // 入退室状況印字制御終了処理で発生した異常結果をPC等閲覧画面及びログ表示
                g_clsMainLogFile.m_stLogData = g_pclsMainSubOpe->CreateEventMessageProc(
                    ERRORSET, 
                    g_clsMainLogFile.m_maErrorActionName.at(iEndResult).c_str(), 
                    g_maStartEndErrorCode.at(iEndResult));
#ifdef SCREEN
                printf_s("%s\n", g_clsMainLogFile.m_stLogData.pcMessage);
                printf_s("エラーコードは：%d\n", g_clsMainLogFile.m_stLogData.eErrorCode);
                printf_s("戻り値は：%d\n", iRtn);
#endif // SCREEN
                LOGOUTPUTERROR(
                    TRACEOUTPUTLOG, 
                    ERRORINFORMATION1, 
                    g_clsMainLogFile.m_stLogData.pcMessage, 
                    g_clsMainLogFile.m_stLogData.eErrorCode);
                sprintf_s(
                    g_clsMainLogFile.m_cLogData, 
                    sizeof(g_clsMainLogFile.m_cLogData), 
                    "エラーコードは：%d", 
                    g_clsMainLogFile.m_stLogData.eErrorCode);
                LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
                sprintf_s(g_clsMainLogFile.m_cLogData, sizeof(g_clsMainLogFile.m_cLogData), "戻り値は：%d", iRtn);
                LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
            }

            // 入退室状況印字制御メイン処理終了日時出力
            LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** 入退室状況印字制御メイン処理異常終了 *****");
            g_clsMainLogFile.m_stTimeData = g_clsMainLogFile.GetDateTimeProc();
            sprintf_s(
                g_clsMainLogFile.m_cLogData, 
                sizeof(g_clsMainLogFile.m_cLogData), 
                "入退室状況印字制御メイン処理終了日時：%s %s", 
                (char *)g_clsMainLogFile.m_stTimeData.strMachDatePlus.c_str(), 
                (char *)g_clsMainLogFile.m_stTimeData.strMachTimePlus.c_str());
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
            LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** 入退室状況印字処理異常終了 *****");
#ifdef SCREEN
            printf_s("***** 入退室状況印字処理異常終了 *****\n");
#endif // SCREEN
        }
    }

    // 最終形態各ファイル名出力
    // 絶対パス変換ファイル名取得処理で取得したファイル名（最終形態）をログ出力
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, "***** 絶対パスを付与した各ファイル名（最終形態） *****");
    sprintf_s(
        g_clsMainLogFile.m_cLogData, 
        sizeof(g_clsMainLogFile.m_cLogData), 
        "%s名：%s", 
        g_maFileName.at(tagFileType::FILETYPE1).c_str(), 
        g_stFileNameInfo.strReadName.c_str());
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, g_clsMainLogFile.m_cLogData);
    sprintf_s(
        g_clsMainLogFile.m_cLogData, 
        sizeof(g_clsMainLogFile.m_cLogData), 
        "%s名：%s", 
        g_maFileName.at(tagFileType::FILETYPE2).c_str(), 
        g_stFileNameInfo.strWriteName.c_str());
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, g_clsMainLogFile.m_cLogData);
    sprintf_s(
        g_clsMainLogFile.m_cLogData, 
        sizeof(g_clsMainLogFile.m_cLogData), 
        "%s名：%s", 
        g_maFileName.at(tagFileType::FILETYPE3).c_str(), 
        g_stFileNameInfo.strTraceLogName.c_str());
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, g_clsMainLogFile.m_cLogData);
    sprintf_s(
        g_clsMainLogFile.m_cLogData, 
        sizeof(g_clsMainLogFile.m_cLogData), 
        "%s名：%s", 
        g_maFileName.at(tagFileType::FILETYPE4).c_str(), 
        g_stFileNameInfo.strDataLogName.c_str());
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, g_clsMainLogFile.m_cLogData);
    sprintf_s(
        g_clsMainLogFile.m_cLogData, 
        sizeof(g_clsMainLogFile.m_cLogData), 
        "%s名：%s", 
        g_maFileName.at(tagFileType::FILETYPE5).c_str(), 
        g_stFileNameInfo.strBackupName.c_str());
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, g_clsMainLogFile.m_cLogData);
    sprintf_s(
        g_clsMainLogFile.m_cLogData, 
        sizeof(g_clsMainLogFile.m_cLogData), 
        "%s名：%s", 
        g_maFileName.at(tagFileType::FILETYPE6).c_str(), 
        g_stFileNameInfo.strChangeName.c_str());
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, g_clsMainLogFile.m_cLogData);
    sprintf_s(
        g_clsMainLogFile.m_cLogData, 
        sizeof(g_clsMainLogFile.m_cLogData), 
        "%s名：%s", 
        g_maFileName.at(tagFileType::FILETYPE7).c_str(), 
        g_stFileNameInfo.strDeleteName.c_str());
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, g_clsMainLogFile.m_cLogData);
    sprintf_s(
        g_clsMainLogFile.m_cLogData, 
        sizeof(g_clsMainLogFile.m_cLogData), 
        "%s名：%s", 
        g_maFileName.at(tagFileType::FILETYPE8).c_str(), 
        g_stFileNameInfo.strModuleName.c_str());
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, g_clsMainLogFile.m_cLogData);
    sprintf_s(
        g_clsMainLogFile.m_cLogData, 
        sizeof(g_clsMainLogFile.m_cLogData), 
        "%s名：%s", 
        g_maFileName.at(tagFileType::FILETYPE9).c_str(),
        g_stFileNameInfo.strFraudDataName.c_str());
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, g_clsMainLogFile.m_cLogData);

    // 日時情報取得処理
    // 入退室状況印字処理終了日時表示
    g_clsMainLogFile.m_stTimeData = g_clsMainLogFile.GetDateTimeProc();
    sprintf_s(
        g_clsMainLogFile.m_cLogData, 
        sizeof(g_clsMainLogFile.m_cLogData), 
        "入退室状況印字処理終了日時：%s %s", 
        (char *)g_clsMainLogFile.m_stTimeData.strMachDatePlus.c_str(), 
        (char *)g_clsMainLogFile.m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, g_clsMainLogFile.m_cLogData);
#ifdef SCREEN
    printf_s("%s\n", g_clsMainLogFile.m_cLogData);
#endif // SCREEN

    // 実行結果を返却
    return iRtn;
}

//********************************************
// 関数名：入退室状況印字制御開始処理
// Function Name：DataControlStartProc
// 
// 処理概要：
// 入退室状況を印字するための準備を行う
// 
// 処理手順：
//   １．初期値設定
//   ２．入退室読み込みデータファイル名／入退室書き込みデータファイル名環境変数取得
//   ３．パラメータチェック（環境変数の設定がない場合のみ）
//   ４．入退室読み込みデータファイル絶対パス付き変換
//   ５．バックアップファイル絶対パス付き変換
//   ６．ファイルコピー処理によるバックアップファイル生成
//   ７．ファイル名変換処理によるバックアップファイル名変換
//   ８．入退室読み込みデータファイルオープン
//   ９．入退室書き込みデータファイル絶対パス付き変換
//   １０．入退室書き込みデータファイルオープン
//   １１．エラーメッセージログ出力（エラーが発生した場合のみ）
// 
// 引数：    コマンド数
//           コマンド情報
//           入退室読み込みデータファイルポインタ
//           入退室書き込みデータファイルポインタ
//           ファイルコピー呼び出し実行結果
// 戻り値：  実行結果（正常終了はstdlib.hで定義している内容を利用）
//           正常終了：EXIT_SUCCESS（0）
//           異常終了：環境変数取得エラー：CM_ENVERROR（101）
//                     パラメータ指定なし：CM_PARAMERROR（102）
//                     パラメータ不正：CM_PARAMERROR（103）
//                     ファイルオープンエラー：CM_FILEOPENERROR（104）
//                     ファイルコピーエラー：CM_FILECOPYERROR（110）
//                     ファイル名変換エラー：CM_RENAMEERROR（111）
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
int DataControlStartProc(
    int iCmdCnt,
    vector<string> vecCmdData,
    FILE **pfReadFile,
    FILE **pfWriteFile,
    int* piFileCopyResult)
{
    // 変数宣言
    FILETYPETABLE eFileType = tagFileType::NORMAL;
    char cReadLocalName[FILENAMESIZE + 1];
    char cWriteLocalName[FILENAMESIZE + 1];
    errno_t eReadErrNo = 0;
    errno_t eWriteErrNo = 0;
    int iStartResult = EXIT_SUCCESS;
    int iEnvResult = EXIT_SUCCESS;
    int iRenameResult = EXIT_SUCCESS;
    int iFileStatResult = EXIT_FAILURE;
    int iErrFlag = ERR_OFF;
    string strBackUpLocalName;
    deque<string> deqBackUpFileName;
    string strBackUpFileName;
    char cActinData[DATASIZE];
    string strCheckStatFileName;

    // 初期化
    memset(cReadLocalName, 0x00, sizeof(cReadLocalName));
    memset(cWriteLocalName, 0x00, sizeof(cWriteLocalName));
    memset(cActinData, 0x00, sizeof(cActinData));

    // 前処理
    // 日時情報取得処理
    // 入退室状況印字制御開始処理開始日時取得
    g_clsMainLogFile.m_stTimeData = g_clsMainLogFile.GetDateTimeProc();
    sprintf_s(
        g_clsMainLogFile.m_cLogData, 
        sizeof(g_clsMainLogFile.m_cLogData), 
        "入退室状況印字制御開始処理開始日時：%s %s", 
        (char *)g_clsMainLogFile.m_stTimeData.strMachDatePlus.c_str(), 
        (char *)g_clsMainLogFile.m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, g_clsMainLogFile.m_cLogData);
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 入退室状況印字制御開始処理開始 *****");

    // 初期値を設定
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, "***** 初期値 *****");
    g_clsMainLogFile.m_stLogData.eErrorCode = tagErrorCode::SUCCESS;
    sprintf_s(
        g_clsMainLogFile.m_cLogData, 
        sizeof(g_clsMainLogFile.m_cLogData), 
        "エラーコード：%d", 
        g_clsMainLogFile.m_stLogData.eErrorCode);
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, g_clsMainLogFile.m_cLogData);
    strncpy_s(
        g_pclsMainRecOpe->m_stEntryExitInfo.cLastID, 
        sizeof(g_pclsMainRecOpe->m_stEntryExitInfo.cLastID), 
        "----", 
        sizeof(g_pclsMainRecOpe->m_stEntryExitInfo.cLastID) - 1);
    sprintf_s(
        g_clsMainLogFile.m_cLogData, 
        sizeof(g_clsMainLogFile.m_cLogData), 
        "退避カードID：%s", g_pclsMainRecOpe->m_stEntryExitInfo.cLastID);
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, g_clsMainLogFile.m_cLogData);

    try
    {
        // 環境変数取得処理
        // システムの環境変数から入退室書き込みデータファイルの情報を取得
        sprintf_s(
            g_clsMainLogFile.m_cLogData, 
            sizeof(g_clsMainLogFile.m_cLogData), 
            "***** （%s）環境変数取得呼び出し *****", 
            g_maFileName.at(tagFileType::FILETYPE2).c_str());
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, g_clsMainLogFile.m_cLogData);
        iEnvResult = g_pclsMainSubOpe->GetEnvironmentDataProc(WRITE_FILE, cWriteLocalName);
        sprintf_s(g_clsMainLogFile.m_cLogData, sizeof(g_clsMainLogFile.m_cLogData), "環境変数取得結果：%d", iEnvResult);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
        g_clsMainLogFile.m_stLogData = g_pclsMainSubOpe->CreateEventMessageProc(
            ACTIONSET, 
            g_maFileName.at(tagFileType::FILETYPE2).c_str());

        // 環境変数を取得出来たかチェック
        if (CM_ENVERROR == iEnvResult)
        {
            // 入退室書き込みデータファイルの環境変数が設定されていない
            // 入退室書き込みデータファイル名にデフォルトの入退室書き込みデータファイル名を設定
            strncpy_s(cWriteLocalName, sizeof(cWriteLocalName), DEFAULTFILENAME, sizeof(cWriteLocalName) - 1);
            sprintf_s(
                g_clsMainLogFile.m_cLogData,
                sizeof(g_clsMainLogFile.m_cLogData),
                "%s名：%s",
                g_maFileName.at(tagFileType::FILETYPE2).c_str(),
                cWriteLocalName);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
            sprintf_s(
                g_clsMainLogFile.m_cLogData,
                sizeof(g_clsMainLogFile.m_cLogData),
                "%sの%s",
                g_clsMainLogFile.m_stLogData.pcActionName,
                GETNOTENVCONTINUED);
            LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, g_clsMainLogFile.m_cLogData);
        }
        else
        {
            // 入退室書き込みデータファイルの環境変数の設定あり
            // 環境変数取得成功メッセージを出力
            sprintf_s(
                g_clsMainLogFile.m_cLogData,
                sizeof(g_clsMainLogFile.m_cLogData),
                "%s名：%s",
                g_maFileName.at(tagFileType::FILETYPE2).c_str(),
                cWriteLocalName);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
            sprintf_s(
                g_clsMainLogFile.m_cLogData,
                sizeof(g_clsMainLogFile.m_cLogData),
                "%s%s",
                g_clsMainLogFile.m_stLogData.pcActionName,
                GETENVSUCCESS);
            LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, g_clsMainLogFile.m_cLogData);
        }

        // システムの環境変数から入退室読み込みデータファイルの情報を取得
        sprintf_s(
            g_clsMainLogFile.m_cLogData,
            sizeof(g_clsMainLogFile.m_cLogData),
            "***** （%s）環境変数取得呼び出し *****",
            g_maFileName.at(tagFileType::FILETYPE1).c_str());
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, g_clsMainLogFile.m_cLogData);
        iEnvResult = g_pclsMainSubOpe->GetEnvironmentDataProc(READ_FILE, cReadLocalName);
        sprintf_s(g_clsMainLogFile.m_cLogData, sizeof(g_clsMainLogFile.m_cLogData), "環境変数取得結果：%d", iEnvResult);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
        g_clsMainLogFile.m_stLogData = g_pclsMainSubOpe->CreateEventMessageProc(
            ACTIONSET,
            g_maFileName.at(tagFileType::FILETYPE1).c_str());

        // 環境変数を取得出来たかチェック
        if (CM_ENVERROR == iEnvResult)
        {
            // 入退室読み込みデータファイルが設定されていない
            // エラーフラグをON
            iErrFlag = ERR_ON;

            // エラーコードを設定
            g_clsMainLogFile.m_eErrorResultCode = tagErrorCode::ENVERROR;
            g_clsMainLogFile.m_maErrorActionName.insert(make_pair(iEnvResult, g_clsMainLogFile.m_stLogData.pcActionName));

            // エラーを設定するがパラメータとして設定している可能性があるため処理を継続する
            LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** 環境変数取得呼び出し異常終了 *****");
            sprintf_s(
                g_clsMainLogFile.m_cLogData,
                sizeof(g_clsMainLogFile.m_cLogData),
                "%s（後続処理：パラメータチェック）",
                ERRORPROCCONTINUED);
            LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, g_clsMainLogFile.m_cLogData);
        }
        else
        {
            // 入退室読み込みデータファイルの環境変数の設定あり
            // 環境変数取得成功メッセージを出力
            sprintf_s(
                g_clsMainLogFile.m_cLogData,
                sizeof(g_clsMainLogFile.m_cLogData),
                "%s名：%s",
                g_maFileName.at(tagFileType::FILETYPE1).c_str(),
                cReadLocalName);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
            LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, "***** 環境変数取得呼び出し正常終了 *****");
            sprintf_s(
                g_clsMainLogFile.m_cLogData,
                sizeof(g_clsMainLogFile.m_cLogData),
                "%sの%s",
                g_clsMainLogFile.m_stLogData.pcActionName,
                GETENVSUCCESS);
            LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, g_clsMainLogFile.m_cLogData);

            // 絶対パス変換ファイル名取得処理へ移動
            goto MAIN;
        }

        // 起動パラメータチェック
        // パラメータをチェック（先頭部分は実行ファイル名・末尾部分は指定文字列のためチェック対象外）
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** パラメータチェック開始 *****");
        sprintf_s(g_clsMainLogFile.m_cLogData, sizeof(g_clsMainLogFile.m_cLogData), "パラメータ設定数：%d", iCmdCnt);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
        sprintf_s(
            cActinData,
            sizeof(cActinData),
            "%s／%sの",
            g_maFileName.at(tagFileType::FILETYPE1).c_str(),
            g_maFileName.at(tagFileType::FILETYPE2).c_str());
        g_clsMainLogFile.m_stLogData = g_pclsMainSubOpe->CreateEventMessageProc(ACTIONSET, cActinData);
        switch (iCmdCnt)
        {
            case 2:
            {
                // コマンド数2個
                // 入退室読み込みデータファイル名及び入退室書き込みデータファイルの指定がある場合
                // 入退室読み込みデータファイル名及び入退室書き込みデータファイル名を設定
                memcpy_s(
                    cReadLocalName,
                    sizeof(cReadLocalName),
                    vecCmdData.at(static_cast<size_t>(iCmdCnt) - 1).c_str(),
                    sizeof(cReadLocalName) - 1);
                memcpy_s(
                    cWriteLocalName,
                    sizeof(cWriteLocalName),
                    vecCmdData.at(static_cast<size_t>(iCmdCnt)).c_str(),
                    sizeof(cWriteLocalName) - 1);
                sprintf_s(
                    g_clsMainLogFile.m_cLogData,
                    sizeof(g_clsMainLogFile.m_cLogData),
                    "%s名：%s",
                    g_maFileName.at(tagFileType::FILETYPE1).c_str(),
                    cReadLocalName);
                LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
                sprintf_s(
                    g_clsMainLogFile.m_cLogData,
                    sizeof(g_clsMainLogFile.m_cLogData),
                    "%s名：%s",
                    g_maFileName.at(tagFileType::FILETYPE2).c_str(),
                    cWriteLocalName);
                LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
                LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, "***** パラメータチェック正常終了 *****");
                sprintf_s(
                    g_clsMainLogFile.m_cLogData,
                    sizeof(g_clsMainLogFile.m_cLogData),
                    "%s%s", g_clsMainLogFile.m_stLogData.pcActionName,
                    GETPARAMSUCCESS);
                LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, g_clsMainLogFile.m_cLogData);
                break;
            }
            case 1:
            {
                // コマンド数1個
                // 入退室読み込みデータファイル名のみの指定がある場合
                // 入退室読み込みデータファイル名を設定
                memcpy_s(cReadLocalName, sizeof(cReadLocalName), vecCmdData.at(iCmdCnt).c_str(), sizeof(cReadLocalName) - 1);

                // 環境変数での取得により入退室書き込みデータファイル名が設定されているかチェック
                if (0 == strncmp(cWriteLocalName, "", sizeof(cWriteLocalName) - 1))
                {
                    // 入退室書き込みデータファイル名が設定されていない
                    // 入退室書き込みデータファイル名にデフォルトの入退室書き込みデータファイル名を設定
                    memcpy_s(cWriteLocalName, sizeof(cWriteLocalName), DEFAULTFILENAME, sizeof(cWriteLocalName) - 1);
                    sprintf_s(
                        g_clsMainLogFile.m_cLogData,
                        sizeof(g_clsMainLogFile.m_cLogData),
                        "%s名：%s",
                        g_maFileName.at(tagFileType::FILETYPE1).c_str(),
                        cReadLocalName);
                    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
                    sprintf_s(
                        g_clsMainLogFile.m_cLogData,
                        sizeof(g_clsMainLogFile.m_cLogData),
                        "%s名：%s",
                        g_maFileName.at(tagFileType::FILETYPE2).c_str(),
                        cWriteLocalName);
                    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
                    LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, (char*)"***** パラメータチェック正常終了 *****");
                    sprintf_s(
                        g_clsMainLogFile.m_cLogData,
                        sizeof(g_clsMainLogFile.m_cLogData),
                        "%s%s",
                        g_clsMainLogFile.m_stLogData.pcActionName,
                        GETPARAMSUCCESS);
                    LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, g_clsMainLogFile.m_cLogData);
                }
                else
                {
                    // 入退室書き込みデータファイル名が設定されている
                    // 入退室読み込みデータファイル名及び入退室書き込みデータファイル名をログに出力
                    sprintf_s(
                        g_clsMainLogFile.m_cLogData,
                        sizeof(g_clsMainLogFile.m_cLogData),
                        "%s名：%s",
                        g_maFileName.at(tagFileType::FILETYPE1).c_str(),
                        cReadLocalName);
                    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
                    sprintf_s(
                        g_clsMainLogFile.m_cLogData,
                        sizeof(g_clsMainLogFile.m_cLogData),
                        "%s名：%s", g_maFileName.at(tagFileType::FILETYPE2).c_str(),
                        cWriteLocalName);
                    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
                    LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, "***** パラメータチェック正常終了 *****");
                    sprintf_s(
                        g_clsMainLogFile.m_cLogData,
                        sizeof(g_clsMainLogFile.m_cLogData),
                        "%s%s",
                        g_clsMainLogFile.m_stLogData.pcActionName,
                        GETPARAMSUCCESS);
                    LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, g_clsMainLogFile.m_cLogData);
                }

                // 分岐抜け
                break;
            }
            case 0:
            {
                // コマンドなし
                // エラーとして例外処理へ移動
                iStartResult = CM_NOPARAM;
                g_clsMainLogFile.m_eErrorResultCode = tagErrorCode::NOPARAM;
                g_clsMainLogFile.m_maErrorActionName.insert(make_pair(iStartResult, g_clsMainLogFile.m_stLogData.pcActionName));
                LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** パラメータチェック異常終了 *****");
                throw iStartResult;
            }
            default:
            {
                // 上記以外
                // エラーとして例外処理へ移動
                iStartResult = CM_PARAMERROR;
                g_clsMainLogFile.m_eErrorResultCode = tagErrorCode::FAILPARAM;
                g_clsMainLogFile.m_maErrorActionName.insert(make_pair(iStartResult, g_clsMainLogFile.m_stLogData.pcActionName));
                LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** パラメータチェック異常終了 *****");
                throw iStartResult;
            }
        }

MAIN:
        // 主処理
        // 絶対パス変換ファイル名取得処理
        // 入退室読み込みデータファイル名を絶対パス付きに変換
        eFileType = tagFileType::FILETYPE1;
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 絶対パス変換ファイル名取得関数呼び出し *****");
        g_stFileNameInfo.strReadName = g_pclsMainFileOpe->GetConvertFileNameProc(
            eFileType,
            cReadLocalName,
            vecCmdData.at(0).c_str(),
            (char *)READPATH);
        sprintf_s(
            g_clsMainLogFile.m_cLogData,
            sizeof(g_clsMainLogFile.m_cLogData),
            "絶対パス付き%s名：%s",
            g_maFileName.at(tagFileType::FILETYPE1).c_str(),
            g_stFileNameInfo.strReadName.c_str());
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

        // 入退室読み込みデータファイルの存在状態を取得
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ファイル状態チェック関数呼び出し *****");
        memset(&g_pclsMainRecOpe->m_stStat, 0x00, sizeof(g_pclsMainRecOpe->m_stStat));
        iFileStatResult = g_pclsMainFileOpe->CheckFileStatProc(
            g_stFileNameInfo.strReadName.c_str(),
            &g_pclsMainRecOpe->m_stStat);
        sprintf_s(g_clsMainLogFile.m_cLogData, sizeof(g_clsMainLogFile.m_cLogData), "ファイル存在チェック結果：%d", iFileStatResult);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
        strCheckStatFileName =
            ("" == g_stFileNameInfo.strReadName) ?
            g_maFileName.at(tagFileType::FILETYPE1) :
            g_stFileNameInfo.strReadName;
        g_clsMainLogFile.m_stLogData = g_pclsMainSubOpe->CreateEventMessageProc(ACTIONSET, strCheckStatFileName.c_str());

        // 結果をチェック
        if (EXIT_SUCCESS != iFileStatResult)
        {
            // ファイルが存在しない
            // エラーコード及び対象ファイル名を設定して例外処理へ移動
            LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** ファイル状態チェック異常終了 *****");
            iStartResult = CM_FILESTATERROR;
            g_clsMainLogFile.m_eErrorResultCode = tagErrorCode::STATERROR;
            g_clsMainLogFile.m_maErrorActionName.insert(
                make_pair(
                    iStartResult, 
                    g_clsMainLogFile.m_stLogData.pcActionName));
            throw iStartResult;
        }
        else
        {
            // ファイルが存在する
            // ファイル名をログに出力
            sprintf_s(
                g_clsMainLogFile.m_cLogData,
                sizeof(g_clsMainLogFile.m_cLogData),
                "ファイル名は：%s",
                g_stFileNameInfo.strReadName.c_str());
            LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, g_clsMainLogFile.m_cLogData);

            // ファイルの状態取得成功のメッセージ及び入退室読み込みデータファイルサイズをログに出力
            LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, "***** ファイル状態チェック正常終了 *****");
            sprintf_s(
                g_clsMainLogFile.m_cLogData,
                sizeof(g_clsMainLogFile.m_cLogData),
                "%s %s",
                g_clsMainLogFile.m_stLogData.pcActionName,
                GETSTATSUCCESS);
            LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, g_clsMainLogFile.m_cLogData);
            sprintf_s(
                g_clsMainLogFile.m_cLogData,
                sizeof(g_clsMainLogFile.m_cLogData),
                "%sの現在のファイルサイズは：%d",
                g_maFileName.at(tagFileType::FILETYPE1).c_str(),
                g_pclsMainRecOpe->m_stStat.st_size);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
        }

        if (0 == g_pclsMainRecOpe->m_stStat.st_size)
        {
            // 入退室読み込みデータファイルのサイズが0
            // ファイルサイズをログに出力
            sprintf_s(
                g_clsMainLogFile.m_cLogData,
                sizeof(g_clsMainLogFile.m_cLogData),
                "%s のファイルサイズは：%d",
                g_stFileNameInfo.strReadName.c_str(),
                g_pclsMainRecOpe->m_stStat.st_size);
            LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, g_clsMainLogFile.m_cLogData);

            // ファイルコピーを行わず入退室読み込みデータファイル読み込みへ移動
            goto READ;
        }

        // 入退室読み込みデータファイルを分割
        strBackUpLocalName = g_stFileNameInfo.strReadName;
        deqBackUpFileName = g_pclsMainFileOpe->SplitDataProc(strBackUpLocalName, '\\');
        strBackUpFileName = deqBackUpFileName.back();
        sprintf_s(
            g_clsMainLogFile.m_cLogData,
            sizeof(g_clsMainLogFile.m_cLogData),
            "%s名：%s",
            g_maFileName.at(tagFileType::FILETYPE5).c_str(),
            strBackUpFileName.c_str());
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

        // 絶対パス変換ファイル名取得処理
        // バックアップファイル名を絶対パス付きに変換
        eFileType = tagFileType::FILETYPE5;
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 絶対パス変換ファイル名取得関数呼び出し *****");
        g_stFileNameInfo.strBackupName = g_pclsMainFileOpe->GetConvertFileNameProc(
            eFileType,
            strBackUpFileName.c_str(),
            vecCmdData.at(0).c_str(),
            (char *)COPYPATH);
        sprintf_s(
            g_clsMainLogFile.m_cLogData,
            sizeof(g_clsMainLogFile.m_cLogData),
            "絶対パス付き%s名：%s",
            g_maFileName.at(tagFileType::FILETYPE5).c_str(),
            g_stFileNameInfo.strBackupName.c_str());
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

        // ファイルコピー処理
        // 入退室読み込みデータファイルをバックアップファイルへコピー
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ファイルコピー関数呼び出し *****");
        *piFileCopyResult = g_pclsMainFileOpe->CopyFileProc(
            strBackUpLocalName.c_str(),
            g_stFileNameInfo.strBackupName.c_str());
        sprintf_s(g_clsMainLogFile.m_cLogData, sizeof(g_clsMainLogFile.m_cLogData), "コピー結果：%d", *piFileCopyResult);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
        g_clsMainLogFile.m_stLogData = g_pclsMainSubOpe->CreateEventMessageProc(
            ACTIONSET,
            g_stFileNameInfo.strBackupName.c_str());

        // コピー結果をチェック
        if (EXIT_SUCCESS != *piFileCopyResult && CM_FILECOPYERROR != *piFileCopyResult)
        {
            // 異常終了（ファイルコピーエラー以外）
            // エラーコードを設定して例外処理へ移動
            iStartResult = *piFileCopyResult;
            g_clsMainLogFile.m_eErrorResultCode = (ERRORCODETABLE)iStartResult;
            g_clsMainLogFile.m_maErrorActionName.insert(
                make_pair(
                    iStartResult, 
                    g_clsMainLogFile.m_stLogData.pcActionName));
            LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** ファイルコピー関数呼び出し異常終了 *****");
            throw iStartResult;
        }
        else if (CM_FILECOPYERROR == *piFileCopyResult)
        {
            // ファイルコピーエラー発生
            // エラーフラグをON
            iErrFlag = ERR_ON;

            // エラーコードを設定
            iStartResult = CM_FILECOPYERROR;
            g_clsMainLogFile.m_eErrorResultCode = tagErrorCode::COPYERROR;
            g_clsMainLogFile.m_maErrorActionName.insert(
                make_pair(
                    iStartResult, 
                    g_clsMainLogFile.m_stLogData.pcActionName));

            // エラーを設定するが直接影響がないため処理を継続する
            LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** ファイルコピー関数呼び出し異常終了 *****");
            sprintf_s(
                g_clsMainLogFile.m_cLogData,
                sizeof(g_clsMainLogFile.m_cLogData),
                "%s（後続処理：%sのリネーム）",
                ERRORPROCCONTINUED,
                g_maFileName.at(tagFileType::FILETYPE1).c_str());
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
        }
        else
        {
            // ファイルコピー成功
            // ファイルコピー成功のメッセージをログに出力
            LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, "***** ファイルコピー関数呼び出し正常終了 *****");
            sprintf_s(
                g_clsMainLogFile.m_cLogData,
                sizeof(g_clsMainLogFile.m_cLogData),
                "%s に対する%s",
                g_clsMainLogFile.m_stLogData.pcActionName,
                FILECOPYSUCCESS);
            LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, g_clsMainLogFile.m_cLogData);
            sprintf_s(
                g_clsMainLogFile.m_cLogData, 
                sizeof(g_clsMainLogFile.m_cLogData), 
                "戻り値は：%d", 
                *piFileCopyResult);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
        }

        // 絶対パス変換ファイル名取得処理
        // バックアップファイル名に基づいて名称変更対象ファイル名を取得
        eFileType = tagFileType::FILETYPE6;
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 絶対パス変換ファイル名取得関数呼び出し *****");
        g_stFileNameInfo.strChangeName = g_pclsMainFileOpe->GetConvertFileNameProc(
            eFileType,
            g_stFileNameInfo.strBackupName.c_str(),
            vecCmdData.at(0).c_str(),
            (char *)"",
            (char *)"",
            iStartResult);
        sprintf_s(
            g_clsMainLogFile.m_cLogData,
            sizeof(g_clsMainLogFile.m_cLogData),
            "絶対パス付き%s名：%s",
            g_maFileName.at(tagFileType::FILETYPE6).c_str(),
            g_stFileNameInfo.strChangeName.c_str());
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

        // ファイル名変換処理
        // コピーしたファイル名を名称変更対象ファイル名（バックアップファイル名）にリネーム
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ファイル名変換関数呼び出し *****");
        iRenameResult = g_pclsMainFileOpe->ChangeFileNameProc(
            g_stFileNameInfo.strBackupName.c_str(),
            g_stFileNameInfo.strChangeName.c_str());
        g_clsMainLogFile.m_stLogData = g_pclsMainSubOpe->CreateEventMessageProc(
            ACTIONSET,
            g_stFileNameInfo.strBackupName.c_str());
        sprintf_s(g_clsMainLogFile.m_cLogData, sizeof(g_clsMainLogFile.m_cLogData), "実行結果：%d", iRenameResult);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

        // リネーム出来たかチェック
        if (CM_RENAMEERROR == iRenameResult)
        {
            // コピーしたファイルに対するリネーム失敗
            // エラーコードを設定して例外処理へ移動
            iStartResult = iRenameResult;
            g_clsMainLogFile.m_eErrorResultCode = tagErrorCode::RENAMEERROR;
            g_clsMainLogFile.m_maErrorActionName.insert(
                make_pair(
                    iRenameResult, 
                    g_clsMainLogFile.m_stLogData.pcActionName));
            LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** ファイル名変換関数呼び出し異常終了 *****");
            throw iStartResult;
        }
        else
        {
            // リネーム成功
            // リネーム成功のメッセージをログに出力
            LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, "***** ファイル名変換関数呼び出し正常終了 *****");
            sprintf_s(
                g_clsMainLogFile.m_cLogData,
                sizeof(g_clsMainLogFile.m_cLogData),
                "%s に対する%s", g_clsMainLogFile.m_stLogData.pcActionName,
                RENAMESUCCESS);
            LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, g_clsMainLogFile.m_cLogData);
            sprintf_s(g_clsMainLogFile.m_cLogData, sizeof(g_clsMainLogFile.m_cLogData), "戻り値は：%d", iRenameResult);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

            // リネーム対象の名称変更対象ファイル名をバックアップファイル名にコピー
            g_stFileNameInfo.strBackupName = g_stFileNameInfo.strChangeName;
            g_pclsMainFileOpe->m_strFileOpeCopyFileName = g_stFileNameInfo.strChangeName;
            g_pclsMainRecOpe->m_strFileOpeCopyFileName = g_stFileNameInfo.strChangeName;
            sprintf_s(
                g_clsMainLogFile.m_cLogData,
                sizeof(g_clsMainLogFile.m_cLogData),
                "絶対パス付き%s名：%s",
                g_maFileName.at(tagFileType::FILETYPE5).c_str(),
                g_stFileNameInfo.strBackupName.c_str());
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
        }

READ:
        // 絶対パス付き入退室読み込みデータファイル名を設定
        strncpy_s(
            cReadLocalName,
            sizeof(cReadLocalName),
            g_stFileNameInfo.strReadName.c_str(),
            sizeof(cReadLocalName) - 1);
        sprintf_s(
            g_clsMainLogFile.m_cLogData,
            sizeof(g_clsMainLogFile.m_cLogData),
            "ローカルの%s名：%s",
            g_maFileName.at(tagFileType::FILETYPE1).c_str(),
            cReadLocalName);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

        // 入退室読み込みデータファイルを読み取りのバイナリ型でオープン
        eReadErrNo = fopen_s(pfReadFile, cReadLocalName, "rb");
        g_clsMainLogFile.m_stLogData = g_pclsMainSubOpe->CreateEventMessageProc(ACTIONSET, cReadLocalName);
        sprintf_s(
            g_clsMainLogFile.m_cLogData,
            sizeof(g_clsMainLogFile.m_cLogData),
            "***** %sオープン開始 *****",
            g_maFileName.at(tagFileType::FILETYPE1).c_str());
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, g_clsMainLogFile.m_cLogData);
        sprintf_s(g_clsMainLogFile.m_cLogData, sizeof(g_clsMainLogFile.m_cLogData), "オープン結果：%d", eReadErrNo);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

        // ファイルオープン結果をチェック
        if (0 != eReadErrNo)
        {
            // ファイルオープン失敗
            // エラーコードを設定して例外処理へ移動
            sprintf_s(
                g_clsMainLogFile.m_cLogData,
                sizeof(g_clsMainLogFile.m_cLogData),
                "***** %sオープン異常終了 *****",
                g_maFileName.at(tagFileType::FILETYPE1).c_str());
            LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, g_clsMainLogFile.m_cLogData);
            iStartResult = CM_FILEOPENERROR;
            g_clsMainLogFile.m_eErrorResultCode = tagErrorCode::FILEOPENERROR;
            g_clsMainLogFile.m_maErrorActionName.insert(
                make_pair(
                    iStartResult, 
                    g_clsMainLogFile.m_stLogData.pcActionName));
            throw iStartResult;
        }
        else
        {
            // ファイルオープン成功
            // ファイルオープン成功のメッセージをログに出力
            sprintf_s(
                g_clsMainLogFile.m_cLogData,
                sizeof(g_clsMainLogFile.m_cLogData),
                "***** %sオープン正常終了 *****",
                g_maFileName.at(tagFileType::FILETYPE1).c_str());
            LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, g_clsMainLogFile.m_cLogData);
            sprintf_s(
                g_clsMainLogFile.m_cLogData,
                sizeof(g_clsMainLogFile.m_cLogData),
                "%s %s", g_clsMainLogFile.m_stLogData.pcActionName,
                FILEOPENSUCCESS);
            LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, g_clsMainLogFile.m_cLogData);
        }

        // 絶対パス変換ファイル名取得処理
        // 入退室書き込みデータファイル名を絶対パス付きファイル名に変換
        eFileType = tagFileType::FILETYPE2;
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 絶対パス変換ファイル名取得関数呼び出し *****");
        g_stFileNameInfo.strWriteName = g_pclsMainFileOpe->GetConvertFileNameProc(
            eFileType,
            cWriteLocalName,
            vecCmdData.at(0).c_str(),
            (char *)WRITEPATH, 
            (char *)DEFAULTFILENAME);
        sprintf_s(
            g_clsMainLogFile.m_cLogData,
            sizeof(g_clsMainLogFile.m_cLogData),
            "絶対パス付き%s名：%s",
            g_maFileName.at(tagFileType::FILETYPE2).c_str(),
            g_stFileNameInfo.strWriteName.c_str());
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

        // 退避した入退室書き込みデータファイル名を設定
        strncpy_s(
            cWriteLocalName,
            sizeof(cWriteLocalName),
            g_stFileNameInfo.strWriteName.c_str(),
            sizeof(cWriteLocalName) - 1);
        sprintf_s(
            g_clsMainLogFile.m_cLogData,
            sizeof(g_clsMainLogFile.m_cLogData),
            "ローカルの%s名：%s",
            g_maFileName.at(tagFileType::FILETYPE2).c_str(),
            cWriteLocalName);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

        // 入退室書き込みデータファイルを更新追記型でオープン
        sprintf_s(
            g_clsMainLogFile.m_cLogData,
            sizeof(g_clsMainLogFile.m_cLogData),
            "***** %sオープン開始 *****",
            g_maFileName.at(tagFileType::FILETYPE2).c_str());
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, g_clsMainLogFile.m_cLogData);
        eWriteErrNo = fopen_s(pfWriteFile, cWriteLocalName, "a+");
        g_clsMainLogFile.m_stLogData = g_pclsMainSubOpe->CreateEventMessageProc(ACTIONSET, cWriteLocalName);
        sprintf_s(g_clsMainLogFile.m_cLogData, sizeof(g_clsMainLogFile.m_cLogData), "オープン結果：%d", eWriteErrNo);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

        // ファイルオープン結果をチェック
        if (0 != eWriteErrNo)
        {
            // ファイルオープン失敗
            // エラーコードを設定して例外処理へ移動
            sprintf_s(
                g_clsMainLogFile.m_cLogData,
                sizeof(g_clsMainLogFile.m_cLogData),
                "***** %sオープン異常終了 *****",
                g_maFileName.at(tagFileType::FILETYPE2).c_str());
            LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, g_clsMainLogFile.m_cLogData);
            iStartResult = CM_FILEOPENERROR;
            g_clsMainLogFile.m_eErrorResultCode = tagErrorCode::FILEOPENERROR;
            g_clsMainLogFile.m_maErrorActionName.insert(
                make_pair(
                    iStartResult, 
                    g_clsMainLogFile.m_stLogData.pcActionName));
            throw iStartResult;
        }
        else
        {
            // ファイルオープン成功
            // ファイルオープン成功のメッセージをログに出力
            sprintf_s(
                g_clsMainLogFile.m_cLogData,
                sizeof(g_clsMainLogFile.m_cLogData),
                "***** %sオープン正常終了 *****",
                g_maFileName.at(tagFileType::FILETYPE2).c_str());
            LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, g_clsMainLogFile.m_cLogData);
            sprintf_s(
                g_clsMainLogFile.m_cLogData,
                sizeof(g_clsMainLogFile.m_cLogData),
                "%s %s",
                g_clsMainLogFile.m_stLogData.pcActionName,
                FILEOPENSUCCESS);
            LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, g_clsMainLogFile.m_cLogData);
        }
    }

    // 例外処理
    catch (const int iExecErr)
    {
        // エラーメッセージ生成処理
        // エラーメッセージをログに出力
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** エラーメッセージ生成関数呼び出し *****");
        g_clsMainLogFile.m_stLogData = g_pclsMainSubOpe->CreateEventMessageProc(
            ERRORSET,
            g_clsMainLogFile.m_maErrorActionName.at(iExecErr).c_str(),
            g_clsMainLogFile.m_eErrorResultCode);
        LOGOUTPUTERROR(
            TRACEOUTPUTLOG,
            ERRORINFORMATION1,
            g_clsMainLogFile.m_stLogData.pcMessage,
            g_clsMainLogFile.m_stLogData.eErrorCode);
        sprintf_s(g_clsMainLogFile.m_cLogData, sizeof(g_clsMainLogFile.m_cLogData), "戻り値は：%d", iExecErr);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

        // 呼び出しに対する異常終了のメッセージをログに出力
        LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** 入退室状況印字制御開始処理異常終了 *****");
    }

    // 後処理
    // エラーフラグをチェック
    if (ERR_ON == iErrFlag)
    {
        // エラーフラグがON
        // エラーの数分ループ（mapオブジェクトのイテレータ使用）
        for (auto auErrItr = g_clsMainLogFile.m_maErrorActionName.begin(); 
                  auErrItr != g_clsMainLogFile.m_maErrorActionName.end(); 
                  ++auErrItr)
        {
            // エラーメッセージ生成処理
            // エラーメッセージをログに出力
            LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** エラーメッセージ生成関数呼び出し *****");
            g_clsMainLogFile.m_stLogData = g_pclsMainSubOpe->CreateEventMessageProc(
                ERRORSET,
                auErrItr->second.c_str(),
                (ERRORCODETABLE)auErrItr->first);
            LOGOUTPUTERROR(
                TRACEOUTPUTLOG,
                ERRORINFORMATION1,
                g_clsMainLogFile.m_stLogData.pcMessage,
                g_clsMainLogFile.m_stLogData.eErrorCode);
            sprintf_s(
                g_clsMainLogFile.m_cLogData,
                sizeof(g_clsMainLogFile.m_cLogData),
                "戻り値は：%d",
                (ERRORCODETABLE)auErrItr->first);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
        }
    }
    else
    {
        // エラーフラグがOFF（呼び出し成功）
        // 呼び出しに対する正常終了のメッセージをログに出力
        LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, "***** 入退室状況印字制御開始処理正常終了 *****");
        sprintf_s(g_clsMainLogFile.m_cLogData, sizeof(g_clsMainLogFile.m_cLogData), "戻り値は：%d", iStartResult);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
    }

    // 日時情報取得処理
    // 入退室状況印字制御開始処理終了日時取得
    g_clsMainLogFile.m_stTimeData = g_clsMainLogFile.GetDateTimeProc();
    sprintf_s(
        g_clsMainLogFile.m_cLogData, 
        sizeof(g_clsMainLogFile.m_cLogData), 
        "入退室状況印字制御開始処理終了日時：%s %s", 
        (char *)g_clsMainLogFile.m_stTimeData.strMachDatePlus.c_str(), 
        (char *)g_clsMainLogFile.m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, g_clsMainLogFile.m_cLogData);
    return iStartResult;
}

//********************************************
// 関数名：入退室状況印字制御終了処理
// Function Name：DataControlEndProc
// 
// 処理概要：
// アプリケーション終了に基づいて後処理を行う
// 
// 処理手順：
//   １．入退室書き込みデータファイルクローズ
//   ２．入退室読み込みデータファイルクローズ
//   ３．入退室読み込みデータファイル状態取得
//   ４．入退室書き込みデータファイル状態取得
//   ５．入退室読み込みデータファイル名→バックアップファイル名へのリネーム
//   ６．入退室読み込みデータファイル削除
//   ７．入退室書き込みデータファイル削除
//   ８．エラーメッセージログ出力
// 
// 引数：     入退室読み込みデータファイルポインタ
//            入退室書き込みデータファイルポインタ
//            コマンド情報
//            ファイルコピー呼び出し実行結果
// 戻り値：   実行結果（正常終了はstdlib.hで定義している内容を利用）
//            正常終了：EXIT_SUCCESS（0）
//            異常終了：ファイルクローズエラー：CM_FILECLOSEERROR（105）
//                      ファイル名変換エラー：CM_RENAMEERROR（111）
//                      ファイル削除エラー：CM_FILEDELETEERROR（112）
//                      状態取得エラー：CM_FILESTATERROR（113）
// 
// 補足事項
// ・上記３, ４はオープン中のみ実行する
// ・上記５はファイルコピーに失敗した場合のみ実行する
// ・上記６はファイルコピー成功且つファイルが存在する場合のみ実行する
// ・上記７は書き込みファイルにデータがない場合のみ実行する
// ・上記８はエラーが発生した場合のみ実行する
// 
// 
// 作成履歴
// 2020/9/25  s-saito   新規作成
// 
// 更新履歴
// 
// 
//********************************************
int DataControlEndProc(
    FILE **pfReadFile,
    FILE **pfWriteFile,
    vector<string> vecCmdData,
    int iCopyResult)
{
    // 変数宣言
    int iEndResult = EXIT_SUCCESS;
    int iCloseResult = EXIT_SUCCESS;
    int iDeleteResult = EXIT_SUCCESS;
    int iRenameResult = EXIT_SUCCESS;
    int iFileStatResult = EXIT_FAILURE;
    FILETYPETABLE eFileType = tagFileType::NORMAL;
    string strBackUpLocalName;
    deque<string> deqBackUpFileName;
    string strBackUpFileName;
    string strCheckStatFileName;

    // 前処理
    // 日時情報取得処理
    // 入退室状況印字制御終了処理開始日時取得
    g_clsMainLogFile.m_stTimeData = g_clsMainLogFile.GetDateTimeProc();
    sprintf_s(
        g_clsMainLogFile.m_cLogData, 
        sizeof(g_clsMainLogFile.m_cLogData), 
        "入退室状況印字制御終了処理開始日時：%s %s", 
        (char *)g_clsMainLogFile.m_stTimeData.strMachDatePlus.c_str(), 
        (char *)g_clsMainLogFile.m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, g_clsMainLogFile.m_cLogData);
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 入退室状況印字制御終了処理開始 *****");

    // 主処理
    try
    {
        // ファイルがオープンされているかチェック
        // 入退室書き込みデータファイル
        if (NULL != *pfWriteFile)
        {
            // 入退室書き込みデータファイルオープン中
            // 入退室書き込みデータファイルをクローズ
            sprintf_s(
                g_clsMainLogFile.m_cLogData,
                sizeof(g_clsMainLogFile.m_cLogData),
                "***** %sクローズ開始 *****",
                g_maFileName.at(tagFileType::FILETYPE2).c_str());
            LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, g_clsMainLogFile.m_cLogData);
            iCloseResult = fclose(*pfWriteFile);
            g_clsMainLogFile.m_stLogData = g_pclsMainSubOpe->CreateEventMessageProc(
                ACTIONSET,
                g_stFileNameInfo.strWriteName.c_str());
            sprintf_s(g_clsMainLogFile.m_cLogData, sizeof(g_clsMainLogFile.m_cLogData), "結果：%d", iCloseResult);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

            // 入退室書き込みデータファイルをクローズ出来たかチェック
            if (EOF == iCloseResult)
            {
                // クローズ失敗
                // エラーとしてエラーコードを設定して例外処理へスロー
                iEndResult = CM_FILECLOSEERROR;
                g_clsMainLogFile.m_eErrorResultCode = tagErrorCode::FILECLOSEERROR;
                g_clsMainLogFile.m_maErrorActionName.insert(
                    make_pair(
                        iEndResult, 
                        g_clsMainLogFile.m_stLogData.pcActionName));
                sprintf_s(
                    g_clsMainLogFile.m_cLogData,
                    sizeof(g_clsMainLogFile.m_cLogData),
                    "***** %sクローズ異常終了 *****",
                    g_maFileName.at(tagFileType::FILETYPE2).c_str());
                LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, g_clsMainLogFile.m_cLogData);
                throw iEndResult;
            }
            else
            {
                // ファイルクローズ成功
                // ファイルクローズ成功のメッセージをログに出力
                sprintf_s(g_clsMainLogFile.m_cLogData, sizeof(g_clsMainLogFile.m_cLogData), "結果：%d", iCloseResult);
                LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
                sprintf_s(
                    g_clsMainLogFile.m_cLogData,
                    sizeof(g_clsMainLogFile.m_cLogData),
                    "***** %sクローズ正常終了 *****",
                    g_maFileName.at(tagFileType::FILETYPE2).c_str());
                LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, g_clsMainLogFile.m_cLogData);
                sprintf_s(
                    g_clsMainLogFile.m_cLogData,
                    sizeof(g_clsMainLogFile.m_cLogData),
                    "%s %s",
                    g_clsMainLogFile.m_stLogData.pcActionName,
                    FILECLOSESUCCESS);
                LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, g_clsMainLogFile.m_cLogData);
            }

            // 入退室書き込みデータファイルの存在状態を取得
            LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ファイル状態チェック関数呼び出し *****");
            memset(&g_pclsMainRecOpe->m_stStat, 0x00, sizeof(g_pclsMainRecOpe->m_stStat));
            iFileStatResult = g_pclsMainFileOpe->CheckFileStatProc(
                g_stFileNameInfo.strWriteName.c_str(),
                &g_pclsMainRecOpe->m_stStat);
            sprintf_s(
                g_clsMainLogFile.m_cLogData, 
                sizeof(g_clsMainLogFile.m_cLogData), 
                "ファイル存在チェック結果：%d", 
                iFileStatResult);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
            strCheckStatFileName =
                ("" == g_stFileNameInfo.strWriteName) ?
                g_maFileName.at(tagFileType::FILETYPE2) :
                g_stFileNameInfo.strWriteName;
            g_clsMainLogFile.m_stLogData = g_pclsMainSubOpe->CreateEventMessageProc(
                ACTIONSET, 
                strCheckStatFileName.c_str());

            // 結果をチェック
            if (EXIT_SUCCESS != iFileStatResult)
            {
                // ファイルが存在しない
                // エラーコード及び対象ファイル名を設定して例外処理へスロー
                LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** ファイル状態チェック異常終了 *****");
                iEndResult = CM_FILESTATERROR;
                g_clsMainLogFile.m_eErrorResultCode = tagErrorCode::STATERROR;
                g_clsMainLogFile.m_maErrorActionName.insert(
                    make_pair(
                        iEndResult, 
                        g_clsMainLogFile.m_stLogData.pcActionName));
                sprintf_s(
                    g_clsMainLogFile.m_cLogData,
                    sizeof(g_clsMainLogFile.m_cLogData),
                    "ファイル名は：%s",
                    g_stFileNameInfo.strWriteName.c_str());
                LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
                throw iEndResult;
            }
            else
            {
                // ファイルが存在する
                // ファイル名をログに出力
                sprintf_s(
                    g_clsMainLogFile.m_cLogData,
                    sizeof(g_clsMainLogFile.m_cLogData),
                    "ファイル名は：%s",
                    g_stFileNameInfo.strWriteName.c_str());
                LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, g_clsMainLogFile.m_cLogData);

                // ファイルの状態取得成功のメッセージ及び入退室書き込みデータファイルサイズをログに出力
                LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, "***** ファイル状態チェック正常終了 *****");
                sprintf_s(
                    g_clsMainLogFile.m_cLogData,
                    sizeof(g_clsMainLogFile.m_cLogData),
                    "%s %s",
                    g_clsMainLogFile.m_stLogData.pcActionName,
                    GETSTATSUCCESS);
                LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, g_clsMainLogFile.m_cLogData);
                sprintf_s(
                    g_clsMainLogFile.m_cLogData,
                    sizeof(g_clsMainLogFile.m_cLogData),
                    "%sの現在のファイルサイズは：%d",
                    g_maFileName.at(tagFileType::FILETYPE2).c_str(),
                    g_pclsMainRecOpe->m_stStat.st_size);
                LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
            }
        }

        // 入退室読み込みデータファイル
        if (NULL != *pfReadFile)
        {
            // 入退室読み込みデータファイルオープン中
            // 入退室読み込みデータファイルをクローズ
            sprintf_s(
                g_clsMainLogFile.m_cLogData,
                sizeof(g_clsMainLogFile.m_cLogData),
                "***** %sクローズ開始 *****",
                g_maFileName.at(tagFileType::FILETYPE1).c_str());
            LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, g_clsMainLogFile.m_cLogData);
            iCloseResult = fclose(*pfReadFile);
            g_clsMainLogFile.m_stLogData = g_pclsMainSubOpe->CreateEventMessageProc(
                ACTIONSET,
                g_stFileNameInfo.strReadName.c_str());
            sprintf_s(g_clsMainLogFile.m_cLogData, sizeof(g_clsMainLogFile.m_cLogData), "結果：%d", iCloseResult);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

            // 入退室読み込みデータファイルをクローズ出来たかチェック
            if (EOF == iCloseResult)
            {
                // クローズ失敗
                // エラーとしてエラーコードを設定して例外処理へスロー
                iEndResult = CM_FILECLOSEERROR;
                g_clsMainLogFile.m_eErrorResultCode = tagErrorCode::FILECLOSEERROR;
                g_clsMainLogFile.m_maErrorActionName.insert(
                    make_pair(iEndResult, g_clsMainLogFile.m_stLogData.pcActionName));
                sprintf_s(
                    g_clsMainLogFile.m_cLogData,
                    sizeof(g_clsMainLogFile.m_cLogData),
                    "***** %sクローズ異常終了 *****",
                    g_maFileName.at(tagFileType::FILETYPE1).c_str());
                LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, g_clsMainLogFile.m_cLogData);
                throw iEndResult;
            }
            else
            {
                // ファイルクローズ成功
                // ファイルクローズ成功のメッセージをログに出力
                sprintf_s(
                    g_clsMainLogFile.m_cLogData,
                    sizeof(g_clsMainLogFile.m_cLogData),
                    "***** %sクローズ正常終了 *****",
                    g_maFileName.at(tagFileType::FILETYPE1).c_str());
                LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, g_clsMainLogFile.m_cLogData);
                sprintf_s(
                    g_clsMainLogFile.m_cLogData,
                    sizeof(g_clsMainLogFile.m_cLogData),
                    "%s %s",
                    g_clsMainLogFile.m_stLogData.pcActionName, FILECLOSESUCCESS);
                LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, g_clsMainLogFile.m_cLogData);
            }
        }

        // ファイルコピーエラーが発生しているかチェック
        sprintf_s(
            g_clsMainLogFile.m_cLogData, 
            sizeof(g_clsMainLogFile.m_cLogData), 
            "ファイルコピー結果は：%d", 
            iCopyResult);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
        if (CM_FILECOPYERROR == iCopyResult)
        {
            // ファイルコピーエラー発生
            // 入退室読み込みデータファイル名を基にバックアップファイル名を再生成
            // 入退室読み込みデータファイルを分割
            strBackUpLocalName = g_stFileNameInfo.strReadName;
            deqBackUpFileName = g_pclsMainFileOpe->SplitDataProc(strBackUpLocalName, '\\');
            strBackUpFileName = deqBackUpFileName.back();
            sprintf_s(
                g_clsMainLogFile.m_cLogData,
                sizeof(g_clsMainLogFile.m_cLogData),
                "%s名：%s", g_maFileName.at(tagFileType::FILETYPE5).c_str(),
                strBackUpFileName.c_str());
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

            // 絶対パス変換ファイル名取得処理
            // バックアップファイル名を絶対パス付きに変換
            eFileType = tagFileType::FILETYPE5;
            LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 絶対パス変換ファイル名取得関数呼び出し *****");
            g_stFileNameInfo.strBackupName = g_pclsMainFileOpe->GetConvertFileNameProc(
                eFileType,
                strBackUpFileName.c_str(),
                vecCmdData.at(0).c_str(),
                (char *)COPYPATH);
            sprintf_s(
                g_clsMainLogFile.m_cLogData,
                sizeof(g_clsMainLogFile.m_cLogData),
                "絶対パス付き%s名：%s",
                g_maFileName.at(tagFileType::FILETYPE5).c_str(),
                g_stFileNameInfo.strBackupName.c_str());
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

            // 絶対パス変換ファイル名取得処理
            // バックアップファイル名を基に名称変更対象ファイル名を取得
            eFileType = tagFileType::FILETYPE6;
            LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 絶対パス変換ファイル名取得関数呼び出し *****");
            g_stFileNameInfo.strChangeName = g_pclsMainFileOpe->GetConvertFileNameProc(
                eFileType,
                g_stFileNameInfo.strBackupName.c_str(),
                vecCmdData.at(0).c_str());
            sprintf_s(
                g_clsMainLogFile.m_cLogData,
                sizeof(g_clsMainLogFile.m_cLogData),
                "絶対パス付き%s名：%s",
                g_maFileName.at(tagFileType::FILETYPE6).c_str(),
                g_stFileNameInfo.strChangeName.c_str());
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

            // ファイル名変換処理
            // 入退室読み込みデータファイル名をバックアップファイル名にリネームしてバックアップパスへ移動
            // ファイルを移動させる場合もChangeFileNameProc関数のrename関数を使用する
            LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ファイル名変換関数呼び出し *****");
            iRenameResult = g_pclsMainFileOpe->ChangeFileNameProc(
                strBackUpLocalName.c_str(),
                g_stFileNameInfo.strChangeName.c_str());
            g_clsMainLogFile.m_stLogData = g_pclsMainSubOpe->CreateEventMessageProc(
                ACTIONSET,
                g_stFileNameInfo.strChangeName.c_str());
            sprintf_s(g_clsMainLogFile.m_cLogData, sizeof(g_clsMainLogFile.m_cLogData), "実行結果：%d", iRenameResult);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

            // リネーム出来たかチェック
            if (CM_RENAMEERROR == iRenameResult)
            {
                // コピーしたファイルに対するリネーム失敗
                // エラーコードを設定して例外処理へ移動
                iEndResult = CM_RENAMEERROR;
                g_clsMainLogFile.m_eErrorResultCode = tagErrorCode::RENAMEERROR;
                g_clsMainLogFile.m_maErrorActionName.insert(
                    make_pair(
                        iEndResult, 
                        g_clsMainLogFile.m_stLogData.pcActionName));
                LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** ファイル名変換関数呼び出し異常終了 *****");
                throw iEndResult;
            }
            else
            {
                // リネーム成功
                // リネーム成功のメッセージをログに出力
                LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, "***** ファイル名変換関数呼び出し正常終了 *****");
                sprintf_s(
                    g_clsMainLogFile.m_cLogData,
                    sizeof(g_clsMainLogFile.m_cLogData),
                    "%s %s", g_clsMainLogFile.m_stLogData.pcActionName,
                    RENAMESUCCESS);
                LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, g_clsMainLogFile.m_cLogData);
                sprintf_s(g_clsMainLogFile.m_cLogData, sizeof(g_clsMainLogFile.m_cLogData), "戻り値は：%d", iRenameResult);
                LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

                // リネーム対象の名称変更対象ファイル名をバックアップファイル名にコピー
                g_stFileNameInfo.strBackupName = g_stFileNameInfo.strChangeName;
                sprintf_s(
                    g_clsMainLogFile.m_cLogData,
                    sizeof(g_clsMainLogFile.m_cLogData),
                    "絶対パス付き%s名：%s",
                    g_maFileName.at(tagFileType::FILETYPE5).c_str(),
                    g_stFileNameInfo.strBackupName.c_str());
                LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
            }
        }
        else if (CM_FILECOPYERROR != iCopyResult && EXIT_SUCCESS != iCopyResult)
        {
            // ファイルコピー以外のエラー発生
            // ファイルコピー異常終了のメッセージをログに出力
            LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** ファイルコピー処理にて異常終了 *****");
        }
        else
        {
            // ファイルコピー成功
            // 絶対パス変換ファイル名取得処理
            // 削除対象ファイル名を取得
            eFileType = tagFileType::FILETYPE7;
            LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 絶対パス変換ファイル名取得関数呼び出し *****");
            g_stFileNameInfo.strDeleteName = g_pclsMainFileOpe->GetConvertFileNameProc(
                eFileType,
                g_stFileNameInfo.strReadName.c_str(),
                vecCmdData.at(0).c_str());
            sprintf_s(
                g_clsMainLogFile.m_cLogData,
                sizeof(g_clsMainLogFile.m_cLogData),
                "絶対パス付き%s名：%s",
                g_maFileName.at(tagFileType::FILETYPE7).c_str(),
                g_stFileNameInfo.strDeleteName.c_str());
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

            // ファイル削除処理
            // 元の入退室読み込みデータファイルは不要のためファイル削除処理を実行
            LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ファイル削除関数呼び出し *****");
            iDeleteResult = g_pclsMainFileOpe->DeleteFileProc(g_stFileNameInfo.strDeleteName.c_str());
            g_clsMainLogFile.m_stLogData = g_pclsMainSubOpe->CreateEventMessageProc(
                ACTIONSET,
                g_stFileNameInfo.strDeleteName.c_str());
            sprintf_s(g_clsMainLogFile.m_cLogData, sizeof(g_clsMainLogFile.m_cLogData), "実行結果：%d", iDeleteResult);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

            // エラーが発生していないかチェック
            if (CM_FILEDELETEERROR == iDeleteResult)
            {
                // ファイル削除失敗
                // エラーコードを設定して例外処理へ移動
                iEndResult = CM_FILEDELETEERROR;
                g_clsMainLogFile.m_eErrorResultCode = tagErrorCode::DELETEERROR;
                g_clsMainLogFile.m_maErrorActionName.insert(
                    make_pair(
                        iEndResult, 
                        g_clsMainLogFile.m_stLogData.pcActionName));
                LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** ファイル削除関数呼び出し異常終了 *****");
                throw iEndResult;
            }
            else
            {
                // ファイル削除成功
                // ファイル削除成功のメッセージをログに出力
                LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, "***** ファイル削除関数呼び出し正常終了 *****");
                sprintf_s(
                    g_clsMainLogFile.m_cLogData,
                    sizeof(g_clsMainLogFile.m_cLogData),
                    "%s %s",
                    g_clsMainLogFile.m_stLogData.pcActionName,
                    DELETESUCCESS);
                LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, g_clsMainLogFile.m_cLogData);
                sprintf_s(g_clsMainLogFile.m_cLogData, sizeof(g_clsMainLogFile.m_cLogData), "戻り値は：%d", iDeleteResult);
                LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
            }
        }

        // 入退室書き込みデータファイルのサイズをチェック
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ファイルサイズのチェックを実行 *****");
        sprintf_s(
            g_clsMainLogFile.m_cLogData,
            sizeof(g_clsMainLogFile.m_cLogData),
            "ファイルサイズは：%d", g_pclsMainRecOpe->m_stStat.st_size);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
        if (EXIT_SUCCESS == iFileStatResult && 0 == g_pclsMainRecOpe->m_stStat.st_size)
        {
            // ファイルに書き込みがない
            // ファイル名をログに出力
            sprintf_s(
                g_clsMainLogFile.m_cLogData,
                sizeof(g_clsMainLogFile.m_cLogData),
                "ファイル名は：%s",
                g_stFileNameInfo.strWriteName.c_str());
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

            // 絶対パス変換ファイル名取得処理
            // 削除対象ファイル名を取得
            eFileType = tagFileType::FILETYPE7;
            LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 絶対パス変換ファイル名取得関数呼び出し *****");
            g_stFileNameInfo.strDeleteName = g_pclsMainFileOpe->GetConvertFileNameProc(
                eFileType,
                g_stFileNameInfo.strWriteName.c_str(),
                vecCmdData.at(0).c_str());
            sprintf_s(
                g_clsMainLogFile.m_cLogData,
                sizeof(g_clsMainLogFile.m_cLogData),
                "絶対パス付き%s名：%s",
                g_maFileName.at(tagFileType::FILETYPE7).c_str(),
                g_stFileNameInfo.strDeleteName.c_str());
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

            // ファイル削除処理
            // 対象とするファイルを削除
            LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ファイル削除関数呼び出し *****");
            iDeleteResult = g_pclsMainFileOpe->DeleteFileProc(g_stFileNameInfo.strDeleteName.c_str());
            g_clsMainLogFile.m_stLogData = g_pclsMainSubOpe->CreateEventMessageProc(
                ACTIONSET,
                g_stFileNameInfo.strDeleteName.c_str());
            sprintf_s(g_clsMainLogFile.m_cLogData, sizeof(g_clsMainLogFile.m_cLogData), "実行結果：%d", iDeleteResult);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

            // エラーが発生していないかチェック
            if (CM_FILEDELETEERROR == iDeleteResult)
            {
                // ファイル削除失敗
                // エラーコードを設定して例外処理へ移動
                iEndResult = CM_FILEDELETEERROR;
                g_clsMainLogFile.m_eErrorResultCode = tagErrorCode::DELETEERROR;
                g_clsMainLogFile.m_maErrorActionName.insert(
                    make_pair(
                        iEndResult, 
                        g_clsMainLogFile.m_stLogData.pcActionName));
                LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** ファイル削除関数呼び出し異常終了 *****");
                throw iEndResult;
            }
            else
            {
                // ファイル削除成功
                // ファイル削除成功のメッセージをログに出力
                LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, "***** ファイル削除関数呼び出し正常終了 *****");
                sprintf_s(
                    g_clsMainLogFile.m_cLogData,
                    sizeof(g_clsMainLogFile.m_cLogData),
                    "%s の%s",
                    g_clsMainLogFile.m_stLogData.pcActionName,
                    DELETESUCCESS);
                LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, g_clsMainLogFile.m_cLogData);
                sprintf_s(g_clsMainLogFile.m_cLogData, sizeof(g_clsMainLogFile.m_cLogData), "戻り値は：%d", iDeleteResult);
                LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
            }
        }
    }

    // 例外処理
    catch (const int iExecErr)
    {
        // エラーメッセージ生成処理
        // エラーメッセージをログに出力
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 入退室状況印字制御終了処理異常終了 *****");
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** エラーメッセージ生成関数呼び出し *****");
        g_clsMainLogFile.m_stLogData = g_pclsMainSubOpe->CreateEventMessageProc(
            ERRORSET,
            g_clsMainLogFile.m_stLogData.pcActionName,
            g_clsMainLogFile.m_eErrorResultCode);
        LOGOUTPUTERROR(
            TRACEOUTPUTLOG,
            ERRORINFORMATION1,
            g_clsMainLogFile.m_stLogData.pcMessage,
            g_clsMainLogFile.m_stLogData.eErrorCode);
        sprintf_s(g_clsMainLogFile.m_cLogData, sizeof(g_clsMainLogFile.m_cLogData), "戻り値は：%d", iExecErr);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
    }

    // 後処理
    // 日時情報取得処理
    // 入退室状況印字制御終了処理終了日時取得
    LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, "***** 入退室状況印字制御終了処理正常終了 *****");
    g_clsMainLogFile.m_stTimeData = g_clsMainLogFile.GetDateTimeProc();
    sprintf_s(
        g_clsMainLogFile.m_cLogData, 
        sizeof(g_clsMainLogFile.m_cLogData), 
        "入退室状況印字制御終了処理終了日時：%s %s", 
        (char *)g_clsMainLogFile.m_stTimeData.strMachDatePlus.c_str(), 
        (char *)g_clsMainLogFile.m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, g_clsMainLogFile.m_cLogData);
    return iEndResult;
}

//********************************************
// 関数名：DLL アプリケーションエントリポイント処理
// Function Name：DllMain
// 
// 処理概要：
// アプリケーションから最初と最後に呼び出されて
// 入退室状況印字実行に対して準備・後片付けを行う
// 
// 処理手順：
// （プロセスアタッチ時）
//   １．コマンドライン引数取得処理
//   ２．コマンドライン解析処理
// （プロセスデタッチ時）
//   ３．コマンドライン引数削除
//   ４．各クラス削除
// 
// 引数：      DLLモジュールハンドル
//             関数呼び出し内容
//             予約済み定義
// 戻り値：    正常終了：TRUE
// 
// 補足事項
// ・当機能はプロセスアタッチ時及びプロセスデタッチ時のみ実行する
// 
// 作成履歴
// 2020/9/25  s-saito   新規作成
// 
// 更新履歴
// 
// 
//********************************************
BOOL APIENTRY DllMain(
    HMODULE hModule,
    DWORD ul_reason_for_call,
    LPVOID lpReserved)
{
    // 変数宣言
    string strCommandLine;

    // メモリリーク検出設定
    // （_CrtSetDbgFlagはC++言語定義）
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    // CRTメモリ検知イベントハンドラ設定
    // 第一引数には _CRT_RPTHOOK_INSTALL（ハンドラ追加）を指定
    // 第二引数にはイベントハンドラを指定
    _CrtSetReportHook2(_CRT_RPTHOOK_INSTALL, MemoryLeakDookProc);

    // エントリーポイントに基づいて処理を分岐
    switch (ul_reason_for_call)
    {
        // プロセスアタッチ時
        case DLL_PROCESS_ATTACH:
        {
            // PC閲覧画面タイトル設定
            // （SetConsoleTitleはC++言語定義）
            SetConsoleTitle(TEXT("入退室状況印字実行画面"));

            // コマンドライン引数取得
            // （GetCommandLineAはC++言語定義）
            strCommandLine = GetCommandLineA();

            // コマンドライン引数を解析
            g_vecParseArgc = g_clsMainSubOpe.ParseArgsProc(strCommandLine, &g_iParseArgv);
            if (0 == g_vecParseArgc.size())
            {
                // 解析に失敗
                // デバッグウィンドウに出力
                // （OutputDebugStringAはC++言語定義）
                OutputDebugStringA("コマンド情報が解析出来ません。\n");
                OutputDebugStringA("\n");

#ifdef SCREEN
                // PC閲覧画面に出力
                printf_s("コマンド情報が解析出来ません。\n");

                // PC閲覧画面の自動クローズを防ぐため一時停止
                system("pause");
#endif // SCREEN

                // アプリケーション終了
                exit(EXIT_FAILURE);
            }
            else
            {
                // 解析に成功
                // ファイル出力タイプをキーとしたファイル名テーブル生成
                g_maFileName.insert(make_pair(tagFileType::FILETYPE1, "入退室読み込みデータファイル"));
                g_maFileName.insert(make_pair(tagFileType::FILETYPE2, "入退室書き込みデータファイル"));
                g_maFileName.insert(make_pair(tagFileType::FILETYPE3, "トレースログ出力ファイル"));
                g_maFileName.insert(make_pair(tagFileType::FILETYPE4, "データログ出力ファイル"));
                g_maFileName.insert(make_pair(tagFileType::FILETYPE5, "バックアップファイル"));
                g_maFileName.insert(make_pair(tagFileType::FILETYPE6, "名称変更対象ファイル"));
                g_maFileName.insert(make_pair(tagFileType::FILETYPE7, "削除対象ファイル"));
                g_maFileName.insert(make_pair(tagFileType::FILETYPE8, "実行アプリケーション"));
                g_maFileName.insert(make_pair(tagFileType::FILETYPE9, "不正データファイル"));

                // 分岐抜け
                break;
            }
        }

        // スレッドアタッチ時
        case DLL_THREAD_ATTACH:
        {
            // 分岐抜け
            break;
        }

        // スレッドデタッチ時
        case DLL_THREAD_DETACH:
        {
            // 分岐抜け
            break;
        }

        // プロセスデタッチ時
        case DLL_PROCESS_DETACH:
        {
            // コマンドライン引数削除
            strCommandLine.clear();
            g_iParseArgv = 0;
            g_vecParseArgc.clear();

            // マップテーブル情報を削除
            g_maFileName.clear();
            g_clsMainLogFile.m_maErrorActionName.clear();

            // 各クラスメンバ変数削除
            // レコード操作クラス
            SAFE_FREE_NEWALLOC(g_pclsMainRecOpe);

            // ファイル操作クラス
            SAFE_FREE_NEWALLOC(g_pclsMainFileOpe);

            // 共通操作クラス
            SAFE_FREE_NEWALLOC(g_pclsMainSubOpe);

            // CRTメモリ検知イベントハンドラ設定
            // 第一引数には _CRT_RPTHOOK_REMOVE（ハンドラ削除）を指定
            // 第二引数にはイベントハンドラを指定
            _CrtSetReportHook2(_CRT_RPTHOOK_REMOVE, MemoryLeakDookProc);

            // 分岐抜け
            break;
        }

        // 上記以外
        default:
        {
            // 分岐抜け
            break;
        }
    }

    // 正常終了
    return TRUE;
}

//********************************************
// 関数名：CRTメモリ検知イベントハンドラ処理
// Function Name：MemoryLeakDookProc
// 
// 処理概要：
// メモリリーク検知時の処理をフックするイベントハンドラを登録する
// 
// 処理手順：
//   １．処理タイプを判別
//   ２．トレースログ出力
//   ３．見出し出力
// 
// 引数：      処理タイプ
//             CRTからの出力メッセージ
// 戻り値：    CRTメモリ検知イベントハンドラ使用有無
//             TRUE：_CrtSetReportHookを呼び出さない
//             FALSE：_CrtSetReportHookを呼び出す
// 
// 補足事項
// ・トレースログへの出力はメモリリークの場合のみとする
// 
// 作成履歴
// 2020/9/25  s-saito   新規作成
// 
// 更新履歴
// 
// 
//********************************************
int __cdecl MemoryLeakDookProc(
    int iReportType,
    char *pcLeakMessage,
    int *piReturnValue)
{
    // 処理タイプに応じて処理を分岐
    switch (iReportType)
    {
        case _CRT_ASSERT:
        {
            // _CrtSetReportHookを呼び出さないで終了
            return TRUE;
        }

        case _CRT_WARN:
        {
            // メモリリーク
            // デバッグウィンドウに出力
            // （OutputDebugStringAはC++言語定義）
            OutputDebugStringA(pcLeakMessage);
            break;
        }

        case _CRT_ERROR:
        {
            // assert等が呼ばれた場合
            // _CrtSetReportHookを呼び出さないで終了
            return TRUE;
        }

        default:
        {
            // 上記以外
            // _CrtSetReportHookを呼び出さないで終了
            return TRUE;
        }
    }

    // FALSE を返却する事により _CrtSetReportHook で登録したイベントハンドラ呼び出し
    return FALSE;
}