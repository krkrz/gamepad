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

#include "GamePadForceFeedbackDevice.h"
#include "GamePadLog.h"

#define _USE_MATH_DEFINES 
#include <cmath> 

namespace gamepad {

CForceFeedbackDevice::CForceFeedbackDevice( IDirectInputDevice8* input, CInputDevicePort* port )
 : CDirectInputDevice(input,port), left_magnitude_(0.0), right_magnitude_(0.0)
{
}

// �t�H�[�X�t�B�[�h�o�b�O�f�o�C�X��������
bool CForceFeedbackDevice::Initialize( HWND hWnd )
{
	if( !device_ ) {
		return false;
	}

	HRESULT		hr;
	if( FAILED( hr = device_->SetDataFormat( &c_dfDIJoystick ) ) ) {
		Log( L"Failed to Call SetDataFormat.", hr );
		return false;
	}

	// �t�H�[�X�t�B�[�h�o�b�N���g���ꍇ�ADISCL_EXCLUSIVE ���w�肷��K�v������
	if( FAILED( hr = device_->SetCooperativeLevel( hWnd, DISCL_EXCLUSIVE |  DISCL_FOREGROUND ) ) ) {
		Log( L"Failed to Call SetCooperativeLevel.", hr );
		return false;
	}

	// �t�H�[�X�t�B�[�h�o�b�N���g���ꍇ�Aauto-centering �����ɂȂ�炵���̂ŁA�����ɂ���
	DIPROPDWORD dipdw;
	dipdw.diph.dwSize		= sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize	= sizeof(DIPROPHEADER);
	dipdw.diph.dwObj		= 0;
	dipdw.diph.dwHow		= DIPH_DEVICE;
	dipdw.dwData			= FALSE;
	if( FAILED( hr = device_->SetProperty( DIPROP_AUTOCENTER, &dipdw.diph ) ) ) {
		Log( L"Failed to Call SetProperty( DIPROP_AUTOCENTER ).", hr );
		return false;
	}

	if( !CDirectInputDevice::InitializeDeviceDetail() ) {
		return false;
	}

	// �t�H�[�X�t�B�[�h�o�b�N������̂�2�܂ł̃T�|�[�g�ɂ���
	if( enable_objs_.forcefeedback > 2 ) {
		Log( L"This system support less than 3 axes force freedback. rewrite number of axes." );
		enable_objs_.forcefeedback = 2;
	}

	// DirectInput �̃t�H�[�X�t�B�[�h�o�b�N�ł͂��Ȃ肢�낢��Ȑݒ肪�o���邪�A�������g���ƕ��G�ɂȂ��Ă��܂����߁A
	// �R���X�^���g�t�H�[�X��p���āA�\�t�g���Œl��ς��ăR���g���[������悤�ɂ���B
	// XInput �͂��̂悤�ȃX�^�C���̂悤�Ȃ̂ŁA����ƍ��킹��B
	rgdw_axes_[0] = DIJOFS_X;
	rgdw_axes_[1] = DIJOFS_Y;
	rgl_direction_[0] = 0;
	rgl_direction_[1] = 0;
	cf_.lMagnitude = 0;

	ZeroMemory( &effect_param_, sizeof(effect_param_) );
	effect_param_.dwSize					= sizeof(DIEFFECT);
	effect_param_.dwFlags					= DIEFF_CARTESIAN | DIEFF_OBJECTOFFSETS;	// �������W�Œl�w�� 
	effect_param_.dwDuration				= INFINITE;
	effect_param_.dwSamplePeriod			= 0;
	effect_param_.dwGain					= DI_FFNOMINALMAX;
	effect_param_.dwTriggerButton			= DIEB_NOTRIGGER;
	effect_param_.dwTriggerRepeatInterval	= 0;
	effect_param_.cAxes						= enable_objs_.forcefeedback;
	effect_param_.rgdwAxes					= rgdw_axes_;
	effect_param_.rglDirection				= rgl_direction_;
	effect_param_.lpEnvelope				= 0;
	effect_param_.cbTypeSpecificParams		= sizeof(DICONSTANTFORCE);
	effect_param_.lpvTypeSpecificParams		= &cf_;
	effect_param_.dwStartDelay				= 0;
	if( FAILED( hr = device_->CreateEffect( GUID_ConstantForce, &effect_param_, &effect_, NULL ) ) ) {
		Log( L"Failed to Call CreateEffect.", hr );
		return false;
	}

	return true;
}

void CForceFeedbackDevice::SetLeftVibration( double val )
{
	if( val < 0.0 ) val = 0.0;
	if( val > 1.0 ) val = 1.0;
	left_magnitude_ = val;
}
void CForceFeedbackDevice::SetRightVibration( double val )
{
	if( val < 0.0 ) val = 0.0;
	if( val > 1.0 ) val = 1.0;
	right_magnitude_ = val;
}
//! �t�H�[�X�t�B�[�h�o�b�N�����s�B�k��������
bool CForceFeedbackDevice::UpdateFF()
{
	// �G�t�F�N�g���Ȃ���c
	if( !effect_ ) {
		return false;
	}

	// �t�H�[�X�t�B�[�h�o�b�N�����Ȃ���c
	if( enable_objs_.forcefeedback == 0 )
		return true;

	int	left_force = (int)(DI_FFNOMINALMAX * left_magnitude_);
	int	right_force = (int)(DI_FFNOMINALMAX * right_magnitude_);

	if( enable_objs_.forcefeedback == 1 ) {
		// 1�����̎��́A�������ƍl����
		cf_.lMagnitude = left_force;
		rgl_direction_[0] = 0;
		rgl_direction_[1] = 0;
	} else {
		// 2���̎��́A���l�ƃ}�O�j�`���[�h�Ő���
		rgl_direction_[0] = left_force;
		rgl_direction_[1] = right_force;
		cf_.lMagnitude = (DWORD)sqrt( (double)left_force * (double)left_force + (double)right_force * (double)right_force );
	}

	// �t�H�[�X��K�p�I �k����I
	HRESULT	hr;
	if( FAILED( hr = effect_->SetParameters( &effect_param_, DIEP_DIRECTION | DIEP_TYPESPECIFICPARAMS | DIEP_START ) ) ) {
		Log( L"Failed to Call SetParameters.", hr );
		return false;
	}
	return true;
}



}; // namespace gamepad


