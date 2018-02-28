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

#ifndef __GAME_PAD_X_INPUT_DEVICE_H__
#define __GAME_PAD_X_INPUT_DEVICE_H__

#include "GamePadNullInputDevice.h"
#include <xinput.h>


namespace gamepad {

class CXInputDevice : public CInputDeviceBase
{
public:
	enum EnableUserIndex {
		UserIndex0 = 0x0001,
		UserIndex1 = 0x0002,
		UserIndex2 = 0x0004,
		UserIndex3 = 0x0008,
		UserIndexEOT,
	};

private:
	unsigned long		device_num_;
	XINPUT_STATE		state_;
	XINPUT_VIBRATION	cur_vibration_;
	XINPUT_VIBRATION	vibration_;

	bool	is_connected_;

private:
	static double GetAxisValue( short val, short deadZone );

public:
	CXInputDevice( unsigned long num, CInputDevicePort* port);
	virtual ~CXInputDevice() {}

	void Update();

	long GetTypeOfDevice() const { return IDT_XInput; }

	unsigned long GetKeyState() const;
	double GetLeftTrigger() const;
	double GetRightTrigger() const;
	double GetLeftThumbStickX() const;
	double GetLeftThumbStickY() const;
	double GetRightThumbStickX() const;
	double GetRightThumbStickY() const;

	void SetLeftVibration( double val )
	{
		if( val < 0.0 ) val = 0.0;
		else if( val > 1.0 ) val = 1.0;
		vibration_.wLeftMotorSpeed = (WORD)(65535 * val);
	}
	void SetRightVibration( double val )
	{
		if( val < 0.0 ) val = 0.0;
		else if( val > 1.0 ) val = 1.0;
		vibration_.wRightMotorSpeed = (WORD)(65535 * val);
	}

	unsigned long Sensing() const;

public:
	//! XInput �̏����� �� �Ȃ����Ă���p�b�h�̃`�F�b�N
	static long InitializeXInput();
};

}; // namespace gamepad


#endif // __GAME_PAD_X_INPUT_DEVICE_H__

