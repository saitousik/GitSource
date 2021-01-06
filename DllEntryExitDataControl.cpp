//============================================
// ���ގ��󋵈󎚐���\�[�X�t�@�C��
// DllEntryExitDataControl.cpp
// 
// �@�\�T�v
// �t���A�̃Z�L�����e�B�ɂ��h�A�̓��ގ��ǂݍ��݃f�[�^�t�@�C����ǂݍ��݁A
// �e�L�X�g���ł̒��[�`���œ��ގ��������݃f�[�^�t�@�C���ֈ󎚂���
// 
// �@�\���e
// �P�D���ގ��󋵈󎚐��䃁�C������
// �Q�D���ގ��󋵈󎚐���J�n����
// �R�D���ގ��󋵈󎚐���I������
// 
// �����菇
//  �P�D���ގ��ǂݍ��݃f�[�^�t�@�C���̓ǂݍ���
//  �Q�D���ގ��󋵂̃`�F�b�N
//  �R�D���ގ��󋵂ɑ΂���PC���{����ʂւ̕\��
//  �S�D���ގ��󋵂ɑ΂�����ގ��������݃f�[�^�t�@�C���ւ̏�������
// 
// �⑫����
// �E���@�\�̏ڍׂ��Ԃɂ��Ă͋��ʋ@�\���Ăяo�����O���Ƃ��ă��O�o�̓t�@�C���ɏo�͂���
// �E���@�\�ł͏�L�̑����ʋ@�\���Ăяo���o�b�N�A�b�v�t�@�C���������΃p�X�ւ̕ϊ������s��
// �E���@�\�ł̓��������[�N�ɑ΂���CRT���������m�C�x���g�n���h���������s��
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
// �e�w�b�_�[�C���N���[�h�E�N���X��`
//��������������������������������������������
// �C���N���[�h��`
#include "DllEntryExitDataControl.h"

// �N���X��`
CLogFile g_clsMainLogFile;
CFileOperations g_clsMainFileOpe;
CSubOperations g_clsMainSubOpe;
CSubOperations *g_pclsMainSubOpe;
CFileOperations *g_pclsMainFileOpe;
CRecordOperations *g_pclsMainRecOpe;

// �}�N����`
#define LOGOUTPUT(iOutputType, iTraceOutputLogType, cMessageInfo) \
    g_clsMainLogFile.LogOutputControlProc(LOGARGS, iOutputType, iTraceOutputLogType, cMessageInfo)
#define LOGOUTPUTERROR(iOutputType, iTraceOutputLogType, cErrorMessageInfo, eErrorInfoCode) \
    g_clsMainLogFile.LogOutputControlProc(LOGARGS, iOutputType, iTraceOutputLogType, cErrorMessageInfo, eErrorInfoCode)

// �O���[�o���ϐ�
static map<int, ERRORCODETABLE> g_maStartEndErrorCode;
int g_iParseArgv;
vector<string> g_vecParseArgc;
FILENAMEINFORMATION g_stFileNameInfo;
map<FILETYPETABLE, string> g_maFileName;

//��������������������������������������������
// �e�@�\�iAPI�j��`
//��������������������������������������������
//********************************************
// �֐����F���ގ��󋵈󎚐��䃁�C������
// Function Name�FDataControlMain
// 
// �����T�v�F
// ���ގ��ǂݍ��݃f�[�^�t�@�C����ǂݍ��݁A
// PC�{����ʋy�ѓ��ގ��������݃f�[�^�t�@�C���ֈ󎚂���
// 
// �����菇�F
//   �P�D���ގ��󋵈󎚐���J�n����
//   �Q�D���ގ��f�[�^�ҏW���擾�����i�P���ځj�i�f�[�^���Ȃ��ꍇ�̓_�~�[�f�[�^��ݒ�j
//   �R�D���o���o��
// �i�ȍ~��������S�Ẵf�[�^���擾�o����܂Ń��[�v�j
//   �S�D���ގ��f�[�^�ҏW���o�͏���
//   �T�D���ގ��f�[�^�ҏW���擾�����i�Q���ڈȍ~�j
//   �U�D�����G���A���x���N���A����
// �i�����܂Ń��[�v�j
//   �V�D���ގ��󋵈󎚐���I������
//   �W�D�G���[���b�Z�[�WPC�{����ʋy�уg���[�X���O�o�́i�G���[�����������ꍇ�̂݁j
// 
// �����F      �R�}���h��
//             �R�}���h���
// �߂�l�F    ���s���ʁi���ގ��󋵈󎚐��䏉�������^���ގ��󋵈󎚐���I�������̖߂�l�j
//             ����I���FEXIT_SUCCESS�i0�j
//             �ُ�I���F���ގ��󋵈󎚐��䏉�������^���ގ��󋵈󎚐���I�������̖߂�l
//                       ���ގ��f�[�^�ҏW���擾�����̖߂�l
//                         ���R�[�h�Ȃ��FCM_NODATA�i106�j
//                         ���R�[�h�s���FCM_DATAFRAUD�i107�j
//                         ���R�[�h�擾�G���[�FCM_DATAERROR�i108�j
// 
// �⑫����
// �E�t�@�C���ւ̌��o���o�͂͐V�K�쐬�݂̂Ƃ���
// �E���ގ��ǂݍ��݃f�[�^�t�@�C���̑S�Ẵf�[�^���擾�o����܂łS�`�U���J��Ԃ�
// 
// �쐬����
// 2020/9/25  s-saito   �V�K�쐬
// 
// �X�V����
// 
// 
//********************************************
int DataControlMain(
    int iArgv,
    vector<string> vecArgc)
{
    // �ϐ��錾
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

    // ������
    memset(cInitData, 0x00, sizeof(cInitData));
    memset(cInitDataDateTime, 0x00, sizeof(cInitDataDateTime));
    g_clsMainLogFile.m_eErrorResultCode = tagErrorCode::SUCCESS;

    // �O����
    try
    {
        // ��΃p�X�ϊ��t�@�C�����擾����
        // �g���[�X���O�o�̓t�@�C�������΃p�X�t����ԂŎ擾
        eFileType = tagFileType::FILETYPE3;
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ��΃p�X�ϊ��t�@�C�����擾�֐��Ăяo�� *****");
        g_stFileNameInfo.strTraceLogName = g_clsMainFileOpe.GetConvertFileNameProc(
            eFileType, 
            "", 
            vecArgc.at(0).c_str(), 
            (char *)LOGFILEPATH, 
            (char *)LOGFILENAME);

        // ��΃p�X�ϊ��t�@�C�����擾����
        // �f�[�^���O�o�̓t�@�C�������΃p�X�t����ԂŎ擾
        eFileType = tagFileType::FILETYPE4;
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ��΃p�X�ϊ��t�@�C�����擾�֐��Ăяo�� *****");
        g_stFileNameInfo.strDataLogName = g_clsMainFileOpe.GetConvertFileNameProc(
            eFileType, 
            "", 
            vecArgc.at(0).c_str(), 
            (char *)LOGFILEPATH, 
            (char *)DATALOGFILENAME);

        // ��΃p�X�ϊ��t�@�C�����擾����
        // �s���f�[�^�t�@�C�������΃p�X�t����ԂŎ擾
        eFileType = tagFileType::FILETYPE9;
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ��΃p�X�ϊ��t�@�C�����擾�֐��Ăяo�� *****");
        g_stFileNameInfo.strFraudDataName = g_clsMainFileOpe.GetConvertFileNameProc(
            eFileType,
            "",
            vecArgc.at(0).c_str(),
            (char *)WRITEPATH,
            (char *)FRAUDDATAFILENAME);

        // �擾�������O�o�̓t�@�C�����Ɋ�Â��Ċe�N���X����
        // ���O�o�̓N���X
        CLogFile clsMainLogFile(
            g_stFileNameInfo.strTraceLogName, 
            g_stFileNameInfo.strDataLogName);
        g_clsMainLogFile.m_strTraceLogFileName = clsMainLogFile.m_strTraceLogFileName;
        g_clsMainLogFile.m_strDataLogFileName = clsMainLogFile.m_strDataLogFileName;

        // ���ʑ���N���X
        g_pclsMainSubOpe = DBG_NEW CSubOperations(
            g_stFileNameInfo.strTraceLogName, 
            g_stFileNameInfo.strDataLogName);

        // �t�@�C������N���X
        g_pclsMainFileOpe = DBG_NEW CFileOperations(
            g_stFileNameInfo.strTraceLogName, 
            g_stFileNameInfo.strDataLogName, 
            g_stFileNameInfo.strBackupName);

        // ���ގ����R�[�h����N���X
        g_pclsMainRecOpe = DBG_NEW CRecordOperations(
            g_stFileNameInfo.strTraceLogName, 
            g_stFileNameInfo.strDataLogName);

        // �������擾����
        // ���ގ��󋵈󎚏����J�n�����\��
        g_clsMainLogFile.m_stTimeData = g_clsMainLogFile.GetDateTimeProc();
        sprintf_s(
            g_clsMainLogFile.m_cLogData, 
            sizeof(g_clsMainLogFile.m_cLogData), 
            "���ގ��󋵈󎚏����J�n�����F%s %s", 
            (char *)g_clsMainLogFile.m_stTimeData.strMachDatePlus.c_str(), 
            (char *)g_clsMainLogFile.m_stTimeData.strMachTimePlus.c_str());
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, g_clsMainLogFile.m_cLogData);
#ifdef SCREEN
        printf_s("%s\n", g_clsMainLogFile.m_cLogData);
        printf_s("***** ���ގ��󋵈󎚏����J�n *****\n");
#endif // SCREEN
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���ގ��󋵈󎚏����J�n *****");

        // �������擾����
        // �����œ��t�y�ю����̐ݒ��Ԃ��f�[�^���O�ɏo��
        // �����ȊO�ł̓}�V�������̎擾�̂ݍs��
        g_clsMainLogFile.m_stTimeData = g_pclsMainSubOpe->GetDateTimeProc();

        // �p�����[�^���̃R�}���h�������O�ɏo��
        sprintf_s(g_clsMainLogFile.m_cLogData, sizeof(g_clsMainLogFile.m_cLogData), "�p�����[�^���́F %d", iArgv);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, "***** �R�}���h���ڍ� *****");
        for (int iCnt = 0; iCnt < iArgv; iCnt++)
        {
            // �R�}���h�������O�ɏo��
            sprintf_s(
                g_clsMainLogFile.m_cLogData, 
                sizeof(g_clsMainLogFile.m_cLogData), 
                "%d �ڂ̃R�}���h���́F %s", 
                iCnt + 1, 
                vecArgc.at(iCnt).c_str());
            LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, g_clsMainLogFile.m_cLogData);
        }

        // ��΃p�X�ϊ��t�@�C�����擾����
        // �A�v���P�[�V���������΃p�X�t����ԂŎ擾
        eFileType = tagFileType::FILETYPE8;
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ��΃p�X�ϊ��t�@�C�����擾�֐��Ăяo�� *****");

        // �p�����[�^���̃R�}���h�����`�F�b�N
        if (0 != strncmp(
            vecArgc.at(static_cast<size_t>(iArgv) - 1).c_str(), 
            ".", 
            vecArgc.at(static_cast<size_t>(iArgv) - 1).size()) &&
            0 != strncmp(
                vecArgc.at(static_cast<size_t>(iArgv) - 1).c_str(), 
                ".\\", 
                vecArgc.at(static_cast<size_t>(iArgv) - 1).size()))
        {
            // �R�}���h���̖����̕ϊ��w�蕶���񂪂Ȃ�
            // �w.�x�w��ɂăA�v���P�[�V���������擾
            g_stFileNameInfo.strModuleName = g_pclsMainFileOpe->GetConvertFileNameProc(
                eFileType, 
                ".", 
                vecArgc.at(0).c_str());

            // �p�����[�^����1�}�C�i�X
            iParamCnt = iArgv - 1;
            sprintf_s(
                g_clsMainLogFile.m_cLogData, 
                sizeof(g_clsMainLogFile.m_cLogData), 
                "�p�����[�^���́F %d ", 
                iParamCnt);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
        }
        else
        {
            // �R�}���h���̖����̕ϊ��w�蕶���񂪂���
            // �R�}���h���̖����̕ϊ��w�蕶������`�F�b�N
            if (0 == strncmp(
                vecArgc.at(static_cast<size_t>(iArgv) - 1).c_str(), 
                ".", 
                vecArgc.at(static_cast<size_t>(iArgv) - 1).size()))
            {
                // �ϊ��w�蕶���񂪁w.�x
                // �w.�x�w��ɂăA�v���P�[�V���������擾
                g_stFileNameInfo.strModuleName = g_pclsMainFileOpe->GetConvertFileNameProc(
                    eFileType, 
                    ".", 
                    vecArgc.at(0).c_str());
            }
            else
            {
                // �ϊ��w�蕶���񂪁w.\\�x
                // �w.\\�x�w��ɂăA�v���P�[�V���������擾
                g_stFileNameInfo.strModuleName = g_pclsMainFileOpe->GetConvertFileNameProc(
                    eFileType, 
                    ".\\", 
                    vecArgc.at(0).c_str());
            }

            // �p�����[�^����2�}�C�i�X
            iParamCnt = iArgv - 2;
            sprintf_s(
                g_clsMainLogFile.m_cLogData, 
                sizeof(g_clsMainLogFile.m_cLogData), 
                "�p�����[�^���́F %d ", 
                iParamCnt);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
        }

        // �e�t�@�C������PC���{����ʏo��
#ifdef SCREEN
        // ���s�A�v���P�[�V������
        printf_s(
            "%s���F%s\n",
            g_maFileName.at(tagFileType::FILETYPE8).c_str(),
            g_stFileNameInfo.strModuleName.c_str());

        // �g���[�X���O�o�̓t�@�C����
        printf_s(
            "%s���F%s\n",
            g_maFileName.at(tagFileType::FILETYPE3).c_str(),
            g_stFileNameInfo.strTraceLogName.c_str());

        // �f�[�^���O�o�̓t�@�C����
        printf_s(
            "%s���F%s\n",
            g_maFileName.at(tagFileType::FILETYPE4).c_str(),
            g_stFileNameInfo.strDataLogName.c_str());
#endif // SCREEN

        // �e�t�@�C���������O�o��
        // ���s�A�v���P�[�V������
        sprintf_s(
            g_clsMainLogFile.m_cLogData,
            sizeof(g_clsMainLogFile.m_cLogData),
            "%s���F%s",
            g_maFileName.at(tagFileType::FILETYPE8).c_str(),
            g_stFileNameInfo.strModuleName.c_str());
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

        // �g���[�X���O�o�̓t�@�C����
        sprintf_s(
            g_clsMainLogFile.m_cLogData,
            sizeof(g_clsMainLogFile.m_cLogData),
            "%s���F%s",
            g_maFileName.at(tagFileType::FILETYPE3).c_str(),
            g_stFileNameInfo.strTraceLogName.c_str());
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

        // �f�[�^���O�o�̓t�@�C����
        sprintf_s(
            g_clsMainLogFile.m_cLogData,
            sizeof(g_clsMainLogFile.m_cLogData),
            "%s���F%s",
            g_maFileName.at(tagFileType::FILETYPE4).c_str(),
            g_stFileNameInfo.strDataLogName.c_str());
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

        // �������擾����
        // ���ގ��󋵈󎚐��䃁�C�������J�n�����o��
        g_clsMainLogFile.m_stTimeData = g_clsMainLogFile.GetDateTimeProc();
        sprintf_s(
            g_clsMainLogFile.m_cLogData, 
            sizeof(g_clsMainLogFile.m_cLogData), 
            "���ގ��󋵈󎚐��䃁�C�������J�n�����F%s %s", 
            (char *)g_clsMainLogFile.m_stTimeData.strMachDatePlus.c_str(), 
            (char *)g_clsMainLogFile.m_stTimeData.strMachTimePlus.c_str());
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, g_clsMainLogFile.m_cLogData);
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���ގ��󋵈󎚐��䃁�C�������J�n *****");

        // �h�A���x�����\���̂̃����o�̒l���o��
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, "***** �h�A���x�����̏������ *****");
        sprintf_s(
            g_clsMainLogFile.m_cLogData, 
            sizeof(g_clsMainLogFile.m_cLogData), 
            "�ʉ߂����h�A���x���F%d", 
            g_pclsMainRecOpe->m_stEntryExitInfo.stDoorLevelData.iDoorLevel);
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, g_clsMainLogFile.m_cLogData);
        sprintf_s(
            g_clsMainLogFile.m_cLogData, 
            sizeof(g_clsMainLogFile.m_cLogData), 
            "�ʉߑO�h�A���x���F%d", 
            g_pclsMainRecOpe->m_stEntryExitInfo.stDoorLevelData.iBeforeLevel);
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, g_clsMainLogFile.m_cLogData);
        sprintf_s(
            g_clsMainLogFile.m_cLogData, 
            sizeof(g_clsMainLogFile.m_cLogData), 
            "�ʉߌ�h�A���x���F%d", 
            g_pclsMainRecOpe->m_stEntryExitInfo.stDoorLevelData.iAfterLevel);
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, g_clsMainLogFile.m_cLogData);
        sprintf_s(
            g_clsMainLogFile.m_cLogData, 
            sizeof(g_clsMainLogFile.m_cLogData), 
            "�O���R�[�h�ʉߌ�h�A���x���F%d", 
            g_pclsMainRecOpe->m_stEntryExitInfo.stDoorLevelData.iLevel);
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, g_clsMainLogFile.m_cLogData);

        // ���ގ��󋵈󎚐���J�n����
        // �t�@�C���̃I�[�v���������s
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���ގ��󋵈󎚐���J�n�����֐��Ăяo�� *****");
        iStartResult = DataControlStartProc(iParamCnt, vecArgc, &pfEntryExitFile, &pfOutputFile, &iFileCopyResult);
        sprintf_s(g_clsMainLogFile.m_cLogData, sizeof(g_clsMainLogFile.m_cLogData), "�p�����[�^���́F%d", iParamCnt);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
        sprintf_s(g_clsMainLogFile.m_cLogData, sizeof(g_clsMainLogFile.m_cLogData), "�߂�l�́F%d", iStartResult);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

        // ���ގ��󋵈󎚐���J�n�����ɐ����������`�F�b�N
        if (EXIT_SUCCESS != iStartResult)
        {
            // ���ގ��󋵈󎚐���J�n�������s
            // ���ގ��󋵈󎚐���J�n�������s�̃��b�Z�[�W�����O�ɏo�͂��Č㏈���ֈړ�
            g_maStartEndErrorCode.insert(make_pair(iStartResult, g_clsMainLogFile.m_eErrorResultCode));
            LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** ���ގ��󋵈󎚐���J�n�����ُ�I�� *****");
            LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, INITFAILED);
            goto END;
        }
        else
        {
            // ���ގ��󋵈󎚐��䏉����������
            // ���ގ��󋵈󎚐��䏉�����������̃��b�Z�[�W�����O�ɏo��
            LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, "***** ���ގ��󋵈󎚐���J�n��������I�� *****");
            LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, INITSUCCESS);
        }

        // ���ގ��ǂݍ��݃f�[�^�t�@�C�����y�ѓ��ގ��������݃f�[�^�t�@�C������PC���{����ʏo��
#ifdef SCREEN
        // ���ގ��ǂݍ��݃f�[�^�t�@�C����
        printf_s(
            "%s���F%s\n",
            g_maFileName.at(tagFileType::FILETYPE1).c_str(),
            g_stFileNameInfo.strReadName.c_str());

        // ���ގ��������݃f�[�^�t�@�C����
        printf_s(
            "%s���F%s\n\n",
            g_maFileName.at(tagFileType::FILETYPE2).c_str(),
            g_stFileNameInfo.strWriteName.c_str());
#endif // SCREEN

        // �e�t�@�C���������O�o��
        // ���ގ��ǂݍ��݃f�[�^�t�@�C����
        sprintf_s(
            g_clsMainLogFile.m_cLogData,
            sizeof(g_clsMainLogFile.m_cLogData),
            "%s���F%s",
            g_maFileName.at(tagFileType::FILETYPE1).c_str(),
            g_stFileNameInfo.strReadName.c_str());
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

        // ���ގ��������݃f�[�^�t�@�C����
        sprintf_s(
            g_clsMainLogFile.m_cLogData,
            sizeof(g_clsMainLogFile.m_cLogData),
            "%s���F%s",
            g_maFileName.at(tagFileType::FILETYPE2).c_str(),
            g_stFileNameInfo.strWriteName.c_str());
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

        // �又��
        // ���ގ��f�[�^�ҏW���擾����
        // 1���ڂ̃��R�[�h���擾
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���ގ��f�[�^�ҏW���擾�֐��Ăяo�� *****");
        g_clsMainLogFile.m_stLogData = g_pclsMainSubOpe->CreateEventMessageProc(ACTIONSET, "���ގ��f�[�^�ҏW����");
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
            "�t�@�C���I���t���O�F%d ���ގ����R�[�h�擾���ʁF%d", 
            iFileEndFlag, 
            iGetDataResult);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
        sprintf_s(
            g_clsMainLogFile.m_cLogData, 
            sizeof(g_clsMainLogFile.m_cLogData), 
            "�X�L�b�v����t���O�F%s", 
            bSkipFlag ? "true" : "false");
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

        // ���ގ����R�[�h�̎擾���ʂ��`�F�b�N
        if (CM_DATAFRAUD == iGetDataResult)
        {
            // �P���ڂ̃f�[�^���s��
            // �G���[���b�Z�[�W��������
            // �G���[���b�Z�[�W�����O�ɏo��
            LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �G���[���b�Z�[�W�����֐��Ăяo�� *****");
            g_clsMainLogFile.m_stLogData = g_pclsMainSubOpe->CreateEventMessageProc(
                ERRORSET, 
                g_clsMainLogFile.m_stLogData.pcActionName, 
                g_clsMainLogFile.m_eErrorResultCode);
            LOGOUTPUTERROR(
                TRACEOUTPUTLOG,
                ERRORINFORMATION1, 
                g_clsMainLogFile.m_stLogData.pcMessage, 
                g_clsMainLogFile.m_stLogData.eErrorCode);
            sprintf_s(g_clsMainLogFile.m_cLogData, sizeof(g_clsMainLogFile.m_cLogData), "�߂�l�́F%d", iGetDataResult);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

            // �����p�����b�Z�[�W�����O�ɏo��
            // �f�[�^�s���Ƃ����G���[�ł͍s�������@�\�ɑ΂��钼�ڂ̉e���͂Ȃ����ߏ������p������
            LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** ���ގ��f�[�^�ҏW���擾�֐��Ăяo���f�[�^�s�� *****");
            LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, ERRORPROCCONTINUED);
        }
        else
        {
            // �o�͑Ώۂ̃��R�[�h������
            // �擾����1���R�[�h���̓��ގ��f�[�^�ҏW�����Ǘ��p�̓��ގ����f�[�^�ҏW�p�\���̂֊i�[
            // �ideqEntryExitEditData�ϐ���C++����R���e�ideque��`�j
            g_pclsMainRecOpe->m_stEntryExitInfo.deqEntryExitEditData.push_back(*pstEntryExitEditData);

            // ���R�[�h�擾�����̃��b�Z�[�W�����O�ɏo��
            LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, "***** ���ގ��f�[�^�ҏW���擾�֐��Ăяo������I�� *****");
            LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, GETRECORDSUCCESS);
            LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���ގ����R�[�h���擾�o�����̂ŏ������p�� *****");
        }

        // ���ގ��������݃f�[�^�t�@�C���̃t�@�C����Ԃ��擾
        sprintf_s(
            g_clsMainLogFile.m_cLogData, 
            sizeof(g_clsMainLogFile.m_cLogData), 
            "***** %s��Ԏ擾�J�n *****", 
            g_maFileName.at(tagFileType::FILETYPE2).c_str());
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, g_clsMainLogFile.m_cLogData);
        iStatResult = g_pclsMainRecOpe->CheckFileStatProc(
            g_stFileNameInfo.strWriteName.c_str(), 
            &g_pclsMainRecOpe->m_stStat);
        sprintf_s(
            g_clsMainLogFile.m_cLogData, 
            sizeof(g_clsMainLogFile.m_cLogData), 
            "%s���F%s", 
            g_maFileName.at(tagFileType::FILETYPE2).c_str(), 
            g_stFileNameInfo.strWriteName.c_str());
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
        sprintf_s(g_clsMainLogFile.m_cLogData, sizeof(g_clsMainLogFile.m_cLogData), "��Ԍ��ʂ́F%d", iStatResult);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
        g_clsMainLogFile.m_stLogData = g_pclsMainSubOpe->CreateEventMessageProc(
            ACTIONSET, 
            g_stFileNameInfo.strWriteName.c_str());

        // ��Ԍ��ʂ��`�F�b�N
        if (EXIT_SUCCESS != iStatResult)
        {
            // �t�@�C���̏�Ԏ擾�Ɏ��s
            // ���ގ����f�[�^�ҏW�p�\���̃������̈�����
            LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���ގ����f�[�^�ҏW�p�\���̃������̈��� *****");
#ifdef _DEBUG
            SAFE_FREE_DEBUG(pstEntryExitEditData);
#else
            SAFE_FREE(pstEntryExitEditData);
#endif // _DEBUG

            // �������̈����̃��b�Z�[�W�����O�ɏo��
            LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���ގ����f�[�^�ҏW�p�\���̃������̈����I�� *****");
            sprintf_s(
                g_clsMainLogFile.m_cLogData, 
                sizeof(g_clsMainLogFile.m_cLogData), 
                "%s%s", 
                g_clsMainLogFile.m_stLogData.pcActionName, 
                MEMORYRELEASESUCCESS);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

            // �G���[�R�[�h��ݒ肵�Č㏈���ֈړ�
            LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** �t�@�C����ԃ`�F�b�N�����ُ�I�� *****");
            iRtn = iStatResult;
            g_clsMainLogFile.m_maErrorActionName.insert(make_pair(iRtn, g_clsMainLogFile.m_stLogData.pcActionName));
            goto END;
        }
        else
        {
            // �t�@�C���̏�Ԏ擾����
            // �t�@�C���������O�ɏo��
            sprintf_s(
                g_clsMainLogFile.m_cLogData, 
                sizeof(g_clsMainLogFile.m_cLogData), 
                "�t�@�C�����́F%s", 
                g_clsMainLogFile.m_stLogData.pcActionName);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

            // �t�@�C���̏�Ԏ擾�����̃��b�Z�[�W�y�ѓ��ގ��������݃f�[�^�t�@�C���T�C�Y�����O�ɏo��
            sprintf_s(
                g_clsMainLogFile.m_cLogData, 
                sizeof(g_clsMainLogFile.m_cLogData), 
                "***** %s��Ԏ擾����I�� *****", 
                g_maFileName.at(tagFileType::FILETYPE2).c_str());
            LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, g_clsMainLogFile.m_cLogData);
            LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, (char*)GETSTATSUCCESS);
            sprintf_s(
                g_clsMainLogFile.m_cLogData, 
                sizeof(g_clsMainLogFile.m_cLogData), 
                "%s�̌��݂̃t�@�C���T�C�Y�́F%d", 
                g_maFileName.at(tagFileType::FILETYPE2).c_str(), 
                g_pclsMainRecOpe->m_stStat.st_size);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
        }

        // ���ގ��󋵈󎚌��o���ݒ菈��
        // PC�{����ʋy�ѓ��ގ��������݃f�[�^�t�@�C���֌��o�����o��
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���ގ��󋵈󎚌��o���ݒ�֐��Ăяo�� *****");
        g_pclsMainRecOpe->SetEntryExitDataTitleProc(pfOutputFile);

        // �ǂݍ��݃��R�[�h���Ȃ��Ȃ�܂Ń��[�v
        do
        {
            // ���R�[�h�o�͑Ώۂ��`�F�b�N
            if (true != bSkipFlag)
            {
                // ���R�[�h�o�͑Ώ�
                // ���ގ��f�[�^�ҏW���o�͏���
                // �擾�������R�[�h�𒠕[�`���ɕҏW���ďo��
                // �ideqEntryExitEditData�ϐ���C++����R���e�ideque��`�j
                LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���ގ��f�[�^�ҏW���o�͊֐��Ăяo�� *****");
                g_pclsMainRecOpe->PutEditDataProc(
                    pfOutputFile, 
                    &g_pclsMainRecOpe->m_stEntryExitInfo.deqEntryExitEditData[siRecCnt],
                    &g_pclsMainRecOpe->m_stEntryExitInfo.stDoorLevelData);

                // �h�A���x�����\���̂̍X�V��Ԃ��o��
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, "***** �h�A���x�����̍X�V��� *****");
                sprintf_s(
                    g_clsMainLogFile.m_cLogData, 
                    sizeof(g_clsMainLogFile.m_cLogData), 
                    "�ʉ߃h�A���x���F%d", 
                    g_pclsMainRecOpe->m_stEntryExitInfo.stDoorLevelData.iDoorLevel);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, g_clsMainLogFile.m_cLogData);
                sprintf_s(
                    g_clsMainLogFile.m_cLogData, 
                    sizeof(g_clsMainLogFile.m_cLogData), 
                    "�ʉߑO�h�A���x���F%d", 
                    g_pclsMainRecOpe->m_stEntryExitInfo.stDoorLevelData.iBeforeLevel);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, g_clsMainLogFile.m_cLogData);
                sprintf_s(
                    g_clsMainLogFile.m_cLogData, 
                    sizeof(g_clsMainLogFile.m_cLogData), 
                    "�ʉߌ�h�A���x���F%d", 
                    g_pclsMainRecOpe->m_stEntryExitInfo.stDoorLevelData.iAfterLevel);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, g_clsMainLogFile.m_cLogData);
                sprintf_s(
                    g_clsMainLogFile.m_cLogData, 
                    sizeof(g_clsMainLogFile.m_cLogData), 
                    "���݂̒ʉ߃h�A���x���F%d", 
                    g_pclsMainRecOpe->m_stEntryExitInfo.stDoorLevelData.iLevel);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, g_clsMainLogFile.m_cLogData);

                // ���ގ����f�[�^�ҏW�p�\���̃������̈�����
                LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���ގ����f�[�^�ҏW�p�\���̃������̈��� *****");
#ifdef _DEBUG
                SAFE_FREE_DEBUG(pstEntryExitEditData);
#else
                SAFE_FREE(pstEntryExitEditData);
#endif // _DEBUG

                // �������̈����̃��b�Z�[�W�����O�ɏo��
                LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���ގ����f�[�^�ҏW�p�\���̃������̈����I�� *****");
                sprintf_s(
                    g_clsMainLogFile.m_cLogData,
                    sizeof(g_clsMainLogFile.m_cLogData),
                    "%s%s",
                    g_clsMainLogFile.m_stLogData.pcActionName,
                    MEMORYRELEASESUCCESS);
                LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
            }

            // ���ގ��f�[�^�ҏW���擾����
            // 2���ڈȍ~�̃��R�[�h���擾
            LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���ގ��f�[�^�ҏW���擾�֐��Ăяo�� *****");
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
                "�t�@�C���I���t���O�F%d ���ގ����R�[�h�擾���ʁF%d", 
                iFileEndFlag, iGetDataResult);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
            sprintf_s(
                g_clsMainLogFile.m_cLogData, 
                sizeof(g_clsMainLogFile.m_cLogData), 
                "�X�L�b�v����t���O�F%s", 
                bSkipFlag ? "true" : "false");
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
            g_clsMainLogFile.m_stLogData = g_pclsMainSubOpe->CreateEventMessageProc(ACTIONSET, "���ގ��f�[�^�ҏW����");

            // ���R�[�h�o�͑Ώۂ��`�F�b�N
            if (true != bSkipFlag)
            {
                // ���R�[�h�o�͑Ώ�
                // �擾����1���R�[�h���̓��ގ��f�[�^�ҏW�����Ǘ��p�̓��ގ����f�[�^�ҏW�p�\���̂֊i�[
                // �ideqEntryExitEditData�ϐ���C++����R���e�ideque��`�j
                g_pclsMainRecOpe->m_stEntryExitInfo.deqEntryExitEditData.push_back(*pstEntryExitEditData);

                // ���J�E���^�[���擾
                // �ideqEntryExitEditData�ϐ��̐�-1�j
                siRecCnt = g_pclsMainRecOpe->m_stEntryExitInfo.deqEntryExitEditData.size() - 1;

                // �����G���A���x���N���A����
                // ���ގ������G���A���x�����N���A
                // �ideqEntryExitEditData�ϐ���C++����R���e�ideque��`�j
                LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �����G���A���x���N���A�֐��Ăяo�� *****");
                g_pclsMainRecOpe->ClearLevelProc(
                    pfOutputFile, 
                    g_pclsMainRecOpe->m_stEntryExitInfo.deqEntryExitEditData[siRecCnt],
                    iFileEndFlag, 
                    &g_pclsMainRecOpe->m_stEntryExitInfo.stDoorLevelData);

                // �N���A��̃h�A���x�����\���̂̍X�V��Ԃ��o��
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, "***** �h�A���x�����̃N���A��� *****");
                sprintf_s(
                    g_clsMainLogFile.m_cLogData, 
                    sizeof(g_clsMainLogFile.m_cLogData), 
                    "�ʉ߃h�A���x���F%d", 
                    g_pclsMainRecOpe->m_stEntryExitInfo.stDoorLevelData.iDoorLevel);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, g_clsMainLogFile.m_cLogData);
                sprintf_s(
                    g_clsMainLogFile.m_cLogData, 
                    sizeof(g_clsMainLogFile.m_cLogData), 
                    "�ʉߑO�h�A���x���F%d", 
                    g_pclsMainRecOpe->m_stEntryExitInfo.stDoorLevelData.iBeforeLevel);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, g_clsMainLogFile.m_cLogData);
                sprintf_s(
                    g_clsMainLogFile.m_cLogData, 
                    sizeof(g_clsMainLogFile.m_cLogData), 
                    "�ʉߌ�h�A���x���F%d", 
                    g_pclsMainRecOpe->m_stEntryExitInfo.stDoorLevelData.iAfterLevel);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, g_clsMainLogFile.m_cLogData);
                sprintf_s(
                    g_clsMainLogFile.m_cLogData, 
                    sizeof(g_clsMainLogFile.m_cLogData), 
                    "���݂̒ʉ߃h�A���x���F%d", 
                    g_pclsMainRecOpe->m_stEntryExitInfo.stDoorLevelData.iLevel);
                LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, g_clsMainLogFile.m_cLogData);
            }

            // �t�@�C���I���t���O��Ԃ����O�ɏo��
            sprintf_s(g_clsMainLogFile.m_cLogData, sizeof(g_clsMainLogFile.m_cLogData), "�t�@�C���I���t���O��Ԃ́F%d", iFileEndFlag);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
        }
        // �t�@�C���I���t���OON�ł���ΏI��
        while (FILEEND_OFF == iFileEndFlag);

        // ���ގ����f�[�^�ҏW�p�\���̃������̈�����
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���ގ����f�[�^�ҏW�p�\���̃������̈��� *****");
#ifdef _DEBUG
        SAFE_FREE_DEBUG(pstEntryExitEditData);
#else
        SAFE_FREE(pstEntryExitEditData);
#endif // _DEBUG

        // �������̈����̃��b�Z�[�W�����O�ɏo��
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���ގ����f�[�^�ҏW�p�\���̃������̈����I�� *****");
        sprintf_s(
            g_clsMainLogFile.m_cLogData,
            sizeof(g_clsMainLogFile.m_cLogData),
            "%s%s",
            g_clsMainLogFile.m_stLogData.pcActionName,
            MEMORYRELEASESUCCESS);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

        // �ŏI���ގ��f�[�^�o�͌����o��
        // �idistance��C++�����`�j
        siDataPrintCnt = distance(
            g_pclsMainRecOpe->m_stEntryExitInfo.deqEntryExitEditData.begin(),
            g_pclsMainRecOpe->m_stEntryExitInfo.deqEntryExitEditData.end());
#ifdef SCREEN
        printf("\n���ގ����R�[�h�����́F%ld ��\n", lRecCnt);
        printf("���ގ��f�[�^�󎚌����́F%zu ��\n", siDataPrintCnt);
#endif // SCREEN
        LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, ALLREADEND);
        sprintf_s(g_clsMainLogFile.m_cLogData, sizeof(g_clsMainLogFile.m_cLogData), "���ގ����R�[�h�����F%ld", lRecCnt);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
        sprintf_s(
            g_clsMainLogFile.m_cLogData, 
            sizeof(g_clsMainLogFile.m_cLogData), 
            "���ގ��f�[�^�󎚌����F%zu", 
            siDataPrintCnt);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

        // ���������`�F�b�N
        if (lRecCnt == siDataPrintCnt)
        {
            // ���ގ����R�[�h�����Ɠ��ގ��f�[�^�󎚌�������v
            // ��v�̃��b�Z�[�W����ʋy�у��O�ɏo��
#ifdef SCREEN
            printf("***** ����I���I�I�iOK�j *****\n");
#endif // SCREEN
            sprintf_s(
                g_clsMainLogFile.m_cLogData,
                sizeof(g_clsMainLogFile.m_cLogData),
                "***** ����I���I�I�iOK�j *****");
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

        }
        else
        {
            // ���ގ����R�[�h�����Ɠ��ގ��f�[�^�󎚌���������
            // �s��v�̃��b�Z�[�W����ʋy�у��O�ɏo��
#ifdef SCREEN
            printf("***** �����s��v�I�I�iNG�j *****\n");
            printf("�s���f�[�^�����́F%zu ��\n", lRecCnt - siDataPrintCnt);
#endif // SCREEN
            sprintf_s(
                g_clsMainLogFile.m_cLogData,
                sizeof(g_clsMainLogFile.m_cLogData),
                "***** �����s��v�I�I�iNG�j *****");
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
            sprintf_s(
                g_clsMainLogFile.m_cLogData,
                sizeof(g_clsMainLogFile.m_cLogData),
                "�s���f�[�^�����́F%zu ��",
                lRecCnt - siDataPrintCnt);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
        }
    }

    // ��O����
    catch (const ERRORCODETABLE eRecDataErr)
    {
        // �G���[�R�[�h��ݒ�
        g_clsMainLogFile.m_eErrorResultCode = eRecDataErr;
        g_clsMainLogFile.m_maErrorActionName.insert(
            make_pair(
                (int)eRecDataErr, 
                g_clsMainLogFile.m_stLogData.pcActionName));
        iRtn = (int)eRecDataErr;

        // PC�{����ʋy�у��O�ɃG���[���b�Z�[�W��\��
        g_clsMainLogFile.m_stLogData = g_pclsMainSubOpe->CreateEventMessageProc(
            ERRORSET,
            g_clsMainLogFile.m_maErrorActionName.at((int)eRecDataErr).c_str(),
            g_clsMainLogFile.m_eErrorResultCode);
        LOGOUTPUTERROR(
            TRACEOUTPUTLOG,
            ERRORINFORMATION1,
            g_clsMainLogFile.m_stLogData.pcMessage,
            g_clsMainLogFile.m_stLogData.eErrorCode);
        sprintf_s(g_clsMainLogFile.m_cLogData, sizeof(g_clsMainLogFile.m_cLogData), "�߂�l�́F%d", eRecDataErr);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���ގ��f�[�^�ҏW���擾�֐��Ăяo���ُ�I�� *****");
    }

END:
    // �㏈��
    // ���ގ��󋵈󎚐���I������
    // �t�@�C���̃N���[�Y�������s
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���ގ��󋵈󎚐���I�������֐��Ăяo�� *****");
    iEndResult = DataControlEndProc(&pfEntryExitFile, &pfOutputFile, vecArgc, iFileCopyResult);
    sprintf_s(g_clsMainLogFile.m_cLogData, sizeof(g_clsMainLogFile.m_cLogData), "�߂�l�́F%d", iEndResult);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

    // ���ގ��󋵈󎚐���I�������̎��s���ʂ��`�F�b�N
    if (EXIT_SUCCESS != iEndResult)
    {
        // ���ގ��󋵈󎚐���I���������s
        // �Ăяo���ɑ΂���ُ�I���̃��b�Z�[�W�����O�ɏo��
        g_maStartEndErrorCode.insert(make_pair(iEndResult, g_clsMainLogFile.m_eErrorResultCode));
        LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** ���ގ��󋵈󎚐���I�������ُ�I�� *****");
        LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, ENDFAILED);
    }
    else
    {
        // ���ގ��󋵈󎚐���I����������
        // �Ăяo���ɑ΂��鐳��I���̃��b�Z�[�W�����O�ɏo��
        LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, "***** ���ގ��󋵈󎚐���I����������I�� *****");
        LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, ENDSUCCESS);
    }

    // ���ގ��󋵈󎚐��䃁�C�������ł̖߂�l���`�F�b�N
    if (EXIT_SUCCESS != iRtn)
    {
        // �ُ�l
        // ���ގ��󋵈󎚐��䃁�C�������I�������o��
        LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** ���ގ��󋵈󎚐��䃁�C�������ُ�I�� *****");
        g_clsMainLogFile.m_stTimeData = g_clsMainLogFile.GetDateTimeProc();
        sprintf_s(
            g_clsMainLogFile.m_cLogData, 
            sizeof(g_clsMainLogFile.m_cLogData), 
            "���ގ��󋵈󎚐��䃁�C�������I�������F%s %s", 
            (char *)g_clsMainLogFile.m_stTimeData.strMachDatePlus.c_str(), 
            (char *)g_clsMainLogFile.m_stTimeData.strMachTimePlus.c_str());
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

        // PC�{����ʋy�у��O�ɃG���[���b�Z�[�W��\��
        g_clsMainLogFile.m_stLogData = g_pclsMainSubOpe->CreateEventMessageProc(
            ERRORSET, 
            g_clsMainLogFile.m_maErrorActionName.at(iRtn).c_str(),
            g_clsMainLogFile.m_eErrorResultCode);
        LOGOUTPUTERROR(
            TRACEOUTPUTLOG, 
            ERRORINFORMATION1, 
            g_clsMainLogFile.m_stLogData.pcMessage, 
            g_clsMainLogFile.m_stLogData.eErrorCode);
        sprintf_s(g_clsMainLogFile.m_cLogData, sizeof(g_clsMainLogFile.m_cLogData), "�߂�l�́F%d", iRtn);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
#ifdef SCREEN
        printf_s("%s\n", g_clsMainLogFile.m_stLogData.pcMessage);
        printf_s("�G���[�R�[�h�́F%d\n", g_clsMainLogFile.m_stLogData.eErrorCode);
        printf_s("�߂�l�́F%d\n", iRtn);
        printf_s("***** ���ގ��󋵈󎚏����ُ�I�� *****\n");
#endif // SCREEN
        LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** ���ގ��󋵈󎚏����ُ�I�� *****");
    }
    else
    {
        // ����l
        // ���ގ��󋵈󎚐���J�n�����^���ގ��󋵈󎚐���I�������̌��ʂ��`�F�b�N
        if (EXIT_SUCCESS == iStartResult && EXIT_SUCCESS == iEndResult)
        {
            // ���ގ��󋵈󎚐���J�n�����y�ѓ��ގ��󋵈󎚐���I���������ɐ���
            // ���ގ��󋵈󎚐��䃁�C�������I�������o��
            LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, "***** ���ގ��󋵈󎚐��䃁�C����������I�� *****");
            g_clsMainLogFile.m_stTimeData = g_clsMainLogFile.GetDateTimeProc();
            sprintf_s(
                g_clsMainLogFile.m_cLogData, 
                sizeof(g_clsMainLogFile.m_cLogData), 
                "���ގ��󋵈󎚐��䃁�C�������I�������F%s %s", 
                (char *)g_clsMainLogFile.m_stTimeData.strMachDatePlus.c_str(), 
                (char *)g_clsMainLogFile.m_stTimeData.strMachTimePlus.c_str());
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

            // ���팋�ʂ��o��
#ifdef SCREEN
            printf_s("���ގ��󋵈󎚏������s���ʂ́F%s\n\n", g_stFileNameInfo.strWriteName.c_str());
            printf_s("***** ���ގ��󋵈󎚏�������I�� *****\n");
#endif // SCREEN
            sprintf_s(
                g_clsMainLogFile.m_cLogData,
                sizeof(g_clsMainLogFile.m_cLogData),
                "�߂�l�́F%d",
                iRtn);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
            sprintf_s(
                g_clsMainLogFile.m_cLogData,
                sizeof(g_clsMainLogFile.m_cLogData),
                "���ގ��󋵈󎚏������s���ʂ́F%s",
                g_stFileNameInfo.strWriteName.c_str());
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
            LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, "***** ���ގ��󋵈󎚏�������I�� *****");
        }
        else
        {
            // ���ގ��󋵈󎚊J�n�����������͓��ގ��󋵈󎚏I�������ǂ��炩���s
            // ���ގ��󋵈󎚊J�n�����y�ѓ��ގ��󋵈󎚐���I���������ꂼ��̖߂�l�ŃG���[���b�Z�[�W���o�͂���
            // ���ގ��󋵈󎚊J�n�����߂�l���`�F�b�N
            if (EXIT_SUCCESS != iStartResult)
            {
                // ���ގ��󋵈󎚐���J�n�����ňُ�I��
                // �߂�l�ɓ��ގ��󋵈󎚐���J�n�����̌��ʃR�[�h��ݒ�
                iRtn = iStartResult;

                // �G���[���b�Z�[�W��PC�{����ʋy�у��O�ɏo��
                LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �G���[���b�Z�[�W�����֐��Ăяo�� *****");
                eInitResultCode = tagErrorCode::FAILERROR;
                g_clsMainLogFile.m_stLogData = g_pclsMainSubOpe->CreateEventMessageProc(
                    ACTIONSET, 
                    "���ގ��󋵈󎚐���J�n������");
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

                // ���ގ��󋵈󎚐���J�n�����Ŕ��������ُ팋�ʂ�PC���{����ʋy�у��O�\��
                g_clsMainLogFile.m_stLogData = g_pclsMainSubOpe->CreateEventMessageProc(
                    ERRORSET, 
                    g_clsMainLogFile.m_maErrorActionName.at(iStartResult).c_str(), 
                    g_maStartEndErrorCode.at(iStartResult));
#ifdef SCREEN
                printf_s("%s\n", g_clsMainLogFile.m_stLogData.pcMessage);
                printf_s("�G���[�R�[�h�́F%d\n", g_clsMainLogFile.m_stLogData.eErrorCode);
                printf_s("�߂�l�́F%d\n", iRtn);
#endif // SCREEN
                LOGOUTPUTERROR(
                    TRACEOUTPUTLOG, 
                    ERRORINFORMATION1, 
                    g_clsMainLogFile.m_stLogData.pcMessage, 
                    g_clsMainLogFile.m_stLogData.eErrorCode);
                sprintf_s(
                    g_clsMainLogFile.m_cLogData, 
                    sizeof(g_clsMainLogFile.m_cLogData), 
                    "�G���[�R�[�h�́F%d", 
                    g_clsMainLogFile.m_stLogData.eErrorCode);
                LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
                sprintf_s(g_clsMainLogFile.m_cLogData, sizeof(g_clsMainLogFile.m_cLogData), "�߂�l�́F%d", iRtn);
                LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
            }

            // ���ގ��󋵈󎚏I�������߂�l���`�F�b�N
            if (EXIT_SUCCESS != iEndResult)
            {
                // ���ގ��󋵈󎚐���I�������ňُ�I��
                // �߂�l�ɓ��ގ��󋵈󎚐���I�������̌��ʃR�[�h��ݒ�
                iRtn = iEndResult;

                // �G���[���b�Z�[�W��PC�{����ʋy�у��O�ɏo��
                LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �G���[���b�Z�[�W�����֐��Ăяo�� *****");
                eEndResultCode = tagErrorCode::FAILERROR;
                g_clsMainLogFile.m_stLogData = g_pclsMainSubOpe->CreateEventMessageProc(
                    ACTIONSET, 
                    "���ގ��󋵈󎚐���I��������");
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

                // ���ގ��󋵈󎚐���I�������Ŕ��������ُ팋�ʂ�PC���{����ʋy�у��O�\��
                g_clsMainLogFile.m_stLogData = g_pclsMainSubOpe->CreateEventMessageProc(
                    ERRORSET, 
                    g_clsMainLogFile.m_maErrorActionName.at(iEndResult).c_str(), 
                    g_maStartEndErrorCode.at(iEndResult));
#ifdef SCREEN
                printf_s("%s\n", g_clsMainLogFile.m_stLogData.pcMessage);
                printf_s("�G���[�R�[�h�́F%d\n", g_clsMainLogFile.m_stLogData.eErrorCode);
                printf_s("�߂�l�́F%d\n", iRtn);
#endif // SCREEN
                LOGOUTPUTERROR(
                    TRACEOUTPUTLOG, 
                    ERRORINFORMATION1, 
                    g_clsMainLogFile.m_stLogData.pcMessage, 
                    g_clsMainLogFile.m_stLogData.eErrorCode);
                sprintf_s(
                    g_clsMainLogFile.m_cLogData, 
                    sizeof(g_clsMainLogFile.m_cLogData), 
                    "�G���[�R�[�h�́F%d", 
                    g_clsMainLogFile.m_stLogData.eErrorCode);
                LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
                sprintf_s(g_clsMainLogFile.m_cLogData, sizeof(g_clsMainLogFile.m_cLogData), "�߂�l�́F%d", iRtn);
                LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
            }

            // ���ގ��󋵈󎚐��䃁�C�������I�������o��
            LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** ���ގ��󋵈󎚐��䃁�C�������ُ�I�� *****");
            g_clsMainLogFile.m_stTimeData = g_clsMainLogFile.GetDateTimeProc();
            sprintf_s(
                g_clsMainLogFile.m_cLogData, 
                sizeof(g_clsMainLogFile.m_cLogData), 
                "���ގ��󋵈󎚐��䃁�C�������I�������F%s %s", 
                (char *)g_clsMainLogFile.m_stTimeData.strMachDatePlus.c_str(), 
                (char *)g_clsMainLogFile.m_stTimeData.strMachTimePlus.c_str());
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
            LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** ���ގ��󋵈󎚏����ُ�I�� *****");
#ifdef SCREEN
            printf_s("***** ���ގ��󋵈󎚏����ُ�I�� *****\n");
#endif // SCREEN
        }
    }

    // �ŏI�`�Ԋe�t�@�C�����o��
    // ��΃p�X�ϊ��t�@�C�����擾�����Ŏ擾�����t�@�C�����i�ŏI�`�ԁj�����O�o��
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, "***** ��΃p�X��t�^�����e�t�@�C�����i�ŏI�`�ԁj *****");
    sprintf_s(
        g_clsMainLogFile.m_cLogData, 
        sizeof(g_clsMainLogFile.m_cLogData), 
        "%s���F%s", 
        g_maFileName.at(tagFileType::FILETYPE1).c_str(), 
        g_stFileNameInfo.strReadName.c_str());
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, g_clsMainLogFile.m_cLogData);
    sprintf_s(
        g_clsMainLogFile.m_cLogData, 
        sizeof(g_clsMainLogFile.m_cLogData), 
        "%s���F%s", 
        g_maFileName.at(tagFileType::FILETYPE2).c_str(), 
        g_stFileNameInfo.strWriteName.c_str());
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, g_clsMainLogFile.m_cLogData);
    sprintf_s(
        g_clsMainLogFile.m_cLogData, 
        sizeof(g_clsMainLogFile.m_cLogData), 
        "%s���F%s", 
        g_maFileName.at(tagFileType::FILETYPE3).c_str(), 
        g_stFileNameInfo.strTraceLogName.c_str());
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, g_clsMainLogFile.m_cLogData);
    sprintf_s(
        g_clsMainLogFile.m_cLogData, 
        sizeof(g_clsMainLogFile.m_cLogData), 
        "%s���F%s", 
        g_maFileName.at(tagFileType::FILETYPE4).c_str(), 
        g_stFileNameInfo.strDataLogName.c_str());
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, g_clsMainLogFile.m_cLogData);
    sprintf_s(
        g_clsMainLogFile.m_cLogData, 
        sizeof(g_clsMainLogFile.m_cLogData), 
        "%s���F%s", 
        g_maFileName.at(tagFileType::FILETYPE5).c_str(), 
        g_stFileNameInfo.strBackupName.c_str());
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, g_clsMainLogFile.m_cLogData);
    sprintf_s(
        g_clsMainLogFile.m_cLogData, 
        sizeof(g_clsMainLogFile.m_cLogData), 
        "%s���F%s", 
        g_maFileName.at(tagFileType::FILETYPE6).c_str(), 
        g_stFileNameInfo.strChangeName.c_str());
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, g_clsMainLogFile.m_cLogData);
    sprintf_s(
        g_clsMainLogFile.m_cLogData, 
        sizeof(g_clsMainLogFile.m_cLogData), 
        "%s���F%s", 
        g_maFileName.at(tagFileType::FILETYPE7).c_str(), 
        g_stFileNameInfo.strDeleteName.c_str());
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, g_clsMainLogFile.m_cLogData);
    sprintf_s(
        g_clsMainLogFile.m_cLogData, 
        sizeof(g_clsMainLogFile.m_cLogData), 
        "%s���F%s", 
        g_maFileName.at(tagFileType::FILETYPE8).c_str(), 
        g_stFileNameInfo.strModuleName.c_str());
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, g_clsMainLogFile.m_cLogData);
    sprintf_s(
        g_clsMainLogFile.m_cLogData, 
        sizeof(g_clsMainLogFile.m_cLogData), 
        "%s���F%s", 
        g_maFileName.at(tagFileType::FILETYPE9).c_str(),
        g_stFileNameInfo.strFraudDataName.c_str());
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, g_clsMainLogFile.m_cLogData);

    // �������擾����
    // ���ގ��󋵈󎚏����I�������\��
    g_clsMainLogFile.m_stTimeData = g_clsMainLogFile.GetDateTimeProc();
    sprintf_s(
        g_clsMainLogFile.m_cLogData, 
        sizeof(g_clsMainLogFile.m_cLogData), 
        "���ގ��󋵈󎚏����I�������F%s %s", 
        (char *)g_clsMainLogFile.m_stTimeData.strMachDatePlus.c_str(), 
        (char *)g_clsMainLogFile.m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, g_clsMainLogFile.m_cLogData);
#ifdef SCREEN
    printf_s("%s\n", g_clsMainLogFile.m_cLogData);
#endif // SCREEN

    // ���s���ʂ�ԋp
    return iRtn;
}

//********************************************
// �֐����F���ގ��󋵈󎚐���J�n����
// Function Name�FDataControlStartProc
// 
// �����T�v�F
// ���ގ��󋵂��󎚂��邽�߂̏������s��
// 
// �����菇�F
//   �P�D�����l�ݒ�
//   �Q�D���ގ��ǂݍ��݃f�[�^�t�@�C�����^���ގ��������݃f�[�^�t�@�C�������ϐ��擾
//   �R�D�p�����[�^�`�F�b�N�i���ϐ��̐ݒ肪�Ȃ��ꍇ�̂݁j
//   �S�D���ގ��ǂݍ��݃f�[�^�t�@�C����΃p�X�t���ϊ�
//   �T�D�o�b�N�A�b�v�t�@�C����΃p�X�t���ϊ�
//   �U�D�t�@�C���R�s�[�����ɂ��o�b�N�A�b�v�t�@�C������
//   �V�D�t�@�C�����ϊ������ɂ��o�b�N�A�b�v�t�@�C�����ϊ�
//   �W�D���ގ��ǂݍ��݃f�[�^�t�@�C���I�[�v��
//   �X�D���ގ��������݃f�[�^�t�@�C����΃p�X�t���ϊ�
//   �P�O�D���ގ��������݃f�[�^�t�@�C���I�[�v��
//   �P�P�D�G���[���b�Z�[�W���O�o�́i�G���[�����������ꍇ�̂݁j
// 
// �����F    �R�}���h��
//           �R�}���h���
//           ���ގ��ǂݍ��݃f�[�^�t�@�C���|�C���^
//           ���ގ��������݃f�[�^�t�@�C���|�C���^
//           �t�@�C���R�s�[�Ăяo�����s����
// �߂�l�F  ���s���ʁi����I����stdlib.h�Œ�`���Ă�����e�𗘗p�j
//           ����I���FEXIT_SUCCESS�i0�j
//           �ُ�I���F���ϐ��擾�G���[�FCM_ENVERROR�i101�j
//                     �p�����[�^�w��Ȃ��FCM_PARAMERROR�i102�j
//                     �p�����[�^�s���FCM_PARAMERROR�i103�j
//                     �t�@�C���I�[�v���G���[�FCM_FILEOPENERROR�i104�j
//                     �t�@�C���R�s�[�G���[�FCM_FILECOPYERROR�i110�j
//                     �t�@�C�����ϊ��G���[�FCM_RENAMEERROR�i111�j
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
int DataControlStartProc(
    int iCmdCnt,
    vector<string> vecCmdData,
    FILE **pfReadFile,
    FILE **pfWriteFile,
    int* piFileCopyResult)
{
    // �ϐ��錾
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

    // ������
    memset(cReadLocalName, 0x00, sizeof(cReadLocalName));
    memset(cWriteLocalName, 0x00, sizeof(cWriteLocalName));
    memset(cActinData, 0x00, sizeof(cActinData));

    // �O����
    // �������擾����
    // ���ގ��󋵈󎚐���J�n�����J�n�����擾
    g_clsMainLogFile.m_stTimeData = g_clsMainLogFile.GetDateTimeProc();
    sprintf_s(
        g_clsMainLogFile.m_cLogData, 
        sizeof(g_clsMainLogFile.m_cLogData), 
        "���ގ��󋵈󎚐���J�n�����J�n�����F%s %s", 
        (char *)g_clsMainLogFile.m_stTimeData.strMachDatePlus.c_str(), 
        (char *)g_clsMainLogFile.m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, g_clsMainLogFile.m_cLogData);
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���ގ��󋵈󎚐���J�n�����J�n *****");

    // �����l��ݒ�
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, "***** �����l *****");
    g_clsMainLogFile.m_stLogData.eErrorCode = tagErrorCode::SUCCESS;
    sprintf_s(
        g_clsMainLogFile.m_cLogData, 
        sizeof(g_clsMainLogFile.m_cLogData), 
        "�G���[�R�[�h�F%d", 
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
        "�ޔ��J�[�hID�F%s", g_pclsMainRecOpe->m_stEntryExitInfo.cLastID);
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, g_clsMainLogFile.m_cLogData);

    try
    {
        // ���ϐ��擾����
        // �V�X�e���̊��ϐ�������ގ��������݃f�[�^�t�@�C���̏����擾
        sprintf_s(
            g_clsMainLogFile.m_cLogData, 
            sizeof(g_clsMainLogFile.m_cLogData), 
            "***** �i%s�j���ϐ��擾�Ăяo�� *****", 
            g_maFileName.at(tagFileType::FILETYPE2).c_str());
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, g_clsMainLogFile.m_cLogData);
        iEnvResult = g_pclsMainSubOpe->GetEnvironmentDataProc(WRITE_FILE, cWriteLocalName);
        sprintf_s(g_clsMainLogFile.m_cLogData, sizeof(g_clsMainLogFile.m_cLogData), "���ϐ��擾���ʁF%d", iEnvResult);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
        g_clsMainLogFile.m_stLogData = g_pclsMainSubOpe->CreateEventMessageProc(
            ACTIONSET, 
            g_maFileName.at(tagFileType::FILETYPE2).c_str());

        // ���ϐ����擾�o�������`�F�b�N
        if (CM_ENVERROR == iEnvResult)
        {
            // ���ގ��������݃f�[�^�t�@�C���̊��ϐ����ݒ肳��Ă��Ȃ�
            // ���ގ��������݃f�[�^�t�@�C�����Ƀf�t�H���g�̓��ގ��������݃f�[�^�t�@�C������ݒ�
            strncpy_s(cWriteLocalName, sizeof(cWriteLocalName), DEFAULTFILENAME, sizeof(cWriteLocalName) - 1);
            sprintf_s(
                g_clsMainLogFile.m_cLogData,
                sizeof(g_clsMainLogFile.m_cLogData),
                "%s���F%s",
                g_maFileName.at(tagFileType::FILETYPE2).c_str(),
                cWriteLocalName);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
            sprintf_s(
                g_clsMainLogFile.m_cLogData,
                sizeof(g_clsMainLogFile.m_cLogData),
                "%s��%s",
                g_clsMainLogFile.m_stLogData.pcActionName,
                GETNOTENVCONTINUED);
            LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, g_clsMainLogFile.m_cLogData);
        }
        else
        {
            // ���ގ��������݃f�[�^�t�@�C���̊��ϐ��̐ݒ肠��
            // ���ϐ��擾�������b�Z�[�W���o��
            sprintf_s(
                g_clsMainLogFile.m_cLogData,
                sizeof(g_clsMainLogFile.m_cLogData),
                "%s���F%s",
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

        // �V�X�e���̊��ϐ�������ގ��ǂݍ��݃f�[�^�t�@�C���̏����擾
        sprintf_s(
            g_clsMainLogFile.m_cLogData,
            sizeof(g_clsMainLogFile.m_cLogData),
            "***** �i%s�j���ϐ��擾�Ăяo�� *****",
            g_maFileName.at(tagFileType::FILETYPE1).c_str());
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, g_clsMainLogFile.m_cLogData);
        iEnvResult = g_pclsMainSubOpe->GetEnvironmentDataProc(READ_FILE, cReadLocalName);
        sprintf_s(g_clsMainLogFile.m_cLogData, sizeof(g_clsMainLogFile.m_cLogData), "���ϐ��擾���ʁF%d", iEnvResult);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
        g_clsMainLogFile.m_stLogData = g_pclsMainSubOpe->CreateEventMessageProc(
            ACTIONSET,
            g_maFileName.at(tagFileType::FILETYPE1).c_str());

        // ���ϐ����擾�o�������`�F�b�N
        if (CM_ENVERROR == iEnvResult)
        {
            // ���ގ��ǂݍ��݃f�[�^�t�@�C�����ݒ肳��Ă��Ȃ�
            // �G���[�t���O��ON
            iErrFlag = ERR_ON;

            // �G���[�R�[�h��ݒ�
            g_clsMainLogFile.m_eErrorResultCode = tagErrorCode::ENVERROR;
            g_clsMainLogFile.m_maErrorActionName.insert(make_pair(iEnvResult, g_clsMainLogFile.m_stLogData.pcActionName));

            // �G���[��ݒ肷�邪�p�����[�^�Ƃ��Đݒ肵�Ă���\�������邽�ߏ������p������
            LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** ���ϐ��擾�Ăяo���ُ�I�� *****");
            sprintf_s(
                g_clsMainLogFile.m_cLogData,
                sizeof(g_clsMainLogFile.m_cLogData),
                "%s�i�㑱�����F�p�����[�^�`�F�b�N�j",
                ERRORPROCCONTINUED);
            LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, g_clsMainLogFile.m_cLogData);
        }
        else
        {
            // ���ގ��ǂݍ��݃f�[�^�t�@�C���̊��ϐ��̐ݒ肠��
            // ���ϐ��擾�������b�Z�[�W���o��
            sprintf_s(
                g_clsMainLogFile.m_cLogData,
                sizeof(g_clsMainLogFile.m_cLogData),
                "%s���F%s",
                g_maFileName.at(tagFileType::FILETYPE1).c_str(),
                cReadLocalName);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
            LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, "***** ���ϐ��擾�Ăяo������I�� *****");
            sprintf_s(
                g_clsMainLogFile.m_cLogData,
                sizeof(g_clsMainLogFile.m_cLogData),
                "%s��%s",
                g_clsMainLogFile.m_stLogData.pcActionName,
                GETENVSUCCESS);
            LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, g_clsMainLogFile.m_cLogData);

            // ��΃p�X�ϊ��t�@�C�����擾�����ֈړ�
            goto MAIN;
        }

        // �N���p�����[�^�`�F�b�N
        // �p�����[�^���`�F�b�N�i�擪�����͎��s�t�@�C�����E���������͎w�蕶����̂��߃`�F�b�N�ΏۊO�j
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �p�����[�^�`�F�b�N�J�n *****");
        sprintf_s(g_clsMainLogFile.m_cLogData, sizeof(g_clsMainLogFile.m_cLogData), "�p�����[�^�ݒ萔�F%d", iCmdCnt);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
        sprintf_s(
            cActinData,
            sizeof(cActinData),
            "%s�^%s��",
            g_maFileName.at(tagFileType::FILETYPE1).c_str(),
            g_maFileName.at(tagFileType::FILETYPE2).c_str());
        g_clsMainLogFile.m_stLogData = g_pclsMainSubOpe->CreateEventMessageProc(ACTIONSET, cActinData);
        switch (iCmdCnt)
        {
            case 2:
            {
                // �R�}���h��2��
                // ���ގ��ǂݍ��݃f�[�^�t�@�C�����y�ѓ��ގ��������݃f�[�^�t�@�C���̎w�肪����ꍇ
                // ���ގ��ǂݍ��݃f�[�^�t�@�C�����y�ѓ��ގ��������݃f�[�^�t�@�C������ݒ�
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
                    "%s���F%s",
                    g_maFileName.at(tagFileType::FILETYPE1).c_str(),
                    cReadLocalName);
                LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
                sprintf_s(
                    g_clsMainLogFile.m_cLogData,
                    sizeof(g_clsMainLogFile.m_cLogData),
                    "%s���F%s",
                    g_maFileName.at(tagFileType::FILETYPE2).c_str(),
                    cWriteLocalName);
                LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
                LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, "***** �p�����[�^�`�F�b�N����I�� *****");
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
                // �R�}���h��1��
                // ���ގ��ǂݍ��݃f�[�^�t�@�C�����݂̂̎w�肪����ꍇ
                // ���ގ��ǂݍ��݃f�[�^�t�@�C������ݒ�
                memcpy_s(cReadLocalName, sizeof(cReadLocalName), vecCmdData.at(iCmdCnt).c_str(), sizeof(cReadLocalName) - 1);

                // ���ϐ��ł̎擾�ɂ����ގ��������݃f�[�^�t�@�C�������ݒ肳��Ă��邩�`�F�b�N
                if (0 == strncmp(cWriteLocalName, "", sizeof(cWriteLocalName) - 1))
                {
                    // ���ގ��������݃f�[�^�t�@�C�������ݒ肳��Ă��Ȃ�
                    // ���ގ��������݃f�[�^�t�@�C�����Ƀf�t�H���g�̓��ގ��������݃f�[�^�t�@�C������ݒ�
                    memcpy_s(cWriteLocalName, sizeof(cWriteLocalName), DEFAULTFILENAME, sizeof(cWriteLocalName) - 1);
                    sprintf_s(
                        g_clsMainLogFile.m_cLogData,
                        sizeof(g_clsMainLogFile.m_cLogData),
                        "%s���F%s",
                        g_maFileName.at(tagFileType::FILETYPE1).c_str(),
                        cReadLocalName);
                    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
                    sprintf_s(
                        g_clsMainLogFile.m_cLogData,
                        sizeof(g_clsMainLogFile.m_cLogData),
                        "%s���F%s",
                        g_maFileName.at(tagFileType::FILETYPE2).c_str(),
                        cWriteLocalName);
                    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
                    LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, (char*)"***** �p�����[�^�`�F�b�N����I�� *****");
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
                    // ���ގ��������݃f�[�^�t�@�C�������ݒ肳��Ă���
                    // ���ގ��ǂݍ��݃f�[�^�t�@�C�����y�ѓ��ގ��������݃f�[�^�t�@�C���������O�ɏo��
                    sprintf_s(
                        g_clsMainLogFile.m_cLogData,
                        sizeof(g_clsMainLogFile.m_cLogData),
                        "%s���F%s",
                        g_maFileName.at(tagFileType::FILETYPE1).c_str(),
                        cReadLocalName);
                    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
                    sprintf_s(
                        g_clsMainLogFile.m_cLogData,
                        sizeof(g_clsMainLogFile.m_cLogData),
                        "%s���F%s", g_maFileName.at(tagFileType::FILETYPE2).c_str(),
                        cWriteLocalName);
                    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
                    LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, "***** �p�����[�^�`�F�b�N����I�� *****");
                    sprintf_s(
                        g_clsMainLogFile.m_cLogData,
                        sizeof(g_clsMainLogFile.m_cLogData),
                        "%s%s",
                        g_clsMainLogFile.m_stLogData.pcActionName,
                        GETPARAMSUCCESS);
                    LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, g_clsMainLogFile.m_cLogData);
                }

                // ���򔲂�
                break;
            }
            case 0:
            {
                // �R�}���h�Ȃ�
                // �G���[�Ƃ��ė�O�����ֈړ�
                iStartResult = CM_NOPARAM;
                g_clsMainLogFile.m_eErrorResultCode = tagErrorCode::NOPARAM;
                g_clsMainLogFile.m_maErrorActionName.insert(make_pair(iStartResult, g_clsMainLogFile.m_stLogData.pcActionName));
                LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** �p�����[�^�`�F�b�N�ُ�I�� *****");
                throw iStartResult;
            }
            default:
            {
                // ��L�ȊO
                // �G���[�Ƃ��ė�O�����ֈړ�
                iStartResult = CM_PARAMERROR;
                g_clsMainLogFile.m_eErrorResultCode = tagErrorCode::FAILPARAM;
                g_clsMainLogFile.m_maErrorActionName.insert(make_pair(iStartResult, g_clsMainLogFile.m_stLogData.pcActionName));
                LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** �p�����[�^�`�F�b�N�ُ�I�� *****");
                throw iStartResult;
            }
        }

MAIN:
        // �又��
        // ��΃p�X�ϊ��t�@�C�����擾����
        // ���ގ��ǂݍ��݃f�[�^�t�@�C�������΃p�X�t���ɕϊ�
        eFileType = tagFileType::FILETYPE1;
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ��΃p�X�ϊ��t�@�C�����擾�֐��Ăяo�� *****");
        g_stFileNameInfo.strReadName = g_pclsMainFileOpe->GetConvertFileNameProc(
            eFileType,
            cReadLocalName,
            vecCmdData.at(0).c_str(),
            (char *)READPATH);
        sprintf_s(
            g_clsMainLogFile.m_cLogData,
            sizeof(g_clsMainLogFile.m_cLogData),
            "��΃p�X�t��%s���F%s",
            g_maFileName.at(tagFileType::FILETYPE1).c_str(),
            g_stFileNameInfo.strReadName.c_str());
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

        // ���ގ��ǂݍ��݃f�[�^�t�@�C���̑��ݏ�Ԃ��擾
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �t�@�C����ԃ`�F�b�N�֐��Ăяo�� *****");
        memset(&g_pclsMainRecOpe->m_stStat, 0x00, sizeof(g_pclsMainRecOpe->m_stStat));
        iFileStatResult = g_pclsMainFileOpe->CheckFileStatProc(
            g_stFileNameInfo.strReadName.c_str(),
            &g_pclsMainRecOpe->m_stStat);
        sprintf_s(g_clsMainLogFile.m_cLogData, sizeof(g_clsMainLogFile.m_cLogData), "�t�@�C�����݃`�F�b�N���ʁF%d", iFileStatResult);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
        strCheckStatFileName =
            ("" == g_stFileNameInfo.strReadName) ?
            g_maFileName.at(tagFileType::FILETYPE1) :
            g_stFileNameInfo.strReadName;
        g_clsMainLogFile.m_stLogData = g_pclsMainSubOpe->CreateEventMessageProc(ACTIONSET, strCheckStatFileName.c_str());

        // ���ʂ��`�F�b�N
        if (EXIT_SUCCESS != iFileStatResult)
        {
            // �t�@�C�������݂��Ȃ�
            // �G���[�R�[�h�y�ёΏۃt�@�C������ݒ肵�ė�O�����ֈړ�
            LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** �t�@�C����ԃ`�F�b�N�ُ�I�� *****");
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
            // �t�@�C�������݂���
            // �t�@�C���������O�ɏo��
            sprintf_s(
                g_clsMainLogFile.m_cLogData,
                sizeof(g_clsMainLogFile.m_cLogData),
                "�t�@�C�����́F%s",
                g_stFileNameInfo.strReadName.c_str());
            LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, g_clsMainLogFile.m_cLogData);

            // �t�@�C���̏�Ԏ擾�����̃��b�Z�[�W�y�ѓ��ގ��ǂݍ��݃f�[�^�t�@�C���T�C�Y�����O�ɏo��
            LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, "***** �t�@�C����ԃ`�F�b�N����I�� *****");
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
                "%s�̌��݂̃t�@�C���T�C�Y�́F%d",
                g_maFileName.at(tagFileType::FILETYPE1).c_str(),
                g_pclsMainRecOpe->m_stStat.st_size);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
        }

        if (0 == g_pclsMainRecOpe->m_stStat.st_size)
        {
            // ���ގ��ǂݍ��݃f�[�^�t�@�C���̃T�C�Y��0
            // �t�@�C���T�C�Y�����O�ɏo��
            sprintf_s(
                g_clsMainLogFile.m_cLogData,
                sizeof(g_clsMainLogFile.m_cLogData),
                "%s �̃t�@�C���T�C�Y�́F%d",
                g_stFileNameInfo.strReadName.c_str(),
                g_pclsMainRecOpe->m_stStat.st_size);
            LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, g_clsMainLogFile.m_cLogData);

            // �t�@�C���R�s�[���s�킸���ގ��ǂݍ��݃f�[�^�t�@�C���ǂݍ��݂ֈړ�
            goto READ;
        }

        // ���ގ��ǂݍ��݃f�[�^�t�@�C���𕪊�
        strBackUpLocalName = g_stFileNameInfo.strReadName;
        deqBackUpFileName = g_pclsMainFileOpe->SplitDataProc(strBackUpLocalName, '\\');
        strBackUpFileName = deqBackUpFileName.back();
        sprintf_s(
            g_clsMainLogFile.m_cLogData,
            sizeof(g_clsMainLogFile.m_cLogData),
            "%s���F%s",
            g_maFileName.at(tagFileType::FILETYPE5).c_str(),
            strBackUpFileName.c_str());
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

        // ��΃p�X�ϊ��t�@�C�����擾����
        // �o�b�N�A�b�v�t�@�C�������΃p�X�t���ɕϊ�
        eFileType = tagFileType::FILETYPE5;
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ��΃p�X�ϊ��t�@�C�����擾�֐��Ăяo�� *****");
        g_stFileNameInfo.strBackupName = g_pclsMainFileOpe->GetConvertFileNameProc(
            eFileType,
            strBackUpFileName.c_str(),
            vecCmdData.at(0).c_str(),
            (char *)COPYPATH);
        sprintf_s(
            g_clsMainLogFile.m_cLogData,
            sizeof(g_clsMainLogFile.m_cLogData),
            "��΃p�X�t��%s���F%s",
            g_maFileName.at(tagFileType::FILETYPE5).c_str(),
            g_stFileNameInfo.strBackupName.c_str());
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

        // �t�@�C���R�s�[����
        // ���ގ��ǂݍ��݃f�[�^�t�@�C�����o�b�N�A�b�v�t�@�C���փR�s�[
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �t�@�C���R�s�[�֐��Ăяo�� *****");
        *piFileCopyResult = g_pclsMainFileOpe->CopyFileProc(
            strBackUpLocalName.c_str(),
            g_stFileNameInfo.strBackupName.c_str());
        sprintf_s(g_clsMainLogFile.m_cLogData, sizeof(g_clsMainLogFile.m_cLogData), "�R�s�[���ʁF%d", *piFileCopyResult);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
        g_clsMainLogFile.m_stLogData = g_pclsMainSubOpe->CreateEventMessageProc(
            ACTIONSET,
            g_stFileNameInfo.strBackupName.c_str());

        // �R�s�[���ʂ��`�F�b�N
        if (EXIT_SUCCESS != *piFileCopyResult && CM_FILECOPYERROR != *piFileCopyResult)
        {
            // �ُ�I���i�t�@�C���R�s�[�G���[�ȊO�j
            // �G���[�R�[�h��ݒ肵�ė�O�����ֈړ�
            iStartResult = *piFileCopyResult;
            g_clsMainLogFile.m_eErrorResultCode = (ERRORCODETABLE)iStartResult;
            g_clsMainLogFile.m_maErrorActionName.insert(
                make_pair(
                    iStartResult, 
                    g_clsMainLogFile.m_stLogData.pcActionName));
            LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** �t�@�C���R�s�[�֐��Ăяo���ُ�I�� *****");
            throw iStartResult;
        }
        else if (CM_FILECOPYERROR == *piFileCopyResult)
        {
            // �t�@�C���R�s�[�G���[����
            // �G���[�t���O��ON
            iErrFlag = ERR_ON;

            // �G���[�R�[�h��ݒ�
            iStartResult = CM_FILECOPYERROR;
            g_clsMainLogFile.m_eErrorResultCode = tagErrorCode::COPYERROR;
            g_clsMainLogFile.m_maErrorActionName.insert(
                make_pair(
                    iStartResult, 
                    g_clsMainLogFile.m_stLogData.pcActionName));

            // �G���[��ݒ肷�邪���ډe�����Ȃ����ߏ������p������
            LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** �t�@�C���R�s�[�֐��Ăяo���ُ�I�� *****");
            sprintf_s(
                g_clsMainLogFile.m_cLogData,
                sizeof(g_clsMainLogFile.m_cLogData),
                "%s�i�㑱�����F%s�̃��l�[���j",
                ERRORPROCCONTINUED,
                g_maFileName.at(tagFileType::FILETYPE1).c_str());
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
        }
        else
        {
            // �t�@�C���R�s�[����
            // �t�@�C���R�s�[�����̃��b�Z�[�W�����O�ɏo��
            LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, "***** �t�@�C���R�s�[�֐��Ăяo������I�� *****");
            sprintf_s(
                g_clsMainLogFile.m_cLogData,
                sizeof(g_clsMainLogFile.m_cLogData),
                "%s �ɑ΂���%s",
                g_clsMainLogFile.m_stLogData.pcActionName,
                FILECOPYSUCCESS);
            LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, g_clsMainLogFile.m_cLogData);
            sprintf_s(
                g_clsMainLogFile.m_cLogData, 
                sizeof(g_clsMainLogFile.m_cLogData), 
                "�߂�l�́F%d", 
                *piFileCopyResult);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
        }

        // ��΃p�X�ϊ��t�@�C�����擾����
        // �o�b�N�A�b�v�t�@�C�����Ɋ�Â��Ė��̕ύX�Ώۃt�@�C�������擾
        eFileType = tagFileType::FILETYPE6;
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ��΃p�X�ϊ��t�@�C�����擾�֐��Ăяo�� *****");
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
            "��΃p�X�t��%s���F%s",
            g_maFileName.at(tagFileType::FILETYPE6).c_str(),
            g_stFileNameInfo.strChangeName.c_str());
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

        // �t�@�C�����ϊ�����
        // �R�s�[�����t�@�C�����𖼏̕ύX�Ώۃt�@�C�����i�o�b�N�A�b�v�t�@�C�����j�Ƀ��l�[��
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �t�@�C�����ϊ��֐��Ăяo�� *****");
        iRenameResult = g_pclsMainFileOpe->ChangeFileNameProc(
            g_stFileNameInfo.strBackupName.c_str(),
            g_stFileNameInfo.strChangeName.c_str());
        g_clsMainLogFile.m_stLogData = g_pclsMainSubOpe->CreateEventMessageProc(
            ACTIONSET,
            g_stFileNameInfo.strBackupName.c_str());
        sprintf_s(g_clsMainLogFile.m_cLogData, sizeof(g_clsMainLogFile.m_cLogData), "���s���ʁF%d", iRenameResult);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

        // ���l�[���o�������`�F�b�N
        if (CM_RENAMEERROR == iRenameResult)
        {
            // �R�s�[�����t�@�C���ɑ΂��郊�l�[�����s
            // �G���[�R�[�h��ݒ肵�ė�O�����ֈړ�
            iStartResult = iRenameResult;
            g_clsMainLogFile.m_eErrorResultCode = tagErrorCode::RENAMEERROR;
            g_clsMainLogFile.m_maErrorActionName.insert(
                make_pair(
                    iRenameResult, 
                    g_clsMainLogFile.m_stLogData.pcActionName));
            LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** �t�@�C�����ϊ��֐��Ăяo���ُ�I�� *****");
            throw iStartResult;
        }
        else
        {
            // ���l�[������
            // ���l�[�������̃��b�Z�[�W�����O�ɏo��
            LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, "***** �t�@�C�����ϊ��֐��Ăяo������I�� *****");
            sprintf_s(
                g_clsMainLogFile.m_cLogData,
                sizeof(g_clsMainLogFile.m_cLogData),
                "%s �ɑ΂���%s", g_clsMainLogFile.m_stLogData.pcActionName,
                RENAMESUCCESS);
            LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, g_clsMainLogFile.m_cLogData);
            sprintf_s(g_clsMainLogFile.m_cLogData, sizeof(g_clsMainLogFile.m_cLogData), "�߂�l�́F%d", iRenameResult);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

            // ���l�[���Ώۂ̖��̕ύX�Ώۃt�@�C�������o�b�N�A�b�v�t�@�C�����ɃR�s�[
            g_stFileNameInfo.strBackupName = g_stFileNameInfo.strChangeName;
            g_pclsMainFileOpe->m_strFileOpeCopyFileName = g_stFileNameInfo.strChangeName;
            g_pclsMainRecOpe->m_strFileOpeCopyFileName = g_stFileNameInfo.strChangeName;
            sprintf_s(
                g_clsMainLogFile.m_cLogData,
                sizeof(g_clsMainLogFile.m_cLogData),
                "��΃p�X�t��%s���F%s",
                g_maFileName.at(tagFileType::FILETYPE5).c_str(),
                g_stFileNameInfo.strBackupName.c_str());
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
        }

READ:
        // ��΃p�X�t�����ގ��ǂݍ��݃f�[�^�t�@�C������ݒ�
        strncpy_s(
            cReadLocalName,
            sizeof(cReadLocalName),
            g_stFileNameInfo.strReadName.c_str(),
            sizeof(cReadLocalName) - 1);
        sprintf_s(
            g_clsMainLogFile.m_cLogData,
            sizeof(g_clsMainLogFile.m_cLogData),
            "���[�J����%s���F%s",
            g_maFileName.at(tagFileType::FILETYPE1).c_str(),
            cReadLocalName);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

        // ���ގ��ǂݍ��݃f�[�^�t�@�C����ǂݎ��̃o�C�i���^�ŃI�[�v��
        eReadErrNo = fopen_s(pfReadFile, cReadLocalName, "rb");
        g_clsMainLogFile.m_stLogData = g_pclsMainSubOpe->CreateEventMessageProc(ACTIONSET, cReadLocalName);
        sprintf_s(
            g_clsMainLogFile.m_cLogData,
            sizeof(g_clsMainLogFile.m_cLogData),
            "***** %s�I�[�v���J�n *****",
            g_maFileName.at(tagFileType::FILETYPE1).c_str());
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, g_clsMainLogFile.m_cLogData);
        sprintf_s(g_clsMainLogFile.m_cLogData, sizeof(g_clsMainLogFile.m_cLogData), "�I�[�v�����ʁF%d", eReadErrNo);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

        // �t�@�C���I�[�v�����ʂ��`�F�b�N
        if (0 != eReadErrNo)
        {
            // �t�@�C���I�[�v�����s
            // �G���[�R�[�h��ݒ肵�ė�O�����ֈړ�
            sprintf_s(
                g_clsMainLogFile.m_cLogData,
                sizeof(g_clsMainLogFile.m_cLogData),
                "***** %s�I�[�v���ُ�I�� *****",
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
            // �t�@�C���I�[�v������
            // �t�@�C���I�[�v�������̃��b�Z�[�W�����O�ɏo��
            sprintf_s(
                g_clsMainLogFile.m_cLogData,
                sizeof(g_clsMainLogFile.m_cLogData),
                "***** %s�I�[�v������I�� *****",
                g_maFileName.at(tagFileType::FILETYPE1).c_str());
            LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, g_clsMainLogFile.m_cLogData);
            sprintf_s(
                g_clsMainLogFile.m_cLogData,
                sizeof(g_clsMainLogFile.m_cLogData),
                "%s %s", g_clsMainLogFile.m_stLogData.pcActionName,
                FILEOPENSUCCESS);
            LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, g_clsMainLogFile.m_cLogData);
        }

        // ��΃p�X�ϊ��t�@�C�����擾����
        // ���ގ��������݃f�[�^�t�@�C�������΃p�X�t���t�@�C�����ɕϊ�
        eFileType = tagFileType::FILETYPE2;
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ��΃p�X�ϊ��t�@�C�����擾�֐��Ăяo�� *****");
        g_stFileNameInfo.strWriteName = g_pclsMainFileOpe->GetConvertFileNameProc(
            eFileType,
            cWriteLocalName,
            vecCmdData.at(0).c_str(),
            (char *)WRITEPATH, 
            (char *)DEFAULTFILENAME);
        sprintf_s(
            g_clsMainLogFile.m_cLogData,
            sizeof(g_clsMainLogFile.m_cLogData),
            "��΃p�X�t��%s���F%s",
            g_maFileName.at(tagFileType::FILETYPE2).c_str(),
            g_stFileNameInfo.strWriteName.c_str());
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

        // �ޔ��������ގ��������݃f�[�^�t�@�C������ݒ�
        strncpy_s(
            cWriteLocalName,
            sizeof(cWriteLocalName),
            g_stFileNameInfo.strWriteName.c_str(),
            sizeof(cWriteLocalName) - 1);
        sprintf_s(
            g_clsMainLogFile.m_cLogData,
            sizeof(g_clsMainLogFile.m_cLogData),
            "���[�J����%s���F%s",
            g_maFileName.at(tagFileType::FILETYPE2).c_str(),
            cWriteLocalName);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

        // ���ގ��������݃f�[�^�t�@�C�����X�V�ǋL�^�ŃI�[�v��
        sprintf_s(
            g_clsMainLogFile.m_cLogData,
            sizeof(g_clsMainLogFile.m_cLogData),
            "***** %s�I�[�v���J�n *****",
            g_maFileName.at(tagFileType::FILETYPE2).c_str());
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, g_clsMainLogFile.m_cLogData);
        eWriteErrNo = fopen_s(pfWriteFile, cWriteLocalName, "a+");
        g_clsMainLogFile.m_stLogData = g_pclsMainSubOpe->CreateEventMessageProc(ACTIONSET, cWriteLocalName);
        sprintf_s(g_clsMainLogFile.m_cLogData, sizeof(g_clsMainLogFile.m_cLogData), "�I�[�v�����ʁF%d", eWriteErrNo);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

        // �t�@�C���I�[�v�����ʂ��`�F�b�N
        if (0 != eWriteErrNo)
        {
            // �t�@�C���I�[�v�����s
            // �G���[�R�[�h��ݒ肵�ė�O�����ֈړ�
            sprintf_s(
                g_clsMainLogFile.m_cLogData,
                sizeof(g_clsMainLogFile.m_cLogData),
                "***** %s�I�[�v���ُ�I�� *****",
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
            // �t�@�C���I�[�v������
            // �t�@�C���I�[�v�������̃��b�Z�[�W�����O�ɏo��
            sprintf_s(
                g_clsMainLogFile.m_cLogData,
                sizeof(g_clsMainLogFile.m_cLogData),
                "***** %s�I�[�v������I�� *****",
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

    // ��O����
    catch (const int iExecErr)
    {
        // �G���[���b�Z�[�W��������
        // �G���[���b�Z�[�W�����O�ɏo��
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �G���[���b�Z�[�W�����֐��Ăяo�� *****");
        g_clsMainLogFile.m_stLogData = g_pclsMainSubOpe->CreateEventMessageProc(
            ERRORSET,
            g_clsMainLogFile.m_maErrorActionName.at(iExecErr).c_str(),
            g_clsMainLogFile.m_eErrorResultCode);
        LOGOUTPUTERROR(
            TRACEOUTPUTLOG,
            ERRORINFORMATION1,
            g_clsMainLogFile.m_stLogData.pcMessage,
            g_clsMainLogFile.m_stLogData.eErrorCode);
        sprintf_s(g_clsMainLogFile.m_cLogData, sizeof(g_clsMainLogFile.m_cLogData), "�߂�l�́F%d", iExecErr);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

        // �Ăяo���ɑ΂���ُ�I���̃��b�Z�[�W�����O�ɏo��
        LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** ���ގ��󋵈󎚐���J�n�����ُ�I�� *****");
    }

    // �㏈��
    // �G���[�t���O���`�F�b�N
    if (ERR_ON == iErrFlag)
    {
        // �G���[�t���O��ON
        // �G���[�̐������[�v�imap�I�u�W�F�N�g�̃C�e���[�^�g�p�j
        for (auto auErrItr = g_clsMainLogFile.m_maErrorActionName.begin(); 
                  auErrItr != g_clsMainLogFile.m_maErrorActionName.end(); 
                  ++auErrItr)
        {
            // �G���[���b�Z�[�W��������
            // �G���[���b�Z�[�W�����O�ɏo��
            LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �G���[���b�Z�[�W�����֐��Ăяo�� *****");
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
                "�߂�l�́F%d",
                (ERRORCODETABLE)auErrItr->first);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
        }
    }
    else
    {
        // �G���[�t���O��OFF�i�Ăяo�������j
        // �Ăяo���ɑ΂��鐳��I���̃��b�Z�[�W�����O�ɏo��
        LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, "***** ���ގ��󋵈󎚐���J�n��������I�� *****");
        sprintf_s(g_clsMainLogFile.m_cLogData, sizeof(g_clsMainLogFile.m_cLogData), "�߂�l�́F%d", iStartResult);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
    }

    // �������擾����
    // ���ގ��󋵈󎚐���J�n�����I�������擾
    g_clsMainLogFile.m_stTimeData = g_clsMainLogFile.GetDateTimeProc();
    sprintf_s(
        g_clsMainLogFile.m_cLogData, 
        sizeof(g_clsMainLogFile.m_cLogData), 
        "���ގ��󋵈󎚐���J�n�����I�������F%s %s", 
        (char *)g_clsMainLogFile.m_stTimeData.strMachDatePlus.c_str(), 
        (char *)g_clsMainLogFile.m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, g_clsMainLogFile.m_cLogData);
    return iStartResult;
}

//********************************************
// �֐����F���ގ��󋵈󎚐���I������
// Function Name�FDataControlEndProc
// 
// �����T�v�F
// �A�v���P�[�V�����I���Ɋ�Â��Č㏈�����s��
// 
// �����菇�F
//   �P�D���ގ��������݃f�[�^�t�@�C���N���[�Y
//   �Q�D���ގ��ǂݍ��݃f�[�^�t�@�C���N���[�Y
//   �R�D���ގ��ǂݍ��݃f�[�^�t�@�C����Ԏ擾
//   �S�D���ގ��������݃f�[�^�t�@�C����Ԏ擾
//   �T�D���ގ��ǂݍ��݃f�[�^�t�@�C�������o�b�N�A�b�v�t�@�C�����ւ̃��l�[��
//   �U�D���ގ��ǂݍ��݃f�[�^�t�@�C���폜
//   �V�D���ގ��������݃f�[�^�t�@�C���폜
//   �W�D�G���[���b�Z�[�W���O�o��
// 
// �����F     ���ގ��ǂݍ��݃f�[�^�t�@�C���|�C���^
//            ���ގ��������݃f�[�^�t�@�C���|�C���^
//            �R�}���h���
//            �t�@�C���R�s�[�Ăяo�����s����
// �߂�l�F   ���s���ʁi����I����stdlib.h�Œ�`���Ă�����e�𗘗p�j
//            ����I���FEXIT_SUCCESS�i0�j
//            �ُ�I���F�t�@�C���N���[�Y�G���[�FCM_FILECLOSEERROR�i105�j
//                      �t�@�C�����ϊ��G���[�FCM_RENAMEERROR�i111�j
//                      �t�@�C���폜�G���[�FCM_FILEDELETEERROR�i112�j
//                      ��Ԏ擾�G���[�FCM_FILESTATERROR�i113�j
// 
// �⑫����
// �E��L�R, �S�̓I�[�v�����̂ݎ��s����
// �E��L�T�̓t�@�C���R�s�[�Ɏ��s�����ꍇ�̂ݎ��s����
// �E��L�U�̓t�@�C���R�s�[�������t�@�C�������݂���ꍇ�̂ݎ��s����
// �E��L�V�͏������݃t�@�C���Ƀf�[�^���Ȃ��ꍇ�̂ݎ��s����
// �E��L�W�̓G���[�����������ꍇ�̂ݎ��s����
// 
// 
// �쐬����
// 2020/9/25  s-saito   �V�K�쐬
// 
// �X�V����
// 
// 
//********************************************
int DataControlEndProc(
    FILE **pfReadFile,
    FILE **pfWriteFile,
    vector<string> vecCmdData,
    int iCopyResult)
{
    // �ϐ��錾
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

    // �O����
    // �������擾����
    // ���ގ��󋵈󎚐���I�������J�n�����擾
    g_clsMainLogFile.m_stTimeData = g_clsMainLogFile.GetDateTimeProc();
    sprintf_s(
        g_clsMainLogFile.m_cLogData, 
        sizeof(g_clsMainLogFile.m_cLogData), 
        "���ގ��󋵈󎚐���I�������J�n�����F%s %s", 
        (char *)g_clsMainLogFile.m_stTimeData.strMachDatePlus.c_str(), 
        (char *)g_clsMainLogFile.m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, g_clsMainLogFile.m_cLogData);
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���ގ��󋵈󎚐���I�������J�n *****");

    // �又��
    try
    {
        // �t�@�C�����I�[�v������Ă��邩�`�F�b�N
        // ���ގ��������݃f�[�^�t�@�C��
        if (NULL != *pfWriteFile)
        {
            // ���ގ��������݃f�[�^�t�@�C���I�[�v����
            // ���ގ��������݃f�[�^�t�@�C�����N���[�Y
            sprintf_s(
                g_clsMainLogFile.m_cLogData,
                sizeof(g_clsMainLogFile.m_cLogData),
                "***** %s�N���[�Y�J�n *****",
                g_maFileName.at(tagFileType::FILETYPE2).c_str());
            LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, g_clsMainLogFile.m_cLogData);
            iCloseResult = fclose(*pfWriteFile);
            g_clsMainLogFile.m_stLogData = g_pclsMainSubOpe->CreateEventMessageProc(
                ACTIONSET,
                g_stFileNameInfo.strWriteName.c_str());
            sprintf_s(g_clsMainLogFile.m_cLogData, sizeof(g_clsMainLogFile.m_cLogData), "���ʁF%d", iCloseResult);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

            // ���ގ��������݃f�[�^�t�@�C�����N���[�Y�o�������`�F�b�N
            if (EOF == iCloseResult)
            {
                // �N���[�Y���s
                // �G���[�Ƃ��ăG���[�R�[�h��ݒ肵�ė�O�����փX���[
                iEndResult = CM_FILECLOSEERROR;
                g_clsMainLogFile.m_eErrorResultCode = tagErrorCode::FILECLOSEERROR;
                g_clsMainLogFile.m_maErrorActionName.insert(
                    make_pair(
                        iEndResult, 
                        g_clsMainLogFile.m_stLogData.pcActionName));
                sprintf_s(
                    g_clsMainLogFile.m_cLogData,
                    sizeof(g_clsMainLogFile.m_cLogData),
                    "***** %s�N���[�Y�ُ�I�� *****",
                    g_maFileName.at(tagFileType::FILETYPE2).c_str());
                LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, g_clsMainLogFile.m_cLogData);
                throw iEndResult;
            }
            else
            {
                // �t�@�C���N���[�Y����
                // �t�@�C���N���[�Y�����̃��b�Z�[�W�����O�ɏo��
                sprintf_s(g_clsMainLogFile.m_cLogData, sizeof(g_clsMainLogFile.m_cLogData), "���ʁF%d", iCloseResult);
                LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
                sprintf_s(
                    g_clsMainLogFile.m_cLogData,
                    sizeof(g_clsMainLogFile.m_cLogData),
                    "***** %s�N���[�Y����I�� *****",
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

            // ���ގ��������݃f�[�^�t�@�C���̑��ݏ�Ԃ��擾
            LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �t�@�C����ԃ`�F�b�N�֐��Ăяo�� *****");
            memset(&g_pclsMainRecOpe->m_stStat, 0x00, sizeof(g_pclsMainRecOpe->m_stStat));
            iFileStatResult = g_pclsMainFileOpe->CheckFileStatProc(
                g_stFileNameInfo.strWriteName.c_str(),
                &g_pclsMainRecOpe->m_stStat);
            sprintf_s(
                g_clsMainLogFile.m_cLogData, 
                sizeof(g_clsMainLogFile.m_cLogData), 
                "�t�@�C�����݃`�F�b�N���ʁF%d", 
                iFileStatResult);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
            strCheckStatFileName =
                ("" == g_stFileNameInfo.strWriteName) ?
                g_maFileName.at(tagFileType::FILETYPE2) :
                g_stFileNameInfo.strWriteName;
            g_clsMainLogFile.m_stLogData = g_pclsMainSubOpe->CreateEventMessageProc(
                ACTIONSET, 
                strCheckStatFileName.c_str());

            // ���ʂ��`�F�b�N
            if (EXIT_SUCCESS != iFileStatResult)
            {
                // �t�@�C�������݂��Ȃ�
                // �G���[�R�[�h�y�ёΏۃt�@�C������ݒ肵�ė�O�����փX���[
                LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** �t�@�C����ԃ`�F�b�N�ُ�I�� *****");
                iEndResult = CM_FILESTATERROR;
                g_clsMainLogFile.m_eErrorResultCode = tagErrorCode::STATERROR;
                g_clsMainLogFile.m_maErrorActionName.insert(
                    make_pair(
                        iEndResult, 
                        g_clsMainLogFile.m_stLogData.pcActionName));
                sprintf_s(
                    g_clsMainLogFile.m_cLogData,
                    sizeof(g_clsMainLogFile.m_cLogData),
                    "�t�@�C�����́F%s",
                    g_stFileNameInfo.strWriteName.c_str());
                LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
                throw iEndResult;
            }
            else
            {
                // �t�@�C�������݂���
                // �t�@�C���������O�ɏo��
                sprintf_s(
                    g_clsMainLogFile.m_cLogData,
                    sizeof(g_clsMainLogFile.m_cLogData),
                    "�t�@�C�����́F%s",
                    g_stFileNameInfo.strWriteName.c_str());
                LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, g_clsMainLogFile.m_cLogData);

                // �t�@�C���̏�Ԏ擾�����̃��b�Z�[�W�y�ѓ��ގ��������݃f�[�^�t�@�C���T�C�Y�����O�ɏo��
                LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, "***** �t�@�C����ԃ`�F�b�N����I�� *****");
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
                    "%s�̌��݂̃t�@�C���T�C�Y�́F%d",
                    g_maFileName.at(tagFileType::FILETYPE2).c_str(),
                    g_pclsMainRecOpe->m_stStat.st_size);
                LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
            }
        }

        // ���ގ��ǂݍ��݃f�[�^�t�@�C��
        if (NULL != *pfReadFile)
        {
            // ���ގ��ǂݍ��݃f�[�^�t�@�C���I�[�v����
            // ���ގ��ǂݍ��݃f�[�^�t�@�C�����N���[�Y
            sprintf_s(
                g_clsMainLogFile.m_cLogData,
                sizeof(g_clsMainLogFile.m_cLogData),
                "***** %s�N���[�Y�J�n *****",
                g_maFileName.at(tagFileType::FILETYPE1).c_str());
            LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, g_clsMainLogFile.m_cLogData);
            iCloseResult = fclose(*pfReadFile);
            g_clsMainLogFile.m_stLogData = g_pclsMainSubOpe->CreateEventMessageProc(
                ACTIONSET,
                g_stFileNameInfo.strReadName.c_str());
            sprintf_s(g_clsMainLogFile.m_cLogData, sizeof(g_clsMainLogFile.m_cLogData), "���ʁF%d", iCloseResult);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

            // ���ގ��ǂݍ��݃f�[�^�t�@�C�����N���[�Y�o�������`�F�b�N
            if (EOF == iCloseResult)
            {
                // �N���[�Y���s
                // �G���[�Ƃ��ăG���[�R�[�h��ݒ肵�ė�O�����փX���[
                iEndResult = CM_FILECLOSEERROR;
                g_clsMainLogFile.m_eErrorResultCode = tagErrorCode::FILECLOSEERROR;
                g_clsMainLogFile.m_maErrorActionName.insert(
                    make_pair(iEndResult, g_clsMainLogFile.m_stLogData.pcActionName));
                sprintf_s(
                    g_clsMainLogFile.m_cLogData,
                    sizeof(g_clsMainLogFile.m_cLogData),
                    "***** %s�N���[�Y�ُ�I�� *****",
                    g_maFileName.at(tagFileType::FILETYPE1).c_str());
                LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, g_clsMainLogFile.m_cLogData);
                throw iEndResult;
            }
            else
            {
                // �t�@�C���N���[�Y����
                // �t�@�C���N���[�Y�����̃��b�Z�[�W�����O�ɏo��
                sprintf_s(
                    g_clsMainLogFile.m_cLogData,
                    sizeof(g_clsMainLogFile.m_cLogData),
                    "***** %s�N���[�Y����I�� *****",
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

        // �t�@�C���R�s�[�G���[���������Ă��邩�`�F�b�N
        sprintf_s(
            g_clsMainLogFile.m_cLogData, 
            sizeof(g_clsMainLogFile.m_cLogData), 
            "�t�@�C���R�s�[���ʂ́F%d", 
            iCopyResult);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
        if (CM_FILECOPYERROR == iCopyResult)
        {
            // �t�@�C���R�s�[�G���[����
            // ���ގ��ǂݍ��݃f�[�^�t�@�C��������Ƀo�b�N�A�b�v�t�@�C�������Đ���
            // ���ގ��ǂݍ��݃f�[�^�t�@�C���𕪊�
            strBackUpLocalName = g_stFileNameInfo.strReadName;
            deqBackUpFileName = g_pclsMainFileOpe->SplitDataProc(strBackUpLocalName, '\\');
            strBackUpFileName = deqBackUpFileName.back();
            sprintf_s(
                g_clsMainLogFile.m_cLogData,
                sizeof(g_clsMainLogFile.m_cLogData),
                "%s���F%s", g_maFileName.at(tagFileType::FILETYPE5).c_str(),
                strBackUpFileName.c_str());
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

            // ��΃p�X�ϊ��t�@�C�����擾����
            // �o�b�N�A�b�v�t�@�C�������΃p�X�t���ɕϊ�
            eFileType = tagFileType::FILETYPE5;
            LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ��΃p�X�ϊ��t�@�C�����擾�֐��Ăяo�� *****");
            g_stFileNameInfo.strBackupName = g_pclsMainFileOpe->GetConvertFileNameProc(
                eFileType,
                strBackUpFileName.c_str(),
                vecCmdData.at(0).c_str(),
                (char *)COPYPATH);
            sprintf_s(
                g_clsMainLogFile.m_cLogData,
                sizeof(g_clsMainLogFile.m_cLogData),
                "��΃p�X�t��%s���F%s",
                g_maFileName.at(tagFileType::FILETYPE5).c_str(),
                g_stFileNameInfo.strBackupName.c_str());
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

            // ��΃p�X�ϊ��t�@�C�����擾����
            // �o�b�N�A�b�v�t�@�C��������ɖ��̕ύX�Ώۃt�@�C�������擾
            eFileType = tagFileType::FILETYPE6;
            LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ��΃p�X�ϊ��t�@�C�����擾�֐��Ăяo�� *****");
            g_stFileNameInfo.strChangeName = g_pclsMainFileOpe->GetConvertFileNameProc(
                eFileType,
                g_stFileNameInfo.strBackupName.c_str(),
                vecCmdData.at(0).c_str());
            sprintf_s(
                g_clsMainLogFile.m_cLogData,
                sizeof(g_clsMainLogFile.m_cLogData),
                "��΃p�X�t��%s���F%s",
                g_maFileName.at(tagFileType::FILETYPE6).c_str(),
                g_stFileNameInfo.strChangeName.c_str());
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

            // �t�@�C�����ϊ�����
            // ���ގ��ǂݍ��݃f�[�^�t�@�C�������o�b�N�A�b�v�t�@�C�����Ƀ��l�[�����ăo�b�N�A�b�v�p�X�ֈړ�
            // �t�@�C�����ړ�������ꍇ��ChangeFileNameProc�֐���rename�֐����g�p����
            LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �t�@�C�����ϊ��֐��Ăяo�� *****");
            iRenameResult = g_pclsMainFileOpe->ChangeFileNameProc(
                strBackUpLocalName.c_str(),
                g_stFileNameInfo.strChangeName.c_str());
            g_clsMainLogFile.m_stLogData = g_pclsMainSubOpe->CreateEventMessageProc(
                ACTIONSET,
                g_stFileNameInfo.strChangeName.c_str());
            sprintf_s(g_clsMainLogFile.m_cLogData, sizeof(g_clsMainLogFile.m_cLogData), "���s���ʁF%d", iRenameResult);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

            // ���l�[���o�������`�F�b�N
            if (CM_RENAMEERROR == iRenameResult)
            {
                // �R�s�[�����t�@�C���ɑ΂��郊�l�[�����s
                // �G���[�R�[�h��ݒ肵�ė�O�����ֈړ�
                iEndResult = CM_RENAMEERROR;
                g_clsMainLogFile.m_eErrorResultCode = tagErrorCode::RENAMEERROR;
                g_clsMainLogFile.m_maErrorActionName.insert(
                    make_pair(
                        iEndResult, 
                        g_clsMainLogFile.m_stLogData.pcActionName));
                LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** �t�@�C�����ϊ��֐��Ăяo���ُ�I�� *****");
                throw iEndResult;
            }
            else
            {
                // ���l�[������
                // ���l�[�������̃��b�Z�[�W�����O�ɏo��
                LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, "***** �t�@�C�����ϊ��֐��Ăяo������I�� *****");
                sprintf_s(
                    g_clsMainLogFile.m_cLogData,
                    sizeof(g_clsMainLogFile.m_cLogData),
                    "%s %s", g_clsMainLogFile.m_stLogData.pcActionName,
                    RENAMESUCCESS);
                LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, g_clsMainLogFile.m_cLogData);
                sprintf_s(g_clsMainLogFile.m_cLogData, sizeof(g_clsMainLogFile.m_cLogData), "�߂�l�́F%d", iRenameResult);
                LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

                // ���l�[���Ώۂ̖��̕ύX�Ώۃt�@�C�������o�b�N�A�b�v�t�@�C�����ɃR�s�[
                g_stFileNameInfo.strBackupName = g_stFileNameInfo.strChangeName;
                sprintf_s(
                    g_clsMainLogFile.m_cLogData,
                    sizeof(g_clsMainLogFile.m_cLogData),
                    "��΃p�X�t��%s���F%s",
                    g_maFileName.at(tagFileType::FILETYPE5).c_str(),
                    g_stFileNameInfo.strBackupName.c_str());
                LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
            }
        }
        else if (CM_FILECOPYERROR != iCopyResult && EXIT_SUCCESS != iCopyResult)
        {
            // �t�@�C���R�s�[�ȊO�̃G���[����
            // �t�@�C���R�s�[�ُ�I���̃��b�Z�[�W�����O�ɏo��
            LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** �t�@�C���R�s�[�����ɂĈُ�I�� *****");
        }
        else
        {
            // �t�@�C���R�s�[����
            // ��΃p�X�ϊ��t�@�C�����擾����
            // �폜�Ώۃt�@�C�������擾
            eFileType = tagFileType::FILETYPE7;
            LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ��΃p�X�ϊ��t�@�C�����擾�֐��Ăяo�� *****");
            g_stFileNameInfo.strDeleteName = g_pclsMainFileOpe->GetConvertFileNameProc(
                eFileType,
                g_stFileNameInfo.strReadName.c_str(),
                vecCmdData.at(0).c_str());
            sprintf_s(
                g_clsMainLogFile.m_cLogData,
                sizeof(g_clsMainLogFile.m_cLogData),
                "��΃p�X�t��%s���F%s",
                g_maFileName.at(tagFileType::FILETYPE7).c_str(),
                g_stFileNameInfo.strDeleteName.c_str());
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

            // �t�@�C���폜����
            // ���̓��ގ��ǂݍ��݃f�[�^�t�@�C���͕s�v�̂��߃t�@�C���폜���������s
            LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �t�@�C���폜�֐��Ăяo�� *****");
            iDeleteResult = g_pclsMainFileOpe->DeleteFileProc(g_stFileNameInfo.strDeleteName.c_str());
            g_clsMainLogFile.m_stLogData = g_pclsMainSubOpe->CreateEventMessageProc(
                ACTIONSET,
                g_stFileNameInfo.strDeleteName.c_str());
            sprintf_s(g_clsMainLogFile.m_cLogData, sizeof(g_clsMainLogFile.m_cLogData), "���s���ʁF%d", iDeleteResult);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

            // �G���[���������Ă��Ȃ����`�F�b�N
            if (CM_FILEDELETEERROR == iDeleteResult)
            {
                // �t�@�C���폜���s
                // �G���[�R�[�h��ݒ肵�ė�O�����ֈړ�
                iEndResult = CM_FILEDELETEERROR;
                g_clsMainLogFile.m_eErrorResultCode = tagErrorCode::DELETEERROR;
                g_clsMainLogFile.m_maErrorActionName.insert(
                    make_pair(
                        iEndResult, 
                        g_clsMainLogFile.m_stLogData.pcActionName));
                LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** �t�@�C���폜�֐��Ăяo���ُ�I�� *****");
                throw iEndResult;
            }
            else
            {
                // �t�@�C���폜����
                // �t�@�C���폜�����̃��b�Z�[�W�����O�ɏo��
                LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, "***** �t�@�C���폜�֐��Ăяo������I�� *****");
                sprintf_s(
                    g_clsMainLogFile.m_cLogData,
                    sizeof(g_clsMainLogFile.m_cLogData),
                    "%s %s",
                    g_clsMainLogFile.m_stLogData.pcActionName,
                    DELETESUCCESS);
                LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, g_clsMainLogFile.m_cLogData);
                sprintf_s(g_clsMainLogFile.m_cLogData, sizeof(g_clsMainLogFile.m_cLogData), "�߂�l�́F%d", iDeleteResult);
                LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
            }
        }

        // ���ގ��������݃f�[�^�t�@�C���̃T�C�Y���`�F�b�N
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �t�@�C���T�C�Y�̃`�F�b�N�����s *****");
        sprintf_s(
            g_clsMainLogFile.m_cLogData,
            sizeof(g_clsMainLogFile.m_cLogData),
            "�t�@�C���T�C�Y�́F%d", g_pclsMainRecOpe->m_stStat.st_size);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
        if (EXIT_SUCCESS == iFileStatResult && 0 == g_pclsMainRecOpe->m_stStat.st_size)
        {
            // �t�@�C���ɏ������݂��Ȃ�
            // �t�@�C���������O�ɏo��
            sprintf_s(
                g_clsMainLogFile.m_cLogData,
                sizeof(g_clsMainLogFile.m_cLogData),
                "�t�@�C�����́F%s",
                g_stFileNameInfo.strWriteName.c_str());
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

            // ��΃p�X�ϊ��t�@�C�����擾����
            // �폜�Ώۃt�@�C�������擾
            eFileType = tagFileType::FILETYPE7;
            LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ��΃p�X�ϊ��t�@�C�����擾�֐��Ăяo�� *****");
            g_stFileNameInfo.strDeleteName = g_pclsMainFileOpe->GetConvertFileNameProc(
                eFileType,
                g_stFileNameInfo.strWriteName.c_str(),
                vecCmdData.at(0).c_str());
            sprintf_s(
                g_clsMainLogFile.m_cLogData,
                sizeof(g_clsMainLogFile.m_cLogData),
                "��΃p�X�t��%s���F%s",
                g_maFileName.at(tagFileType::FILETYPE7).c_str(),
                g_stFileNameInfo.strDeleteName.c_str());
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

            // �t�@�C���폜����
            // �ΏۂƂ���t�@�C�����폜
            LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �t�@�C���폜�֐��Ăяo�� *****");
            iDeleteResult = g_pclsMainFileOpe->DeleteFileProc(g_stFileNameInfo.strDeleteName.c_str());
            g_clsMainLogFile.m_stLogData = g_pclsMainSubOpe->CreateEventMessageProc(
                ACTIONSET,
                g_stFileNameInfo.strDeleteName.c_str());
            sprintf_s(g_clsMainLogFile.m_cLogData, sizeof(g_clsMainLogFile.m_cLogData), "���s���ʁF%d", iDeleteResult);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);

            // �G���[���������Ă��Ȃ����`�F�b�N
            if (CM_FILEDELETEERROR == iDeleteResult)
            {
                // �t�@�C���폜���s
                // �G���[�R�[�h��ݒ肵�ė�O�����ֈړ�
                iEndResult = CM_FILEDELETEERROR;
                g_clsMainLogFile.m_eErrorResultCode = tagErrorCode::DELETEERROR;
                g_clsMainLogFile.m_maErrorActionName.insert(
                    make_pair(
                        iEndResult, 
                        g_clsMainLogFile.m_stLogData.pcActionName));
                LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** �t�@�C���폜�֐��Ăяo���ُ�I�� *****");
                throw iEndResult;
            }
            else
            {
                // �t�@�C���폜����
                // �t�@�C���폜�����̃��b�Z�[�W�����O�ɏo��
                LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, "***** �t�@�C���폜�֐��Ăяo������I�� *****");
                sprintf_s(
                    g_clsMainLogFile.m_cLogData,
                    sizeof(g_clsMainLogFile.m_cLogData),
                    "%s ��%s",
                    g_clsMainLogFile.m_stLogData.pcActionName,
                    DELETESUCCESS);
                LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, g_clsMainLogFile.m_cLogData);
                sprintf_s(g_clsMainLogFile.m_cLogData, sizeof(g_clsMainLogFile.m_cLogData), "�߂�l�́F%d", iDeleteResult);
                LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
            }
        }
    }

    // ��O����
    catch (const int iExecErr)
    {
        // �G���[���b�Z�[�W��������
        // �G���[���b�Z�[�W�����O�ɏo��
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ���ގ��󋵈󎚐���I�������ُ�I�� *****");
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �G���[���b�Z�[�W�����֐��Ăяo�� *****");
        g_clsMainLogFile.m_stLogData = g_pclsMainSubOpe->CreateEventMessageProc(
            ERRORSET,
            g_clsMainLogFile.m_stLogData.pcActionName,
            g_clsMainLogFile.m_eErrorResultCode);
        LOGOUTPUTERROR(
            TRACEOUTPUTLOG,
            ERRORINFORMATION1,
            g_clsMainLogFile.m_stLogData.pcMessage,
            g_clsMainLogFile.m_stLogData.eErrorCode);
        sprintf_s(g_clsMainLogFile.m_cLogData, sizeof(g_clsMainLogFile.m_cLogData), "�߂�l�́F%d", iExecErr);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, g_clsMainLogFile.m_cLogData);
    }

    // �㏈��
    // �������擾����
    // ���ގ��󋵈󎚐���I�������I�������擾
    LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, "***** ���ގ��󋵈󎚐���I����������I�� *****");
    g_clsMainLogFile.m_stTimeData = g_clsMainLogFile.GetDateTimeProc();
    sprintf_s(
        g_clsMainLogFile.m_cLogData, 
        sizeof(g_clsMainLogFile.m_cLogData), 
        "���ގ��󋵈󎚐���I�������I�������F%s %s", 
        (char *)g_clsMainLogFile.m_stTimeData.strMachDatePlus.c_str(), 
        (char *)g_clsMainLogFile.m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, g_clsMainLogFile.m_cLogData);
    return iEndResult;
}

//********************************************
// �֐����FDLL �A�v���P�[�V�����G���g���|�C���g����
// Function Name�FDllMain
// 
// �����T�v�F
// �A�v���P�[�V��������ŏ��ƍŌ�ɌĂяo�����
// ���ގ��󋵈󎚎��s�ɑ΂��ď����E��Еt�����s��
// 
// �����菇�F
// �i�v���Z�X�A�^�b�`���j
//   �P�D�R�}���h���C�������擾����
//   �Q�D�R�}���h���C����͏���
// �i�v���Z�X�f�^�b�`���j
//   �R�D�R�}���h���C�������폜
//   �S�D�e�N���X�폜
// 
// �����F      DLL���W���[���n���h��
//             �֐��Ăяo�����e
//             �\��ςݒ�`
// �߂�l�F    ����I���FTRUE
// 
// �⑫����
// �E���@�\�̓v���Z�X�A�^�b�`���y�уv���Z�X�f�^�b�`���̂ݎ��s����
// 
// �쐬����
// 2020/9/25  s-saito   �V�K�쐬
// 
// �X�V����
// 
// 
//********************************************
BOOL APIENTRY DllMain(
    HMODULE hModule,
    DWORD ul_reason_for_call,
    LPVOID lpReserved)
{
    // �ϐ��錾
    string strCommandLine;

    // ���������[�N���o�ݒ�
    // �i_CrtSetDbgFlag��C++�����`�j
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    // CRT���������m�C�x���g�n���h���ݒ�
    // �������ɂ� _CRT_RPTHOOK_INSTALL�i�n���h���ǉ��j���w��
    // �������ɂ̓C�x���g�n���h�����w��
    _CrtSetReportHook2(_CRT_RPTHOOK_INSTALL, MemoryLeakDookProc);

    // �G���g���[�|�C���g�Ɋ�Â��ď����𕪊�
    switch (ul_reason_for_call)
    {
        // �v���Z�X�A�^�b�`��
        case DLL_PROCESS_ATTACH:
        {
            // PC�{����ʃ^�C�g���ݒ�
            // �iSetConsoleTitle��C++�����`�j
            SetConsoleTitle(TEXT("���ގ��󋵈󎚎��s���"));

            // �R�}���h���C�������擾
            // �iGetCommandLineA��C++�����`�j
            strCommandLine = GetCommandLineA();

            // �R�}���h���C�����������
            g_vecParseArgc = g_clsMainSubOpe.ParseArgsProc(strCommandLine, &g_iParseArgv);
            if (0 == g_vecParseArgc.size())
            {
                // ��͂Ɏ��s
                // �f�o�b�O�E�B���h�E�ɏo��
                // �iOutputDebugStringA��C++�����`�j
                OutputDebugStringA("�R�}���h��񂪉�͏o���܂���B\n");
                OutputDebugStringA("\n");

#ifdef SCREEN
                // PC�{����ʂɏo��
                printf_s("�R�}���h��񂪉�͏o���܂���B\n");

                // PC�{����ʂ̎����N���[�Y��h�����߈ꎞ��~
                system("pause");
#endif // SCREEN

                // �A�v���P�[�V�����I��
                exit(EXIT_FAILURE);
            }
            else
            {
                // ��͂ɐ���
                // �t�@�C���o�̓^�C�v���L�[�Ƃ����t�@�C�����e�[�u������
                g_maFileName.insert(make_pair(tagFileType::FILETYPE1, "���ގ��ǂݍ��݃f�[�^�t�@�C��"));
                g_maFileName.insert(make_pair(tagFileType::FILETYPE2, "���ގ��������݃f�[�^�t�@�C��"));
                g_maFileName.insert(make_pair(tagFileType::FILETYPE3, "�g���[�X���O�o�̓t�@�C��"));
                g_maFileName.insert(make_pair(tagFileType::FILETYPE4, "�f�[�^���O�o�̓t�@�C��"));
                g_maFileName.insert(make_pair(tagFileType::FILETYPE5, "�o�b�N�A�b�v�t�@�C��"));
                g_maFileName.insert(make_pair(tagFileType::FILETYPE6, "���̕ύX�Ώۃt�@�C��"));
                g_maFileName.insert(make_pair(tagFileType::FILETYPE7, "�폜�Ώۃt�@�C��"));
                g_maFileName.insert(make_pair(tagFileType::FILETYPE8, "���s�A�v���P�[�V����"));
                g_maFileName.insert(make_pair(tagFileType::FILETYPE9, "�s���f�[�^�t�@�C��"));

                // ���򔲂�
                break;
            }
        }

        // �X���b�h�A�^�b�`��
        case DLL_THREAD_ATTACH:
        {
            // ���򔲂�
            break;
        }

        // �X���b�h�f�^�b�`��
        case DLL_THREAD_DETACH:
        {
            // ���򔲂�
            break;
        }

        // �v���Z�X�f�^�b�`��
        case DLL_PROCESS_DETACH:
        {
            // �R�}���h���C�������폜
            strCommandLine.clear();
            g_iParseArgv = 0;
            g_vecParseArgc.clear();

            // �}�b�v�e�[�u�������폜
            g_maFileName.clear();
            g_clsMainLogFile.m_maErrorActionName.clear();

            // �e�N���X�����o�ϐ��폜
            // ���R�[�h����N���X
            SAFE_FREE_NEWALLOC(g_pclsMainRecOpe);

            // �t�@�C������N���X
            SAFE_FREE_NEWALLOC(g_pclsMainFileOpe);

            // ���ʑ���N���X
            SAFE_FREE_NEWALLOC(g_pclsMainSubOpe);

            // CRT���������m�C�x���g�n���h���ݒ�
            // �������ɂ� _CRT_RPTHOOK_REMOVE�i�n���h���폜�j���w��
            // �������ɂ̓C�x���g�n���h�����w��
            _CrtSetReportHook2(_CRT_RPTHOOK_REMOVE, MemoryLeakDookProc);

            // ���򔲂�
            break;
        }

        // ��L�ȊO
        default:
        {
            // ���򔲂�
            break;
        }
    }

    // ����I��
    return TRUE;
}

//********************************************
// �֐����FCRT���������m�C�x���g�n���h������
// Function Name�FMemoryLeakDookProc
// 
// �����T�v�F
// ���������[�N���m���̏������t�b�N����C�x���g�n���h����o�^����
// 
// �����菇�F
//   �P�D�����^�C�v�𔻕�
//   �Q�D�g���[�X���O�o��
//   �R�D���o���o��
// 
// �����F      �����^�C�v
//             CRT����̏o�̓��b�Z�[�W
// �߂�l�F    CRT���������m�C�x���g�n���h���g�p�L��
//             TRUE�F_CrtSetReportHook���Ăяo���Ȃ�
//             FALSE�F_CrtSetReportHook���Ăяo��
// 
// �⑫����
// �E�g���[�X���O�ւ̏o�͂̓��������[�N�̏ꍇ�݂̂Ƃ���
// 
// �쐬����
// 2020/9/25  s-saito   �V�K�쐬
// 
// �X�V����
// 
// 
//********************************************
int __cdecl MemoryLeakDookProc(
    int iReportType,
    char *pcLeakMessage,
    int *piReturnValue)
{
    // �����^�C�v�ɉ����ď����𕪊�
    switch (iReportType)
    {
        case _CRT_ASSERT:
        {
            // _CrtSetReportHook���Ăяo���Ȃ��ŏI��
            return TRUE;
        }

        case _CRT_WARN:
        {
            // ���������[�N
            // �f�o�b�O�E�B���h�E�ɏo��
            // �iOutputDebugStringA��C++�����`�j
            OutputDebugStringA(pcLeakMessage);
            break;
        }

        case _CRT_ERROR:
        {
            // assert�����Ă΂ꂽ�ꍇ
            // _CrtSetReportHook���Ăяo���Ȃ��ŏI��
            return TRUE;
        }

        default:
        {
            // ��L�ȊO
            // _CrtSetReportHook���Ăяo���Ȃ��ŏI��
            return TRUE;
        }
    }

    // FALSE ��ԋp���鎖�ɂ�� _CrtSetReportHook �œo�^�����C�x���g�n���h���Ăяo��
    return FALSE;
}