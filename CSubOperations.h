#ifndef _CSUBOPERATIONS_H_
#define _CSUBOPERATIONS_H_
//============================================
// ���ʑ���N���X�w�b�_�[�t�@�C��
// CSubOperations.h
// 
// �@�\�T�v
// �A�v���P�[�V�����������s���邽�߂̕K�v�ȋ@�\���`����
// �����ł̓��O�o�̓N���X���p�����Ďg�p����萔�y��API�̃v���g�^�C�v��錾����
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
// �e�v���g�^�C�v��`
//��������������������������������������������
// ���C���N���[�h��`
#pragma once
#include "CLogFile.h"

// ���N���X��`
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
class CSubOperations : 
    public CLogFile
{
// Protected�����o�[�ϐ�
protected:
    // �g���[�X���O�o�̓t�@�C�����ϐ�
    string m_strSubOpeTraceLogFileName;

    // �f�[�^���O�o�̓t�@�C�����ϐ�
    string m_strSubOpeDataLogFileName;

// Public�����o�[�֐�
public:
    // �f�t�H���g�R���X�g���N�^
    COMMONLIBRARY_API CSubOperations();

    // �p�����[�^���w��R���X�g���N�^
    COMMONLIBRARY_API CSubOperations(const string, const string);

    // �f�X�g���N�^
    COMMONLIBRARY_API virtual ~CSubOperations();

    // ���ϐ��擾����
    int GetEnvironmentDataProc(const char *, char *);

    // �}�V�������擾����
    COMMONLIBRARY_API TIMEDATA GetDateTimeProc(void);

    // �C�x���g���b�Z�[�W��������
    LOGINFORMATION CreateEventMessageProc(int, const char *, ERRORCODETABLE eErr = tagErrorCode::SUCCESS);

    // �w�蕶���폜����
    void DeleteDesignationCharProc(const char *, const char, char *);

    // �w�蕶���񕪊�����
    deque<string> SplitDataProc(const string, const char);

    // �}���`�o�C�g�����񃏃C�h������ϊ�����
    COMMONLIBRARY_API wstring MultiToWideProc(string);

    // ���C�h������}���`�o�C�g������ϊ�����
    COMMONLIBRARY_API string WideToMultiProc(wstring);

    // �R�}���h���C��������͏���
    COMMONLIBRARY_API vector<string> ParseArgsProc(string, int*);

// Private�����o�[�֐�
private:
    // �G���[���b�Z�[�W�擾����
    const char *GetErrorMessageProc(ERRORCODETABLE);
};
#ifdef __cplusplus
}
#endif // __cplusplus
#endif // _CSUBOPERATIONS_H_

