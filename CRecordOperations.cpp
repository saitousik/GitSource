//============================================
// ���ގ����R�[�h����N���X�\�[�X�t�@�C��
// CRecordOperations.cpp
// 
// �@�\�T�v
// ���ގ����R�[�h�ǂݍ��݋y�ѓ��ގ����R�[�h�`�F�b�N�A
// PC�{����ʋy�ѓ��ގ��������݃f�[�^�t�@�C���ւ̈󎚓��f�[�^������s��
// 
// �@�\���e
// �P�D���ގ��f�[�^�ҏW���擾����
// �Q�D���ގ��f�[�^�ҏW���o�͎w������
// �R�D�����G���A���x���N���A����
// �S�D���ގ����ގ��󋵈󎚌��o���ݒ菈��
// �T�D���ގ��f�[�^�ҏW��񐶐�����
// �U�D�����G���A���x���`�F�b�N����
// �V�D���ގ��f�[�^�t�@�C���o�͏���
// �W�D���ގ��f�[�^PC�{����ʏo�͏���
// 
// �⑫����
// �E���@�\�̏ڍׂ��Ԃɂ��Ă͋��ʋ@�\���Ăяo�����O���Ƃ��ă��O�o�̓t�@�C���ɏo�͂���
// �E���@�\�ł͋��ʋ@�\���Ăяo�����s�R�[�h�폜�����s��
// 
// �쐬����
// 2020/9/25  s-saito   �V�K�쐬
// 
// �X�V����
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
#include "CRecordOperations.h"

// �N���X��`
CLogFile g_clsRecOpeLogFile;

// �}�N����`
#define LOGOUTPUT(iOutputType, iTraceOutputLogType, cMessageInfo) \
    g_clsRecOpeLogFile.LogOutputControlProc(LOGARGS, iOutputType, iTraceOutputLogType, cMessageInfo)
#define LOGOUTPUTERROR(iOutputType, iTraceOutputLogType, cErrorMessageInfo, eErrorInfoCode) \
    g_clsRecOpeLogFile.LogOutputControlProc(LOGARGS, iOutputType, iTraceOutputLogType, cErrorMessageInfo, eErrorInfoCode)

//��������������������������������������������
// �e�@�\�iAPI�j��`
//��������������������������������������������
//********************************************
// �֐����F�f�t�H���g�R���X�g���N�^
// Function Name�FCRecordOperations
// 
// �����T�v�F
// �N���X�����ɔ������������e���`����
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
CRecordOperations::CRecordOperations()
{
    // STAT�\���̕ϐ�������
    memset(&m_stStat, 0x00, sizeof(struct stat));
}

//********************************************
// �֐����F�p�����[�^���t���R���X�g���N�^
// Function Name�FCRecordOperations
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
CRecordOperations::CRecordOperations(
    const string strTraceLogFileName, 
    const string strDataLogFileName)
{
    // ���O�o�̓N���X����
    g_clsRecOpeLogFile = CLogFile(strTraceLogFileName, strDataLogFileName);

    // �Ăяo�����Ŏw�肳�ꂽ���O�o�̓t�@�C�����ݒ�
    m_strRecOpeTraceLogFileName = strTraceLogFileName;
    m_strRecOpeDataLogFileName = strDataLogFileName;
    m_strFileOpeTraceLogFileName = strTraceLogFileName;
    m_strFileOpeDataLogFileName = strDataLogFileName;
    m_strSubOpeTraceLogFileName = strTraceLogFileName;
    m_strSubOpeDataLogFileName = strDataLogFileName;

    // ���ގ��������\���̕ϐ�������
    memset(&m_stEntryExitInfo, 0x00, sizeof(ENTRYEXITINFORMATION));
    memset(&m_stEntryExitInfo.unEntryExitRecord.stEntryExitRecord, 
        0x00, 
        sizeof(ENTRYEXITRECORDINFO));
    memset(&m_stEntryExitInfo.unEntryExitRecord.cEntryExitData, 
        0x00, 
        sizeof(m_stEntryExitInfo.unEntryExitRecord.cEntryExitData));
}

//********************************************
// �֐����F�f�X�g���N�^
// Function Name�F~CRecordOperations
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
CRecordOperations::~CRecordOperations()
{
    // �}�b�v�e�[�u�������폜
    m_maErrorActionName.clear();

    // ���O�o�̓t�@�C�����ϐ��N���A
    m_strRecOpeTraceLogFileName.clear();
    m_strRecOpeDataLogFileName.clear();
    m_strFileOpeTraceLogFileName.clear();
    m_strFileOpeDataLogFileName.clear();
    m_strFileOpeCopyFileName.clear();
    m_strSubOpeTraceLogFileName.clear();
    m_strSubOpeDataLogFileName.clear();

    // �e�����o�ϐ��N���A
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
// �֐����F���ގ��f�[�^�ҏW���擾����
// Function Name�FGetEditDataProc
// 
// �����T�v�F
// ���ގ��ǂݍ��݃f�[�^�t�@�C������1���R�[�h��ǂݍ���
// 
// �����菇�F
//   �P�D���R�[�h�ǂݍ���
//   �Q�D�t�@�C���I�[�`�F�b�N�i���R�[�h�擾�o���Ȃ������ꍇ�j
//   �R�D���ގ��f�[�^�ҏW��񐶐������i���R�[�h�擾���������R�[�h����33�o�C�g�̏ꍇ�j
// 
// �����F    ���ގ��ǂݍ��݃f�[�^�t�@�C���|�C���^
//           ���ގ��������݃f�[�^�t�@�C���|�C���^
//           �X�L�b�v����t���O
//           �t�@�C���I���t���O
//           ���s����
//           ���ގ����R�[�h����
// �߂�l�F  ���ގ��f�[�^�ҏW�p�\���́i���I�\���́j
// 
// �⑫����
// �E���R�[�h�ǂݍ��݂̓��R�[�h����33�o�C�g��1���R�[�h�P�ʂœǂݍ���
// �E���ގ��f�[�^�ҏW�p�\���̐��������̓��R�[�h����33�o�C�g�̏ꍇ�̂ݍs��
// �E���s���ʂ͈ȉ��̏���ݒ肷��
//         ����I���FEXIT_SUCCESS�i0�j�i����I����stdlib.h�Œ�`���Ă�����e�𗘗p�j
//         �ُ�I���F���R�[�h�Ȃ��FCM_NODATA�i106�j
//                   ���R�[�h�s���FCM_DATAFRAUD�i107�j
//                   ���R�[�h�擾�G���[�FCM_DATAERROR�i108�j
// 
// 
// �쐬����
// 2020/9/25  s-saito   �V�K�쐬
// 
// �X�V����
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
    // �ϐ��錾
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

    // ������
    memset(cReadResult, 0x00, sizeof(cReadResult));
    memset(cReadResultLogout, 0x00, sizeof(cReadResultLogout));
    memset(cFraudData, 0x00, sizeof(cFraudData));
    memset(cFraudOutputData, 0x00, sizeof(cFraudOutputData));

    // �W���o�͂ɓ��{�ꂪ�\�������悤�Ƀ��P�[������{�ɐݒ�
    setlocale(LC_ALL, "japanese");

    // �O����
    // �������擾����
    // ���ގ��f�[�^�ҏW���擾�����J�n�����擾
    m_stTimeData = g_clsRecOpeLogFile.GetDateTimeProc();
    sprintf_s(m_cLogData, 
        sizeof(m_cLogData), 
        "���ގ��f�[�^�ҏW���擾�����J�n�����F%s %s", 
        (char *)m_stTimeData.strMachDatePlus.c_str(), 
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
    sprintf_s(m_cLogData,
        sizeof(m_cLogData),
        "***** ���ގ��f�[�^�ҏW���擾�����J�n�i%ld��ځj *****",
        *plRecCnt + 1);
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);

    try
    {
        // ���ގ����f�[�^�ҏW�p�\���̃������̈�̊m��
        pstEditData = (ENTRYEXITDATA *)malloc(sizeof(ENTRYEXITDATA));
        m_stLogData = CreateEventMessageProc(
            ACTIONSET,
            "���ގ��f�[�^�ҏW����");

        // �������̈悪�m�ۏo�������`�F�b�N
        if (NULL == pstEditData)
        {
            // �������̈�̊m�ێ��s
            // ��O�����փX���[
            throw tagErrorCode::MEMORYERROR;
        }

        // �������̏�����
        memset(pstEditData, 0x00, sizeof(ENTRYEXITDATA));

        // �\���̂ɏ����l��ݒ�
        *pstEditData = stInitInfoData;
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, "***** ���ގ��f�[�^�ҏW���̏������ *****");
        sprintf_s(
            m_cLogData,
            sizeof(m_cLogData),
            "�J�[�hID�F%s",
            pstEditData->cCardID);
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
        sprintf_s(
            m_cLogData,
            sizeof(m_cLogData),
            "�ޔ��J�[�hID�F%s",
            m_stEntryExitInfo.cLastID);
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
        sprintf_s(
            m_cLogData,
            sizeof(m_cLogData),
            "���[�U�[���F%s",
            pstEditData->cName);
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
        sprintf_s(
            m_cLogData,
            sizeof(m_cLogData),
            "���t�F%s",
            pstEditData->cDate);
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
        sprintf_s(
            m_cLogData,
            sizeof(m_cLogData),
            "�����F%s",
            pstEditData->cTime);
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
        sprintf_s(
            m_cLogData,
            sizeof(m_cLogData),
            "�h�A�ԍ��F%s",
            pstEditData->cDoor);
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
        sprintf_s(
            m_cLogData,
            sizeof(m_cLogData),
            "���ގ���ԁF%s",
            pstEditData->cDir);
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
        sprintf_s(
            m_cLogData,
            sizeof(m_cLogData),
            "�ۏ��F%s",
            pstEditData->cAct);
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

        // �������̈�m�ې����̃��b�Z�[�W�����O�ɏo��
        LOGOUTPUT(
            TRACEOUTPUTLOG,
            SUCCESSINFORMATION,
            "***** ���ގ����f�[�^�ҏW�p�\���̃������̈�m�ې���I�� *****");
        sprintf_s(
            m_cLogData,
            sizeof(m_cLogData),
            "%s%s",
            m_stLogData.pcActionName,
            MEMORYCREATESUCCESS);
        LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, m_cLogData);

        // �又��
        // �t�@�C�����烌�R�[�h���擾
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���ގ����R�[�h�擾�J�n *****");
        pcReadResult = fgets(m_stEntryExitInfo.unEntryExitRecord.cEntryExitData, DATASIZE, pfDataFile);
        lDatalen = (long)strlen(m_stEntryExitInfo.unEntryExitRecord.cEntryExitData);
        sprintf_s(m_cLogData, sizeof(m_cLogData), "���R�[�h���F%ld", lDatalen);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���ގ����R�[�h�擾�I�� *****");

        // �ǂݍ��݌��ʂ��`�F�b�N
        if (NULL == pcReadResult)
        {
            // ���R�[�h���擾�o���Ȃ�����
            // �I�[�����`�F�b�N
            LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���ގ����R�[�h�̏I�[�l�`�F�b�N *****");
            iDataEOF = feof(pfDataFile);
            sprintf_s(m_cLogData, sizeof(m_cLogData), "�I�[�l�́F%d", iDataEOF);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
            if (EXIT_SUCCESS != iDataEOF)
            {
                // �I�[��񂠂�
                // �O��Ƀ��R�[�h���擾���Ă������`�F�b�N
                if (0 == lDataCnt)
                {
                    // ���R�[�h�Ȃ��G���[�Ƃ��ė�O�𔭐����ė�O�����փX���[
                    throw tagErrorCode::NODATA;
                }
                else
                {
                    // �t�@�C���I���t���O��ON
                    LOGOUTPUT(
                        TRACEOUTPUTLOG,
                        SUCCESSINFORMATION,
                        "***** ���ގ��f�[�^�ҏW���擾����I�� *****");
                    *piFileEndFlag = FILEEND_ON;
                    LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, READEND);

                    // ���R�[�h���擾�o���Ă��Ȃ����߃X�L�b�v����t���O��ON
                    *pbSkipFlag = true;
                    sprintf_s(
                        m_cLogData,
                        sizeof(m_cLogData),
                        "�X�L�b�v����t���O�F%s",
                        *pbSkipFlag ? "true" : "false");
                    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
                }
            }
            else
            {
                // �I�[���Ȃ�
                // ���R�[�h�擾�G���[�Ƃ��ė�O�𔭐����ė�O�����փX���[
                throw tagErrorCode::DATAERROR;
            }
        }
        else
        {
            // ���R�[�h�����݂��Ă����̂ŃJ�E���g�A�b�v
            (*plRecCnt)++;

            // �w�蕶���폜����
            // ���s�R�[�h���폜���Ď擾���R�[�h���o��
            strncpy_s(cReadResult, sizeof(cReadResult), m_stEntryExitInfo.unEntryExitRecord.cEntryExitData, lDatalen);
            DeleteDesignationCharProc(cReadResult, (const char)CRLF_CODE, cReadResultLogout);
            sprintf_s(m_cLogData, sizeof(m_cLogData), "�擾���R�[�h�F%s", cReadResultLogout);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

            // ���R�[�h�����`�F�b�N
            if (READDATA == lDatalen)
            {
                // ���R�[�h������
                // ���ގ��f�[�^�ҏW��񐶐�����
                // ���R�[�h����ގ����f�[�^�ҏW�p�\���̂Ő���
                LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���ގ��f�[�^�ҏW��񐶐��֐��Ăяo�� *****");
                *pstEditData = CreateEditDataProc(m_stEntryExitInfo.unEntryExitRecord);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "�擾�����J�[�hID�́F%s",
                    pstEditData->cCardID);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                // �����������ގ����f�[�^�ҏW�p�\���̂����O�o��
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, "***** ���ގ��f�[�^�ҏW���̐������ *****");
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "�J�[�hID�F%s", pstEditData->cCardID);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "�ޔ��J�[�hID�F%s",
                    m_stEntryExitInfo.cLastID);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "���[�U�[���F%s",
                    pstEditData->cName);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "���t�F%s",
                    pstEditData->cDate);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "�����F%s",
                    pstEditData->cTime);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "�h�A�ԍ��F%s",
                    pstEditData->cDoor);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "���ގ���ԁF%s",
                    pstEditData->cDir);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "�ۏ��F%s",
                    pstEditData->cAct);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                // �f�[�^�ҏW�y�ш󎚉\�������J�E���g
                lDataCnt++;
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "�󎚉\�ȃ��R�[�h�����F%ld",
                    lDataCnt);
                LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

                // �󎚑Ώۃ��R�[�h�̂��߃X�L�b�v����t���O��OFF
                *piGetResult = EXIT_SUCCESS;
                *pbSkipFlag = false;
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "�X�L�b�v����t���O�F%s",
                    *pbSkipFlag ? "true" : "false");
                LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
                LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, "***** ���ގ��f�[�^�ҏW���擾����I�� *****");
            }
            else
            {
                // ���R�[�h�����s��
                // �w�蕶���폜����
                // ���s�R�[�h���폜������������o��
                strncpy_s(cFraudData, sizeof(cFraudData), pcReadResult, sizeof(cFraudData) - 1);
                DeleteDesignationCharProc(cFraudData, (const char)CRLF_CODE, cFraudOutputData);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "�o�̓f�[�^�́F%s",
                    cFraudOutputData);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                // ���ގ��s���f�[�^�t�@�C���o�͏���
                PrintFraudDataFileProc(cFraudOutputData, g_stFileNameInfo.strFraudDataName.c_str());

                // �󎚑ΏۊO���R�[�h�̂��߃X�L�b�v����t���O��ON
                *pbSkipFlag = true;
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "�X�L�b�v����t���O�F%s",
                    *pbSkipFlag ? "true" : "false");
                LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
                LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** ���ގ��f�[�^�ҏW���擾�ُ�I�� *****");

                // ���R�[�h�s���G���[�Ƃ��ė�O�𔭐����ė�O�����փX���[
                throw tagErrorCode::DATAFRAUD;
            }
        }
    }

    // ��O����
    catch (const ERRORCODETABLE eDataErr)
    {
        // ��O�R�[�h���`�F�b�N
        if (tagErrorCode::MEMORYERROR == eDataErr)
        {
            // �������̈�m�ۃG���[
            // �G���[�R�[�h��ݒ�
            LOGOUTPUT(
                TRACEOUTPUTLOG,
                ERRORINFORMATION2,
                "***** ���ގ����f�[�^�ҏW�p�\���̃������̈�m�ۈُ�I�� *****");
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
            // ���R�[�h���Ȃ�
            LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, "***** ���R�[�h���Ȃ��B�B�B( *�L�E��)/(�G�t�G ) *****");
            LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, "***** �_�~�[�f�[�^���g�p�E�E�E(�G�L�D�M) *****");
            LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, "***** ���ގ��f�[�^�ҏW���_�~�[��� *****");
            sprintf_s(
                m_cLogData,
                sizeof(m_cLogData),
                "�J�[�hID�F%s",
                pstEditData->cCardID);
            LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
            sprintf_s(
                m_cLogData,
                sizeof(m_cLogData),
                "�ޔ��J�[�hID�F%s",
                m_stEntryExitInfo.cLastID);
            LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
            sprintf_s(
                m_cLogData,
                sizeof(m_cLogData),
                "���[�U�[���F%s",
                pstEditData->cName);
            LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
            sprintf_s(
                m_cLogData,
                sizeof(m_cLogData),
                "���t�F%s",
                pstEditData->cDate);
            LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
            sprintf_s(
                m_cLogData,
                sizeof(m_cLogData),
                "�����F%s",
                pstEditData->cTime);
            LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
            sprintf_s(
                m_cLogData,
                sizeof(m_cLogData),
                "�h�A�ԍ��F%s",
                pstEditData->cDoor);
            LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
            sprintf_s(
                m_cLogData,
                sizeof(m_cLogData),
                "���ގ���ԁF%s",
                pstEditData->cDir);
            LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
            sprintf_s(
                m_cLogData,
                sizeof(m_cLogData),
                "�ۏ��F%s",
                pstEditData->cAct);
            LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

            // PC�{����ʂɌ��o�����o�́i���ގ��������݃f�[�^�t�@�C���ւ͏o�͂��Ȃ��j
            LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���ގ��󋵈󎚌��o���ݒ�֐��Ăяo�� *****");
            SetEntryExitDataTitleProc(pfOutputFile, eDataErr);

            // �����l���_�~�[�f�[�^�Ƃ��Đ���
            // ���[�U�[ID�E���[�U�[��
            strDummyData = pstEditData->cCardID;
            strDummyData += "  ";
            strDummyData += pstEditData->cName;
            strDummyData += "  ";

            // ���t�E�����E�h�A�ԍ�
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

            // �_�~�[�f�[�^
            strDummyDataInfo = strDummyData;
            strDummyDataInfo += strDummyDateTime;
            strDummyDataInfo += "       ";
            strDummyDataInfo += strDummyDateTime;
            strDummyDataInfo += "       ";
            strDummyDataInfo += strDummyDateTime;

            // ��U���O�o�͗p�Ɋi�[���ĉ��s�R�[�h��ݒ�
            strDummyDataInfoLog = strDummyDataInfo;
            strDummyDataInfo += "\n\n";

            // ���ގ��f�[�^�ҏW���o�͐��䏈��
            // �_�~�[�f�[�^�o�͂��w���i�����ł͓��ގ��������݃f�[�^�t�@�C���|�C���^�͎g�p���Ȃ��j
            ePrintType = tagPrintType::DUMMYDATA;
            LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���ގ��f�[�^�ҏW���o�͐���֐��Ăяo�� *****");
            PrintDataControlProc(
                SCREENPRINT,
                pfOutputFile,
                "%s",
                "",
                *pstEditData,
                strDummyDataInfo.c_str(), 
                ePrintType);
            LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �_�~�[�f�[�^�Ƃ��ď����l���o�� *****");
            sprintf_s(m_cLogData, sizeof(m_cLogData), "�_�~�[�f�[�^�F%s", strDummyDataInfoLog.c_str());
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

            // ���ގ����f�[�^�ҏW�p�\���̃������̈�����
            LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���ގ����f�[�^�ҏW�p�\���̃������̈��� *****");
#ifdef _DEBUG
            SAFE_FREE_DEBUG(pstEditData);
#else
            SAFE_FREE(pstEditData);
#endif // _DEBUG

            // �������̈����̃��b�Z�[�W�����O�ɏo��
            LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���ގ����f�[�^�ҏW�p�\���̃������̈����I�� *****");
            sprintf_s(
                m_cLogData,
                sizeof(m_cLogData),
                "%s%s",
                m_stLogData.pcActionName,
                MEMORYRELEASESUCCESS);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

            // �t�@�C���I���t���O��ON�i�߂�l�����R�[�h�Ȃ��j
            LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** ���ގ��f�[�^�ҏW���擾�ُ�I�� *****");
            m_stLogData = CreateEventMessageProc(ACTIONSET, "���ގ��f�[�^�ҏW����");
            *piFileEndFlag = FILEEND_ON;
            *piGetResult = CM_NODATA;
            g_clsRecOpeLogFile.m_eErrorResultCode = eDataErr;
            g_clsRecOpeLogFile.m_maErrorActionName.insert(make_pair(*piGetResult, m_stLogData.pcActionName));

            // �������擾����
            // ���ގ��f�[�^�ҏW���擾�I�������擾
            m_stTimeData = g_clsRecOpeLogFile.GetDateTimeProc();
            sprintf_s(
                m_cLogData,
                sizeof(m_cLogData),
                "���ގ��f�[�^�ҏW���擾�I�������F%s %s",
                (char *)m_stTimeData.strMachDatePlus.c_str(),
                (char *)m_stTimeData.strMachTimePlus.c_str());
            LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);

            // ��O�����X���[
            throw eDataErr;
        }
        else if (tagErrorCode::DATAERROR == eDataErr)
        {
            // ���R�[�h�擾�G���[
            // �t�@�C���I���t���O��ON
            LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** ���ގ��f�[�^�ҏW���擾�ُ�I�� *****");
            *piFileEndFlag = FILEEND_ON;

            // ���R�[�h���擾�o���Ă��Ȃ����߃X�L�b�v����t���O��ON
            *pbSkipFlag = true;
            sprintf_s(
                m_cLogData,
                sizeof(m_cLogData),
                "�X�L�b�v����t���O�F%s",
                *pbSkipFlag ? "true" : "false");
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

            // ���ގ����f�[�^�ҏW�p�\���̃������̈�����
            LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���ގ����f�[�^�ҏW�p�\���̃������̈��� *****");
#ifdef _DEBUG
            SAFE_FREE_DEBUG(pstEditData);
#else
            SAFE_FREE(pstEditData);
#endif // _DEBUG

            // �������̈����̃��b�Z�[�W�����O�ɏo��
            LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���ގ����f�[�^�ҏW�p�\���̃������̈����I�� *****");
            sprintf_s(
                m_cLogData,
                sizeof(m_cLogData),
                "%s%s",
                m_stLogData.pcActionName,
                MEMORYRELEASESUCCESS);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

            // �G���[�R�[�h��ݒ�
            m_stLogData = CreateEventMessageProc(ACTIONSET, "���ގ��f�[�^�ҏW����");
            *piGetResult = CM_DATAERROR;
            g_clsRecOpeLogFile.m_eErrorResultCode = eDataErr;
            g_clsRecOpeLogFile.m_maErrorActionName.insert(make_pair(*piGetResult, m_stLogData.pcActionName));

            // �������擾����
            // ���ގ��f�[�^�ҏW���擾�I�������擾
            m_stTimeData = g_clsRecOpeLogFile.GetDateTimeProc();
            sprintf_s(
                m_cLogData,
                sizeof(m_cLogData),
                "���ގ��f�[�^�ҏW���擾�I�������F%s %s",
                (char *)m_stTimeData.strMachDatePlus.c_str(),
                (char *)m_stTimeData.strMachTimePlus.c_str());
            LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);

            // ��O�����X���[
            throw eDataErr;
        }
        else
        {
            // ���R�[�h�s��
            // ���b�Z�[�W�𐶐�
            m_stLogData = CreateEventMessageProc(ACTIONSET, "���ގ��f�[�^�ҏW����");

            // 2��ڈȍ~���`�F�b�N
            if (2 <= *plRecCnt)
            {
                // 2��ڈȍ~
                // �G���[���b�Z�[�W��������
                // �G���[�R�[�h��ݒ�y�у��b�Z�[�W�����O�ɏo��
                *piGetResult = CM_DATAFRAUD;
                m_maErrorActionName.insert(
                    make_pair(
                        *piGetResult, m_stLogData.pcActionName));
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "%d��ڂ̃��R�[�h���F%d",
                    *plRecCnt,
                    lDatalen);
                LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
                LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �G���[���b�Z�[�W�����֐��Ăяo�� *****");
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
                    "�߂�l�́F%d",
                    *piGetResult);
                LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

                // �����p�����b�Z�[�W�����O�ɏo��
                LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, DATACONTINUED);
            }
            else
            {
                // 1���
                // �G���[�R�[�h�̂ݐݒ�
                *piGetResult = CM_DATAFRAUD;
                m_eErrorResultCode = eDataErr;
                m_maErrorActionName.insert(
                    make_pair(
                        *piGetResult, m_stLogData.pcActionName));
            }

            // ���ގ����f�[�^�ҏW�p�\���̃������̈�����
            LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���ގ����f�[�^�ҏW�p�\���̃������̈��� *****");
#ifdef _DEBUG
            SAFE_FREE_DEBUG(pstEditData);
#else
            SAFE_FREE(pstEditData);
#endif // _DEBUG

            // �������̈����̃��b�Z�[�W�����O�ɏo��
            LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���ގ����f�[�^�ҏW�p�\���̃������̈����I�� *****");
            sprintf_s(
                m_cLogData,
                sizeof(m_cLogData),
                "%s%s",
                m_stLogData.pcActionName,
                MEMORYRELEASESUCCESS);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
        }
    }

    // �㏈��
    // �������擾����
    // ���ގ��f�[�^�ҏW���擾�I�������擾
    m_stTimeData = g_clsRecOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "���ގ��f�[�^�ҏW���擾�I�������F%s %s", 
        (char *)m_stTimeData.strMachDatePlus.c_str(), 
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);

    // �擾�������R�[�h����ԋp
    return pstEditData;
}

//********************************************
// �֐����F���ގ��f�[�^�ҏW���o�͎w������
// Function Name�FPutEditDataProc
// 
// �����T�v�F
// ���ގ��f�[�^�ҏW���擾�����Ŏ擾�������R�[�h�𒠕[�`���ɕҏW����
// PC�{����ʋy�ѓ��ގ��������݃f�[�^�t�@�C���ւ̈󎚂��w������
// 
// �����菇�F
//   �P�D�J�[�hID�y�у��[�U�[���o�͎w��
//   �Q�D�����G���A���x���`�F�b�N����
//   �R�D�s�����ɑ΂���f�[�^�o�͎w���i�s�����̏ꍇ�̂݁j
//   �S�D�����G���A���x���X�V
//   �T�D�ǂݍ��݃f�[�^�̐����G���A���x���Z�o
//   �U�D�擾�f�[�^�o�͎w��
//   �V�D�ۏ��`�F�b�N
//   �W�D�ۏ��o�͎w��
//   �X�D���ގ���ԃ`�F�b�N
//   �P�O�D���ގ���ԏo�͎w��
// 
// �����F     ���ގ��������݃f�[�^�t�@�C���|�C���^
//            ���ގ����f�[�^�ҏW�p�\���̃|�C���^
//            �h�A���x�����\���̃|�C���^
// �߂�l�F   �Ȃ�
// 
// �⑫����
// 
// 
// 
// �쐬����
// 2020/9/25  s-saito   �V�K�쐬
// 
// �X�V����
// 
// 
//********************************************
void CRecordOperations::PutEditDataProc(
	FILE *pfOutputFile, 
	ENTRYEXITDATA *pstEData, 
	DOORLEVELINFORMATION *pstDoorInfo)
{
    // �ϐ��錾
    int iPutSpace = 0;
    int iChkResult = ERR_OFF;
    PRINTTYPETABLE ePrintType = tagPrintType::DUMMY;

    // �O����
    // �������擾����
    // ���ގ��f�[�^�ҏW���o�͎w���J�n�����擾
    m_stTimeData = g_clsRecOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "���ގ��f�[�^�ҏW���o�͎w���J�n�����F%s %s", 
        (char *)m_stTimeData.strMachDatePlus.c_str(), 
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���ގ��f�[�^�ҏW���o�͎w���J�n *****");

    // �J�[�hID�������O�ɏo��
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "�ǂݍ��񂾃��R�[�h�̃J�[�hID�́F%s", 
        pstEData->cCardID);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "�ޔ𕪂̃J�[�hID�́F%s", 
        m_stEntryExitInfo.cLastID);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    // �又��
    // �J�[�hID������̏ꍇ
    if (0 == strncmp(pstEData->cCardID, m_stEntryExitInfo.cLastID, RECORD_CARDID))
    {
        // ���ގ��f�[�^�ҏW���o�͐��䏈��
        // ���p�󔒕����o�͎w���i�J�[�hID�Ɩ��O���o�͂��Ȃ�����ɔ��p�󔒕����ŕ␳�j
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���ގ��f�[�^�ҏW���o�͐���֐��Ăяo�� *****");
        PrintDataControlProc(BOTHPRINT, pfOutputFile, "%18s", "", *pstEData, " ");
    }
    else
    {
        // ���ގ��f�[�^�ҏW���o�͐��䏈��
        // �J�[�hID�ƃ��[�U�[���̏o�͂��w��
        ePrintType = tagPrintType::CARDID;
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���ގ��f�[�^�ҏW���o�͐���֐��Ăяo�� *****");
        PrintDataControlProc(BOTHPRINT, pfOutputFile, "%4s  %10s  ", "", *pstEData, "", ePrintType);

        // �J�[�hID��ޔ�
        strncpy_s(m_stEntryExitInfo.cLastID, sizeof(m_stEntryExitInfo.cLastID), pstEData->cCardID, RECORD_CARDID);
        sprintf_s(
            m_cLogData, 
            sizeof(m_cLogData), 
            "�ޔ��J�[�hID�́F%s", 
            m_stEntryExitInfo.cLastID);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

        // ���ގ��f�[�^�ҏW�������O�o��
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, "***** ���ގ��f�[�^�ҏW���̍X�V��� *****");
        sprintf_s(
            m_cLogData, 
            sizeof(m_cLogData), 
            "�J�[�hID�F%s", 
            pstEData->cCardID);
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
        sprintf_s(
            m_cLogData, 
            sizeof(m_cLogData), 
            "�ޔ��J�[�hID�F%s", 
            m_stEntryExitInfo.cLastID);
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
        sprintf_s(
            m_cLogData, 
            sizeof(m_cLogData), 
            "���[�U�[���F%s", 
            pstEData->cName);
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
        sprintf_s(
            m_cLogData, 
            sizeof(m_cLogData), 
            "���t�F%s", 
            pstEData->cDate);
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
        sprintf_s(
            m_cLogData, 
            sizeof(m_cLogData), 
            "�����F%s", 
            pstEData->cTime);
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
        sprintf_s(
            m_cLogData, 
            sizeof(m_cLogData), 
            "�h�A�ԍ��F%s", 
            pstEData->cDoor);
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
        sprintf_s(
            m_cLogData, 
            sizeof(m_cLogData), 
            "���ގ���ԁF%s", 
            pstEData->cDir);
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
        sprintf_s(
            m_cLogData, 
            sizeof(m_cLogData), 
            "�ۏ��F%s", 
            pstEData->cAct);
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
    }

    // �����G���A���x���`�F�b�N����
    // �h�A�̒ʉߏ󋵂��`�F�b�N
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �����G���A���x���`�F�b�N�֐��Ăяo�� *****");
    iChkResult = CheckLevelProc(*pstEData, pstDoorInfo);
    sprintf_s(m_cLogData, sizeof(m_cLogData), "�`�F�b�N���ʂ́F%d", iChkResult);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    // �`�F�b�N���ʂ��m�F
    // �����ł̔���l�wERR_ON�x�̓G���[�ł͂Ȃ��s�����Ƃ��Ĕ��肷��
    if (ERR_ON == iChkResult)
    {
        // �s�����̏ꍇ
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���݂̐����G���A���x�����s���� *****");
        sprintf_s(
            m_cLogData, 
            sizeof(m_cLogData), 
            "���ޏo��Ԃ́F%s", 
            pstEData->cDir);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

        // ���݂̒ʉ߃h�A���x���ƒʉߑO�h�A���x�������O�o��
        sprintf_s(
            m_cLogData, 
            sizeof(m_cLogData), 
            "���݂̒ʉ߃h�A���x���́F%d", 
            pstDoorInfo->iLevel);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
        sprintf_s(
            m_cLogData, 
            sizeof(m_cLogData), 
            "�ʉߑO�h�A���x���́F%d", 
            pstDoorInfo->iBeforeLevel);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

        // ���ޏo��Ԃ��`�F�b�N
        if (0 == strncmp(pstEData->cDir, NYUSHITSU, RECORD_DIR))
        {
            // ���ގ���Ԃ��wI�x
            // ���݂̃h�A���x�����Z�o
            m_iNowDoorLevel = pstDoorInfo->iLevel - pstDoorInfo->iBeforeLevel;
            sprintf_s(
                m_cLogData, 
                sizeof(m_cLogData), 
                "���݂̃h�A���x���́F%d", 
                m_iNowDoorLevel);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

            // ���݂̃h�A���x�����`�F�b�N
            if (1 < m_iNowDoorLevel || 1 < m_iNowDoorLevel * (-1))
            {
                // �L�^���Ă��Ȃ���񂪕�������
                // ���ގ��f�[�^�ҏW���o�͐��䏈��
                // �{�����ގ��̋L�^������ׂ��s�ւ̏o�͂��w��
                ePrintType = tagPrintType::LEVELNG2;
                LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �L�^���Ă��Ȃ���񂪕������� *****");
                LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���ގ��f�[�^�ҏW���o�͐���֐��Ăяo�� *****");
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
                // �L�^���Ă��Ȃ���񂪕����Ȃ�
                // ���ގ��f�[�^�ҏW���o�͐��䏈��
                // �{�����ގ��̋L�^������ׂ��s�ւ̏o�͂��w��
                ePrintType = tagPrintType::LEVELNG1;
                LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �L�^���Ă��Ȃ���񂪕����Ȃ� *****");
                LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���ގ��f�[�^�ҏW���o�͐���֐��Ăяo�� *****");
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
            // ���ގ���Ԃ��wO�x
            // ���݂̃h�A���x�����Z�o
            m_iNowDoorLevel = pstDoorInfo->iBeforeLevel - pstDoorInfo->iLevel;
            sprintf_s(
                m_cLogData, 
                sizeof(m_cLogData), 
                "���݂̃h�A���x���́F%d", 
                m_iNowDoorLevel);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

            // ���݂̃h�A���x�����`�F�b�N
            if (1 < m_iNowDoorLevel)
            {
                // �L�^���Ă��Ȃ���񂪕�������
                // ���݂̃h�A���x�����ő傩�`�F�b�N
                if (FLOORMAX == m_iNowDoorLevel)
                {
                    // ���݂̃h�A���x�����ő�i�����L�^�Ȃ��j
                    // ���ގ��f�[�^�ҏW���o�͐��䏈��
                    // �{�����ގ��̋L�^������ׂ��s�ւ̏o�͂��w��
                    ePrintType = tagPrintType::LEVELNG3;
                    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �����L�^�Ȃ� *****");
                    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���ގ��f�[�^�ҏW���o�͐���֐��Ăяo�� *****");
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
                    // ���݂̃h�A���x�����ő�ł͂Ȃ��i�����L�^����j
                    // ���ގ��f�[�^�ҏW���o�͐��䏈��
                    // �{�����ގ��̋L�^������ׂ��s�ւ̏o�͂��w��
                    ePrintType = tagPrintType::LEVELNG2;
                    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �L�^���Ă��Ȃ���񂪕������� *****");
                    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���ގ��f�[�^�ҏW���o�͐���֐��Ăяo�� *****");
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
                // �L�^���Ă��Ȃ���񂪕����Ȃ�
                // ���ގ��f�[�^�ҏW���o�͐��䏈��
                // �{�����ގ��̋L�^������ׂ��s�ւ̏o�͂��w��
                ePrintType = tagPrintType::LEVELNG1;
                LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �L�^���Ă��Ȃ���񂪕����Ȃ� *****");
                LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���ގ��f�[�^�ҏW���o�͐���֐��Ăяo�� *****");
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

        // ���݂̒ʉ߃h�A���x�����X�V
        pstDoorInfo->iLevel = pstDoorInfo->iAfterLevel;
        sprintf_s(
            m_cLogData, 
            sizeof(m_cLogData), 
            "���݂̒ʉ߃h�A���x���́F%d", 
            pstDoorInfo->iLevel);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
    }
    else
    {
        // �����̏ꍇ
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���݂̐����G���A���x�������� *****");
        sprintf_s(
            m_cLogData, 
            sizeof(m_cLogData), 
            "���ޏo��Ԃ́F%s", 
            pstEData->cDir);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
    }

    // �㏈��
    // �h�A�ԍ����烌�x�����Z�o
    iPutSpace = pstEData->cDoor[0] - '0' - 1;
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "�Z�o��̃��x���́F%d", 
        iPutSpace);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    // ���x���l�Ɋ�Â��ă��[�v
    while (0 < iPutSpace--)
    {
        // ���ގ��f�[�^�ҏW���o�͐��䏈��
        // ���p�󔒕����o�͂��w��
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���ގ��f�[�^�ҏW���o�͐���֐��Ăяo�� *****");
        PrintDataControlProc(
            BOTHPRINT, 
            pfOutputFile,
            "%21s",
            "",
            *pstEData,
            " ");
    }

    // ���ގ��f�[�^�ҏW���o�͐��䏈��
    // �擾�����f�[�^�̏o�͂��w��
    ePrintType = tagPrintType::DOOR;
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���ގ��f�[�^�ҏW���o�͐���֐��Ăяo�� *****");
    PrintDataControlProc(
        BOTHPRINT, 
        pfOutputFile,
        "%.2s/%.2s %.2s:%.2s %2s ",
        "",
        *pstEData,
        "",
        ePrintType);

    // �ۏ������O�ɏo��
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "�ۏ��́F%s", 
        pstEData->cAct);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    // �ۏ����`�F�b�N
    if (0 == strncmp(pstEData->cAct, KYOHI, RECORD_ACT))
    {
        // �ۏ�񂪁w���ہx�̏ꍇ
        // ���ގ��f�[�^�ҏW���o�͐��䏈��
        // �w���ہx�̏��o�͂��w��
        ePrintType = tagPrintType::ACT;
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���ގ��f�[�^�ҏW���o�͐���֐��Ăяo�� *****");
        PrintDataControlProc(
            BOTHPRINT, 
            pfOutputFile,
            "%s",
            "(R)",
            *pstEData,
            "",
            ePrintType);
    }

    // ���ގ���Ԃ����O�ɏo��
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "���ގ���Ԃ́F%s", 
        pstEData->cDir);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    // ���ގ���Ԃ��`�F�b�N
    if (0 == strncmp(pstEData->cDir, NYUSHITSU, RECORD_DIR))
    {
        // ���ގ���Ԃ��wI�x
        // ���ގ��f�[�^�ҏW���o�͐��䏈��
        // ���ގ���Ԃɑ΂��āwIN�x�ł̏o�͂��w��
        ePrintType = tagPrintType::ACT;
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���ގ��f�[�^�ҏW���o�͐���֐��Ăяo�� *****");
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
        // ���ގ���񂪁wO�x
        // ���ގ��f�[�^�ҏW���o�͐��䏈��
        // ���ގ����ɑ΂��āwOUT�x�ł̏o�͂��w��
        ePrintType = tagPrintType::ACT;
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���ގ��f�[�^�ҏW���o�͐���֐��Ăяo�� *****");
        PrintDataControlProc(
            BOTHPRINT, 
            pfOutputFile,
            "%s\n",
            "OUT",
            *pstEData,
            "",
            ePrintType);
    }

    // �������擾����
    // ���ގ��f�[�^�ҏW���o�͎w���I�������擾
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���ގ��f�[�^�ҏW���o�͎w���I�� *****");
    m_stTimeData = g_clsRecOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "���ގ��f�[�^�ҏW���o�͎w���I�������F%s %s", 
        (char *)m_stTimeData.strMachDatePlus.c_str(), 
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
}

//********************************************
// �֐����F�����G���A���x���N���A����
// Function Name�FClearLevelProc
// 
// �����T�v�F
// �t�@�C���I���t���O�y�уJ�[�hID�Ƒޔ��J�[�hID�Ɋ�Â���
// �h�A���x�����\���̂̑O���R�[�h�ʉߌ�h�A���x�����N���A����
// 
// �����菇�F
//   �P�D�J�[�hID�`�F�b�N
//   �Q�D���݂̐����G���A���x���`�F�b�N
//   �R�D�s�����ɑ΂���f�[�^�o�͎w��
//   �S�D�����G���A���x���N���A
// 
// �����F     ���ގ��������݃f�[�^�t�@�C���|�C���^
//            ���ގ����f�[�^�ҏW�p�\����
//            �t�@�C���I���t���O
//            �h�A���x�����\���̃|�C���^
// �߂�l�F   �Ȃ�
// 
// �⑫����
// �E�����G���A���x����0���傫���ꍇ�̓��R�[�h�Ȃ����b�Z�[�W��
//     PC���{����ʋy�ѓ��ގ��������݃f�[�^�t�@�C���ֈ󎚂���
// 
// 
// �쐬����
// 2020/9/25  s-saito   �V�K�쐬
// 
// �X�V����
// 
// 
//********************************************
void CRecordOperations::ClearLevelProc(
	FILE *pfOutputFile, 
	const ENTRYEXITDATA stEData, 
	const int iFileEndFlg, 
	DOORLEVELINFORMATION *pstDoorData)
{
    // �ϐ��錾
    PRINTTYPETABLE ePrintType = tagPrintType::DUMMY;

    // �O����
    // �������擾����
    // �����G���A���x���N���A�J�n�����擾
    m_stTimeData = g_clsRecOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "�����G���A���x���N���A�J�n�����F%s %s", 
        (char *)m_stTimeData.strMachDatePlus.c_str(), 
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �����G���A���x���N���A�J�n *****");

    // �f�[�^��Ԃ����O�ɏo��
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "�t�@�C���I�[�l�́F%d", 
        iFileEndFlg);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "���ގ��f�[�^�ҏW���̃J�[�hID�́F%4s", 
        stEData.cCardID);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "�ޔ��J�[�hID�́F%4s", 
        m_stEntryExitInfo.cLastID);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    // �又��
    // �J�[�hID���`�F�b�N
    if ((FILEEND_ON == iFileEndFlg) ||
        (0 != strncmp(stEData.cCardID, m_stEntryExitInfo.cLastID, RECORD_CARDID)))
    {
        // �t�@�C���I���t���O��ON�������̓J�[�hID���ޔ𕪂ƈقȂ�ꍇ
        // ���݂̒ʉ߃h�A���x����0���傫�����`�F�b�N
        if (0 < pstDoorData->iLevel)
        {
            // ���݂̒ʉ߃h�A���x����0���傫���ꍇ
            sprintf_s(
                m_cLogData, 
                sizeof(m_cLogData), 
                "���݂̃G���A���x���́F%d", 
                pstDoorData->iLevel);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

            // ���݂̒ʉ߃h�A���x�����`�F�b�N
            // �L�^���Ă��Ȃ���񂪕�������
            switch (pstDoorData->iLevel)
            {
                case 3:
                {
                    // ���݂̒ʉ߃h�A���x�����w3�x�i�ގ��L�^�Ȃ��j
                    // ���ގ��f�[�^�ҏW���o�͐��䏈��
                    // �{���ގ��̋L�^������ׂ��s�ւ�3�s�o�͂��w��
                    ePrintType = tagPrintType::LEVELNG4;
                    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �ގ��L�^�Ȃ� *****");
                    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���ގ��f�[�^�ҏW���o�͐���֐��Ăяo�� *****");
                    PrintDataControlProc(
                        BOTHPRINT, 
                        pfOutputFile,
                        "%18s***** Level %d-->%d\n",
                        "",
                        stEData,
                        " ",
                        ePrintType,
                        pstDoorData->iLevel);

                    // ���򔲂�
                    break;
                }
                case 2:
                {
                    // ���݂̒ʉ߃h�A���x�����w2�x
                    // ���ގ��f�[�^�ҏW���o�͐��䏈��
                    // �{���ގ��̋L�^������ׂ��s�ւ�2�s�o�͂��w��
                    ePrintType = tagPrintType::LEVELNG5;
                    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���ގ��f�[�^�ҏW���o�͐���֐��Ăяo�� *****");
                    PrintDataControlProc(
                        BOTHPRINT, 
                        pfOutputFile,
                        "%18s***** Level %d-->%d\n",
                        "",
                        stEData,
                        " ",
                        ePrintType,
                        pstDoorData->iLevel);

                    // ���򔲂�
                    break;
                }
                default:
                {
                    // �L�^���Ă��Ȃ���񂪕����Ȃ�
                    // ���ގ��f�[�^�ҏW���o�͐��䏈��
                    // �{���ގ��̋L�^������ׂ��s�ւ�1�s�o�͂��w��
                    ePrintType = tagPrintType::LEVELNG6;
                    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���ގ��f�[�^�ҏW���o�͐���֐��Ăяo�� *****");
                    PrintDataControlProc(
                        BOTHPRINT, 
                        pfOutputFile,
                        "%18s***** Level %d-->0\n",
                        "",
                        stEData,
                        " ",
                        ePrintType,
                        pstDoorData->iLevel);

                    // ���򔲂�
                    break;
                }
            }

            // ���݂̒ʉ߃h�A���x�����N���A
            pstDoorData->iLevel = 0;
            sprintf_s(
                m_cLogData, 
                sizeof(m_cLogData), 
                "�N���A��̌��݂̒ʉ߃h�A���x���́F%d", 
                pstDoorData->iLevel);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
        }
    }

    // �㏈��
    // �������擾����
    // �����G���A���x���N���A�I�������擾
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �����G���A���x���N���A�I�� *****");
    m_stTimeData = g_clsRecOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "�����G���A���x���N���A�I�������F%s %s", 
        (char *)m_stTimeData.strMachDatePlus.c_str(), 
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
}

//********************************************
// �֐����F���ގ��󋵈󎚌��o���ݒ菈��
// Function Name�FSetEntryExitDataTitleProc
// 
// �����T�v�F
// ���ގ��������݃f�[�^�t�@�C���y��PC�{����ʂɌ��o����ݒ肷��
// 
// �����菇�F
//   �P�D�J�[�hID�`�F�b�N
//   �Q�D���݂̐����G���A���x���`�F�b�N
//   �R�D�s�����ɑ΂���f�[�^�o�͎w��
//   �S�D�����G���A���x���N���A
// 
// �����F     ���ގ��������݃f�[�^�t�@�C���|�C���^
// �@�@�@�@   �Ăяo�������s����
// �߂�l�F   �Ȃ�
// 
// �⑫����
// �E���ގ��������݃f�[�^�t�@�C���ւ̓t�@�C���ɏ������݂��Ȃ�����
//   ���ގ����R�[�h���擾�o���Ă���ꍇ�݈̂󎚂���
// 
// 
// �쐬����
// 2020/9/25  s-saito   �V�K�쐬
// 
// �X�V����
// 
// 
//********************************************
void CRecordOperations::SetEntryExitDataTitleProc(
    FILE *pfOutputFile, 
    ERRORCODETABLE eResult)
{
    // �ϐ��錾
    string strTitle;

    // �O����
    // �������擾����
    // ���ގ��󋵈󎚌��o���ݒ�J�n�����擾
    m_stTimeData = g_clsRecOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData,
        sizeof(m_cLogData),
        "���ގ��󋵈󎚌��o���ݒ�J�n�����F%s %s",
        (char *)m_stTimeData.strMachDatePlus.c_str(),
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���ގ��󋵈󎚌��o���ݒ�J�n *****");

    // �又��
    // ������𐶐�
    strTitle = TITLE;

    // �t�@�C���T�C�Y�����O�ɏo��
    sprintf_s(
        m_cLogData,
        sizeof(m_cLogData),
        "�t�@�C���T�C�Y�́F%d",
        m_stStat.st_size);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    // �t�@�C���T�C�Y���`�F�b�N
    if (0 == m_stStat.st_size && tagErrorCode::SUCCESS == eResult)
    {
        // �t�@�C���̒����Ȃ��i�V�K�쐬�j�����R�[�h�擾����
        // ���o�����t�@�C���֏o��
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �t�@�C���֏������� *****");
        fprintf_s(pfOutputFile, "%s\n", strTitle.c_str());
        fprintf_s(pfOutputFile, "\n");

        // �o�͂������o�������O�ɏo��
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �t�@�C���ւ̌��o���o�� *****");
        sprintf_s(
            m_cLogData,
            sizeof(m_cLogData),
            "�t�@�C���ւ̏o�̓f�[�^�́F%s",
            strTitle.c_str());
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
    }

#ifdef SCREEN
    // ���o����PC�{����ʂ֏o��
    printf_s("%s\n", strTitle.c_str());
    printf_s("\n");

    // �o�͂������o�������O�ɏo��
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** PC�{����ʂւ̌��o���o�� *****");
    sprintf_s(
        m_cLogData,
        sizeof(m_cLogData),
        "PC�{����ʂ̃f�[�^�́F%s",
        strTitle.c_str());
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
#endif // SCREEN

    // �㏈��
    // �������擾����
    // ���ގ��󋵈󎚌��o���ݒ�I�������擾
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���ގ��󋵈󎚌��o���ݒ�I�� *****");
    m_stTimeData = g_clsRecOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData,
        sizeof(m_cLogData),
        "���ގ��󋵈󎚌��o���ݒ�I�������F%s %s",
        (char *)m_stTimeData.strMachDatePlus.c_str(),
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
}

//********************************************
// �֐����F���ގ��f�[�^�ҏW��񐶐�����
// Function Name�FCreateEditDataProc
// 
// �����T�v�F
// ���ގ��ɑ΂���f�[�^�ҏW���Ƃ���
// ���ގ����R�[�h���擾����
// 
// �����菇�F
//      �P�D�J�[�hID�擾
//      �Q�D���[�U�[���擾
//      �R�F���t�擾
//      �S�F�����擾
//      �T�F�h�A�ԍ��擾
//      �U�F���ގ���Ԏ擾
//      �V�F�ۏ��擾
// 
// �����F     ���ގ���񃌃R�[�h
// �߂�l�F   ���ގ����f�[�^�ҏW�p�\����
// 
// �⑫����
// �E�������̓��R�[�h����33�o�C�g�̏ꍇ�̂ݍs�����Ƃ���
// 
// 
// �쐬����
// 2020/9/25  s-saito   �V�K�쐬
// 
// �X�V����
// 
// 
//********************************************
ENTRYEXITDATA CRecordOperations::CreateEditDataProc(
	const ENTRYEXITRECORD unRecData)
{
    // �ϐ��錾
    ENTRYEXITDATA stEditData;

    // ������
    memset(&stEditData, 0x00, sizeof(stEditData));

    // �O����
    // �������擾����
    // ���ގ����f�[�^�ҏW��񐶐��J�n�����擾
    m_stTimeData = g_clsRecOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "���ގ����f�[�^�ҏW��񐶐��J�n�����F%s %s", 
        (char *)m_stTimeData.strMachDatePlus.c_str(), 
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���ގ����f�[�^�ҏW��񐶐��J�n *****");

    // �又��
    // ���ގ����f�[�^�ҏW�p�\���̂ɑ΂��ē��ގ��f�[�^���R�[�h��������擾
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, "***** ���ގ����f�[�^�ҏW�p�\���̂̏�� *****");

    // �J�[�hID
    strncpy_s(stEditData.cCardID, sizeof(stEditData.cCardID), unRecData.stEntryExitRecord.cCardID, RECORD_CARDID);
    sprintf_s(m_cLogData, sizeof(m_cLogData), "�J�[�hID�F%s", stEditData.cCardID);
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

    // ���O
    strncpy_s(stEditData.cName, sizeof(stEditData.cName), unRecData.stEntryExitRecord.cName, RECORD_NAME);
    sprintf_s(m_cLogData, sizeof(m_cLogData), "���[�U�[���F%s", stEditData.cName);
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

    // ���t
    strncpy_s(stEditData.cDate, sizeof(stEditData.cDate), unRecData.stEntryExitRecord.cDate, RECORD_DATE);
    sprintf_s(m_cLogData, sizeof(m_cLogData), "���t�F%s", stEditData.cDate);
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

    // ����
    strncpy_s(stEditData.cTime, sizeof(stEditData.cTime), unRecData.stEntryExitRecord.cTime, RECORD_TIME);
    sprintf_s(m_cLogData, sizeof(m_cLogData), "�����F%s", stEditData.cTime);
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

    // �h�A�ԍ�
    strncpy_s(stEditData.cDoor, sizeof(stEditData.cDoor), unRecData.stEntryExitRecord.cDoor, RECORD_DOOR);
    sprintf_s(m_cLogData, sizeof(m_cLogData), "�h�A�ԍ��F%s", stEditData.cDoor);
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

    // ���ގ����
    strncpy_s(stEditData.cDir, sizeof(stEditData.cDir), unRecData.stEntryExitRecord.cDir, RECORD_DIR);
    sprintf_s(m_cLogData, sizeof(m_cLogData), "���ގ���ԁF%s", stEditData.cDir);
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

    // �ۏ��
    strncpy_s(stEditData.cAct, sizeof(stEditData.cAct), unRecData.stEntryExitRecord.cAct, RECORD_ACT);
    sprintf_s(m_cLogData, sizeof(m_cLogData), "�ۏ��F%s", stEditData.cAct);
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

    // �㏈��
    // �������擾����
    // ���ގ����f�[�^�ҏW��񐶐��I�������擾
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���ގ����f�[�^�ҏW��񐶐��I�� *****");
    m_stTimeData = g_clsRecOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "���ގ����f�[�^�ҏW��񐶐��I�������F%s %s", 
        (char *)m_stTimeData.strMachDatePlus.c_str(), 
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);

    // ���ގ����f�[�^�ҏW�p�\���̂�ԋp
    return stEditData;
}

//********************************************
// �֐����F�����G���A���x���`�F�b�N����
// Function Name�FCheckLevelProc
// 
// �����T�v�F
// �ۏ����`�F�b�N�y�ѓ��ގ���Ԃ�
// �����G���A���x���̐�����Ԃ��`�F�b�N����
// 
// �����菇�F
//   �P�D���ޏo��ԃ`�F�b�N
//   �Q�D�ۏ��`�F�b�N
//   �R�D�����G���A���x���`�F�b�N
//   �S�D�����G���A���x���X�V�i�����̏ꍇ�̂݁j
// 
// �����F     ���ގ����f�[�^�ҏW�p�\����
//            �h�A���x�����\���̃|�C���^
// �߂�l�F   �ƍ�����
//              ����    �FERR_OFF�i0�j
//              �s����  �FERR_ON�i1�j
// 
// �⑫����
// �E�����ł̒萔�wERR_OFF�x�wERR_ON�x�̓G���[�ł͂Ȃ�
//     �����E�s�����̔���l�Ƃ��Ďg�p����
// 
// 
// �쐬����
// 2020/9/25  s-saito   �V�K�쐬
// 
// �X�V����
// 
// 
//********************************************
int CRecordOperations::CheckLevelProc(
	const ENTRYEXITDATA stEData, 
	DOORLEVELINFORMATION *pstDoorData)
{
    // �ϐ��錾
    int iConsistencyFlag = 0;
    int iCheckResult = ERR_OFF;

    // �O����
    // �������擾����
    // �����G���A���x���`�F�b�N�J�n�����擾
    m_stTimeData = g_clsRecOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "�����G���A���x���`�F�b�N�J�n�����F%s %s", 
        (char *)m_stTimeData.strMachDatePlus.c_str(), 
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �����G���A���x���`�F�b�N�J�n *****");

    // �����l��ݒ�
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, "***** �����G���A���x�������l *****");
    pstDoorData->iDoorLevel = stEData.cDoor[0] - '0';
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "�ʉ߃h�A���x���F%d", 
        pstDoorData->iDoorLevel);
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
    pstDoorData->iBeforeLevel = pstDoorData->iDoorLevel;
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "�ʉߑO�h�A���x���F%d", 
        pstDoorData->iBeforeLevel);
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
    pstDoorData->iAfterLevel = pstDoorData->iDoorLevel;
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "�ʉߌ�h�A���x���F%d", 
        pstDoorData->iAfterLevel);
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "���݂̕s�����t���O�́F%d", 
        iConsistencyFlag);
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

    // �又��
    // ���ޏo��Ԃ����O�ɏo��
    sprintf_s(m_cLogData, sizeof(m_cLogData), "���ޏo��Ԃ́F%s", stEData.cDir);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    // ���ޏo��Ԃ��`�F�b�N
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, "***** �X�V�㐧���G���A���x���l *****");
    if (0 == strncmp(stEData.cDir, NYUSHITSU, RECORD_DIR))
    {
        // ���ޏo��Ԃ��wI�x�i�����j�̏ꍇ
        // �ʉߑO�h�A���x����ݒ�
        pstDoorData->iBeforeLevel = pstDoorData->iDoorLevel - 1;
        sprintf_s(
            m_cLogData, 
            sizeof(m_cLogData), 
            "�ʉߑO�h�A���x���́F%d", 
            pstDoorData->iBeforeLevel);
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
    }
    else if (0 == strncmp(stEData.cDir, TAISHITSU, RECORD_DIR))
    {
        // ���ޏo��Ԃ��wO�x�i�ގ��j�̏ꍇ
        // �ʉߌ�h�A���x����ݒ�
        pstDoorData->iAfterLevel = pstDoorData->iDoorLevel - 1;
        sprintf_s(
            m_cLogData, 
            sizeof(m_cLogData), 
            "�ʉߌ�G���A���x���́F%d", pstDoorData->iAfterLevel);
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
    }

    // �ۏ������O�ɏo��
    sprintf_s(m_cLogData, sizeof(m_cLogData), "�ۏ��́F%s", stEData.cAct);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    // �ۏ����`�F�b�N
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, "***** �X�V�㐧���G���A���x���l *****");
    if (0 == strncmp(stEData.cAct, KYOHI, RECORD_ACT))
    {
        // �ۏ�񂪁wR�x�i���ہj�̏ꍇ
        // �ʉߌ�h�A���x���ɒʉߑO�h�A���x����ݒ�
        pstDoorData->iAfterLevel = pstDoorData->iBeforeLevel;
        sprintf_s(
            m_cLogData, 
            sizeof(m_cLogData), 
            "�ʉߌ�h�A���x���́F%d", pstDoorData->iAfterLevel);
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
    }

    // ���݂̒ʉ߃h�A���x���ƒʉߑO�h�A���x�������ꂩ�`�F�b�N
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, "***** �X�V�㐧���G���A���x���l *****");
    if (pstDoorData->iLevel != pstDoorData->iBeforeLevel)
    {
        // �s����
        // ���݂̒ʉ߃h�A���x�����ʉߑO�h�A���x���ƈقȂ�ꍇ
        // �s�����t���O��ON
        iConsistencyFlag = 1;
        sprintf_s(
            m_cLogData, 
            sizeof(m_cLogData), 
            "�s�����t���O�́F%d", 
            iConsistencyFlag);
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
    }
    else
    {
        // ����
        // ���݂̒ʉ߃h�A���x�����ʉߑO�h�A���x���Ɠ���̏ꍇ
        sprintf_s(
            m_cLogData, 
            sizeof(m_cLogData), 
            "�s�����t���O�́F%d", 
            iConsistencyFlag);
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

        // ���݂̒ʉ߃h�A���x����ʉߌ�h�A���x���ōX�V
        pstDoorData->iLevel = pstDoorData->iAfterLevel;
        sprintf_s(
            m_cLogData, 
            sizeof(m_cLogData), 
            "�ʉ߃h�A���x���́F%d", 
            pstDoorData->iDoorLevel);
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
        sprintf_s(
            m_cLogData, 
            sizeof(m_cLogData), 
            "���݂̒ʉ߃h�A���x���́F%d", 
            pstDoorData->iLevel);
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
    }

    // �㏈��
    // �s�����t���O�����Ė߂�l��ݒ�
    iCheckResult = (1 == iConsistencyFlag) ? ERR_ON : ERR_OFF;
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "�ƍ����ʂ́F%d", iCheckResult);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �����G���A���x���`�F�b�N�I�� *****");

    // �������擾����
    // �����G���A���x���`�F�b�N�I�������擾
    m_stTimeData = g_clsRecOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "�����G���A���x���`�F�b�N�I�������F%s %s", 
        (char *)m_stTimeData.strMachDatePlus.c_str(), 
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);

    // �ƍ����ʂ�ԋp
    return iCheckResult;
}

//********************************************
// �֐����F���ގ��f�[�^�ҏW���o�͐��䏈��
// Function Name�FPrintDataControlProc
// 
// �����T�v�F
// �����󋵋y�ю��s���Ɋ�Â��ē��ގ��������݃f�[�^�t�@�C���y��
// PC�{����ʂւ̏o�͂�؂蕪����
// 
// �����菇�F
//   �P�D�f�[�^�o�͐�^�C�v�̕���
//   �Q�D���ގ��������݃f�[�^�t�@�C���݂̂̏o��
//   �R�DPC�{����ʂ݂̂̏o��
//   �S�D�����ւ̏o��
// 
// �����F�@ �f�[�^�o�͐�^�C�v
// �@�@�@�@ ���ގ��������݃f�[�^�t�@�C���|�C���^
// �@�@�@�@ �o�͌`��
// �@�@�@�@ ���ޏo���
// �@�@�@�@ ���ގ����f�[�^�ҏW�p�\����
//          �o�͕�����i�󔒓��j
// �@�@�@�@ �f�[�^�o�̓^�C�v
// �@�@�@�@ �����G���A���x�����P
// �@�@�@�@ �����G���A���x�����Q
// �߂�l�F �Ȃ�
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
    // �O����
    // �������擾����
    // ���ގ��f�[�^�ҏW���o�͐���J�n�����擾
    m_stTimeData = g_clsRecOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData,
        sizeof(m_cLogData),
        "���ގ��f�[�^�ҏW���o�͐���J�n�����F%s %s",
        (char *)m_stTimeData.strMachDatePlus.c_str(),
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���ގ��f�[�^�ҏW���o�͐���J�n *****");

    // �f�[�^�o�͐掯�ʎq�����O�ɏo��
    sprintf_s(
        m_cLogData,
        sizeof(m_cLogData),
        "�f�[�^�o�͐掯�ʎq�́F%d",
        iDataPrintType);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    // �又��
    // �f�[�^�o�͐掯�ʎq�Ɋ�Â��ďo�͐�𕪊�
    switch (iDataPrintType)
    {
    case BOTHPRINT:
    {
        // �t�@�C���E��ʗ����ɏo��
        // ���ގ��f�[�^PC�{����ʏo�͏����y�ѓ��ގ��f�[�^�t�@�C���o�͏���
        // PC�{����ʋy�ѓ��ގ��������݃f�[�^�t�@�C���֏o��
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���ގ��f�[�^�t�@�C���o�͊֐��Ăяo�� *****");
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
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���ގ��f�[�^PC�{����ʏo�͊֐��Ăяo�� *****");
        PrintScreenProc(
            pcOutputType,
            pcAct,
            stEData,
            pcEtcChar,
            eType,
            iLevel1,
            iLevel2);
#endif // SCREEN

        // ���򔲂�
        break;
    }
    case SCREENPRINT:
    {
        // PC�{����ʂ̂ݏo��
        // ���ގ��f�[�^PC�{����ʏo�͏���
        // PC�{����ʂ֏o��
#ifdef SCREEN
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���ގ��f�[�^PC�{����ʏo�͊֐��Ăяo�� *****");
        PrintScreenProc(
            pcOutputType,
            pcAct,
            stEData,
            pcEtcChar,
            eType,
            iLevel1,
            iLevel2);
#endif // SCREEN

        // ���򔲂�
        break;
    }
    default:
    {
        // ���ގ��������݃f�[�^�t�@�C���̂ݏo��
        // ���ގ��f�[�^�t�@�C���o�͏���
        // ���ގ��������݃f�[�^�t�@�C���֏o��
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���ގ��f�[�^�t�@�C���o�͊֐��Ăяo�� *****");
        PrintReportFileProc(
            pfOutputFile,
            pcOutputType,
            pcAct,
            stEData,
            pcEtcChar,
            eType,
            iLevel1,
            iLevel2);

        // ���򔲂�
        break;
    }
    }

    // �㏈��
    // �������擾����
    // ���ގ��f�[�^�ҏW���o�͐���I�������擾
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���ގ��f�[�^�ҏW���o�͐���I�� *****");
    m_stTimeData = g_clsRecOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData,
        sizeof(m_cLogData),
        "���ގ��f�[�^�ҏW���o�͐���I�������F%s %s",
        (char *)m_stTimeData.strMachDatePlus.c_str(),
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
}

//********************************************
// �֐����F���ގ��s���f�[�^�t�@�C���o�͏���
// Function Name�FPrintFraudDataFileProc
// 
// �����T�v�F
// ���ގ��������݃f�[�^�t�@�C����
// �o�͏o���Ȃ������s�����ގ��f�[�^�����R�[�h�`���ŏo�͂���
// 
// �����菇�F
//   �P�D���ގ��s���f�[�^�t�@�C���I�[�v��
//   �Q�D���ގ��s���f�[�^�t�@�C���ւ̏o��
// 
// �����F�@ ���ގ����R�[�h�s���f�[�^
// �@�@�@�@ ���ގ����R�[�h�s���f�[�^�t�@�C��
// �߂�l�F �Ȃ�
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
void CRecordOperations::PrintFraudDataFileProc(
    const char *pcFraudData, 
    const char *pcFraudDataFileName)
{
    // �ϐ��錾
    FILE *pfFraudDataFile = NULL;
    errno_t eLogErrNo = 0;
    int iCloseResult = EXIT_SUCCESS;

    // �O����
    // �������擾����
    // ���ގ��s���f�[�^�t�@�C���o�͊J�n�����擾
    m_stTimeData = g_clsRecOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData,
        sizeof(m_cLogData),
        "���ގ��s���f�[�^�t�@�C���o�͊J�n�����F%s %s",
        (char *)m_stTimeData.strMachDatePlus.c_str(),
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���ގ��s���f�[�^�t�@�C���o�͊J�n *****");

    // �又��
    // ���ގ��s���f�[�^�t�@�C���i�X�V�ǋL�^�j���I�[�v��
    // ���̏����ɉe���͂Ȃ��ד��ގ��s���f�[�^�t�@�C���̓I�[�v���Ɏ��s�����ꍇ�o�͂���߂ď������p��
    eLogErrNo = fopen_s(&pfFraudDataFile, pcFraudDataFileName, "a+");

    // ���ގ��s���f�[�^�t�@�C�����I�[�v���o�������`�F�b�N
    if (NULL != pfFraudDataFile)
    {
        // ���ގ��s���f�[�^�t�@�C���I�[�v���ɐ������Ă���ꍇ
        // ���ގ��s���f�[�^�t�@�C���ɏo��
        fprintf(pfFraudDataFile, "%s\n", pcFraudData);
        sprintf_s(
            m_cLogData,
            sizeof(m_cLogData),
            "���ގ��s���f�[�^�́F%s", pcFraudData);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

        // ���ގ��s���f�[�^�t�@�C�����N���[�Y
        // ���ގ��s���f�[�^�t�@�C�����N���[�Y�o���Ȃ��Ă����b�Z�[�W���o�����ɏ����I��
        iCloseResult = fclose(pfFraudDataFile);
    }

    // �㏈��
    // �������擾����
    // ���ގ��s���f�[�^�t�@�C���o�͏I�������擾
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���ގ��s���f�[�^�t�@�C���o�͏I�� *****");
    m_stTimeData = g_clsRecOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData,
        sizeof(m_cLogData),
        "���ގ��s���f�[�^�t�@�C���o�͏I�������F%s %s",
        (char *)m_stTimeData.strMachDatePlus.c_str(),
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
}

//********************************************
// �֐����F���ގ��f�[�^�t�@�C���o�͏���
// Function Name�FPrintReportFileProc
// 
// �����T�v�F
// ���ގ��������݃f�[�^�t�@�C����
// �o�̓^�C�v�Ɋ�Â��ē��ގ��f�[�^���o�͂���
//      �^�C�v1�F�h�A���
//      �^�C�v2�F�J�[�hID�y�у��[�U�[��
//      �^�C�v3�F�ۏ��
//      �^�C�v4�F�s�����ɂ�鐧���G���A���x��NG���P
//      �^�C�v5�F�s�����ɂ�鐧���G���A���x��NG���Q
//      �^�C�v6�F�s�����ɂ�鐧���G���A���x��NG���R
//      �^�C�v7�F�s�����ɂ�鐧���G���A���x��NG���S
//      �^�C�v8�F�s�����ɂ�鐧���G���A���x��NG���T
//      �^�C�v9�F�s�����ɂ�鐧���G���A���x��NG���U
//      �^�C�v10�F��L�ȊO�̏o�͏��i�󔒕������j
// 
// �����菇�F
//   �P�D�f�[�^�o�̓������̈�m��
//   �Q�D���ގ��������݃f�[�^�t�@�C���ւ̏o��
//   �R�D�f�[�^�o�̓������̈���
// 
// �����F�@ ���ގ��������݃f�[�^�t�@�C���|�C���^
// �@�@�@�@ �o�͌`��
// �@�@�@�@ ���ޏo���
// �@�@�@�@ ���ގ����f�[�^�ҏW�p�\����
//          �o�͕�����i�󔒓��j
// �@�@�@�@ �f�[�^�o�̓^�C�v
// �@�@�@�@ �����G���A���x�����P
// �@�@�@�@ �����G���A���x�����Q
// �߂�l�F �Ȃ�
// 
// �⑫����
// �E�f�[�^�o�̓������̈�m�ۂɎ��s�����ꍇ��
//     �������̈�m�ۃG���[�iCM_MEMORYERROR�i109�j�j�Ƃ��ă��O�ɏo�͂���
// 
// 
// �쐬����
// 2020/9/25  s-saito   �V�K�쐬
// 
// �X�V����
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
    // �ϐ��錾
    char cFileOutputData[DATASIZE + 1];
    char cFileOutputNGDataLogout[DATASIZE + 1];
    char *pcFileOutputData = NULL;

    // ������
    memset(cFileOutputData, 0x00, sizeof(cFileOutputData));
    memset(cFileOutputNGDataLogout, 0x00, sizeof(cFileOutputNGDataLogout));

    // �W���o�͂ɓ��{�ꂪ�\�������悤�Ƀ��P�[������{�ɐݒ�
    setlocale(LC_ALL, "japanese");

    // �������̈�m�ۂɎ��s�����ꍇ��NULL�ݒ�
    // ���̐ݒ�ɂ��m�ۂɎ��s�����ꍇ��NULL��Ԃ��悤�ɂ���
    set_new_handler(NULL);

    // �O����
    // �������擾����
    // ���ގ��f�[�^�t�@�C���o�͊J�n�����擾
    m_stTimeData = g_clsRecOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData,
        sizeof(m_cLogData),
        "���ގ��f�[�^�t�@�C���o�͊J�n�����F%s %s",
        (char *)m_stTimeData.strMachDatePlus.c_str(),
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���ގ��f�[�^�t�@�C���o�͊J�n *****");

    try
    {
        // �f�[�^�o�̓������̈�m��
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �f�[�^�o�̓������̈�m�� *****");
        pcFileOutputData = DBG_NEW char[DATASIZE + 1];
        m_stLogData = CreateEventMessageProc(
            ACTIONSET,
            "���ގ��f�[�^�t�@�C���o��");

        // �������̈悪�m�ۏo�������`�F�b�N
        if (NULL == pcFileOutputData)
        {
            // �������̈�̊m�ێ��s
            // ��O�����փX���[
            throw tagErrorCode::MEMORYERROR;
        }

        // �������̏�����
        memset(pcFileOutputData, 0x00, DATASIZE + 1);

        // �������̈�m�ې����̃��b�Z�[�W�����O�ɏo��
        LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, "***** �f�[�^�o�̓������̈�m�ې���I�� *****");
        sprintf_s(
            m_cLogData,
            sizeof(m_cLogData),
            "%s��%s",
            m_stLogData.pcActionName,
            MEMORYCREATESUCCESS);
        LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, m_cLogData);

        // �f�[�^�o�̓^�C�v�����O�ɏo��
        sprintf_s(
            m_cLogData,
            sizeof(m_cLogData),
            "�f�[�^�o�̓^�C�v�́F%d",
            eType);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

        // �又��
        // �t�@�C���ւ̃f�[�^�o��
        // �o�̓^�C�v�ɂďo�͕��@�𕪊�
        switch (eType)
        {
            case tagPrintType::DOOR:
            {
                // ���ޏo�L�^
                // �擾�������ޏo�L�^���o��
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
                    "�o�̓f�[�^�́F%s",
                    pcFileOutputData);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                // ���򔲂�
                break;
            }
            case tagPrintType::CARDID:
            {
                // �J�[�hID�ƃ��[�U�[��
                // �擾�����J�[�hID�ƃ��[�U�[�����o��
                sprintf_s(cFileOutputData, sizeof(cFileOutputData), pcOutputType, stEData.cCardID,
                    stEData.cName);
                sprintf_s(pcFileOutputData, DATASIZE, "%s", cFileOutputData);
                fprintf_s(pfOutputFile, "%s", pcFileOutputData);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "�o�̓f�[�^�́F%s",
                    cFileOutputData);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                // ���򔲂�
                break;
            }
            case tagPrintType::ACT:
            {
                // ���ޏo��ԁE�ۏ��
                // �擾�������ޏo���E�ۏ����o��
                sprintf_s(pcFileOutputData, DATASIZE, pcOutputType, pcAct);
                fprintf_s(pfOutputFile, "%s", pcFileOutputData);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "�o�̓f�[�^�́F%s",
                    pcAct);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                // ���򔲂�
                break;
            }
            case tagPrintType::LEVELNG1:
            {
                // �����G���A���x���o�́i�A�����ċL�^�Ȃ��j
                // �{�����ގ��̋L�^������ׂ��s�ɏo��
                sprintf_s(pcFileOutputData, DATASIZE, pcOutputType, iLevel1, iLevel2, pcEtcChar);
                fprintf_s(pfOutputFile, "%s", pcFileOutputData);

                // �w�蕶���폜����
                // ���s�R�[�h���폜��������������O�ɏo��
                strncpy_s(cFileOutputData, sizeof(cFileOutputData), pcFileOutputData, sizeof(cFileOutputData) - 1);
                DeleteDesignationCharProc(cFileOutputData, (const char)CRLF_CODE, cFileOutputNGDataLogout);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "�o�̓f�[�^�́F%s",
                    cFileOutputNGDataLogout);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                // ���򔲂�
                break;
            }
            case tagPrintType::LEVELNG2:
            {
                // �����G���A���x���o�́i�A�����ċL�^����j
                // ���ޏo��Ԃ����O�ɏo��
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "���݂̓��ޏo��Ԃ́F%s",
                    stEData.cDir);
                LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

                // ���ޏo��Ԃ��`�F�b�N
                if (0 == strncmp(stEData.cDir, NYUSHITSU, RECORD_DIR))
                {
                    // ���ޏo��Ԃ��wI�x�i�����j�̏ꍇ
                    // ���݂̃h�A���x���l���}�C�i�X���`�F�b�N
                    if (0 > m_iNowDoorLevel)
                    {
                        // ���݂̃h�A���x���l���}�C�i�X�l
                        // �{�����ގ��̋L�^������ׂ��s��2�s�����o��
                        // 1�s��
                        sprintf_s(pcFileOutputData, DATASIZE, pcOutputType, iLevel1, iLevel2 - 1, pcEtcChar);
                        fprintf_s(pfOutputFile, "%s", pcFileOutputData);

                        // �w�蕶���폜����
                        // ���s�R�[�h���폜��������������O�ɏo��
                        strncpy_s(cFileOutputData, sizeof(cFileOutputData), pcFileOutputData, sizeof(cFileOutputData) - 1);
                        DeleteDesignationCharProc(cFileOutputData, (const char)CRLF_CODE, cFileOutputNGDataLogout);
                        sprintf_s(
                            m_cLogData,
                            sizeof(m_cLogData),
                            "�o�̓f�[�^�́F%s",
                            cFileOutputNGDataLogout);
                        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                        // 2�s��
                        sprintf_s(pcFileOutputData, DATASIZE, pcOutputType, iLevel1 + 1, iLevel2, pcEtcChar);
                        fprintf_s(pfOutputFile, "%s", pcFileOutputData);

                        // �w�蕶���폜����
                        // ���s�R�[�h���폜��������������O�ɏo��
                        strncpy_s(cFileOutputData, sizeof(cFileOutputData), pcFileOutputData, sizeof(cFileOutputData) - 1);
                        DeleteDesignationCharProc(cFileOutputData, (const char)CRLF_CODE, cFileOutputNGDataLogout);
                        sprintf_s(
                            m_cLogData,
                            sizeof(m_cLogData),
                            "�o�̓f�[�^�́F%s",
                            cFileOutputNGDataLogout);
                        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
                    }
                    else
                    {
                        // ���݂̃h�A���x���l���}�C�i�X�l�ł͂Ȃ�
                        // �{�����ގ��̋L�^������ׂ��s��2�s�����o��
                        // 1�s��
                        sprintf_s(pcFileOutputData, DATASIZE, pcOutputType, iLevel1, iLevel2 + 1, pcEtcChar);
                        fprintf_s(pfOutputFile, "%s", pcFileOutputData);

                        // �w�蕶���폜����
                        // ���s�R�[�h���폜��������������O�ɏo��
                        strncpy_s(cFileOutputData, sizeof(cFileOutputData), pcFileOutputData, sizeof(cFileOutputData) - 1);
                        DeleteDesignationCharProc(cFileOutputData, (const char)CRLF_CODE, cFileOutputNGDataLogout);
                        sprintf_s(
                            m_cLogData,
                            sizeof(m_cLogData),
                            "�o�̓f�[�^�́F%s",
                            cFileOutputNGDataLogout);
                        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                        // 2�s��
                        sprintf_s(pcFileOutputData, DATASIZE, pcOutputType, iLevel1 - 1, iLevel2, pcEtcChar);
                        fprintf_s(pfOutputFile, "%s", pcFileOutputData);

                        // �w�蕶���폜����
                        // ���s�R�[�h���폜��������������O�ɏo��
                        strncpy_s(cFileOutputData, sizeof(cFileOutputData), pcFileOutputData, sizeof(cFileOutputData) - 1);
                        DeleteDesignationCharProc(cFileOutputData, (const char)CRLF_CODE, cFileOutputNGDataLogout);
                        sprintf_s(
                            m_cLogData,
                            sizeof(m_cLogData),
                            "�o�̓f�[�^�́F%s",
                            cFileOutputNGDataLogout);
                        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
                    }
                }
                else if (0 == strncmp(stEData.cDir, TAISHITSU, RECORD_DIR))
                {
                    // ���ޏo��Ԃ��wO�x�i�ގ��j�̏ꍇ
                    // �{�����ގ��̋L�^������ׂ��s��2�s�~���o��
                    // 1�s��
                    sprintf_s(pcFileOutputData, DATASIZE, pcOutputType, iLevel1, iLevel2 - 1, pcEtcChar);
                    fprintf_s(pfOutputFile, "%s", pcFileOutputData);

                    // �w�蕶���폜����
                    // ���s�R�[�h���폜��������������O�ɏo��
                    strncpy_s(cFileOutputData, sizeof(cFileOutputData), pcFileOutputData, sizeof(cFileOutputData) - 1);
                    DeleteDesignationCharProc(cFileOutputData, (const char)CRLF_CODE, cFileOutputNGDataLogout);
                    sprintf_s(
                        m_cLogData,
                        sizeof(m_cLogData),
                        "�o�̓f�[�^�́F%s",
                        cFileOutputNGDataLogout);
                    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                    // 2�s��
                    sprintf_s(pcFileOutputData, DATASIZE, pcOutputType, iLevel1 + 1, iLevel2, pcEtcChar);
                    fprintf_s(pfOutputFile, "%s", pcFileOutputData);

                    // �w�蕶���폜����
                    // ���s�R�[�h���폜��������������O�ɏo��
                    strncpy_s(cFileOutputData, sizeof(cFileOutputData), pcFileOutputData, sizeof(cFileOutputData) - 1);
                    DeleteDesignationCharProc(cFileOutputData, (const char)CRLF_CODE, cFileOutputNGDataLogout);
                    sprintf_s(
                        m_cLogData,
                        sizeof(m_cLogData),
                        "�o�̓f�[�^�́F%s",
                        cFileOutputNGDataLogout);
                    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
                }

                // ���򔲂�
                break;
            }
            case tagPrintType::LEVELNG3:
            {
                // �����G���A���x���o�́i�����L�^�Ȃ��j
                // �{�����ގ��̋L�^������ׂ��s��3�s�����o��
                // 1�s��
                sprintf_s(pcFileOutputData, DATASIZE, "***** Level 0-->%d\n%18s", iLevel1 - 2, pcEtcChar);
                fprintf_s(pfOutputFile, "%s", pcFileOutputData);

                // �w�蕶���폜����
                // ���s�R�[�h���폜��������������O�ɏo��
                strncpy_s(cFileOutputData, sizeof(cFileOutputData), pcFileOutputData, sizeof(cFileOutputData) - 1);
                DeleteDesignationCharProc(cFileOutputData, (const char)CRLF_CODE, cFileOutputNGDataLogout);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "�o�̓f�[�^�́F%s",
                    cFileOutputNGDataLogout);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                // 2�s��
                sprintf_s(pcFileOutputData, DATASIZE, pcOutputType, iLevel1 - 2, iLevel1 - 1, pcEtcChar);
                fprintf_s(pfOutputFile, "%s", pcFileOutputData);

                // �w�蕶���폜����
                // ���s�R�[�h���폜��������������O�ɏo��
                strncpy_s(cFileOutputData, sizeof(cFileOutputData), pcFileOutputData, sizeof(cFileOutputData) - 1);
                DeleteDesignationCharProc(cFileOutputData, (const char)CRLF_CODE, cFileOutputNGDataLogout);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "�o�̓f�[�^�́F%s",
                    cFileOutputNGDataLogout);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                // 3�s��
                sprintf_s(pcFileOutputData, DATASIZE, pcOutputType, iLevel1 - 1, iLevel1, pcEtcChar);
                fprintf_s(pfOutputFile, "%s", pcFileOutputData);

                // �w�蕶���폜����
                // ���s�R�[�h���폜��������������O�ɏo��
                strncpy_s(cFileOutputData, sizeof(cFileOutputData), pcFileOutputData, sizeof(cFileOutputData) - 1);
                DeleteDesignationCharProc(cFileOutputData, (const char)CRLF_CODE, cFileOutputNGDataLogout);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "�o�̓f�[�^�́F%s",
                    cFileOutputNGDataLogout);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                // ���򔲂�
                break;
            }
            case tagPrintType::LEVELNG4:
            {
                // �����G���A���x���o�́i�ގ��L�^�Ȃ��j
                // �{�����ގ��̋L�^������ׂ��s��3�s�~���o��
                // 1�s��
                sprintf_s(pcFileOutputData, DATASIZE, pcOutputType, pcEtcChar, iLevel1, iLevel1 - 1);
                fprintf_s(pfOutputFile, "%s", pcFileOutputData);

                // �w�蕶���폜����
                // ���s�R�[�h���폜��������������O�ɏo��
                strncpy_s(cFileOutputData, sizeof(cFileOutputData), pcFileOutputData, sizeof(cFileOutputData) - 1);
                DeleteDesignationCharProc(cFileOutputData, (const char)CRLF_CODE, cFileOutputNGDataLogout);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "�o�̓f�[�^�́F%s",
                    cFileOutputNGDataLogout);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                // 2�s��
                sprintf_s(pcFileOutputData, DATASIZE, pcOutputType, pcEtcChar, iLevel1 - 1, iLevel1 - 2);
                fprintf_s(pfOutputFile, "%s", pcFileOutputData);

                // �w�蕶���폜����
                // ���s�R�[�h���폜��������������O�ɏo��
                strncpy_s(cFileOutputData, sizeof(cFileOutputData), pcFileOutputData, sizeof(cFileOutputData) - 1);
                DeleteDesignationCharProc(cFileOutputData, (const char)CRLF_CODE, cFileOutputNGDataLogout);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "�o�̓f�[�^�́F%s",
                    cFileOutputNGDataLogout);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                // 3�s��
                sprintf_s(pcFileOutputData, DATASIZE, "%18s***** Level %d-->0\n", pcEtcChar, iLevel1 - 2);
                fprintf_s(pfOutputFile, "%s", pcFileOutputData);

                // �w�蕶���폜����
                // ���s�R�[�h���폜��������������O�ɏo��
                strncpy_s(cFileOutputData, sizeof(cFileOutputData), pcFileOutputData, sizeof(cFileOutputData) - 1);
                DeleteDesignationCharProc(cFileOutputData, (const char)CRLF_CODE, cFileOutputNGDataLogout);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "�o�̓f�[�^�́F%s",
                    cFileOutputNGDataLogout);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                // ���򔲂�
                break;
            }
            case tagPrintType::LEVELNG5:
            {
                // �����G���A���x���o��
                // �{�����ގ��̋L�^������ׂ��s��2�s�~���o��
                // 1�s��
                sprintf_s(pcFileOutputData, DATASIZE, pcOutputType, pcEtcChar, iLevel1, iLevel1 - 1);
                fprintf_s(pfOutputFile, "%s", pcFileOutputData);

                // �w�蕶���폜����
                // ���s�R�[�h���폜��������������O�ɏo��
                strncpy_s(cFileOutputData, sizeof(cFileOutputData), pcFileOutputData, sizeof(cFileOutputData) - 1);
                DeleteDesignationCharProc(cFileOutputData, (const char)CRLF_CODE, cFileOutputNGDataLogout);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "�o�̓f�[�^�́F%s",
                    cFileOutputNGDataLogout);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                // 2�s��
                sprintf_s(pcFileOutputData, DATASIZE, "%18s***** Level %d-->0\n", pcEtcChar, iLevel1 - 1);
                fprintf_s(pfOutputFile, "%s", pcFileOutputData);

                // �w�蕶���폜����
                // ���s�R�[�h���폜��������������O�ɏo��
                strncpy_s(cFileOutputData, sizeof(cFileOutputData), pcFileOutputData, sizeof(cFileOutputData) - 1);
                DeleteDesignationCharProc(cFileOutputData, (const char)CRLF_CODE, cFileOutputNGDataLogout);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "�o�̓f�[�^�́F%s",
                    cFileOutputNGDataLogout);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                // ���򔲂�
                break;
            }
            case tagPrintType::LEVELNG6:
            {
                // �����G���A���x���w0�x�o��
                // �{�����ގ��̋L�^������ׂ��s�ɏo��
                sprintf_s(pcFileOutputData, DATASIZE, pcOutputType, pcEtcChar, iLevel1);
                fprintf_s(pfOutputFile, "%s", pcFileOutputData);

                // �w�蕶���폜����
                // ���s�R�[�h���폜��������������O�ɏo��
                strncpy_s(cFileOutputData, sizeof(cFileOutputData), pcFileOutputData, sizeof(cFileOutputData) - 1);
                DeleteDesignationCharProc(cFileOutputData, (const char)CRLF_CODE, cFileOutputNGDataLogout);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "�o�̓f�[�^�́F%s",
                    cFileOutputNGDataLogout);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                // ���򔲂�
                break;
            }
            default:
            {
                // ��L�ȊO
                // ��������o��
                sprintf_s(pcFileOutputData, DATASIZE, pcOutputType, pcEtcChar);
                fprintf_s(pfOutputFile, pcOutputType, pcFileOutputData);

                // ���p�󔒕������`�F�b�N
                if (0 == strncmp(pcEtcChar, " ", 1))
                {
                    // ���p�󔒕���
                    // ���O�Ɂw���pSpace�x�Ɛݒ�
                    sprintf_s(
                        m_cLogData,
                        sizeof(m_cLogData),
                        "�o�̓f�[�^�́F���pSpace");
                }
                else
                {
                    // �擾�����������ݒ�
                    sprintf_s(
                        m_cLogData,
                        sizeof(m_cLogData),
                        "�o�̓f�[�^�́F%s",
                        pcFileOutputData);
                }

                // �f�[�^�����O�ɏo��
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                // ���򔲂�
                break;
            }
        }
    }

    // ��O����
    catch (const ERRORCODETABLE eAllocErr)
    {
        // �G���[���b�Z�[�W��������
            // �G���[���b�Z�[�W���o��
        pcFileOutputData = NULL;
        LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** �f�[�^�o�̓������̈�m�ۈُ�I�� *****");
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �G���[���b�Z�[�W�����֐��Ăяo�� *****");
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
            "�߂�l�́F%d",
            m_stLogData.eErrorCode);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
    }

    // �㏈��
    // �f�[�^�o�̓������̈�����
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �f�[�^�o�̓������̈��� *****");
    SAFE_FREE_NEWALLOC_ARRAY(pcFileOutputData);

    // �������̈����̃��b�Z�[�W�����O�ɏo��
    LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, "***** �f�[�^�o�̓������̈����I�� *****");
    sprintf_s(
        m_cLogData,
        sizeof(m_cLogData),
        "%s��%s",
        m_stLogData.pcActionName,
        MEMORYRELEASESUCCESS);
    LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, m_cLogData);

    // �������擾����
    // ���ގ��f�[�^�t�@�C���o�͏I�������擾
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���ގ��f�[�^�t�@�C���o�͏I�� *****");
    m_stTimeData = g_clsRecOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData,
        sizeof(m_cLogData),
        "���ގ��f�[�^�t�@�C���o�͏I�������F%s %s",
        (char *)m_stTimeData.strMachDatePlus.c_str(),
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
}

//********************************************
// �֐����F���ގ��f�[�^PC�{����ʏo�͏���
// Function Name�FPrintScreenProc
// 
// �����T�v�F
// PC���{����ʂ֏o�̓^�C�v�Ɋ�Â��ē��ގ��f�[�^���o�͂���
//      �^�C�v1�F�h�A���
//      �^�C�v2�F�J�[�hID�y�у��[�U�[��
//      �^�C�v3�F�ۏ��
//      �^�C�v4�F�s�����ɂ�鐧���G���A���x��NG���P
//      �^�C�v5�F�s�����ɂ�鐧���G���A���x��NG���Q
//      �^�C�v6�F�s�����ɂ�鐧���G���A���x��NG���R
//      �^�C�v7�F�s�����ɂ�鐧���G���A���x��NG���S
//      �^�C�v8�F�s�����ɂ�鐧���G���A���x��NG���T
//      �^�C�v9�F�s�����ɂ�鐧���G���A���x��NG���U
//      �^�C�v10�F��L�ȊO�̏o�͏��i�󔒕������j
// 
// �����菇�F
//   �P�D�f�[�^�o�̓������̈�m��
//   �Q�DPC�{����ʂւ̏o��
//   �R�D�f�[�^�o�̓������̈���
// 
// �����F�@ �o�͌`��
// �@�@�@�@ ���ޏo���
// �@�@�@�@ ���ގ����f�[�^�ҏW�p�\����
//          �o�͕�����i�󔒓��j
// �@�@�@�@ �f�[�^�o�̓^�C�v
// �@�@�@�@ �����G���A���x�����P
// �@�@�@�@ �����G���A���x�����Q
// �߂�l�F �Ȃ�
// 
// �⑫����
// �E�f�[�^�o�̓������̈�m�ۂɎ��s�����ꍇ��
//     �������̈�m�ۃG���[�iCM_MEMORYERROR�i109�j�j�Ƃ��ă��O�ɏo�͂���
// 
// 
// �쐬����
// 2020/9/25  s-saito   �V�K�쐬
// 
// �X�V����
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
    // �ϐ��錾
    char cScreenOutputData[DATASIZE + 1];
    char cScreenOutputNGDataLogout[DATASIZE + 1];
    char cScreenOutputNODataLogout[DATASIZE + 1];
    char *pcScreenOutputData = NULL;

    // ������
    memset(cScreenOutputData, 0x00, sizeof(cScreenOutputData));
    memset(cScreenOutputNGDataLogout, 0x00, sizeof(cScreenOutputNGDataLogout));
    memset(cScreenOutputNODataLogout, 0x00, sizeof(cScreenOutputNODataLogout));

    // �W���o�͂ɓ��{�ꂪ�\�������悤�Ƀ��P�[������{�ɐݒ�
    setlocale(LC_ALL, "japanese");

    // �O����
    // �������擾����
    // ���ގ��f�[�^PC�{����ʏo�͊J�n�����擾
    m_stTimeData = g_clsRecOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData,
        sizeof(m_cLogData),
        "���ގ��f�[�^PC�{����ʏo�͊J�n�����F%s %s",
        (char *)m_stTimeData.strMachDatePlus.c_str(),
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���ގ��f�[�^PC�{����ʏo�͊J�n *****");

    try
    {
        // �f�[�^�o�̓������̈�m��
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �f�[�^�o�̓������̈�m�� *****");
        pcScreenOutputData = (char *)calloc(DATASIZE + 1, sizeof(char));
        m_stLogData = CreateEventMessageProc(
            ACTIONSET,
            "���ގ��f�[�^PC�{����ʏo��");

        // �������̈悪�m�ۏo�������`�F�b�N
        if (NULL == pcScreenOutputData)
        {
            // �������̈�̊m�ێ��s
            // ��O�����փX���[
            throw tagErrorCode::MEMORYERROR;
        }

        // �������̈�m�ې����̃��b�Z�[�W�����O�ɏo��
        LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, "***** �f�[�^�o�̓������̈�m�ې���I�� *****");
        sprintf_s(
            m_cLogData,
            sizeof(m_cLogData),
            "%s��%s",
            m_stLogData.pcActionName,
            MEMORYCREATESUCCESS);
        LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, m_cLogData);

        // �f�[�^�o�̓^�C�v�����O�ɏo��
        sprintf_s(m_cLogData, sizeof(m_cLogData), "�f�[�^�o�̓^�C�v�́F%d", eType);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

        // �又��
        // �R���\�[���E�t�@�C���ւ̃f�[�^�o��
        // �o�̓^�C�v�ɂďo�͕��@�𕪊�
        switch (eType)
        {
            case tagPrintType::DOOR:
            {
                // ���ޏo�L�^
                // �擾�������ޏo�L�^���o��
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
                    "PC�{����ʂ̃f�[�^�́F%s",
                    pcScreenOutputData);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                // ���򔲂�
                break;
            }
            case tagPrintType::CARDID:
            {
                // �J�[�hID�ƃ��[�U�[��
                // �擾�����J�[�hID�ƃ��[�U�[�����o��
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
                    "PC�{����ʂ̃f�[�^�́F%s",
                    cScreenOutputData);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                // ���򔲂�
                break;
            }
            case tagPrintType::ACT:
            {
                // ���ޏo���E�ۏ��
                // �擾�������ޏo���E�ۏ����o��
                sprintf_s(pcScreenOutputData, DATASIZE, pcOutputType, pcAct);
                printf_s("%s", pcScreenOutputData);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "PC�{����ʂ̃f�[�^�́F%s",
                    pcAct);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                // ���򔲂�
                break;
            }
            case tagPrintType::LEVELNG1:
            {
                // �����G���A���x��NG�o��
                // �{�����ގ��̋L�^������ׂ��s�ɏo��
                sprintf_s(pcScreenOutputData, DATASIZE, pcOutputType, iLevel1, iLevel2, pcEtcChar);
                printf_s("%s", pcScreenOutputData);

                // �w�蕶���폜����
                // ���s�R�[�h���폜��������������O�ɏo��
                strncpy_s(
                    cScreenOutputData,
                    sizeof(cScreenOutputData),
                    pcScreenOutputData,
                    sizeof(cScreenOutputData) - 1);
                DeleteDesignationCharProc(cScreenOutputData, (const char)CRLF_CODE, cScreenOutputNGDataLogout);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "PC�{����ʂ̃f�[�^�́F%s",
                    cScreenOutputNGDataLogout);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                // ���򔲂�
                break;
            }
            case tagPrintType::LEVELNG2:
            {
                // �����G���A���x��NG�o�́i�A�����ċL�^�Ȃ��j
                // ���ޏo��Ԃ����O�ɏo��
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "���݂̓��ޏo��Ԃ́F%s",
                    stEData.cDir);
                LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

                // ���ޏo��Ԃ��`�F�b�N
                if (0 == strncmp(stEData.cDir, NYUSHITSU, RECORD_DIR))
                {
                    // ���ޏo��Ԃ��wI�x�i�����j�̏ꍇ
                    // ���݂̃h�A���x���l���}�C�i�X���`�F�b�N
                    if (0 > m_iNowDoorLevel)
                    {
                        // ���݂̃h�A���x���l���}�C�i�X�l
                        // �{�����ގ��̋L�^������ׂ��s��2�s�����o��
                        // 1�s��
                        sprintf_s(pcScreenOutputData, DATASIZE, pcOutputType, iLevel1, iLevel2 - 1, pcEtcChar);
                        printf_s("%s", pcScreenOutputData);

                        // �w�蕶���폜����
                        // ���s�R�[�h���폜��������������O�ɏo��
                        strncpy_s(
                            cScreenOutputData,
                            sizeof(cScreenOutputData),
                            pcScreenOutputData,
                            sizeof(cScreenOutputData) - 1);
                        DeleteDesignationCharProc(cScreenOutputData, (const char)CRLF_CODE, cScreenOutputNGDataLogout);
                        sprintf_s(
                            m_cLogData,
                            sizeof(m_cLogData),
                            "PC�{����ʂ̃f�[�^�́F%s",
                            cScreenOutputNGDataLogout);
                        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                        // 2�s��
                        sprintf_s(pcScreenOutputData, DATASIZE, pcOutputType, iLevel1 + 1, iLevel2, pcEtcChar);
                        printf_s("%s", pcScreenOutputData);

                        // �w�蕶���폜����
                        // ���s�R�[�h���폜��������������O�ɏo��
                        strncpy_s(
                            cScreenOutputData,
                            sizeof(cScreenOutputData),
                            pcScreenOutputData,
                            sizeof(cScreenOutputData) - 1);
                        DeleteDesignationCharProc(cScreenOutputData, (const char)CRLF_CODE, cScreenOutputNGDataLogout);
                        sprintf_s(
                            m_cLogData,
                            sizeof(m_cLogData),
                            "PC�{����ʂ̃f�[�^�́F%s",
                            cScreenOutputNGDataLogout);
                        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
                    }
                    else
                    {
                        // ���݂̃h�A���x���l���}�C�i�X�l�ł͂Ȃ�
                        // �{�����ގ��̋L�^������ׂ��s��2�s�����o��
                        // 1�s��
                        sprintf_s(pcScreenOutputData, DATASIZE, pcOutputType, iLevel1, iLevel2 + 1, pcEtcChar);
                        printf_s("%s", pcScreenOutputData);

                        // �w�蕶���폜����
                        // ���s�R�[�h���폜��������������O�ɏo��
                        strncpy_s(
                            cScreenOutputData,
                            sizeof(cScreenOutputData),
                            pcScreenOutputData,
                            sizeof(cScreenOutputData) - 1);
                        DeleteDesignationCharProc(cScreenOutputData, (const char)CRLF_CODE, cScreenOutputNGDataLogout);
                        sprintf_s(
                            m_cLogData,
                            sizeof(m_cLogData),
                            "PC�{����ʂ̃f�[�^�́F%s",
                            cScreenOutputNGDataLogout);
                        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                        // 2�s��
                        sprintf_s(pcScreenOutputData, DATASIZE, pcOutputType, iLevel1 - 1, iLevel2, pcEtcChar);
                        printf_s("%s", pcScreenOutputData);

                        // �w�蕶���폜����
                        // ���s�R�[�h���폜��������������O�ɏo��
                        strncpy_s(
                            cScreenOutputData,
                            sizeof(cScreenOutputData),
                            pcScreenOutputData,
                            sizeof(cScreenOutputData) - 1);
                        DeleteDesignationCharProc(cScreenOutputData, (const char)CRLF_CODE, cScreenOutputNGDataLogout);
                        sprintf_s(
                            m_cLogData,
                            sizeof(m_cLogData),
                            "PC�{����ʂ̃f�[�^�́F%s",
                            cScreenOutputNGDataLogout);
                        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
                    }
                }
                else if (0 == strncmp(stEData.cDir, TAISHITSU, RECORD_DIR))
                {
                    // ���ޏo��Ԃ��wO�x�i�ގ��j�̏ꍇ
                    // �{�����ގ��̋L�^������ׂ��s��2�s�~���o��
                    // 1�s��
                    sprintf_s(pcScreenOutputData, DATASIZE, pcOutputType, iLevel1, iLevel2 - 1, pcEtcChar);
                    printf_s("%s", pcScreenOutputData);

                    // �w�蕶���폜����
                    // ���s�R�[�h���폜��������������O�ɏo��
                    strncpy_s(
                        cScreenOutputData,
                        sizeof(cScreenOutputData),
                        pcScreenOutputData,
                        sizeof(cScreenOutputData) - 1);
                    DeleteDesignationCharProc(cScreenOutputData, (const char)CRLF_CODE, cScreenOutputNGDataLogout);
                    sprintf_s(
                        m_cLogData,
                        sizeof(m_cLogData),
                        "PC�{����ʂ̃f�[�^�́F%s",
                        cScreenOutputNGDataLogout);
                    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                    // 2�s��
                    sprintf_s(pcScreenOutputData, DATASIZE, pcOutputType, iLevel1 + 1, iLevel2, pcEtcChar);
                    printf_s("%s", pcScreenOutputData);

                    // �w�蕶���폜����
                    // ���s�R�[�h���폜��������������O�ɏo��
                    strncpy_s(
                        cScreenOutputData,
                        sizeof(cScreenOutputData),
                        pcScreenOutputData,
                        sizeof(cScreenOutputData) - 1);
                    DeleteDesignationCharProc(cScreenOutputData, (const char)CRLF_CODE, cScreenOutputNGDataLogout);
                    sprintf_s(
                        m_cLogData,
                        sizeof(m_cLogData),
                        "PC�{����ʂ̃f�[�^�́F%s",
                        cScreenOutputNGDataLogout);
                    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);
                }

                // ���򔲂�
                break;
            }
            case tagPrintType::LEVELNG3:
            {
                // �����G���A���x��NG�o�́i�����L�^�Ȃ��j
                // �{�����ގ��̋L�^������ׂ��s��3�s�����o��
                // 1�s��
                sprintf_s(pcScreenOutputData, DATASIZE, "***** Level 0-->%d\n%18s", iLevel1 - 2, pcEtcChar);
                printf_s("%s", pcScreenOutputData);

                // �w�蕶���폜����
                // ���s�R�[�h���폜��������������O�ɏo��
                strncpy_s(
                    cScreenOutputData,
                    sizeof(cScreenOutputData),
                    pcScreenOutputData,
                    sizeof(cScreenOutputData) - 1);
                DeleteDesignationCharProc(cScreenOutputData, (const char)CRLF_CODE, cScreenOutputNGDataLogout);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "PC�{����ʂ̃f�[�^�́F%s",
                    cScreenOutputNGDataLogout);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                // 2�s��
                sprintf_s(pcScreenOutputData, DATASIZE, pcOutputType, iLevel1 - 2, iLevel1 - 1, pcEtcChar);
                printf_s("%s", pcScreenOutputData);

                // �w�蕶���폜����
                // ���s�R�[�h���폜��������������O�ɏo��
                strncpy_s(
                    cScreenOutputData,
                    sizeof(cScreenOutputData),
                    pcScreenOutputData,
                    sizeof(cScreenOutputData) - 1);
                DeleteDesignationCharProc(cScreenOutputData, (const char)CRLF_CODE, cScreenOutputNGDataLogout);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "PC�{����ʂ̃f�[�^�́F%s",
                    cScreenOutputNGDataLogout);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                // 3�s��
                sprintf_s(pcScreenOutputData, DATASIZE, pcOutputType, iLevel1 - 1, iLevel1, pcEtcChar);
                printf_s("%s", pcScreenOutputData);

                // �w�蕶���폜����
                // ���s�R�[�h���폜��������������O�ɏo��
                strncpy_s(
                    cScreenOutputData,
                    sizeof(cScreenOutputData),
                    pcScreenOutputData,
                    sizeof(cScreenOutputData) - 1);
                DeleteDesignationCharProc(cScreenOutputData, (const char)CRLF_CODE, cScreenOutputNGDataLogout);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "PC�{����ʂ̃f�[�^�́F%s",
                    cScreenOutputNGDataLogout);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                // ���򔲂�
                break;
            }
            case tagPrintType::LEVELNG4:
            {
                // �����G���A���x��NG�o�́i�ގ��L�^�Ȃ��j
                // �{�����ގ��̋L�^������ׂ��s��3�s�~���o��
                // 1�s��
                sprintf_s(pcScreenOutputData, DATASIZE, pcOutputType, pcEtcChar, iLevel1, iLevel1 - 1);
                printf_s("%s", pcScreenOutputData);

                // �w�蕶���폜����
                // ���s�R�[�h���폜��������������O�ɏo��
                strncpy_s(
                    cScreenOutputData,
                    sizeof(cScreenOutputData),
                    pcScreenOutputData,
                    sizeof(cScreenOutputData) - 1);
                DeleteDesignationCharProc(cScreenOutputData, (const char)CRLF_CODE, cScreenOutputNGDataLogout);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "PC�{����ʂ̃f�[�^�́F%s",
                    cScreenOutputNGDataLogout);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                // 2�s��
                sprintf_s(pcScreenOutputData, DATASIZE, pcOutputType, pcEtcChar, iLevel1 - 1, iLevel1 - 2);
                printf_s("%s", pcScreenOutputData);

                // �w�蕶���폜����
                // ���s�R�[�h���폜��������������O�ɏo��
                strncpy_s(
                    cScreenOutputData,
                    sizeof(cScreenOutputData),
                    pcScreenOutputData,
                    sizeof(cScreenOutputData) - 1);
                DeleteDesignationCharProc(cScreenOutputData, (const char)CRLF_CODE, cScreenOutputNGDataLogout);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "PC�{����ʂ̃f�[�^�́F%s",
                    cScreenOutputNGDataLogout);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                // 3�s��
                sprintf_s(pcScreenOutputData, DATASIZE, "%18s***** Level %d-->0\n", pcEtcChar, iLevel1 - 2);
                printf_s("%s", pcScreenOutputData);

                // �w�蕶���폜����
                // ���s�R�[�h���폜��������������O�ɏo��
                strncpy_s(
                    cScreenOutputData,
                    sizeof(cScreenOutputData),
                    pcScreenOutputData,
                    sizeof(cScreenOutputData) - 1);
                DeleteDesignationCharProc(cScreenOutputData, (const char)CRLF_CODE, cScreenOutputNGDataLogout);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "PC�{����ʂ̃f�[�^�́F%s",
                    cScreenOutputNGDataLogout);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                // ���򔲂�
                break;
            }
            case tagPrintType::LEVELNG5:
            {
                // �����G���A���x��NG�o��
                // �{�����ގ��̋L�^������ׂ��s��2�s�~���o��
                // 1�s��
                sprintf_s(pcScreenOutputData, DATASIZE, pcOutputType, pcEtcChar, iLevel1, iLevel1 - 1);
                printf_s("%s", pcScreenOutputData);

                // �w�蕶���폜����
                // ���s�R�[�h���폜��������������O�ɏo��
                strncpy_s(
                    cScreenOutputData,
                    sizeof(cScreenOutputData),
                    pcScreenOutputData,
                    sizeof(cScreenOutputData) - 1);
                DeleteDesignationCharProc(cScreenOutputData, (const char)CRLF_CODE, cScreenOutputNGDataLogout);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "PC�{����ʂ̃f�[�^�́F%s",
                    cScreenOutputNGDataLogout);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                // 2�s��
                sprintf_s(pcScreenOutputData, DATASIZE, "%18s***** Level %d-->0\n", pcEtcChar, iLevel1 - 1);
                printf_s("%s", pcScreenOutputData);

                // �w�蕶���폜����
                // ���s�R�[�h���폜��������������O�ɏo��
                strncpy_s(
                    cScreenOutputData,
                    sizeof(cScreenOutputData),
                    pcScreenOutputData,
                    sizeof(cScreenOutputData) - 1);
                DeleteDesignationCharProc(cScreenOutputData, (const char)CRLF_CODE, cScreenOutputNGDataLogout);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "PC�{����ʂ̃f�[�^�́F%s",
                    cScreenOutputNGDataLogout);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                // ���򔲂�
                break;
            }
            case tagPrintType::LEVELNG6:
            {
                // �����G���A���x��NG�o��
                // �{�����ގ��̋L�^������ׂ��s�ɏo��
                sprintf_s(pcScreenOutputData, DATASIZE, pcOutputType, pcEtcChar, iLevel1);
                printf_s("%s", pcScreenOutputData);

                // �w�蕶���폜����
                // ���s�R�[�h���폜��������������O�ɏo��
                strncpy_s(
                    cScreenOutputData,
                    sizeof(cScreenOutputData),
                    pcScreenOutputData,
                    sizeof(cScreenOutputData) - 1);
                DeleteDesignationCharProc(cScreenOutputData, (const char)CRLF_CODE, cScreenOutputNGDataLogout);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "PC�{����ʂ̃f�[�^�́F%s",
                    cScreenOutputNGDataLogout);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                // ���򔲂�
                break;
            }
            case tagPrintType::DUMMYDATA:
            {
                // �_�~�[�f�[�^
                // ��������o��
                sprintf_s(pcScreenOutputData, DATASIZE, pcOutputType, pcEtcChar);
                printf_s(pcOutputType, pcScreenOutputData);

                // �w�蕶���폜����
                // ���s�R�[�h���폜��������������O�ɏo��
                strncpy_s(
                    cScreenOutputData,
                    sizeof(cScreenOutputData),
                    pcScreenOutputData,
                    sizeof(cScreenOutputData) - 1);
                DeleteDesignationCharProc(cScreenOutputData, (const char)CRLF_CODE, cScreenOutputNODataLogout);
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "PC�{����ʂ̃f�[�^�́F%s",
                    cScreenOutputNODataLogout);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                // ���򔲂�
                break;
            }
            default:
            {
                // ��L�ȊO
                // ��������o��
                sprintf_s(pcScreenOutputData, DATASIZE, pcOutputType, pcEtcChar);
                printf_s(pcOutputType, pcScreenOutputData);

                // ���p�󔒕������`�F�b�N
                if (0 == strncmp(pcEtcChar, " ", 1))
                {
                    // ���p�󔒕���
                    // ���O�Ɂw���pSpace�x�Ɛݒ�
                    sprintf_s(
                        m_cLogData,
                        sizeof(m_cLogData),
                        "PC�{����ʂ̃f�[�^�́F���pSpace");
                }
                else
                {
                    // �擾�����������ݒ�
                    sprintf_s(
                        m_cLogData,
                        sizeof(m_cLogData),
                        "PC�{����ʂ̃f�[�^�́F%s",
                        pcScreenOutputData);
                }

                // �f�[�^�����O�ɏo��
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

                // ���򔲂�
                break;
            }
        }
    }

    // ��O����
    catch (const ERRORCODETABLE eAllocErr)
    {
        // �G���[���b�Z�[�W��������
        // �G���[���b�Z�[�W���o��
        pcScreenOutputData = NULL;
        LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** �f�[�^�o�̓������̈�m�ۈُ�I�� *****");
        LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** �G���[���b�Z�[�W�����֐��Ăяo�� *****");
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
            "�߂�l�́F%d",
            m_stLogData.eErrorCode);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
    }

    // �㏈��
    // �f�[�^�o�̓������̈�����
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �f�[�^�o�̓������̈��� *****");
#ifdef _DEBUG
    SAFE_FREE_DEBUG(pcScreenOutputData);
#else
    SAFE_FREE(pcScreenOutputData);
#endif // _DEBUG

    // �������̈����̃��b�Z�[�W�����O�ɏo��
    LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, "***** �f�[�^�o�̓������̈����I�� *****");
    sprintf_s(
        m_cLogData,
        sizeof(m_cLogData),
        "%s��%s", m_stLogData.pcActionName,
        MEMORYRELEASESUCCESS);
    LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, m_cLogData);

    // �������擾����
    // ���ގ��f�[�^PC�{����ʏo�͏I�������擾
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���ގ��f�[�^PC�{����ʏo�͏I�� *****");
    m_stTimeData = g_clsRecOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData,
        sizeof(m_cLogData),
        "���ގ��f�[�^PC�{����ʏo�͏I�������F%s %s",
        (char *)m_stTimeData.strMachDatePlus.c_str(),
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
}
