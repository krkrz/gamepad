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

#ifndef __GAME_PAD_DIRECT_INPUT_DEVICE_H__
#define __GAME_PAD_DIRECT_INPUT_DEVICE_H__

#include <windows.h>
#include <dinput.h>
#include <string>
#include <atlbase.h>
#include "GamePadInputDeviceBase.h"
#include "GamePadDeviceInspector.h"

namespace gamepad {

//! �{�^���⎲�̑Ή���\��
//! Slider �̏󋵂ɂ���Ă͕ύX���邩��
struct DirectInputObjectMap
{
	int		obj_num;
	int		direction;	// �������]���Ă��鎞�� -1�A�����łȂ����� 1�BPOV �� �{�^���̎��͈Ӗ��Ȃ�
};

enum DirectInputObjectNum {
	DIObj_AxisX = 0,
	DIObj_AxisY = 1,
	DIObj_AxisZ = 2,
	DIObj_AxisRotX = 3,
	DIObj_AxisRotY = 4,
	DIObj_AxisRotZ = 5,
	DIObj_Slider_0 = 6,
	DIObj_Slider_1 = 7,
	DIObj_POV_0 = 8,
	DIObj_POV_1 = 9,
	DIObj_POV_2 = 10,
	DIObj_POV_3 = 11,
	DIObj_Button1 = 12,
	DIObj_Button2 = 13,
	DIObj_Button3 = 14,
	DIObj_Button4 = 15,
	DIObj_Button5 = 16,
	DIObj_Button6 = 17,
	DIObj_Button7 = 18,
	DIObj_Button8 = 19,
	DIObj_Button9 = 20,
	DIObj_Button10 = 21,
	DIObj_Button11 = 22,
	DIObj_Button12 = 23,
	DIObj_Button13 = 24,
	DIObj_Button14 = 25,
	DIObj_Button15 = 26,
	DIObj_Button16 = 27,
	DIObj_Button17 = 28,
	DIObj_Button18 = 29,
	DIObj_Button19 = 30,
	DIObj_Button20 = 31,
	DIObj_Button21 = 32,
	DIObj_Button22 = 33,
	DIObj_Button23 = 34,
	DIObj_Button24 = 35,
	DIObj_Button25 = 36,
	DIObj_Button26 = 37,
	DIObj_Button27 = 38,
	DIObj_Button28 = 39,
	DIObj_Button29 = 40,
	DIObj_Button30 = 41,
	DIObj_Button31 = 42,
	DIObj_Button32 = 43,
	DIObj_Disable = 44,
	DIObj_EOT
};

enum DirectInputButtonMapNum {
	DIBMAP_NUM_DPAD_UP			= 0,	// �\���L�[ ��
	DIBMAP_NUM_DPAD_DOWN		= 1,	// �\���L�[ ��
	DIBMAP_NUM_DPAD_LEFT		= 2,	// �\���L�[ ��
	DIBMAP_NUM_DPAD_RIGHT		= 3,	// �\���L�[ �E
	DIBMAP_NUM_START			= 4,	// START
	DIBMAP_NUM_BACK				= 5,	// BACK
	DIBMAP_NUM_LEFT_THUMB		= 6,	// ���T���{�^��
	DIBMAP_NUM_RIGHT_THUMB		= 7,	// �E�T���{�^��
	DIBMAP_NUM_LEFT_SHOULDER	= 8,	// �����{�^��
	DIBMAP_NUM_RIGHT_SHOULDER	= 9,	// �E���{�^��
	DIBMAP_NUM_A				= 10,	// A�{�^��
	DIBMAP_NUM_B				= 11,	// B�{�^��
	DIBMAP_NUM_X				= 12,	// X�{�^��
	DIBMAP_NUM_Y				= 13,	// Y�{�^��
	DIBMAP_NUM_EOT
};

enum DirectInputTriggerMapNum {
	DITMAP_NUM_LEFT = 0,
	DITMAP_NUM_RIGHT = 1,
	DITMAP_NUM_EOT
};
enum DirectInputAxisMapNum {
	DIAMAP_NUM_LX = 0,		// ���X�e�B�b�NX��
	DIAMAP_NUM_LY = 1,		// ���X�e�B�b�NY��
	DIAMAP_NUM_RX = 2,		// �E�X�e�B�b�NX��
	DIAMAP_NUM_RY = 3,		// �E�X�e�B�b�NY��
	DIAMAP_NUM_EOT
};

struct DirectInputObjectMappingTable
{
	DirectInputObjectMap	button[DIBMAP_NUM_EOT];	// �\���L�[�ƃ{�^��
	DirectInputObjectMap	trigger[DITMAP_NUM_EOT];
	DirectInputObjectMap	axis[DIAMAP_NUM_EOT];
	bool					leftforcefirst;
};


class CDirectInputDevice : public CInputDeviceBase
{
protected:
	struct DirectInputObjects
	{
		static const int AXIS_X		= 0x01 << 0;
		static const int AXIS_Y		= 0x01 << 1;
		static const int AXIS_Z		= 0x01 << 2;
		static const int AXIS_ROT_X	= 0x01 << 3;
		static const int AXIS_ROT_Y	= 0x01 << 4;
		static const int AXIS_ROT_Z	= 0x01 << 5;

		int		axis;
		int		slider;
		int		pov;
		int		button;
		int		forcefeedback;

		DirectInputObjects()
		: axis(0), slider(0), pov(0), button(0), forcefeedback(0)
		{}
		void clear()
		{
			axis = 0;
			slider = 0;
			pov = 0;
			button = 0;
			forcefeedback = 0;
		}
	};
	struct DirectInputState {
		DIJOYSTATE	state;
		BYTE		dummy[4];	//!< �_�~�[�z��BDIJOYSTATE �̃{�^���z��𒴂��ăA�N�Z�X���Ă������ɂȂ�悤�ɂ���
	};

	DirectInputObjects		enable_objs_;
	CGamePadDeviceInspector	inspector_;

	CComPtr<IDirectInputDevice8>	device_;

	WORD	vendor_id_;		//!< �x���_�[ID
	WORD	product_id_;	//!< �v���_�N�gID

	GUID	instance_guid_;	//!< �C���X�^���XID �Đ������鎞�ɕK�v

	DirectInputState	state_;

	DirectInputObjectMappingTable	key_map_;	//!< �e�{�^���Ȃǂ̊��蓖��

	bool		device_creating_;	//!< �f�o�C�X�̍Đ��������ǂ���
	bool		request_poll_method_;

	std::wstring	physical_port_name_;	//!< �ڑ�����Ă���|�[�g�̖��O

	static const long		AXIS_RANGE_MAX = +32767;
	static const long		AXIS_RANGE_MIN = -32768;
	static const long		AXIS_RANGE = 65535;

	static const long		DEADZONE = 8689;

	void ClearState() {
		ZeroMemory( &state_, sizeof(state_) );
		state_.state.rgdwPOV[0] = -1;
		state_.state.rgdwPOV[1] = -1;
		state_.state.rgdwPOV[2] = -1;
		state_.state.rgdwPOV[3] = -1;
	}

	static BOOL CALLBACK EnumObjectsCallback( const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext );
	bool EnumObjectsCallback( const DIDEVICEOBJECTINSTANCE* pdidoi );

	void RetrieveVendorAndProductID();
	void RetrieveConnectedPortName();

	bool InitializeDeviceDetail();

	bool Acquire();

	void RecreateDevice();

	//! �S�ẴL�[�������ɂȂ�悤�ɐݒ肷��
	void ClearKeyMap();

	//! HID �� page �� usage �𓾂�
	void GetHIDDesc( WORD page, WORD id, std::wstring& pageName, std::wstring& usage );

	// POV �l���牟����Ă��邩�ǂ����𔻒肷��
	static inline bool IsPressUpForPovVal( DWORD pov ) {
		return ( ( pov >= 0 && pov < 9000 ) || ( pov > 27000 && pov <= 36000 ) );
	}
	static inline bool IsPressDownForPovVal( DWORD pov ) { return ( pov > 9000 && pov < 27000 ); }
	static inline bool IsPressLeftForPovVal( DWORD pov ) { return ( pov > 18000 && pov < 36000 ); }
	static inline bool IsPressRightForPovVal( DWORD pov ) { return ( pov > 0 && pov < 18000 ); }
	static double GetAxisValue( long val, int dir );

	bool IsPressPlusKey( long key, int num, int dir ) const;
	double GetThumbStickValue( int num, int dir, bool h ) const;
	double GetTriggerValue( int num ) const;

	//! �o���邾���p�b�h�ɍ����L�[�}�b�s���O���s��
	void DoAutoKeyMapping();

	//! Force Feedback Device�̐ݒ�𔽉f����
	virtual bool UpdateFF() { return true; }
public:
	CDirectInputDevice( IDirectInputDevice8* input, CInputDevicePort* port );
	virtual ~CDirectInputDevice() {}

	virtual bool Initialize( HWND hWnd );

	virtual void Update();

	virtual long GetTypeOfDevice() const { return IDT_DirectInput; };

	virtual unsigned long GetKeyState() const;
	virtual double GetLeftTrigger() const;
	virtual double GetRightTrigger() const;
	virtual double GetLeftThumbStickX() const;
	virtual double GetLeftThumbStickY() const;
	virtual double GetRightThumbStickX() const;
	virtual double GetRightThumbStickY() const;

	//! ��t�H�[�X�t�B�[�h�o�b�N�f�o�C�X�Ƀo�C�u�͂Ȃ�
	virtual void SetLeftVibration( double val ) {}
	virtual void SetRightVibration( double val ) {}

	virtual unsigned long Sensing() const;

	// DirectInput �p
//	virtual int StartSensingRow( bool multikey );
//	virtual unsigned long SensingRow() const;

#if 0
	virtual void SetButtonMapping( int id, int dkey );
	virtual void SetAxisMapping( int id, int dkey, int dir );
	virtual void SetTriggerMapping( int id, int dkey );
	virtual void SetLeftForceFirst( bool b );

	virtual int GetButtonMapping( int id );
	virtual int GetAxisMapping( int id );
	virtual int GetAxisDirectionMapping( int id );
	virtual int GetTriggerMapping( int id );
	virtual bool GetLeftForceFirst();

	//! �w�肳�ꂽ�L�[�̃}�b�s���O�����݂�
	// ���Ƀ{�^����ݒ肷�鎞�ȂǓ���ȃG���[�R�[�h���`����K�v����
	virtual int SensingMapping( int id, int dir );
#endif
};

}; // namespace gamepad


#endif // __GAME_PAD_DIRECT_INPUT_DEVICE_H__

