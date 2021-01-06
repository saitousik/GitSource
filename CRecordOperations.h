#ifndef _CRECORDOPERATIONS_H_
#define _CRECORDOPERATIONS_H_
//============================================
// ���ގ����R�[�h����N���X�w�b�_�[�t�@�C��
// CRecordOperations.h
// 
// �@�\�T�v
// ���ގ��󋵈󎚂ɑ΂���f�[�^�ҏW�y�яo�͏����`����
// �����ł͎g�p����萔�y��API�̃v���g�^�C�v��錾����
//
// ��`���e
// �P�D�C���N���[�h��`
// �Q�D���C�u�����Ăяo��
// �R�D�N���X��`
//
// �쐬����
// 2020/9/25  s-saito   �V�K�쐬
// 
// �X�V����
// 
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
//�� �C���N���[�h��`
#pragma once
#include "CFileOperations.h"
#include "DllEntryExitDataDefineStruct.h"

//�� �O���Q�ƕϐ���`
extern FILENAMEINFORMATION g_stFileNameInfo;

// ���N���X��`
class CRecordOperations : 
	public CFileOperations
{
// Private�����o�[�ϐ�
private:
	// �g���[�X���O�o�̓t�@�C�����ϐ�
	string m_strRecOpeTraceLogFileName;

	// �f�[�^���O�o�̓t�@�C�����ϐ�
	string m_strRecOpeDataLogFileName;

// Public�����o�[�ϐ�
public:
	// ���݂̃h�A���x��
	int m_iNowDoorLevel = 0;

	// STAT�\���̕ϐ�
	struct stat m_stStat;

	// ���ގ��������\���̕ϐ�
	ENTRYEXITINFORMATION m_stEntryExitInfo;

// Public�����o�[�֐�
public:
	// �f�t�H���g�R���X�g���N�^
	CRecordOperations();

	// �p�����[�^���w��R���X�g���N�^
	CRecordOperations(const string, const string);

	// �f�X�g���N�^
	~CRecordOperations();

	// ���ގ��f�[�^�ҏW���擾����
	ENTRYEXITDATA *GetEditDataProc(FILE *, FILE *, bool *, int *, int *, long *);

	// ���ގ��f�[�^�ҏW���o�͎w������
	void PutEditDataProc(FILE *, ENTRYEXITDATA *, DOORLEVELINFORMATION *);

	// �����G���A���x���N���A����
	void ClearLevelProc(FILE *, const ENTRYEXITDATA, const int, DOORLEVELINFORMATION *);

	// ���ގ��󋵈󎚌��o���ݒ菈��
	void SetEntryExitDataTitleProc(FILE *, ERRORCODETABLE eResult = tagErrorCode::SUCCESS);

// Private�����o�[�֐�
private:
	// ���ގ��f�[�^�ҏW��񐶐�����
	ENTRYEXITDATA CreateEditDataProc(const ENTRYEXITRECORD);

	// �����G���A���x���`�F�b�N����
	int CheckLevelProc(const ENTRYEXITDATA, DOORLEVELINFORMATION *);

	// ���ގ��f�[�^�ҏW���o�͐��䏈��
	void PrintDataControlProc(
		int,
		FILE *,
		const char *,
		const char *,
		const ENTRYEXITDATA,
		const char *,
		PRINTTYPETABLE eType = tagPrintType::DUMMY,
		int iLevel1 = 0,
		int iLevel2 = 0);

	// ���ގ��s���f�[�^�t�@�C���o�͏���
	void PrintFraudDataFileProc(const char *, const char *);

	// ���ގ��f�[�^�t�@�C���o�͏���
	void PrintReportFileProc(FILE *, const char *, const char *, const ENTRYEXITDATA, const char *, PRINTTYPETABLE, int, int);

	// ���ގ��f�[�^PC�{����ʏo�͏���
	void PrintScreenProc(const char *, const char *, const ENTRYEXITDATA, const char *, PRINTTYPETABLE, int, int);
};
#endif // _CRECORDOPERATIONS_H_
