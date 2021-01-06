#ifndef _CLOGFILE_H_
#define _CLOGFILE_H_
//============================================
// ���O�o�̓N���X�w�b�_�[�t�@�C��
// CLogFile.h
// 
// �@�\�T�v
// ���O���o�͂�������`����
// �����ł͎g�p����萔�y��API�̃v���g�^�C�v��錾����
//
// ��`���e
// �P�D�C���N���[�h��`
// �Q�D�N���X��`
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
// �E���w�b�_�[�y�у\�[�X���ɂ��Ă͂��̑��̋@�\�ł�
// �@�T�|�[�g���\�Ƃ���
// 
//============================================
//��������������������������������������������
// �e�萔�E�v���g�^�C�v��`
//��������������������������������������������
// ���C���N���[�h��`
#pragma once
#include "DllCommonOperation.h"

// ���N���X��`
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
class CLogFile
{
// Public�����o�[�ϐ�
public:
    // �g���[�X���O�o�̓t�@�C�����ϐ�
    string m_strTraceLogFileName;

    // �f�[�^���O�o�̓t�@�C�����ϐ�
    string m_strDataLogFileName;

    // �������\���̕ϐ�
    TIMEDATA m_stTimeData{};

    // ���O�o�͏��\���̕ϐ�
    LOGINFORMATION m_stLogData;

    // ���O�o�̓f�[�^�ϐ�
    char m_cLogData[LOGDATA]{};

    // �G���[�R�[�h�ϐ�
    ERRORCODETABLE m_eErrorResultCode{};

    // �C�x���g���b�Z�[�W�Ǘ��e�[�u���ϐ�
    map<int, string> m_maErrorActionName;

// Public�����o�[�֐�
public:
	// �f�t�H���g�R���X�g���N�^
    COMMONLIBRARY_API CLogFile();

    // �p�����[�^���w��R���X�g���N�^
    COMMONLIBRARY_API CLogFile(const string, const string);

	// �f�X�g���N�^
    COMMONLIBRARY_API virtual ~CLogFile();

    // �}�V�������擾�����i�I�[�o�[���C�h�j
    COMMONLIBRARY_API virtual TIMEDATA GetDateTimeProc(void);

	// ���O�o�͐��䏈��
	void LogOutputControlProc(
        const char *, 
        const char *, 
        int, 
        int, 
        int, 
        const char *, 
        ERRORCODETABLE eErrCd = tagErrorCode::SUCCESS);

// Private�����o�[�֐�
private:
	// �g���[�X���O�o�͏���
	void WriteTraceLogProc(
        const char *, 
        const char *, 
        int, 
        const char *, 
        const char *, 
        ERRORCODETABLE eErrCd = tagErrorCode::SUCCESS);

	// �f�[�^���O�o�͏���
	void WriteDataLogProc(const char *, const char *, int, const char *);
};
#ifdef __cplusplus
}
#endif // __cplusplus
#endif // _CLOGFILE_H_
