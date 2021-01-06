//============================================
// ログ出力クラスソースファイル
// CLogFile.cpp
// 
// 機能概要
// ログ出力ファイルに情報を出力する機能を定義する
// 
// 機能内容
// １．日時情報取得処理
// ２．ログ出力制御処理
// ３．マシン日時取得処理（ログ出力使用）
// ４．トレースログ出力処理
// ５．データログ出力処理
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
#include "CLogFile.h"

//☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★
// 各機能（API）定義
//☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★
//********************************************
// 関数名：デフォルトコンストラクタ
// Function Name：CLogFile
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
CLogFile::CLogFile()
{
    // 現在デフォルトコンストラクタでの処理はなし
}

//********************************************
// 関数名：パラメータ情報付きコンストラクタ
// Function Name：CLogFile
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
CLogFile::CLogFile(
    const string strTraceLogLogFileOpeFileName, 
    const string strDataLogLogFileOpeFileName)
{
    // 呼び出し元で指定されたログ出力ファイル名設定
    m_strTraceLogFileName = strTraceLogLogFileOpeFileName;
    m_strDataLogFileName = strDataLogLogFileOpeFileName;
}

//********************************************
// 関数名：デストラクタ
// Function Name：~CLogFile
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
CLogFile::~CLogFile()
{
    // メンバ変数クリア
    m_strTraceLogFileName.clear();
    m_strDataLogFileName.clear();
    memset(&m_stTimeData, 0x00, sizeof(TIMEDATA));
    memset(&m_stLogData, 0x00, sizeof(LOGINFORMATION));
    memset(m_cLogData, 0x00, sizeof(m_cLogData));
    m_eErrorResultCode = tagErrorCode::SUCCESS;
}

//********************************************
// 関数名：マシン日時取得処理（ログ出力使用）
// Function Name：GetDateTimeLogOutputUseProc
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
TIMEDATA CLogFile::GetDateTimeProc(void)
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
    char* pcDayOfWeek[] = { (char*)"Sun",
                           (char*)"Mon" ,
                           (char*)"Tue" ,
                           (char*)"Wed" ,
                           (char*)"Thu" ,
                           (char*)"Fri" ,
                           (char*)"Sat" };
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

    // 後処理
    // マシン日付
    stTimeDataResult.strMachDate = (string)cTimeYear + (string)cTimeMonth + (string)cTimeDay;

    // マシン時刻
    stTimeDataResult.strMachTime = (string)cTimeHour + (string)cTimeMinite + (string)cTimeSecond;

    // 区切り文字を入れたマシン日付
    stTimeDataResult.strMachDatePlus = (string)cTimeYear +
        (string)"/" +
        (string)cTimeMonth +
        (string)"/" +
        (string)cTimeDay +
        (string)"[" +
        (string)cTimeWeek +
        (string)"]";

    // 区切り文字を入れたマシン時刻
    stTimeDataResult.strMachTimePlus = (string)cTimeHour +
        (string)":" +
        (string)cTimeMinite +
        (string)":" +
        (string)cTimeSecond +
        (string)"." +
        (string)cTimeMsec;

    // ログ出力用マシン日時
    stTimeDataResult.strMachDateTime = stTimeDataResult.strMachDatePlus + ", " + stTimeDataResult.strMachTimePlus;

    // 設定したマシン日時情報を返却
    return stTimeDataResult;
}

//********************************************
// 関数名：ログ出力制御処理
// Function Name：LogOutputControlProc
// 
// 処理概要：
// トレースログ出力ファイル名及びデータログ出力ファイル名が
// 格納されているかチェックして格納されている場合のみログ出力を指示する
// 
// 処理手順：
//   １．メッセージ等ローカルコピー
//   ２．ファイル名格納チェック
//   ３．出力ログ識別子によるフラグ設定
//   ４．トレースログ出力指示
//               ・データ情報
//               ・エラー情報
//               ・エラーメッセージ情報
//               ・成功情報
//               ・エラー以外のメッセージ
//   ５．データログ出力指示
//               ・データ情報
// 
// 引数：     実行ソースファイル名
//            実行関数名
//            実行ライン名
//            出力ログ識別子
//            トレースログ識別子
//            ログ出力用文字列
//            エラーコード(設定がない場合は0)
// 戻り値：   なし
// 
// 補足事項
// ・エラーコードはエラーメッセージの場合のみ出力する
// 
// 
// 作成履歴
// 2020/9/25  s-saito   新規作成
// 
// 更新履歴
// 
// 
//********************************************
void CLogFile::LogOutputControlProc(
	const char *pcExecSrcName, 
	const char *pcExecProcName, 
	int iExecLineCnt, 
	int iLogOutputType, 
	int iLogTraceType, 
	const char *pcMessage, 
	ERRORCODETABLE eErrCd)
{
    // 変数宣言
    char cExecSrcName[FILENAMESIZE + 1];
    char cExecProcName[APINAME_SIZE + 1];
    char cOutputMessage[MESSAGESIZE + 1];
    int iTraceLogFlag = 0;

    // 初期化
    memset(cExecSrcName, 0x00, sizeof(cExecSrcName));
    memset(cExecProcName, 0x00, sizeof(cExecProcName));
    memset(cOutputMessage, 0x00, sizeof(cOutputMessage));

    // 前処理
    // メッセージをコピー
    strncpy_s(cExecSrcName, sizeof(cExecSrcName), pcExecSrcName, sizeof(cExecSrcName) - 1);
    strncpy_s(cExecProcName, sizeof(cExecProcName), pcExecProcName, sizeof(cExecProcName) - 1);
    strncpy_s(cOutputMessage, sizeof(cOutputMessage), pcMessage, sizeof(cOutputMessage) - 1);

    // 主処理
    // 出力ログ識別子をチェック
    if (TRACEOUTPUTLOG == iLogOutputType)
    {
        // トレースログ
        // トレースログフラグをON
        iTraceLogFlag = 1;
    }
    else
    {
        // データログ
        // トレースログフラグをOFF
        iTraceLogFlag = 0;
    }

    // 後処理
    // トレースログフラグがONかチェック
    if (1 == iTraceLogFlag)
    {
        // トレースログフラグがON
        // トレースログ
        // トレースログ識別子に基づきトレースログ出力ファイルにメッセージ出力指示
        switch (iLogTraceType)
        {
        case DATAINFORMATION:
            // データ情報
            // トレースログ出力ファイルにデータ情報を出力指示
            WriteTraceLogProc(cExecSrcName, cExecProcName, iExecLineCnt, (const char*)LOG_TYPE_DATA, cOutputMessage);
            break;
        case ERRORINFORMATION1:
            // エラー情報
            // トレースログ出力ファイルにエラー情報を出力指示
            WriteTraceLogProc(cExecSrcName, cExecProcName, iExecLineCnt, (const char*)LOG_TYPE_ERROR, cOutputMessage, eErrCd);
            break;
        case ERRORINFORMATION2:
            // エラーメッセージ情報
            // トレースログ出力ファイルにエラーメッセージ情報を出力指示
            WriteTraceLogProc(cExecSrcName, cExecProcName, iExecLineCnt, (const char*)LOG_TYPE_ERROR, cOutputMessage);
            break;
        case SUCCESSINFORMATION:
            // 成功情報
            // トレースログ出力ファイルに成功情報を出力指示
            WriteTraceLogProc(cExecSrcName, cExecProcName, iExecLineCnt, (const char*)LOG_TYPE_SUCCESS, cOutputMessage);
            break;
        default:
            // 上記以外
            // インフォメーション
            // トレースログ出力ファイルにインフォメーションを出力指示
            WriteTraceLogProc(cExecSrcName, cExecProcName, iExecLineCnt, (const char*)LOG_TYPE_INFORMATION, cOutputMessage);
            break;
        }
    }
    else
    {
        // トレースログフラグがOFF
        // データログ
        // データログ出力ファイルにデータ情報を出力指示
        WriteDataLogProc(cExecSrcName, cExecProcName, iExecLineCnt, cOutputMessage);
    }

    // 処理終了
    return;
}

//********************************************
// 関数名：トレースログ出力処理
// Function Name：WriteTraceLogProc
// 
// 処理概要：
// トレースログ出力ファイルにメッセージ等を出力する
// 
// 処理手順：
//   １．トレースログ出力ファイルオープン
//   ２．メッセージ等ローカルコピー
//   ３．エラーコードチェック
//   ４．ファイル出力（エラーメッセージ）
//   ５．ファイル出力（エラー以外のメッセージ）
//   ６．トレースログ出力ファイルクローズ
// 
// 引数：     実行ソースファイル名
//            実行関数名
//            実行ライン名
//            ログ出力種別
//            ログ出力用文字列
//            エラーコード(設定がない場合は0)
// 戻り値：   なし
// 
// 補足事項
// ・エラーコードはエラーメッセージの場合のみ出力する
// 
// 
// 作成履歴
// 2020/9/25  s-saito   新規作成
// 
// 更新履歴
// 
// 
//********************************************
void CLogFile::WriteTraceLogProc(
	const char *pcExecSrcName, 
	const char *pcExecProcName, 
	int iExecLineCnt, 
	const char *pcLogType, 
	const char *pcMessage, 
	ERRORCODETABLE eErrCd)
{
    // 変数宣言
    FILE* pfTraceLogFile = NULL;
    char cOutputMessage[MESSAGESIZE + 1];
    char cExecProcName[APINAME_SIZE + 1];
    char cExecSrcName[FILENAMESIZE + 1];
    char cExecBaseSrcName[FILENAMESIZE + 1];
    char cExecSrcNameMain[FILENAMESIZE + 1];
    char cExecSrcExtension[FILEEXTENSION_SIZE + 1];
    char cLogType[LOG_TYPE_SIZE + 1];
    errno_t eLogErrNo = 0;
    int iCloseResult = EXIT_SUCCESS;
    string strLogOutputData;

    // 初期化
    memset(cOutputMessage, 0x00, sizeof(cOutputMessage));
    memset(cExecProcName, 0x00, sizeof(cExecProcName));
    memset(cExecSrcName, 0x00, sizeof(cExecSrcName));
    memset(cExecBaseSrcName, 0x00, sizeof(cExecBaseSrcName));
    memset(cExecSrcNameMain, 0x00, sizeof(cExecSrcNameMain));
    memset(cExecSrcExtension, 0x00, sizeof(cExecSrcExtension));
    memset(cLogType, 0x00, sizeof(cLogType));

    // 前処理
    // 現在日時取得
    m_stTimeData = CLogFile::GetDateTimeProc();

    // マシン日時（ログ出力用）を取得
    m_stLogData.strDateTime = m_stTimeData.strMachDateTime;

    // トレースログ出力ファイル（更新追記型）をオープン
    // トレースログ出力ファイルはオープンに失敗した場合出力は諦めて処理を継続
    eLogErrNo = fopen_s(&pfTraceLogFile, m_strTraceLogFileName.c_str(), "a+");

    // 主処理
    // メッセージをコピー
    strncpy_s(cExecBaseSrcName, sizeof(cExecBaseSrcName), pcExecSrcName, sizeof(cExecBaseSrcName) - 1);
    strncpy_s(cExecProcName, sizeof(cExecProcName), pcExecProcName, sizeof(cExecProcName) - 1);
    strncpy_s(cLogType, sizeof(cLogType), pcLogType, sizeof(cLogType) - 1);
    strncpy_s(cOutputMessage, sizeof(cOutputMessage), pcMessage, sizeof(cOutputMessage) - 1);

    // 当実行ソースファイルパスを分割（_splitpath_s関数を使用）
    // _splitpath_s関数はファイル名を含んだ完全パス(フルパス)を分解する機能をもつ
    // ここではソースファイル名と拡張子のみを必要とするためソースファイル名と拡張子以外はNULLを指定
    _splitpath_s(
        cExecBaseSrcName, 
        NULL, 
        0, 
        NULL, 
        0, 
        cExecSrcNameMain, 
        sizeof(cExecSrcNameMain), 
        cExecSrcExtension, 
        sizeof(cExecSrcExtension));
    strncpy_s(cExecSrcName, sizeof(cExecSrcName), cExecSrcNameMain, sizeof(cExecSrcName) - 1);
    strncat_s(cExecSrcName, sizeof(cExecSrcName), cExecSrcExtension, sizeof(cExecSrcName) - 1);

    // エラーコードをチェック
    if (tagErrorCode::SUCCESS != eErrCd)
    {
        // エラー出力の場合
        // エラーコードを含めた状態にてログで出力するための文字列を生成
        strLogOutputData = m_stLogData.strDateTime + 
            ", " + 
            cExecSrcName + 
            ", " + 
            cExecProcName + 
            ", " + 
            to_string(iExecLineCnt) + 
            ", " + 
            cLogType + 
            ", " + 
            cOutputMessage + 
            ", " + 
            "ErrorCode=[" + 
            to_string((int)eErrCd) + 
            "]";
    }
    else
    {
        // メッセージ出力の場合
        // エラーコードを含まない状態にてログで出力するための文字列を生成
        strLogOutputData = m_stLogData.strDateTime + 
            ", " + 
            cExecSrcName + 
            ", " + 
            cExecProcName + 
            ", " + 
            to_string(iExecLineCnt) + 
            ", " + 
            cLogType + 
            ", " + 
            cOutputMessage;
    }

    // 後処理
    // トレースログ出力ファイルをオープンしているかチェック
    if (NULL == pfTraceLogFile)
    {
        // トレースログ出力ファイルオープンに失敗している場合
        // デバッグウィンドウに出力
        // （OutputDebugStringAはC++言語定義）
        OutputDebugStringA(strLogOutputData.c_str());
        OutputDebugStringA("\n");
    }
    else
    {
        // トレースログ出力ファイルオープンに成功している場合
        // トレースログ出力ファイルに出力
        fprintf(pfTraceLogFile, "%s\n", strLogOutputData.c_str());

        // デバッグウィンドウに出力
        // （OutputDebugStringAはC++言語定義）
        OutputDebugStringA(strLogOutputData.c_str());
        OutputDebugStringA("\n");

        // トレースログ出力ファイルをクローズ
        // トレースログ出力ファイルがクローズ出来なくてもメッセージ等出さずに処理終了
        iCloseResult = fclose(pfTraceLogFile);
    }

    // 処理終了
    return;
}

//********************************************
// 関数名：データログ出力処理
// Function Name：WriteDataLogProc
// 
// 処理概要：
// データログ出力ファイルにメッセージ等を出力する
// 
// 処理手順：
//   １．データログ出力ファイルオープン
//   ２．メッセージ等ローカルコピー
//   ３．ファイル出力
//   ４．データログ出力ファイルクローズ
// 
// 引数：	 実行ソースファイル名
//           実行関数名
//           実行ライン名
//           ログ出力用文字列
// 戻り値：  なし
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
void CLogFile::WriteDataLogProc(
	const char *pcExecSrcName, 
	const char *pcExecProcName, 
	int iExecLineCnt, 
	const char *pcMessage)
{
    // 変数宣言
    FILE* pfDataLogFile = NULL;
    char cDataOutputMessage[MESSAGESIZE + 1];
    char cExecProcName[APINAME_SIZE + 1];
    char cExecSrcName[FILENAMESIZE + 1];
    char cExecBaseSrcName[FILENAMESIZE + 1];
    char cExecSrcNameMain[FILENAMESIZE + 1];
    char cExecSrcExtension[FILEEXTENSION_SIZE + 1];
    errno_t eLogErrNo = 0;
    int iCloseResult = EXIT_SUCCESS;
    string strDataLogOutputData;

    // 初期化
    memset(cDataOutputMessage, 0x00, sizeof(cDataOutputMessage));
    memset(cExecProcName, 0x00, sizeof(cExecProcName));
    memset(cExecSrcName, 0x00, sizeof(cExecSrcName));
    memset(cExecBaseSrcName, 0x00, sizeof(cExecBaseSrcName));
    memset(cExecSrcNameMain, 0x00, sizeof(cExecSrcNameMain));
    memset(cExecSrcExtension, 0x00, sizeof(cExecSrcExtension));

    // 前処理
    // 日時情報取得処理
    // 現在日時取得
    m_stTimeData = CLogFile::GetDateTimeProc();

    // マシン日時（ログ出力用）を取得
    m_stLogData.strDateTime = m_stTimeData.strMachDateTime;

    // データログ出力ファイル（更新追記型）をオープン
    // データログ出力ファイルはオープンに失敗した場合出力は諦めて処理を継続
    eLogErrNo = fopen_s(&pfDataLogFile, m_strDataLogFileName.c_str(), "a+");

    // 主処理
    // メッセージをコピー
    strncpy_s(cExecBaseSrcName, sizeof(cExecBaseSrcName), pcExecSrcName, sizeof(cExecBaseSrcName) - 1);
    strncpy_s(cExecProcName, sizeof(cExecProcName), pcExecProcName, sizeof(cExecProcName) - 1);
    strncpy_s(cDataOutputMessage, sizeof(cDataOutputMessage), pcMessage, sizeof(cDataOutputMessage) - 1);

    // 当実行ソースファイルパスを分割（_splitpath_s関数を使用）
    // _splitpath_s関数はファイル名を含んだ完全パス(フルパス)を分解する機能をもつ
    // ここではソースファイル名と拡張子のみを必要とするためソースファイル名と拡張子以外はNULLを指定
    _splitpath_s(
        cExecBaseSrcName, 
        NULL, 
        0, 
        NULL, 
        0, 
        cExecSrcNameMain, 
        sizeof(cExecSrcNameMain), 
        cExecSrcExtension, 
        sizeof(cExecSrcExtension));
    strncpy_s(cExecSrcName, sizeof(cExecSrcName), cExecSrcNameMain, sizeof(cExecSrcName) - 1);
    strncat_s(cExecSrcName, sizeof(cExecSrcName), cExecSrcExtension, sizeof(cExecSrcName) - 1);

    // データログで出力するための文字列を生成
    strDataLogOutputData = m_stLogData.strDateTime + 
        ", " + 
        cExecSrcName + 
        ", " + 
        cExecProcName + 
        ", " + 
        to_string(iExecLineCnt) + 
        ", " + 
        cDataOutputMessage;

    // 後処理
    // データログ出力ファイルをオープンしているかチェック
    if (NULL == pfDataLogFile)
    {
        // データログ出力ファイルオープンに失敗している場合
        // デバッグウィンドウに出力
        // （OutputDebugStringAはC++言語定義）
        OutputDebugStringA(strDataLogOutputData.c_str());
        OutputDebugStringA("\n");
    }
    else
    {
        // データログ出力ファイルオープンに成功している場合
        // データログ出力ファイルに出力
        fprintf(pfDataLogFile, "%s\n", strDataLogOutputData.c_str());

        // デバッグウィンドウに出力
        // （OutputDebugStringAはC++言語定義）
        OutputDebugStringA(strDataLogOutputData.c_str());
        OutputDebugStringA("\n");

        // データログ出力ファイルをクローズ
        // データログ出力ファイルがクローズ出来なくてもメッセージ等出さずに処理終了
        iCloseResult = fclose(pfDataLogFile);
    }

    // 処理終了
    return;
}
