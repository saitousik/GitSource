#ifndef _CFILEOPERATION_H_
#define _CFILEOPERATION_H_
//============================================
// �t�@�C������N���X�w�b�_�[�t�@�C��
// CFileOperations.h
// 
// �@�\�T�v
// �t�@�C���𑀍삷������`����
// �����ł͋��ʑ���N���X���p�����Ďg�p����萔�y��API�̃v���g�^�C�v��錾����
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
#include "CSubOperations.h"

// ���N���X��`
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
class CFileOperations : 
    public CSubOperations
{
// Protected�����o�[�ϐ�
protected:
    // �g���[�X���O�o�̓t�@�C�����ϐ�
    string m_strFileOpeTraceLogFileName;

    // �f�[�^���O�o�̓t�@�C�����ϐ�
    string m_strFileOpeDataLogFileName;

// Public�����o�[�ϐ�
public:
    // �R�s�[�t�@�C�����ϐ�
    string m_strFileOpeCopyFileName;

// Public�����o�[�֐�
public:
    // �f�t�H���g�R���X�g���N�^
    COMMONLIBRARY_API CFileOperations();

    // �p�����[�^���w��R���X�g���N�^
    COMMONLIBRARY_API CFileOperations(const string, const string, const string);

    // �f�X�g���N�^
    COMMONLIBRARY_API virtual ~CFileOperations();

    // �t�@�C���R�s�[����
    int CopyFileProc(const char *, const char *);

    // �t�@�C�����ϊ�����
    int ChangeFileNameProc(const char *, const char *);

    // �t�@�C���폜����
    int DeleteFileProc(const char *);

    // �t�@�C����ԃ`�F�b�N����
    int CheckFileStatProc(const char *, struct stat *);

    // ��΃p�X�ϊ��t�@�C�����擾����
    string GetConvertFileNameProc(
        FILETYPETABLE,
        const char *,
        const char *,
        char *pcDirectoryName = (char *)"", 
        char *pcDefaultName = (char *)"", 
        int iBaseResult = EXIT_SUCCESS);

// Private�����o�[�֐�
private:
    // �t�@�C������������
    string CreateFileNameProc(int, const char *, const char *, int iResult = EXIT_SUCCESS);

    // ��΃p�X�擾����
    string GetFullPathProc(const char, const char *, const char *);
};
#ifdef __cplusplus
}
#endif // __cplusplus
#endif // _CFILEOPERATION_H_

