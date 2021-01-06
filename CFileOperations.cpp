//============================================
// �t�@�C������N���X�\�[�X�t�@�C��
// CFileOperations.cpp
// 
// �@�\�T�v
// �t�@�C�����쓙���T�|�[�g����@�\���`����
// 
// �@�\���e
// �P�D�t�@�C���R�s�[����
// �Q�D�t�@�C�����ϊ�����
// �R�D�t�@�C���폜����
// �S�D�t�@�C����ԃ`�F�b�N����
// �T�D��΃p�X�ϊ��t�@�C�����擾����
// �U�D�t�@�C������������
// �V�D��΃p�X�擾����
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
#include "CFileOperations.h"

// �O���[�o���ϐ��錾
CLogFile g_clsFileOpeLogFile;

// �}�N����`
#define LOGOUTPUT(iOutputType, iTraceOutputLogType, cMessageInfo) \
    g_clsFileOpeLogFile.LogOutputControlProc(LOGARGS, iOutputType, iTraceOutputLogType, cMessageInfo)
#define LOGOUTPUTERROR(iOutputType, iTraceOutputLogType, cErrorMessageInfo, eErrorInfoCode) \
    g_clsFileOpeLogFile.LogOutputControlProc(LOGARGS, iOutputType, iTraceOutputLogType, cErrorMessageInfo, eErrorInfoCode)

//��������������������������������������������
// �e�@�\�iAPI�j��`
//��������������������������������������������
//********************************************
// �֐����F�f�t�H���g�R���X�g���N�^
// Function Name�FCFileOperations
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
CFileOperations::CFileOperations()
{
    // ���݃f�t�H���g�R���X�g���N�^�ł̏����͂Ȃ�
}

//********************************************
// �֐����F�p�����[�^���t���R���X�g���N�^
// Function Name�FCFileOperations
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
CFileOperations::CFileOperations(
    const string strTraceLogFileName, 
    const string strDataLogFileName, 
    const string strCopyFileName)
{
    // ���O�o�̓N���X����
    g_clsFileOpeLogFile = CLogFile(strTraceLogFileName, strDataLogFileName);

    // �Ăяo�����Ŏw�肳�ꂽ�t�@�C�����ݒ�
    m_strFileOpeTraceLogFileName = strTraceLogFileName;
    m_strFileOpeDataLogFileName = strDataLogFileName;
    m_strFileOpeCopyFileName = strCopyFileName;
    m_strSubOpeTraceLogFileName = strTraceLogFileName;
    m_strSubOpeDataLogFileName = strDataLogFileName;
}

//********************************************
// �֐����F�f�X�g���N�^
// Function Name�F~CFileOperations
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
CFileOperations::~CFileOperations()
{
    // �����o�ϐ��N���A�����o�ϐ��N���A
    m_strFileOpeTraceLogFileName.clear();
    m_strFileOpeDataLogFileName.clear();
    m_strFileOpeCopyFileName.clear();
    m_strSubOpeTraceLogFileName.clear();
    m_strSubOpeDataLogFileName.clear();
}

//********************************************
// �֐����F�t�@�C���R�s�[����
// Function Name�FCopyFileProc
// 
// �����T�v�F
// �R�s�[���Ƃ���t�@�C�����
// �R�s�[��Ƃ���t�@�C���փ��R�[�h���R�s�[����
// 
// �����菇�F
//   �P�D�R�s�[���t�@�C���I�[�v��
//   �Q�D�R�s�[��t�@�C���I�[�v��
// �i�������疳�����[�v�j
//   �R�D�t�@�C���ǂݍ���
//   �S�D�t�@�C����������
// �i�����܂Ŗ������[�v�j
//   �T�D�R�s�[��t�@�C���N���[�Y
//   �U�D�R�s�[���t�@�C���N���[�Y
// 
// �����F      �R�s�[���t�@�C����
//             �R�s�[��t�@�C����
// �߂�l�F    ���s���ʁi����I����stdlib.h�Œ�`���Ă�����e�𗘗p�j
//             ����I���FEXIT_SUCCESS�i0�j
//             �ُ�I���F�t�@�C���I�[�v���G���[�FCM_FILEOPENERROR�i104�j
//                       �t�@�C���N���[�Y�G���[�FCM_FILECLOSEERROR�i105�j
//                       �t�@�C���R�s�[�G���[�FCM_FILECOPYERROR�i110�j
// 
// �⑫����
// �E�������[�v���g�p����ꍇ�͕K�����[�v�𔲂������������
// 
// 
// �쐬����
// 2020/9/25  s-saito   �V�K�쐬
// 
// �X�V����
// 
// 
//********************************************
int CFileOperations::CopyFileProc(
	const char *pcBaseFileName, 
	const char *pcDestFileName)
{
    // �ϐ��錾
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

    // ������
    memset(cCopyData, 0x00, sizeof(cCopyData));
    memset(cCopyDataLogout, 0x00, sizeof(cCopyDataLogout));
    memset(cBaseFileName, 0x00, sizeof(cBaseFileName));
    memset(cDestFileName, 0x00, sizeof(cDestFileName));

    // �O����
    // �������擾����
    // �t�@�C���R�s�[�J�n�����擾
    m_stTimeData = g_clsFileOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "�t�@�C���R�s�[�J�n�����F%s %s", 
        (char *)m_stTimeData.strMachDatePlus.c_str(), 
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �t�@�C���R�s�[�����J�n *****");

    // �p�����[�^�����R�s�[
    strncpy_s(cBaseFileName, sizeof(cBaseFileName), pcBaseFileName, sizeof(cBaseFileName) - 1);
    sprintf_s(m_cLogData, sizeof(m_cLogData), "�R�s�[���t�@�C�����F%s", cBaseFileName);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
    strncpy_s(cDestFileName, sizeof(cDestFileName), pcDestFileName, sizeof(cDestFileName) - 1);
    sprintf_s(m_cLogData, sizeof(m_cLogData), "�R�s�[��t�@�C�����F%s", cDestFileName);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    // �R�s�[���t�@�C�����I�[�v��
    eFileErrNo = fopen_s(&pfBaseFile, cBaseFileName, "rb");
    sprintf_s(m_cLogData, sizeof(m_cLogData), "�t�@�C���I�[�v�����ʁF%d", eFileErrNo);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
    m_stLogData = CreateEventMessageProc(ACTIONSET, cBaseFileName);

    // �I�[�v�����ʂ��`�F�b�N
    if (0 != eFileErrNo)
    {
        // �t�@�C���I�[�v�����s
        // �G���[�t���O��ON
        iErrFlag = ERR_ON;

        // �G���[�R�[�h��ݒ肵�Č㏈���ֈړ�
        LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** �t�@�C���R�s�[�����ُ�I�� *****");
        iCopyResult = CM_FILEOPENERROR;
        goto END;
    }
    else
    {
        // �t�@�C���I�[�v������
        // �t�@�C���I�[�v�������̃��b�Z�[�W�����O�ɏo��
        sprintf_s(m_cLogData, sizeof(m_cLogData), "%s %s", m_stLogData.pcActionName, FILEOPENSUCCESS);
        LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, m_cLogData);
    }

    // �R�s�[��t�@�C�����I�[�v��
    eFileErrNo = fopen_s(&pfDestFile, cDestFileName, "wb");
    sprintf_s(m_cLogData, sizeof(m_cLogData), "�t�@�C���I�[�v�����ʁF%d", eFileErrNo);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
    m_stLogData = CreateEventMessageProc(ACTIONSET, cDestFileName);

    // �I�[�v�����ʂ��`�F�b�N
    if (0 != eFileErrNo)
    {
        // �t�@�C���I�[�v�����s
        // �G���[�t���O��ON
        iErrFlag = ERR_ON;

        // �G���[�R�[�h��ݒ肵�Č㏈���ֈړ�
        LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** �t�@�C���R�s�[�����ُ�I�� *****");
        iCopyResult = CM_FILEOPENERROR;
        goto END;
    }
    else
    {
        // �t�@�C���I�[�v������
        // �t�@�C���I�[�v�������̃��b�Z�[�W�����O�ɏo��
        sprintf_s(m_cLogData, sizeof(m_cLogData), "%s %s", m_stLogData.pcActionName, FILEOPENSUCCESS);
        LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, m_cLogData);
    }

    // �又��
    // �������[�v���s
    // ���ӁI�I
    // �K�����[�v�I�����������邱�ƁI�I
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �t�@�C���ǂݏ����J�n *****");
    for (; ;)
    {
        // �t�@�C������1�s�ǂݍ���
        pcReadResult = fgets(cCopyData, DATASIZE, pfBaseFile);
        m_stLogData = CreateEventMessageProc(ACTIONSET, cBaseFileName);

        // ���s�R�[�h���폜���ă��O�ɏo��
        DeleteDesignationCharProc(cCopyData, (const char)CRLF_CODE, cCopyDataLogout);
        sprintf_s(m_cLogData, sizeof(m_cLogData), "�ǂݍ��݌��ʁF%s", cCopyDataLogout);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

        // �ǂݍ��݌��ʂ��`�F�b�N
        if (NULL == pcReadResult)
        {
            // �t�@�C���̏I���H
            iReadResult = feof(pfBaseFile);
            if (EXIT_SUCCESS != iReadResult)
            {
                // �t�@�C���̏I���
                // �S�Ẵf�[�^���I��
                LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, "***** �t�@�C���ǂݏ����I�� *****");
                LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, "***** �t�@�C���R�s�[��������I�� *****");

                // �����I��
                LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, ALLREADEND);
                break;
            }
            else
            {
                // �t�@�C���̏I���ł͂Ȃ�
                LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "�ǂݍ��ݎ��s m(�B���D���B)m");
                LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** �t�@�C���R�s�[�����ُ�I�� *****");
                sprintf_s(m_cLogData, sizeof(m_cLogData), "�t�@�C�����́F %s", m_stLogData.pcActionName);
                LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, m_cLogData);

                // �G���[�R�[�h��ݒ�
                iCopyResult = CM_FILECOPYERROR;
                goto END;
            }
        }
        else
        {
            // �t�@�C���ǂݍ��ݐ���
            // �t�@�C���ǂݍ��ݐ����̃��b�Z�[�W�����O�ɏo��
            LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, "�ǂݍ��ݐ��� ( ^-^)�m�ځ��B.:*:�E'����");
            sprintf_s(m_cLogData, sizeof(m_cLogData), "%s %s", m_stLogData.pcActionName, FILEREADSUCCESS);
            LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, m_cLogData);
            sprintf_s(m_cLogData, sizeof(m_cLogData), "�ǂݍ��݌��ʃR�[�h�F%d", iCopyResult);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
        }

        // �ǂݍ���1�s����������
        iWriteResult = fputs(cCopyData, pfDestFile);
        sprintf_s(m_cLogData, sizeof(m_cLogData), "�������݃f�[�^�F%s ���ʃR�[�h�F%d", cCopyDataLogout, iWriteResult);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
        m_stLogData = CreateEventMessageProc(ACTIONSET, cDestFileName);

        // �������݌��ʂ��`�F�b�N
        if (EOF == iWriteResult)
        {
            // �������ݎ��s
            LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "�������ݎ��s ( *�L�E��)/(�G�t�G )");
            LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** �t�@�C���R�s�[�����ُ�I�� *****");
            sprintf_s(m_cLogData, sizeof(m_cLogData), "�t�@�C�����́F %s", m_stLogData.pcActionName);
            LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, m_cLogData);

            // �G���[�R�[�h��ݒ�
            iCopyResult = CM_FILECOPYERROR;
            goto END;
        }
        else
        {
            // �t�@�C���������ݐ���
            // �t�@�C���������ݐ����̃��b�Z�[�W�����O�ɏo��
            LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, "�������ݐ��� ( ^-^)�m�ځ��B.:*:�E'����");
            sprintf_s(m_cLogData, sizeof(m_cLogData), "%s %s", m_stLogData.pcActionName, FILEWRITESUCCESS);
            LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, m_cLogData);
            sprintf_s(m_cLogData, sizeof(m_cLogData), "�������݌��ʃR�[�h�F%d", iWriteResult);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
        }
    }

END:
    // �㏈��
    // �t�@�C�����I�[�v�����Ă��邩�`�F�b�N
    // �R�s�[��t�@�C��
    if (NULL != pfDestFile)
    {
        // �t�@�C�����I�[�v�����Ă���
        // �t�@�C�����N���[�Y
        iCloseResult = fclose(pfDestFile);
        m_stLogData = CreateEventMessageProc(ACTIONSET, "�R�s�[��");

        // �N���[�Y�o�������`�F�b�N
        if (EXIT_SUCCESS != iCloseResult)
        {
            // �N���[�Y���s
            // �G���[�R�[�h��ݒ�
            LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** �t�@�C���R�s�[�����ُ�I�� *****");
            iCopyResult = CM_FILECLOSEERROR;
        }
        else
        {
            // �t�@�C���N���[�Y����
            // �t�@�C���N���[�Y�����̃��b�Z�[�W�����O�ɏo��
            sprintf_s(m_cLogData, sizeof(m_cLogData), "%s%s", m_stLogData.pcActionName, FILECLOSESUCCESS);
            LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, m_cLogData);
        }
    }

    // �R�s�[���t�@�C��
    if (NULL != pfBaseFile)
    {
        // �t�@�C�����I�[�v�����Ă���
        // �t�@�C�����N���[�Y
        iCloseResult = fclose(pfBaseFile);
        m_stLogData = CreateEventMessageProc(ACTIONSET, "�R�s�[��");

        // �N���[�Y�o�������`�F�b�N
        if (EXIT_SUCCESS != iCloseResult)
        {
            // �N���[�Y���s
            // �G���[�R�[�h��ݒ�
            LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** �t�@�C���R�s�[�����ُ�I�� *****");
            iCopyResult = CM_FILECLOSEERROR;
        }
        else
        {
            // �t�@�C���N���[�Y����
            // �t�@�C���N���[�Y�����̃��b�Z�[�W�����O�ɏo��
            sprintf_s(m_cLogData, sizeof(m_cLogData), "%s%s", m_stLogData.pcActionName, FILECLOSESUCCESS);
            LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, m_cLogData);
        }
    }

    // �������擾����
    // �t�@�C���R�s�[�I�������擾
    m_stTimeData = g_clsFileOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "�t�@�C���R�s�[�I�������F%s %s", 
        (char *)m_stTimeData.strMachDatePlus.c_str(), 
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);

    // �t�@�C���R�s�[���ʂ�ԋp
    return iCopyResult;
}

//********************************************
// �֐����F�t�@�C�����ϊ�����
// Function Name�FChangeFileNameProc
// 
// �����T�v�F
// �t�@�C�����ɑ΂��ăt�@�C�����{�����`���ɕϊ�����
// 
// �����菇�F
//   �P�D�t�@�C�������l�[��
// 
// �����F     �ϊ��O�t�@�C����
//            �ϊ���t�@�C����
// �߂�l�F   ���s���ʁi����I����stdlib.h�Œ�`���Ă�����e�𗘗p�j
//            ����I���FEXIT_SUCCESS�i0�j
//            �ُ�I���F�t�@�C�����ϊ��G���[�FCM_RENAMEERROR�i111�j
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
int CFileOperations::ChangeFileNameProc(
	const char *pcBaseFileName, 
	const char *pcFileCustomName)
{
    // �ϐ��錾
    int iFileConvResult = EXIT_SUCCESS;
    int iRenameResult = EXIT_SUCCESS;
    char cFileCustomName[FILENAMESIZE + 1];
    char cBaseFileName[FILENAMESIZE + 1];

    // ������
    memset(cFileCustomName, 0x00, sizeof(cFileCustomName));
    memset(cBaseFileName, 0x00, sizeof(cBaseFileName));

    // �O����
    // �������擾����
    // �t�@�C�����ϊ��J�n�����擾
    m_stTimeData = g_clsFileOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "�t�@�C�����ϊ��J�n�����F%s %s", 
        (char *)m_stTimeData.strMachDatePlus.c_str(), 
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �t�@�C�����ϊ������J�n *****");

    // �p�����[�^�����R�s�[
    strncpy_s(cBaseFileName, sizeof(cBaseFileName), pcBaseFileName, sizeof(cBaseFileName) - 1);
    sprintf_s(m_cLogData, sizeof(m_cLogData), "�ϊ��O�t�@�C�����F%s", cBaseFileName);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
    strncpy_s(cFileCustomName, sizeof(cFileCustomName), pcFileCustomName, sizeof(cFileCustomName) - 1);
    sprintf_s(m_cLogData, sizeof(m_cLogData), "�ϊ���t�@�C�����F%s", cFileCustomName);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    // �又��
    // �t�@�C���������l�[��
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �t�@�C�����ϊ��J�n *****");
    iRenameResult = rename(cBaseFileName, cFileCustomName);
    sprintf_s(m_cLogData, sizeof(m_cLogData), "���l�[���O�t�@�C�����F%s", cBaseFileName);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
    sprintf_s(m_cLogData, sizeof(m_cLogData), "���l�[����t�@�C�����F%s", cFileCustomName);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
    sprintf_s(m_cLogData, sizeof(m_cLogData), "���s���ʁF%d", iRenameResult);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
    m_stLogData = CreateEventMessageProc(ACTIONSET, cFileCustomName);

    // ���l�[���o�������`�F�b�N
    if (EXIT_SUCCESS != iRenameResult)
    {
        // ���l�[�����s
        // �G���[�R�[�h��ݒ�
        LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** �t�@�C�����ϊ��ُ�I�� *****");
        iFileConvResult = CM_RENAMEERROR;
        sprintf_s(m_cLogData, sizeof(m_cLogData), "�t�@�C�����́F %s", m_stLogData.pcActionName);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
    }
    else
    {
        // ���l�[������
        // ���l�[�������̃��b�Z�[�W�����O�ɏo��
        LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, "***** �t�@�C�����ϊ�����I�� *****");
        sprintf_s(m_cLogData, sizeof(m_cLogData), "�t�@�C�����́F %s", m_stLogData.pcActionName);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
        sprintf_s(m_cLogData, sizeof(m_cLogData), "%s ��%s", m_stLogData.pcActionName, RENAMESUCCESS);
        LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, m_cLogData);
    }

    // �㏈��
    // �������擾����
    // �t�@�C�����ϊ��I�������擾
    m_stTimeData = g_clsFileOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "�t�@�C�����ϊ��I�������F%s %s", 
        (char *)m_stTimeData.strMachDatePlus.c_str(), 
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);

    // ���s���ʂ�ԋp
    return iFileConvResult;
}

//********************************************
// �֐����F�t�@�C���폜����
// Function Name�FDeleteFileProc
// 
// �����T�v�F
// �t�@�C�����Ɋ�Â���
// �w�肵���t�@�C���𕨗��폜����
// 
// �����菇�F
//   �P�D�t�@�C���폜
// 
// �����F      �폜�Ώۃt�@�C����
// �߂�l�F    ���s���ʁi����I����stdlib.h�Œ�`���Ă�����e�𗘗p�j
//             ����I���FEXIT_SUCCESS�i0�j
//             �ُ�I���F�t�@�C���폜�G���[�FCM_FILEDELETEERROR�i112�j
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
int CFileOperations::DeleteFileProc(
	const char *pcDeleteFile)
{
    // �ϐ��錾
    int iFileDeleteResult = 0;
    int iDeleteResult = EXIT_SUCCESS;
    char cDeleteFileName[FILENAMESIZE + 1];

    // ������
    memset(cDeleteFileName, 0x00, sizeof(cDeleteFileName));

    // �O����
    // �������擾����
    // �t�@�C���폜�J�n�����擾
    m_stTimeData = g_clsFileOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "�t�@�C���폜�J�n�����F%s %s", 
        (char *)m_stTimeData.strMachDatePlus.c_str(), 
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �t�@�C���폜�����J�n *****");

    // �p�����[�^�����R�s�[
    strncpy_s(cDeleteFileName, sizeof(cDeleteFileName), pcDeleteFile, sizeof(cDeleteFileName) - 1);
    sprintf_s(m_cLogData, sizeof(m_cLogData), "�폜�t�@�C�����F%s", cDeleteFileName);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    // �又��
    // �t�@�C�����폜
    iFileDeleteResult = remove(cDeleteFileName);
    m_stLogData = CreateEventMessageProc(ACTIONSET, cDeleteFileName);
    sprintf_s(m_cLogData, sizeof(m_cLogData), "�t�@�C���폜���ʁF%d", iFileDeleteResult);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    // �폜�o�������`�F�b�N
    if (EXIT_SUCCESS == iFileDeleteResult)
    {
        // �폜����
        // ���b�Z�[�W�����O�ɏo��
        LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, "***** �t�@�C���폜��������I�� *****");
        sprintf_s(m_cLogData, sizeof(m_cLogData), "�t�@�C�����́F%s", m_stLogData.pcActionName);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
        sprintf_s(m_cLogData, sizeof(m_cLogData), "%s ��%s", m_stLogData.pcActionName, DELETESUCCESS);
        LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, m_cLogData);
    }
    else
    {
        // �폜���s
        // �G���[�R�[�h��ݒ�
        LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** �t�@�C���폜�����ُ�I�� *****");
        iDeleteResult = CM_FILEDELETEERROR;
        sprintf_s(m_cLogData, sizeof(m_cLogData), "�t�@�C�����́F%s", m_stLogData.pcActionName);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
    }

    // �㏈��
    // �������擾����
    // �t�@�C���폜�I�������擾
    m_stTimeData = g_clsFileOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "�t�@�C���폜�I�������F%s %s", 
        (char *)m_stTimeData.strMachDatePlus.c_str(), 
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);

    // �t�@�C���폜���ʂ�ԋp
    return iDeleteResult;
}

//********************************************
// �֐����F�t�@�C����ԃ`�F�b�N����
// Function Name�FCheckFileStatProc
// 
// �����T�v�F
// �w�肳�ꂽ�t�@�C���ɑ΂��ď�Ԃ��`�F�b�N����
// 
// �����菇�F
//   �P�D�p�����[�^���R�s�[
//   �Q�D�t�@�C����Ԏ擾
// 
// �����F      ��ԃ`�F�b�N�Ώۃt�@�C����
//             �`�F�b�NSTAT�\����
// �߂�l�F    ���s���ʁi����I����stdlib.h�Œ�`���Ă�����e�𗘗p�j
//             ����I���FEXIT_SUCCESS�i0�j
//             �ُ�I���F�t�@�C����Ԏ擾�G���[�FSTATERROR�i113�j
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
int CFileOperations::CheckFileStatProc(
	const char *pcCheckFileName, 
	struct stat *pstStatResult)
{
    // �ϐ��錾
    int iStatResult = EXIT_SUCCESS;
    int iStatCheckResult = EXIT_SUCCESS;
    char cCheckFileName[FILENAMESIZE + 1];

    // ������
    memset(cCheckFileName, 0x00, sizeof(cCheckFileName));

    // �O����
    // �������擾����
    // �t�@�C����ԃ`�F�b�N�J�n�����擾
    m_stTimeData = g_clsFileOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "�t�@�C����ԃ`�F�b�N�J�n�����F%s %s", 
        (char *)m_stTimeData.strMachDatePlus.c_str(), 
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �t�@�C����ԃ`�F�b�N�����J�n *****");

    // �p�����[�^�����R�s�[
    strncpy_s(cCheckFileName, sizeof(cCheckFileName), pcCheckFileName, sizeof(cCheckFileName) - 1);
    sprintf_s(m_cLogData, sizeof(m_cLogData), "��ԃ`�F�b�N�t�@�C�����F%s", cCheckFileName);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    // �又��
    // �t�@�C����Ԃ��擾�istat�֐��g�p�j
    iStatCheckResult = stat(cCheckFileName, pstStatResult);
    sprintf_s(m_cLogData, sizeof(m_cLogData), "�t�@�C����Ԏ擾���ʁF%d", iStatCheckResult);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    // �㏈��
    // �擾���ʂ��`�F�b�N
    if (0 != iStatCheckResult)
    {
        // �t�@�C����Ԏ擾���s
        // �G���[�R�[�h��ݒ�
        LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** �t�@�C����ԃ`�F�b�N�����ُ�I�� *****");
        iStatResult = CM_FILESTATERROR;
    }
    else
    {
        // �t�@�C����Ԏ擾����
        // �t�@�C����Ԏ擾�����̃��b�Z�[�W�����O�ɏo��
        m_stLogData = CreateEventMessageProc(ACTIONSET, cCheckFileName);
        LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, "***** �t�@�C����ԃ`�F�b�N��������I�� *****");
        sprintf_s(m_cLogData, sizeof(m_cLogData), "%s %s", m_stLogData.pcActionName, GETSTATSUCCESS);
        LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, m_cLogData);
    }

    // �������擾����
    // �t�@�C����ԃ`�F�b�N�I�������擾
    m_stTimeData = g_clsFileOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "�t�@�C����ԃ`�F�b�N�I�������F%s %s", 
        (char *)m_stTimeData.strMachDatePlus.c_str(), 
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);

    // �t�@�C����ԃ`�F�b�N���ʂ�ԋp
    return iStatResult;
}

//********************************************
// �֐����F��΃p�X�ϊ��t�@�C�����擾����
// Function Name�FGetConvertFileNameProc
// 
// �����T�v�F
// �ȉ��̃p�����[�^���Ɋ�Â���
// ��΃p�X�t���̃t�@�C�������t�@�C�������Ǘ��\���̂ɂĎ擾����
//      �E�ϊ��Ώۃt�@�C���^�C�v
//      �E�ϊ��Ώۃt�@�C����
//      �E�Ăяo�������s���ʁi�R�}���h���j
// 
// �����菇�F
//   �P�D�x�[�X�Ƃ���A�v���P�[�V�����̐�΃p�X����
//   �Q�D�p�X���ҏW�������̈���
//   �R�D�ϊ��Ώۃt�@�C�����^�C�v�Ɋ�Â��Đ�΃p�X����
//   �S�D�p�X���ҏW�������̈���
//   �T�D�i�[�f�B���N�g������
//   �U�D��΃p�X�t�^�ɂ���΃p�X�t���t�@�C��������
//   �V�D�t�@�C�����ҏW�������̈���
//   �W�D��΃p�X�t���̃t�@�C�����i�[
// 
// �����F     �ϊ��Ώۃt�@�C�����^�C�v
//            �ϊ��Ώۃt�@�C����
//            �R�}���h���i���s�A�v���P�[�V�������j
//            �p�X�g�ݍ��݃f�B���N�g����
//            �f�t�H���g�t�@�C����
//            �Ăяo�������s���ʁi�ݒ肪�Ȃ��ꍇ��0�j
// 
// �߂�l�F   ��΃p�X�t���t�@�C����
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
string CFileOperations::GetConvertFileNameProc(
    FILETYPETABLE eConvertKind,
    const char *pcDesignationName,
    const char *pcCmdFileName,
    char *pcDirectoryName, 
    char *pcDefaultName, 
    int iBaseResult)
{
    // �ϐ��錾
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

    // ������
    memset(cEditPath, 0x00, sizeof(cEditPath));

    // �Ăяo���񐔂��`�F�b�N
    if (2 == iConvertCnt)
    {
        // ����3��ڂ̏ꍇ
        // �g���[�X���O�y�уf�[�^���O�ŌĂяo�����|�����O�ɏo��
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "�ŏ���2��̓g���[�X���O�y�уf�[�^���O�ŌĂяo���ς�");
    }

    // �Ăяo���񐔂��J�E���g
    iConvertCnt++;
    sprintf_s(m_cLogData, sizeof(m_cLogData), "��΃p�X�ϊ��t�@�C�����擾�Ăяo���񐔁F %d ��", iConvertCnt);
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);

    // �O����
    // �������擾����
    // �p�X�ϊ��J�n�����擾
    m_stTimeData = g_clsFileOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData,
        sizeof(m_cLogData),
        "��΃p�X�ϊ��t�@�C�����擾�J�n�����F%s %s",
        (char *)m_stTimeData.strMachDatePlus.c_str(),
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ��΃p�X�ϊ��t�@�C�����擾�����J�n *****");

    // �p�����[�^�����R�s�[
    strDesignationFileName = pcDesignationName;
    sprintf_s(m_cLogData, sizeof(m_cLogData), "�ϊ��w�蕶����́F%s", strDesignationFileName.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
    strCmdFileName = pcCmdFileName;
    sprintf_s(m_cLogData, sizeof(m_cLogData), "�R�}���h���́F%s", strCmdFileName.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
    strDirectoryName = pcDirectoryName;
    sprintf_s(m_cLogData, sizeof(m_cLogData), "�p�X�g�ݍ��݃f�B���N�g�����́F%s", strDirectoryName.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
    strDefaultName = pcDefaultName;
    sprintf_s(m_cLogData, sizeof(m_cLogData), "�f�t�H���g�t�@�C�����́F%s", strDefaultName.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    // �R�}���h���𕪊����ăh���C�u�����擾
    deqCmdFileName = SplitDataProc(strCmdFileName, '\\');
    strDriveName = deqCmdFileName.front();
    sprintf_s(m_cLogData, sizeof(m_cLogData), "�h���C�u���́F%s", strDriveName.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    // �R�}���h���𕪊����ăp�X�����擾
    // ���������������[�v
    LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, "***** ��������Ă���f�B���N�g���� *****");
    for (size_t siCnt = 1; siCnt < deqCmdFileName.size(); siCnt++)
    {
        // ���������p�X�����ēx�g�ݍ��킹
        strPathName += '\\';
        strPathName += deqCmdFileName.at(siCnt);
        sprintf_s(m_cLogData, sizeof(m_cLogData), "%zu �ڂ̃f�B���N�g�����F%s", siCnt, deqCmdFileName.at(siCnt).c_str());
        LOGOUTPUT(DATAOUTPUTLOG, NOTRACE, m_cLogData);

        // �Ōオ�t�@�C�������`�F�b�N
        if (deqCmdFileName.at(siCnt) == deqCmdFileName.back())
        {
            // �Ōオ�t�@�C����
            // string�N���X��replace�֐����g�p���čŌ�̃t�@�C�������󕶎���ɒu��
            // ��`�Fstring�N���X.replace(�J�n�ʒu, ����, ������);
            strPathName.replace(strPathName.size() - deqCmdFileName.back().size(), deqCmdFileName.back().size(), "");
        }
    }

    // ������̃p�X�������O�ɏo��
    sprintf_s(m_cLogData, sizeof(m_cLogData), "������̃p�X���́F%s", strPathName.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    // ��΃p�X�擾����
    // ���֐��Ŏg�p����x�[�X��΃p�X�𐶐�
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ��΃p�X�擾�֐��Ăяo�� *****");
    strConvertPathName = GetFullPathProc(strDriveName.at(0), "..", strPathName.c_str());
    strAppPathName = strDriveName + strConvertPathName;
    sprintf_s(m_cLogData, sizeof(m_cLogData), "�x�[�X�Ƃ����΃p�X���F%s", strAppPathName.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    // �又��
    // �ϊ��Ώۃt�@�C�����^�C�v�����O�ɏo��
    sprintf_s(m_cLogData, sizeof(m_cLogData), "�ϊ��Ώۃt�@�C�����^�C�v�́F%d", eConvertKind);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    // �L�[���[�h�f�B���N�g�������ݒ肳��Ă��邩�`�F�b�N
    if ("" != strDirectoryName)
    {
        // �L�[���[�h�f�B���N�g�������ݒ肳��Ă���
        // �ϊ��Ώۃt�@�C�����^�C�v�Ɋ�Â��ăf�B���N�g�������p�X�`���ɕҏW
        switch (eConvertKind)
        {
            // �f�B���N�g�����w�肠�芎�t�@�C�����ύX�Ȃ��i���s�t�H���_�Ɠ��K�w�j
            case tagFileType::FILETYPE1:

            // �f�B���N�g�����w�肠�芎�t�@�C�����ύX����i���s�t�H���_�Ɠ��K�w�j
            case tagFileType::FILETYPE2:
            {
                // �f�B���N�g�������p�X�`���ɕҏW
                sprintf_s(cEditPath, sizeof(cEditPath), ".\\%s", strDirectoryName.c_str());

                // �f�t�H���g�̃t�@�C�������w�肵�Ă��邩�`�F�b�N
                if ("" != strDefaultName && strDefaultName == strDesignationFileName)
                {
                    // �f�t�H���g�̃t�@�C�������w�肵�Ă���
                    // �t�@�C��������
                    // �w��t�@�C��������Ƀt�@�C�����𐶐�
                    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �t�@�C���������֐��Ăяo�� *****");
                    strFileCustomName = CreateFileNameProc(CREATE_DATE_NAME, strDesignationFileName.c_str(), DELIMITER);
                    strDesignationFileName = strFileCustomName;
                    sprintf_s(
                        m_cLogData,
                        sizeof(m_cLogData),
                        "�J�X�^�}�C�Y��̃t�@�C�����́F%s",
                        strDesignationFileName.c_str());
                    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
                }

                // ���򔲂�
                break;
            }

            // �f�B���N�g�����w�肠�芎�t�@�C�����ύX����i���s�t�H���_�Ɠ��K�w�j
            case tagFileType::FILETYPE9:
            {
                // �f�B���N�g�������p�X�`���ɕҏW
                sprintf_s(cEditPath, sizeof(cEditPath), ".\\%s", strDirectoryName.c_str());

                // �t�@�C��������
                // �f�t�H���g����Ƀt�@�C�����𐶐�
                LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �t�@�C���������֐��Ăяo�� *****");
                strFileCustomName = CreateFileNameProc(CREATE_DATE_NAME, strDefaultName.c_str(), DELIMITER);
                strDesignationFileName = strFileCustomName;
                sprintf_s(
                    m_cLogData,
                    sizeof(m_cLogData),
                    "�J�X�^�}�C�Y��̃t�@�C�����́F%s",
                    strDesignationFileName.c_str());
                LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

                // ���򔲂�
                break;
            }

            // �f�B���N�g�����w�肠�芎�t�@�C�����ύX�Ȃ��i���s�t�H���_�̂P�K�w��j
            case tagFileType::FILETYPE5:

            // �f�B���N�g�����w�肠�芎�t�@�C�����ύX����i���s�t�H���_�̂P�K�w��j
            default:
            {
                // �L�[���[�h�f�B���N�g�������p�X�`���ɕҏW
                sprintf_s(cEditPath, sizeof(cEditPath), "..\\%s", strDirectoryName.c_str());

                // �f�t�H���g�̃t�@�C������ݒ肵�Ă��邩�`�F�b�N
                if ("" != strDefaultName)
                {
                    // �f�t�H���g�̃t�@�C������ݒ肵�Ă���
                    // �t�@�C��������
                    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �t�@�C���������֐��Ăяo�� *****");
                    strFileCustomName = CreateFileNameProc(CREATE_DATE_NAME, strDefaultName.c_str(), DELIMITER);
                    strDesignationFileName = strFileCustomName;
                    sprintf_s(
                        m_cLogData,
                        sizeof(m_cLogData),
                        "�J�X�^�}�C�Y��̃t�@�C�����́F%s",
                        strDesignationFileName.c_str());
                    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
                }

                // ���򔲂�
                break;
            }
        }

        // ��΃p�X�擾����
        // �f�B���N�g�����ɐ�΃p�X��t�^
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ��΃p�X�擾�֐��Ăяo�� *****");
        strConvertPathName = GetFullPathProc(strDriveName.at(0), cEditPath, strAppPathName.c_str());
        sprintf_s(m_cLogData, sizeof(m_cLogData), "�p�X��Ԃ́F%s", strConvertPathName.c_str());
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
    }

    // �L�[���[�h�f�B���N�g���������芎�ϊ��Ώۃt�@�C�����^�C�v���������݂������̓g���[�X���O�������̓R�s�[���`�F�b�N
    if ("" != strDirectoryName && tagFileType::FILETYPE1 != eConvertKind)
    {
        // �f�B���N�g�����w�肠�芎�t�@�C�����ύX�Ȃ��i���s�t�H���_�Ɠ��K�w�j�w��ȊO
        // �i�[�f�B���N�g���𐶐�
        // ���Ɋi�[�f�B���N�g�������݂���ꍇ��
        // �����Ɏ��s���邪�����������̃f�B���N�g�����g�p���邽�ߏ������p������
        // �������A�������ʂɑ΂��郁�b�Z�[�W�̓��O�ɏo�͂���
        iDirMakeResult = _mkdir(strConvertPathName.c_str());
        sprintf_s(m_cLogData, sizeof(m_cLogData), "�������ʂ́F%d", iDirMakeResult);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

        // �������ʂ��`�F�b�N
        if (0 != iDirMakeResult)
        {
            // ���Ɋi�[�f�B���N�g�������݂���
            // ���݂���|�̃��b�Z�[�W�����O�ɏo��
            sprintf_s(m_cLogData, sizeof(m_cLogData), "%s %s", strConvertPathName.c_str(), ALREADYEXISTS);
            LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, m_cLogData);
        }
        else
        {
            // �i�[�f�B���N�g����������
            // �i�[�f�B���N�g�����������̃��b�Z�[�W�����O�ɏo��
            sprintf_s(m_cLogData, sizeof(m_cLogData), "%s %s", strConvertPathName.c_str(), MAKEDIRSUCCESS);
            LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, m_cLogData);
        }
    }

    // �㏈��
    // �L�[���[�h�f�B���N�g�������ݒ肳��Ă��邩�`�F�b�N
    if ("" != strDirectoryName)
    {
        // �L�[���[�h�f�B���N�g�������ݒ肳��Ă���
        // �t�@�C���p�X�ɂăL�[���[�h�f�B���N�g��������
        pcSearch = strstr((char*)strDesignationFileName.c_str(), strDirectoryName.c_str());
        sprintf_s(m_cLogData, sizeof(m_cLogData), "�������ʁF%s", pcSearch);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

        // �t�@�C���p�X�ɑ΂��L�[���[�h�f�B���N�g�����w�肵�Ă��邩�`�F�b�N
        if (NULL == pcSearch)
        {
            // �L�[���[�h�f�B���N�g���w��Ȃ�
            // ��΃p�X�擾����
            // �ҏW������΃p�X��t�^
            LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ��΃p�X�擾�֐��Ăяo�� *****");
            strConvertFileName = GetFullPathProc(
                strDriveName.at(0),
                strDesignationFileName.c_str(),
                strConvertPathName.c_str());
            sprintf_s(m_cLogData, sizeof(m_cLogData), "��΃p�X�t���t�@�C�����́F%s", strConvertFileName.c_str());
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
        }
        else
        {
            // �L�[���[�h�f�B���N�g���w�肠��
            // ��΃p�X�擾����
            // �x�[�X��΃p�X��t�^
            LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ��΃p�X�擾�֐��Ăяo�� *****");
            strConvertFileName = GetFullPathProc(strDriveName.at(0), strDesignationFileName.c_str(), strAppPathName.c_str());
            sprintf_s(m_cLogData, sizeof(m_cLogData), "��΃p�X�t���t�@�C�����́F%s", strConvertFileName.c_str());
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
        }
    }
    else
    {
        // �L�[���[�h�f�B���N�g�������ݒ肳��Ă��Ȃ�
        // �ϊ��Ώۃt�@�C�����^�C�v�Ɋ�Â��Đ�΃p�X�t���t�@�C�����𐶐�
        sprintf_s(m_cLogData, sizeof(m_cLogData), "�ϊ��Ώۃt�@�C�����^�C�v�́F%d", eConvertKind);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
        switch (eConvertKind)
        {
            // �f�B���N�g�����w��Ȃ����t�@�C�����ύX����
            case tagFileType::FILETYPE6:
            {
                // �ϊ��t�@�C��������
                LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �t�@�C���������֐��Ăяo�� *****");
                strFileCustomName = CreateFileNameProc(CREATE_TIME_NAME, strDesignationFileName.c_str(), DELIMITER, iBaseResult);
                sprintf_s(m_cLogData, sizeof(m_cLogData), "�J�X�^�}�C�Y��̃t�@�C�����́F%s", strFileCustomName.c_str());
                LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

                // ��΃p�X�擾����
                // �x�[�X��΃p�X��t�^
                LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ��΃p�X�擾�֐��Ăяo�� *****");
                strDesignationFileName = strFileCustomName;
                strConvertFileName = GetFullPathProc(strDriveName.at(0), strDesignationFileName.c_str(), strAppPathName.c_str());
                sprintf_s(m_cLogData, sizeof(m_cLogData), "��΃p�X�t���t�@�C�����́F%s", strConvertFileName.c_str());
                LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

                // ���򔲂�
                break;
            }

            // �f�B���N�g�����w��Ȃ����t�@�C�����ύX�Ȃ��i�x�[�X��΃p�X�ݒ�j
            case tagFileType::FILETYPE7:
            {
                // ��΃p�X�擾����
                // �x�[�X��΃p�X��t�^
                LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ��΃p�X�擾�֐��Ăяo�� *****");
                strConvertFileName = GetFullPathProc(strDriveName.at(0), strDesignationFileName.c_str(), strAppPathName.c_str());
                sprintf_s(m_cLogData, sizeof(m_cLogData), "��΃p�X�t���t�@�C�����́F%s", strConvertFileName.c_str());
                LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

                // ���򔲂�
                break;
            }

            // �f�B���N�g�����w��Ȃ����t�@�C�����ύX�Ȃ��i�R�}���h���ݒ�j
            default:
            {
                // ��΃p�X�擾����
                // �R�}���h��񂩂��΃p�X��t�^
                LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ��΃p�X�擾�֐��Ăяo�� *****");
                strConvertFileName = GetFullPathProc(strDriveName.at(0), strDesignationFileName.c_str(), strCmdFileName.c_str());
                sprintf_s(m_cLogData, sizeof(m_cLogData), "��΃p�X�t���t�@�C�����́F%s", strConvertFileName.c_str());
                LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

                // ���򔲂�
                break;
            }
        }
    }

    // �������擾����
    // ��΃p�X�ϊ��t�@�C�����擾�I�������擾
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ��΃p�X�ϊ��t�@�C�����擾�����I�� *****");
    m_stTimeData = g_clsFileOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData,
        sizeof(m_cLogData),
        "��΃p�X�ϊ��t�@�C�����擾�I�������F%s %s",
        (char *)m_stTimeData.strMachDatePlus.c_str(),
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);

    // ��΃p�X�t�^�̃t�@�C������ԋp
    return strConvertFileName;
}

//********************************************
// �֐����F�t�@�C������������
// Function Name�FCreateFileNameProc
// 
// �����T�v�F
// �t�@�C�������^�C�v�y�ьĂяo�������s���ʂɊ�Â���
// �p�r�ɉ������t�@�C��������������
// 
// �����菇�F
//   �P�D�t�@�C��������
//   �Q�D�t�@�C��������
// 
// �����F     �t�@�C�������^�C�v
//            �����O�t�@�C����
//            ��؂蕶��
//            �Ăяo�������s���ʁi�ݒ肪�Ȃ��ꍇ��0�j
// �߂�l�F   ������t�@�C����
// 
// �⑫����
// �E�����`���̏ꍇ�Ăяo�������s���ʂɊ�Â���
//     �t�@�C�������ȉ��̌`���Ő�������
//     ����I���F�t�@�C�����{�g���q�D���t�{����
//     �ُ�I���F�t�@�C�����{�g���q�D99999999999999
// 
// 
// �쐬����
// 2020/9/25  s-saito   �V�K�쐬
// 
// �X�V����
// 
// 
//********************************************
string CFileOperations::CreateFileNameProc(
	int iKind, 
	const char *pcFileName, 
	const char *pcDelimiter, 
	int iResult)
{
    // �ϐ��錾
    string strCustomFileName;
    string strFileNameMain;
    string strFileExtension;
    char cEditFileName[FILENAMESIZE + 1];
    char* pcEditFileName = NULL;
    char* pcCtx = NULL;

    // ������
    memset(cEditFileName, 0x00, sizeof(cEditFileName));

    // �O����
    // �������擾����
    // �t�@�C���������J�n�����擾
    m_stTimeData = g_clsFileOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "�t�@�C���������J�n�����F%s %s", 
        (char *)m_stTimeData.strMachDatePlus.c_str(), 
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �t�@�C�������������J�n *****");

    // �p�����[�^�����R�s�[
    strncpy_s(cEditFileName, sizeof(cEditFileName), pcFileName, sizeof(cEditFileName) - 1);
    sprintf_s(m_cLogData, sizeof(m_cLogData), "�t�@�C�����́F%s", cEditFileName);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    // �又��
    // strtok_s�֐��Ńt�@�C��������
    // �t�@�C����
    pcEditFileName = strtok_s(cEditFileName, pcDelimiter, &pcCtx);
    strFileNameMain = pcEditFileName;
    sprintf_s(m_cLogData, sizeof(m_cLogData), "�擪�̃t�@�C�����́F%s", strFileNameMain.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    // �g���q
    while (NULL != pcEditFileName)
    {
        // strtok_s�֐��ɂ��ύX���ꂽNULL�̃|�C���^���擪
        pcEditFileName = strtok_s(NULL, pcDelimiter, &pcCtx);

        // pcFileName��NULL���`�F�b�N
        if (NULL != pcEditFileName)
        {
            // NULL�łȂ���Ε�������i�[
            strFileExtension += ".";
            strFileExtension += pcEditFileName;
            sprintf_s(m_cLogData, sizeof(m_cLogData), "�g���q�́F%s", strFileExtension.c_str());
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
        }
    }

    // �㏈��
    // �t�@�C�������^�C�v�����O�ɏo��
    sprintf_s(m_cLogData, sizeof(m_cLogData), "�t�@�C�������^�C�v�́F%d", iKind);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    // �t�@�C�����̌`���𔻒�
    switch (iKind)
    {
        case 1:
        {
            // ���t�̂�
            // ���t�Ƃ̑g�ݍ��킹�ŘA��
            strCustomFileName = strFileNameMain + "_" + m_stTimeData.strMachDate + strFileExtension;
            break;
        }
        case 2:
        {
            // ��������
            // �Ăяo�����Ŏw�肳�ꂽ���s���ʂ��`�F�b�N
            if (EXIT_SUCCESS == iResult)
            {
                // ���s���ʂ͐���
                // ���t�Ǝ����Ƃ̑g�ݍ��킹�ŘA��
                strCustomFileName = strFileNameMain + strFileExtension + "." + m_stTimeData.strMachDate + m_stTimeData.strMachTime;
            }
            else
            {
                // ���s���ʂ͐���ł͂Ȃ�
                // �_�~�[�f�[�^��t�^
                strCustomFileName = strFileNameMain + strFileExtension + "." + "99999999" + "999999";
            }

            // ��������
            break;
        }
    }

    // ������̃t�@�C�������o��
    sprintf_s(m_cLogData, sizeof(m_cLogData), "������̃t�@�C�����́F%s", strCustomFileName.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    // �������擾����
    // �t�@�C���������I�������擾
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �t�@�C�������������I�� *****");
    m_stTimeData = g_clsFileOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "�t�@�C���������I�������F%s %s", 
        (char *)m_stTimeData.strMachDatePlus.c_str(), 
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);

    // ������̃t�@�C������ԋp
    return strCustomFileName;
}

//********************************************
// �֐����F��΃p�X�擾����
// Function Name�FGetFullPathProc
// 
// �����T�v�F
// �w�肳�ꂽ�p�X���y�уt�@�C�����Ɋ�Â���
// ��΃p�X�t���̃p�X���y�уt�@�C�������擾����
// 
// �����菇�F
//   �P�D�������̈�m��
//   �Q�D�p�����[�^�`�F�b�N
// �i��΃p�X�������̓J�����g�p�X���`�F�b�N�j
// �i�ȍ~�͑��΃p�X�̏ꍇ�̂݁j
//   �R�D���΃p�X�����΃p�X�ւ̕ϊ�
//   �S�D��΃p�X�ϊ���̌��ʂ��i�[
//   �T�D�������̈���
// 
// �����F     �h���C�u���i�擪�����j
//            �ϊ��O�t�@�C���p�X
//            �x�[�X��΃p�X
// �߂�l�F   �ϊ���t�@�C���p�X
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
string CFileOperations::GetFullPathProc(
	const char cDriveNameTop, 
	const char *pcDesignationPath, 
	const char *pcBasePath)
{
    // �ϐ��錾
    string strResultPath;
    char *pcResultPath = NULL;
    const char *pcPathPoint = NULL;
    const char *pcBasePoint = NULL;
    char *pcResultPoint = NULL;
    size_t siLength = 0;

    // �W���o�͂ɓ��{�ꂪ�\�������悤�Ƀ��P�[������{�ɐݒ�
    setlocale(LC_ALL, "japanese");

    // �������̈�m�ۂɎ��s�����ꍇ��NULL�ݒ�
    // ���̐ݒ�ɂ��m�ۂɎ��s�����ꍇ��NULL��Ԃ��悤�ɂ���
    set_new_handler(NULL);

    // �O����
    // �������擾����
    // ��΃p�X�ݒ�J�n�����擾
    m_stTimeData = g_clsFileOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "��΃p�X�ݒ�J�n�����F%s %s", 
        (char *)m_stTimeData.strMachDatePlus.c_str(), 
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ��΃p�X�擾�����J�n *****");

    // �p�����[�^�����o��
    sprintf_s(m_cLogData, sizeof(m_cLogData), "�ϊ��O�t�@�C���p�X�́F%s", pcDesignationPath);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
    sprintf_s(m_cLogData, sizeof(m_cLogData), "�x�[�X�̐�΃p�X�́F%s", pcBasePath);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    try
    {
        // �p�X���E�t�@�C�����ҏW�������̈�̊m��
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �p�X���E�t�@�C�����ҏW�������̈�m�� *****");
        pcResultPath = DBG_NEW char[FILENAMESIZE + 1];
        m_stLogData = CreateEventMessageProc(ACTIONSET, "�p�X���E�t�@�C�����ҏW��");

        // �������̈悪�m�ۏo�������`�F�b�N
        if (NULL == pcResultPath)
        {
            // �������̈�̊m�ێ��s
            // ��O�����փX���[
            throw tagErrorCode::MEMORYERROR;
        }

        // �������̏�����
        memset(pcResultPath, 0x00, FILENAMESIZE + 1);

        // �������̈�m�ې����̃��b�Z�[�W�����O�ɏo��
        LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, "***** �p�X���E�t�@�C�����ҏW�������̈�m�ې���I�� *****");
        sprintf_s(m_cLogData, sizeof(m_cLogData), "%s��%s", m_stLogData.pcActionName, MEMORYCREATESUCCESS);
        LOGOUTPUT(TRACEOUTPUTLOG, SUCCESSINFORMATION, m_cLogData);

        // �ϊ��O�̃p�X����΃p�X�\�L���`�F�b�N
        // Windows�łł͐�΃p�X�̓h���C�u������n�܂邽�߃h���C�u���Ŕ��ʂ���
        if (cDriveNameTop == *pcDesignationPath)
        {
            // �ϊ��O�̃p�X����΃p�X�\�L
            // �ϊ��O�̃p�X�����̂܂܃R�s�[���Č㏈���ֈړ�
            strncpy_s(pcResultPath, FILENAMESIZE + 1, pcDesignationPath, FILENAMESIZE);
            pcResultPath[strlen(pcResultPath)] = '\0';
            strResultPath = pcResultPath;
            LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ��΃p�X�w�� *****");
            sprintf_s(m_cLogData, sizeof(m_cLogData), "�ϊ���̌��ʂ́F%s", strResultPath.c_str());
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
            LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, FULLPATHDEFINITION);
            goto END;
        }
        else
        {
            // �ϊ��O�̃p�X����΃p�X�\�L�ł͂Ȃ�
            // �p�X�������O�ɏo��
            sprintf_s(m_cLogData, sizeof(m_cLogData), "�w��̃p�X���́F%s", pcDesignationPath);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
        }

        // �ϊ��O�̃p�X���J�����g�p�X�\�L���`�F�b�N
        if (1 == !strncmp(pcDesignationPath, ".", sizeof(pcDesignationPath)) ||
            1 == !strncmp(pcDesignationPath, ".\\", sizeof(pcDesignationPath)))
        {
            // �ϊ��O�̃p�X���J�����g�p�X�\�L
            // �x�[�X��΃p�X���R�s�[���Č㏈���ֈړ�
            strncpy_s(pcResultPath, FILENAMESIZE + 1, pcBasePath, FILENAMESIZE);
            pcResultPath[strlen(pcResultPath)] = '\0';
            strResultPath = pcResultPath;
            LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �w.�x�������́w.\\�x�ɂ��J�����g�p�X�w�� *****");
            sprintf_s(m_cLogData, sizeof(m_cLogData), "�ϊ���̌��ʂ́F%s", strResultPath.c_str());
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
            goto END;
        }
        else
        {
            // �ϊ��O�̃p�X���J�����g�p�X�\�L�ł͂Ȃ�
            // �p�X�������O�ɏo��
            sprintf_s(m_cLogData, sizeof(m_cLogData), "�w��̃p�X���́F%s", pcDesignationPath);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
        }

        // �又��
        // �W���n�_���擾
        // �x�[�X��΃p�X�̒������擾
        siLength = strlen(pcBasePath);
        sprintf_s(m_cLogData, sizeof(m_cLogData), "�����́F%zu", siLength);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

        // �x�[�X��΃p�X�̏I�[�ʒu���擾
        // pcBasePoint �̓x�[�X��΃p�X�̏I�[��\��
        pcBasePoint = pcBasePath + siLength;
        sprintf_s(m_cLogData, sizeof(m_cLogData), "�I�[�ʒu�́F%s", pcBasePoint);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

        // �I�[�ʒu���`�F�b�N
        if ('\\' == *(pcBasePoint - 1))
        {
            // �I�[�ʒu��1�O�̒l���w\\�x
            // �|�C���^�̈ʒu���f�N�������g
            --pcBasePoint;
            sprintf_s(m_cLogData, sizeof(m_cLogData), "�W���n�_�́F%s", pcBasePoint);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
        }

        // �ϊ��O�t�@�C���p�X�̐擪�����ɂ���w.\\�x�E�w..�x�E�w..\\�x����͂��鎖�ɂ��
        // �x�[�X��΃p�X�̃p�X�\�L�ɑ΂��A�����܂ŕϊ���t�@�C���p�X�Ƌ��ʂɂȂ邩�𒲂ׂ�
        for (pcPathPoint = pcDesignationPath; *pcPathPoint != '\0' && *pcPathPoint == '.';)
        {
            // �擪�ʒu�����O�ɏo��
            sprintf_s(m_cLogData, sizeof(m_cLogData), "�擪�ʒu�́F%s", pcPathPoint);
            LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

            // pcPathPoint �̈ʒu���`�F�b�N
            if (1 == !strncmp(pcPathPoint, "..\\", PATHCHECK_SIZE_3BYTE))
            {
                // pcPathPoint ���w..\\�x
                // �|�C���^�̈ʒu���ړ�
                pcPathPoint += 3;
                while (pcBasePoint > pcBasePath && *--pcBasePoint != '\\');
                LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, (char*)"***** �w..\\�x�̏����ŏ��� *****");
                sprintf_s(m_cLogData, sizeof(m_cLogData), "pcPathPoint �ʒu�́F%s", pcPathPoint);
                LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
                sprintf_s(m_cLogData, sizeof(m_cLogData), "pcBasePoint �I�[�ʒu�́F%s", pcBasePoint);
                LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
            }
            else if (1 == !strncmp(pcPathPoint, ".\\", PATHCHECK_SIZE_2BYTE))
            {
                // pcPathPoint ���w.\\�x
                // �|�C���^�̈ʒu���ړ�
                pcPathPoint += 2;
                LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, (char*)"***** �w..\\�x�̏����ŏ��� *****");
                sprintf_s(m_cLogData, sizeof(m_cLogData), "pcPathPoint �ʒu�́F%s", pcPathPoint);
                LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
            }
            else if (1 == !strncmp(pcPathPoint, "..\0", PATHCHECK_SIZE_3BYTE))
            {
                // pcPathPoint ���w..�x
                // �|�C���^�̈ʒu���ړ�
                pcPathPoint += 2;
                while (pcBasePoint > pcBasePath && *--pcBasePoint != '\\');
                LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, (char*)"***** �w..�x�̏����ŏ��� *****");
                sprintf_s(m_cLogData, sizeof(m_cLogData), "pcPathPoint �ʒu�́F%s", pcPathPoint);
                LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
                sprintf_s(m_cLogData, sizeof(m_cLogData), "pcBasePoint �I�[�ʒu�́F%s", pcBasePoint);
                LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
            }
            else
            {
                // pcPathPoint ����L�ȊO
                // pcPathPoint �ʒu�����O�ɏo��
                sprintf_s(m_cLogData, sizeof(m_cLogData), "pcPathPoint �ʒu�́F%s", pcPathPoint);
                LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

                // ���[�v�I��
                break;
            }
        }

        // �x�[�X��΃p�X�Ƌ��ʕ�����ϊ���t�@�C���p�X�փR�s�[
        siLength = pcBasePoint - pcBasePath;
        strncpy_s(pcResultPath, siLength + 1, pcBasePath, siLength);
        sprintf_s(m_cLogData, sizeof(m_cLogData), "�����́F%zu", siLength);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
        sprintf_s(m_cLogData, sizeof(m_cLogData), "��Ԃ́F%s", pcResultPath);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

        // �ϊ���t�@�C���p�X�|�C���^�̈ʒu���擾
        pcResultPoint = pcResultPath + siLength;
        *pcResultPoint++ = '\\';
        sprintf_s(m_cLogData, sizeof(m_cLogData), "�|�C���^�́F%s", pcResultPoint);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

        // �ϊ��O�t�@�C���p�X������̂����A�擪�����́w.\\�x��w..\\�x���������c��̕���
        // �ipcPathPoint ���w��������j�� �ϊ���t�@�C���p�X������ɒǉ�
        strcpy(pcResultPoint, pcPathPoint);
        pcResultPath[strlen(pcResultPath)] = '\0';
        strResultPath = pcResultPath;
        sprintf_s(m_cLogData, sizeof(m_cLogData), "�ϊ���̌��ʂ́F%s", strResultPath.c_str());
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
    }

    // ��O����
    catch (const ERRORCODETABLE &eAllocErr)
    {
        // �G���[���b�Z�[�W��������
        // �G���[���b�Z�[�W���o��
        pcResultPath = NULL;
        LOGOUTPUT(TRACEOUTPUTLOG, ERRORINFORMATION2, "***** �p�X���E�t�@�C�����ҏW�������̈�m�ۈُ�I�� *****");
        LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �G���[���b�Z�[�W�����֐��Ăяo�� *****");
        m_eErrorResultCode = eAllocErr;
        m_stLogData = CreateEventMessageProc(ERRORSET, m_stLogData.pcActionName, m_eErrorResultCode);
        LOGOUTPUTERROR(TRACEOUTPUTLOG, ERRORINFORMATION1, m_stLogData.pcMessage, m_stLogData.eErrorCode);
        sprintf_s(m_cLogData, sizeof(m_cLogData), "�߂�l�́F%d", m_stLogData.eErrorCode);
        LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);
    }

END:
    // �㏈��
    // ��΃p�X�t���t�@�C�����ݒ�Ŏg�p�����t�@�C�����ҏW�������̈�����
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �t�@�C�����ҏW�������̈��� *****");
    SAFE_FREE_NEWALLOC_ARRAY(pcResultPath);
    pcResultPoint = NULL;

    // �������̈����̃��b�Z�[�W�����O�ɏo��
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** �t�@�C�����ҏW�������̈����I�� *****");
    sprintf_s(m_cLogData, sizeof(m_cLogData), "%s%s", m_stLogData.pcActionName, MEMORYRELEASESUCCESS);
    LOGOUTPUT(TRACEOUTPUTLOG, DATAINFORMATION, m_cLogData);

    // �������擾����
    // ��΃p�X�ݒ�I�������擾
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, "***** ��΃p�X�擾�����I�� *****");
    m_stTimeData = g_clsFileOpeLogFile.GetDateTimeProc();
    sprintf_s(
        m_cLogData, 
        sizeof(m_cLogData), 
        "��΃p�X�ݒ�I�������F%s %s", 
        (char *)m_stTimeData.strMachDatePlus.c_str(), 
        (char *)m_stTimeData.strMachTimePlus.c_str());
    LOGOUTPUT(TRACEOUTPUTLOG, NORMALINFORMATION, m_cLogData);

    // �ϊ���t�@�C���p�X��ԋp
    return strResultPath;
}
