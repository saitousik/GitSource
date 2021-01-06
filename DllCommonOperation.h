#ifndef _DLLCOMMONOPERATION_H_
#define _DLLCOMMONOPERATION_H_
//============================================
// ���ʑ���@�\�w�b�_�[�t�@�C��
// DllCommonOperation.h
// 
// �@�\�T�v
// �@�\���T�|�[�g��������`����
// �����ł͎g�p����萔�y�у��b�Z�[�W�����`����
//
// ��`���e
// �P�D�C���N���[�h��`
// �Q�D�萔��`
// �R�D���ϐ���`
// �S�D���b�Z�[�W��`
// �T�D���������[�N��`
// �U�D���������[�N�̔����ꏊ��`
// �V�D�O���ւ̃��C�u�����Ăяo����`
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
#include "DllDataModel.h"
#include <time.h>
#include <errno.h>
#include <direct.h>
#include <locale.h>

// ���萔��`
#define CREATE_DATE_NAME 1
#define CREATE_TIME_NAME 2
#define LOG_TYPE_INFORMATION "Information"
#define LOG_TYPE_DATA "Data"
#define LOG_TYPE_ERROR "Error"
#define LOG_TYPE_SUCCESS "Success"
#define LOGARGS __FILE__, __FUNCTION__, __LINE__
#define PATH_SIZE 10
#define FILEEXTENSION_SIZE 6
#define APINAME_SIZE 64
#define LOG_TYPE_SIZE 14
#define PATHCHECK_SIZE_2BYTE 2
#define PATHCHECK_SIZE_3BYTE 3
#define DATA_SIGN 1
#define ERR_OFF 0
#define ERR_ON 1
#define CRLF_CODE 0x0D0A
#define DELIMITER "."
#define ACTIONSET 1
#define ERRORSET 2

// �����b�Z�[�W��`
// ���ُ탁�b�Z�[�W
#define ENVGETERROR "���ϐ���������擾�o���܂���B���ϐ���ݒ肵�Ă��邩���m�F���肢�v���܂��B"
#define NOTPARAMETER "�p�����[�^���w�肳��Ă���܂���B�ǂݍ��݃t�@�C�����͕K�{�ł��B"
#define PARAMETERERROR "�p�����[�^�w�肪�s���ł��B�w����e�����m�F���肢�v���܂��B"
#define FILECANNOTOPEN "�t�@�C�����I�[�v���o���܂���B�t�@�C���i�[�ꏊ�w��̂��m�F���肢�v���܂��B"
#define FILECANNOTCLOSE "�t�@�C�����N���[�Y�o���܂���B"
#define NOTDATA "�f�[�^������܂���B���̓f�[�^�̂���t�@�C�������w�肵�Ă��������B"
#define DATAINJUSTICE "�f�[�^���s���̂��ߏo�͏����͏o���܂���B���̃��R�[�h�͈󎚑ΏۊO�ɂȂ�܂��B"
#define DATAGETERROR "���擾�Ɏ��s���܂����B�f�[�^���擾�o���Ȃ������Ǝv���܂��B"
#define MEMORYENSUREERROR "�������̈���m�ۏo���܂���B"
#define FILECOPYERROR "�t�@�C���̃R�s�[�Ɏ��s���܂����B���炩�̌��������������Ǝv���܂��B"
#define FILERENAMEERROR "�t�@�C������ϊ��o���܂���B"
#define FILEDELETEERROR "�t�@�C�����폜�o���܂���B�t�@�C���̏�Ԃ����m�F���肢�v���܂��B"
#define FILESTATERROR "�t�@�C����Ԃ��擾�o���܂���B�Y���t�@�C�����ړ��������͍폜����Ă���\��������܂��B"
#define OPERATIONERROR "����Ɏ��s���܂����B"

// �����탁�b�Z�[�W
#define ALLREADEND "�S�Ẵf�[�^�擾���I���܂����B"
#define MAKEDIRSUCCESS "�t�H���_�𐶐����܂����B"
#define ALREADYEXISTS "�t�H���_�͊��ɐ����ς݂̂��߈����������̃t�H���_���g�p���܂��B"
#define MEMORYCREATESUCCESS "�������̈���m�ۏo���܂����B"
#define MEMORYRELEASESUCCESS "�������̈��������܂����B"
#define FILEOPENSUCCESS "�t�@�C�����I�[�v���o���܂����B"
#define FILECLOSESUCCESS "�t�@�C�����N���[�Y�o���܂����B"
#define FULLPATHDEFINITION "�t�@�C�������΃p�X�Ŏw�肵�Ă��邽�ߕҏW�����ɂ��̂܂܎g�p���܂��B"
#define GETSTATSUCCESS "�t�@�C���̏�Ԃ��擾�o���܂����B"
#define FILEREADSUCCESS "�t�@�C������̓ǂݍ��݂��o���܂����B"
#define FILEWRITESUCCESS "�t�@�C���ւ̏������݂��o���܂����B"
#define FILECOPYSUCCESS "�t�@�C���̃R�s�[���������܂����B"
#define RENAMESUCCESS "�t�@�C�����ɕύX�o���܂����B"
#define DELETESUCCESS "�t�@�C�����폜���܂����B"
#define DATACONTINUED "�ǂݍ��񂾃f�[�^���s���ł������������p�����܂��B"
#define ERRORPROCCONTINUED "�֐��ł̏����͎��s���܂������@�\�ɑ΂��钼�ڂ̉e���͂Ȃ����ߏ������p�����܂��B"
#define GETENVSUCCESS "���ϐ����擾�o���܂����̂Ŋ��ϐ��̃t�@�C�����ŏ�����i�߂܂��B"
#define GETNOTENVCONTINUED "���ϐ����擾�o���܂���ł����̂ŋ@�\�Œ�߂Ă���t�@�C�����ŏ�����i�߂܂��B"
#define GETPARAMSUCCESS "�p�����[�^�̃R�}���h��񂪎擾�o���܂����̂ŃR�}���h���̃t�@�C�����ŏ�����i�߂܂��B"

// �����������[�N��`
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#endif // _DEBUG

// ���������m�ے�`
#ifdef _DEBUG
#define DBG_NEW new (_NORMAL_BLOCK, __FILE__, __LINE__)
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif // _DEBUG
#ifdef _DEBUG
#define malloc(MMEMORY) _malloc_dbg(MMEMORY, _NORMAL_BLOCK, __FILE__, __LINE__)
#define calloc(MCOUNT, CMEMORY) _calloc_dbg(MCOUNT, CMEMORY, _NORMAL_BLOCK, __FILE__, __LINE__)
#endif // _DEBUG

// ���O���ւ̃��C�u�����Ăяo����`
#ifndef OperationsSupport_EXPORTS
#define COMMONLIBRARY_API __declspec(dllimport)
#else
#define COMMONLIBRARY_API __declspec(dllexport)
#endif // OperationsSupport_EXPORTS
#endif // _DLLCOMMONOPERATION_H_
