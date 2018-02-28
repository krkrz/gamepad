/****************************************************************************/
/*! @file
@brief 

-----------------------------------------------------------------------------
	Copyright (C) 2008 Takenori Imoto. All rights reserved.
	( http://www.kaede-software.com/ )
-----------------------------------------------------------------------------
@author		T.Imoto
@date		2008/02/01
@note

	SQLite �� DB ���g���悤�ɂ����ق�����������

	vender_tabel( pid integer primary, name text )
	product_table( pid integer primary, vender_id integer, product_id integer, map_id integer, name text )
	map_table( pid integer primary, mapping... )


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

#include "GamePadDirectInputDevice.h"
#include "GamePadDirectInputMappingDB.h"

namespace gamepad {

struct ProductMappingTable
{
	unsigned short	product;
	const DirectInputObjectMappingTable*	table;
	// ���݂��鎲��{�^���̐����ێ����Ă����������悳��
};
struct VenderMappingTable
{
	unsigned short	vender;
	const ProductMappingTable*	table;
};
// Logtech Rumble Pad 2
static const DirectInputObjectMappingTable LogitechRumblePad2 =
{
	{	// buttons
		{ DIObj_POV_0, 1 },	// UP
		{ DIObj_POV_0, 1 },	// DOWN
		{ DIObj_POV_0, 1 },	// LEFT
		{ DIObj_POV_0, 1 },	// RIGHT

		{ DIObj_Button10, 1 },	// START
		{ DIObj_Button9, 1 },	// BACK
		{ DIObj_Button11, 1 },	// ���T���{�^��
		{ DIObj_Button12, 1 },	// �E�T���{�^��
		{ DIObj_Button5, 1 },	// �����{�^��
		{ DIObj_Button6, 1 },	// �E���{�^��
#if 0	// 
		{ DIObj_Button2, 1 },	// A�{�^��
		{ DIObj_Button3, 1 },	// B�{�^��
		{ DIObj_Button1, 1 },	// X�{�^��
		{ DIObj_Button4, 1 },	// Y�{�^��
#else
		{ DIObj_Button1, 1 },	// A�{�^��
		{ DIObj_Button2, 1 },	// B�{�^��
		{ DIObj_Button3, 1 },	// X�{�^��
		{ DIObj_Button4, 1 },	// Y�{�^��
#endif
	},
	{	// triggers
		{ DIObj_Button7, 1 },	// ���g���K�[
		{ DIObj_Button8, 1 },	// �E�g���K�[
	},
	{	// axis
		{ DIObj_AxisX, 1 },		// ���X�e�B�b�NX��
		{ DIObj_AxisY, -1 },	// ���X�e�B�b�NY��
		{ DIObj_AxisZ, 1 },		// �E�X�e�B�b�NX��
		{ DIObj_AxisRotZ, -1 },	// �E�X�e�B�b�NY��
	},
	true,
};
static const ProductMappingTable LogitechProduct[] = 
{
	{ 0xC218, &LogitechRumblePad2 },
	{ 0, NULL },	// �I�[
};
static const VenderMappingTable VenderTable[] = 
{
	{ 0x046D, LogitechProduct },
	{ 0, NULL },	// �I�[
};

int CopyKeyMap( DirectInputObjectMappingTable& map, unsigned short vender, unsigned short product )
{
	for( int v = 0; ; v++ ) {
		if( VenderTable[v].table == NULL ) break;

		if( VenderTable[v].vender == vender ) {
			// found vender
			const ProductMappingTable*	table = VenderTable[v].table;
			for( int p = 0; ; p++ ) {
				if( table[p].table == NULL ) break;

				if( table[p].product == product ) {
					// found product
					memcpy( &map, table[p].table, sizeof(DirectInputObjectMappingTable) );
					return KMR_SuccessProductMapping;
				}
			}
			// ���t����Ȃ������B�����x���_�[�̍ŏ��̃e�[�u�����R�s�[����
			if( table[0].table != NULL ) {
				memcpy( &map, table[0].table, sizeof(DirectInputObjectMappingTable) );
				return KMR_SuccessVenderMapping;
			}
		}
	}
	return KMR_NotFound;
}


}; // namespace gamepad

