#ifndef _DLLENTRYEXITDATADEFINESTRUCT_H_
#define _DLLENTRYEXITDATADEFINESTRUCT_H_
//============================================
// ���ގ��󋵈󎚒萔�E�\���́E���p�̒�`�w�b�_�[�t�@�C��
// DllEntryExitDataDefineStruct.h
// 
// �@�\�T�v
// ���ގ��󋵈󎚋@�\�Ŏg�p����萔�y�э\���́E���p�̂�񋟂���
//
// ��`���e
// �P�D�C���N���[�h��`
// �Q�D���R�[�h������`
// �R�D�f�B���N�g�����萔��`
// �S�D���̑��萔��`
// �T�D���b�Z�[�W��`
// �U�D�񋓌^�錾
// �V�D�\���́E���p�̐錾
//
// �쐬����
// 2020/9/25  s-saito   �V�K�쐬
// 
// �X�V����
// 
// 
// �o�[�W����
//                         ver.1.0
// �⑫����
// �E���w�b�_�[�y�у\�[�X���ɂ��Ă͓��ގ��󋵈󎚋@�\��
// �@�g�p������݂̂Ƃ���
// 
//============================================
//��������������������������������������������
// �e�萔�E�v���g�^�C�v��`
//��������������������������������������������
#pragma once
// ���O���Q�ƒ�`
#ifdef DLLENTRYEXITDATAOUTPUT_EXPORTS
#define ENTRYEXITDATALIBRARY_API __declspec(dllexport)
#else
#define ENTRYEXITDATALIBRARY_API __declspec(dllimport)
#endif // DLLENTRYEXITDATAOUTPUT_EXPORTS

// �����C�u�����Ăяo��
// DLL�t�@�C���݂̂̏ꍇ��LoadModule�֐��ELoadLibrary�֐��Ń��C�u�������Ăяo���B
#ifdef NDEBUG
#ifdef WIN64
#pragma comment(lib, "x64\\Release\\OperationsSupport.lib")
#else
#pragma comment(lib, "Release\\OperationsSupport.lib")
#endif // WIN64
#else
#ifdef WIN64
#pragma comment(lib, "x64\\Debug\\OperationsSupport.lib")
#else
#pragma comment(lib, "Debug\\OperationsSupport.lib")
#endif // WIN64
#endif // NDEBUG

// �����R�[�h������`
#define RECORD_CARDID 4
#define RECORD_DATE 8
#define RECORD_TIME 6
#define RECORD_DOOR 2
#define RECORD_DIR 1
#define RECORD_ACT 1
#define RECORD_NAME 10

// ���f�B���N�g�����E�t�@�C�����萔��`
#define READPATH "Data\\"
#define WRITEPATH "Output\\"
#define LOGFILEPATH "Log\\"
#define COPYPATH "Backup\\"
#define DEFAULTFILENAME "AccessOutput.txt"
#define LOGFILENAME "TraceInfo.log"
#define DATALOGFILENAME "DataInfo.log"
#define FRAUDDATAFILENAME "FraudDataInfo.txt"

// �����ϐ���`
#define READ_FILE "READFILE"
#define WRITE_FILE "WRITEFILE"

// �����̑��萔��`
#define TITLE "ID    ���O        ���x���P             ���x���Q             ���x���R             "
#define NYUSHITSU "I"
#define TAISHITSU "O"
#define KYOHI "R"
#define READDATA 33
#define FLOORMAX 3
#define FILEEND_OFF 0
#define FILEEND_ON 1

// �����b�Z�[�W��`
// ���ُ탁�b�Z�[�W
#define INITFAILED "�J�n�����Ɏ��s���܂����B"
#define ENDFAILED "�I�������Ɏ��s���܂����B"

// �����탁�b�Z�[�W
#define READEND "���ގ��ǂݍ��݃f�[�^�t�@�C���̓ǂݍ��ݏI���܂����B"
#define GETRECORDSUCCESS "���ގ����R�[�h���擾�o���܂����B"
#define INITSUCCESS "�J�n�������������܂����B"
#define ENDSUCCESS "�I���������������܂����B"

// ���񋓌^�錾
// �f�[�^�o�̓^�C�v
typedef enum struct tagPrintType
{
	DUMMY,											// �����l
	DOOR,											// �h�A�ԍ�
	CARDID,											// �J�[�hID�y�у��[�U�[��
	ACT,											// �ۏ��
	LEVELNG1,										// �����G���A���x��NG���P
	LEVELNG2,										// �����G���A���x��NG���Q
	LEVELNG3,										// �����G���A���x��NG���R
	LEVELNG4,										// �����G���A���x��NG���S
	LEVELNG5,										// �����G���A���x��NG���T
	LEVELNG6,										// �����G���A���x��NG���U
	DUMMYDATA										// �_�~�[�f�[�^
}PRINTTYPETABLE;

// �f�[�^�o�͐掯�ʎq
enum
{
	FILEPRINT = 1,									// ���ގ��������݃f�[�^�t�@�C���̂ݏo��
	SCREENPRINT,									// PC�{����ʂ̂ݏo��
	BOTHPRINT										// �����ɏo��
};

// ���\���̐錾
// �t�@�C�������Ǘ��\����
typedef struct tagFileNameInfo
{
	string strReadName;				                // ���ގ��ǂݍ��݃f�[�^�t�@�C����
	string strWriteName;				            // ���ގ��������݃f�[�^�t�@�C����
	string strTraceLogName;				            // �g���[�X���O�o�̓t�@�C����
	string strDataLogName;			                // �f�[�^���O�o�̓t�@�C����
	string strBackupName;			                // �o�b�N�A�b�v�t�@�C����
	string strChangeName;				            // ���̕ύX�Ώۃt�@�C����
	string strDeleteName;				            // �폜�Ώۃt�@�C����
	string strModuleName;				            // ���s�A�v���P�[�V������
	string strFraudDataName;						// �s���f�[�^�t�@�C����
}FILENAMEINFORMATION;

// ���ގ���񃌃R�[�h
typedef struct tagEntryExitRecordInfo
{
	char cCardID[RECORD_CARDID];					// �J�[�hID
	char cDate[RECORD_DATE];						// ���t
	char cTime[RECORD_TIME];						// ����
	char cDoor[RECORD_DOOR];						// �h�A�ԍ�
	char cDir[RECORD_DIR];							// ���ޏo���
	char cAct[RECORD_ACT];							// �ۏ��
	char cName[RECORD_NAME];						// ���[�U�[��
}ENTRYEXITRECORDINFO;

// ���ގ����f�[�^�ҏW�p�\����
typedef struct tagEntryExitData
{
	char cCardID[RECORD_CARDID + 1];				// �J�[�hID
	char cName[RECORD_NAME + 1];					// ���[�U�[��
	char cDate[RECORD_DATE + 1];					// ���t
	char cTime[RECORD_TIME + 1];					// ����
	char cDoor[RECORD_DOOR + 1];					// �h�A�ԍ�
	char cDir[RECORD_DIR + 1];						// ���ޏo���
	char cAct[RECORD_ACT + 1];						// �ۏ��
}ENTRYEXITDATA;

// ���ގ���񋤗p��
typedef union tagEntryExitRecord
{
	ENTRYEXITRECORDINFO stEntryExitRecord;			// ���ގ��f�[�^
	char cEntryExitData[DATASIZE];					// ���ގ��f�[�^�i������^�j
}ENTRYEXITRECORD;

// �h�A���x�����\����
typedef struct tagDoorLevelInfo
{
	unsigned int iDoorLevel : 2;					// �ʉ߃h�A���x��
	unsigned int iBeforeLevel : 2;					// �ʉߑO�h�A���x��
	unsigned int iAfterLevel : 2;					// �ʉߌ�h�A���x��
	unsigned int iLevel : 2;						// ���݂̒ʉ߃h�A���x��
}DOORLEVELINFORMATION;

// ���ގ��������\����
typedef struct tagEntryExitInfo
{
	ENTRYEXITRECORD unEntryExitRecord;				// ���ގ����R�[�h
	deque<ENTRYEXITDATA> deqEntryExitEditData;		// ���ގ����f�[�^�ҏW�p�\���́ideque�R���e�i�N���X�j
	DOORLEVELINFORMATION stDoorLevelData{};			// �h�A���x�����\����
	char cLastID[RECORD_CARDID + 1];				// �ޔ��J�[�hID
}ENTRYEXITINFORMATION;
#endif // _DLLENTRYEXITDATADEFINESTRUCT_H_
