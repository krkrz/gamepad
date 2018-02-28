/****************************************************************************/
/*! @file
@brief �W���C�X�e�B�b�N�|�[�g

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

#ifndef __GAME_PAD_INPUT_DEVICE_PORT_H__
#define __GAME_PAD_INPUT_DEVICE_PORT_H__

#include <windows.h>
#include <dinput.h>
#include <vector>
#include <list>
#include <string>
#include <atlbase.h>
#include "GamePadDLLLoader.h"
#include "GamePadInputDevice.h"

namespace gamepad {

//! ���z�I�ȃN���X
//! �|�[�g�ɃR���g���[�����Ȃ����Ă���悤�Ɉ���
class CInputDevicePort
{
//	std::vector<IInputDevice*>	controllers_;		//!< ���g�p�R���g���[���̃��X�g
	std::list<DWORD>			xinput_vid_pids_;	//!< XInput �f�o�C�X�̃x���_�[ID�ƃv���_�N�gID�̃��X�g

	//! �R���g���[���p�l�� - �Q�[���R���g���[���ŗD��f�o�C�X�ɐݒ肳��Ă�����̂�GUID
	GUID				preferred_device_guid_;
	bool				is_valid_preferred_device_guid_;	//!< �D��f�o�C�XGUID���L�����ǂ���

	std::list<IInputDevice*>	controllers_;

//	static const size_t NUM_OF_PORTS = 4;

	static CDLLLoader			dinput_dll_;	//!< Direct Input DLL
	CComPtr<IDirectInput8>		direct_input_;	//!< Direct Input

	//! ���̃��X�g�Ɋi�[����
	void PutTempControllers( IInputDevice* device ) {
		controllers_.push_back( device );
	}
	//! ���̃��X�g�̐擪�Ɋi�[����
	void PutTempControllersFront( IInputDevice* device ) {
		controllers_.push_front( device );
	}

	//! XInput�̏�����
	void InitializeXInput();
	//! DirectInput �̏�����
	void InitializeDirectInput();

	//! DirectInput �f�o�C�X����������
	void FindDirectInputDevice();
	//! DirectInput �f�o�C�X�񋓎��̃R�[���o�b�N�֐�
	static BOOL CALLBACK EnumDevicesCallback( const DIDEVICEINSTANCE* pInst, VOID* pContext );
	//! DirectInput �f�o�C�X�񋓎��̃R�[���o�b�N�֐�
	bool EnumDevicesCallback( const DIDEVICEINSTANCE* pInst );

	//! XInput �f�o�C�X����������
	bool FindXInputDevice();
	//! XInput �f�o�C�X���ǂ����`�F�b�N����
	bool IsXInputDevice( const GUID& GuidProduct ) const;

	//! �D��f�o�C�X�� GUID ���擾����
	void GetPreferredDevice();

	//! �S�f�o�C�X������������
	void InitializeAllDevice( HWND hWnd );

	//! ��̃|�[�g����������
	// @return < 0 : �󂫂Ȃ�, other : �󂫃|�[�g
//	int GetEmptyPort();

	//! �w��|�[�g�Ƀf�o�C�X���Z�b�g����
//	bool InsertDeviceToPort( int num, IInputDevice* device );

	void ClearAllDevice();

public:
	CInputDevicePort();
	~CInputDevicePort();

	// �R���g���[��������������
	void InitializeControllers( HWND hWnd );

	// �L���ȃR���g���[�������擾����
	size_t GetNumberOfEnableControllers() const { return controllers_.size(); }

	IInputDevice* GetController( size_t idx );

	//! Direct Input Device �����X�g���A�Ď擾����K�v�����鎞�Ɏg�p����
	bool CreateDirectInputDevice( GUID& instGuid, IDirectInputDevice8** device );
};

}; // namespace gamepad

#endif // __GAME_PAD_INPUT_DEVICE_PORT_H__


