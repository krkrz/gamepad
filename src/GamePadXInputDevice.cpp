/****************************************************************************/
/*! @file
@brief XInput �f�o�C�X

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



#include "GamePadXInputDevice.h"
#include "GamePadDLLLoader.h"

namespace gamepad {

/*
DWORD WINAPI XInputGetState( DWORD dwUserIndex, XINPUT_STATE* pState );
DWORD WINAPI XInputSetState( DWORD dwUserIndex, XINPUT_VIBRATION* pVibration );
DWORD WINAPI XInputGetCapabilities( DWORD dwUserIndex, DWORD dwFlags, XINPUT_CAPABILITIES* pCapabilities );
void WINAPI XInputEnable( BOOL enable );
DWORD WINAPI XInputGetDSoundAudioDeviceGuids( DWORD dwUserIndex, GUID* pDSoundRenderGuid, GUID* pDSoundCaptureGuid );
DWORD WINAPI XInputGetBatteryInformation( DWORD dwUserIndex, BYTE devType, XINPUT_BATTERY_INFORMATION* pBatteryInformation );
DWORD WINAPI XInputGetKeystroke( DWORD dwUserIndex, DWORD dwReserved, PXINPUT_KEYSTROKE pKeystroke );
*/
class xinput
{
	static CDLLLoader			dll_;

	static bool LoadFunctions();
	static bool CheckLoading();

public:
	typedef DWORD (WINAPI *FuncXInputGetState)( DWORD dwUserIndex, XINPUT_STATE* pState );
	typedef DWORD (WINAPI *FuncXInputSetState)( DWORD dwUserIndex, XINPUT_VIBRATION* pVibration );
	typedef DWORD (WINAPI *FuncXInputGetCapabilities)( DWORD dwUserIndex, DWORD dwFlags, XINPUT_CAPABILITIES* pCapabilities );
	typedef void (WINAPI *FuncXInputEnable)( BOOL enable );
// �ȉ��̃��\�b�h�͎g��Ȃ�
//	typedef DWORD (WINAPI *FuncXInputGetDSoundAudioDeviceGuids)( DWORD dwUserIndex, GUID* pDSoundRenderGuid, GUID* pDSoundCaptureGuid );
//	typedef DWORD (WINAPI *FuncXInputGetBatteryInformation)( DWORD dwUserIndex, BYTE devType, XINPUT_BATTERY_INFORMATION* pBatteryInformation );
//	typedef DWORD (WINAPI *FuncXInputGetKeystroke)( DWORD dwUserIndex, DWORD dwReserved, PXINPUT_KEYSTROKE pKeystroke );

public:
	static bool Initialize();

	static FuncXInputGetState						GetState;
	static FuncXInputSetState						SetState;
	static FuncXInputGetCapabilities				GetCapabilities;
	static FuncXInputEnable							Enable;
// �ȉ��̃��\�b�h�͎g��Ȃ�
//	static FuncXInputGetDSoundAudioDeviceGuids		GetDSoundAudioDeviceGuids;
//	static FuncXInputGetBatteryInformation			GetBatteryInformation;
//	static FuncXInputGetKeystroke					GetKeystroke;
};

CDLLLoader xinput::dll_;
xinput::FuncXInputGetState						xinput::GetState = NULL;
xinput::FuncXInputSetState						xinput::SetState = NULL;
xinput::FuncXInputGetCapabilities				xinput::GetCapabilities = NULL;
xinput::FuncXInputEnable						xinput::Enable = NULL;

// �ȉ��̃��\�b�h�͎g��Ȃ�
//xinput::FuncXInputGetDSoundAudioDeviceGuids		xinput::GetDSoundAudioDeviceGuids = NULL;
//xinput::FuncXInputGetBatteryInformation			xinput::GetBatteryInformation = NULL;
//xinput::FuncXInputGetKeystroke					xinput::GetKeystroke = NULL;

bool xinput::Initialize()
{
	if( !GetState || !SetState || !GetCapabilities || !Enable ) {
		return LoadFunctions();
	}
	return true;	// �ǂݍ��ݍς�
}
bool xinput::CheckLoading()
{
	if( dll_.IsLoaded() == false ) {
		dll_.Load( "xinput1_3.dll" );
	}
// �Â��̂������H
#if 0
	if( dll_.IsLoaded() == false ) {
		dll_.Load( "xinput1_2.dll" );
	}
	if( dll_.IsLoaded() == false ) {
		dll_.Load( "xinput1_1.dll" );
	}
#endif
	return dll_.IsLoaded();
}
bool xinput::LoadFunctions()
{
	if( !CheckLoading() ) {
		return false;
	}

	GetState = (FuncXInputGetState)dll_.GetProcAddress( "XInputGetState" );
	if( !GetState ) { return false; }

	SetState = (FuncXInputSetState)dll_.GetProcAddress( "XInputSetState" );
	if( !SetState ) { return false; }

	GetCapabilities = (FuncXInputGetCapabilities)dll_.GetProcAddress( "XInputGetCapabilities" );
	if( !GetCapabilities ) { return false; }

	Enable = (FuncXInputEnable)dll_.GetProcAddress( "XInputEnable" );
	if( !Enable ) { return false; }

// �ȉ��̃��\�b�h�͎g��Ȃ�
/*
	GetDSoundAudioDeviceGuids = (FuncXInputGetDSoundAudioDeviceGuids)dll_.GetProcAddress( "XInputGetDSoundAudioDeviceGuids" );
	if( !GetDSoundAudioDeviceGuids ) { return false; }

	GetBatteryInformation = (FuncXInputGetBatteryInformation)dll_.GetProcAddress( "XInputGetBatteryInformation" );
	if( !GetBatteryInformation ) { return false; }

	GetKeystroke = (FuncXInputGetKeystroke)dll_.GetProcAddress( "XInputGetKeystroke" );
	if( !GetKeystroke ) { return false; }
*/
	return true;
}
CXInputDevice::CXInputDevice( unsigned long num, CInputDevicePort* port)
 : CInputDeviceBase(port), device_num_(num), is_connected_( true )
{
	ZeroMemory( &state_, sizeof(state_) );
	ZeroMemory( &cur_vibration_, sizeof(cur_vibration_) );
	ZeroMemory( &vibration_, sizeof(vibration_) );

	wchar_t		name_num[2] = {L'\0',L'\0'};
	if( num < 10 ) {
		name_num[0] = L'0' + (wchar_t)num;
	}
	name_ = std::wstring(L"XInput Game Pad ") + std::wstring( (wchar_t*)name_num );
}
void CXInputDevice::Update()
{
	DWORD ret = 0;
	if( is_connected_ ) {
		if( cur_vibration_.wLeftMotorSpeed != vibration_.wLeftMotorSpeed || 
			cur_vibration_.wRightMotorSpeed != vibration_.wRightMotorSpeed ) {
			ret = xinput::SetState( device_num_, &vibration_ );
			cur_vibration_ = vibration_;
			is_connected_ = (ret != ERROR_DEVICE_NOT_CONNECTED);
		}
		if( is_connected_ ) {
			ret = xinput::GetState( device_num_, &state_ );
			is_connected_ = (ret != ERROR_DEVICE_NOT_CONNECTED);
		}
		if( !is_connected_ ) {
			// �����ꂽ�͗l�B�[���N���A���Ă���
			ZeroMemory( &state_, sizeof(state_) );
			ZeroMemory( &cur_vibration_, sizeof(cur_vibration_) );
		}
	} else {
		// �r���Őؒf���ꂽ�̂ŁA�Ȃ����Ă��邩�`�F�b�N�B
		XINPUT_CAPABILITIES	cap;
		ret = xinput::GetCapabilities( device_num_, XINPUT_FLAG_GAMEPAD, &cap );
		is_connected_ = (ret != ERROR_DEVICE_NOT_CONNECTED);
	}
}
unsigned long CXInputDevice::GetKeyState() const
{
	return state_.Gamepad.wButtons;
#if 0
	unsigned long	ret = 0;
	WORD	btn = state_.Gamepad.wButtons;
	if( btn & XINPUT_GAMEPAD_DPAD_UP ) ret |= IDBTN_DPAD_UP;
	if( btn & XINPUT_GAMEPAD_DPAD_DOWN ) ret |= IDBTN_DPAD_DOWN;
	if( btn & XINPUT_GAMEPAD_DPAD_LEFT ) ret |= IDBTN_DPAD_LEFT;
	if( btn & XINPUT_GAMEPAD_DPAD_RIGHT ) ret |= IDBTN_DPAD_RIGHT;
	if( btn & XINPUT_GAMEPAD_START ) ret |= IDBTN_START;
	if( btn & XINPUT_GAMEPAD_BACK ) ret |= IDBTN_BACK;
	if( btn & XINPUT_GAMEPAD_LEFT_THUMB ) ret |= IDBTN_LEFT_THUMB;
	if( btn & XINPUT_GAMEPAD_RIGHT_THUMB ) ret |= IDBTN_RIGHT_THUMB;
	if( btn & XINPUT_GAMEPAD_LEFT_SHOULDER ) ret |= IDBTN_LEFT_SHOULDER;
	if( btn & XINPUT_GAMEPAD_RIGHT_SHOULDER ) ret |= IDBTN_RIGHT_SHOULDER;
	if( btn & XINPUT_GAMEPAD_A ) ret |= IDBTN_A;
	if( btn & XINPUT_GAMEPAD_B ) ret |= IDBTN_B;
	if( btn & XINPUT_GAMEPAD_X ) ret |= IDBTN_X;
	if( btn & XINPUT_GAMEPAD_Y ) ret |= IDBTN_Y;
	return ret;
#endif
}
double CXInputDevice::GetAxisValue( short val, short deadZone )
{
	if( deadZone >= val && val >= (-deadZone) ) {
		return 0.0;
	} else if( val >= 0 ) {
		return (double)(val-deadZone) / (double)(32767-deadZone);
	} else {
		return (double)(val+deadZone) / (double)(32768-deadZone);
	}
}
double CXInputDevice::GetLeftTrigger() const
{
	return state_.Gamepad.bLeftTrigger / 255.0;
}
double CXInputDevice::GetRightTrigger() const
{
	return state_.Gamepad.bRightTrigger / 255.0;
}
double CXInputDevice::GetLeftThumbStickX() const
{
	return GetAxisValue( state_.Gamepad.sThumbLX, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE );
}
double CXInputDevice::GetLeftThumbStickY() const
{
	return GetAxisValue( state_.Gamepad.sThumbLY, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE );
}
double CXInputDevice::GetRightThumbStickX() const
{
	return GetAxisValue( state_.Gamepad.sThumbRX, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE );
}
double CXInputDevice::GetRightThumbStickY() const
{
	return GetAxisValue( state_.Gamepad.sThumbRY, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE );
}
unsigned long CXInputDevice::Sensing() const
{
	unsigned long ret = state_.Gamepad.wButtons;

	short deadZone = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
	if( state_.Gamepad.sThumbLX > deadZone || state_.Gamepad.sThumbLX < (-deadZone) )
		ret |= IDBTN_LEFT_THUMB_AXIS_X;

	if( state_.Gamepad.sThumbLY > deadZone || state_.Gamepad.sThumbLY < (-deadZone) )
		ret |= IDBTN_LEFT_THUMB_AXIS_Y;

	deadZone = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
	if( state_.Gamepad.sThumbRX > deadZone || state_.Gamepad.sThumbRX < (-deadZone) )
		ret |= IDBTN_RIGHT_THUMB_AXIS_X;

	if( state_.Gamepad.sThumbRY > deadZone || state_.Gamepad.sThumbRY < (-deadZone) )
		ret |= IDBTN_RIGHT_THUMB_AXIS_Y;

	if( state_.Gamepad.bLeftTrigger )
		ret |= IDBTN_LEFT_TRIGGER;

	if( state_.Gamepad.bRightTrigger )
		ret |= IDBTN_RIGHT_TRIGGER;

	return ret;
}

// XInput �f�o�C�X�̏��������s��
long CXInputDevice::InitializeXInput()
{
	bool	is_xinput_enable = xinput::Initialize();
	if( !is_xinput_enable ) return 0;

	xinput::Enable( TRUE );

	long	ret = 0;
	for( DWORD i = 0; i < 4; ++i ) {
		XINPUT_CAPABILITIES	cap;
		DWORD caps = xinput::GetCapabilities( i, XINPUT_FLAG_GAMEPAD, &cap );
		if( caps == ERROR_SUCCESS ) {
			// connect xinput device
			ret |= 0x01 << i;
		} // else if( ret == ERROR_DEVICE_NOT_CONNECTED ) {}
	}
	if( ret == 0 ) {
		xinput::Enable( FALSE );
	}
	return ret;
}

}; // namespace gamepad


