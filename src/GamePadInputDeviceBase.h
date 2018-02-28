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

#ifndef __GAME_PAD_INPUT_DEVICE_BASE_H__
#define __GAME_PAD_INPUT_DEVICE_BASE_H__

#include "GamePadInputDevice.h"
#include <assert.h>

namespace gamepad {

class CInputDeviceBase : public IInputDevice
{

protected:

	std::wstring	name_;

	CInputDevicePort* port_;
	CInputDevicePort* Port() {
		assert(port_);
		return port_;
	}

public:
	CInputDeviceBase(CInputDevicePort* port) : port_(port) {}
	virtual ~CInputDeviceBase() {}

	virtual bool Initialize( HWND hWnd ) { return true; }

	virtual void Update() {}

	virtual void GetDeviceName( std::wstring& name ) const { name = name_; }

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


#endif // __GAME_PAD_INPUT_DEVICE_BASE_H__


