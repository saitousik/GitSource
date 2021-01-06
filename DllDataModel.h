#ifndef _DLLDATAMODEL_H_
#define _DLLDATAMODEL_H_
//============================================
// �\���̓���`�w�b�_�[�t�@�C��
// DllDataModel.h
// 
// �@�\�T�v
// �e�@�\�Ŏg�p����\���̓��̃f�[�^���f����`
// 
// 
// ��`���e
// �P�D�C���N���[�h��`
// �Q�D���O��Ԏw��
// �R�D������񌅐���`
// �S�D���̑������E�萔��`
// �T�D���ʃ}�N����`
// �U�D�񋓌^�錾
// �V�D�\���̐錾
//
// �쐬����
// 2020/7/25  s-saito   �V�K�쐬
// 
// �X�V����
// 
// 
// 
// �o�[�W����
//                         ver.1.0
// �⑫����
// �E���w�b�_�[���ɂ��Ă͐�������@�\�ɑ΂���
// �@���ʂƂ��Ĉ�����`���L�ڂ���
// 
// 
//============================================
//��������������������������������������������
// �e�񋓌^�E�\���̒�`
//��������������������������������������������
// ���C���N���[�h��`
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <crtdbg.h>
#include <vector>
#include <deque>
#include <sstream>
#include <map>

// �����O��Ԏw��
using namespace std;

// ��������񌅐���`
#define TIMEDATA_YEAR 4
#define TIMEDATA_MONTH 2
#define TIMEDATA_DAY 2
#define TIMEDATA_DAYOFWEEK 3
#define TIMEDATA_HOUR 2
#define TIMEDATA_MINITE 2
#define TIMEDATA_SECOND 2
#define TIMEDATA_MSECOND 6

// �����̑������E�萔��`
#define LOGDATA 2048
#define DATASIZE 1024
#define MESSAGESIZE 1024
#define FILENAMESIZE _MAX_PATH + FILENAME_MAX

// �����ʃ}�N����`
#define SAFE_FREE(pMemory) free(pMemory); pMemory = NULL;
#define SAFE_FREE_DEBUG(pMemory) _freea_dbg(pMemory, _NORMAL_BLOCK); pMemory = NULL;
#define SAFE_FREE_NEWALLOC(pMemory) delete pMemory; pMemory = NULL;
#define SAFE_FREE_NEWALLOC_ARRAY(pMemory) delete[] pMemory; pMemory = NULL;

// ���񋓌^�錾
// �G���[�R�[�h
typedef enum class tagErrorCode
{
    SUCCESS = 0,									// ����I��
    ENVERROR = 101,									// ���ϐ��G���[
    NOPARAM,										// �p�����[�^�w��Ȃ�
    FAILPARAM,										// �p�����[�^�s��
    FILEOPENERROR,									// �t�@�C���I�[�v���G���[
    FILECLOSEERROR,									// �t�@�C���N���[�Y�G���[
    NODATA,											// �f�[�^�Ȃ�
    DATAFRAUD,										// �f�[�^�s��
    DATAERROR,										// �f�[�^�擾�G���[
    MEMORYERROR,									// �������̈�m�ۃG���[
    COPYERROR,										// �t�@�C���R�s�[�G���[
    RENAMEERROR,									// �t�@�C�����ϊ��G���[
    DELETEERROR,									// �폜�G���[
    STATERROR,										// ��Ԏ擾�G���[
    FAILERROR										// ���̑��G���[
}ERRORCODETABLE;

// �G���[�߂�l
enum
{
    CM_ENVERROR = 101,								// ���ϐ��G���[
    CM_NOPARAM,										// �p�����[�^�w��Ȃ�
    CM_PARAMERROR,									// �p�����[�^�s��
    CM_FILEOPENERROR,								// �t�@�C���I�[�v���G���[
    CM_FILECLOSEERROR,								// �t�@�C���N���[�Y�G���[
    CM_NODATA,										// �f�[�^�Ȃ�
    CM_DATAFRAUD,									// �f�[�^�s��
    CM_DATAERROR,									// �f�[�^�擾�G���[
    CM_MEMORYERROR,									// �������̈�m�ۃG���[
    CM_FILECOPYERROR,								// �t�@�C���R�s�[�G���[
    CM_RENAMEERROR,									// �t�@�C�����ϊ��G���[
    CM_FILEDELETEERROR,								// �폜�G���[
    CM_FILESTATERROR,								// ��Ԏ擾�G���[
    CM_FAILERROR									// ���̑��G���[
};

// �t�@�C�����ϊ��^�C�v
typedef enum class tagFileType
{
    NORMAL,											// �����l
    FILETYPE1,										// �f�B���N�g�����w�肠�芎�t�@�C�����ύX�Ȃ��i���s�t�H���_�Ɠ��K�w�j
    FILETYPE2,										// �f�B���N�g�����w�肠�芎�t�@�C�����ύX����i���s�t�H���_�Ɠ��K�w�j
    FILETYPE3,										// �f�B���N�g�����w�肠�芎�t�@�C�����ύX����i���s�t�H���_�̂P�K�w��j
    FILETYPE4,										// �f�B���N�g�����w�肠�芎�t�@�C�����ύX����i���s�t�H���_�̂P�K�w��j
    FILETYPE5,										// �f�B���N�g�����w�肠�芎�t�@�C�����ύX�Ȃ��i���s�t�H���_�̂P�K�w��j
    FILETYPE6,										// �f�B���N�g�����w��Ȃ����t�@�C�����ύX����
    FILETYPE7,										// �f�B���N�g�����w��Ȃ����t�@�C�����ύX�Ȃ��i�x�[�X��΃p�X�ݒ�j
    FILETYPE8,										// �f�B���N�g�����w��Ȃ����t�@�C�����ύX�Ȃ��i�R�}���h���ݒ�j
    FILETYPE9										// �f�B���N�g�����w�肠�芎�t�@�C�����ύX����i���s�t�H���_�Ɠ��K�w�j
}FILETYPETABLE;

// �o�̓��O���ʎq
enum
{
    TRACEOUTPUTLOG = 1,								// �g���[�X���O
	DATAOUTPUTLOG									// �f�[�^���O
};

// �g���[�X���O���ʎq
enum
{
    NOTRACE,										// �f�[�^���O���
    DATAINFORMATION,								// �f�[�^���
    ERRORINFORMATION1,								// �G���[���
    ERRORINFORMATION2,								// �G���[���b�Z�[�W���
    SUCCESSINFORMATION,								// �������
    NORMALINFORMATION								// �C���t�H���[�V����
};

// ���\���̐錾
// ���O�o�͏��\����
typedef struct tagLogInfo
{
    string strDateTime;                                         // ���O�o�͓���
    string strActionName;				                        // �������́^�t�@�C������
    char *pcActionName = NULL;				                    // �������́^�t�@�C�����̃|�C���^
    string strMessage;						                    // ���O�o�̓��b�Z�[�W
    char *pcMessage = NULL;						                // ���O�o�̓��b�Z�[�W�|�C���^
	ERRORCODETABLE eErrorCode = tagErrorCode::SUCCESS;          // �G���[�R�[�h
}LOGINFORMATION;

// �������\����
typedef struct tagTimeData
{
    string strMachDate;				                 // �}�V�����t
    string strMachTime;				                 // �}�V������
    string strMachDateTime;		                     // �}�V�������i���O�o�͗p�j
    string strMachDatePlus;		                     // �}�V�����t�i�L���t���j
    string strMachTimePlus;		                     // �}�V�������i�L���t���j
}TIMEDATA;
#endif // _DLLDATAMODEL_H_
