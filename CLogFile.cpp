//============================================
// ���O�o�̓N���X�\�[�X�t�@�C��
// CLogFile.cpp
// 
// �@�\�T�v
// ���O�o�̓t�@�C���ɏ����o�͂���@�\���`����
// 
// �@�\���e
// �P�D�������擾����
// �Q�D���O�o�͐��䏈��
// �R�D�}�V�������擾�����i���O�o�͎g�p�j
// �S�D�g���[�X���O�o�͏���
// �T�D�f�[�^���O�o�͏���
// 
// �⑫����
// �E���@�\�͂��̑��̋@�\�ł��T�|�[�g���\�Ƃ���
// 
// �쐬����
// 2020/9/25  s-saito   �V�K�쐬
// 
// �X�V����
// 
// 
// 
// �o�[�W����
//                     ver.1.0
//============================================
// �Z�L���A�Ή�
#define _CRT_SECURE_NO_WARNINGS

//��������������������������������������������
// �e�w�b�_�[�C���N���[�h��`
//��������������������������������������������
// �C���N���[�h��`
#include "CLogFile.h"

//��������������������������������������������
// �e�@�\�iAPI�j��`
//��������������������������������������������
//********************************************
// �֐����F�f�t�H���g�R���X�g���N�^
// Function Name�FCLogFile
// 
// �����T�v�F
// �N���X�����ɔ������������e���`����
// 
// 
// �⑫����
// �E���݃f�t�H���g�R���X�g���N�^�ł̏����͂Ȃ�
// 
// 
// �쐬����
// 2020/9/25  s-saito   �V�K�쐬
// 
// �X�V����
// 
// 
//********************************************
CLogFile::CLogFile()
{
    // ���݃f�t�H���g�R���X�g���N�^�ł̏����͂Ȃ�
}

//********************************************
// �֐����F�p�����[�^���t���R���X�g���N�^
// Function Name�FCLogFile
// 
// �����T�v�F
// �p�����[�^���Ɋ�Â��ăN���X�𐶐�����
// 
// 
// �⑫����
// 
// 
// �쐬����
// 2020/9/25  s-saito   �V�K�쐬
// 
// �X�V����
// 
// 
//********************************************
CLogFile::CLogFile(
    const string strTraceLogLogFileOpeFileName, 
    const string strDataLogLogFileOpeFileName)
{
    // �Ăяo�����Ŏw�肳�ꂽ���O�o�̓t�@�C�����ݒ�
    m_strTraceLogFileName = strTraceLogLogFileOpeFileName;
    m_strDataLogFileName = strDataLogLogFileOpeFileName;
}

//********************************************
// �֐����F�f�X�g���N�^
// Function Name�F~CLogFile
// 
// �����T�v�F
// �N���X�I���ɔ�����������������`����
// 
// 
// �⑫����
// 
// 
// �쐬����
// 2020/9/25  s-saito   �V�K�쐬
// 
// �X�V����
// 
// 
//********************************************
CLogFile::~CLogFile()
{
    // �����o�ϐ��N���A
    m_strTraceLogFileName.clear();
    m_strDataLogFileName.clear();
    memset(&m_stTimeData, 0x00, sizeof(TIMEDATA));
    memset(&m_stLogData, 0x00, sizeof(LOGINFORMATION));
    memset(m_cLogData, 0x00, sizeof(m_cLogData));
    m_eErrorResultCode = tagErrorCode::SUCCESS;
}

//********************************************
// �֐����F�}�V�������擾�����i���O�o�͎g�p�j
// Function Name�FGetDateTimeLogOutputUseProc
// 
// �����T�v�F
// timespec_get�֐��y��time�֐��Ŏ擾�������Ԃ�n�����ԁi���{���ԁFJST�j�ɕϊ�����
// �N�E���E���E�j���E���E���E�b�E�}�C�N���b���擾����
// 
// �����菇�F
//   �P�D���ԏ��擾�itimespec_get�֐��y��time�֐����g�p�j
//   �Q�D���ԏ�񕶎���ւ̊i�[
//   �R�D�}�V�����t�ݒ�
//   �S�D�}�V�������ݒ�
//   �T�D�}�V�����t�ݒ�i��؂蕶���t���j
//   �U�D�}�V�������ݒ�i��؂蕶���t���j
//   �V�D���O�o�͗p�}�V�������ݒ�
// 
// �����F     �Ȃ�
// �߂�l�F   �������\����
// 
// �⑫����
// �E���@�\�ł�Windows���p��timespec_get�֐��y��time�֐��Ŏ擾�������Ԃ�
//     �n�����ԁi���{���ԁFJST�j�ɕϊ����Ď擾����
//     ���萢�E���ԁiUTC�j�̊��i���UNIX���j�ł�gettimeofday�֐��Ŏ擾����
//     �܂����Ԃ̕ϊ����@���n�����ԁi���{���ԁFlocaltime�j��
//     �O���j�b�W�W�����ԁi���E�W�����ԁFgmtime�j�̂Q��ނ�����
//     �n�����ԁi���{���ԁj�̓O���j�b�W�W�����ԁi���E�W�����ԁj��
//     9���Ԍ�ɒ�߂���
// 
// 
// �쐬����
// 2020/9/25  s-saito   �V�K�쐬
// 
// �X�V����
// 
// 
//********************************************
TIMEDATA CLogFile::GetDateTimeProc(void)
{
    // �ϐ��錾
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

    // ������
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

    // �O����
    // ���ԏ����擾
    iTimespec = timespec_get(&stTimeSpec, TIME_UTC);
    tiTime = time(NULL);

    // ���ɉ����Ď��Ԍ`����ݒ�
    // ���A�v���P�[�V�����ł̎g�p�͓��{���ԁi�n�����ԁj�`������
    // �O���j�b�W�W�����ԁi���E�W�����ԁj�`���̒�`�������Ă���
    // ���̏ꍇ�}�N���Ńr���h������e�𐧌䂷��
#if 0
    // �O���j�b�W�W�����ԁi���E�W�����ԁj�`���ɕϊ�
    pstMTime = gmtime(&stTimeSpec.tv_sec);
    pstTime = gmtime(&tiTime);
#else
    // ���{���ԁi�n�����ԁj�`���ɕϊ�
    pstMTime = localtime(&stTimeSpec.tv_sec);
    pstTime = localtime(&tiTime);
#endif

    // �又��
    // �擾�������ԏ����e�����̕�����Ɋi�[
    // �N
    sprintf_s(cTimeYear, sizeof(cTimeYear), "%04d", pstTime->tm_year + 1900);

    // ��
    sprintf_s(cTimeMonth, sizeof(cTimeMonth), "%02d", pstTime->tm_mon + 1);

    // ��
    sprintf_s(cTimeDay, sizeof(cTimeDay), "%02d", pstTime->tm_mday);

    // �j��
    sprintf_s(cTimeWeek, sizeof(cTimeWeek), "%s", pcDayOfWeek[pstTime->tm_wday]);

    // ��
    sprintf_s(cTimeHour, sizeof(cTimeHour), "%02d", pstTime->tm_hour);

    // ��
    sprintf_s(cTimeMinite, sizeof(cTimeMinite), "%02d", pstTime->tm_min);

    // �b
    sprintf_s(cTimeSecond, sizeof(cTimeSecond), "%02d", pstTime->tm_sec);

    // �}�C�N���b
    sprintf_s(cTimeMsec, sizeof(cTimeMsec), "%06ld", stTimeSpec.tv_nsec / 1000);

    // �㏈��
    // �}�V�����t
    stTimeDataResult.strMachDate = (string)cTimeYear + (string)cTimeMonth + (string)cTimeDay;

    // �}�V������
    stTimeDataResult.strMachTime = (string)cTimeHour + (string)cTimeMinite + (string)cTimeSecond;

    // ��؂蕶������ꂽ�}�V�����t
    stTimeDataResult.strMachDatePlus = (string)cTimeYear +
        (string)"/" +
        (string)cTimeMonth +
        (string)"/" +
        (string)cTimeDay +
        (string)"[" +
        (string)cTimeWeek +
        (string)"]";

    // ��؂蕶������ꂽ�}�V������
    stTimeDataResult.strMachTimePlus = (string)cTimeHour +
        (string)":" +
        (string)cTimeMinite +
        (string)":" +
        (string)cTimeSecond +
        (string)"." +
        (string)cTimeMsec;

    // ���O�o�͗p�}�V������
    stTimeDataResult.strMachDateTime = stTimeDataResult.strMachDatePlus + ", " + stTimeDataResult.strMachTimePlus;

    // �ݒ肵���}�V����������ԋp
    return stTimeDataResult;
}

//********************************************
// �֐����F���O�o�͐��䏈��
// Function Name�FLogOutputControlProc
// 
// �����T�v�F
// �g���[�X���O�o�̓t�@�C�����y�уf�[�^���O�o�̓t�@�C������
// �i�[����Ă��邩�`�F�b�N���Ċi�[����Ă���ꍇ�̂݃��O�o�͂��w������
// 
// �����菇�F
//   �P�D���b�Z�[�W�����[�J���R�s�[
//   �Q�D�t�@�C�����i�[�`�F�b�N
//   �R�D�o�̓��O���ʎq�ɂ��t���O�ݒ�
//   �S�D�g���[�X���O�o�͎w��
//               �E�f�[�^���
//               �E�G���[���
//               �E�G���[���b�Z�[�W���
//               �E�������
//               �E�G���[�ȊO�̃��b�Z�[�W
//   �T�D�f�[�^���O�o�͎w��
//               �E�f�[�^���
// 
// �����F     ���s�\�[�X�t�@�C����
//            ���s�֐���
//            ���s���C����
//            �o�̓��O���ʎq
//            �g���[�X���O���ʎq
//            ���O�o�͗p������
//            �G���[�R�[�h(�ݒ肪�Ȃ��ꍇ��0)
// �߂�l�F   �Ȃ�
// 
// �⑫����
// �E�G���[�R�[�h�̓G���[���b�Z�[�W�̏ꍇ�̂ݏo�͂���
// 
// 
// �쐬����
// 2020/9/25  s-saito   �V�K�쐬
// 
// �X�V����
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
    // �ϐ��錾
    char cExecSrcName[FILENAMESIZE + 1];
    char cExecProcName[APINAME_SIZE + 1];
    char cOutputMessage[MESSAGESIZE + 1];
    int iTraceLogFlag = 0;

    // ������
    memset(cExecSrcName, 0x00, sizeof(cExecSrcName));
    memset(cExecProcName, 0x00, sizeof(cExecProcName));
    memset(cOutputMessage, 0x00, sizeof(cOutputMessage));

    // �O����
    // ���b�Z�[�W���R�s�[
    strncpy_s(cExecSrcName, sizeof(cExecSrcName), pcExecSrcName, sizeof(cExecSrcName) - 1);
    strncpy_s(cExecProcName, sizeof(cExecProcName), pcExecProcName, sizeof(cExecProcName) - 1);
    strncpy_s(cOutputMessage, sizeof(cOutputMessage), pcMessage, sizeof(cOutputMessage) - 1);

    // �又��
    // �o�̓��O���ʎq���`�F�b�N
    if (TRACEOUTPUTLOG == iLogOutputType)
    {
        // �g���[�X���O
        // �g���[�X���O�t���O��ON
        iTraceLogFlag = 1;
    }
    else
    {
        // �f�[�^���O
        // �g���[�X���O�t���O��OFF
        iTraceLogFlag = 0;
    }

    // �㏈��
    // �g���[�X���O�t���O��ON���`�F�b�N
    if (1 == iTraceLogFlag)
    {
        // �g���[�X���O�t���O��ON
        // �g���[�X���O
        // �g���[�X���O���ʎq�Ɋ�Â��g���[�X���O�o�̓t�@�C���Ƀ��b�Z�[�W�o�͎w��
        switch (iLogTraceType)
        {
        case DATAINFORMATION:
            // �f�[�^���
            // �g���[�X���O�o�̓t�@�C���Ƀf�[�^�����o�͎w��
            WriteTraceLogProc(cExecSrcName, cExecProcName, iExecLineCnt, (const char*)LOG_TYPE_DATA, cOutputMessage);
            break;
        case ERRORINFORMATION1:
            // �G���[���
            // �g���[�X���O�o�̓t�@�C���ɃG���[�����o�͎w��
            WriteTraceLogProc(cExecSrcName, cExecProcName, iExecLineCnt, (const char*)LOG_TYPE_ERROR, cOutputMessage, eErrCd);
            break;
        case ERRORINFORMATION2:
            // �G���[���b�Z�[�W���
            // �g���[�X���O�o�̓t�@�C���ɃG���[���b�Z�[�W�����o�͎w��
            WriteTraceLogProc(cExecSrcName, cExecProcName, iExecLineCnt, (const char*)LOG_TYPE_ERROR, cOutputMessage);
            break;
        case SUCCESSINFORMATION:
            // �������
            // �g���[�X���O�o�̓t�@�C���ɐ��������o�͎w��
            WriteTraceLogProc(cExecSrcName, cExecProcName, iExecLineCnt, (const char*)LOG_TYPE_SUCCESS, cOutputMessage);
            break;
        default:
            // ��L�ȊO
            // �C���t�H���[�V����
            // �g���[�X���O�o�̓t�@�C���ɃC���t�H���[�V�������o�͎w��
            WriteTraceLogProc(cExecSrcName, cExecProcName, iExecLineCnt, (const char*)LOG_TYPE_INFORMATION, cOutputMessage);
            break;
        }
    }
    else
    {
        // �g���[�X���O�t���O��OFF
        // �f�[�^���O
        // �f�[�^���O�o�̓t�@�C���Ƀf�[�^�����o�͎w��
        WriteDataLogProc(cExecSrcName, cExecProcName, iExecLineCnt, cOutputMessage);
    }

    // �����I��
    return;
}

//********************************************
// �֐����F�g���[�X���O�o�͏���
// Function Name�FWriteTraceLogProc
// 
// �����T�v�F
// �g���[�X���O�o�̓t�@�C���Ƀ��b�Z�[�W�����o�͂���
// 
// �����菇�F
//   �P�D�g���[�X���O�o�̓t�@�C���I�[�v��
//   �Q�D���b�Z�[�W�����[�J���R�s�[
//   �R�D�G���[�R�[�h�`�F�b�N
//   �S�D�t�@�C���o�́i�G���[���b�Z�[�W�j
//   �T�D�t�@�C���o�́i�G���[�ȊO�̃��b�Z�[�W�j
//   �U�D�g���[�X���O�o�̓t�@�C���N���[�Y
// 
// �����F     ���s�\�[�X�t�@�C����
//            ���s�֐���
//            ���s���C����
//            ���O�o�͎��
//            ���O�o�͗p������
//            �G���[�R�[�h(�ݒ肪�Ȃ��ꍇ��0)
// �߂�l�F   �Ȃ�
// 
// �⑫����
// �E�G���[�R�[�h�̓G���[���b�Z�[�W�̏ꍇ�̂ݏo�͂���
// 
// 
// �쐬����
// 2020/9/25  s-saito   �V�K�쐬
// 
// �X�V����
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
    // �ϐ��錾
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

    // ������
    memset(cOutputMessage, 0x00, sizeof(cOutputMessage));
    memset(cExecProcName, 0x00, sizeof(cExecProcName));
    memset(cExecSrcName, 0x00, sizeof(cExecSrcName));
    memset(cExecBaseSrcName, 0x00, sizeof(cExecBaseSrcName));
    memset(cExecSrcNameMain, 0x00, sizeof(cExecSrcNameMain));
    memset(cExecSrcExtension, 0x00, sizeof(cExecSrcExtension));
    memset(cLogType, 0x00, sizeof(cLogType));

    // �O����
    // ���ݓ����擾
    m_stTimeData = CLogFile::GetDateTimeProc();

    // �}�V�������i���O�o�͗p�j���擾
    m_stLogData.strDateTime = m_stTimeData.strMachDateTime;

    // �g���[�X���O�o�̓t�@�C���i�X�V�ǋL�^�j���I�[�v��
    // �g���[�X���O�o�̓t�@�C���̓I�[�v���Ɏ��s�����ꍇ�o�͂͒��߂ď������p��
    eLogErrNo = fopen_s(&pfTraceLogFile, m_strTraceLogFileName.c_str(), "a+");

    // �又��
    // ���b�Z�[�W���R�s�[
    strncpy_s(cExecBaseSrcName, sizeof(cExecBaseSrcName), pcExecSrcName, sizeof(cExecBaseSrcName) - 1);
    strncpy_s(cExecProcName, sizeof(cExecProcName), pcExecProcName, sizeof(cExecProcName) - 1);
    strncpy_s(cLogType, sizeof(cLogType), pcLogType, sizeof(cLogType) - 1);
    strncpy_s(cOutputMessage, sizeof(cOutputMessage), pcMessage, sizeof(cOutputMessage) - 1);

    // �����s�\�[�X�t�@�C���p�X�𕪊��i_splitpath_s�֐����g�p�j
    // _splitpath_s�֐��̓t�@�C�������܂񂾊��S�p�X(�t���p�X)�𕪉�����@�\������
    // �����ł̓\�[�X�t�@�C�����Ɗg���q�݂̂�K�v�Ƃ��邽�߃\�[�X�t�@�C�����Ɗg���q�ȊO��NULL���w��
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

    // �G���[�R�[�h���`�F�b�N
    if (tagErrorCode::SUCCESS != eErrCd)
    {
        // �G���[�o�͂̏ꍇ
        // �G���[�R�[�h���܂߂���Ԃɂă��O�ŏo�͂��邽�߂̕�����𐶐�
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
        // ���b�Z�[�W�o�͂̏ꍇ
        // �G���[�R�[�h���܂܂Ȃ���Ԃɂă��O�ŏo�͂��邽�߂̕�����𐶐�
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

    // �㏈��
    // �g���[�X���O�o�̓t�@�C�����I�[�v�����Ă��邩�`�F�b�N
    if (NULL == pfTraceLogFile)
    {
        // �g���[�X���O�o�̓t�@�C���I�[�v���Ɏ��s���Ă���ꍇ
        // �f�o�b�O�E�B���h�E�ɏo��
        // �iOutputDebugStringA��C++�����`�j
        OutputDebugStringA(strLogOutputData.c_str());
        OutputDebugStringA("\n");
    }
    else
    {
        // �g���[�X���O�o�̓t�@�C���I�[�v���ɐ������Ă���ꍇ
        // �g���[�X���O�o�̓t�@�C���ɏo��
        fprintf(pfTraceLogFile, "%s\n", strLogOutputData.c_str());

        // �f�o�b�O�E�B���h�E�ɏo��
        // �iOutputDebugStringA��C++�����`�j
        OutputDebugStringA(strLogOutputData.c_str());
        OutputDebugStringA("\n");

        // �g���[�X���O�o�̓t�@�C�����N���[�Y
        // �g���[�X���O�o�̓t�@�C�����N���[�Y�o���Ȃ��Ă����b�Z�[�W���o�����ɏ����I��
        iCloseResult = fclose(pfTraceLogFile);
    }

    // �����I��
    return;
}

//********************************************
// �֐����F�f�[�^���O�o�͏���
// Function Name�FWriteDataLogProc
// 
// �����T�v�F
// �f�[�^���O�o�̓t�@�C���Ƀ��b�Z�[�W�����o�͂���
// 
// �����菇�F
//   �P�D�f�[�^���O�o�̓t�@�C���I�[�v��
//   �Q�D���b�Z�[�W�����[�J���R�s�[
//   �R�D�t�@�C���o��
//   �S�D�f�[�^���O�o�̓t�@�C���N���[�Y
// 
// �����F	 ���s�\�[�X�t�@�C����
//           ���s�֐���
//           ���s���C����
//           ���O�o�͗p������
// �߂�l�F  �Ȃ�
// 
// �⑫����
// 
// 
// �쐬����
// 2020/9/25  s-saito   �V�K�쐬
// 
// �X�V����
// 
// 
//********************************************
void CLogFile::WriteDataLogProc(
	const char *pcExecSrcName, 
	const char *pcExecProcName, 
	int iExecLineCnt, 
	const char *pcMessage)
{
    // �ϐ��錾
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

    // ������
    memset(cDataOutputMessage, 0x00, sizeof(cDataOutputMessage));
    memset(cExecProcName, 0x00, sizeof(cExecProcName));
    memset(cExecSrcName, 0x00, sizeof(cExecSrcName));
    memset(cExecBaseSrcName, 0x00, sizeof(cExecBaseSrcName));
    memset(cExecSrcNameMain, 0x00, sizeof(cExecSrcNameMain));
    memset(cExecSrcExtension, 0x00, sizeof(cExecSrcExtension));

    // �O����
    // �������擾����
    // ���ݓ����擾
    m_stTimeData = CLogFile::GetDateTimeProc();

    // �}�V�������i���O�o�͗p�j���擾
    m_stLogData.strDateTime = m_stTimeData.strMachDateTime;

    // �f�[�^���O�o�̓t�@�C���i�X�V�ǋL�^�j���I�[�v��
    // �f�[�^���O�o�̓t�@�C���̓I�[�v���Ɏ��s�����ꍇ�o�͂͒��߂ď������p��
    eLogErrNo = fopen_s(&pfDataLogFile, m_strDataLogFileName.c_str(), "a+");

    // �又��
    // ���b�Z�[�W���R�s�[
    strncpy_s(cExecBaseSrcName, sizeof(cExecBaseSrcName), pcExecSrcName, sizeof(cExecBaseSrcName) - 1);
    strncpy_s(cExecProcName, sizeof(cExecProcName), pcExecProcName, sizeof(cExecProcName) - 1);
    strncpy_s(cDataOutputMessage, sizeof(cDataOutputMessage), pcMessage, sizeof(cDataOutputMessage) - 1);

    // �����s�\�[�X�t�@�C���p�X�𕪊��i_splitpath_s�֐����g�p�j
    // _splitpath_s�֐��̓t�@�C�������܂񂾊��S�p�X(�t���p�X)�𕪉�����@�\������
    // �����ł̓\�[�X�t�@�C�����Ɗg���q�݂̂�K�v�Ƃ��邽�߃\�[�X�t�@�C�����Ɗg���q�ȊO��NULL���w��
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

    // �f�[�^���O�ŏo�͂��邽�߂̕�����𐶐�
    strDataLogOutputData = m_stLogData.strDateTime + 
        ", " + 
        cExecSrcName + 
        ", " + 
        cExecProcName + 
        ", " + 
        to_string(iExecLineCnt) + 
        ", " + 
        cDataOutputMessage;

    // �㏈��
    // �f�[�^���O�o�̓t�@�C�����I�[�v�����Ă��邩�`�F�b�N
    if (NULL == pfDataLogFile)
    {
        // �f�[�^���O�o�̓t�@�C���I�[�v���Ɏ��s���Ă���ꍇ
        // �f�o�b�O�E�B���h�E�ɏo��
        // �iOutputDebugStringA��C++�����`�j
        OutputDebugStringA(strDataLogOutputData.c_str());
        OutputDebugStringA("\n");
    }
    else
    {
        // �f�[�^���O�o�̓t�@�C���I�[�v���ɐ������Ă���ꍇ
        // �f�[�^���O�o�̓t�@�C���ɏo��
        fprintf(pfDataLogFile, "%s\n", strDataLogOutputData.c_str());

        // �f�o�b�O�E�B���h�E�ɏo��
        // �iOutputDebugStringA��C++�����`�j
        OutputDebugStringA(strDataLogOutputData.c_str());
        OutputDebugStringA("\n");

        // �f�[�^���O�o�̓t�@�C�����N���[�Y
        // �f�[�^���O�o�̓t�@�C�����N���[�Y�o���Ȃ��Ă����b�Z�[�W���o�����ɏ����I��
        iCloseResult = fclose(pfDataLogFile);
    }

    // �����I��
    return;
}
