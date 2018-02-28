/****************************************************************************/
/*! @file
@brief �Q�[���p�b�h�C���X�y�N�^

-----------------------------------------------------------------------------
	Copyright (C) 2008 Takenori Imoto. All rights reserved.
	( http://www.kaede-software.com/ )
-----------------------------------------------------------------------------
@author		T.Imoto
@date		2008/01/21
@note

�\�[�X�R�[�h�`���ł���o�C�i���`���ł���A�ύX�̗L���Ɋւ�炸�A�ȉ��̏�����
��������ɂ����āA�Ĕz�z����юg�p�������܂�:

   1. �\�[�X�R�[�h�`���ōĔz�z����ꍇ�A��L���쌠�\���A�{����������щ��L�ӔC
      ����K���K���܂߂Ă��������B
   2. �o�C�i���`���ōĔz�z����ꍇ�A��L���쌠�\���A�{����������щ��L�ӔC����
      �K����A�z�z���ƂƂ��ɒ񋟂���镶������ё��̎����ɕK���܂߂Ă��������B

�{�\�t�g�E�F�A�͒��쌠�҂ɂ���āA�h����̂܂܁h�񋟂������̂Ƃ��܂��B�{�\�t
�g�E�F�A�ɂ��ẮA�����َ����킸�A���p�i�Ƃ��Ēʏ킻�Ȃ���ׂ��i�������Ȃ�
�Ă���Ƃ̕ۏ؂��A����̖ړI�ɓK������Ƃ̕ۏ؂��܂߁A���̕ۏ؂��Ȃ���܂���B
���R�̂�������킸�A���Q�����̌�����������킸�A���A�ӔC�̍������_��ł�
�邩���i�ӔC�ł��邩 (�ߎ����̑�) �s�@�s�ׂł��邩���킸�A���쌠�҂͉��ɂ���
�悤�ȑ��Q����������\����m�炳��Ă����Ƃ��Ă��A�{�\�t�g�E�F�A�̎g�p���甭
���������ڑ��Q�A�Ԑڑ��Q�A�����I�ȑ��Q�A���ʑ��Q�A�����I���Q�܂��͌��ʑ��Q�̂�
����ɑ΂��Ă� (��֕i�܂��� �T�[�r�X�̒�;�g�p�@��A�f�[�^�܂��͗��v�̑�����
�⏞; �܂��́A�Ɩ��̒��f�ɑ΂���⏞���܂�)�ӔC���������������܂���B

*****************************************************************************/


#ifndef __GAME_PAD_DEVICE_INSPECTOR_H__
#define __GAME_PAD_DEVICE_INSPECTOR_H__

#include <string>
#include <windows.h>
#include <dinput.h>

namespace gamepad {


class CGamePadDeviceInspector
{
	DIDEVCAPS				device_capabilities_;
	DIDEVICEINSTANCE		device_instance_info_;

public:
	CGamePadDeviceInspector();
	bool GetInfomation( IDirectInputDevice8* device );

	// ���O�ɏڍ׏��������o��
	void PrintDescription() const;

	static bool GetCaps( IDirectInputDevice8* device, DIDEVCAPS& caps );
	static bool GetInstInfo( IDirectInputDevice8* device, DIDEVICEINSTANCE& info );

	// �C���X�^���X�𐶐������ɁA�f�o�C�X�̃|�C���^�����n���Ĕ��肷��
	static bool IsForceFeedbackDevice( IDirectInputDevice8* device );

	static bool IsForceFeedbackDevice( unsigned long flag ) {
		return (flag & DIDC_FORCEFEEDBACK) ? true : false;
	}

	//! �f�o�C�X�̃C���X�^���X�ɑ΂����ӂȎ��ʎq�B
	//! �A�v���P�[�V�����́A���̃C���X�^���X �O���[�o����ӎ��ʎq (GUID) ���\���t�@�C���ɕۑ����A��Ŏg�����Ƃ��ł���B
	//! �C���X�^���X GUID �́A�R���s���[�^���ƂɌŗL�ł���B
	//! 1 �̃R���s���[�^����擾�����C���X�^���X GUID �́A���̃R���s���[�^��̃C���X�^���X GUID �Ƃ͖��֌W�ł���B 
	GUID GetInstanceGuid() const { return device_instance_info_.guidInstance; }
	//! ���i�̈�ӂȎ��ʎq�B���̎��ʎq�́A�f�o�C�X ���[�J�[���ݒ肷��B 
	GUID GetProductGuid() const { return device_instance_info_.guidProduct; }
	//! �C���X�^���X�̓o�^���B���Ƃ��΁A"Joystick 1"�B 
	void GetInstanceName( std::string& name ) const { name = std::string(device_instance_info_.tszInstanceName); }
	//! ���i�̓o�^���B 
	void GetProductName( std::string& name ) const { name = std::string(device_instance_info_.tszProductName); }
	//! �t�H�[�X �t�B�[�h�o�b�N�Ɏg����h���C�o�̈�ӂȎ��ʎq�B�h���C�o�̃��[�J�[�����̎��ʎq��ݒ肷��B 
	GUID GetForceFeedbackDriverGuid() const { return device_instance_info_.guidFFDriver; }
	//! �f�o�C�X�� HID (Human Interface Device) �f�o�C�X�ł���ꍇ�A���̃����o�ɂ́AHID �g�p�y�[�W �R�[�h���܂܂��B 
	WORD GetUsagePage() const { return device_instance_info_.wUsagePage; }
	//! �f�o�C�X�� HID (Human Interface Device) �f�o�C�X�ł���ꍇ�A���̃����o�ɂ́AHID �g�p�R�[�h���܂܂��B
	WORD GetUsage() const { return device_instance_info_.wUsage; }


	//! �f�o�C�X�Ɋ֘A�t����ꂽ�t���O�B
	unsigned long GetDeviceFlag() const { return device_capabilities_.dwFlags; }
	//! �f�o�C�X �^�C�v�w��q�B���̃����o�́ADIDEVICEINSTANCE �\���̂� dwDevType �����o�Ɠ����Ȓl���i�[�ł���B 
	void GetDeviceType( int& main, int& sub ) { main = device_capabilities_.dwDevType&0xFF; sub = (device_capabilities_.dwDevType&0xFF00)>>8; }
	//! �f�o�C�X��Ŏg�p�\�Ȏ��̐�
	unsigned long GetNumberOfAxes() const { return device_capabilities_.dwAxes; }
	//! �f�o�C�X��Ŏg�p�\�ȃ{�^���̐�
	unsigned long GetNumberOfButtons() const { return device_capabilities_.dwButtons; }
	//! �f�o�C�X��Ŏg�p�\�Ȏ��_�R���g���[���̐�
	unsigned long GetNumberOfPOVs() const { return device_capabilities_.dwPOVs; }
	//! �����H�̃t�H�[�X �R�}���h��A�����čĐ�����ۂ̊Ԋu��\���A�}�C�N���b�P�ʂ̍ŏ�����
	unsigned long GetForceFeedbackSamplePeriod() const { return device_capabilities_.dwFFSamplePeriod; }
	//! �}�C�N���b�P�ʂ̃f�o�C�X�̕���\�̍ŏ����ԁB�f�o�C�X�́A���ׂĂ̎��Ԃ��ł��߂������l�Ɋۂ߂�B
	//! ���Ƃ��΁AdwFFMinTimeResolution �̒l�� 1000 �̏ꍇ�A�f�o�C�X�͂��ׂĂ̎��Ԃ��ł��߂��~���b�Ɋۂ߂�B
	unsigned long GetForceFeedbackMinTimeResolution() const { return device_capabilities_.dwFFMinTimeResolution; }
	//! �f�o�C�X�̃t�@�[���E�F�A ���r�W�����ԍ�
	unsigned long GetFirmwareRevisionNumber() const { return device_capabilities_.dwFirmwareRevision; }
	//! �f�o�C�X�̃n�[�h�E�F�A ���r�W�����ԍ�
	unsigned long GetHardwareRevisionNumber() const { return device_capabilities_.dwHardwareRevision; }
	//! �f�o�C�X �h���C�o�̃o�[�W�����ԍ�
	unsigned long GetDriverVersionNumber() const { return device_capabilities_.dwFFDriverVersion; }

	//! �e��̒l�����O�ɏ����o��
	void PrintDetail();

	static void GetDeviceFlagDesc( unsigned long flag, std::wstring& desc );
	static void GetDeviceTypeName( int main, int sub, std::wstring& mainname, std::wstring& subname );
};

}; // namespace gamepad

#endif	// __GAME_PAD_DEVICE_INSPECTOR_H__


