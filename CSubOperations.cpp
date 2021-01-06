//============================================
// 共通操作クラスソースファイル
// CSubOperations.cpp
// 
// 機能概要
// 他のメイン操作等をサポートする機能を定義する
// 
// 機能内容
// １．マルチバイト文字列ワイド文字列変換処理
// ２．ワイド文字列マルチバイト文字列変換処理
// ３．コマンドライン引数解析処理
// ４．環境変数取得処理
// ５．マシン日時取得処理
// ６．イベントメッセージ生成処理
// ７．指定文字削除処理
// ８．指定文字列分割処理
// ９．エラーメッセージ取得処理
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
#include "CSubOperations.h"
#include "CLogFile.h"

// グローバル変数宣言
CLogFile g_clsSubOpeLogFile;

// マクロ定義
#define LOGOUTPUT(iOutputType, iTraceOutputLogType, cMessageInfo) \
    g_clsSubOpeLogFile.LogOutputControlProc(LOGARGS, iOutputType, iTraceOutputLogType, cMessageInfo)
#define LOGOUTPUTERROR(iOutputType, iTraceOutputLogType, cErrorMessageInfo, eErrorInfoCode) \
    g_clsSubOpeLogFile.LogOutputControlProc(LOGARGS, iOutputType, iTraceOutputLogType, cErrorMessageInfo, eErrorInfoCode)

//☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★
// 各機能（API）定義
//☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★
//********************************************
// 関数名：デフォルトコンストラクタ
// Function Name：CSubOperations
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
CSubOperations::CSubOperations()
{
    // 現在デフォルトコンストラクタでの処理はなし
}

//********************************************
// 関数名：パラメータ情報付きコンストラクタ
// Function Name：CSubOperations
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
CSubOperations::CSubOperations(
    const string strTraceLogFileName, 
    const string strDataLogFileName)
{
    // ログ出力クラス生成
    g_clsSubOpeLogFile = CLogFile(strTraceLogFileName, strDataLogFileName);

    // 呼び出し元で指定されたログ出力ファイル名設定
    m_strSubOpeTraceLogFileName = strTraceLogFileName;
    m_strSubOpeDataLogFileName = strDataLogFileName;
}

//********************************************
// 関数名：デストラクタ
// Function Name：~CSubOperations
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
CSubOperations::~CSubOperations()
{
    // ログ出力ファイル名変数クリア
    m_strSubOpeTraceLogFileName.clear();
    m_strSubOpeDataLogFileName.clear();
}

//********************************************
// 関数名：環境変数取得処理
// Function Name：GetEnvironmentDataProc
// 
// 処理概要：
// マシンの『システムの環境変数』に設定した情報を取得する
// 
// 処理手順：
//   １．環境変数取得
// 
// 引数：      環境変数名
//             取得した環境変数情報
// 戻り値：    実行結果（正常終了はstdlib.hで定義している内容を利用）
//             正常終了：EXIT_SUCCESS（0）
//             異常終了：環境変数取得エラー：CM_ENVERROR（101）
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
int CSubOperations::GetEnvironmentDataProc(
    const char* pcEnvType,
    char* pcGetEnvData)
{
    // 変数宣言
    char* pcEnvData = NULL;
    int iEnvResult = EXIT_SUCCESS;

    // 前処理
    // 日時情報取得処理
    // 環境変数取得開始日時取得
    m_stTimeData = g_clsSubOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "環境変数取得開始日時：%s %s", 
        (char *)m_stTimeData.strMachDatePlus.c_str(), 
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 環境変数取得開始*****");
    m_stLogData = CreateEventMessageProc(ACTIONSET, "環境変数の");

    // 主処理
    // 呼び出し元で指定された環境変数名に基づいて環境変数を取得
    pcEnvData = getenv(pcEnvType);
    sprintf_s(m_cLogData, sizeof(m_cLogData), "取得した%s情報は：%s", m_stLogData.pcActionName, pcEnvData);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    // 取得結果をチェック
    if (NULL != pcEnvData)
    {
        // 環境変数の設定あり
        // 取得した環境変数をコピー
        strncpy_s(pcGetEnvData, FILENAMESIZE + 1, pcEnvData, FILENAMESIZE);
        sprintf_s(m_cLogData, sizeof(m_cLogData), "%s情報は：%s", m_stLogData.pcActionName, pcGetEnvData);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
        LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, (char*)"***** 環境変数取得正常終了 *****");
    }
    else
    {
        // 環境変数の設定なし
        // エラーコードを設定
        iEnvResult = CM_ENVERROR;
        sprintf_s(m_cLogData, sizeof(m_cLogData), "戻り値は：%d", iEnvResult);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
        LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, (char*)"***** 環境変数取得異常終了 *****");
    }

    // 後処理
    // 日時情報取得処理
    // 環境変数取得終了日時取得
    m_stTimeData = g_clsSubOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "環境変数取得終了日時：%s %s", 
        (char *)m_stTimeData.strMachDatePlus.c_str(), 
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);

    // 取得結果を返却
    return iEnvResult;
}

//********************************************
// 関数名：マシン日時取得処理
// Function Name：GetDateTimeProc
// 
// 処理概要：
// timespec_get関数及びtime関数で取得した時間を地方時間（日本時間：JST）に変換して
// 年・月・日・曜日・時・分・秒・マイクロ秒を取得する
// 
// 処理手順：
//   １．時間情報取得（timespec_get関数及びtime関数を使用）
//   ２．時間情報文字列への格納
//   ３．マシン日付設定
//   ４．マシン時刻設定
//   ５．マシン日付設定（区切り文字付き）
//   ６．マシン時刻設定（区切り文字付き）
//   ７．ログ出力用マシン日時設定
// 
// 引数：     なし
// 戻り値：   日時情報構造体
// 
// 補足事項
// ・当機能ではWindows環境用にtimespec_get関数及びtime関数で取得した時間を
//     地方時間（日本時間：JST）に変換して取得する
//     協定世界時間（UTC）の環境（主にUNIX環境）ではgettimeofday関数で取得する
//     また時間の変換方法も地方時間（日本時間：localtime）と
//     グリニッジ標準時間（世界標準時間：gmtime）の２種類があり
//     地方時間（日本時間）はグリニッジ標準時間（世界標準時間）の
//     9時間後に定められる
// 
// 
// 作成履歴
// 2020/9/25  s-saito   新規作成
// 
// 更新履歴
// 
// 
//********************************************
TIMEDATA CSubOperations::GetDateTimeProc(void)
{
    // 変数宣言
    TIMEDATA stTimeDataResult;
    char cTimeYear[TIMEDATA_YEAR + 1];
    char cTimeMonth[TIMEDATA_MONTH + 1];
    char cTimeDay[TIMEDATA_DAY + 1];
    char cTimeWeek[TIMEDATA_DAYOFWEEK + 1];
    char cTimeHour[TIMEDATA_HOUR + 1];
    char cTimeMinite[TIMEDATA_MINITE + 1];
    char cTimeSecond[TIMEDATA_SECOND + 1];
    char cTimeMsec[TIMEDATA_MSECOND + 1];
    time_t tiTime;
    struct tm* pstTime = NULL;
    struct tm* pstMTime = NULL;
    struct timespec stTimeSpec;
    char* pcDayOfWeek[] = { (char *)"Sun",
                           (char *)"Mon",
                           (char *)"Tue",
                           (char *)"Wed",
                           (char *)"Thu",
                           (char *)"Fri",
                           (char *)"Sat" };
    int iTimespec = 0;

    // 初期化
    memset(&stTimeDataResult, 0x00, sizeof(stTimeDataResult));
    memset(&stTimeSpec, 0x00, sizeof(stTimeSpec));
    memset(cTimeYear, 0x00, sizeof(cTimeYear));
    memset(cTimeMonth, 0x00, sizeof(cTimeMonth));
    memset(cTimeDay, 0x00, sizeof(cTimeDay));
    memset(cTimeWeek, 0x00, sizeof(cTimeWeek));
    memset(cTimeHour, 0x00, sizeof(cTimeHour));
    memset(cTimeMinite, 0x00, sizeof(cTimeMinite));
    memset(cTimeSecond, 0x00, sizeof(cTimeSecond));
    memset(cTimeMsec, 0x00, sizeof(cTimeMsec));

    // 前処理
    // 時間情報を取得
    iTimespec = timespec_get(&stTimeSpec, TIME_UTC);
    tiTime = time(NULL);

    // 環境に応じて時間形式を設定
    // 当アプリケーションでの使用は日本時間（地方時間）形式だが
    // グリニッジ標準時間（世界標準時間）形式の定義も加えておく
    // その場合マクロでビルドする内容を制御する
#if 0
    // グリニッジ標準時間（世界標準時間）形式に変換
    pstMTime = gmtime(&stTimeSpec.tv_sec);
    pstTime = gmtime(&tiTime);
#else
    // 日本時間（地方時間）形式に変換
    pstMTime = localtime(&stTimeSpec.tv_sec);
    pstTime = localtime(&tiTime);
#endif

    // 主処理
    // 取得した時間情報より各日時の文字列に格納
    // 年
    sprintf_s(cTimeYear, sizeof(cTimeYear), "%04d", pstTime->tm_year + 1900);

    // 月
    sprintf_s(cTimeMonth, sizeof(cTimeMonth), "%02d", pstTime->tm_mon + 1);

    // 日
    sprintf_s(cTimeDay, sizeof(cTimeDay), "%02d", pstTime->tm_mday);

    // 曜日
    sprintf_s(cTimeWeek, sizeof(cTimeWeek), "%s", pcDayOfWeek[pstTime->tm_wday]);

    // 時
    sprintf_s(cTimeHour, sizeof(cTimeHour), "%02d", pstTime->tm_hour);

    // 分
    sprintf_s(cTimeMinite, sizeof(cTimeMinite), "%02d", pstTime->tm_min);

    // 秒
    sprintf_s(cTimeSecond, sizeof(cTimeSecond), "%02d", pstTime->tm_sec);

    // マイクロ秒
    sprintf_s(cTimeMsec, sizeof(cTimeMsec), "%06ld", stTimeSpec.tv_nsec / 1000);

    // マシン日付
    stTimeDataResult.strMachDate = (const string)cTimeYear + (const string)cTimeMonth + (const string)cTimeDay;

    // マシン時刻
    stTimeDataResult.strMachTime = (const string)cTimeHour + (const string)cTimeMinite + (const string)cTimeSecond;

    // 区切り文字を入れたマシン日付
    stTimeDataResult.strMachDatePlus = (const string)cTimeYear +
        (const string)"/" +
        (const string)cTimeMonth +
        (const string)"/" +
        (const string)cTimeDay +
        (const string)"[" +
        (const string)cTimeWeek +
        (const string)"]";

    // 区切り文字を入れたマシン時刻
    stTimeDataResult.strMachTimePlus = (const string)cTimeHour +
        (const string)":" +
        (const string)cTimeMinite +
        (const string)":" +
        (const string)cTimeSecond +
        (const string)"." +
        (const string)cTimeMsec;

    // ログ出力用マシン日時
    stTimeDataResult.strMachDateTime = stTimeDataResult.strMachDatePlus + ", " + stTimeDataResult.strMachTimePlus;

    // 取得内容をログへ出力

    // 開始情報
    sprintf_s(
        m_cLogData,
        sizeof(m_cLogData),
        "マシン日時取得開始日時：%s %s",
        (char *)stTimeDataResult.strMachDatePlus.c_str(),
        (char *)stTimeDataResult.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** マシン日時取得処理開始 *****");

    // timespec_get関数で取得した時刻
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, "***** 日時情報構造体の状態 *****");
    sprintf_s(m_cLogData, sizeof(m_cLogData), "時間情報（timespec_get）：%lld %09ld", stTimeSpec.tv_sec, stTimeSpec.tv_nsec);
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

    // timespec_get関数で取得した時刻の日時情報
    // 個別情報
    // 年
    sprintf_s(m_cLogData, sizeof(m_cLogData), "年：%s", cTimeYear);
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

    // 月
    sprintf_s(m_cLogData, sizeof(m_cLogData), "月：%s", cTimeMonth);
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

    // 日
    sprintf_s(m_cLogData, sizeof(m_cLogData), "日：%s", cTimeDay);
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

    // 曜日
    sprintf_s(m_cLogData, sizeof(m_cLogData), "曜日（数値）：%d", pstTime->tm_wday);
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
    sprintf_s(m_cLogData, sizeof(m_cLogData), "曜日（文字列）：%s", cTimeWeek);
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

    // 時
    sprintf_s(m_cLogData, sizeof(m_cLogData), "時：%s", cTimeHour);
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

    // 分
    sprintf_s(m_cLogData, sizeof(m_cLogData), "分：%s", cTimeMinite);
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

    // 秒
    sprintf_s(m_cLogData, sizeof(m_cLogData), "秒：%s", cTimeSecond);
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

    // マイクロ秒
    sprintf_s(m_cLogData, sizeof(m_cLogData), "マイクロ秒：%s", cTimeMsec);
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

    // 日時文字列情報
    // マシン日付
    sprintf_s(m_cLogData, sizeof(m_cLogData), "マシン日付：%s", (char *)stTimeDataResult.strMachDate.c_str());
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

    // マシン時刻
    sprintf_s(m_cLogData, sizeof(m_cLogData), "マシン時刻：%s", (char *)stTimeDataResult.strMachTime.c_str());
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

    // 区切り文字を入れたマシン日付
    sprintf_s(m_cLogData, sizeof(m_cLogData), "区切り文字付きマシン日付：%s", (char *)stTimeDataResult.strMachDatePlus.c_str());
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

    // 区切り文字を入れたマシン時刻
    sprintf_s(m_cLogData, sizeof(m_cLogData), "区切り文字付きマシン時刻：%s", (char *)stTimeDataResult.strMachTimePlus.c_str());
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

    // ログ出力用マシン日時
    sprintf_s(m_cLogData, sizeof(m_cLogData), "ログ出力用マシン日時：%s", (char *)stTimeDataResult.strMachDateTime.c_str());
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

    // 終了情報
    sprintf_s(
        m_cLogData,
        sizeof(m_cLogData),
        "マシン日時取得終了日時：%s %s",
        (char *)stTimeDataResult.strMachDatePlus.c_str(),
        (char *)stTimeDataResult.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** マシン日時取得処理終了 *****");

    // 設定したマシン日時情報を返却
    return stTimeDataResult;
}

//********************************************
// 関数名：イベントメッセージ生成処理
// Function Name：CreateEventMessageProc
// 
// 処理概要：
// 処理の発生に伴い処理名もしくはファイル名、エラー等を
// 表示出力用のメッセージに生成する
// 
// 処理手順：
//   １．エラーコードに基づいたメッセージ取得
//   ２．発生した処理名もしくはファイル名と結合
// 
// 引数：     メッセージ生成タイプ
//            発生した処理名もしくはファイル名
//            エラーコード（設定がない場合は0）
// 戻り値：   ログ出力情報構造体
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
LOGINFORMATION CSubOperations::CreateEventMessageProc(
    int iEventType,
    const char* pcActionName,
    ERRORCODETABLE eErr)
{
    // 変数宣言
    LOGINFORMATION stEventData;
    char cActionName[FILENAMESIZE + 1];
    char cMessage[MESSAGESIZE + 1];
    const char* pcErrorMessage = "";

    // 初期化
    memset(&stEventData, 0x00, sizeof(stEventData));
    memset(cActionName, 0x00, sizeof(cActionName));
    memset(cMessage, 0x00, sizeof(cMessage));

    // 前処理
    // 日時情報取得処理
    // イベントメッセージ生成開始日時取得
    m_stTimeData = g_clsSubOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "イベントメッセージ生成開始日時：%s %s", 
        (char *)m_stTimeData.strMachDatePlus.c_str(), 
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** イベントメッセージ生成処理開始 *****");

    // 主処理
    // メッセージ生成タイプをチェック
    if (ACTIONSET == iEventType)
    {
        // 発生処理名もしくは発生ファイル名生成を選択
        // 発生処理名もしくは発生ファイル名含む文言を取得
        strncpy_s(cActionName, sizeof(cActionName), pcActionName, sizeof(cActionName) - 1);
        string strActionNameAfter(cActionName, sizeof(cActionName) / sizeof(cActionName[0]));
        stEventData.strActionName = strActionNameAfter;
        sprintf_s(
            m_cLogData, 
            sizeof(m_cLogData), 
            "発生処理名もしくはファイル名を含む文言は：%s", 
            stEventData.strActionName.c_str());
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

        // 処理名称／ファイル名称ポインタのアドレスを設定
        stEventData.pcActionName = (char*)stEventData.strActionName.c_str();
        sprintf_s(
            m_cLogData, 
            sizeof(m_cLogData), 
            "発生処理名もしくはファイル名を含む文言のポインタは：%s", 
            stEventData.pcActionName);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
    }
    else
    {
        // エラーメッセージ生成を選択
        // パラメータ情報を設定
        stEventData.eErrorCode = eErr;
        sprintf_s(m_cLogData, sizeof(m_cLogData), "エラーコードは：%d", stEventData.eErrorCode);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
        strncpy_s(cActionName, sizeof(cActionName), pcActionName, sizeof(cActionName) - 1);
        sprintf_s(m_cLogData, sizeof(m_cLogData), "発生した処理名もしくはファイル名は：%s", cActionName);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

        // エラーメッセージ取得処理
        // エラーコードに基づきエラーメッセージを取得
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, (char*)"***** エラーメッセージ取得関数呼び出し *****");
        pcErrorMessage = GetErrorMessageProc(stEventData.eErrorCode);
        sprintf_s(m_cLogData, sizeof(m_cLogData), "エラーメッセージは：%s", pcErrorMessage);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

        // 表示出力用エラーメッセージ生成
        // ファイル関係で発生したエラーかチェック
        if (tagErrorCode::FILEOPENERROR == stEventData.eErrorCode ||
            tagErrorCode::FILECLOSEERROR == stEventData.eErrorCode ||
            tagErrorCode::COPYERROR == stEventData.eErrorCode ||
            tagErrorCode::RENAMEERROR == stEventData.eErrorCode ||
            tagErrorCode::DELETEERROR == stEventData.eErrorCode ||
            tagErrorCode::STATERROR == stEventData.eErrorCode)
        {
            // ファイル関係で発生したエラー
            // 発生したファイル名を含めたエラーメッセージを生成
            sprintf_s(cMessage, sizeof(cMessage), "%s に対する%s", cActionName, GetErrorMessageProc(stEventData.eErrorCode));
            string strMessageAfter(cMessage, sizeof(cMessage) / sizeof(cMessage[0]));
            stEventData.strMessage = strMessageAfter;
        }
        else
        {
            // ファイル関係以外で発生したエラー
            // 発生した処理名を含めたメッセージを生成
            sprintf_s(cMessage, sizeof(cMessage), "%s%s", cActionName, pcErrorMessage);
            string strMessageAfter(cMessage, sizeof(cMessage) / sizeof(cMessage[0]));
            stEventData.strMessage = strMessageAfter;
        }

        // 生成したイベントメッセージをログに出力
        sprintf_s(m_cLogData, sizeof(m_cLogData), "生成後のイベントメッセージは：%s", stEventData.strMessage.c_str());
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

        // メッセージポインタのアドレスを設定
        stEventData.pcMessage = (char*)stEventData.strMessage.c_str();
        sprintf_s(m_cLogData, sizeof(m_cLogData), "イベントメッセージポインタは：%s", stEventData.pcMessage);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
    }

    // 後処理
    // 日時情報取得処理
    // イベントメッセージ生成終了日時取得
    m_stTimeData = g_clsSubOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "イベントメッセージ生成終了日時：%s %s", 
        (char *)m_stTimeData.strMachDatePlus.c_str(), 
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** イベントメッセージ生成処理終了 *****");

    // ログ出力情報構造体のメッセージ情報を返却
    return stEventData;
}

//********************************************
// 関数名：指定文字削除処理
// Function Name：DeleteDesignationCharProc
// 
// 処理概要：
// 文字列から指定文字を検索して
// 指定文字を外した文字列を生成する
// 
// 処理手順：
//   １．指定文字検索
//   ２．指定文字を外した文字設定
// 
// 引数：      指定文字込み文字列
//             指定文字
//             指定文字を外した文字列
// 戻り値：    なし
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
void CSubOperations::DeleteDesignationCharProc(
    const char* pcEditData,
    const char cDesignationChar,
    char* pcConvertResult)
{
    // 変数宣言
    char cEditBeforeData[DATASIZE + 1];
    char cEditAfterData[DATASIZE + 1];
    size_t siReadCnt = 0;
    size_t siWriteCnt = 0;
    size_t siEditDataLength = 0;

    // 初期化
    memset(cEditBeforeData, 0x00, sizeof(cEditBeforeData));
    memset(cEditAfterData, 0x00, sizeof(cEditAfterData));

    // 前処理
    // 日時情報取得処理
    // 指定文字削除開始日時取得
    m_stTimeData = g_clsSubOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData,
        sizeof(m_cLogData),
        "指定文字削除開始日時：%s %s",
        (char *)m_stTimeData.strMachDatePlus.c_str(),
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 指定文字削除処理開始 *****");

    // パラメータ情報をコピー
    strncpy_s(cEditBeforeData, sizeof(cEditBeforeData), pcEditData, sizeof(cEditBeforeData) - 1);

    // 指定文字込み文字列の長さを取得
    siEditDataLength = strlen(cEditBeforeData);
    sprintf_s(m_cLogData, sizeof(m_cLogData), "長さは：%zu", siEditDataLength);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    // 主処理
    // 指定文字込み文字列の長さ分ループ
    for (siReadCnt = 0; siReadCnt < siEditDataLength; siReadCnt++)
    {
        // 指定文字込み文字列の文字が指定文字かチェック
        if (cDesignationChar == cEditBeforeData[siReadCnt])
        {
            // チェックしている指定文字込み文字列の文字が指定文字
            // 指定文字を無視
            // スキップしてループへ
            continue;
        }
        else
        {
            // チェックしている指定文字込み文字列の文字が指定文字以外
            // チェックしている指定文字込み文字列の文字を設定
            cEditAfterData[siWriteCnt] = cEditBeforeData[siReadCnt];

            // 設定位置をインクリメント
            siWriteCnt++;
        }
    }

    // 後処理
    // 指定文字を外した文字列を格納
    cEditAfterData[strlen(cEditAfterData)] = '\0';
    siEditDataLength = strlen(cEditAfterData);
    strncpy_s(pcConvertResult, DATASIZE + 1, cEditAfterData, DATASIZE);
    pcConvertResult[siEditDataLength] = '\0';
    sprintf_s(m_cLogData, sizeof(m_cLogData), "削除後の結果は：%s", pcConvertResult);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    // 日時情報取得処理
    // 指定文字削除終了日時取得
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 指定文字削除処理終了 *****");
    m_stTimeData = g_clsSubOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData,
        sizeof(m_cLogData),
        "指定文字削除終了日時：%s %s",
        (char *)m_stTimeData.strMachDatePlus.c_str(),
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
}

//********************************************
// 関数名：指定文字列分割処理
// Function Name：SplitDataProc
// 
// 処理概要：
// 指定文字列から指定文字を検索して
// 文字列を分割する
// 
// 処理手順：
//   １．入出力可能データ変換
//   ２．指定文字列分割
// 
// 引数：      指定文字列
//             指定文字
// 戻り値：    分割結果
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
deque<string> CSubOperations::SplitDataProc(
    const string strSplitData,
    const char cChar)
{
    // 変数宣言
    deque<string> deqSplitData;
    string strSplitBuffer;

    // 前処理
    // 日時情報取得処理
    // 指定文字列分割開始日時取得
    m_stTimeData = g_clsSubOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData,
        sizeof(m_cLogData),
        "指定文字列分割開始日時：%s %s",
        (char *)m_stTimeData.strMachDatePlus.c_str(),
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 指定文字列分割処理開始 *****");

    // 入出力可能なデータに変換
    stringstream ssSplitData(strSplitData);

    // 主処理
    // 文字列の長さ分ループ
    // （getlineはC++言語定義）
    while (getline(ssSplitData, strSplitBuffer, cChar))
    {
        // 抽出された文字列を格納
        deqSplitData.push_back(strSplitBuffer);
    }

    // 後処理
    // 格納行数分ループ
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, "***** 分割後の文字列状態 *****");
    for (size_t siCnt = 0; siCnt < deqSplitData.size(); siCnt++)
    {
        // 格納した文字列をログに出力
        sprintf_s(m_cLogData, sizeof(m_cLogData), "%zu つ目の文字列は：%s", siCnt + 1, deqSplitData[siCnt].c_str());
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
    }

    // 日時情報取得処理
    // 指定文字削除終了日時取得
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** 指定文字列分割処理終了 *****");
    m_stTimeData = g_clsSubOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData,
        sizeof(m_cLogData),
        "指定文字列分割終了日時：%s %s",
        (char *)m_stTimeData.strMachDatePlus.c_str(),
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);

    // 分割結果を返却
    return deqSplitData;
}

//********************************************
// 関数名：マルチバイト文字列ワイド文字列変換処理
// Function Name：MultiToWideProc
// 
// 処理概要：
// マルチバイト文字列に対してワイド文字列に変換する
// 
// 処理手順：
//   １．メモリ領域確保
//   ２．マルチバイト文字列からワイド文字列に変換
// 
// 引数：	 マルチバイト文字列
// 戻り値：  ワイド文字列
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
wstring CSubOperations::MultiToWideProc(
    string strMultiByte)
{
    // 変数宣言
    wstring wstrWideCharBuff;
    WCHAR* pcwWideCharBuff = NULL;
    size_t siRet = 0;
    int iDestsize = 0;
    int iChangeLen = 0;
    int iBuffSize = 0;

    // 標準出力に日本語が表示されるようにロケールを日本に設定
    setlocale(LC_ALL, "japanese");

    // メモリ領域確保に失敗した場合のNULL設定
    // この設定により確保に失敗した場合はNULLを返すようにする
    set_new_handler(NULL);

    // 前処理
    // 日時情報取得処理
    // マルチバイト文字列ワイド文字列変換開始日時取得
    m_stTimeData = g_clsSubOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData,
        sizeof(m_cLogData),
        "マルチバイト文字列ワイド文字列変換開始日時：%s %s",
        (char*)m_stTimeData.strMachDatePlus.c_str(),
        (char*)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** マルチバイト文字列ワイド文字列変換処理開始 *****");

    // ロケールを設定
    setlocale(LC_CTYPE, "jpn");

    // マルチバイト文字列からワイド文字列に変換したときの文字列長取得
    // （MultiByteToWideCharはC++言語定義）
    iChangeLen = MultiByteToWideChar(CP_ACP, 0, strMultiByte.c_str(), -1, NULL, 0);
    sprintf_s(m_cLogData, sizeof(m_cLogData), "マルチバイト文字列は：%s", strMultiByte.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
    sprintf_s(m_cLogData, sizeof(m_cLogData), "ワイド文字列への変換に必要な長さは：%d", iChangeLen);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    // 文字列長が取得出来たかチェック
    if (0 == iChangeLen)
    {
        // 文字列長が取得出来なかった
        // 文字列長取得失敗メッセージをログに出力
        LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** 文字列長の取得失敗(。>д<) *****");

        // NULLを設定
        pcwWideCharBuff = NULL;
        goto END;
    }

    // 主処理
    try
    {
        // マルチバイト文字列ワイド文字列変換メモリ領域を確保
        iBuffSize = iChangeLen + 1;
        pcwWideCharBuff = DBG_NEW WCHAR[iBuffSize];
        m_stLogData = CreateEventMessageProc(ACTIONSET, "マルチバイト文字列ワイド文字列変換の");

        // メモリ領域が確保出来たかチェック
        if (NULL == pcwWideCharBuff)
        {
            // メモリ領域の確保失敗
            // 例外処理へスロー
            throw tagErrorCode::MEMORYERROR;
        }

        // メモリの初期化
        memset(pcwWideCharBuff, 0x00, iBuffSize);

        // マルチバイト文字列ワイド文字列変換メモリ領域確保成功のメッセージをログに出力
        LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, "***** マルチバイト文字列ワイド文字列変換メモリ領域確保正常終了 *****");
        sprintf_s(m_cLogData, sizeof(m_cLogData), "%sの%s", m_stLogData.pcActionName, MEMORYCREATESUCCESS);
        LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, m_cLogData);

        // マルチバイト文字列からワイド文字列に変換
        // （mbstowcs_sはC++言語定義）
        mbstowcs_s(&siRet, pcwWideCharBuff, iBuffSize, strMultiByte.c_str(), _TRUNCATE);
        sprintf_s(m_cLogData, sizeof(m_cLogData), "ワイド文字列は：%ls", pcwWideCharBuff);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
        sprintf_s(m_cLogData, sizeof(m_cLogData), "ワイド文字列の長さは：%zu", siRet);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

        // 変換が出来たかチェック
        if (iChangeLen != siRet)
        {
            // 変換失敗
            // 変換失敗メッセージをログに出力
            LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** マルチバイト文字列からのワイド文字列変換失敗(。>д<) *****");

            // メモリ領域を解放
            // 必ずdeleteすること！
            SAFE_FREE_NEWALLOC_ARRAY(pcwWideCharBuff);
            goto END;
        }
        else
        {
            // 変換成功
            // 変換されたワイド文字列をログに出力
            wstrWideCharBuff = (const wstring)pcwWideCharBuff;
            sprintf_s(m_cLogData, sizeof(m_cLogData), "ワイド文字列は：%ws", wstrWideCharBuff.c_str());
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

            // メモリ領域を解放
            // 必ずdeleteすること！
            SAFE_FREE_NEWALLOC_ARRAY(pcwWideCharBuff);
        }
    }

    // 例外処理
    catch (const ERRORCODETABLE eAllocErr)
    {
        // エラーメッセージ生成処理
        // エラーメッセージを出力
        pcwWideCharBuff = NULL;
        LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** マルチバイト文字列ワイド文字列変換メモリ領域確保異常終了 *****");
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** エラーメッセージ生成関数呼び出し *****");
        m_eErrorResultCode = eAllocErr;
        m_stLogData = CreateEventMessageProc(ERRORSET, m_stLogData.pcActionName, m_eErrorResultCode);
        LOGOUTPUTERROR(TRACEOUTPUTLOG, ERRORINFORMATION1, m_stLogData.pcMessage, m_stLogData.eErrorCode);
        sprintf_s(m_cLogData, sizeof(m_cLogData), "戻り値は：%d", m_stLogData.eErrorCode);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
    }

END:
    // 後処理
    // 日時情報取得処理
    // マルチバイト文字列ワイド文字列変換終了日時取得
    m_stTimeData = g_clsSubOpeLogFile.GetDateTimeProc();
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** マルチバイト文字列ワイド文字列変換処理終了 *****");
    sprintf_s(
        m_cLogData,
        sizeof(m_cLogData),
        "マルチバイト文字列ワイド文字列変換終了日時：%s %s",
        (char*)m_stTimeData.strMachDatePlus.c_str(),
        (char*)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);

    // 変換後の文字列を返却
    return wstrWideCharBuff;
}

//********************************************
// 関数名：ワイド文字列マルチバイト文字列変換処理
// Function Name：WideToMultiProc
// 
// 処理概要：
// ワイド文字列に対してマルチバイト文字列に変換する
// 
// 処理手順：
//   １．メモリ領域確保
//   ２．ワイド文字列からマルチバイト文字列に変換
// 
// 引数：	 ワイド文字列
// 戻り値：  マルチバイト文字列
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
string CSubOperations::WideToMultiProc(
    wstring wstrWideChar)
{
    // 変数宣言
    string strMultiByteBuff;
    char* pcMultiByteBuff = NULL;
    int iChangeLen = 0;
    size_t siRet = 0;
    int iBuffSize = 0;

    // 標準出力に日本語が表示されるようにロケールを日本に設定
    setlocale(LC_ALL, "japanese");

    // メモリ領域確保に失敗した場合のNULL設定
    // この設定により確保に失敗した場合はNULLを返すようにする
    set_new_handler(NULL);

    // 前処理
    // 日時情報取得処理
    // ワイド文字列マルチバイト文字列変換開始日時取得
    m_stTimeData = g_clsSubOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData,
        sizeof(m_cLogData),
        "ワイド文字列マルチバイト文字列変換開始日時：%s %s",
        (char*)m_stTimeData.strMachDatePlus.c_str(),
        (char*)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ワイド文字列マルチバイト文字列変換処理開始 *****");

    // ロケールを設定
    setlocale(LC_CTYPE, "jpn");

    // ワイド文字列からマルチバイト文字列に変換したときの文字列長取得
    // （WideCharToMultiByteはC++言語定義）
    iChangeLen = WideCharToMultiByte(CP_ACP, 0, wstrWideChar.c_str(), -1, NULL, 0, NULL, NULL);
    sprintf_s(m_cLogData, sizeof(m_cLogData), "ワイド文字列は：%ls", wstrWideChar.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
    sprintf_s(m_cLogData, sizeof(m_cLogData), "マルチバイト文字列への変換に必要な長さは：%d", iChangeLen);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    // 文字列長が取得出来たかチェック
    if (0 == iChangeLen)
    {
        // 文字列長が取得出来なかった
        // 文字列長取得失敗メッセージをログに出力
        LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** 文字列長取得失敗(。>д<) *****");

        // NULLを設定
        pcMultiByteBuff = NULL;
        goto END;
    }

    // 主処理
    try
    {
        // ワイド文字列マルチバイト文字列変換メモリ領域を確保
        iBuffSize = iChangeLen + 1;
        pcMultiByteBuff = DBG_NEW char[iBuffSize];
        m_stLogData = CreateEventMessageProc(ACTIONSET, "ワイド文字列マルチバイト文字列変換の");

        // メモリ領域が確保出来たかチェック
        if (NULL == pcMultiByteBuff)
        {
            // メモリ領域の確保失敗
            // 例外処理へスロー
            throw tagErrorCode::MEMORYERROR;
        }

        // メモリの初期化
        memset(pcMultiByteBuff, 0x00, iBuffSize);

        // ワイド文字列マルチバイト文字列変換メモリ領域確保成功のメッセージをログに出力
        LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, "***** ワイド文字列マルチバイト文字列変換メモリ領域確保正常終了 *****");
        sprintf_s(m_cLogData, sizeof(m_cLogData), "%sの%s", m_stLogData.pcActionName, MEMORYCREATESUCCESS);
        LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, m_cLogData);

        // ワイド文字列からマルチバイト文字列に変換
        // （wcstombs_sはC++言語定義）
        wcstombs_s(&siRet, pcMultiByteBuff, iBuffSize, wstrWideChar.c_str(), _TRUNCATE);
        sprintf_s(m_cLogData, sizeof(m_cLogData), "マルチバイト文字列は：%s", pcMultiByteBuff);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
        sprintf_s(m_cLogData, sizeof(m_cLogData), "マルチバイト文字列の長さは：%zu", siRet);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

        // 変換が出来たかチェック
        if (iChangeLen != siRet)
        {
            // 変換失敗
            // 変換失敗メッセージをログに出力
            LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** ワイド文字列からのマルチバイト文字列変換失敗(。>д<) *****");

            // メモリ領域を解放
            // 必ずdeleteすること！
            SAFE_FREE_NEWALLOC_ARRAY(pcMultiByteBuff);
            goto END;
        }
        else
        {
            // 変換成功
            // 変換されたマルチバイト文字列をログに出力
            strMultiByteBuff = (const string)pcMultiByteBuff;
            sprintf_s(m_cLogData, sizeof(m_cLogData), "マルチバイト文字列は：%s", strMultiByteBuff.c_str());
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

            // メモリ領域を解放
            // 必ずdeleteすること！
            SAFE_FREE_NEWALLOC_ARRAY(pcMultiByteBuff);
        }
    }

    // 後処理
    // 例外処理
    catch (const ERRORCODETABLE eAllocErr)
    {
        // エラーメッセージ生成処理
        // エラーメッセージを出力
        pcMultiByteBuff = NULL;
        LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** ワイド文字列マルチバイト文字列変換メモリ領域確保異常終了 *****");
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** エラーメッセージ生成関数呼び出し *****");
        m_eErrorResultCode = eAllocErr;
        m_stLogData = CreateEventMessageProc(ERRORSET, m_stLogData.pcActionName, m_eErrorResultCode);
        LOGOUTPUTERROR(TRACEOUTPUTLOG, ERRORINFORMATION1, m_stLogData.pcMessage, m_stLogData.eErrorCode);
        sprintf_s(m_cLogData, sizeof(m_cLogData), "戻り値は：%d", m_stLogData.eErrorCode);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
    }

END:
    // 後処理
    // 日時情報取得処理
    // ワイド文字列マルチバイト文字列変換終了日時取得
    m_stTimeData = g_clsSubOpeLogFile.GetDateTimeProc();
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ワイド文字列マルチバイト文字列変換処理終了 *****");
    sprintf_s(
        m_cLogData,
        sizeof(m_cLogData),
        "ワイド文字列マルチバイト文字列変換終了日時：%s %s",
        (char*)m_stTimeData.strMachDatePlus.c_str(),
        (char*)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);

    // 変換後の文字列を返却
    return strMultiByteBuff;
}

//********************************************
// 関数名：コマンドライン引数解析処理
// Function Name：ParseArgsProc
// 
// 処理概要：
// コマンドライン引数を解析する
// 
// 処理手順：
//   １．引数文字列ワイド文字列変換
//   ２．コマンドライン引数解析
// （以降ここからコマンド情報数分ループ）
//   ３．解析した情報をマルチバイト文字列変換
// （ここまでループ）
//   ４．解析した情報メモリ解放
// 
// 引数：	 コマンドライン引数
//           コマンド情報数
// 戻り値：  コマンド情報
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
vector<string> CSubOperations::ParseArgsProc(
    string strArgst,
    int* iArgv)
{
    // 変数宣言
    vector<string> vecArgvc;
    WCHAR* pcwArgsw = NULL;
    wstring wstrArgsw;
    LPWSTR* pcwArgvw = NULL;
    string strArgvc;
    int iICnt = 0;
    int iJCnt = 0;

    // 前処理
    // パラメータをチェック
    if (0 == strArgst.size())
    {
        // コマンドラインが取得出来ていない
        // NULLを返却
        *iArgv = 0;
        goto END;
    }

    // 引数文字列をワイド文字列に変換
    wstrArgsw = MultiToWideProc(strArgst);

    // ワイド文字列に変換出来たかチェック
    if (0 == wstrArgsw.size())
    {
        // 変換失敗
        // 関数を終了
        goto END;
    }

    // 主処理
    // コマンドライン引数を解析
    // （CommandLineToArgvWはC++言語定義）
    pcwArgvw = CommandLineToArgvW(wstrArgsw.c_str(), iArgv);

    // 解析出来たかチェック
    if (NULL == pcwArgvw)
    {
        // 解析失敗
        // NULLを返却
        pcwArgvw = NULL;
        goto END;
    }

    // 後処理
    // 解析したコマンドライン引数の数分ループ
    for (iICnt = 0; iICnt < *iArgv; ++iICnt)
    {
        // 解析した情報をマルチバイト文字列に変換して格納
        strArgvc = WideToMultiProc(pcwArgvw[iICnt]);

        // 格納出来たかチェック
        if (0 == strArgvc.size() && 0 == iICnt)
        {
            // 格納に失敗
            // 関数を終了
            goto END;
        }

        // 変換したマルチバイト文字列を格納
        vecArgvc.push_back(strArgvc);
    }

    // 解析した情報を解放
    // （LocalFreeはC++言語定義）
    (void)LocalFree((HLOCAL)pcwArgvw);

END:
    // 格納したマルチバイト文字列群を返却
    return vecArgvc;
}

//********************************************
// 関数名：エラーメッセージ取得処理
// Function Name：GetErrorMessageProc
// 
// 処理概要：
// エラーコードに対して以下のメッセージを設定する
//      ・環境変数取得エラー
//      ・パラメータエラー（指定なし）
//      ・パラメータエラー（複数指定）
//      ・ファイルオープンエラー
//      ・ファイルクローズエラー
//      ・データなしエラー
//      ・データ不正エラー
//      ・データ取得エラー
//      ・メモリ領域確保エラー
//      ・ファイルコピーエラー
//      ・ファイル名変換エラー
//      ・ファイル削除エラー
//      ・ファイル状態取得エラー
//      ・その他エラー
// 
// 処理手順：
//   １．エラーコードに基づいたメッセージ設定
// 
// 引数：     エラーコード
// 戻り値：   エラーメッセージ
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
const char *CSubOperations::GetErrorMessageProc(
    ERRORCODETABLE eErr)
{
    // 変数宣言
    const char* pcErrMessage = "";

    // 前処理
    // 日時情報取得処理
    // エラーメッセージ取得開始日時取得
    m_stTimeData = g_clsSubOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "エラーメッセージ取得開始日時：%s %s", 
        (char *)m_stTimeData.strMachDatePlus.c_str(), 
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** エラーメッセージ取得処理開始 *****");

    // エラーコードをログに出力
    sprintf_s(m_cLogData, sizeof(m_cLogData), "エラーコードは：%d", eErr);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    // 主処理
    // エラーコードに基づきメッセージを取得
    switch (eErr)
    {
        case tagErrorCode::ENVERROR:
            // 環境変数取得エラー
            // メッセージを設定
            pcErrMessage = ENVGETERROR;
            break;
        case tagErrorCode::NOPARAM:
            // パラメータ指定なし
            // メッセージを設定
            pcErrMessage = NOTPARAMETER;
            break;
        case tagErrorCode::FAILPARAM:
            // パラメータエラー（複数指定）
            // メッセージを設定
            pcErrMessage = PARAMETERERROR;
            break;
        case tagErrorCode::FILEOPENERROR:
            // ファイルが開けない（指定したファイルがない等）
            // メッセージを設定
            pcErrMessage = FILECANNOTOPEN;
            break;
        case tagErrorCode::FILECLOSEERROR:
            // ファイルが閉じれない
            // メッセージを設定
            pcErrMessage = FILECANNOTCLOSE;
            break;
        case tagErrorCode::NODATA:
            // データがない
            // メッセージを設定
            pcErrMessage = NOTDATA;
            break;
        case tagErrorCode::DATAFRAUD:
            // データが不正
            // メッセージを設定
            pcErrMessage = DATAINJUSTICE;
            break;
        case tagErrorCode::DATAERROR:
            // データ取得エラー
            // メッセージを設定
            pcErrMessage = DATAGETERROR;
            break;
        case tagErrorCode::MEMORYERROR:
            // メモリ領域確保失敗
            // メッセージを設定
            pcErrMessage = MEMORYENSUREERROR;
            break;
        case tagErrorCode::COPYERROR:
            // ファイルコピーエラー
            // メッセージを設定
            pcErrMessage = FILECOPYERROR;
            break;
        case tagErrorCode::RENAMEERROR:
            // ファイル名変換エラー
            // メッセージを設定
            pcErrMessage = FILERENAMEERROR;
            break;
        case tagErrorCode::DELETEERROR:
            // ファイル削除エラー
            // メッセージを設定
            pcErrMessage = FILEDELETEERROR;
            break;
        case tagErrorCode::STATERROR:
            // ファイル状態取得エラー
            // メッセージを設定
            pcErrMessage = FILESTATERROR;
            break;
        default:
            // 上記以外
            // メッセージを設定
            pcErrMessage = OPERATIONERROR;
            break;
    }

    // 後処理
    // 取得したメッセージをログに出力
    sprintf_s(m_cLogData, sizeof(m_cLogData), "エラーメッセージは：%s", pcErrMessage);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    // 日時情報取得処理
    // エラーメッセージ取得終了日時取得
    m_stTimeData = g_clsSubOpeLogFile.GetDateTimeProc();
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** エラーメッセージ取得処理終了 *****");
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "エラーメッセージ取得終了日時：%s %s", 
        (char *)m_stTimeData.strMachDatePlus.c_str(), 
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);

    // エラーメッセージを返却
    return pcErrMessage;
}
