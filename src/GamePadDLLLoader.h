/****************************************************************************/
/*! @file
@brief DLL Loader

-----------------------------------------------------------------------------
	Copyright (C) 2005-2008 Takenori Imoto (�䌳 ����). All rights reserved.
	( http://www.kaede-software.com/ )
-----------------------------------------------------------------------------
@author		T.Imoto
@date		2005/12/22
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

#ifndef __GAMEPAD_DLL_LOADER_H__
#define __GAMEPAD_DLL_LOADER_H__

#include <windows.h>

namespace gamepad {
//----------------------------------------------------------------------------
//! @brief DLL��ǂݍ��ނ��߂̃N���X
/*! @note ��:
	if( D3DDll_.IsLoaded() == false )
		D3DDll_.Load("d3d9.dll");

	if( D3DDll_.IsLoaded() == false )
		return D3DDll_.GetLastError();

	typedef IDirect3D9 *(WINAPI *FuncDirect3DCreate9)( UINT SDKVersion );
	FuncDirect3DCreate9 pDirect3DCreate9 = (FuncDirect3DCreate9)D3DDll_.GetProcAddress("Direct3DCreate9");
*/
//----------------------------------------------------------------------------
class CDLLLoader
{
	HMODULE		module_handle_;

public:
	CDLLLoader() : module_handle_(NULL) {}
	CDLLLoader(LPCTSTR lpFileName)
	{
		module_handle_ = ::LoadLibrary( lpFileName );
	}
	~CDLLLoader() { Free(); }
	inline bool IsLoaded() const { return( module_handle_ != NULL ); }
	bool Load( LPCTSTR lpFileName )
	{
		Free();
		module_handle_ = ::LoadLibrary( lpFileName );
		return( module_handle_ != NULL );
	}
	void Free()
	{
		if( module_handle_ )
			::FreeLibrary( module_handle_ );
		module_handle_ = NULL;
	}
	inline HRESULT GetLastError() const { return HRESULT_FROM_WIN32( ::GetLastError() ); }

	//! @param lpProcName : �֐���
	inline FARPROC GetProcAddress( LPCSTR lpProcName ) const
	{
		if( IsLoaded() ) {
			return ::GetProcAddress( module_handle_, lpProcName );
		} else {
			return NULL;
		}
	}
};

}; // namespace gamepad

#endif // __GAMEPAD_DLL_LOADER_H__

