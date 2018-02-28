//---------------------------------------------------------------------------
#include <windows.h>
#include "tp_stub.h"
#include "GamePadInputDevicePort.h"
#include "GamePadInputDevice.h"
#include "GamepadInputContext.h"
#include <assert.h>
//---------------------------------------------------------------------------
//! �l�C�e�B�u�C���X�^���X
class NI_GamepadPort : public tTJSNativeInstance
{
public:
	gamepad::CInputDevicePort	port_;

	NI_GamepadPort() {}
	tjs_error TJS_INTF_METHOD Construct(tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *tjs_obj) {
		return S_OK;
	}
	void TJS_INTF_METHOD Invalidate() {}
};
//---------------------------------------------------------------------------
/*! ����̓I�u�W�F�N�g���쐬���ĕԂ������̊֐��ł��B
	��q�� TJSCreateNativeClassForPlugin �̈����Ƃ��ēn���܂��B */
static iTJSNativeInstance * TJS_INTF_METHOD Create_NI_GamepadPort()
{
	return new NI_GamepadPort();
}
//---------------------------------------------------------------------------
/*! TJS2 �̃l�C�e�B�u�N���X�͈�ӂ� ID �ŋ�ʂ���Ă���K�v������܂��B
	����͌�q�� TJS_BEGIN_NATIVE_MEMBERS �}�N���Ŏ����I�Ɏ擾����܂����A
	���� ID ���i�[����ϐ����ƁA���̕ϐ��������Ő錾���܂��B
	�����l�ɂ͖����� ID ��\�� -1 ���w�肵�Ă��������B */
#define TJS_NATIVE_CLASSID_NAME ClassID_GamepadPort
static tjs_int32 TJS_NATIVE_CLASSID_NAME = -1;
//---------------------------------------------------------------------------
//! TJS2 �p�́u�N���X�v���쐬���ĕԂ��֐��ł��B
static iTJSDispatch2 * Create_NC_GamepadPort()
{
	/// �N���X�I�u�W�F�N�g�̍쐬
	tTJSNativeClassForPlugin * classobj = TJSCreateNativeClassForPlugin(TJS_W("GamepadPort"), Create_NI_GamepadPort);

	/// �����o��`
	TJS_BEGIN_NATIVE_MEMBERS(/*TJS class name*/GamepadPort)

		TJS_DECL_EMPTY_FINALIZE_METHOD

//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_CONSTRUCTOR_DECL( /*var.name*/_this, /*var.type*/NI_GamepadPort, /*TJS class name*/GamepadPort)
		{
			// NI_GamepadPort::Construct �ɂ����e���L�q�ł���̂ł����ł͉������Ȃ�
			return TJS_S_OK;

		} TJS_END_NATIVE_CONSTRUCTOR_DECL(/*TJS class name*/GamepadPort)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_METHOD_DECL(/*func. name*/initialize)
		{
			TJS_GET_NATIVE_INSTANCE(/*var. name*/_this, /*var. type*/NI_GamepadPort);

			if( numparams < 1 ) return TJS_E_BADPARAMCOUNT;
			_this->port_.InitializeControllers( (HWND)(tjs_int)*param[0] );
			return TJS_S_OK;

		} TJS_END_NATIVE_METHOD_DECL(/*func. name*/initialize)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_METHOD_DECL(/*func. name*/getController)
		{
			TJS_GET_NATIVE_INSTANCE(/*var. name*/_this, /*var. type*/NI_GamepadPort);

			if( numparams < 1 ) return TJS_E_BADPARAMCOUNT;
			if( result ) {
				gamepad::IInputDevice*	device = _this->port_.GetController( (tjs_int)*param[0] );
				if( device ) {
					tTJSVariant varScripts;
					TVPExecuteExpression(TJS_W("Gamepad"), &varScripts);
					iTJSDispatch2 *dispatch = varScripts.AsObjectNoAddRef();

					tTJSVariant val;
					val = tTJSVariant( (tjs_int)device );
					tTJSVariant *varsp[1] = {&val};
					iTJSDispatch2 *instance = NULL;
//					if( SUCCEEDED( dispatch->CreateNew( 0, NULL, NULL, &instance, 0, NULL, _this))) {
					if( SUCCEEDED( dispatch->CreateNew( 0, NULL, NULL, &instance, 1, varsp, objthis))) {
						if( instance ) {
							instance->AddRef();
//							tTJSVariant val;
//							val = tTJSVariant( (tjs_int)device );
//							instance->PropSet( TJS_MEMBERENSURE, TJS_W("instance"), NULL, &val, _this );
							*result = instance;
							return TJS_S_OK;
						}
					}
				}
			}
			return TJS_E_INVALIDPARAM;

		} TJS_END_NATIVE_METHOD_DECL(/*func. name*/getController)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(count) // count �ǂݏo����p�v���p�e�B
		{
			TJS_BEGIN_NATIVE_PROP_GETTER
			{
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_GamepadPort);

				if( result ) {
					*result = tjs_int( _this->port_.GetNumberOfEnableControllers() );
				}
				return TJS_S_OK;

			} TJS_END_NATIVE_PROP_GETTER

			TJS_DENY_NATIVE_PROP_SETTER

		} TJS_END_NATIVE_PROP_DECL(count)
//-------------------------------------------------------------------

	TJS_END_NATIVE_MEMBERS

	return classobj;
}
//---------------------------------------------------------------------------
//	TJS_NATIVE_CLASSID_NAME �͈ꉞ undef ���Ă������ق����悢�ł��傤
#undef TJS_NATIVE_CLASSID_NAME
//---------------------------------------------------------------------------






//---------------------------------------------------------------------------
//! �l�C�e�B�u�C���X�^���X
class NI_Gamepad : public tTJSNativeInstance
{
public:
	gamepad::IInputDevice	*device_;
	gamepad::CInputContext	context_;

	NI_Gamepad() {}
	tjs_error TJS_INTF_METHOD Construct(tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *tjs_obj) {
		return S_OK;
	}
	void TJS_INTF_METHOD Invalidate() { device_ = NULL; }

	void Update();
};
void NI_Gamepad::Update()
{
	if( device_ ) {
		device_->Update();
		gamepad::GamePadState	state;
		state.leftX = device_->GetLeftThumbStickX();
		state.leftY = device_->GetLeftThumbStickY();
		state.rightX = device_->GetRightThumbStickX();
		state.rightY = device_->GetRightThumbStickY();
		state.leftTrigger = device_->GetLeftThumbStickX();
		state.rightTrigger = device_->GetLeftThumbStickX();
		state.keyState = device_->GetKeyState();
		context_.Input( state );
	}
}
//---------------------------------------------------------------------------
/*! ����̓I�u�W�F�N�g���쐬���ĕԂ������̊֐��ł��B
	��q�� TJSCreateNativeClassForPlugin �̈����Ƃ��ēn���܂��B */
static iTJSNativeInstance * TJS_INTF_METHOD Create_NI_Gamepad()
{
	return new NI_Gamepad();
}
//---------------------------------------------------------------------------
/*! TJS2 �̃l�C�e�B�u�N���X�͈�ӂ� ID �ŋ�ʂ���Ă���K�v������܂��B
	����͌�q�� TJS_BEGIN_NATIVE_MEMBERS �}�N���Ŏ����I�Ɏ擾����܂����A
	���� ID ���i�[����ϐ����ƁA���̕ϐ��������Ő錾���܂��B
	�����l�ɂ͖����� ID ��\�� -1 ���w�肵�Ă��������B */
#define TJS_NATIVE_CLASSID_NAME ClassID_Gamepad
static tjs_int32 TJS_NATIVE_CLASSID_NAME = -1;
//---------------------------------------------------------------------------
//! TJS2 �p�́u�N���X�v���쐬���ĕԂ��֐��ł��B
static iTJSDispatch2 * Create_NC_Gamepad()
{
	/// �N���X�I�u�W�F�N�g�̍쐬
	tTJSNativeClassForPlugin * classobj = TJSCreateNativeClassForPlugin(TJS_W("Gamepad"), Create_NI_Gamepad);

	/// �����o��`
	TJS_BEGIN_NATIVE_MEMBERS(/*TJS class name*/Gamepad)

		TJS_DECL_EMPTY_FINALIZE_METHOD

//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_CONSTRUCTOR_DECL( /*var.name*/_this, /*var.type*/NI_Gamepad, /*TJS class name*/Gamepad) {
			TJS_GET_NATIVE_INSTANCE(/*var. name*/_this, /*var. type*/NI_Gamepad);

			if( numparams < 1 ) return TJS_E_BADPARAMCOUNT;

			_this->device_ = (gamepad::IInputDevice*)(tjs_int)*param[0];
			return TJS_S_OK;

		} TJS_END_NATIVE_CONSTRUCTOR_DECL(/*TJS class name*/Gamepad)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_METHOD_DECL(/*func. name*/update) {
			TJS_GET_NATIVE_INSTANCE(/*var. name*/_this, /*var. type*/NI_Gamepad);
			_this->Update();
			return TJS_S_OK;

		} TJS_END_NATIVE_METHOD_DECL(/*func. name*/update)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(name) {
			TJS_BEGIN_NATIVE_PROP_GETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( result && _this->device_ ) {
					std::wstring name;
					_this->device_->GetDeviceName( name );
					*result = tTJSVariant( (const tjs_char *)name.c_str() );
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_GETTER
			TJS_DENY_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(name)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(type) {
			TJS_BEGIN_NATIVE_PROP_GETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( result && _this->device_ ) {
					*result = (tjs_int)_this->device_->GetTypeOfDevice();
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_GETTER
			TJS_DENY_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(type)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(leftTrigger) {
			TJS_BEGIN_NATIVE_PROP_GETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( result && _this->device_ ) {
					*result = (tjs_real)_this->device_->GetLeftTrigger();
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_GETTER
			TJS_DENY_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(leftTrigger)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(rightTrigger) {
			TJS_BEGIN_NATIVE_PROP_GETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( result && _this->device_ ) {
					*result = (tjs_real)_this->device_->GetRightTrigger();
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_GETTER
			TJS_DENY_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(rightTrigger)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(leftThumbStickX) {
			TJS_BEGIN_NATIVE_PROP_GETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( result && _this->device_ ) {
					*result = (tjs_real)_this->device_->GetLeftThumbStickX();
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_GETTER
			TJS_DENY_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(leftThumbStickX)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(leftThumbStickY) {
			TJS_BEGIN_NATIVE_PROP_GETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( result && _this->device_ ) {
					*result = (tjs_real)_this->device_->GetLeftThumbStickY();
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_GETTER
			TJS_DENY_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(leftThumbStickY)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(rightThumbStickX) {
			TJS_BEGIN_NATIVE_PROP_GETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( result && _this->device_ ) {
					*result = (tjs_real)_this->device_->GetRightThumbStickX();
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_GETTER
			TJS_DENY_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(rightThumbStickX)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(rightThumbStickY) {
			TJS_BEGIN_NATIVE_PROP_GETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( result && _this->device_ ) {
					*result = (tjs_real)_this->device_->GetRightThumbStickY();
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_GETTER
			TJS_DENY_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(rightThumbStickY)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(leftVibration) {
			TJS_DENY_NATIVE_PROP_GETTER
			TJS_BEGIN_NATIVE_PROP_SETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( _this->device_ ) {
					_this->device_->SetLeftVibration( (tjs_real)*param );
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(leftVibration)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(rightVibration) {
			TJS_DENY_NATIVE_PROP_GETTER
			TJS_BEGIN_NATIVE_PROP_SETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( _this->device_ ) {
					_this->device_->SetRightVibration( (tjs_real)*param );
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(rightVibration)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(keyState) {
			TJS_BEGIN_NATIVE_PROP_GETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( result && _this->device_ ) {
					*result = (tjs_int)_this->device_->GetKeyState();
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_GETTER
			TJS_DENY_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(keyState)
//-------------------------------------------------------------------
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(analogLeftUpCount) {
			TJS_BEGIN_NATIVE_PROP_GETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( result ) {
					*result = (tjs_int)_this->context_.GetAnalogLeftUpCount();
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_GETTER
			TJS_DENY_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(analogLeftUpCount)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(analogLeftDownCount) {
			TJS_BEGIN_NATIVE_PROP_GETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( result ) {
					*result = (tjs_int)_this->context_.GetAnalogLeftDownCount();
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_GETTER
			TJS_DENY_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(analogLeftDownCount)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(analogLeftLeftCount) {
			TJS_BEGIN_NATIVE_PROP_GETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( result ) {
					*result = (tjs_int)_this->context_.GetAnalogLeftLeftCount();
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_GETTER
			TJS_DENY_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(analogLeftLeftCount)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(analogLeftRightCount) {
			TJS_BEGIN_NATIVE_PROP_GETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( result ) {
					*result = (tjs_int)_this->context_.GetAnalogLeftRightCount();
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_GETTER
			TJS_DENY_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(analogLeftRightCount)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(analogRightUpCount) {
			TJS_BEGIN_NATIVE_PROP_GETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( result ) {
					*result = (tjs_int)_this->context_.GetAnadloRightUpCount();
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_GETTER
			TJS_DENY_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(analogRightUpCount)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(analogRightDownCount) {
			TJS_BEGIN_NATIVE_PROP_GETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( result ) {
					*result = (tjs_int)_this->context_.GetAnadloRightDownCount();
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_GETTER
			TJS_DENY_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(analogRightDownCount)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(analogRightLeftCount) {
			TJS_BEGIN_NATIVE_PROP_GETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( result ) {
					*result = (tjs_int)_this->context_.GetAnadloRightLeftCount();
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_GETTER
			TJS_DENY_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(analogRightLeftCount)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(analogRightRightCount) {
			TJS_BEGIN_NATIVE_PROP_GETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( result ) {
					*result = (tjs_int)_this->context_.GetAnadloRightRightCount();
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_GETTER
			TJS_DENY_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(analogRightRightCount)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(degitalUpCount) {
			TJS_BEGIN_NATIVE_PROP_GETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( result ) {
					*result = (tjs_int)_this->context_.GetDegitalUpCount();
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_GETTER
			TJS_DENY_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(degitalUpCount)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(degitalDownCount) {
			TJS_BEGIN_NATIVE_PROP_GETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( result ) {
					*result = (tjs_int)_this->context_.GetDegitalDownCount();
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_GETTER
			TJS_DENY_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(degitalDownCount)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(degitalLeftCount) {
			TJS_BEGIN_NATIVE_PROP_GETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( result ) {
					*result = (tjs_int)_this->context_.GetDegitalLeftCount();
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_GETTER
			TJS_DENY_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(degitalLeftCount)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(degitalRightCount) {
			TJS_BEGIN_NATIVE_PROP_GETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( result ) {
					*result = (tjs_int)_this->context_.GetDegitalRightCount();
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_GETTER
			TJS_DENY_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(degitalRightCount)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(buttonStartCount) {
			TJS_BEGIN_NATIVE_PROP_GETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( result ) {
					*result = (tjs_int)_this->context_.GetButtonStartCount();
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_GETTER
			TJS_DENY_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(buttonStartCount)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(buttonBackCount) {
			TJS_BEGIN_NATIVE_PROP_GETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( result ) {
					*result = (tjs_int)_this->context_.GetButtonBackCount();
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_GETTER
			TJS_DENY_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(buttonBackCount)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(buttonLeftThumbCount) {
			TJS_BEGIN_NATIVE_PROP_GETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( result ) {
					*result = (tjs_int)_this->context_.GetButtonLeftThumbCount();
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_GETTER
			TJS_DENY_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(buttonLeftThumbCount)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(buttonRightThumbCount) {
			TJS_BEGIN_NATIVE_PROP_GETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( result ) {
					*result = (tjs_int)_this->context_.GetButtonRightThumbCount();
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_GETTER
			TJS_DENY_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(buttonRightThumbCount)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(buttonLeftShoulderCount) {
			TJS_BEGIN_NATIVE_PROP_GETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( result ) {
					*result = (tjs_int)_this->context_.GetButtonLeftShoulderCount();
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_GETTER
			TJS_DENY_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(buttonLeftShoulderCount)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(buttonLeftTriggerCount) {
			TJS_BEGIN_NATIVE_PROP_GETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( result ) {
					*result = (tjs_int)_this->context_.GetButtonLeftTriggerCount();
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_GETTER
			TJS_DENY_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(buttonLeftTriggerCount)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(buttonRightShoulderCount) {
			TJS_BEGIN_NATIVE_PROP_GETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( result ) {
					*result = (tjs_int)_this->context_.GetButtonRightShoulderCount();
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_GETTER
			TJS_DENY_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(buttonRightShoulderCount)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(buttonRightTriggerCount) {
			TJS_BEGIN_NATIVE_PROP_GETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( result ) {
					*result = (tjs_int)_this->context_.GetButtonRightTriggerCount();
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_GETTER
			TJS_DENY_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(buttonRightTriggerCount)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(buttonACount) {
			TJS_BEGIN_NATIVE_PROP_GETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( result ) {
					*result = (tjs_int)_this->context_.GetButtonACount();
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_GETTER
			TJS_DENY_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(buttonACount)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(buttonBCount) {
			TJS_BEGIN_NATIVE_PROP_GETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( result ) {
					*result = (tjs_int)_this->context_.GetButtonBCount();
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_GETTER
			TJS_DENY_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(buttonBCount)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(buttonXCount) {
			TJS_BEGIN_NATIVE_PROP_GETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( result ) {
					*result = (tjs_int)_this->context_.GetButtonXCount();
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_GETTER
			TJS_DENY_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(buttonXCount)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(buttonYCount) {
			TJS_BEGIN_NATIVE_PROP_GETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( result ) {
					*result = (tjs_int)_this->context_.GetButtonYCount();
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_GETTER
			TJS_DENY_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(buttonYCount)
//-------------------------------------------------------------------
	TJS_END_NATIVE_MEMBERS

	return classobj;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//	TJS_NATIVE_CLASSID_NAME �͈ꉞ undef ���Ă������ق����悢�ł��傤
#undef TJS_NATIVE_CLASSID_NAME
//---------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
	return 1;
}
//---------------------------------------------------------------------------
static tjs_int GlobalRefCountAtInit = 0;
extern "C" __declspec(dllexport) HRESULT _stdcall V2Link(iTVPFunctionExporter *exporter)
{
	// �X�^�u�̏�����(�K���L�q����)
	TVPInitImportStub(exporter);

	tTJSVariant val;

	// TJS �̃O���[�o���I�u�W�F�N�g���擾����
	iTJSDispatch2 * global = TVPGetScriptDispatch();

	TVPExecuteExpression(TJS_W("const gpDInput = 3, gpFFDInput = 2, gpXInput = 1,\
		gpButtonDpadUp = 0x00000001, gpButtonDpadDown = 0x00000002, gpButtonDpadLeft = 0x00000004,\
		gpButtonDpadRight = 0x00000008, gpButtonStart = 0x00000010, gpButtonBack = 0x00000020,\
		gpButtonLeftThumb = 0x00000040, gpButtonRightThumb = 0x00000080, gpButtonLeftShoulder = 0x00000100,\
		gpButtonRightShoulder = 0x00000200, gpButtonA = 0x00001000, gpButtonB = 0x00002000,\
		gpButtonX = 0x00004000, gpButtonY = 0x00008000, gpLeftAxisX = 0x00010000, gpLeftAxisY = 0x00020000,\
		gpRightAxisX = 0x00040000, gpRightAxisY = 0x00080000, gpLeftTrigger = 0x00100000, gpRightTrigger = 0x00200000,\
		gpDIAxisX = 0, gpDIAxisY = 1, gpDIAxisZ = 2, gpDIAxisRotX = 3, gpDIAxisRotY = 4, gpDIAxisRotZ = 5,\
		gpDISlider_0 = 6, gpDISlider_1 = 7, gpDIPOV_0 = 8, gpDIPOV_1 = 9, gpDIPOV_2 = 10, gpDIPOV_3 = 11,\
		gpDIButton1 = 12, gpDIButton2 = 13, gpDIButton3 = 14, gpDIButton4 = 15, gpDIButton5 = 16,\
		gpDIButton6 = 17, gpDIButton7 = 18, gpDIButton8 = 19, gpDIButton9 = 20, gpDIButton10 = 21,\
		gpDIButton11 = 22, gpDIButton12 = 23, gpDIButton13 = 24, gpDIButton14 = 25, gpDIButton15 = 26,\
		gpDIButton16 = 27, gpDIButton17 = 28, gpDIButton18 = 29, gpDIButton19 = 30, gpDIButton20 = 31,\
		gpDIButton21 = 32, gpDIButton22 = 33, gpDIButton23 = 34, gpDIButton24 = 35, gpDIButton25 = 36,\
		gpDIButton26 = 37, gpDIButton27 = 38, gpDIButton28 = 39, gpDIButton29 = 40, gpDIButton30 = 41,\
		gpDIButton31 = 42, gpDIButton32 = 43, gpDIDisable = 44,\
		gpTouchNo = 0, gpTouchDown = 1, gpTouchLiftoff = 0;"));
	{
		//-----------------------------------------------------------------------
		// 1 �܂��N���X�I�u�W�F�N�g���쐬
		iTJSDispatch2 * tjsclass = Create_NC_GamepadPort();

		// 2 tjsclass �� tTJSVariant �^�ɕϊ�
		val = tTJSVariant(tjsclass);

		// 3 ���ł� val �� tjsclass ��ێ����Ă���̂ŁAtjsclass ��
		//   Release ����
		tjsclass->Release();

		// 4 global �� PropSet ���\�b�h��p���A�I�u�W�F�N�g��o�^����
		global->PropSet(
			TJS_MEMBERENSURE, // �����o���Ȃ������ꍇ�ɂ͍쐬����悤�ɂ���t���O
			TJS_W("GamepadPort"), // �����o�� ( ���Ȃ炸 TJS_W( ) �ň͂� )
			NULL, // �q���g ( �{���̓����o���̃n�b�V���l�����ANULL �ł��悢 )
			&val, // �o�^����l
			global // �R���e�L�X�g ( global �ł悢 )
			);
		//-----------------------------------------------------------------------

		//-----------------------------------------------------------------------
		// 1 �܂��N���X�I�u�W�F�N�g���쐬
		tjsclass = Create_NC_Gamepad();

		// 2 tjsclass �� tTJSVariant �^�ɕϊ�
		val = tTJSVariant(tjsclass);

		// 3 ���ł� val �� tjsclass ��ێ����Ă���̂ŁAtjsclass ��
		//   Release ����
		tjsclass->Release();

		// 4 global �� PropSet ���\�b�h��p���A�I�u�W�F�N�g��o�^����
		global->PropSet(
			TJS_MEMBERENSURE, // �����o���Ȃ������ꍇ�ɂ͍쐬����悤�ɂ���t���O
			TJS_W("Gamepad"), // �����o�� ( ���Ȃ炸 TJS_W( ) �ň͂� )
			NULL, // �q���g ( �{���̓����o���̃n�b�V���l�����ANULL �ł��悢 )
			&val, // �o�^����l
			global // �R���e�L�X�g ( global �ł悢 )
			);
		//-----------------------------------------------------------------------
	}

	// - global �� Release ����
	global->Release();

	// �����A�o�^����֐�����������ꍇ�� 1 �` 4 ���J��Ԃ�

	// val ���N���A����B
	// ����͕K���s���B�������Ȃ��� val ���ێ����Ă���I�u�W�F�N�g
	// �� Release ���ꂸ�A���Ɏg�� TVPPluginGlobalRefCount �����m�ɂȂ�Ȃ��B
	val.Clear();


	// ���̎��_�ł� TVPPluginGlobalRefCount �̒l��
	GlobalRefCountAtInit = TVPPluginGlobalRefCount;
	// �Ƃ��čT���Ă����BTVPPluginGlobalRefCount �͂��̃v���O�C������
	// �Ǘ�����Ă��� tTJSDispatch �h���I�u�W�F�N�g�̎Q�ƃJ�E���^�̑��v�ŁA
	// ������ɂ͂���Ɠ������A����������Ȃ��Ȃ��ĂȂ��ƂȂ�Ȃ��B
	// �����Ȃ��ĂȂ���΁A�ǂ����ʂ̂Ƃ���Ŋ֐��Ȃǂ��Q�Ƃ���Ă��āA
	// �v���O�C���͉���ł��Ȃ��ƌ������ƂɂȂ�B

	return S_OK;
}
//---------------------------------------------------------------------------
extern "C" __declspec(dllexport) HRESULT _stdcall V2Unlink()
{
	// �g���g��������A�v���O�C����������悤�Ƃ���Ƃ��ɌĂ΂��֐��B

	// �������炩�̏����Ńv���O�C��������ł��Ȃ��ꍇ��
	// ���̎��_�� E_FAIL ��Ԃ��悤�ɂ���B
	// �����ł́ATVPPluginGlobalRefCount �� GlobalRefCountAtInit ����
	// �傫���Ȃ��Ă���Ύ��s�Ƃ������Ƃɂ���B
	if(TVPPluginGlobalRefCount > GlobalRefCountAtInit) return E_FAIL;
		// E_FAIL ���A��ƁAPlugins.unlink ���\�b�h�͋U��Ԃ�

	/*
		�������A�N���X�̏ꍇ�A�����Ɂu�I�u�W�F�N�g���g�p���ł���v�Ƃ������Ƃ�
		�m�邷�ׂ�����܂���B��{�I�ɂ́APlugins.unlink �ɂ��v���O�C���̉����
		�댯�ł���ƍl���Ă������� (�������� Plugins.link �Ń����N������A�Ō��
		�Ńv���O�C������������A�v���O�����I���Ɠ����Ɏ����I�ɉ��������̂��g)�B
	*/


	// - �܂��ATJS �̃O���[�o���I�u�W�F�N�g���擾����
	iTJSDispatch2 * global = TVPGetScriptDispatch();

	// - global �� DeleteMember ���\�b�h��p���A�I�u�W�F�N�g���폜����
	if(global)
	{
		// TJS ���̂����ɉ������Ă����Ƃ��Ȃǂ�
		// global �� NULL �ɂȂ蓾��̂� global �� NULL �łȂ�
		// ���Ƃ��`�F�b�N����

		global->DeleteMember(
			0, // �t���O ( 0 �ł悢 )
			TJS_W("Gamepad"), // �����o��
			NULL, // �q���g
			global // �R���e�L�X�g
			);

		global->DeleteMember(
			0, // �t���O ( 0 �ł悢 )
			TJS_W("GamepadPort"), // �����o��
			NULL, // �q���g
			global // �R���e�L�X�g
			);
	}

	// - global �� Release ����
	if(global) global->Release();

	// �X�^�u�̎g�p�I��(�K���L�q����)
	TVPUninitImportStub();

	return S_OK;
}
//---------------------------------------------------------------------------

