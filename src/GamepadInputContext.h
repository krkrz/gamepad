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


#ifndef __GAME_PAD_INPUT_CONTEXT_H__
#define __GAME_PAD_INPUT_CONTEXT_H__

namespace gamepad {

struct GamePadState
{
	double	leftX;
	double	leftY;
	double	rightX;
	double	rightY;
	double	leftTrigger;
	double	rightTrigger;
	unsigned long	keyState;

	void clear() {
		leftX = leftY = rightX = rightY = leftTrigger = rightTrigger = 0.0;
		keyState = 0;
	}
};

struct GamePadContext
{
	int analogLeftUp;
	int analogLeftDown;
	int analogLeftLeft;
	int analogLeftRight;

	int analogRightUp;
	int analogRightDown;
	int analogRightLeft;
	int analogRightRight;

	int digitalUp;
	int digitalDown;
	int digitalLeft;
	int digitalRight;

	int buttonStart;
	int buttonBack;
	int buttonLeftThumb;
	int buttonRightThumb;
	int buttonLeftShoulder;
	int buttonLeftTrigger;
	int buttonRightShoulder;
	int buttonRightTrigger;
	int buttonA;
	int buttonB;
	int buttonX;
	int buttonY;

	void clear() {
		analogLeftUp = analogLeftDown = analogLeftLeft = analogLeftRight = 0;
		analogRightUp = analogRightDown = analogRightLeft = analogRightRight = 0;
		digitalUp = digitalDown = digitalLeft = digitalRight = 0;
		buttonStart = buttonBack = buttonLeftThumb = buttonRightThumb = buttonLeftShoulder
		 = buttonLeftTrigger = buttonRightShoulder = buttonRightTrigger
		 = buttonA = buttonB = buttonX = buttonY = 0;
	}
};

class CInputContext
{
public:
	static const int TOUCH_NO = 0;				//!< ������Ă��Ȃ�
	static const int TOUCH_DOWN = 1;			//!< �����ꂽ�u��
	static const int TOUCH_LIFTOFF = 0;			//!< �����ꂽ
	// �����ꑱ���Ă��邩�ǂ��� TOUCH_DOWN ���傫�����ǂ����Ŕ���
	// 0 �̎� �����ꂽ�u�ԁA1�̎������ꂽ�u�ԁA������Ă��鎞�Ԃ������قǒl�͑傫���Ȃ�
	// �܂��A������Ă��Ȃ����Ԃ������قǁA���̒l�Œl���������Ȃ�
	// val >= TOUCH_DOWN : ������Ă��� (�l���傫���قǒ���������Ă���
	// val <= TOUCH_NO : ������Ă��Ȃ� (�l���������قǒ���������Ă���
	// val == TOUCH_DOWN �����ꂽ�u��
	// val == TOUCH_LIFTOFF : �����ꂽ�u��

protected:
	GamePadState	game_pad_;
	GamePadContext	game_pad_ctx_;

	void UpdateAxisState( int& stateMinus, int& statePlus, double current );
	void UpdateButtonState( int& state, bool current );

public:
	CInputContext();
	~CInputContext() {}

	inline void ClearAll() {
		game_pad_.clear();
		game_pad_ctx_.clear();
	}

	// �p�b�h�̏�Ԃ��X�V
	void Input( GamePadState& state );

	inline int GetAnalogLeftUpCount() const { return game_pad_ctx_.analogLeftUp; }
	inline int GetAnalogLeftDownCount() const { return game_pad_ctx_.analogLeftDown; }
	inline int GetAnalogLeftLeftCount() const { return game_pad_ctx_.analogLeftLeft; }
	inline int GetAnalogLeftRightCount() const { return game_pad_ctx_.analogLeftRight; }

	inline int GetAnadloRightUpCount() const { return game_pad_ctx_.analogRightUp; }
	inline int GetAnadloRightDownCount() const { return game_pad_ctx_.analogRightDown; }
	inline int GetAnadloRightLeftCount() const { return game_pad_ctx_.analogRightLeft; }
	inline int GetAnadloRightRightCount() const { return game_pad_ctx_.analogRightRight; }

	inline int GetDegitalUpCount() const { return game_pad_ctx_.digitalUp; }
	inline int GetDegitalDownCount() const { return game_pad_ctx_.digitalDown; }
	inline int GetDegitalLeftCount() const { return game_pad_ctx_.digitalLeft; }
	inline int GetDegitalRightCount() const { return game_pad_ctx_.digitalRight; }

	inline int GetButtonStartCount() const { return game_pad_ctx_.buttonStart; }
	inline int GetButtonBackCount() const { return game_pad_ctx_.buttonBack; }
	inline int GetButtonLeftThumbCount() const { return game_pad_ctx_.buttonLeftThumb; }
	inline int GetButtonRightThumbCount() const { return game_pad_ctx_.buttonRightThumb; }
	inline int GetButtonLeftShoulderCount() const { return game_pad_ctx_.buttonLeftShoulder; }
	inline int GetButtonLeftTriggerCount() const { return game_pad_ctx_.buttonLeftTrigger; }
	inline int GetButtonRightShoulderCount() const { return game_pad_ctx_.buttonRightShoulder; }
	inline int GetButtonRightTriggerCount() const { return game_pad_ctx_.buttonRightTrigger; }
	inline int GetButtonACount() const { return game_pad_ctx_.buttonA; }
	inline int GetButtonBCount() const { return game_pad_ctx_.buttonB; }
	inline int GetButtonXCount() const { return game_pad_ctx_.buttonX; }
	inline int GetButtonYCount() const { return game_pad_ctx_.buttonY; }
};

};	// end of namespace gamepad


#endif // __GAME_PAD_INPUT_CONTEXT_H__

