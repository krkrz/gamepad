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


#include "GamePadDeviceInspector.h"
#include "GamePadLog.h"
#include "GamePadUtil.h"
#include <sstream>

namespace gamepad {


CGamePadDeviceInspector::CGamePadDeviceInspector()
{
	ZeroMemory( &device_capabilities_, sizeof(device_capabilities_) );
	ZeroMemory( &device_instance_info_, sizeof(device_instance_info_) );
}

bool CGamePadDeviceInspector::GetInfomation( IDirectInputDevice8* device )
{
	if( !GetCaps( device, device_capabilities_ ) ) {
		return false;
	}

	if( !GetInstInfo( device, device_instance_info_ ) ) {
		return false;
	}

	return true;
}
bool CGamePadDeviceInspector::GetCaps( IDirectInputDevice8* device, DIDEVCAPS& caps )
{
	HRESULT hr;
	caps.dwSize = sizeof(DIDEVCAPS);
	if( FAILED( hr = device->GetCapabilities( &caps ) ) ) {
		Log( L"Failed to Call GetCapabilities.", hr );
		return false;
	}
	return true;
}
bool CGamePadDeviceInspector::GetInstInfo( IDirectInputDevice8* device, DIDEVICEINSTANCE& info )
{
	HRESULT hr;
	info.dwSize = sizeof(DIDEVICEINSTANCE);
	if( FAILED( hr = device->GetDeviceInfo( &info ) ) ) {
		Log( L"Failed to Call GetDeviceInfo.", hr );
		return false;
	}
	return true;
}
bool CGamePadDeviceInspector::IsForceFeedbackDevice( IDirectInputDevice8* device )
{
	DIDEVCAPS	caps;
	if( GetCaps( device, caps ) ) {
		return IsForceFeedbackDevice(caps.dwFlags);
	}
	return false;
}

//! �e��̒l�����O�ɏ����o��
void CGamePadDeviceInspector::PrintDetail()
{
	std::wstringstream mesStream;
	mesStream << L"Devie Detail :" << std::endl;

	std::string	productName;
	std::wstring	wproductName;
	GetProductName( productName );
	if( EncodeToUTF16( wproductName, productName ) ) {
		mesStream << L"Product Name : " << wproductName << std::endl;
	}
	int	main, sub;
	GetDeviceType( main, sub );
	std::wstring mainname, subname;
	GetDeviceTypeName( main, sub, mainname, subname );
	mesStream << L"Device Type Main : " << mainname << std::endl;
	mesStream << L"Device Type Sub : " << subname << std::endl;

	std::wstring desc;
	GetDeviceFlagDesc( GetDeviceFlag(), desc );
	mesStream << L"Devie Description : " << std::endl << desc << std::endl;

	mesStream << L"Number of Axes : " << GetNumberOfAxes() << std::endl;
	mesStream << L"Number of Buttons : " << GetNumberOfButtons() << std::endl;
	mesStream << L"Number of POV : " << GetNumberOfPOVs() << std::endl;
	mesStream << L"Force Feedback Sample Period : " << GetForceFeedbackSamplePeriod() << L" [msec]" << std::endl;
	mesStream << L"Force Feedback Min Time Resolution : " << GetForceFeedbackMinTimeResolution() << L" [msec]" << std::endl;
	mesStream << L"Firmware Revision : " << GetFirmwareRevisionNumber() << std::endl;
	mesStream << L"Hardware Revision : " << GetHardwareRevisionNumber() << std::endl;
	mesStream << L"Driver Version : " << GetDriverVersionNumber() << std::endl;

	Log( mesStream.str().c_str() );
}

void CGamePadDeviceInspector::GetDeviceFlagDesc( unsigned long flag, std::wstring& desc )
{
	desc.clear();

	if( flag & DIDC_ALIAS ) {
		desc += L"�f�o�C�X�́A�ʂ� DirectInput �f�o�C�X�̕����ł���B\n";
	}

	if( flag & DIDC_ATTACHED ) {
		desc += L"�f�o�C�X�́A�����I�ɃA�^�b�`����Ă���B\n";
	}

	if( flag & DIDC_DEADBAND ) {
		desc += L"�f�o�C�X�́A���Ȃ��Ƃ� 1 �̃t�H�[�X �t�B�[�h�o�b�N�����Ńf�b�h�o���h���T�|�[�g���Ă���B\n";
	}

	if( flag & DIDC_EMULATED ) {
		desc += L"���̃t���O���ݒ肳���ƁA�f�[�^�� HID (Human Interface Device) �Ȃǂ̃��[�U�[ ���[�h �f�o�C�X �C���^�[�t�F�C�X����A�܂��͑��̃����O 3 �̎�i�ɂ����͂����B";
		desc += L"�t���O���ݒ肳��Ȃ��ꍇ�A�f�[�^�̓J�[�l�� ���[�h �h���C�o���璼�ړ��͂����B\n";
	}

	if( flag & DIDC_FORCEFEEDBACK ) {
		desc += L"�f�o�C�X�́A�t�H�[�X �t�B�[�h�o�b�N���T�|�[�g����B\n";
	}

	if( flag & DIDC_FFFADE ) {
		desc += L"�t�H�[�X �t�B�[�h�o�b�N �V�X�e���́A���Ȃ��Ƃ� 1 �̃G�t�F�N�g�ɑ΂���t�F�[�h �p�����[�^���T�|�[�g���Ă���B\n";
	}

	if( flag & DIDC_FFATTACK ) {
		desc += L"�t�H�[�X �t�B�[�h�o�b�N �V�X�e���́A���Ȃ��Ƃ� 1 �̃G�t�F�N�g�ɑ΂���A�^�b�N �p�����[�^���T�|�[�g���Ă���B\n";
	}

	if( flag & DIDC_HIDDEN ) {
		desc += L"�f�o�C�X �h���C�o���L�[�{�[�h �C�x���g��}�E�X �C�x���g�𐶐��ł���悤�ɍ쐬���ꂽ���z�f�o�C�X�B\n";
	}

	if( flag & DIDC_PHANTOM ) {
		desc += L"�v���[�X�z���_�B���z�f�o�C�X�́A�f�t�H���g�ł� IDirectInput8::EnumDevices �ɂ���ė񋓂���Ȃ��B\n";
	}

	if( flag & DIDC_POLLEDDATAFORMAT ) {
		desc += L"����̃f�[�^�`���ŏ��Ȃ��Ƃ� 1 �̃I�u�W�F�N�g���A���荞�݋쓮�ł͂Ȃ��A�|�[�����O�����B\n";
	}

	if( flag & DIDC_POLLEDDEVICE ) {
		desc += L"�f�o�C�X��̏��Ȃ��Ƃ� 1 �̃I�u�W�F�N�g���A���荞�݋쓮�ł͂Ȃ��A�|�[�����O�����B\n";
	}

	if( flag & DIDC_POSNEGCOEFFICIENTS ) {
		desc += L"�t�H�[�X �t�B�[�h�o�b�N �V�X�e���́A���Ȃ��Ƃ� 1 �̏����ɑ΂��ď����� 2 �̌W���l���T�|�[�g���� (����́A���̐��ړ��A�����́A���̕��ړ�)�B";
		desc += L"�f�o�C�X������̌W�������T�|�[�g���Ă��Ȃ��ꍇ�ADICONDITION �\���̂̕��̌W���͖��������B\n";
	}

	if( flag & DIDC_POSNEGSATURATION ) {
		desc += L"�t�H�[�X �t�B�[�h�o�b�N �V�X�e���́A���Ȃ��Ƃ� 1 �̏����ɑ΂��āA�����̗����̃t�H�[�X�o�͂ɑ΂���ő�O�a���T�|�[�g����B";
		desc += L"�f�o�C�X������̖O�a�l�����T�|�[�g���Ă��Ȃ��ꍇ�ADICONDITION �\���̂̕��̖O�a�l�͖��������B\n";
	}

	if( flag & DIDC_SATURATION ) {
		desc += L"�t�H�[�X �t�B�[�h�o�b�N �V�X�e���́A���Ȃ��Ƃ� 1 �̏����ɑ΂��ď����G�t�F�N�g�̖O�a���T�|�[�g����B";
		desc += L"�f�o�C�X���O�a���T�|�[�g���Ă��Ȃ��ꍇ�A��������ɂ�萶�������t�H�[�X�́A�f�o�C�X�������ł���ő�t�H�[�X�ɂ���Ă̂ݐ��������B\n";
	}

	if( flag & DIDC_STARTDELAY ) {
		desc += L"�t�H�[�X �t�B�[�h�o�b�N �V�X�e���́A���Ȃ��Ƃ� 1 �̃G�t�F�N�g�ɑ΂���x���p�����[�^���T�|�[�g����B";
		desc += L"�f�o�C�X���J�n�x�����T�|�[�g���Ă��Ȃ��ꍇ�ADIEFFECT �\���̂� dwStartDelay �����o�͖��������B\n";
	}
}

void CGamePadDeviceInspector::GetDeviceTypeName( int main, int sub, std::wstring& mainname, std::wstring& subname )
{
	switch( main ) {
	case DI8DEVTYPE_1STPERSON:
		mainname = L"��l�̌^�̃A�N�V���� �Q�[�� �f�o�C�X";
		switch( sub ) {
		case DI8DEVTYPE1STPERSON_LIMITED:
			subname = L"�A�N�V���� �}�b�v�Ώۂ̃f�o�C�X �I�u�W�F�N�g�̍ŏ������w�肵�Ȃ��f�o�C�X";
			break;
		case DI8DEVTYPE1STPERSON_SHOOTER:
			subname = L"��l�̌^�̃V���[�e�B���O �Q�[���p�ɐ݌v���ꂽ�f�o�C�X";
			break;
		case DI8DEVTYPE1STPERSON_SIXDOF:
			subname = L"6 �i�K (3 �̉��������� 3 �̉�]��) �̎��R��������f�o�C�X";
			break;
		case DI8DEVTYPE1STPERSON_UNKNOWN:
		default:
			subname = L"�s���ȃT�u�^�C�v";
			break;
		}
		break;
	case DI8DEVTYPE_DEVICE:
		mainname = L"�ʂ̃J�e�S���ɕ��ނ���Ȃ��f�o�C�X";
		subname = L"�ʂ̃J�e�S���ɕ��ނ���Ȃ��f�o�C�X";
		break;
	case DI8DEVTYPE_DEVICECTRL:
		mainname = L"�A�v���P�[�V�����̃R���e�L�X�g����ʃ^�C�v�̃f�o�C�X�̐���Ɏg������̓f�o�C�X";
		switch( sub ) {
		case DI8DEVTYPEDEVICECTRL_COMMSSELECTION:
			subname = L"�ʐM�̑I���Ɏg����R���g���[��";
			break;
		case DI8DEVTYPEDEVICECTRL_COMMSSELECTION_HARDWIRED:
			subname = L"�f�t�H���g�̍\�����g���K�v������A�}�b�s���O��ύX�ł��Ȃ��f�o�C�X";
			break;
		case DI8DEVTYPEDEVICECTRL_UNKNOWN:
		default:
			subname = L"�s���ȃT�u�^�C�v";
			break;
		}
		break;
	case DI8DEVTYPE_DRIVING:
		mainname = L"�X�e�A�����O�p�f�o�C�X";
		switch( sub ) {
		case DI8DEVTYPEDRIVING_COMBINEDPEDALS:
			subname = L"�P��̎�����A�N�Z���ƃu���[�L �y�_���̒l��񍐂���X�e�A�����O �f�o�C�X";
			break;
		case DI8DEVTYPEDRIVING_DUALPEDALS:
			subname = L"�ʁX�̎�����A�N�Z���ƃu���[�L �y�_���̒l��񍐂���X�e�A�����O �f�o�C�X";
			break;
		case DI8DEVTYPEDRIVING_HANDHELD:
			subname = L"�n���h�w���h�̃X�e�A�����O �f�o�C�X";
			break;
		case DI8DEVTYPEDRIVING_LIMITED:
			subname = L"�A�N�V���� �}�b�v�Ώۂ̃f�o�C�X �I�u�W�F�N�g�̍ŏ������w�肵�Ȃ��X�e�A�����O �f�o�C�X";
			break;
		case DI8DEVTYPEDRIVING_THREEPEDALS:
			subname = L"�ʁX�̎�����A�N�Z���A�u���[�L�A����уN���b�` �y�_���̒l��񍐂���X�e�A�����O �f�o�C�X";
			break;
		default:
			subname = L"�s���ȃT�u�^�C�v";
			break;
		}
		break;
	case DI8DEVTYPE_FLIGHT:
		mainname = L"�t���C�g �V�~�����[�V�����p�R���g���[��";
		switch( sub ) {
		case DI8DEVTYPEFLIGHT_LIMITED:
			subname = L"�A�N�V���� �}�b�v�Ώۂ̃f�o�C�X �I�u�W�F�N�g�̍ŏ������w�肵�Ȃ��t���C�g �R���g���[��";
			break;
		case DI8DEVTYPEFLIGHT_RC:
			subname = L"�͌^��s�@�̃����[�g �R���g���[���Ɋ�Â��t���C�g �f�o�C�X";
			break;
		case DI8DEVTYPEFLIGHT_STICK:
			subname = L"�W���C�X�e�B�b�N";
			break;
		case DI8DEVTYPEFLIGHT_YOKE:
			subname = L"���c��";
			break;
		default:
			subname = L"�s���ȃT�u�^�C�v";
			break;
		}
		break;
	case DI8DEVTYPE_GAMEPAD:
		mainname = L"�Q�[���p�b�h";
		switch( sub ) {
		case DI8DEVTYPEGAMEPAD_LIMITED:
			subname = L"�A�N�V���� �}�b�v�Ώۂ̃f�o�C�X �I�u�W�F�N�g�̍ŏ������w�肵�Ȃ��Q�[���p�b�h";
			break;
		case DI8DEVTYPEGAMEPAD_STANDARD:
			subname = L"�A�N�V���� �}�b�v�Ώۂ̃f�o�C�X �I�u�W�F�N�g�̍ŏ������w�肵�Ȃ��W���Q�[���p�b�h";
			break;
		case DI8DEVTYPEGAMEPAD_TILT:
			subname = L"�R���g���[���̎p������ x ���� y ���̃f�[�^��񍐂���Q�[���p�b�h";
			break;
		default:
			subname = L"�s���ȃT�u�^�C�v";
			break;
		}
		break;
	case DI8DEVTYPE_JOYSTICK:
		mainname = L"�W���C�X�e�B�b�N";
		switch( sub ) {
		case DI8DEVTYPEJOYSTICK_LIMITED:
			subname = L"�A�N�V���� �}�b�v�Ώۂ̃f�o�C�X �I�u�W�F�N�g�̍ŏ������w�肵�Ȃ��W���C�X�e�B�b�N";
			break;
		case DI8DEVTYPEJOYSTICK_STANDARD:
			subname = L"�A�N�V���� �}�b�v�Ώۂ̃f�o�C�X �I�u�W�F�N�g�̍ŏ������w�肷��W���W���C�X�e�B�b�N";
			break;
		default:
			subname = L"�s���ȃT�u�^�C�v";
			break;
		}
		break;
	case DI8DEVTYPE_KEYBOARD:
		mainname = L"�L�[�{�[�h�܂��̓L�[�{�[�h�ގ��f�o�C�X";
		switch( sub ) {
		case DI8DEVTYPEKEYBOARD_UNKNOWN:
			subname = L"�T�u�^�C�v�͊m��ł��Ȃ�";
			break;
		case DI8DEVTYPEKEYBOARD_PCXT:
			subname = L"IBM PC/XT 83 �L�[ �L�[�{�[�h";
			break;
		case DI8DEVTYPEKEYBOARD_OLIVETTI:
			subname = L"Olivetti 102 �L�[ �L�[�{�[�h";
			break;
		case DI8DEVTYPEKEYBOARD_PCAT:
			subname = L"IBM PC/AT 84 �L�[ �L�[�{�[�h";
			break;
		case DI8DEVTYPEKEYBOARD_PCENH:
			subname = L"IBM PC �g�� 101/102 �L�[���L�[�{�[�h�܂��� Microsoft NaturalR keyboard �L�[�{�[�h";
			break;
		case DI8DEVTYPEKEYBOARD_NOKIA1050:
			subname = L"Nokia 1050 �L�[�{�[�h";
			break;
		case DI8DEVTYPEKEYBOARD_NOKIA9140:
			subname = L"Nokia 9140 �L�[�{�[�h";
			break;
		case DI8DEVTYPEKEYBOARD_NEC98:
			subname = L"���{�� NEC PC98 �L�[�{�[�h";
			break;
		case DI8DEVTYPEKEYBOARD_NEC98LAPTOP:
			subname = L"���{�� NEC PC98 ���b�v�g�b�v �L�[�{�[�h";
			break;
		case DI8DEVTYPEKEYBOARD_NEC98106:
			subname = L"���{�� NEC PC98 106 �L�[ �L�[�{�[�h";
			break;
		case DI8DEVTYPEKEYBOARD_JAPAN106:
			subname = L"���{�� 106 �L�[ �L�[�{�[�h";
			break;
		case DI8DEVTYPEKEYBOARD_JAPANAX:
			subname = L"���{�� AX �L�[�{�[�h";
			break;
		case DI8DEVTYPEKEYBOARD_J3100:
			subname = L"���{�� J3100 �L�[�{�[�h";
			break;
		default:
			subname = L"�s���ȃT�u�^�C�v";
			break;
		}
		break;
	case DI8DEVTYPE_MOUSE:
		mainname = L"�}�E�X�܂��̓}�E�X�ގ��f�o�C�X (�g���b�N�{�[���Ȃ�)";
		switch( sub ) {
		case DI8DEVTYPEMOUSE_ABSOLUTE:
			subname = L"��Ύ��f�[�^��Ԃ��}�E�X";
			break;
		case DI8DEVTYPEMOUSE_FINGERSTICK:
			subname = L"�t�B���K�[�X�e�B�b�N";
			break;
		case DI8DEVTYPEMOUSE_TOUCHPAD:
			subname = L"�^�b�`�p�b�h";
			break;
		case DI8DEVTYPEMOUSE_TRACKBALL:
			subname = L"�g���b�N�{�[��";
			break;
		case DI8DEVTYPEMOUSE_TRADITIONAL:
			subname = L"�]���^�}�E�X";
			break;
		case DI8DEVTYPEMOUSE_UNKNOWN:
			subname = L"�T�u�^�C�v�͊m��ł��Ȃ�";
			break;
		default:
			subname = L"�s���ȃT�u�^�C�v";
			break;
		}
		break;
	case DI8DEVTYPE_REMOTE:
		mainname = L"�����[�g �R���g���[�� �f�o�C�X";
		switch( sub ) {
		case DI8DEVTYPEREMOTE_UNKNOWN:
			subname = L"�T�u�^�C�v�͊m��ł��Ȃ�";
			break;
		default:
			subname = L"�s���ȃT�u�^�C�v";
			break;
		}
		break;
	case DI8DEVTYPE_SCREENPOINTER:
		mainname = L"�X�N���[�� �|�C���^";
		switch( sub ) {
		case DI8DEVTYPESCREENPTR_UNKNOWN:
			subname = L"�s���ȃT�u�^�C�v";
			break;
		case DI8DEVTYPESCREENPTR_LIGHTGUN:
			subname = L"���C�g �K��";
			break;
		case DI8DEVTYPESCREENPTR_LIGHTPEN:
			subname = L"���C�g �y��";
			break;
		case DI8DEVTYPESCREENPTR_TOUCH:
			subname = L"�^�b�` �X�N���[��";
			break;
		default:
			subname = L"�s���ȃT�u�^�C�v";
			break;
		}
		break;
	case DI8DEVTYPE_SUPPLEMENTAL:
		mainname = L"�z�C�[���Ƌ��Ɏg���y�_���ȂǁA�A�v���P�[�V�����̃��C�� �R���g���[���ɓK���Ȃ��@�\�����������f�o�C�X";
		switch( sub ) {
		case DI8DEVTYPESUPPLEMENTAL_2NDHANDCONTROLLER:
			subname = L"2 ���I�n���h�w���h �R���g���[��";
			break;
		case DI8DEVTYPESUPPLEMENTAL_COMBINEDPEDALS:
			subname = L"��@�\�Ƃ��āA�P��̎�����A�N�Z���ƃu���[�L �y�_���̒l��񍐂���f�o�C�X";
			break;
		case DI8DEVTYPESUPPLEMENTAL_DUALPEDALS:
			subname = L"��@�\�Ƃ��āA�ʁX�̎�����A�N�Z���ƃu���[�L �y�_���̒l��񍐂���f�o�C�X";
			break;
		case DI8DEVTYPESUPPLEMENTAL_HANDTRACKER:
			subname = L"��̈ړ���ǐՂ���f�o�C�X";
			break;
		case DI8DEVTYPESUPPLEMENTAL_HEADTRACKER:
			subname = L"���̈ړ���ǐՂ���f�o�C�X";
			break;
		case DI8DEVTYPESUPPLEMENTAL_RUDDERPEDALS:
			subname = L"�����ǃy�_����������f�o�C�X";
			break;
		case DI8DEVTYPESUPPLEMENTAL_SHIFTER:
			subname = L"������M�A�I����񍐂���f�o�C�X";
			break;
		case DI8DEVTYPESUPPLEMENTAL_SHIFTSTICKGATE:
			subname = L"�{�^����Ԃ���M�A�I����񍐂���f�o�C�X";
			break;
		case DI8DEVTYPESUPPLEMENTAL_SPLITTHROTTLE:
			subname = L"��@�\�Ƃ��āA2 �ȏ�̃X���b�g���l��񍐂���f�o�C�X�B���̑��̃R���g���[�������ꍇ������";
			break;
		case DI8DEVTYPESUPPLEMENTAL_THREEPEDALS:
			subname = L"��@�\�Ƃ��āA�ʁX�̎�����A�N�Z���A�u���[�L�A����уN���b�` �y�_���̒l��񍐂���f�o�C�X";
			break;
		case DI8DEVTYPESUPPLEMENTAL_THROTTLE:
			subname = L"��@�\�Ƃ��āA�P��̃X���b�g���l��񍐂���f�o�C�X�B���̑��̃R���g���[�������ꍇ������";
			break;
		case DI8DEVTYPESUPPLEMENTAL_UNKNOWN:
		default:
			subname = L"�s���ȃT�u�^�C�v";
			break;
		}
		break;
	default:
		mainname = L"�s���ȃf�o�C�X";
		subname = L"�s���ȃT�u�^�C�v";
	}
}


}; // namespace gamepad

