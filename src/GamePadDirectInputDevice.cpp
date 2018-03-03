/****************************************************************************/
/*! @file
@brief 

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

#include "GamePadDirectInputDevice.h"
#include "GamePadDirectInputMappingDB.h"
#include "GamePadUtil.h"
#include "GamePadLog.h"
#include "GamePadInputDevicePort.h"
#include <sstream>
#include <string>
#include <vector>
#include <windows.h>

#define _USE_MATH_DEFINES 
#include <cmath> 

namespace gamepad {


CDirectInputDevice::CDirectInputDevice( IDirectInputDevice8* input, CInputDevicePort* port)
 : CInputDeviceBase(port), device_(input), device_creating_(false), request_poll_method_(false)
{
	ClearState();
	ClearKeyMap();
}

// �t�H�[�X�t�B�[�h�o�b�O�����
BOOL CALLBACK CDirectInputDevice::EnumObjectsCallback( const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext )
{
	return reinterpret_cast<CDirectInputDevice*>(pContext)->EnumObjectsCallback( pdidoi ) ? DIENUM_CONTINUE : DIENUM_STOP;
}

bool CDirectInputDevice::EnumObjectsCallback( const DIDEVICEOBJECTINSTANCE* pdidoi )
{
	HRESULT hr;

	// �I�u�W�F�N�g�������o��
	std::wstring	objName;
	if( EncodeToUTF16( objName, std::string(pdidoi->tszName) ) ) {
		Log( (std::wstring(L"[ ") + objName + std::wstring(L" ]")).c_str() );
	}

	// �^�C�v�`�F�b�N
	std::wstringstream typeStr;
	typeStr << L"Type : ";
	if( pdidoi->guidType == GUID_Button ) {
		enable_objs_.button++;
		typeStr << L"Button";
	} else if( pdidoi->guidType == GUID_XAxis ) {
		enable_objs_.axis |= DirectInputObjects::AXIS_X;
		typeStr << L"X - Axis";
	} else if( pdidoi->guidType == GUID_YAxis ) {
		enable_objs_.axis |= DirectInputObjects::AXIS_Y;
		typeStr << L"Y - Axis";
	} else if( pdidoi->guidType == GUID_ZAxis ) {
		enable_objs_.axis |= DirectInputObjects::AXIS_Z;
		typeStr << L"Z - Axis";
	} else if( pdidoi->guidType == GUID_RxAxis ) {
		enable_objs_.axis |= DirectInputObjects::AXIS_ROT_X;
		typeStr << L"Rot - X - Axis";
	} else if( pdidoi->guidType == GUID_RyAxis ) {
		enable_objs_.axis |= DirectInputObjects::AXIS_ROT_Y;
		typeStr << L"Rot - Y - Axis";
	} else if( pdidoi->guidType == GUID_RzAxis ) {
		enable_objs_.axis |= DirectInputObjects::AXIS_ROT_Z;
		typeStr << L"Rot - Z - Axis";
	} else if( pdidoi->guidType == GUID_POV ) {
		enable_objs_.pov++;
		typeStr << L"POV";
	} else if( pdidoi->guidType == GUID_Slider ) {
		enable_objs_.slider++;
		typeStr << L"Slider";
	} else if( pdidoi->guidType == GUID_Key ) {
		typeStr << L"Key";
	} else if( pdidoi->guidType == GUID_Unknown ) {
		typeStr << L"Unknown";
	} else {
		typeStr << L"Invalid";
	}
	Log( typeStr.str().c_str() );

	// �^�C�v�����̎��A�ő�l�ƍŏ��l��ݒ肷��B���͈̔͂́AXInput �͈̔͂ɂ��킹�Ă���B
	// Logitech RamblePad 2 �́A���͈̔͂ł����Ȃ�
	if( pdidoi->dwType & DIDFT_AXIS ) {
		DIPROPRANGE diprg;
		diprg.diph.dwSize		= sizeof(DIPROPRANGE);
		diprg.diph.dwHeaderSize	= sizeof(DIPROPHEADER);
		diprg.diph.dwHow		= DIPH_BYID;
		diprg.diph.dwObj		= pdidoi->dwType;
		diprg.lMin				= AXIS_RANGE_MIN;
		diprg.lMax				= AXIS_RANGE_MAX;

		if( FAILED( hr = device_->SetProperty( DIPROP_RANGE, &diprg.diph ) ) ) {
			Log( L"Failed to Call SetProperty - DIPROP_RANGE.", hr );
			return false;
		}

		ZeroMemory( &diprg, sizeof(diprg) );
		diprg.diph.dwSize		= sizeof(DIPROPRANGE);
		diprg.diph.dwHeaderSize	= sizeof(DIPROPHEADER);
		diprg.diph.dwHow		= DIPH_BYID;
		diprg.diph.dwObj		= pdidoi->dwType;
		if( FAILED( hr = device_->GetProperty( DIPROP_LOGICALRANGE, &diprg.diph ) ) ) {
			Log( L"Failed to Call SetProperty - DIPROP_LOGICALRANGE.", hr );
			return false;
		}

		std::wstringstream stream;
		stream << L"Logical Range Min: " << diprg.lMin << L" Max : " << diprg.lMax;

		if( FAILED( hr = device_->GetProperty( DIPROP_PHYSICALRANGE, &diprg.diph ) ) ) {
			Log( L"Failed to Call SetProperty - DIPROP_PHYSICALRANGE.", hr );
			return false;
		}
		stream << L", Physical Range Min : " << diprg.lMin << L" Max : " << diprg.lMax;

		// �ǂ����f�b�h�]�[����O�a�_���擾���Ă��Ӗ��͂Ȃ��悤��
		// Logitech RamblePad 2 �́A�����̒l�������Ȃ����A���S�t�߂Ƀf�b�h�]�[��������B
		// �f�b�h�]�[���́AXInput �̌Œ�l���g���A�����ł͒l�����O�ɏ����o���݂̂ɂ���
		DIPROPDWORD	propDword;
		ZeroMemory( &propDword, sizeof(propDword) );
		propDword.diph.dwSize = sizeof(DIPROPDWORD);
		propDword.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		propDword.diph.dwHow = DIPH_BYID;
		propDword.diph.dwObj = pdidoi->dwType;
		if( FAILED( hr = device_->GetProperty( DIPROP_DEADZONE, &propDword.diph ) ) ) {
			Log( L"Failed to Call GetProperty - DIPROP_DEADZONE.", hr );
			return false;
		}
		stream << L", Dead Zone : " << propDword.dwData;

		if( FAILED( hr = device_->GetProperty( DIPROP_SATURATION, &propDword.diph ) ) ) {
			Log( L"Failed to Call GetProperty - DIPROP_SATURATION.", hr );
			return false;
		}
		stream << L", Saturation Point : " << propDword.dwData;

		if( FAILED( hr = device_->GetProperty( DIPROP_GRANULARITY, &propDword.diph ) ) ) {
			Log( L"Failed to Call GetProperty - DIPROP_GRANULARITY.", hr );
			return false;
		}
		stream << L", Granulatiry : " << propDword.dwData;

		Log( stream.str().c_str() );
	}

	// �@�\�`�F�b�N
	std::wstringstream function;
	function << L"Functions : ";
	std::wstringstream effectStream;
	effectStream << L"Values : ";
	if( pdidoi->dwFlags & DIDOI_ASPECTMASK ) {
		function << L" ���̂ЂƂ̏��̂ݕ�";
	}
	if( pdidoi->dwFlags & DIDOI_ASPECTACCEL ) {
		function << L" �����x���";
	}
	if( pdidoi->dwFlags & DIDOI_ASPECTFORCE ) {
		function << L" �t�H�[�X���";
	}
	if( pdidoi->dwFlags & DIDOI_ASPECTPOSITION ) {
		function << L" �ʒu���";
	}
	if( pdidoi->dwFlags & DIDOI_ASPECTVELOCITY ) {
		function << L" ���x���";
	}
	if( pdidoi->dwFlags & DIDOI_FFACTUATOR ) {
		function << L" �t�H�[�X�t�B�[�h�o�b�N";
		enable_objs_.forcefeedback++;
		effectStream << L" Max Force = " << pdidoi->dwFFMaxForce << " [N]";
		effectStream << L" Force Resolution = " << pdidoi->dwFFForceResolution;
	}
	if( pdidoi->dwFlags & DIDOI_FFEFFECTTRIGGER ) {
		function << L" �t�H�[�X�t�B�[�h�o�b�N�g���K";
//		enable_objs_.forcefeedback++;
	}
	if( pdidoi->dwFlags & DIDOI_POLLED ) {
		function << L" Poll���\�b�h�Ăяo���K�{";
		request_poll_method_ = true;
	}
	if( pdidoi->dwFlags & DIDOI_GUIDISUSAGE ) {
		function << L" �K�v�Ȏg�p�y�[�W�Ǝg�p�󋵂̓p�b�N���ꂽDWORD�`��";
	}
	effectStream << L" Exponent = " << pdidoi->wExponent;

	Log( function.str().c_str() );

	// http://www.usb.org/developers/devclass_docs/HID1_11.pdf
	// http://www.usb.org/developers/devclass_docs/Hut1_12.pdf
	if( pdidoi->wUsage ) {
		std::wstring	pageName;
		std::wstring	usage;
		GetHIDDesc( pdidoi->wUsagePage, pdidoi->wUsage, pageName, usage );
		effectStream << L" HID Usage = " << pageName << L"(" << pdidoi->wUsage << L")";
		effectStream << L" HID Usage Page = " << usage << L"(" << pdidoi->wUsagePage << L")";
	}
	if( pdidoi->dwDimension ) {
		effectStream << L" HID Dimension Code = " << pdidoi->dwDimension;
	}
	effectStream << L" HID Collection Number = " << pdidoi->wCollectionNumber;
	effectStream << L" HID Designator Index = " << pdidoi->wDesignatorIndex;
	Log( effectStream.str().c_str() );

	return true;
}
void CDirectInputDevice::DoAutoKeyMapping()
{
	if( !CopyKeyMap( key_map_, vendor_id_, product_id_ ) ) {
		// ���t����Ȃ�����
		// ������ۂ��̂Ŋ��蓖�Ă����݂�

		int	pov_used = 0;
		if( enable_objs_.axis & DirectInputObjects::AXIS_X ) {
			key_map_.axis[DIAMAP_NUM_LX].obj_num = DIObj_AxisX;
			key_map_.axis[DIAMAP_NUM_LX].direction = 1;
		}
		if( enable_objs_.axis & DirectInputObjects::AXIS_Y ) {
			key_map_.axis[DIAMAP_NUM_LY].obj_num = DIObj_AxisY;
			key_map_.axis[DIAMAP_NUM_LY].direction = -1;	// Y���͔��]���Ă��邱�Ƃ������H
		}
		if( enable_objs_.axis & DirectInputObjects::AXIS_Z ) {
			key_map_.axis[DIAMAP_NUM_RX].obj_num = DIObj_AxisZ;
			key_map_.axis[DIAMAP_NUM_RX].direction = 1;
		}
		if( enable_objs_.axis & DirectInputObjects::AXIS_ROT_Z ) {
			key_map_.axis[DIAMAP_NUM_RY].obj_num = DIObj_AxisRotZ;
			key_map_.axis[DIAMAP_NUM_RY].direction = -1;	// Y���͔��]���Ă��邱�Ƃ������H
		}
		// ��ʓI�Ȏ��̐ݒ�ɍ��v���Ȃ������ꍇ�c�c �d���Ȃ��̂Ŏ��ɂ��肪���Ȃ̂Őݒ肷��
		if( key_map_.axis[DIAMAP_NUM_LX].obj_num == DIObj_Disable ) {
			if( enable_objs_.pov > pov_used ) {
				key_map_.axis[DIAMAP_NUM_LX].obj_num = DIObj_POV_0;	// POV �����蓖�ĂƂ�
				key_map_.axis[DIAMAP_NUM_LX].direction = 1;
				key_map_.axis[DIAMAP_NUM_LY].obj_num = DIObj_POV_0;
				key_map_.axis[DIAMAP_NUM_LY].direction = 1;
				pov_used++;
			}
		}

		if( key_map_.axis[DIAMAP_NUM_RX].obj_num == DIObj_Disable ) {
			if( enable_objs_.slider > 0 ) {	// �X���C�_�[����
				key_map_.axis[DIAMAP_NUM_RX].obj_num = DIObj_Slider_0;
				key_map_.axis[DIAMAP_NUM_RX].direction = -1;
			} else if( enable_objs_.axis & DirectInputObjects::AXIS_ROT_X ) {	// X����]�H
				key_map_.axis[DIAMAP_NUM_RX].obj_num = DIObj_AxisRotX;
				key_map_.axis[DIAMAP_NUM_RX].direction = -1;
			} else if( enable_objs_.axis & DirectInputObjects::AXIS_ROT_Y ) {	// Y����]�H
				key_map_.axis[DIAMAP_NUM_RX].obj_num = DIObj_AxisRotY;
				key_map_.axis[DIAMAP_NUM_RX].direction = -1;
			} else if( enable_objs_.pov > pov_used ) {
				key_map_.axis[DIAMAP_NUM_RX].obj_num = DIObj_POV_0+pov_used;	// POV �����蓖�ĂƂ�
				key_map_.axis[DIAMAP_NUM_RX].direction = 1;
				pov_used++;
			}
		}
		if( key_map_.axis[DIAMAP_NUM_RY].obj_num == DIObj_Disable ) {
			if( key_map_.axis[DIAMAP_NUM_RX].obj_num >= DIObj_POV_0 && key_map_.axis[DIAMAP_NUM_RX].obj_num <= DIObj_POV_3 ) {
				// X����POV�Ȃ炱������POV
				key_map_.axis[DIAMAP_NUM_RY].obj_num = key_map_.axis[DIAMAP_NUM_RX].obj_num;
				key_map_.axis[DIAMAP_NUM_RY].direction = 1;
			}
		}
		if( enable_objs_.pov > 0 ) {
			key_map_.button[DIBMAP_NUM_DPAD_UP].obj_num = DIObj_POV_0;
			key_map_.button[DIBMAP_NUM_DPAD_UP].direction = 1;
			key_map_.button[DIBMAP_NUM_DPAD_DOWN].obj_num = DIObj_POV_0;
			key_map_.button[DIBMAP_NUM_DPAD_DOWN].direction = 1;
			key_map_.button[DIBMAP_NUM_DPAD_LEFT].obj_num = DIObj_POV_0;
			key_map_.button[DIBMAP_NUM_DPAD_LEFT].direction = 1;
			key_map_.button[DIBMAP_NUM_DPAD_RIGHT].obj_num = DIObj_POV_0;
			key_map_.button[DIBMAP_NUM_DPAD_RIGHT].direction = 1;
		}
		// �ŏ���4�{�^���֗D��I�Ɋ��蓖�Ă�
		int	b = 0;
		for( ; b < enable_objs_.button && b < 4; b++ ) {
			key_map_.button[b+DIBMAP_NUM_A].obj_num = DIObj_Button1 + b;
			key_map_.button[b+DIBMAP_NUM_A].direction = 1;
		}
		// ����LR��SHOULDER�֊��蓖��
		for( int t = 0; b < enable_objs_.button && b < 6; b++, t++ ) {
			key_map_.button[t+DIBMAP_NUM_LEFT_SHOULDER].obj_num = DIObj_Button1 + b;
			key_map_.button[t+DIBMAP_NUM_LEFT_SHOULDER].direction = 1;
		}
		// ����LR��TRIGGER�֊��蓖��
		for( int t = 0; b < enable_objs_.button && b < 8; b++, t++ ) {
			key_map_.trigger[t].obj_num = DIObj_Button1 + b;
			key_map_.trigger[t].direction = 1;
		}
		// ����BACK�{�^��
		for( ; b < enable_objs_.button && b < 9; b++ ) {
			key_map_.button[DIBMAP_NUM_BACK].obj_num = DIObj_Button1 + b;
			key_map_.button[DIBMAP_NUM_BACK].direction = 1;
		}
		// ����START�{�^��
		for( ; b < enable_objs_.button && b < 10; b++ ) {
			key_map_.button[DIBMAP_NUM_START].obj_num = DIObj_Button1 + b;
			key_map_.button[DIBMAP_NUM_START].direction = 1;
		}
		// ���̓X�e�B�b�N��THUMB�{�^��
		for( int t = 0; b < enable_objs_.button && b < 12; b++, t++ ) {
			key_map_.button[t+DIBMAP_NUM_LEFT_THUMB].obj_num = DIObj_Button1 + b;
			key_map_.button[t+DIBMAP_NUM_LEFT_THUMB].direction = 1;
		}
		// �t�H�[�X�t�B�[�h�o�b�N����2�ȏ゠�鎞�́A���������ŏ�
		if( enable_objs_.forcefeedback > 1 ) {
			key_map_.leftforcefirst = true;
		}
	}
}
void CDirectInputDevice::RetrieveVendorAndProductID()
{
	HRESULT hr;
	DIPROPDWORD dipdw;
	dipdw.diph.dwSize		= sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize	= sizeof(DIPROPHEADER);
	dipdw.diph.dwObj		= 0;
	dipdw.diph.dwHow		= DIPH_DEVICE;
	if( FAILED( hr = device_->GetProperty( DIPROP_VIDPID, &dipdw.diph ) ) ) {
		Log( L"Failed to Call GetProperty - DIPROP_VIDPID.", hr );
		vendor_id_  = 0;
		product_id_ = 0;
	} else {
		vendor_id_  = LOWORD(dipdw.dwData);
		product_id_ = HIWORD(dipdw.dwData);

		std::wstringstream stream;
		stream << L"Vernder ID : " << vendor_id_ << L", Product ID : " << product_id_;
		Log( stream.str().c_str() );
	}
}
void CDirectInputDevice::RetrieveConnectedPortName()
{
	DIPROPSTRING	distr;
	ZeroMemory( &distr, sizeof(distr) );
	distr.diph.dwSize		= sizeof(DIPROPSTRING);
	distr.diph.dwHeaderSize	= sizeof(DIPROPHEADER);
	distr.diph.dwObj		= 0;
	distr.diph.dwHow		= DIPH_DEVICE;

// DIPROP_GETPORTDISPLAYNAME �͐��������ꍇ�ł� S_FALSE ���Ԃ����
	HRESULT hr;
	if( FAILED( hr = device_->GetProperty( DIPROP_GETPORTDISPLAYNAME, &distr.diph ) ) ) {
		Log( L"Failed to Call GetProperty - DIPROP_GETPORTDISPLAYNAME.", hr );
		physical_port_name_.clear();
	} else {
		hr = device_->GetProperty( DIPROP_GETPORTDISPLAYNAME, &distr.diph );
		physical_port_name_.assign( distr.wsz );

		std::wstringstream stream;
		stream << L"Connected port name : " << physical_port_name_;
		Log( stream.str().c_str() );
	}
}

bool CDirectInputDevice::Initialize( HWND hWnd )
{
	if( !device_ ) {
		return false;
	}

	HRESULT		hr;
	if( FAILED( hr = device_->SetDataFormat( &c_dfDIJoystick ) ) ) {
		Log( L"Failed to Call SetDataFormat.", hr );
		return false;
	}
	if( FAILED( hr = device_->SetCooperativeLevel( hWnd, DISCL_NONEXCLUSIVE |  DISCL_FOREGROUND ) ) ) {
		Log( L"Failed to Call SetCooperativeLevel.", hr );
		return false;
	}

	return InitializeDeviceDetail();
}
bool CDirectInputDevice::InitializeDeviceDetail()
{
	if( !inspector_.GetInfomation( device_ ) ) {
		return false;
	}

	instance_guid_ = inspector_.GetInstanceGuid();

	std::string name;
	inspector_.GetProductName( name );
	EncodeToUTF16( name_, name );

	std::wstringstream stream;
	stream << L"Game Pad Device Name : " << name_;
	Log( stream.str().c_str() );

	RetrieveVendorAndProductID();
	RetrieveConnectedPortName();
	inspector_.PrintDetail();

	HRESULT		hr;
	if( FAILED( hr = device_->EnumObjects( EnumObjectsCallback, (void*)this, DIDFT_ALL ) ) ) {
		Log( L"Failed to Call EnumObjects.", hr );
		return false;
	}

	DoAutoKeyMapping();

	return true;
}

void CDirectInputDevice::Update()
{
	if( device_creating_ ) {
		RecreateDevice();
	}

	// �܂��擾�ł��Ȃ�
	if( device_creating_ ) return;

	HRESULT		hr;

	if( request_poll_method_ ) {
		hr = device_->Poll();
		if( FAILED(hr) ) {
			if( hr == DIERR_INPUTLOST ) {
//				Log( L"���̓f�o�C�X�ւ̃A�N�Z�X������ꂽ�B�Ď擾���Ȃ���΂Ȃ�Ȃ��B ");
			} else if( hr == DIERR_NOTACQUIRED ) {
//				Log( L"����́A�f�o�C�X���擾����Ȃ�������s�ł��Ȃ��B  ");
			} else if( hr == DIERR_NOTINITIALIZED ) {
				Log( L"���̃I�u�W�F�N�g�͏���������Ă��Ȃ��B  ");
				Log( L"Failed to Call Poll.", hr );
			}
		}
	}

	if( DI_OK != ( hr = device_->GetDeviceState( sizeof(DIJOYSTATE), (void*)&(state_.state) ) ) ) {
		if( hr == DIERR_INPUTLOST ) {
//			RecreateDevice();
			Acquire();
			ClearState();
		} else if( hr == DIERR_NOTACQUIRED ) {
			// �f�o�C�X���擾����Ă��Ȃ��̂Ŏ擾����
			Acquire();
			ClearState();
		} else if( hr == DIERR_NOTINITIALIZED || hr == DIERR_INVALIDPARAM ) {
			//! �v���O�����G���[
			assert(0);
			// �����ŗ�O�𓊂���
		} else if( hr == E_PENDING ) {
			// �f�[�^���܂����p�ł��Ȃ��悤�Ȃ̂ŃN���A���Ă����B
			ClearState();
		} else {
			// Unknown Error
			Log( L"Failed to Call GetDeviceState.", hr );
			ClearState();
		}
	} else {
		// GetDeviceState �������������AForce Feedback�̐ݒ�𔽉f����
		UpdateFF();
	}
}

bool CDirectInputDevice::Acquire()
{
	HRESULT hr;
	if( DI_OK != ( hr = device_->Acquire() ) ) {
		if( hr == DIERR_INVALIDPARAM ) {
			//! �v���O�����G���[
			// assert(0);
			// �����ŗ�O�𓊂���
			return false;
		} else if( hr == DIERR_NOTINITIALIZED ) {
			return false;
		} else if( hr == DIERR_OTHERAPPHASPRIO ) {
			// �A�v�����o�b�N�O���E���h�ɂ���悤��
			return false;
		} else {
			// Unknown Error
			Log( L"Failed to Call Acquire.", hr );
			return false;
		}
	}
	return true;
}
void CDirectInputDevice::RecreateDevice()
{
	if( device_.p ) {
		device_.Release();
		device_ = NULL;
	}
	if( !Port()->CreateDirectInputDevice( instance_guid_, &device_ ) ) {
		device_creating_ = true;
	} else {
		device_creating_ = false;
	}
}

unsigned long CDirectInputDevice::GetKeyState() const
{
	unsigned long	ret = 0;

	const BYTE*	btn = (BYTE*)state_.state.rgbButtons;

	int num = key_map_.button[DIBMAP_NUM_START].obj_num - DIObj_Button1;
	ret |= btn[num] & 0x80 ? IDBTN_START : 0;

	num = key_map_.button[DIBMAP_NUM_BACK].obj_num - DIObj_Button1;
	ret |= btn[num] & 0x80 ? IDBTN_BACK : 0;

	num = key_map_.button[DIBMAP_NUM_LEFT_THUMB].obj_num - DIObj_Button1;
	ret |= btn[num] & 0x80 ? IDBTN_LEFT_THUMB : 0;

	num = key_map_.button[DIBMAP_NUM_RIGHT_THUMB].obj_num - DIObj_Button1;
	ret |= btn[num] & 0x80 ? IDBTN_RIGHT_THUMB : 0;

	num = key_map_.button[DIBMAP_NUM_LEFT_SHOULDER].obj_num - DIObj_Button1;
	ret |= btn[num] & 0x80 ? IDBTN_LEFT_SHOULDER : 0;

	num = key_map_.button[DIBMAP_NUM_RIGHT_SHOULDER].obj_num - DIObj_Button1;
	ret |= btn[num] & 0x80 ? IDBTN_RIGHT_SHOULDER : 0;

	num = key_map_.button[DIBMAP_NUM_A].obj_num - DIObj_Button1;
	ret |= btn[num] & 0x80 ? IDBTN_A : 0;

	num = key_map_.button[DIBMAP_NUM_B].obj_num - DIObj_Button1;
	ret |= btn[num] & 0x80 ? IDBTN_B : 0;

	num = key_map_.button[DIBMAP_NUM_X].obj_num - DIObj_Button1;
	ret |= btn[num] & 0x80 ? IDBTN_X : 0;

	num = key_map_.button[DIBMAP_NUM_Y].obj_num - DIObj_Button1;
	ret |= btn[num] & 0x80 ? IDBTN_Y : 0;

	// ��{�^��
	num = key_map_.button[DIBMAP_NUM_DPAD_UP].obj_num;
	int dir = key_map_.button[DIBMAP_NUM_DPAD_UP].direction;
	if( IsPressPlusKey( DIBMAP_NUM_DPAD_UP, num, dir ) ) {
		ret |= IDBTN_DPAD_UP;
	}

	// ���{�^��
	num = key_map_.button[DIBMAP_NUM_DPAD_DOWN].obj_num;
	dir = key_map_.button[DIBMAP_NUM_DPAD_DOWN].direction;
	if( IsPressPlusKey( DIBMAP_NUM_DPAD_DOWN, num, dir ) ) {
		ret |= IDBTN_DPAD_DOWN;
	}

	// ���{�^��
	num = key_map_.button[DIBMAP_NUM_DPAD_LEFT].obj_num;
	dir = key_map_.button[DIBMAP_NUM_DPAD_LEFT].direction;
	if( IsPressPlusKey( DIBMAP_NUM_DPAD_LEFT, num, dir ) ) {
		ret |= IDBTN_DPAD_LEFT;
	}

	// �E�{�^��
	num = key_map_.button[DIBMAP_NUM_DPAD_RIGHT].obj_num;
	dir = key_map_.button[DIBMAP_NUM_DPAD_RIGHT].direction;
	if( IsPressPlusKey( DIBMAP_NUM_DPAD_RIGHT, num, dir ) ) {
		ret |= IDBTN_DPAD_RIGHT;
	}

	return ret;
}
bool CDirectInputDevice::IsPressPlusKey( long key, int num, int dir ) const
{
	if( num >= DIObj_POV_0 && num <= DIObj_POV_3 ) {
		int	idx = num - DIObj_POV_0;
		switch( key ) {
		case DIBMAP_NUM_DPAD_UP:
			return IsPressUpForPovVal(state_.state.rgdwPOV[idx]);
		case DIBMAP_NUM_DPAD_DOWN:
			return IsPressDownForPovVal(state_.state.rgdwPOV[idx]);
		case DIBMAP_NUM_DPAD_LEFT:
			return IsPressLeftForPovVal(state_.state.rgdwPOV[idx]);
		case DIBMAP_NUM_DPAD_RIGHT:
			return IsPressRightForPovVal(state_.state.rgdwPOV[idx]);
		}
	} else if( num >= DIObj_AxisX && num <= DIObj_Slider_1 ) {
		// �����^�ŕ���ł���̂Ŕz��Ƃ݂Ȃ��ď����B�X���C�_�[���ꏏ
		const LONG	*axis = &(state_.state.lX);
		if( key == DIBMAP_NUM_DPAD_UP || key == DIBMAP_NUM_DPAD_RIGHT ) {
			return ( (axis[num] * dir) > 0 );
		} else {
			return ( (axis[num] * dir) < 0 );
		}
	} else {
		int	idx = num - DIObj_Button1;
		return state_.state.rgbButtons[num] & 0x80 ? true : false;
	}
	return false;
}
double CDirectInputDevice::GetAxisValue( long val, int dir )
{
	if( DEADZONE >= val && val >= (-DEADZONE) ) {
		return 0.0;
	} else if( val >= 0 ) {
		if( dir < 0 ) {
			val *= dir;
			return (double)(val+DEADZONE) / (double)(32767-DEADZONE);
		} else {
			return (double)(val-DEADZONE) / (double)(32767-DEADZONE);
		}
	} else {
		if( dir < 0 ) {
			val *= dir;
			return (double)(val-DEADZONE) / (double)(32768-DEADZONE);
		} else {
			return (double)(val+DEADZONE) / (double)(32768-DEADZONE);
		}
	}
}
double CDirectInputDevice::GetTriggerValue( int num ) const
{
	if( num >= DIObj_AxisX && num <= DIObj_Slider_1 ) {
		// �����^�ŕ���ł���̂Ŕz��Ƃ݂Ȃ��ď����B�X���C�_�[���ꏏ
		const LONG	*axis = &(state_.state.lX);
		LONG	val = axis[num] + (-AXIS_RANGE_MIN);
		return (double)val / (double)AXIS_RANGE;
	} else if( num >= DIObj_POV_0 && num <= DIObj_POV_3 ) {
		int	idx = num - DIObj_POV_0;
		DWORD pov = LOWORD(state_.state.rgdwPOV[idx]);
		if( pov != 0xFFFF ) {
			return 1.0;
		} else {
			return 0.0;
		}
	} else {
		if( state_.state.rgbButtons[num - DIObj_Button1] & 0x80 ) {
			return 1.0;
		} else {
			return 0.0;
		}
	}
}
double CDirectInputDevice::GetLeftTrigger() const
{
	int num = key_map_.trigger[DITMAP_NUM_LEFT].obj_num;
	return GetTriggerValue( num );
}
double CDirectInputDevice::GetRightTrigger() const
{
	int num = key_map_.trigger[DITMAP_NUM_RIGHT].obj_num;
	return GetTriggerValue( num );
}
//! @param h : ����(����)���ǂ���
double CDirectInputDevice::GetThumbStickValue( int num, int dir, bool h ) const
{
	if( num >= DIObj_AxisX || num <= DIObj_Slider_1 ) {
		// �����^�ŕ���ł���̂Ŕz��Ƃ݂Ȃ��ď����B�X���C�_�[���ꏏ
		const LONG	*axis = &(state_.state.lX);
		return GetAxisValue( axis[num], dir );
	} else if( num >= DIObj_POV_0 && num <= DIObj_POV_3 ) {
		int	idx = num - DIObj_POV_0;
		DWORD pov = LOWORD(state_.state.rgdwPOV[idx]);
		if( pov != 0xFFFF ) {
			// ����0�x�ɂ��āA�����v���Ŋp�x���i�ނ悤�ɂ���B
			pov = (36000 - ((pov + 90000) % 36000)) % 36000;
			if( h )
				return cos( (pov * M_PI) / (100.0 * 180.0) );
			else
				return sin( (pov * M_PI) / (100.0 * 180.0) );
		} else {
			return 0.0;
		}
	} else if( num >= DIObj_Button1 && num <= DIObj_Button32 ) {
		if( state_.state.rgbButtons[num - DIObj_Button1] & 0x80 ) {
			return 1.0;
		} else if( state_.state.rgbButtons[dir - DIObj_Button1] & 0x80 ) {
			return -1.0;
		} else {
			return 0.0;
		}
	} else {
		return 0.0;
	}
}
double CDirectInputDevice::GetLeftThumbStickX() const
{
	int num = key_map_.axis[DIAMAP_NUM_LX].obj_num;
	int dir = key_map_.axis[DIAMAP_NUM_LX].direction;
	return GetThumbStickValue( num, dir, true );
}
double CDirectInputDevice::GetLeftThumbStickY() const
{
	int num = key_map_.axis[DIAMAP_NUM_LY].obj_num;
	int dir = key_map_.axis[DIAMAP_NUM_LY].direction;
	return GetThumbStickValue( num, dir, false );
}
double CDirectInputDevice::GetRightThumbStickX() const
{
	int num = key_map_.axis[DIAMAP_NUM_RX].obj_num;
	int dir = key_map_.axis[DIAMAP_NUM_RX].direction;
	return GetThumbStickValue( num, dir, true );
}
double CDirectInputDevice::GetRightThumbStickY() const
{
	int num = key_map_.axis[DIAMAP_NUM_RY].obj_num;
	int dir = key_map_.axis[DIAMAP_NUM_RY].direction;
	return GetThumbStickValue( num, dir, false );
}
unsigned long CDirectInputDevice::Sensing() const
{
	unsigned long ret = GetKeyState();

	if( GetLeftThumbStickX() != 0.0 ) ret |= IDBTN_LEFT_THUMB_AXIS_X;
	if( GetLeftThumbStickY() != 0.0 ) ret |= IDBTN_LEFT_THUMB_AXIS_Y;
	if( GetRightThumbStickX() != 0.0 ) ret |= IDBTN_RIGHT_THUMB_AXIS_X;
	if( GetRightThumbStickY() != 0.0 ) ret |= IDBTN_RIGHT_THUMB_AXIS_Y;

	if( GetLeftTrigger() != 0.0 ) ret |= IDBTN_LEFT_TRIGGER;
	if( GetRightTrigger() != 0.0 ) ret |= IDBTN_RIGHT_TRIGGER;

	return ret;
}

void CDirectInputDevice::ClearKeyMap()
{
	for( int i = 0; i < DIBMAP_NUM_EOT; ++i ) {
		key_map_.button[i].obj_num = DIObj_Disable;
		key_map_.button[i].direction = 1;
	}
	for( int i = 0; i < DITMAP_NUM_EOT; ++i ) {
		key_map_.trigger[i].obj_num = DIObj_Disable;
		key_map_.trigger[i].direction = 1;
	}
	for( int i = 0; i < DIAMAP_NUM_EOT; ++i ) {
		key_map_.axis[i].obj_num = DIObj_Disable;
		key_map_.axis[i].direction = 1;
	}
	key_map_.leftforcefirst = true;
}
void CDirectInputDevice::GetHIDDesc( WORD page, WORD id, std::wstring& pageName, std::wstring& usage )
{
	switch( page ) {
	case 0x00:
		pageName = L"Undefined";
		usage = L"Undefined";
		break;
	case 0x01:
		pageName = L"Generic Desktop Controls";
		switch( id ) {
			case 0x00: usage = L"Undefined"; break;
			case 0x01: usage = L"Pointer"; break;
			case 0x02: usage = L"Mouse"; break;
			case 0x04: usage = L"Joystick"; break;
			case 0x05: usage = L"Game Pad"; break;
			case 0x06: usage = L"Keyboard"; break;
			case 0x07: usage = L"Keypad"; break;
			case 0x08: usage = L"Multi-axis Controller"; break;
			case 0x09: usage = L"Tablet PC System Controls"; break;
			case 0x30: usage = L"X"; break;
			case 0x31: usage = L"Y"; break;
			case 0x32: usage = L"Z"; break;
			case 0x33: usage = L"Rx"; break;
			case 0x34: usage = L"Ry"; break;
			case 0x35: usage = L"Rz"; break;
			case 0x36: usage = L"Slider"; break;
			case 0x37: usage = L"Dial"; break;
			case 0x38: usage = L"Wheel"; break;
			case 0x39: usage = L"Hat switch"; break;
			case 0x3A: usage = L"Counted Buffer"; break;
			case 0x3B: usage = L"Byte Count"; break;
			case 0x3C: usage = L"Motion Wakeup"; break;
			case 0x3D: usage = L"Start"; break;
			case 0x3E: usage = L"Select"; break;
			case 0x40: usage = L"Vx"; break;
			case 0x41: usage = L"Vy"; break;
			case 0x42: usage = L"Vz"; break;
			case 0x43: usage = L"Vbrx"; break;
			case 0x44: usage = L"Vbry"; break;
			case 0x45: usage = L"Vbrz"; break;
			case 0x46: usage = L"Vno"; break;
			case 0x47: usage = L"Feature Notification"; break;
			case 0x48: usage = L"Resolution Multiplier"; break;
			case 0x80: usage = L"System Control"; break;
			case 0x81: usage = L"System Power Down"; break;
			case 0x82: usage = L"System Sleep"; break;
			case 0x83: usage = L"System Wake Up"; break;
			case 0x84: usage = L"System Context Menu"; break;
			case 0x85: usage = L"System Main Menu"; break;
			case 0x86: usage = L"System App Menu"; break;
			case 0x87: usage = L"System Menu Help"; break;
			case 0x88: usage = L"System Menu Exit"; break;
			case 0x89: usage = L"System Menu Select"; break;
			case 0x8A: usage = L"System Menu Right"; break;
			case 0x8B: usage = L"System Menu Left"; break;
			case 0x8C: usage = L"System Menu Up"; break;
			case 0x8D: usage = L"System Menu Down"; break;
			case 0x8E: usage = L"System Cold Restart"; break;
			case 0x8F: usage = L"System Warm Restart"; break;
			case 0x90: usage = L"D-pad Up"; break;
			case 0x91: usage = L"D-pad Down"; break;
			case 0x92: usage = L"D-pad Right"; break;
			case 0x93: usage = L"D-pad Left"; break;
			case 0xA0: usage = L"System Dock"; break;
			case 0xA1: usage = L"System Undock"; break;
			case 0xA2: usage = L"System Setup"; break;
			case 0xA3: usage = L"System Break"; break;
			case 0xA4: usage = L"System Debugger Break"; break;
			case 0xA5: usage = L"Application Break"; break;
			case 0xA6: usage = L"Application Debugger Break"; break;
			case 0xA7: usage = L"System Speaker Mute"; break;
			case 0xA8: usage = L"System Hibernate"; break;
			case 0xB0: usage = L"System Display Invert"; break;
			case 0xB1: usage = L"System Display Internal"; break;
			case 0xB2: usage = L"System Display External"; break;
			case 0xB3: usage = L"System Display Both"; break;
			case 0xB4: usage = L"System Display Dual"; break;
			case 0xB5: usage = L"System Display Toggle Int/Ext"; break;
			case 0xB6: usage = L"System Display Swap Primary/Secondary"; break;
			case 0xB7: usage = L"System Display LCD Autoscale"; break;
			default: usage = L"Reserved"; break;
		}
		break;
	case 0x02:
		pageName = L"Simulation Controls";
		switch( id ) {
			case 0x00: usage = L"Undefined"; break;
			case 0x01: usage = L"Flight Simulation Device"; break;
			case 0x02: usage = L"Automobile Simulation Device"; break;
			case 0x03: usage = L"Tank Simulation Device"; break;
			case 0x04: usage = L"Spaceship Simulation Device"; break;
			case 0x05: usage = L"Submarine Simulation Device"; break;
			case 0x06: usage = L"Sailing Simulation Device"; break;
			case 0x07: usage = L"Motorcycle Simulation Device"; break;
			case 0x08: usage = L"Sports Simulation Device"; break;
			case 0x09: usage = L"Airplane Simulation Device"; break;
			case 0x0A: usage = L"Helicopter Simulation Device"; break;
			case 0x0B: usage = L"Magic Carpet Simulation Device"; break;
			case 0x0C: usage = L"Bicycle Simulation Device"; break;
			case 0x20: usage = L"Flight Control Stick"; break;
			case 0x21: usage = L"Flight Stick"; break;
			case 0x22: usage = L"Cyclic Control"; break;
			case 0x23: usage = L"Cyclic Trim"; break;
			case 0x24: usage = L"Flight Yoke"; break;
			case 0x25: usage = L"Track Control"; break;
			case 0xB0: usage = L"Aileron"; break;
			case 0xB1: usage = L"Aileron Trim"; break;
			case 0xB2: usage = L"Anti-Torque Control"; break;
			case 0xB3: usage = L"Autopilot Enable"; break;
			case 0xB4: usage = L"Chaff Release"; break;
			case 0xB5: usage = L"Collective Control"; break;
			case 0xB6: usage = L"Dive Brake"; break;
			case 0xB7: usage = L"Electronic Countermeasures"; break;
			case 0xB8: usage = L"Elevator"; break;
			case 0xB9: usage = L"Elevator Trim"; break;
			case 0xBA: usage = L"Rudder"; break;
			case 0xBB: usage = L"Throttle"; break;
			case 0xBC: usage = L"Flight Communications"; break;
			case 0xBD: usage = L"Flare Release"; break;
			case 0xBE: usage = L"Landing Gear"; break;
			case 0xBF: usage = L"Toe Brake"; break;
			case 0xC0: usage = L"Trigger"; break;
			case 0xC1: usage = L"Weapons Arm"; break;
			case 0xC2: usage = L"Weapons Select"; break;
			case 0xC3: usage = L"Wing Flaps"; break;
			case 0xC4: usage = L"Accelerator"; break;
			case 0xC5: usage = L"Brake"; break;
			case 0xC6: usage = L"Clutch"; break;
			case 0xC7: usage = L"Shifter"; break;
			case 0xC8: usage = L"Steering"; break;
			case 0xC9: usage = L"Turret Direction"; break;
			case 0xCA: usage = L"Barrel Elevation"; break;
			case 0xCB: usage = L"Dive Plane"; break;
			case 0xCC: usage = L"Ballast"; break;
			case 0xCD: usage = L"Bicycle Crank"; break;
			case 0xCE: usage = L"Handle Bars"; break;
			case 0xCF: usage = L"Front Brake"; break;
			case 0xD0: usage = L"Rear Brake"; break;
			default: usage = L"Reserved"; break;
		}
		break;
	case 0x03:
		pageName = L"VR Controls";
		switch( id ) {
			case 0x00: usage = L"Unidentified"; break;
			case 0x01: usage = L"Belt"; break;
			case 0x02: usage = L"Body Suit"; break;
			case 0x03: usage = L"Flexor"; break;
			case 0x04: usage = L"Glove"; break;
			case 0x05: usage = L"Head Tracker"; break;
			case 0x06: usage = L"Head Mounted Display"; break;
			case 0x07: usage = L"Hand Tracker"; break;
			case 0x08: usage = L"Oculometer"; break;
			case 0x09: usage = L"Vest"; break;
			case 0x0A: usage = L"Animatronic Device"; break;
			case 0x20: usage = L"Stereo Enable"; break;
			case 0x21: usage = L"Display Enable"; break;
			default: usage = L"Reserved"; break;
		}
		break;
	case 0x04:
		pageName = L"Sport Controls";
		switch( id ) {
			case 0x00: usage = L"Unidentified"; break;
			case 0x01: usage = L"Baseball Bat"; break;
			case 0x02: usage = L"Golf Club"; break;
			case 0x03: usage = L"Rowing Machine"; break;
			case 0x04: usage = L"Treadmill"; break;
			case 0x30: usage = L"Oar"; break;
			case 0x31: usage = L"Slope"; break;
			case 0x32: usage = L"Rate"; break;
			case 0x33: usage = L"Stick Speed"; break;
			case 0x34: usage = L"Stick Face Angle"; break;
			case 0x35: usage = L"Stick Heel/Toe"; break;
			case 0x36: usage = L"Stick Follow Through"; break;
			case 0x37: usage = L"Stick Tempo"; break;
			case 0x38: usage = L"Stick Type NAry"; break;
			case 0x39: usage = L"Stick Height"; break;
			case 0x50: usage = L"Putter"; break;
			case 0x51: usage = L"1 Iron"; break;
			case 0x52: usage = L"2 Iron"; break;
			case 0x53: usage = L"3 Iron"; break;
			case 0x54: usage = L"4 Iron"; break;
			case 0x55: usage = L"5 Iron"; break;
			case 0x56: usage = L"6 Iron"; break;
			case 0x57: usage = L"7 Iron"; break;
			case 0x58: usage = L"8 Iron"; break;
			case 0x59: usage = L"9 Iron"; break;
			case 0x5A: usage = L"10 Iron"; break;
			case 0x5B: usage = L"11 Iron"; break;
			case 0x5C: usage = L"Sand Wedge"; break;
			case 0x5D: usage = L"Loft Wedge"; break;
			case 0x5E: usage = L"Power Wedge"; break;
			case 0x5F: usage = L"1 Wood"; break;
			case 0x60: usage = L"3 Wood"; break;
			case 0x61: usage = L"5 Wood"; break;
			case 0x62: usage = L"7 Wood"; break;
			case 0x63: usage = L"9 Wood"; break;
			default: usage = L"Reserved"; break;
		}
		break;
	case 0x05:
		pageName = L"Game Controls";
		switch( id ) {
			case 0x00: usage = L"Undefined"; break;
			case 0x01: usage = L"3D Game Controller"; break;
			case 0x02: usage = L"Pinball Device"; break;
			case 0x03: usage = L"Gun Device"; break;
			case 0x20: usage = L"Point of View"; break;
			case 0x21: usage = L"Turn Right/Left"; break;
			case 0x22: usage = L"Pitch Forward/Backward"; break;
			case 0x23: usage = L"Roll Right/Left"; break;
			case 0x24: usage = L"Move Right/Left"; break;
			case 0x25: usage = L"Move Forward/Backward"; break;
			case 0x26: usage = L"Move Up/Down"; break;
			case 0x27: usage = L"Lean Right/Left"; break;
			case 0x28: usage = L"Lean Forward/Backward"; break;
			case 0x29: usage = L"Height of POV"; break;
			case 0x2A: usage = L"Flipper"; break;
			case 0x2B: usage = L"Secondary Flipper"; break;
			case 0x2C: usage = L"Bump"; break;
			case 0x2D: usage = L"New Game"; break;
			case 0x2E: usage = L"Shoot Ball"; break;
			case 0x2F: usage = L"Player"; break;
			case 0x30: usage = L"Gun Bolt"; break;
			case 0x31: usage = L"Gun Clip"; break;
			case 0x32: usage = L"Gun Selector NAry"; break;
			case 0x33: usage = L"Gun Single Shot"; break;
			case 0x34: usage = L"Gun Burst"; break;
			case 0x35: usage = L"Gun Automatic"; break;
			case 0x36: usage = L"Gun Safety"; break;
			case 0x37: usage = L"Gamepad Fire/Jump"; break;
			case 0x39: usage = L"Gamepad Trigger"; break;
			default: usage = L"Reserved"; break;
		}
		break;
	case 0x06:
		pageName = L"Generic Device Controls";
		switch( id ) {
			case 0x00: usage = L"Unidentified"; break;
			case 0x20: usage = L"Battery Strength"; break;
			case 0x21: usage = L"Wireless Channel"; break;
			case 0x22: usage = L"Wireless ID"; break;
			case 0x23: usage = L"Discover Wireless Control"; break;
			case 0x24: usage = L"Security Code Character Entered"; break;
			case 0x25: usage = L"Security Code Character Erased"; break;
			case 0x26: usage = L"Security Code Cleared"; break;
			default: usage = L"Reserved"; break;
		}
		break;
	case 0x07:
		pageName = L"Keyboard/Keypad";
		usage = L"Key code";
		break;
	case 0x08:
		pageName = L"LEDs";
		switch( id ) {
			case 0x00: usage = L"Undefined"; break;
			case 0x01: usage = L"Num Lock"; break;
			case 0x02: usage = L"Caps Lock"; break;
			case 0x03: usage = L"Scroll Lock"; break;
			case 0x04: usage = L"Compose"; break;
			case 0x05: usage = L"Kana"; break;
			case 0x06: usage = L"Power"; break;
			case 0x07: usage = L"Shift"; break;
			case 0x08: usage = L"Do Not Disturb"; break;
			case 0x09: usage = L"Mute"; break;
			case 0x0A: usage = L"Tone Enable"; break;
			case 0x0B: usage = L"High Cut Filter"; break;
			case 0x0C: usage = L"Low Cut Filter"; break;
			case 0x0D: usage = L"Equalizer Enable"; break;
			case 0x0E: usage = L"Sound Field On"; break;
			case 0x0F: usage = L"Surround On"; break;
			case 0x10: usage = L"Repeat"; break;
			case 0x11: usage = L"Stereo"; break;
			case 0x12: usage = L"Sampling Rate Detect"; break;
			case 0x13: usage = L"Spinning"; break;
			case 0x14: usage = L"CAV"; break;
			case 0x15: usage = L"CLV"; break;
			case 0x16: usage = L"Recording Format Detect"; break;
			case 0x17: usage = L"Off-Hook"; break;
			case 0x18: usage = L"Ring"; break;
			case 0x19: usage = L"Message Waiting"; break;
			case 0x1A: usage = L"Data Mode"; break;
			case 0x1B: usage = L"Battery Operation"; break;
			case 0x1C: usage = L"Battery OK"; break;
			case 0x1D: usage = L"Battery Low"; break;
			case 0x1E: usage = L"Speaker"; break;
			case 0x1F: usage = L"Head Set"; break;
			case 0x20: usage = L"Hold"; break;
			case 0x21: usage = L"Microphone"; break;
			case 0x22: usage = L"Coverage"; break;
			case 0x23: usage = L"Night Mode"; break;
			case 0x24: usage = L"Send Calls"; break;
			case 0x25: usage = L"Call Pickup"; break;
			case 0x26: usage = L"Conference"; break;
			case 0x27: usage = L"Stand-by"; break;
			case 0x28: usage = L"Camera On"; break;
			case 0x29: usage = L"Camera Off"; break;
			case 0x2A: usage = L"On-Line"; break;
			case 0x2B: usage = L"Off-Line"; break;
			case 0x2C: usage = L"Busy"; break;
			case 0x2D: usage = L"Ready"; break;
			case 0x2E: usage = L"Paper-Out"; break;
			case 0x2F: usage = L"Paper-Jam"; break;
			case 0x30: usage = L"Remote"; break;
			case 0x31: usage = L"Forward"; break;
			case 0x32: usage = L"Reverse"; break;
			case 0x33: usage = L"Stop"; break;
			case 0x34: usage = L"Rewind"; break;
			case 0x35: usage = L"Fast Forward"; break;
			case 0x36: usage = L"Play"; break;
			case 0x37: usage = L"Pause"; break;
			case 0x38: usage = L"Record"; break;
			case 0x39: usage = L"Error"; break;
			case 0x3A: usage = L"Usage Selected Indicator"; break;
			case 0x3B: usage = L"Usage In Use Indicator"; break;
			case 0x3C: usage = L"Usage Multi Mode Indicator"; break;
			case 0x3D: usage = L"Indicator On"; break;
			case 0x3E: usage = L"Indicator Flash"; break;
			case 0x3F: usage = L"Indicator Slow Blink"; break;
			case 0x40: usage = L"Indicator Fast Blink"; break;
			case 0x41: usage = L"Indicator Off"; break;
			case 0x42: usage = L"Flash On Time"; break;
			case 0x43: usage = L"Slow Blink On Time"; break;
			case 0x44: usage = L"Slow Blink Off Time"; break;
			case 0x45: usage = L"Fast Blink On Time"; break;
			case 0x46: usage = L"Fast Blink Off Time"; break;
			case 0x47: usage = L"Usage Indicator Color"; break;
			case 0x48: usage = L"Indicator Red"; break;
			case 0x49: usage = L"Indicator Green"; break;
			case 0x4A: usage = L"Indicator Amber"; break;
			case 0x4B: usage = L"Generic Indicator"; break;
			case 0x4C: usage = L"System Suspend"; break;
			case 0x4D: usage = L"External Power Connected"; break;
			default: usage = L"Reserved"; break;
		}
		break;
	case 0x09:
		pageName = L"Button";
		switch( id ) {
			case 0x00: usage = L"No button pressed"; break;
			default: {
				usage.clear();
				std::wstringstream usageStream;
				usageStream << L"Button " << id;
				usage = usageStream.str();
				break;
			}
		}
		break;
	case 0x0A:
		pageName = L"Ordinal";
		usage = L"Usage not implemented";	// �����g��Ȃ��̂ŏ����ĂȂ�
		break;
	case 0x0B:
		pageName = L"Telephony";
		usage = L"Usage not implemented";	// �����g��Ȃ��̂ŏ����ĂȂ�
		break;
	case 0x0C:
		pageName = L"Consumer";
		usage = L"Usage not implemented";	// �����g��Ȃ��̂ŏ����ĂȂ�
		break;
	case 0x0D:
		pageName = L"Digitizer";
		usage = L"Usage not implemented";	// �����g��Ȃ��̂ŏ����ĂȂ�
		break;
	case 0x0F:
		pageName = L"PID Page USB Physical Interface Device definitions for force feedback and related devices.";
		usage = L"Unknown";	// �����ɂȂ�
		break;
	case 0x10:
		pageName = L"Unicode";
		usage = L"Usage not implemented";	// �����g��Ȃ��̂ŏ����ĂȂ�
		break;
	case 0x14:
		pageName = L"Alphanumeric Display";
		usage = L"Usage not implemented";	// �����g��Ȃ��̂ŏ����ĂȂ�
		break;
	case 0x40:
		pageName = L"Medical Instruments";
		usage = L"Usage not implemented";	// �����g��Ȃ��̂ŏ����ĂȂ�
		break;
	case 0x80:
	case 0x81:
	case 0x82:
	case 0x83:
		pageName = L"Monitor pages USB Device Class Definition for Monitor Devices";
		usage = L"Usage not implemented";	// �����g��Ȃ��̂ŏ����ĂȂ�
		break;
	case 0x84:
	case 0x85:
	case 0x86:
	case 0x87:
		pageName = L"Power pages USB Device Class Definition for Power Devices";
		usage = L"Usage not implemented";	// �����g��Ȃ��̂ŏ����ĂȂ�
		break;
	case 0x8C:
		pageName = L"Bar Code Scanner page";
		usage = L"Usage not implemented";	// �����g��Ȃ��̂ŏ����ĂȂ�
		break;
	case 0x8D:
		pageName = L"Scale page";
		usage = L"Usage not implemented";	// �����g��Ȃ��̂ŏ����ĂȂ�
		break;
	case 0x8E:
		pageName = L"Magnetic Stripe Reading (MSR) Devices";
		usage = L"Usage not implemented";	// �����g��Ȃ��̂ŏ����ĂȂ�
		break;
	case 0x8F:
		pageName = L"Reserved Point of Sale pages USB Device Class Definition for Point of Sale Devices";
		usage = L"Usage not implemented";	// �����g��Ȃ��̂ŏ����ĂȂ�
		break;
	case 0x90:
		pageName = L"Camera Control Page USB Device Class Defin";
		usage = L"Usage not implemented";	// �����g��Ȃ��̂ŏ����ĂȂ�
		break;
	case 0x91:
		pageName = L"Arcade Page OAAF Definitions for arcade and coinop related Devices";
		usage = L"Usage not implemented";	// �����g��Ȃ��̂ŏ����ĂȂ�
		break;
	default:
		if( page >= 0xFF00 && page <= 0xFFFF ) {
			pageName = L"Vendor defined";
		} else {
			pageName = L"Reserved";
		}
		usage = L"Usage not implemented";	// �����g��Ȃ��̂ŏ����ĂȂ�
		break;
	}
}

}; // namespace gamepad


