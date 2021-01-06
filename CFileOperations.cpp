//============================================
// ファイル操作クラスソースファイル
// CFileOperations.cpp
// 
// 機能概要
// ファイル操作等をサポートする機能を定義する
// 
// 機能内容
// １．ファイルコピー処理
// ２．ファイル名変換処理
// ３．ファイル削除処理
// ４．ファイル状態チェック処理
// ５．絶対パス変換ファイル名取得処理
// ６．ファイル名生成処理
// ７．絶対パス取得処理
// 
// 補足事項
// ・当機能はその他の機能でもサポートを可能とする
// 
// 作成履歴
// 2020/9/25  s-saito   新規作成
// 
// 更新履歴
// 
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
#include "CFileOperations.h"

// グローバル変数宣言
CLogFile g_clsFileOpeLogFile;

// マクロ定義
#define LOGOUTPUT(iOutputType, iTraceOutputLogType, cMessageInfo) \
    g_clsFileOpeLogFile.LogOutputControlProc(LOGARGS, iOutputType, iTraceOutputLogType, cMessageInfo)
#define LOGOUTPUTERROR(iOutputType, iTraceOutputLogType, cErrorMessageInfo, eErrorInfoCode) \
    g_clsFileOpeLogFile.LogOutputControlProc(LOGARGS, iOutputType, iTraceOutputLogType, cErrorMessageInfo, eErrorInfoCode)

//☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★
// 各機能（API）定義
//☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★
//********************************************
// 関数名：デフォルトコンストラクタ
// Function Name：CFileOperations
// 
// 処理概要：
// クラス生成に伴い初期化内容を定義する
// 
// 
// 補足事項
// ・現在デフォルトコンストラクタでの処理はなし
// 
// 
// 作成履歴
// 2020/9/25  s-saito   新規作成
// 
// 更新履歴
// 
// 
//********************************************
CFileOperations::CFileOperations()
{
    // 現在デフォルトコンストラクタでの処理はなし
}

//********************************************
// 関数名：パラメータ情報付きコンストラクタ
// Function Name：CFileOperations
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
CFileOperations::CFileOperations(
    const string strTraceLogFileName, 
    const string strDataLogFileName, 
    const string strCopyFileName)
{
    // ログ出力クラス生成
    g_clsFileOpeLogFile = CLogFile(strTraceLogFileName, strDataLogFileName);

    // 呼び出し元で指定されたファイル名設定
    m_strFileOpeTraceLogFileName = strTraceLogFileName;
    m_strFileOpeDataLogFileName = strDataLogFileName;
    m_strFileOpeCopyFileName = strCopyFileName;
    m_strSubOpeTraceLogFileName = strTraceLogFileName;
    m_strSubOpeDataLogFileName = strDataLogFileName;
}

//********************************************
// 関数名：デストラクタ
// Function Name：~CFileOperations
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
CFileOperations::~CFileOperations()
{
    // メンバ変数クリアメンバ変数クリア
    m_strFileOpeTraceLogFileName.clear();
    m_strFileOpeDataLogFileName.clear();
    m_strFileOpeCopyFileName.clear();
    m_strSubOpeTraceLogFileName.clear();
    m_strSubOpeDataLogFileName.clear();
}

//********************************************
// 関数名：ファイルコピー処理
// Function Name：CopyFileProc
// 
// 処理概要：
// コピー元とするファイルより
// コピー先とするファイルへレコードをコピーする
// 
// 処理手順：
//   １．コピー元ファイルオープン
//   ２．コピー先ファイルオープン
// （ここから無限ループ）
//   ３．ファイル読み込み
//   ４．ファイル書き込み
// （ここまで無限ループ）
//   ５．コピー先ファイルクローズ
//   ６．コピー元ファイルクローズ
// 
// 引数：      コピー元ファイル名
//             コピー先ファイル名
// 戻り値：    実行結果（正常終了はstdlib.hで定義している内容を利用）
//             正常終了：EXIT_SUCCESS（0）
//             異常終了：ファイルオープンエラー：CM_FILEOPENERROR（104）
//                       ファイルクローズエラー：CM_FILECLOSEERROR（105）
//                       ファイルコピーエラー：CM_FILECOPYERROR（110）
// 
// 補足事項
// ・無限ループを使用する場合は必ずループを抜ける条件を入れる
// 
// 
// 作成履歴
// 2020/9/25  s-saito   新規作成
// 
// 更新履歴
// 
// 
//********************************************
int CFileOperations::CopyFileProc(
	const char *pcBaseFileName, 
	const char *pcDestFileName)
{
    // 変数宣言
    FILE* pfBaseFile = NULL;
    FILE* pfDestFile = NULL;
    errno_t eFileErrNo = 0;
    int iCopyResult = EXIT_SUCCESS;
    int iReadResult = EXIT_SUCCESS;
    int iWriteResult = EXIT_SUCCESS;
    char* pcReadResult = NULL;
    int iCloseResult = EXIT_SUCCESS;
    char* pcCopyData = NULL;
    char cCopyData[DATASIZE + 1];
    char cCopyDataLogout[DATASIZE + 1];
    int iErrFlag = ERR_OFF;
    char cBaseFileName[FILENAMESIZE + 1];
    char cDestFileName[FILENAMESIZE + 1];

    // 初期化
    memset(cCopyData, 0x00, sizeof(cCopyData));
    memset(cCopyDataLogout, 0x00, sizeof(cCopyDataLogout));
    memset(cBaseFileName, 0x00, sizeof(cBaseFileName));
    memset(cDestFileName, 0x00, sizeof(cDestFileName));

    // 前処理
    // 日時情報取得処理
    // ファイルコピー開始日時取得
    m_stTimeData = g_clsFileOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "ファイルコピー開始日時：%s %s", 
        (char *)m_stTimeData.strMachDatePlus.c_str(), 
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ファイルコピー処理開始 *****");

    // パラメータ情報をコピー
    strncpy_s(cBaseFileName, sizeof(cBaseFileName), pcBaseFileName, sizeof(cBaseFileName) - 1);
    sprintf_s(m_cLogData, sizeof(m_cLogData), "コピー元ファイル名：%s", cBaseFileName);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
    strncpy_s(cDestFileName, sizeof(cDestFileName), pcDestFileName, sizeof(cDestFileName) - 1);
    sprintf_s(m_cLogData, sizeof(m_cLogData), "コピー先ファイル名：%s", cDestFileName);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    // コピー元ファイルをオープン
    eFileErrNo = fopen_s(&pfBaseFile, cBaseFileName, "rb");
    sprintf_s(m_cLogData, sizeof(m_cLogData), "ファイルオープン結果：%d", eFileErrNo);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
    m_stLogData = CreateEventMessageProc(ACTIONSET, cBaseFileName);

    // オープン結果をチェック
    if (0 != eFileErrNo)
    {
        // ファイルオープン失敗
        // エラーフラグをON
        iErrFlag = ERR_ON;

        // エラーコードを設定して後処理へ移動
        LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** ファイルコピー処理異常終了 *****");
        iCopyResult = CM_FILEOPENERROR;
        goto END;
    }
    else
    {
        // ファイルオープン成功
        // ファイルオープン成功のメッセージをログに出力
        sprintf_s(m_cLogData, sizeof(m_cLogData), "%s %s", m_stLogData.pcActionName, FILEOPENSUCCESS);
        LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, m_cLogData);
    }

    // コピー先ファイルをオープン
    eFileErrNo = fopen_s(&pfDestFile, cDestFileName, "wb");
    sprintf_s(m_cLogData, sizeof(m_cLogData), "ファイルオープン結果：%d", eFileErrNo);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
    m_stLogData = CreateEventMessageProc(ACTIONSET, cDestFileName);

    // オープン結果をチェック
    if (0 != eFileErrNo)
    {
        // ファイルオープン失敗
        // エラーフラグをON
        iErrFlag = ERR_ON;

        // エラーコードを設定して後処理へ移動
        LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** ファイルコピー処理異常終了 *****");
        iCopyResult = CM_FILEOPENERROR;
        goto END;
    }
    else
    {
        // ファイルオープン成功
        // ファイルオープン成功のメッセージをログに出力
        sprintf_s(m_cLogData, sizeof(m_cLogData), "%s %s", m_stLogData.pcActionName, FILEOPENSUCCESS);
        LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, m_cLogData);
    }

    // 主処理
    // 無限ループ実行
    // 注意！！
    // 必ずループ終了条件を入れること！！
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ファイル読み書き開始 *****");
    for (; ;)
    {
        // ファイルから1行読み込み
        pcReadResult = fgets(cCopyData, DATASIZE, pfBaseFile);
        m_stLogData = CreateEventMessageProc(ACTIONSET, cBaseFileName);

        // 改行コードを削除してログに出力
        DeleteDesignationCharProc(cCopyData, (const char)CRLF_CODE, cCopyDataLogout);
        sprintf_s(m_cLogData, sizeof(m_cLogData), "読み込み結果：%s", cCopyDataLogout);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

        // 読み込み結果をチェック
        if (NULL == pcReadResult)
        {
            // ファイルの終わり？
            iReadResult = feof(pfBaseFile);
            if (EXIT_SUCCESS != iReadResult)
            {
                // ファイルの終わり
                // 全てのデータが終了
                LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, "***** ファイル読み書き終了 *****");
                LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, "***** ファイルコピー処理正常終了 *****");

                // 処理終了
                LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, ALLREADEND);
                break;
            }
            else
            {
                // ファイルの終わりではない
                LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "読み込み失敗 m(。≧Д≦。)m");
                LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** ファイルコピー処理異常終了 *****");
                sprintf_s(m_cLogData, sizeof(m_cLogData), "ファイル名は： %s", m_stLogData.pcActionName);
                LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, m_cLogData);

                // エラーコードを設定
                iCopyResult = CM_FILECOPYERROR;
                goto END;
            }
        }
        else
        {
            // ファイル読み込み成功
            // ファイル読み込み成功のメッセージをログに出力
            LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, "読み込み成功 ( ^-^)ノ∠※。.:*:・'°☆");
            sprintf_s(m_cLogData, sizeof(m_cLogData), "%s %s", m_stLogData.pcActionName, FILEREADSUCCESS);
            LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, m_cLogData);
            sprintf_s(m_cLogData, sizeof(m_cLogData), "読み込み結果コード：%d", iCopyResult);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
        }

        // 読み込んだ1行を書き込み
        iWriteResult = fputs(cCopyData, pfDestFile);
        sprintf_s(m_cLogData, sizeof(m_cLogData), "書き込みデータ：%s 結果コード：%d", cCopyDataLogout, iWriteResult);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
        m_stLogData = CreateEventMessageProc(ACTIONSET, cDestFileName);

        // 書き込み結果をチェック
        if (EOF == iWriteResult)
        {
            // 書き込み失敗
            LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "書き込み失敗 ( *´・ω)/(；д； )");
            LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** ファイルコピー処理異常終了 *****");
            sprintf_s(m_cLogData, sizeof(m_cLogData), "ファイル名は： %s", m_stLogData.pcActionName);
            LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, m_cLogData);

            // エラーコードを設定
            iCopyResult = CM_FILECOPYERROR;
            goto END;
        }
        else
        {
            // ファイル書き込み成功
            // ファイル書き込み成功のメッセージをログに出力
            LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, "書き込み成功 ( ^-^)ノ∠※。.:*:・'°☆");
            sprintf_s(m_cLogData, sizeof(m_cLogData), "%s %s", m_stLogData.pcActionName, FILEWRITESUCCESS);
            LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, m_cLogData);
            sprintf_s(m_cLogData, sizeof(m_cLogData), "書き込み結果コード：%d", iWriteResult);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
        }
    }

END:
    // 後処理
    // ファイルをオープンしているかチェック
    // コピー先ファイル
    if (NULL != pfDestFile)
    {
        // ファイルをオープンしている
        // ファイルをクローズ
        iCloseResult = fclose(pfDestFile);
        m_stLogData = CreateEventMessageProc(ACTIONSET, "コピー先");

        // クローズ出来たかチェック
        if (EXIT_SUCCESS != iCloseResult)
        {
            // クローズ失敗
            // エラーコードを設定
            LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** ファイルコピー処理異常終了 *****");
            iCopyResult = CM_FILECLOSEERROR;
        }
        else
        {
            // ファイルクローズ成功
            // ファイルクローズ成功のメッセージをログに出力
            sprintf_s(m_cLogData, sizeof(m_cLogData), "%s%s", m_stLogData.pcActionName, FILECLOSESUCCESS);
            LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, m_cLogData);
        }
    }

    // コピー元ファイル
    if (NULL != pfBaseFile)
    {
        // ファイルをオープンしている
        // ファイルをクローズ
        iCloseResult = fclose(pfBaseFile);
        m_stLogData = CreateEventMessageProc(ACTIONSET, "コピー元");

        // クローズ出来たかチェック
        if (EXIT_SUCCESS != iCloseResult)
        {
            // クローズ失敗
            // エラーコードを設定
            LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** ファイルコピー処理異常終了 *****");
            iCopyResult = CM_FILECLOSEERROR;
        }
        else
        {
            // ファイルクローズ成功
            // ファイルクローズ成功のメッセージをログに出力
            sprintf_s(m_cLogData, sizeof(m_cLogData), "%s%s", m_stLogData.pcActionName, FILECLOSESUCCESS);
            LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, m_cLogData);
        }
    }

    // 日時情報取得処理
    // ファイルコピー終了日時取得
    m_stTimeData = g_clsFileOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "ファイルコピー終了日時：%s %s", 
        (char *)m_stTimeData.strMachDatePlus.c_str(), 
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);

    // ファイルコピー結果を返却
    return iCopyResult;
}

//********************************************
// 関数名：ファイル名変換処理
// Function Name：ChangeFileNameProc
// 
// 処理概要：
// ファイル名に対してファイル名＋日時形式に変換する
// 
// 処理手順：
//   １．ファイル名リネーム
// 
// 引数：     変換前ファイル名
//            変換後ファイル名
// 戻り値：   実行結果（正常終了はstdlib.hで定義している内容を利用）
//            正常終了：EXIT_SUCCESS（0）
//            異常終了：ファイル名変換エラー：CM_RENAMEERROR（111）
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
int CFileOperations::ChangeFileNameProc(
	const char *pcBaseFileName, 
	const char *pcFileCustomName)
{
    // 変数宣言
    int iFileConvResult = EXIT_SUCCESS;
    int iRenameResult = EXIT_SUCCESS;
    char cFileCustomName[FILENAMESIZE + 1];
    char cBaseFileName[FILENAMESIZE + 1];

    // 初期化
    memset(cFileCustomName, 0x00, sizeof(cFileCustomName));
    memset(cBaseFileName, 0x00, sizeof(cBaseFileName));

    // 前処理
    // 日時情報取得処理
    // ファイル名変換開始日時取得
    m_stTimeData = g_clsFileOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "ファイル名変換開始日時：%s %s", 
        (char *)m_stTimeData.strMachDatePlus.c_str(), 
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ファイル名変換処理開始 *****");

    // パラメータ情報をコピー
    strncpy_s(cBaseFileName, sizeof(cBaseFileName), pcBaseFileName, sizeof(cBaseFileName) - 1);
    sprintf_s(m_cLogData, sizeof(m_cLogData), "変換前ファイル名：%s", cBaseFileName);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
    strncpy_s(cFileCustomName, sizeof(cFileCustomName), pcFileCustomName, sizeof(cFileCustomName) - 1);
    sprintf_s(m_cLogData, sizeof(m_cLogData), "変換後ファイル名：%s", cFileCustomName);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    // 主処理
    // ファイル名をリネーム
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ファイル名変換開始 *****");
    iRenameResult = rename(cBaseFileName, cFileCustomName);
    sprintf_s(m_cLogData, sizeof(m_cLogData), "リネーム前ファイル名：%s", cBaseFileName);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
    sprintf_s(m_cLogData, sizeof(m_cLogData), "リネーム後ファイル名：%s", cFileCustomName);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
    sprintf_s(m_cLogData, sizeof(m_cLogData), "実行結果：%d", iRenameResult);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
    m_stLogData = CreateEventMessageProc(ACTIONSET, cFileCustomName);

    // リネーム出来たかチェック
    if (EXIT_SUCCESS != iRenameResult)
    {
        // リネーム失敗
        // エラーコードを設定
        LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** ファイル名変換異常終了 *****");
        iFileConvResult = CM_RENAMEERROR;
        sprintf_s(m_cLogData, sizeof(m_cLogData), "ファイル名は： %s", m_stLogData.pcActionName);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
    }
    else
    {
        // リネーム成功
        // リネーム成功のメッセージをログに出力
        LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, "***** ファイル名変換正常終了 *****");
        sprintf_s(m_cLogData, sizeof(m_cLogData), "ファイル名は： %s", m_stLogData.pcActionName);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
        sprintf_s(m_cLogData, sizeof(m_cLogData), "%s に%s", m_stLogData.pcActionName, RENAMESUCCESS);
        LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, m_cLogData);
    }

    // 後処理
    // 日時情報取得処理
    // ファイル名変換終了日時取得
    m_stTimeData = g_clsFileOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "ファイル名変換終了日時：%s %s", 
        (char *)m_stTimeData.strMachDatePlus.c_str(), 
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);

    // 実行結果を返却
    return iFileConvResult;
}

//********************************************
// 関数名：ファイル削除処理
// Function Name：DeleteFileProc
// 
// 処理概要：
// ファイル名に基づいて
// 指定したファイルを物理削除する
// 
// 処理手順：
//   １．ファイル削除
// 
// 引数：      削除対象ファイル名
// 戻り値：    実行結果（正常終了はstdlib.hで定義している内容を利用）
//             正常終了：EXIT_SUCCESS（0）
//             異常終了：ファイル削除エラー：CM_FILEDELETEERROR（112）
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
int CFileOperations::DeleteFileProc(
	const char *pcDeleteFile)
{
    // 変数宣言
    int iFileDeleteResult = 0;
    int iDeleteResult = EXIT_SUCCESS;
    char cDeleteFileName[FILENAMESIZE + 1];

    // 初期化
    memset(cDeleteFileName, 0x00, sizeof(cDeleteFileName));

    // 前処理
    // 日時情報取得処理
    // ファイル削除開始日時取得
    m_stTimeData = g_clsFileOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "ファイル削除開始日時：%s %s", 
        (char *)m_stTimeData.strMachDatePlus.c_str(), 
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ファイル削除処理開始 *****");

    // パラメータ情報をコピー
    strncpy_s(cDeleteFileName, sizeof(cDeleteFileName), pcDeleteFile, sizeof(cDeleteFileName) - 1);
    sprintf_s(m_cLogData, sizeof(m_cLogData), "削除ファイル名：%s", cDeleteFileName);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    // 主処理
    // ファイルを削除
    iFileDeleteResult = remove(cDeleteFileName);
    m_stLogData = CreateEventMessageProc(ACTIONSET, cDeleteFileName);
    sprintf_s(m_cLogData, sizeof(m_cLogData), "ファイル削除結果：%d", iFileDeleteResult);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    // 削除出来たかチェック
    if (EXIT_SUCCESS == iFileDeleteResult)
    {
        // 削除成功
        // メッセージをログに出力
        LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, "***** ファイル削除処理正常終了 *****");
        sprintf_s(m_cLogData, sizeof(m_cLogData), "ファイル名は：%s", m_stLogData.pcActionName);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
        sprintf_s(m_cLogData, sizeof(m_cLogData), "%s の%s", m_stLogData.pcActionName, DELETESUCCESS);
        LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, m_cLogData);
    }
    else
    {
        // 削除失敗
        // エラーコードを設定
        LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** ファイル削除処理異常終了 *****");
        iDeleteResult = CM_FILEDELETEERROR;
        sprintf_s(m_cLogData, sizeof(m_cLogData), "ファイル名は：%s", m_stLogData.pcActionName);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
    }

    // 後処理
    // 日時情報取得処理
    // ファイル削除終了日時取得
    m_stTimeData = g_clsFileOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "ファイル削除終了日時：%s %s", 
        (char *)m_stTimeData.strMachDatePlus.c_str(), 
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);

    // ファイル削除結果を返却
    return iDeleteResult;
}

//********************************************
// 関数名：ファイル状態チェック処理
// Function Name：CheckFileStatProc
// 
// 処理概要：
// 指定されたファイルに対して状態をチェックする
// 
// 処理手順：
//   １．パラメータ情報コピー
//   ２．ファイル状態取得
// 
// 引数：      状態チェック対象ファイル名
//             チェックSTAT構造体
// 戻り値：    実行結果（正常終了はstdlib.hで定義している内容を利用）
//             正常終了：EXIT_SUCCESS（0）
//             異常終了：ファイル状態取得エラー：STATERROR（113）
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
int CFileOperations::CheckFileStatProc(
	const char *pcCheckFileName, 
	struct stat *pstStatResult)
{
    // 変数宣言
    int iStatResult = EXIT_SUCCESS;
    int iStatCheckResult = EXIT_SUCCESS;
    char cCheckFileName[FILENAMESIZE + 1];

    // 初期化
    memset(cCheckFileName, 0x00, sizeof(cCheckFileName));

    // 前処理
    // 日時情報取得処理
    // ファイル状態チェック開始日時取得
    m_stTimeData = g_clsFileOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "ファイル状態チェック開始日時：%s %s", 
        (char *)m_stTimeData.strMachDatePlus.c_str(), 
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ファイル状態チェック処理開始 *****");

    // パラメータ情報をコピー
    strncpy_s(cCheckFileName, sizeof(cCheckFileName), pcCheckFileName, sizeof(cCheckFileName) - 1);
    sprintf_s(m_cLogData, sizeof(m_cLogData), "状態チェックファイル名：%s", cCheckFileName);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    // 主処理
    // ファイル状態を取得（stat関数使用）
    iStatCheckResult = stat(cCheckFileName, pstStatResult);
    sprintf_s(m_cLogData, sizeof(m_cLogData), "ファイル状態取得結果：%d", iStatCheckResult);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    // 後処理
    // 取得結果をチェック
    if (0 != iStatCheckResult)
    {
        // ファイル状態取得失敗
        // エラーコードを設定
        LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** ファイル状態チェック処理異常終了 *****");
        iStatResult = CM_FILESTATERROR;
    }
    else
    {
        // ファイル状態取得成功
        // ファイル状態取得成功のメッセージをログに出力
        m_stLogData = CreateEventMessageProc(ACTIONSET, cCheckFileName);
        LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, "***** ファイル状態チェック処理正常終了 *****");
        sprintf_s(m_cLogData, sizeof(m_cLogData), "%s %s", m_stLogData.pcActionName, GETSTATSUCCESS);
        LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, m_cLogData);
    }

    // 日時情報取得処理
    // ファイル状態チェック終了日時取得
    m_stTimeData = g_clsFileOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "ファイル状態チェック終了日時：%s %s", 
        (char *)m_stTimeData.strMachDatePlus.c_str(), 
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);

    // ファイル状態チェック結果を返却
    return iStatResult;
}

//********************************************
// 関数名：絶対パス変換ファイル名取得処理
// Function Name：GetConvertFileNameProc
// 
// 処理概要：
// 以下のパラメータ情報に基づいて
// 絶対パス付きのファイル名をファイル名情報管理構造体にて取得する
//      ・変換対象ファイルタイプ
//      ・変換対象ファイル名
//      ・呼び出し元実行結果（コマンド情報）
// 
// 処理手順：
//   １．ベースとするアプリケーションの絶対パス生成
//   ２．パス名編集メモリ領域解放
//   ３．変換対象ファイル名タイプに基づいて絶対パス生成
//   ４．パス名編集メモリ領域解放
//   ５．格納ディレクトリ生成
//   ６．絶対パス付与による絶対パス付きファイル名生成
//   ７．ファイル名編集メモリ領域解放
//   ８．絶対パス付きのファイル名格納
// 
// 引数：     変換対象ファイル名タイプ
//            変換対象ファイル名
//            コマンド情報（実行アプリケーション名）
//            パス組み込みディレクトリ名
//            デフォルトファイル名
//            呼び出し元実行結果（設定がない場合は0）
// 
// 戻り値：   絶対パス付きファイル名
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
string CFileOperations::GetConvertFileNameProc(
    FILETYPETABLE eConvertKind,
    const char *pcDesignationName,
    const char *pcCmdFileName,
    char *pcDirectoryName, 
    char *pcDefaultName, 
    int iBaseResult)
{
    // 変数宣言
    string strConvertFileName;
    char cEditPath[PATH_SIZE + 1];
    int iDirMakeResult = 0;
    string strFileCustomName;
    char* pcSearch = NULL;
    static int iConvertCnt = 0;
    string strConvertPathName;
    string strCmdFileName;
    deque<string> deqCmdFileName;
    string strDesignationFileName;
    string strDriveName;
    string strPathName;
    string strAppPathName;
    string strDirectoryName;
    string strDefaultName;

    // 初期化
    memset(cEditPath, 0x00, sizeof(cEditPath));

    // 呼び出し回数をチェック
    if (2 == iConvertCnt)
    {
        // 次が3回目の場合
        // トレースログ及びデータログで呼び出した旨をログに出力
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "最初の2回はトレースログ及びデータログで呼び出し済み");
    }

    // 呼び出し回数をカウント
    iConvertCnt++;
    sprintf_s(m_cLogData, sizeof(m_cLogData), "絶対パス変換ファイル名取得呼び出し回数： %d 回", iConvertCnt);
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);

    // 前処理
    // 日時情報取得処理
    // パス変換開始日時取得
    m_stTimeData = g_clsFileOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData,
        sizeof(m_cLogData),
        "絶対パス変換ファイル名取得開始日時：%s %s",
        (char *)m_stTimeData.strMachDatePlus.c_str(),
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 絶対パス変換ファイル名取得処理開始 *****");

    // パラメータ情報をコピー
    strDesignationFileName = pcDesignationName;
    sprintf_s(m_cLogData, sizeof(m_cLogData), "変換指定文字列は：%s", strDesignationFileName.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
    strCmdFileName = pcCmdFileName;
    sprintf_s(m_cLogData, sizeof(m_cLogData), "コマンド情報は：%s", strCmdFileName.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
    strDirectoryName = pcDirectoryName;
    sprintf_s(m_cLogData, sizeof(m_cLogData), "パス組み込みディレクトリ名は：%s", strDirectoryName.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
    strDefaultName = pcDefaultName;
    sprintf_s(m_cLogData, sizeof(m_cLogData), "デフォルトファイル名は：%s", strDefaultName.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    // コマンド情報を分割してドライブ名を取得
    deqCmdFileName = SplitDataProc(strCmdFileName, '\\');
    strDriveName = deqCmdFileName.front();
    sprintf_s(m_cLogData, sizeof(m_cLogData), "ドライブ名は：%s", strDriveName.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    // コマンド情報を分割してパス名を取得
    // 分割した数分ループ
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, "***** 分割されているディレクトリ名 *****");
    for (size_t siCnt = 1; siCnt < deqCmdFileName.size(); siCnt++)
    {
        // 分割したパス情報を再度組み合わせ
        strPathName += '\\';
        strPathName += deqCmdFileName.at(siCnt);
        sprintf_s(m_cLogData, sizeof(m_cLogData), "%zu つ目のディレクトリ名：%s", siCnt, deqCmdFileName.at(siCnt).c_str());
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

        // 最後がファイル名かチェック
        if (deqCmdFileName.at(siCnt) == deqCmdFileName.back())
        {
            // 最後がファイル名
            // stringクラスのreplace関数を使用して最後のファイル名を空文字列に置換
            // 定義：stringクラス.replace(開始位置, 長さ, 文字列);
            strPathName.replace(strPathName.size() - deqCmdFileName.back().size(), deqCmdFileName.back().size(), "");
        }
    }

    // 生成後のパス名をログに出力
    sprintf_s(m_cLogData, sizeof(m_cLogData), "生成後のパス名は：%s", strPathName.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    // 絶対パス取得処理
    // 当関数で使用するベース絶対パスを生成
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 絶対パス取得関数呼び出し *****");
    strConvertPathName = GetFullPathProc(strDriveName.at(0), "..", strPathName.c_str());
    strAppPathName = strDriveName + strConvertPathName;
    sprintf_s(m_cLogData, sizeof(m_cLogData), "ベースとする絶対パス名：%s", strAppPathName.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    // 主処理
    // 変換対象ファイル名タイプをログに出力
    sprintf_s(m_cLogData, sizeof(m_cLogData), "変換対象ファイル名タイプは：%d", eConvertKind);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    // キーワードディレクトリ名が設定されているかチェック
    if ("" != strDirectoryName)
    {
        // キーワードディレクトリ名が設定されている
        // 変換対象ファイル名タイプに基づいてディレクトリ名をパス形式に編集
        switch (eConvertKind)
        {
            // ディレクトリ名指定あり且つファイル名変更なし（実行フォルダと同階層）
            case tagFileType::FILETYPE1:

            // ディレクトリ名指定あり且つファイル名変更あり（実行フォルダと同階層）
            case tagFileType::FILETYPE2:
            {
                // ディレクトリ名をパス形式に編集
                sprintf_s(cEditPath, sizeof(cEditPath), ".\\%s", strDirectoryName.c_str());

                // デフォルトのファイル名を指定しているかチェック
                if ("" != strDefaultName && strDefaultName == strDesignationFileName)
                {
                    // デフォルトのファイル名を指定している
                    // ファイル名生成
                    // 指定ファイル名を基にファイル名を生成
                    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ファイル名生成関数呼び出し *****");
                    strFileCustomName = CreateFileNameProc(CREATE_DATE_NAME, strDesignationFileName.c_str(), DELIMITER);
                    strDesignationFileName = strFileCustomName;
                    sprintf_s(
                        m_cLogData,
                        sizeof(m_cLogData),
                        "カスタマイズ後のファイル名は：%s",
                        strDesignationFileName.c_str());
                    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
                }

                // 分岐抜け
                break;
            }

            // ディレクトリ名指定あり且つファイル名変更あり（実行フォルダと同階層）
            case tagFileType::FILETYPE9:
            {
                // ディレクトリ名をパス形式に編集
                sprintf_s(cEditPath, sizeof(cEditPath), ".\\%s", strDirectoryName.c_str());

                // ファイル名生成
                // デフォルトを基にファイル名を生成
                LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ファイル名生成関数呼び出し *****");
                strFileCustomName = CreateFileNameProc(CREATE_DATE_NAME, strDefaultName.c_str(), DELIMITER);
                strDesignationFileName = strFileCustomName;
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "カスタマイズ後のファイル名は：%s",
                    strDesignationFileName.c_str());
                LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

                // 分岐抜け
                break;
            }

            // ディレクトリ名指定あり且つファイル名変更なし（実行フォルダの１階層上）
            case tagFileType::FILETYPE5:

            // ディレクトリ名指定あり且つファイル名変更あり（実行フォルダの１階層上）
            default:
            {
                // キーワードディレクトリ名をパス形式に編集
                sprintf_s(cEditPath, sizeof(cEditPath), "..\\%s", strDirectoryName.c_str());

                // デフォルトのファイル名を設定しているかチェック
                if ("" != strDefaultName)
                {
                    // デフォルトのファイル名を設定している
                    // ファイル名生成
                    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ファイル名生成関数呼び出し *****");
                    strFileCustomName = CreateFileNameProc(CREATE_DATE_NAME, strDefaultName.c_str(), DELIMITER);
                    strDesignationFileName = strFileCustomName;
                    sprintf_s(
                        m_cLogData,
                        sizeof(m_cLogData),
                        "カスタマイズ後のファイル名は：%s",
                        strDesignationFileName.c_str());
                    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
                }

                // 分岐抜け
                break;
            }
        }

        // 絶対パス取得処理
        // ディレクトリ名に絶対パスを付与
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 絶対パス取得関数呼び出し *****");
        strConvertPathName = GetFullPathProc(strDriveName.at(0), cEditPath, strAppPathName.c_str());
        sprintf_s(m_cLogData, sizeof(m_cLogData), "パス状態は：%s", strConvertPathName.c_str());
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
    }

    // キーワードディレクトリ名があり且つ変換対象ファイル名タイプが書き込みもしくはトレースログもしくはコピーかチェック
    if ("" != strDirectoryName && tagFileType::FILETYPE1 != eConvertKind)
    {
        // ディレクトリ名指定あり且つファイル名変更なし（実行フォルダと同階層）指定以外
        // 格納ディレクトリを生成
        // 既に格納ディレクトリが存在する場合は
        // 生成に失敗するが引き続きそのディレクトリを使用するため処理を継続する
        // ただし、生成結果に対するメッセージはログに出力する
        iDirMakeResult = _mkdir(strConvertPathName.c_str());
        sprintf_s(m_cLogData, sizeof(m_cLogData), "生成結果は：%d", iDirMakeResult);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

        // 生成結果をチェック
        if (0 != iDirMakeResult)
        {
            // 既に格納ディレクトリが存在する
            // 存在する旨のメッセージをログに出力
            sprintf_s(m_cLogData, sizeof(m_cLogData), "%s %s", strConvertPathName.c_str(), ALREADYEXISTS);
            LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, m_cLogData);
        }
        else
        {
            // 格納ディレクトリ生成成功
            // 格納ディレクトリ生成成功のメッセージをログに出力
            sprintf_s(m_cLogData, sizeof(m_cLogData), "%s %s", strConvertPathName.c_str(), MAKEDIRSUCCESS);
            LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, m_cLogData);
        }
    }

    // 後処理
    // キーワードディレクトリ名が設定されているかチェック
    if ("" != strDirectoryName)
    {
        // キーワードディレクトリ名が設定されている
        // ファイルパスにてキーワードディレクトリを検索
        pcSearch = strstr((char*)strDesignationFileName.c_str(), strDirectoryName.c_str());
        sprintf_s(m_cLogData, sizeof(m_cLogData), "検索結果：%s", pcSearch);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

        // ファイルパスに対しキーワードディレクトリを指定しているかチェック
        if (NULL == pcSearch)
        {
            // キーワードディレクトリ指定なし
            // 絶対パス取得処理
            // 編集した絶対パスを付与
            LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 絶対パス取得関数呼び出し *****");
            strConvertFileName = GetFullPathProc(
                strDriveName.at(0),
                strDesignationFileName.c_str(),
                strConvertPathName.c_str());
            sprintf_s(m_cLogData, sizeof(m_cLogData), "絶対パス付きファイル名は：%s", strConvertFileName.c_str());
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
        }
        else
        {
            // キーワードディレクトリ指定あり
            // 絶対パス取得処理
            // ベース絶対パスを付与
            LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 絶対パス取得関数呼び出し *****");
            strConvertFileName = GetFullPathProc(strDriveName.at(0), strDesignationFileName.c_str(), strAppPathName.c_str());
            sprintf_s(m_cLogData, sizeof(m_cLogData), "絶対パス付きファイル名は：%s", strConvertFileName.c_str());
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
        }
    }
    else
    {
        // キーワードディレクトリ名が設定されていない
        // 変換対象ファイル名タイプに基づいて絶対パス付きファイル名を生成
        sprintf_s(m_cLogData, sizeof(m_cLogData), "変換対象ファイル名タイプは：%d", eConvertKind);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
        switch (eConvertKind)
        {
            // ディレクトリ名指定なし且つファイル名変更あり
            case tagFileType::FILETYPE6:
            {
                // 変換ファイル名生成
                LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ファイル名生成関数呼び出し *****");
                strFileCustomName = CreateFileNameProc(CREATE_TIME_NAME, strDesignationFileName.c_str(), DELIMITER, iBaseResult);
                sprintf_s(m_cLogData, sizeof(m_cLogData), "カスタマイズ後のファイル名は：%s", strFileCustomName.c_str());
                LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

                // 絶対パス取得処理
                // ベース絶対パスを付与
                LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 絶対パス取得関数呼び出し *****");
                strDesignationFileName = strFileCustomName;
                strConvertFileName = GetFullPathProc(strDriveName.at(0), strDesignationFileName.c_str(), strAppPathName.c_str());
                sprintf_s(m_cLogData, sizeof(m_cLogData), "絶対パス付きファイル名は：%s", strConvertFileName.c_str());
                LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

                // 分岐抜け
                break;
            }

            // ディレクトリ名指定なし且つファイル名変更なし（ベース絶対パス設定）
            case tagFileType::FILETYPE7:
            {
                // 絶対パス取得処理
                // ベース絶対パスを付与
                LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 絶対パス取得関数呼び出し *****");
                strConvertFileName = GetFullPathProc(strDriveName.at(0), strDesignationFileName.c_str(), strAppPathName.c_str());
                sprintf_s(m_cLogData, sizeof(m_cLogData), "絶対パス付きファイル名は：%s", strConvertFileName.c_str());
                LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

                // 分岐抜け
                break;
            }

            // ディレクトリ名指定なし且つファイル名変更なし（コマンド情報設定）
            default:
            {
                // 絶対パス取得処理
                // コマンド情報から絶対パスを付与
                LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 絶対パス取得関数呼び出し *****");
                strConvertFileName = GetFullPathProc(strDriveName.at(0), strDesignationFileName.c_str(), strCmdFileName.c_str());
                sprintf_s(m_cLogData, sizeof(m_cLogData), "絶対パス付きファイル名は：%s", strConvertFileName.c_str());
                LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

                // 分岐抜け
                break;
            }
        }
    }

    // 日時情報取得処理
    // 絶対パス変換ファイル名取得終了日時取得
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 絶対パス変換ファイル名取得処理終了 *****");
    m_stTimeData = g_clsFileOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData,
        sizeof(m_cLogData),
        "絶対パス変換ファイル名取得終了日時：%s %s",
        (char *)m_stTimeData.strMachDatePlus.c_str(),
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);

    // 絶対パス付与のファイル名を返却
    return strConvertFileName;
}

//********************************************
// 関数名：ファイル名生成処理
// Function Name：CreateFileNameProc
// 
// 処理概要：
// ファイル生成タイプ及び呼び出し元実行結果に基づいて
// 用途に応じたファイル名を合成する
// 
// 処理手順：
//   １．ファイル名分割
//   ２．ファイル名合成
// 
// 引数：     ファイル生成タイプ
//            生成前ファイル名
//            区切り文字
//            呼び出し元実行結果（設定がない場合は0）
// 戻り値：   生成後ファイル名
// 
// 補足事項
// ・日時形式の場合呼び出し元実行結果に基づいて
//     ファイル名を以下の形式で生成する
//     正常終了：ファイル名＋拡張子．日付＋時刻
//     異常終了：ファイル名＋拡張子．99999999999999
// 
// 
// 作成履歴
// 2020/9/25  s-saito   新規作成
// 
// 更新履歴
// 
// 
//********************************************
string CFileOperations::CreateFileNameProc(
	int iKind, 
	const char *pcFileName, 
	const char *pcDelimiter, 
	int iResult)
{
    // 変数宣言
    string strCustomFileName;
    string strFileNameMain;
    string strFileExtension;
    char cEditFileName[FILENAMESIZE + 1];
    char* pcEditFileName = NULL;
    char* pcCtx = NULL;

    // 初期化
    memset(cEditFileName, 0x00, sizeof(cEditFileName));

    // 前処理
    // 日時情報取得処理
    // ファイル名生成開始日時取得
    m_stTimeData = g_clsFileOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "ファイル名生成開始日時：%s %s", 
        (char *)m_stTimeData.strMachDatePlus.c_str(), 
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ファイル名生成処理開始 *****");

    // パラメータ情報をコピー
    strncpy_s(cEditFileName, sizeof(cEditFileName), pcFileName, sizeof(cEditFileName) - 1);
    sprintf_s(m_cLogData, sizeof(m_cLogData), "ファイル名は：%s", cEditFileName);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    // 主処理
    // strtok_s関数でファイル名分割
    // ファイル名
    pcEditFileName = strtok_s(cEditFileName, pcDelimiter, &pcCtx);
    strFileNameMain = pcEditFileName;
    sprintf_s(m_cLogData, sizeof(m_cLogData), "先頭のファイル名は：%s", strFileNameMain.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    // 拡張子
    while (NULL != pcEditFileName)
    {
        // strtok_s関数により変更されたNULLのポインタが先頭
        pcEditFileName = strtok_s(NULL, pcDelimiter, &pcCtx);

        // pcFileNameがNULLかチェック
        if (NULL != pcEditFileName)
        {
            // NULLでなければ文字列を格納
            strFileExtension += ".";
            strFileExtension += pcEditFileName;
            sprintf_s(m_cLogData, sizeof(m_cLogData), "拡張子は：%s", strFileExtension.c_str());
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
        }
    }

    // 後処理
    // ファイル生成タイプをログに出力
    sprintf_s(m_cLogData, sizeof(m_cLogData), "ファイル生成タイプは：%d", iKind);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    // ファイル名の形式を判定
    switch (iKind)
    {
        case 1:
        {
            // 日付のみ
            // 日付との組み合わせで連結
            strCustomFileName = strFileNameMain + "_" + m_stTimeData.strMachDate + strFileExtension;
            break;
        }
        case 2:
        {
            // 時刻込み
            // 呼び出し元で指定された実行結果をチェック
            if (EXIT_SUCCESS == iResult)
            {
                // 実行結果は正常
                // 日付と時刻との組み合わせで連結
                strCustomFileName = strFileNameMain + strFileExtension + "." + m_stTimeData.strMachDate + m_stTimeData.strMachTime;
            }
            else
            {
                // 実行結果は正常ではない
                // ダミーデータを付与
                strCustomFileName = strFileNameMain + strFileExtension + "." + "99999999" + "999999";
            }

            // 処理抜け
            break;
        }
    }

    // 生成後のファイル名を出力
    sprintf_s(m_cLogData, sizeof(m_cLogData), "生成後のファイル名は：%s", strCustomFileName.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    // 日時情報取得処理
    // ファイル名生成終了日時取得
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ファイル名生成処理終了 *****");
    m_stTimeData = g_clsFileOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "ファイル名生成終了日時：%s %s", 
        (char *)m_stTimeData.strMachDatePlus.c_str(), 
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);

    // 生成後のファイル名を返却
    return strCustomFileName;
}

//********************************************
// 関数名：絶対パス取得処理
// Function Name：GetFullPathProc
// 
// 処理概要：
// 指定されたパス名及びファイル名に基づいて
// 絶対パス付きのパス名及びファイル名を取得する
// 
// 処理手順：
//   １．メモリ領域確保
//   ２．パラメータチェック
// （絶対パスもしくはカレントパスかチェック）
// （以降は相対パスの場合のみ）
//   ３．相対パスから絶対パスへの変換
//   ４．絶対パス変換後の結果を格納
//   ５．メモリ領域解放
// 
// 引数：     ドライブ名（先頭文字）
//            変換前ファイルパス
//            ベース絶対パス
// 戻り値：   変換後ファイルパス
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
string CFileOperations::GetFullPathProc(
	const char cDriveNameTop, 
	const char *pcDesignationPath, 
	const char *pcBasePath)
{
    // 変数宣言
    string strResultPath;
    char *pcResultPath = NULL;
    const char *pcPathPoint = NULL;
    const char *pcBasePoint = NULL;
    char *pcResultPoint = NULL;
    size_t siLength = 0;

    // 標準出力に日本語が表示されるようにロケールを日本に設定
    setlocale(LC_ALL, "japanese");

    // メモリ領域確保に失敗した場合のNULL設定
    // この設定により確保に失敗した場合はNULLを返すようにする
    set_new_handler(NULL);

    // 前処理
    // 日時情報取得処理
    // 絶対パス設定開始日時取得
    m_stTimeData = g_clsFileOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "絶対パス設定開始日時：%s %s", 
        (char *)m_stTimeData.strMachDatePlus.c_str(), 
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 絶対パス取得処理開始 *****");

    // パラメータ情報を出力
    sprintf_s(m_cLogData, sizeof(m_cLogData), "変換前ファイルパスは：%s", pcDesignationPath);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
    sprintf_s(m_cLogData, sizeof(m_cLogData), "ベースの絶対パスは：%s", pcBasePath);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    try
    {
        // パス名・ファイル名編集メモリ領域の確保
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** パス名・ファイル名編集メモリ領域確保 *****");
        pcResultPath = DBG_NEW char[FILENAMESIZE + 1];
        m_stLogData = CreateEventMessageProc(ACTIONSET, "パス名・ファイル名編集の");

        // メモリ領域が確保出来たかチェック
        if (NULL == pcResultPath)
        {
            // メモリ領域の確保失敗
            // 例外処理へスロー
            throw tagErrorCode::MEMORYERROR;
        }

        // メモリの初期化
        memset(pcResultPath, 0x00, FILENAMESIZE + 1);

        // メモリ領域確保成功のメッセージをログに出力
        LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, "***** パス名・ファイル名編集メモリ領域確保正常終了 *****");
        sprintf_s(m_cLogData, sizeof(m_cLogData), "%sの%s", m_stLogData.pcActionName, MEMORYCREATESUCCESS);
        LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, m_cLogData);

        // 変換前のパスが絶対パス表記かチェック
        // Windows版では絶対パスはドライブ名から始まるためドライブ名で判別する
        if (cDriveNameTop == *pcDesignationPath)
        {
            // 変換前のパスが絶対パス表記
            // 変換前のパスをそのままコピーして後処理へ移動
            strncpy_s(pcResultPath, FILENAMESIZE + 1, pcDesignationPath, FILENAMESIZE);
            pcResultPath[strlen(pcResultPath)] = '\0';
            strResultPath = pcResultPath;
            LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 絶対パス指定 *****");
            sprintf_s(m_cLogData, sizeof(m_cLogData), "変換後の結果は：%s", strResultPath.c_str());
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
            LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, FULLPATHDEFINITION);
            goto END;
        }
        else
        {
            // 変換前のパスが絶対パス表記ではない
            // パス名をログに出力
            sprintf_s(m_cLogData, sizeof(m_cLogData), "指定のパス名は：%s", pcDesignationPath);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
        }

        // 変換前のパスがカレントパス表記かチェック
        if (1 == !strncmp(pcDesignationPath, ".", sizeof(pcDesignationPath)) ||
            1 == !strncmp(pcDesignationPath, ".\\", sizeof(pcDesignationPath)))
        {
            // 変換前のパスがカレントパス表記
            // ベース絶対パスをコピーして後処理へ移動
            strncpy_s(pcResultPath, FILENAMESIZE + 1, pcBasePath, FILENAMESIZE);
            pcResultPath[strlen(pcResultPath)] = '\0';
            strResultPath = pcResultPath;
            LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 『.』もしくは『.\\』によるカレントパス指定 *****");
            sprintf_s(m_cLogData, sizeof(m_cLogData), "変換後の結果は：%s", strResultPath.c_str());
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
            goto END;
        }
        else
        {
            // 変換前のパスがカレントパス表記ではない
            // パス名をログに出力
            sprintf_s(m_cLogData, sizeof(m_cLogData), "指定のパス名は：%s", pcDesignationPath);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
        }

        // 主処理
        // 集合地点を取得
        // ベース絶対パスの長さを取得
        siLength = strlen(pcBasePath);
        sprintf_s(m_cLogData, sizeof(m_cLogData), "長さは：%zu", siLength);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

        // ベース絶対パスの終端位置を取得
        // pcBasePoint はベース絶対パスの終端を表す
        pcBasePoint = pcBasePath + siLength;
        sprintf_s(m_cLogData, sizeof(m_cLogData), "終端位置は：%s", pcBasePoint);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

        // 終端位置をチェック
        if ('\\' == *(pcBasePoint - 1))
        {
            // 終端位置の1つ前の値が『\\』
            // ポインタの位置をデクリメント
            --pcBasePoint;
            sprintf_s(m_cLogData, sizeof(m_cLogData), "集合地点は：%s", pcBasePoint);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
        }

        // 変換前ファイルパスの先頭部分にある『.\\』・『..』・『..\\』を解析する事により
        // ベース絶対パスのパス表記に対し、何処まで変換後ファイルパスと共通になるかを調べる
        for (pcPathPoint = pcDesignationPath; *pcPathPoint != '\0' && *pcPathPoint == '.';)
        {
            // 先頭位置をログに出力
            sprintf_s(m_cLogData, sizeof(m_cLogData), "先頭位置は：%s", pcPathPoint);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

            // pcPathPoint の位置をチェック
            if (1 == !strncmp(pcPathPoint, "..\\", PATHCHECK_SIZE_3BYTE))
            {
                // pcPathPoint が『..\\』
                // ポインタの位置を移動
                pcPathPoint += 3;
                while (pcBasePoint > pcBasePath && *--pcBasePoint != '\\');
                LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, (char*)"***** 『..\\』の条件で処理 *****");
                sprintf_s(m_cLogData, sizeof(m_cLogData), "pcPathPoint 位置は：%s", pcPathPoint);
                LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
                sprintf_s(m_cLogData, sizeof(m_cLogData), "pcBasePoint 終端位置は：%s", pcBasePoint);
                LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
            }
            else if (1 == !strncmp(pcPathPoint, ".\\", PATHCHECK_SIZE_2BYTE))
            {
                // pcPathPoint が『.\\』
                // ポインタの位置を移動
                pcPathPoint += 2;
                LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, (char*)"***** 『..\\』の条件で処理 *****");
                sprintf_s(m_cLogData, sizeof(m_cLogData), "pcPathPoint 位置は：%s", pcPathPoint);
                LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
            }
            else if (1 == !strncmp(pcPathPoint, "..\0", PATHCHECK_SIZE_3BYTE))
            {
                // pcPathPoint が『..』
                // ポインタの位置を移動
                pcPathPoint += 2;
                while (pcBasePoint > pcBasePath && *--pcBasePoint != '\\');
                LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, (char*)"***** 『..』の条件で処理 *****");
                sprintf_s(m_cLogData, sizeof(m_cLogData), "pcPathPoint 位置は：%s", pcPathPoint);
                LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
                sprintf_s(m_cLogData, sizeof(m_cLogData), "pcBasePoint 終端位置は：%s", pcBasePoint);
                LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
            }
            else
            {
                // pcPathPoint が上記以外
                // pcPathPoint 位置をログに出力
                sprintf_s(m_cLogData, sizeof(m_cLogData), "pcPathPoint 位置は：%s", pcPathPoint);
                LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

                // ループ終了
                break;
            }
        }

        // ベース絶対パスと共通部分を変換後ファイルパスへコピー
        siLength = pcBasePoint - pcBasePath;
        strncpy_s(pcResultPath, siLength + 1, pcBasePath, siLength);
        sprintf_s(m_cLogData, sizeof(m_cLogData), "長さは：%zu", siLength);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
        sprintf_s(m_cLogData, sizeof(m_cLogData), "状態は：%s", pcResultPath);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

        // 変換後ファイルパスポインタの位置を取得
        pcResultPoint = pcResultPath + siLength;
        *pcResultPoint++ = '\\';
        sprintf_s(m_cLogData, sizeof(m_cLogData), "ポインタは：%s", pcResultPoint);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

        // 変換前ファイルパス文字列のうち、先頭部分の『.\\』や『..\\』を除いた残りの部分
        // （pcPathPoint が指す文字列）を 変換後ファイルパス文字列に追加
        strcpy(pcResultPoint, pcPathPoint);
        pcResultPath[strlen(pcResultPath)] = '\0';
        strResultPath = pcResultPath;
        sprintf_s(m_cLogData, sizeof(m_cLogData), "変換後の結果は：%s", strResultPath.c_str());
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
    }

    // 例外処理
    catch (const ERRORCODETABLE &eAllocErr)
    {
        // エラーメッセージ生成処理
        // エラーメッセージを出力
        pcResultPath = NULL;
        LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** パス名・ファイル名編集メモリ領域確保異常終了 *****");
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** エラーメッセージ生成関数呼び出し *****");
        m_eErrorResultCode = eAllocErr;
        m_stLogData = CreateEventMessageProc(ERRORSET, m_stLogData.pcActionName, m_eErrorResultCode);
        LOGOUTPUTERROR(TRACEOUTPUTLOG, ERRORINFORMATION1, m_stLogData.pcMessage, m_stLogData.eErrorCode);
        sprintf_s(m_cLogData, sizeof(m_cLogData), "戻り値は：%d", m_stLogData.eErrorCode);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
    }

END:
    // 後処理
    // 絶対パス付きファイル名設定で使用したファイル名編集メモリ領域を解放
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ファイル名編集メモリ領域解放 *****");
    SAFE_FREE_NEWALLOC_ARRAY(pcResultPath);
    pcResultPoint = NULL;

    // メモリ領域解放のメッセージをログに出力
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ファイル名編集メモリ領域解放終了 *****");
    sprintf_s(m_cLogData, sizeof(m_cLogData), "%s%s", m_stLogData.pcActionName, MEMORYRELEASESUCCESS);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    // 日時情報取得処理
    // 絶対パス設定終了日時取得
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 絶対パス取得処理終了 *****");
    m_stTimeData = g_clsFileOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "絶対パス設定終了日時：%s %s", 
        (char *)m_stTimeData.strMachDatePlus.c_str(), 
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);

    // 変換後ファイルパスを返却
    return strResultPath;
}
