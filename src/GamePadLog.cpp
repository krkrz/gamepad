/****************************************************************************/
/*! @file
@brief �Q�[���p�b�h���O

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

#include "GamePadLog.h"
#include <string>
#include "GamePadDLLLoader.h"
//#include <dshow.h>
#include <windows.h>

#ifdef __KRKR__
#include "tp_stub.h"
#endif // __KRKR__

#ifndef MAX_ERROR_TEXT_LEN
#define MAX_ERROR_TEXT_LEN 160
#endif // MAX_ERROR_TEXT_LEN

namespace gamepad {

struct quartz
{
	static CDLLLoader			dll_;
	static bool LoadFunctions();
	static bool CheckLoading();

	typedef BOOL (WINAPI* FuncGetErrorText)(HRESULT, WCHAR *, DWORD);

public:
	static bool Initialize();

	static FuncGetErrorText		GetErrorText;
};
CDLLLoader quartz::dll_;
quartz::FuncGetErrorText quartz::GetErrorText = NULL;


bool quartz::Initialize()
{
	if( !GetErrorText ) {
		return LoadFunctions();
	}
	return true;	// �ǂݍ��ݍς�
}

bool quartz::CheckLoading()
{
	if( dll_.IsLoaded() == false ) {
		dll_.Load( "quartz.dll" );
	}
	return dll_.IsLoaded();
}
bool quartz::LoadFunctions()
{
	if( !CheckLoading() ) {
		return false;
	}

	GetErrorText = (FuncGetErrorText)dll_.GetProcAddress( "AMGetErrorTextW" );
	if( !GetErrorText ) { return false; }

	return true;
}

void Log( const wchar_t* mes )
{
	std::wstring head(L"gamepad : ");
	head += std::wstring( mes );
#ifdef __KRKR__
	TVPAddLog( head.c_str() );
#else	// __KRKR__
	OutputDebugStringW( head.c_str() );
#endif	// __KRKR__
}

void Log( const wchar_t* mes, HRESULT hr )
{
	std::wstring head(L"gamepad : ");

	if( quartz::Initialize() ) {
		wchar_t	errorMes[MAX_ERROR_TEXT_LEN] = {0};
		quartz::GetErrorText( hr, errorMes, MAX_ERROR_TEXT_LEN );
		head += std::wstring( (wchar_t*)errorMes );
		head += std::wstring( L" : " );
	}
	head += std::wstring( mes );
#ifdef __KRKR__
	TVPAddLog( head.c_str() );
#else	// __KRKR__
	OutputDebugStringW( head.c_str() );
#endif	// __KRKR__
}

}; // namespace gamepad


