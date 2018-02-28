/****************************************************************************/
/*! @file
@brief 

-----------------------------------------------------------------------------
	Copyright (C) 2008 Takenori Imoto (�䌳 ����). All rights reserved.
	( http://www.kaede-software.com/ )
-----------------------------------------------------------------------------
@author		T.Imoto
@date		2008/05/28
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

#include "GamepadInputContext.h"
#include "GamePadDirectInputDevice.h"
#include <limits.h>

namespace gamepad {

CInputContext::CInputContext()
{
	ClearAll();
}

// �A�i���O�����L�[�𔻒�
void CInputContext::UpdateAxisState( int& stateMinus, int& statePlus, double current )
{
	if( current == 0.0 ) {
		if( stateMinus >= TOUCH_DOWN ) {
			// �O��͉�����Ă���
			stateMinus = 0;
		} else {
			// �����͌p��
			if( stateMinus != INT_MIN ) stateMinus--;
		}

		if( statePlus >= TOUCH_DOWN ) {
			statePlus = 0;
		} else {
			if( statePlus != INT_MIN ) statePlus--;
		}
	} else if( current > 0.0 ) {
		if( stateMinus >= TOUCH_DOWN ) {
			stateMinus = 0;
		} else {
			if( stateMinus != INT_MIN ) stateMinus--;
		}

		if( statePlus <= TOUCH_NO ) {
			statePlus = TOUCH_DOWN;
		} else {
			if( statePlus != INT_MAX ) statePlus++;
		}
	} else {	// current < 0.0
		if( stateMinus <= TOUCH_NO ) {
			stateMinus = TOUCH_DOWN;
		} else {
			if( stateMinus != INT_MAX ) stateMinus++;
		}

		if( statePlus >= TOUCH_DOWN ) {
			statePlus = 0;
		} else {
			if( statePlus != INT_MIN ) statePlus--;
		}
	}
}

void CInputContext::UpdateButtonState( int& state, bool current )
{
	if( current == false ) {
		if( state >= TOUCH_DOWN ) {
			state = 0;
		} else {
			if( state != INT_MIN ) state--;
		}
	} else {	// current == true
		if( state <= TOUCH_NO ) {
			state = TOUCH_DOWN;
		} else {
			if( state != INT_MAX ) state++;
		}
	}
}

void CInputContext::Input( GamePadState& state )
{
	UpdateAxisState( game_pad_ctx_.analogLeftLeft, game_pad_ctx_.analogLeftRight, state.leftX );
	UpdateAxisState( game_pad_ctx_.analogLeftDown, game_pad_ctx_.analogLeftUp, state.leftY );
	UpdateAxisState( game_pad_ctx_.analogRightLeft, game_pad_ctx_.analogRightRight, state.rightX );
	UpdateAxisState( game_pad_ctx_.analogRightDown, game_pad_ctx_.analogRightUp, state.rightY );

	UpdateButtonState( game_pad_ctx_.buttonLeftTrigger, state.leftTrigger != 0.0 );
	UpdateButtonState( game_pad_ctx_.buttonRightTrigger, state.rightTrigger != 0.0 );

	UpdateButtonState( game_pad_ctx_.digitalUp, (state.keyState & IDBTN_DPAD_UP) != 0 );
	UpdateButtonState( game_pad_ctx_.digitalDown, (state.keyState & IDBTN_DPAD_DOWN) != 0 );
	UpdateButtonState( game_pad_ctx_.digitalLeft, (state.keyState & IDBTN_DPAD_LEFT) != 0 );
	UpdateButtonState( game_pad_ctx_.digitalRight, (state.keyState & IDBTN_DPAD_RIGHT) != 0 );

	UpdateButtonState( game_pad_ctx_.buttonStart, (state.keyState & IDBTN_START) != 0 );
	UpdateButtonState( game_pad_ctx_.buttonBack, (state.keyState & IDBTN_BACK) != 0 );
	UpdateButtonState( game_pad_ctx_.buttonLeftThumb, (state.keyState & IDBTN_LEFT_THUMB) != 0 );
	UpdateButtonState( game_pad_ctx_.buttonRightThumb, (state.keyState & IDBTN_RIGHT_THUMB) != 0 );
	UpdateButtonState( game_pad_ctx_.buttonLeftShoulder, (state.keyState & IDBTN_LEFT_SHOULDER) != 0 );
	UpdateButtonState( game_pad_ctx_.buttonRightShoulder, (state.keyState & IDBTN_RIGHT_SHOULDER) != 0 );

	UpdateButtonState( game_pad_ctx_.buttonA, (state.keyState & IDBTN_A) != 0 );
	UpdateButtonState( game_pad_ctx_.buttonB, (state.keyState & IDBTN_B) != 0 );
	UpdateButtonState( game_pad_ctx_.buttonX, (state.keyState & IDBTN_X) != 0 );
	UpdateButtonState( game_pad_ctx_.buttonY, (state.keyState & IDBTN_Y) != 0 );

	game_pad_ = state;
}


};	// end of namespace gamepad

