/****************************************************************************/
/*! @file
@brief 

-----------------------------------------------------------------------------
	Copyright (C) 2008 Takenori Imoto (�䌳 ����). All rights reserved.
	( http://www.kaede-software.com/ )
-----------------------------------------------------------------------------
@author		T.Imoto
@date		2008/01/11
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

#ifndef __GAME_PAD_INPUT_DEVICE_H__
#define __GAME_PAD_INPUT_DEVICE_H__

#include <string>
#include <windows.h>

namespace gamepad {

enum InputDeviceType {
	IDT_Null = 0,
	IDT_XInput = 1,
	IDT_DirectInputFF = 2,
	IDT_DirectInput = 3,
};
// �{�^���n�̃t���O�́AXInput�Ɠ����ɂ��Ă���
enum InputDeviceButtonFlag {
	IDBTN_DPAD_UP			= 0x00000001,	// �\���L�[ ��
	IDBTN_DPAD_DOWN			= 0x00000002,	// �\���L�[ ��
	IDBTN_DPAD_LEFT			= 0x00000004,	// �\���L�[ ��
	IDBTN_DPAD_RIGHT		= 0x00000008,	// �\���L�[ �E
	IDBTN_START				= 0x00000010,	// START
	IDBTN_BACK				= 0x00000020,	// BACK
	IDBTN_LEFT_THUMB		= 0x00000040,	// ���T���{�^��
	IDBTN_RIGHT_THUMB		= 0x00000080,	// �E�T���{�^��
	IDBTN_LEFT_SHOULDER		= 0x00000100,	// �����{�^��
	IDBTN_RIGHT_SHOULDER	= 0x00000200,	// �E���{�^��
	IDBTN_A					= 0x00001000,	// A�{�^��
	IDBTN_B					= 0x00002000,	// B�{�^��
	IDBTN_X					= 0x00004000,	// X�{�^��
	IDBTN_Y					= 0x00008000,	// Y�{�^��
	IDBTN_LEFT_THUMB_AXIS_X	= 0x00010000,	// ���A�i���OX��
	IDBTN_LEFT_THUMB_AXIS_Y	= 0x00020000,	// ���A�i���OY��
	IDBTN_RIGHT_THUMB_AXIS_X= 0x00040000,	// �E�A�i���OX��
	IDBTN_RIGHT_THUMB_AXIS_Y= 0x00080000,	// �E�A�i���OY��
	IDBTN_LEFT_TRIGGER		= 0x00100000,	// ���g���K�[
	IDBTN_RIGHT_TRIGGER		= 0x00200000,	// �E�g���K�[
	IDBTN_EOT
};

enum InputDeviceButtonNum {
	IDBTN_NUM_DPAD_UP			= 0,	// �\���L�[ ��
	IDBTN_NUM_DPAD_DOWN			= 1,	// �\���L�[ ��
	IDBTN_NUM_DPAD_LEFT			= 2,	// �\���L�[ ��
	IDBTN_NUM_DPAD_RIGHT		= 3,	// �\���L�[ �E
	IDBTN_NUM_START				= 4,	// START
	IDBTN_NUM_BACK				= 5,	// BACK
	IDBTN_NUM_LEFT_THUMB		= 6,	// ���T���{�^��
	IDBTN_NUM_RIGHT_THUMB		= 7,	// �E�T���{�^��
	IDBTN_NUM_LEFT_SHOULDER		= 8,	// �����{�^��
	IDBTN_NUM_RIGHT_SHOULDER	= 9,	// �E���{�^��
	IDBTN_NUM_A					= 12,	// A�{�^��
	IDBTN_NUM_B					= 13,	// B�{�^��
	IDBTN_NUM_X					= 14,	// X�{�^��
	IDBTN_NUM_Y					= 15,	// Y�{�^��
	IDBTN_NUM_LEFT_THUMB_AXIS_X	= 16,	// ���A�i���OX��
	IDBTN_NUM_LEFT_THUMB_AXIS_Y	= 17,	// ���A�i���OY��
	IDBTN_NUM_RIGHT_THUMB_AXIS_X= 18,	// �E�A�i���OX��
	IDBTN_NUM_RIGHT_THUMB_AXIS_Y= 19,	// �E�A�i���OY��
	IDBTN_NUM_LEFT_TRIGGER		= 20,	// ���g���K�[
	IDBTN_NUM_RIGHT_TRIGGER		= 21,	// �E�g���K�[
	IDBTN_NUM_EOT
};

class CInputDevicePort;

// �f�o�C�X�̃��\�b�h���`����C���^�[�t�F�C�X�N���X
class IInputDevice
{
public:
	virtual ~IInputDevice() {}

	virtual bool Initialize( HWND hWnd ) = 0;

	virtual void Update() = 0;

	virtual void GetDeviceName( std::wstring& name ) const = 0;

	virtual long GetTypeOfDevice() const = 0;

	virtual unsigned long GetKeyState() const = 0;
	virtual double GetLeftTrigger() const = 0;
	virtual double GetRightTrigger() const = 0;
	virtual double GetLeftThumbStickX() const = 0;
	virtual double GetLeftThumbStickY() const = 0;
	virtual double GetRightThumbStickX() const = 0;
	virtual double GetRightThumbStickY() const = 0;

	virtual void SetLeftVibration( double val ) = 0;
	virtual void SetRightVibration( double val ) = 0;

	virtual unsigned long Sensing() const = 0;
};

}; // namespace gamepad

#endif // __GAME_PAD_INPUT_DEVICE_H__


