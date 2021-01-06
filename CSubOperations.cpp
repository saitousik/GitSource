//============================================
// ���ʑ���N���X�\�[�X�t�@�C��
// CSubOperations.cpp
// 
// �@�\�T�v
// ���̃��C�����쓙���T�|�[�g����@�\���`����
// 
// �@�\���e
// �P�D�}���`�o�C�g�����񃏃C�h������ϊ�����
// �Q�D���C�h������}���`�o�C�g������ϊ�����
// �R�D�R�}���h���C��������͏���
// �S�D���ϐ��擾����
// �T�D�}�V�������擾����
// �U�D�C�x���g���b�Z�[�W��������
// �V�D�w�蕶���폜����
// �W�D�w�蕶���񕪊�����
// �X�D�G���[���b�Z�[�W�擾����
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
#include "CSubOperations.h"
#include "CLogFile.h"

// �O���[�o���ϐ��錾
CLogFile g_clsSubOpeLogFile;

// �}�N����`
#define LOGOUTPUT(iOutputType, iTraceOutputLogType, cMessageInfo) \
    g_clsSubOpeLogFile.LogOutputControlProc(LOGARGS, iOutputType, iTraceOutputLogType, cMessageInfo)
#define LOGOUTPUTERROR(iOutputType, iTraceOutputLogType, cErrorMessageInfo, eErrorInfoCode) \
    g_clsSubOpeLogFile.LogOutputControlProc(LOGARGS, iOutputType, iTraceOutputLogType, cErrorMessageInfo, eErrorInfoCode)

//��������������������������������������������
// �e�@�\�iAPI�j��`
//��������������������������������������������
//********************************************
// �֐����F�f�t�H���g�R���X�g���N�^
// Function Name�FCSubOperations
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
CSubOperations::CSubOperations()
{
    // ���݃f�t�H���g�R���X�g���N�^�ł̏����͂Ȃ�
}

//********************************************
// �֐����F�p�����[�^���t���R���X�g���N�^
// Function Name�FCSubOperations
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
CSubOperations::CSubOperations(
    const string strTraceLogFileName, 
    const string strDataLogFileName)
{
    // ���O�o�̓N���X����
    g_clsSubOpeLogFile = CLogFile(strTraceLogFileName, strDataLogFileName);

    // �Ăяo�����Ŏw�肳�ꂽ���O�o�̓t�@�C�����ݒ�
    m_strSubOpeTraceLogFileName = strTraceLogFileName;
    m_strSubOpeDataLogFileName = strDataLogFileName;
}

//********************************************
// �֐����F�f�X�g���N�^
// Function Name�F~CSubOperations
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
CSubOperations::~CSubOperations()
{
    // ���O�o�̓t�@�C�����ϐ��N���A
    m_strSubOpeTraceLogFileName.clear();
    m_strSubOpeDataLogFileName.clear();
}

//********************************************
// �֐����F���ϐ��擾����
// Function Name�FGetEnvironmentDataProc
// 
// �����T�v�F
// �}�V���́w�V�X�e���̊��ϐ��x�ɐݒ肵�������擾����
// 
// �����菇�F
//   �P�D���ϐ��擾
// 
// �����F      ���ϐ���
//             �擾�������ϐ����
// �߂�l�F    ���s���ʁi����I����stdlib.h�Œ�`���Ă�����e�𗘗p�j
//             ����I���FEXIT_SUCCESS�i0�j
//             �ُ�I���F���ϐ��擾�G���[�FCM_ENVERROR�i101�j
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
int CSubOperations::GetEnvironmentDataProc(
    const char* pcEnvType,
    char* pcGetEnvData)
{
    // �ϐ��錾
    char* pcEnvData = NULL;
    int iEnvResult = EXIT_SUCCESS;

    // �O����
    // �������擾����
    // ���ϐ��擾�J�n�����擾
    m_stTimeData = g_clsSubOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "���ϐ��擾�J�n�����F%s %s", 
        (char *)m_stTimeData.strMachDatePlus.c_str(), 
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���ϐ��擾�J�n*****");
    m_stLogData = CreateEventMessageProc(ACTIONSET, "���ϐ���");

    // �又��
    // �Ăяo�����Ŏw�肳�ꂽ���ϐ����Ɋ�Â��Ċ��ϐ����擾
    pcEnvData = getenv(pcEnvType);
    sprintf_s(m_cLogData, sizeof(m_cLogData), "�擾����%s���́F%s", m_stLogData.pcActionName, pcEnvData);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    // �擾���ʂ��`�F�b�N
    if (NULL != pcEnvData)
    {
        // ���ϐ��̐ݒ肠��
        // �擾�������ϐ����R�s�[
        strncpy_s(pcGetEnvData, FILENAMESIZE + 1, pcEnvData, FILENAMESIZE);
        sprintf_s(m_cLogData, sizeof(m_cLogData), "%s���́F%s", m_stLogData.pcActionName, pcGetEnvData);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
        LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, (char*)"***** ���ϐ��擾����I�� *****");
    }
    else
    {
        // ���ϐ��̐ݒ�Ȃ�
        // �G���[�R�[�h��ݒ�
        iEnvResult = CM_ENVERROR;
        sprintf_s(m_cLogData, sizeof(m_cLogData), "�߂�l�́F%d", iEnvResult);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
        LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, (char*)"***** ���ϐ��擾�ُ�I�� *****");
    }

    // �㏈��
    // �������擾����
    // ���ϐ��擾�I�������擾
    m_stTimeData = g_clsSubOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "���ϐ��擾�I�������F%s %s", 
        (char *)m_stTimeData.strMachDatePlus.c_str(), 
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);

    // �擾���ʂ�ԋp
    return iEnvResult;
}

//********************************************
// �֐����F�}�V�������擾����
// Function Name�FGetDateTimeProc
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
TIMEDATA CSubOperations::GetDateTimeProc(void)
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
    char* pcDayOfWeek[] = { (char *)"Sun",
                           (char *)"Mon",
                           (char *)"Tue",
                           (char *)"Wed",
                           (char *)"Thu",
                           (char *)"Fri",
                           (char *)"Sat" };
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

    // �}�V�����t
    stTimeDataResult.strMachDate = (const string)cTimeYear + (const string)cTimeMonth + (const string)cTimeDay;

    // �}�V������
    stTimeDataResult.strMachTime = (const string)cTimeHour + (const string)cTimeMinite + (const string)cTimeSecond;

    // ��؂蕶������ꂽ�}�V�����t
    stTimeDataResult.strMachDatePlus = (const string)cTimeYear +
        (const string)"/" +
        (const string)cTimeMonth +
        (const string)"/" +
        (const string)cTimeDay +
        (const string)"[" +
        (const string)cTimeWeek +
        (const string)"]";

    // ��؂蕶������ꂽ�}�V������
    stTimeDataResult.strMachTimePlus = (const string)cTimeHour +
        (const string)":" +
        (const string)cTimeMinite +
        (const string)":" +
        (const string)cTimeSecond +
        (const string)"." +
        (const string)cTimeMsec;

    // ���O�o�͗p�}�V������
    stTimeDataResult.strMachDateTime = stTimeDataResult.strMachDatePlus + ", " + stTimeDataResult.strMachTimePlus;

    // �擾���e�����O�֏o��

    // �J�n���
    sprintf_s(
        m_cLogData,
        sizeof(m_cLogData),
        "�}�V�������擾�J�n�����F%s %s",
        (char *)stTimeDataResult.strMachDatePlus.c_str(),
        (char *)stTimeDataResult.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �}�V�������擾�����J�n *****");

    // timespec_get�֐��Ŏ擾��������
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, "***** �������\���̂̏�� *****");
    sprintf_s(m_cLogData, sizeof(m_cLogData), "���ԏ��itimespec_get�j�F%lld %09ld", stTimeSpec.tv_sec, stTimeSpec.tv_nsec);
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

    // timespec_get�֐��Ŏ擾���������̓������
    // �ʏ��
    // �N
    sprintf_s(m_cLogData, sizeof(m_cLogData), "�N�F%s", cTimeYear);
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

    // ��
    sprintf_s(m_cLogData, sizeof(m_cLogData), "���F%s", cTimeMonth);
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

    // ��
    sprintf_s(m_cLogData, sizeof(m_cLogData), "���F%s", cTimeDay);
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

    // �j��
    sprintf_s(m_cLogData, sizeof(m_cLogData), "�j���i���l�j�F%d", pstTime->tm_wday);
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
    sprintf_s(m_cLogData, sizeof(m_cLogData), "�j���i������j�F%s", cTimeWeek);
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

    // ��
    sprintf_s(m_cLogData, sizeof(m_cLogData), "���F%s", cTimeHour);
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

    // ��
    sprintf_s(m_cLogData, sizeof(m_cLogData), "���F%s", cTimeMinite);
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

    // �b
    sprintf_s(m_cLogData, sizeof(m_cLogData), "�b�F%s", cTimeSecond);
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

    // �}�C�N���b
    sprintf_s(m_cLogData, sizeof(m_cLogData), "�}�C�N���b�F%s", cTimeMsec);
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

    // ������������
    // �}�V�����t
    sprintf_s(m_cLogData, sizeof(m_cLogData), "�}�V�����t�F%s", (char *)stTimeDataResult.strMachDate.c_str());
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

    // �}�V������
    sprintf_s(m_cLogData, sizeof(m_cLogData), "�}�V�������F%s", (char *)stTimeDataResult.strMachTime.c_str());
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

    // ��؂蕶������ꂽ�}�V�����t
    sprintf_s(m_cLogData, sizeof(m_cLogData), "��؂蕶���t���}�V�����t�F%s", (char *)stTimeDataResult.strMachDatePlus.c_str());
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

    // ��؂蕶������ꂽ�}�V������
    sprintf_s(m_cLogData, sizeof(m_cLogData), "��؂蕶���t���}�V�������F%s", (char *)stTimeDataResult.strMachTimePlus.c_str());
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

    // ���O�o�͗p�}�V������
    sprintf_s(m_cLogData, sizeof(m_cLogData), "���O�o�͗p�}�V�������F%s", (char *)stTimeDataResult.strMachDateTime.c_str());
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

    // �I�����
    sprintf_s(
        m_cLogData,
        sizeof(m_cLogData),
        "�}�V�������擾�I�������F%s %s",
        (char *)stTimeDataResult.strMachDatePlus.c_str(),
        (char *)stTimeDataResult.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �}�V�������擾�����I�� *****");

    // �ݒ肵���}�V����������ԋp
    return stTimeDataResult;
}

//********************************************
// �֐����F�C�x���g���b�Z�[�W��������
// Function Name�FCreateEventMessageProc
// 
// �����T�v�F
// �����̔����ɔ����������������̓t�@�C�����A�G���[����
// �\���o�͗p�̃��b�Z�[�W�ɐ�������
// 
// �����菇�F
//   �P�D�G���[�R�[�h�Ɋ�Â������b�Z�[�W�擾
//   �Q�D���������������������̓t�@�C�����ƌ���
// 
// �����F     ���b�Z�[�W�����^�C�v
//            ���������������������̓t�@�C����
//            �G���[�R�[�h�i�ݒ肪�Ȃ��ꍇ��0�j
// �߂�l�F   ���O�o�͏��\����
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
LOGINFORMATION CSubOperations::CreateEventMessageProc(
    int iEventType,
    const char* pcActionName,
    ERRORCODETABLE eErr)
{
    // �ϐ��錾
    LOGINFORMATION stEventData;
    char cActionName[FILENAMESIZE + 1];
    char cMessage[MESSAGESIZE + 1];
    const char* pcErrorMessage = "";

    // ������
    memset(&stEventData, 0x00, sizeof(stEventData));
    memset(cActionName, 0x00, sizeof(cActionName));
    memset(cMessage, 0x00, sizeof(cMessage));

    // �O����
    // �������擾����
    // �C�x���g���b�Z�[�W�����J�n�����擾
    m_stTimeData = g_clsSubOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "�C�x���g���b�Z�[�W�����J�n�����F%s %s", 
        (char *)m_stTimeData.strMachDatePlus.c_str(), 
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �C�x���g���b�Z�[�W���������J�n *****");

    // �又��
    // ���b�Z�[�W�����^�C�v���`�F�b�N
    if (ACTIONSET == iEventType)
    {
        // �����������������͔����t�@�C����������I��
        // �����������������͔����t�@�C�����܂ޕ������擾
        strncpy_s(cActionName, sizeof(cActionName), pcActionName, sizeof(cActionName) - 1);
        string strActionNameAfter(cActionName, sizeof(cActionName) / sizeof(cActionName[0]));
        stEventData.strActionName = strActionNameAfter;
        sprintf_s(
            m_cLogData, 
            sizeof(m_cLogData), 
            "�����������������̓t�@�C�������܂ޕ����́F%s", 
            stEventData.strActionName.c_str());
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

        // �������́^�t�@�C�����̃|�C���^�̃A�h���X��ݒ�
        stEventData.pcActionName = (char*)stEventData.strActionName.c_str();
        sprintf_s(
            m_cLogData, 
            sizeof(m_cLogData), 
            "�����������������̓t�@�C�������܂ޕ����̃|�C���^�́F%s", 
            stEventData.pcActionName);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
    }
    else
    {
        // �G���[���b�Z�[�W������I��
        // �p�����[�^����ݒ�
        stEventData.eErrorCode = eErr;
        sprintf_s(m_cLogData, sizeof(m_cLogData), "�G���[�R�[�h�́F%d", stEventData.eErrorCode);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
        strncpy_s(cActionName, sizeof(cActionName), pcActionName, sizeof(cActionName) - 1);
        sprintf_s(m_cLogData, sizeof(m_cLogData), "���������������������̓t�@�C�����́F%s", cActionName);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

        // �G���[���b�Z�[�W�擾����
        // �G���[�R�[�h�Ɋ�Â��G���[���b�Z�[�W���擾
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, (char*)"***** �G���[���b�Z�[�W�擾�֐��Ăяo�� *****");
        pcErrorMessage = GetErrorMessageProc(stEventData.eErrorCode);
        sprintf_s(m_cLogData, sizeof(m_cLogData), "�G���[���b�Z�[�W�́F%s", pcErrorMessage);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

        // �\���o�͗p�G���[���b�Z�[�W����
        // �t�@�C���֌W�Ŕ��������G���[���`�F�b�N
        if (tagErrorCode::FILEOPENERROR == stEventData.eErrorCode ||
            tagErrorCode::FILECLOSEERROR == stEventData.eErrorCode ||
            tagErrorCode::COPYERROR == stEventData.eErrorCode ||
            tagErrorCode::RENAMEERROR == stEventData.eErrorCode ||
            tagErrorCode::DELETEERROR == stEventData.eErrorCode ||
            tagErrorCode::STATERROR == stEventData.eErrorCode)
        {
            // �t�@�C���֌W�Ŕ��������G���[
            // ���������t�@�C�������܂߂��G���[���b�Z�[�W�𐶐�
            sprintf_s(cMessage, sizeof(cMessage), "%s �ɑ΂���%s", cActionName, GetErrorMessageProc(stEventData.eErrorCode));
            string strMessageAfter(cMessage, sizeof(cMessage) / sizeof(cMessage[0]));
            stEventData.strMessage = strMessageAfter;
        }
        else
        {
            // �t�@�C���֌W�ȊO�Ŕ��������G���[
            // �����������������܂߂����b�Z�[�W�𐶐�
            sprintf_s(cMessage, sizeof(cMessage), "%s%s", cActionName, pcErrorMessage);
            string strMessageAfter(cMessage, sizeof(cMessage) / sizeof(cMessage[0]));
            stEventData.strMessage = strMessageAfter;
        }

        // ���������C�x���g���b�Z�[�W�����O�ɏo��
        sprintf_s(m_cLogData, sizeof(m_cLogData), "������̃C�x���g���b�Z�[�W�́F%s", stEventData.strMessage.c_str());
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

        // ���b�Z�[�W�|�C���^�̃A�h���X��ݒ�
        stEventData.pcMessage = (char*)stEventData.strMessage.c_str();
        sprintf_s(m_cLogData, sizeof(m_cLogData), "�C�x���g���b�Z�[�W�|�C���^�́F%s", stEventData.pcMessage);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
    }

    // �㏈��
    // �������擾����
    // �C�x���g���b�Z�[�W�����I�������擾
    m_stTimeData = g_clsSubOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "�C�x���g���b�Z�[�W�����I�������F%s %s", 
        (char *)m_stTimeData.strMachDatePlus.c_str(), 
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �C�x���g���b�Z�[�W���������I�� *****");

    // ���O�o�͏��\���̂̃��b�Z�[�W����ԋp
    return stEventData;
}

//********************************************
// �֐����F�w�蕶���폜����
// Function Name�FDeleteDesignationCharProc
// 
// �����T�v�F
// �����񂩂�w�蕶������������
// �w�蕶�����O����������𐶐�����
// 
// �����菇�F
//   �P�D�w�蕶������
//   �Q�D�w�蕶�����O���������ݒ�
// 
// �����F      �w�蕶�����ݕ�����
//             �w�蕶��
//             �w�蕶�����O����������
// �߂�l�F    �Ȃ�
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
void CSubOperations::DeleteDesignationCharProc(
    const char* pcEditData,
    const char cDesignationChar,
    char* pcConvertResult)
{
    // �ϐ��錾
    char cEditBeforeData[DATASIZE + 1];
    char cEditAfterData[DATASIZE + 1];
    size_t siReadCnt = 0;
    size_t siWriteCnt = 0;
    size_t siEditDataLength = 0;

    // ������
    memset(cEditBeforeData, 0x00, sizeof(cEditBeforeData));
    memset(cEditAfterData, 0x00, sizeof(cEditAfterData));

    // �O����
    // �������擾����
    // �w�蕶���폜�J�n�����擾
    m_stTimeData = g_clsSubOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData,
        sizeof(m_cLogData),
        "�w�蕶���폜�J�n�����F%s %s",
        (char *)m_stTimeData.strMachDatePlus.c_str(),
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �w�蕶���폜�����J�n *****");

    // �p�����[�^�����R�s�[
    strncpy_s(cEditBeforeData, sizeof(cEditBeforeData), pcEditData, sizeof(cEditBeforeData) - 1);

    // �w�蕶�����ݕ�����̒������擾
    siEditDataLength = strlen(cEditBeforeData);
    sprintf_s(m_cLogData, sizeof(m_cLogData), "�����́F%zu", siEditDataLength);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    // �又��
    // �w�蕶�����ݕ�����̒��������[�v
    for (siReadCnt = 0; siReadCnt < siEditDataLength; siReadCnt++)
    {
        // �w�蕶�����ݕ�����̕������w�蕶�����`�F�b�N
        if (cDesignationChar == cEditBeforeData[siReadCnt])
        {
            // �`�F�b�N���Ă���w�蕶�����ݕ�����̕������w�蕶��
            // �w�蕶���𖳎�
            // �X�L�b�v���ă��[�v��
            continue;
        }
        else
        {
            // �`�F�b�N���Ă���w�蕶�����ݕ�����̕������w�蕶���ȊO
            // �`�F�b�N���Ă���w�蕶�����ݕ�����̕�����ݒ�
            cEditAfterData[siWriteCnt] = cEditBeforeData[siReadCnt];

            // �ݒ�ʒu���C���N�������g
            siWriteCnt++;
        }
    }

    // �㏈��
    // �w�蕶�����O������������i�[
    cEditAfterData[strlen(cEditAfterData)] = '\0';
    siEditDataLength = strlen(cEditAfterData);
    strncpy_s(pcConvertResult, DATASIZE + 1, cEditAfterData, DATASIZE);
    pcConvertResult[siEditDataLength] = '\0';
    sprintf_s(m_cLogData, sizeof(m_cLogData), "�폜��̌��ʂ́F%s", pcConvertResult);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    // �������擾����
    // �w�蕶���폜�I�������擾
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �w�蕶���폜�����I�� *****");
    m_stTimeData = g_clsSubOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData,
        sizeof(m_cLogData),
        "�w�蕶���폜�I�������F%s %s",
        (char *)m_stTimeData.strMachDatePlus.c_str(),
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
}

//********************************************
// �֐����F�w�蕶���񕪊�����
// Function Name�FSplitDataProc
// 
// �����T�v�F
// �w�蕶���񂩂�w�蕶������������
// ������𕪊�����
// 
// �����菇�F
//   �P�D���o�͉\�f�[�^�ϊ�
//   �Q�D�w�蕶���񕪊�
// 
// �����F      �w�蕶����
//             �w�蕶��
// �߂�l�F    ��������
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
deque<string> CSubOperations::SplitDataProc(
    const string strSplitData,
    const char cChar)
{
    // �ϐ��錾
    deque<string> deqSplitData;
    string strSplitBuffer;

    // �O����
    // �������擾����
    // �w�蕶���񕪊��J�n�����擾
    m_stTimeData = g_clsSubOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData,
        sizeof(m_cLogData),
        "�w�蕶���񕪊��J�n�����F%s %s",
        (char *)m_stTimeData.strMachDatePlus.c_str(),
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �w�蕶���񕪊������J�n *****");

    // ���o�͉\�ȃf�[�^�ɕϊ�
    stringstream ssSplitData(strSplitData);

    // �又��
    // ������̒��������[�v
    // �igetline��C++�����`�j
    while (getline(ssSplitData, strSplitBuffer, cChar))
    {
        // ���o���ꂽ��������i�[
        deqSplitData.push_back(strSplitBuffer);
    }

    // �㏈��
    // �i�[�s�������[�v
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, "***** ������̕������� *****");
    for (size_t siCnt = 0; siCnt < deqSplitData.size(); siCnt++)
    {
        // �i�[��������������O�ɏo��
        sprintf_s(m_cLogData, sizeof(m_cLogData), "%zu �ڂ̕�����́F%s", siCnt + 1, deqSplitData[siCnt].c_str());
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
    }

    // �������擾����
    // �w�蕶���폜�I�������擾
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �w�蕶���񕪊������I�� *****");
    m_stTimeData = g_clsSubOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData,
        sizeof(m_cLogData),
        "�w�蕶���񕪊��I�������F%s %s",
        (char *)m_stTimeData.strMachDatePlus.c_str(),
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);

    // �������ʂ�ԋp
    return deqSplitData;
}

//********************************************
// �֐����F�}���`�o�C�g�����񃏃C�h������ϊ�����
// Function Name�FMultiToWideProc
// 
// �����T�v�F
// �}���`�o�C�g������ɑ΂��ă��C�h������ɕϊ�����
// 
// �����菇�F
//   �P�D�������̈�m��
//   �Q�D�}���`�o�C�g�����񂩂烏�C�h������ɕϊ�
// 
// �����F	 �}���`�o�C�g������
// �߂�l�F  ���C�h������
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
wstring CSubOperations::MultiToWideProc(
    string strMultiByte)
{
    // �ϐ��錾
    wstring wstrWideCharBuff;
    WCHAR* pcwWideCharBuff = NULL;
    size_t siRet = 0;
    int iDestsize = 0;
    int iChangeLen = 0;
    int iBuffSize = 0;

    // �W���o�͂ɓ��{�ꂪ�\�������悤�Ƀ��P�[������{�ɐݒ�
    setlocale(LC_ALL, "japanese");

    // �������̈�m�ۂɎ��s�����ꍇ��NULL�ݒ�
    // ���̐ݒ�ɂ��m�ۂɎ��s�����ꍇ��NULL��Ԃ��悤�ɂ���
    set_new_handler(NULL);

    // �O����
    // �������擾����
    // �}���`�o�C�g�����񃏃C�h������ϊ��J�n�����擾
    m_stTimeData = g_clsSubOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData,
        sizeof(m_cLogData),
        "�}���`�o�C�g�����񃏃C�h������ϊ��J�n�����F%s %s",
        (char*)m_stTimeData.strMachDatePlus.c_str(),
        (char*)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �}���`�o�C�g�����񃏃C�h������ϊ������J�n *****");

    // ���P�[����ݒ�
    setlocale(LC_CTYPE, "jpn");

    // �}���`�o�C�g�����񂩂烏�C�h������ɕϊ������Ƃ��̕����񒷎擾
    // �iMultiByteToWideChar��C++�����`�j
    iChangeLen = MultiByteToWideChar(CP_ACP, 0, strMultiByte.c_str(), -1, NULL, 0);
    sprintf_s(m_cLogData, sizeof(m_cLogData), "�}���`�o�C�g������́F%s", strMultiByte.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
    sprintf_s(m_cLogData, sizeof(m_cLogData), "���C�h������ւ̕ϊ��ɕK�v�Ȓ����́F%d", iChangeLen);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    // �����񒷂��擾�o�������`�F�b�N
    if (0 == iChangeLen)
    {
        // �����񒷂��擾�o���Ȃ�����
        // �����񒷎擾���s���b�Z�[�W�����O�ɏo��
        LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** �����񒷂̎擾���s(�B>�t<) *****");

        // NULL��ݒ�
        pcwWideCharBuff = NULL;
        goto END;
    }

    // �又��
    try
    {
        // �}���`�o�C�g�����񃏃C�h������ϊ��������̈���m��
        iBuffSize = iChangeLen + 1;
        pcwWideCharBuff = DBG_NEW WCHAR[iBuffSize];
        m_stLogData = CreateEventMessageProc(ACTIONSET, "�}���`�o�C�g�����񃏃C�h������ϊ���");

        // �������̈悪�m�ۏo�������`�F�b�N
        if (NULL == pcwWideCharBuff)
        {
            // �������̈�̊m�ێ��s
            // ��O�����փX���[
            throw tagErrorCode::MEMORYERROR;
        }

        // �������̏�����
        memset(pcwWideCharBuff, 0x00, iBuffSize);

        // �}���`�o�C�g�����񃏃C�h������ϊ��������̈�m�ې����̃��b�Z�[�W�����O�ɏo��
        LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, "***** �}���`�o�C�g�����񃏃C�h������ϊ��������̈�m�ې���I�� *****");
        sprintf_s(m_cLogData, sizeof(m_cLogData), "%s��%s", m_stLogData.pcActionName, MEMORYCREATESUCCESS);
        LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, m_cLogData);

        // �}���`�o�C�g�����񂩂烏�C�h������ɕϊ�
        // �imbstowcs_s��C++�����`�j
        mbstowcs_s(&siRet, pcwWideCharBuff, iBuffSize, strMultiByte.c_str(), _TRUNCATE);
        sprintf_s(m_cLogData, sizeof(m_cLogData), "���C�h������́F%ls", pcwWideCharBuff);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
        sprintf_s(m_cLogData, sizeof(m_cLogData), "���C�h������̒����́F%zu", siRet);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

        // �ϊ����o�������`�F�b�N
        if (iChangeLen != siRet)
        {
            // �ϊ����s
            // �ϊ����s���b�Z�[�W�����O�ɏo��
            LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** �}���`�o�C�g�����񂩂�̃��C�h������ϊ����s(�B>�t<) *****");

            // �������̈�����
            // �K��delete���邱�ƁI
            SAFE_FREE_NEWALLOC_ARRAY(pcwWideCharBuff);
            goto END;
        }
        else
        {
            // �ϊ�����
            // �ϊ����ꂽ���C�h����������O�ɏo��
            wstrWideCharBuff = (const wstring)pcwWideCharBuff;
            sprintf_s(m_cLogData, sizeof(m_cLogData), "���C�h������́F%ws", wstrWideCharBuff.c_str());
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

            // �������̈�����
            // �K��delete���邱�ƁI
            SAFE_FREE_NEWALLOC_ARRAY(pcwWideCharBuff);
        }
    }

    // ��O����
    catch (const ERRORCODETABLE eAllocErr)
    {
        // �G���[���b�Z�[�W��������
        // �G���[���b�Z�[�W���o��
        pcwWideCharBuff = NULL;
        LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** �}���`�o�C�g�����񃏃C�h������ϊ��������̈�m�ۈُ�I�� *****");
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �G���[���b�Z�[�W�����֐��Ăяo�� *****");
        m_eErrorResultCode = eAllocErr;
        m_stLogData = CreateEventMessageProc(ERRORSET, m_stLogData.pcActionName, m_eErrorResultCode);
        LOGOUTPUTERROR(TRACEOUTPUTLOG, ERRORINFORMATION1, m_stLogData.pcMessage, m_stLogData.eErrorCode);
        sprintf_s(m_cLogData, sizeof(m_cLogData), "�߂�l�́F%d", m_stLogData.eErrorCode);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
    }

END:
    // �㏈��
    // �������擾����
    // �}���`�o�C�g�����񃏃C�h������ϊ��I�������擾
    m_stTimeData = g_clsSubOpeLogFile.GetDateTimeProc();
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �}���`�o�C�g�����񃏃C�h������ϊ������I�� *****");
    sprintf_s(
        m_cLogData,
        sizeof(m_cLogData),
        "�}���`�o�C�g�����񃏃C�h������ϊ��I�������F%s %s",
        (char*)m_stTimeData.strMachDatePlus.c_str(),
        (char*)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);

    // �ϊ���̕������ԋp
    return wstrWideCharBuff;
}

//********************************************
// �֐����F���C�h������}���`�o�C�g������ϊ�����
// Function Name�FWideToMultiProc
// 
// �����T�v�F
// ���C�h������ɑ΂��ă}���`�o�C�g������ɕϊ�����
// 
// �����菇�F
//   �P�D�������̈�m��
//   �Q�D���C�h�����񂩂�}���`�o�C�g������ɕϊ�
// 
// �����F	 ���C�h������
// �߂�l�F  �}���`�o�C�g������
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
string CSubOperations::WideToMultiProc(
    wstring wstrWideChar)
{
    // �ϐ��錾
    string strMultiByteBuff;
    char* pcMultiByteBuff = NULL;
    int iChangeLen = 0;
    size_t siRet = 0;
    int iBuffSize = 0;

    // �W���o�͂ɓ��{�ꂪ�\�������悤�Ƀ��P�[������{�ɐݒ�
    setlocale(LC_ALL, "japanese");

    // �������̈�m�ۂɎ��s�����ꍇ��NULL�ݒ�
    // ���̐ݒ�ɂ��m�ۂɎ��s�����ꍇ��NULL��Ԃ��悤�ɂ���
    set_new_handler(NULL);

    // �O����
    // �������擾����
    // ���C�h������}���`�o�C�g������ϊ��J�n�����擾
    m_stTimeData = g_clsSubOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData,
        sizeof(m_cLogData),
        "���C�h������}���`�o�C�g������ϊ��J�n�����F%s %s",
        (char*)m_stTimeData.strMachDatePlus.c_str(),
        (char*)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���C�h������}���`�o�C�g������ϊ������J�n *****");

    // ���P�[����ݒ�
    setlocale(LC_CTYPE, "jpn");

    // ���C�h�����񂩂�}���`�o�C�g������ɕϊ������Ƃ��̕����񒷎擾
    // �iWideCharToMultiByte��C++�����`�j
    iChangeLen = WideCharToMultiByte(CP_ACP, 0, wstrWideChar.c_str(), -1, NULL, 0, NULL, NULL);
    sprintf_s(m_cLogData, sizeof(m_cLogData), "���C�h������́F%ls", wstrWideChar.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
    sprintf_s(m_cLogData, sizeof(m_cLogData), "�}���`�o�C�g������ւ̕ϊ��ɕK�v�Ȓ����́F%d", iChangeLen);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    // �����񒷂��擾�o�������`�F�b�N
    if (0 == iChangeLen)
    {
        // �����񒷂��擾�o���Ȃ�����
        // �����񒷎擾���s���b�Z�[�W�����O�ɏo��
        LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** �����񒷎擾���s(�B>�t<) *****");

        // NULL��ݒ�
        pcMultiByteBuff = NULL;
        goto END;
    }

    // �又��
    try
    {
        // ���C�h������}���`�o�C�g������ϊ��������̈���m��
        iBuffSize = iChangeLen + 1;
        pcMultiByteBuff = DBG_NEW char[iBuffSize];
        m_stLogData = CreateEventMessageProc(ACTIONSET, "���C�h������}���`�o�C�g������ϊ���");

        // �������̈悪�m�ۏo�������`�F�b�N
        if (NULL == pcMultiByteBuff)
        {
            // �������̈�̊m�ێ��s
            // ��O�����փX���[
            throw tagErrorCode::MEMORYERROR;
        }

        // �������̏�����
        memset(pcMultiByteBuff, 0x00, iBuffSize);

        // ���C�h������}���`�o�C�g������ϊ��������̈�m�ې����̃��b�Z�[�W�����O�ɏo��
        LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, "***** ���C�h������}���`�o�C�g������ϊ��������̈�m�ې���I�� *****");
        sprintf_s(m_cLogData, sizeof(m_cLogData), "%s��%s", m_stLogData.pcActionName, MEMORYCREATESUCCESS);
        LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, m_cLogData);

        // ���C�h�����񂩂�}���`�o�C�g������ɕϊ�
        // �iwcstombs_s��C++�����`�j
        wcstombs_s(&siRet, pcMultiByteBuff, iBuffSize, wstrWideChar.c_str(), _TRUNCATE);
        sprintf_s(m_cLogData, sizeof(m_cLogData), "�}���`�o�C�g������́F%s", pcMultiByteBuff);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
        sprintf_s(m_cLogData, sizeof(m_cLogData), "�}���`�o�C�g������̒����́F%zu", siRet);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

        // �ϊ����o�������`�F�b�N
        if (iChangeLen != siRet)
        {
            // �ϊ����s
            // �ϊ����s���b�Z�[�W�����O�ɏo��
            LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** ���C�h�����񂩂�̃}���`�o�C�g������ϊ����s(�B>�t<) *****");

            // �������̈�����
            // �K��delete���邱�ƁI
            SAFE_FREE_NEWALLOC_ARRAY(pcMultiByteBuff);
            goto END;
        }
        else
        {
            // �ϊ�����
            // �ϊ����ꂽ�}���`�o�C�g����������O�ɏo��
            strMultiByteBuff = (const string)pcMultiByteBuff;
            sprintf_s(m_cLogData, sizeof(m_cLogData), "�}���`�o�C�g������́F%s", strMultiByteBuff.c_str());
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

            // �������̈�����
            // �K��delete���邱�ƁI
            SAFE_FREE_NEWALLOC_ARRAY(pcMultiByteBuff);
        }
    }

    // �㏈��
    // ��O����
    catch (const ERRORCODETABLE eAllocErr)
    {
        // �G���[���b�Z�[�W��������
        // �G���[���b�Z�[�W���o��
        pcMultiByteBuff = NULL;
        LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** ���C�h������}���`�o�C�g������ϊ��������̈�m�ۈُ�I�� *****");
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �G���[���b�Z�[�W�����֐��Ăяo�� *****");
        m_eErrorResultCode = eAllocErr;
        m_stLogData = CreateEventMessageProc(ERRORSET, m_stLogData.pcActionName, m_eErrorResultCode);
        LOGOUTPUTERROR(TRACEOUTPUTLOG, ERRORINFORMATION1, m_stLogData.pcMessage, m_stLogData.eErrorCode);
        sprintf_s(m_cLogData, sizeof(m_cLogData), "�߂�l�́F%d", m_stLogData.eErrorCode);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
    }

END:
    // �㏈��
    // �������擾����
    // ���C�h������}���`�o�C�g������ϊ��I�������擾
    m_stTimeData = g_clsSubOpeLogFile.GetDateTimeProc();
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���C�h������}���`�o�C�g������ϊ������I�� *****");
    sprintf_s(
        m_cLogData,
        sizeof(m_cLogData),
        "���C�h������}���`�o�C�g������ϊ��I�������F%s %s",
        (char*)m_stTimeData.strMachDatePlus.c_str(),
        (char*)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);

    // �ϊ���̕������ԋp
    return strMultiByteBuff;
}

//********************************************
// �֐����F�R�}���h���C��������͏���
// Function Name�FParseArgsProc
// 
// �����T�v�F
// �R�}���h���C����������͂���
// 
// �����菇�F
//   �P�D���������񃏃C�h������ϊ�
//   �Q�D�R�}���h���C���������
// �i�ȍ~��������R�}���h��񐔕����[�v�j
//   �R�D��͂��������}���`�o�C�g������ϊ�
// �i�����܂Ń��[�v�j
//   �S�D��͂�����񃁃������
// 
// �����F	 �R�}���h���C������
//           �R�}���h���
// �߂�l�F  �R�}���h���
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
vector<string> CSubOperations::ParseArgsProc(
    string strArgst,
    int* iArgv)
{
    // �ϐ��錾
    vector<string> vecArgvc;
    WCHAR* pcwArgsw = NULL;
    wstring wstrArgsw;
    LPWSTR* pcwArgvw = NULL;
    string strArgvc;
    int iICnt = 0;
    int iJCnt = 0;

    // �O����
    // �p�����[�^���`�F�b�N
    if (0 == strArgst.size())
    {
        // �R�}���h���C�����擾�o���Ă��Ȃ�
        // NULL��ԋp
        *iArgv = 0;
        goto END;
    }

    // ��������������C�h������ɕϊ�
    wstrArgsw = MultiToWideProc(strArgst);

    // ���C�h������ɕϊ��o�������`�F�b�N
    if (0 == wstrArgsw.size())
    {
        // �ϊ����s
        // �֐����I��
        goto END;
    }

    // �又��
    // �R�}���h���C�����������
    // �iCommandLineToArgvW��C++�����`�j
    pcwArgvw = CommandLineToArgvW(wstrArgsw.c_str(), iArgv);

    // ��͏o�������`�F�b�N
    if (NULL == pcwArgvw)
    {
        // ��͎��s
        // NULL��ԋp
        pcwArgvw = NULL;
        goto END;
    }

    // �㏈��
    // ��͂����R�}���h���C�������̐������[�v
    for (iICnt = 0; iICnt < *iArgv; ++iICnt)
    {
        // ��͂��������}���`�o�C�g������ɕϊ����Ċi�[
        strArgvc = WideToMultiProc(pcwArgvw[iICnt]);

        // �i�[�o�������`�F�b�N
        if (0 == strArgvc.size() && 0 == iICnt)
        {
            // �i�[�Ɏ��s
            // �֐����I��
            goto END;
        }

        // �ϊ������}���`�o�C�g��������i�[
        vecArgvc.push_back(strArgvc);
    }

    // ��͂����������
    // �iLocalFree��C++�����`�j
    (void)LocalFree((HLOCAL)pcwArgvw);

END:
    // �i�[�����}���`�o�C�g������Q��ԋp
    return vecArgvc;
}

//********************************************
// �֐����F�G���[���b�Z�[�W�擾����
// Function Name�FGetErrorMessageProc
// 
// �����T�v�F
// �G���[�R�[�h�ɑ΂��Ĉȉ��̃��b�Z�[�W��ݒ肷��
//      �E���ϐ��擾�G���[
//      �E�p�����[�^�G���[�i�w��Ȃ��j
//      �E�p�����[�^�G���[�i�����w��j
//      �E�t�@�C���I�[�v���G���[
//      �E�t�@�C���N���[�Y�G���[
//      �E�f�[�^�Ȃ��G���[
//      �E�f�[�^�s���G���[
//      �E�f�[�^�擾�G���[
//      �E�������̈�m�ۃG���[
//      �E�t�@�C���R�s�[�G���[
//      �E�t�@�C�����ϊ��G���[
//      �E�t�@�C���폜�G���[
//      �E�t�@�C����Ԏ擾�G���[
//      �E���̑��G���[
// 
// �����菇�F
//   �P�D�G���[�R�[�h�Ɋ�Â������b�Z�[�W�ݒ�
// 
// �����F     �G���[�R�[�h
// �߂�l�F   �G���[���b�Z�[�W
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
const char *CSubOperations::GetErrorMessageProc(
    ERRORCODETABLE eErr)
{
    // �ϐ��錾
    const char* pcErrMessage = "";

    // �O����
    // �������擾����
    // �G���[���b�Z�[�W�擾�J�n�����擾
    m_stTimeData = g_clsSubOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "�G���[���b�Z�[�W�擾�J�n�����F%s %s", 
        (char *)m_stTimeData.strMachDatePlus.c_str(), 
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �G���[���b�Z�[�W�擾�����J�n *****");

    // �G���[�R�[�h�����O�ɏo��
    sprintf_s(m_cLogData, sizeof(m_cLogData), "�G���[�R�[�h�́F%d", eErr);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    // �又��
    // �G���[�R�[�h�Ɋ�Â����b�Z�[�W���擾
    switch (eErr)
    {
        case tagErrorCode::ENVERROR:
            // ���ϐ��擾�G���[
            // ���b�Z�[�W��ݒ�
            pcErrMessage = ENVGETERROR;
            break;
        case tagErrorCode::NOPARAM:
            // �p�����[�^�w��Ȃ�
            // ���b�Z�[�W��ݒ�
            pcErrMessage = NOTPARAMETER;
            break;
        case tagErrorCode::FAILPARAM:
            // �p�����[�^�G���[�i�����w��j
            // ���b�Z�[�W��ݒ�
            pcErrMessage = PARAMETERERROR;
            break;
        case tagErrorCode::FILEOPENERROR:
            // �t�@�C�����J���Ȃ��i�w�肵���t�@�C�����Ȃ����j
            // ���b�Z�[�W��ݒ�
            pcErrMessage = FILECANNOTOPEN;
            break;
        case tagErrorCode::FILECLOSEERROR:
            // �t�@�C��������Ȃ�
            // ���b�Z�[�W��ݒ�
            pcErrMessage = FILECANNOTCLOSE;
            break;
        case tagErrorCode::NODATA:
            // �f�[�^���Ȃ�
            // ���b�Z�[�W��ݒ�
            pcErrMessage = NOTDATA;
            break;
        case tagErrorCode::DATAFRAUD:
            // �f�[�^���s��
            // ���b�Z�[�W��ݒ�
            pcErrMessage = DATAINJUSTICE;
            break;
        case tagErrorCode::DATAERROR:
            // �f�[�^�擾�G���[
            // ���b�Z�[�W��ݒ�
            pcErrMessage = DATAGETERROR;
            break;
        case tagErrorCode::MEMORYERROR:
            // �������̈�m�ێ��s
            // ���b�Z�[�W��ݒ�
            pcErrMessage = MEMORYENSUREERROR;
            break;
        case tagErrorCode::COPYERROR:
            // �t�@�C���R�s�[�G���[
            // ���b�Z�[�W��ݒ�
            pcErrMessage = FILECOPYERROR;
            break;
        case tagErrorCode::RENAMEERROR:
            // �t�@�C�����ϊ��G���[
            // ���b�Z�[�W��ݒ�
            pcErrMessage = FILERENAMEERROR;
            break;
        case tagErrorCode::DELETEERROR:
            // �t�@�C���폜�G���[
            // ���b�Z�[�W��ݒ�
            pcErrMessage = FILEDELETEERROR;
            break;
        case tagErrorCode::STATERROR:
            // �t�@�C����Ԏ擾�G���[
            // ���b�Z�[�W��ݒ�
            pcErrMessage = FILESTATERROR;
            break;
        default:
            // ��L�ȊO
            // ���b�Z�[�W��ݒ�
            pcErrMessage = OPERATIONERROR;
            break;
    }

    // �㏈��
    // �擾�������b�Z�[�W�����O�ɏo��
    sprintf_s(m_cLogData, sizeof(m_cLogData), "�G���[���b�Z�[�W�́F%s", pcErrMessage);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    // �������擾����
    // �G���[���b�Z�[�W�擾�I�������擾
    m_stTimeData = g_clsSubOpeLogFile.GetDateTimeProc();
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �G���[���b�Z�[�W�擾�����I�� *****");
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "�G���[���b�Z�[�W�擾�I�������F%s %s", 
        (char *)m_stTimeData.strMachDatePlus.c_str(), 
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);

    // �G���[���b�Z�[�W��ԋp
    return pcErrMessage;
}
