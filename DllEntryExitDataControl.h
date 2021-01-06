#ifndef _DLLENTRYEXITDATACONTROL_H_
#define _DLLENTRYEXITDATACONTROL_H_
//============================================
// ���ގ��󋵈󎚐���w�b�_�[�t�@�C��
// DllEntryExitDataControl.h
// 
// �@�\�T�v
// �t���A�̃Z�L�����e�B�ɂ��h�A�̓��ގ��t�@�C����ǂݍ��݁A
// �e�L�X�g�������͒��[�`���ň󎚂���
// ���@�\�Ŏg�p����萔�y��API�̃v���g�^�C�v��錾����
//
// ��`���e
// �P�D�C���N���[�h��`
// �Q�D�O���Q�ƒ�`
// �R�D�v���g�^�C�v�錾
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
// ���C���N���[�h��`
#pragma once
#include "CRecordOperations.h"
#include "DllCommonOperation.h"

// ���O���Q�ƕϐ�
extern ENTRYEXITDATALIBRARY_API int g_iParseArgv;
extern ENTRYEXITDATALIBRARY_API vector<string> g_vecParseArgc;

// ���v���g�^�C�v�錾
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
// ���ގ��󋵈󎚐��䃁�C������
ENTRYEXITDATALIBRARY_API int DataControlMain(int, vector<string>);
#ifdef __cplusplus
}
#endif // __cplusplus

// ���ގ��󋵈󎚐���J�n����
int DataControlStartProc(int, vector<string>, FILE **, FILE **, int*);

// ���ގ��󋵈󎚐���I������
int DataControlEndProc(FILE **, FILE **, vector<string>, int);

// CRT���������m�C�x���g�n���h������
int __cdecl MemoryLeakDookProc(int, char *, int *);
#endif // _DLLENTRYEXITDATACONTROL_H_
