/**
 * @file CDirectInput.h
 * @brief DirectInput�L�[�{�[�h�E�}�E�X
 * @author �D�ˁ@�엲
 * @date ���t�i2017.02.14�j
 */
#pragma once

#include "stdafx.h"
#include "Common3DType.h"

/**
 * �萔
 */

#define KEY_PRESSED			0x80
#define IS_KEY_PRESSED(x)	(x & KEY_PRESSED)
#define BUFFER_SIZE			256

/*! @class CDirectInput
@brief  DirectInput�L�[�{�[�h�E�}�E�X
*/
class CDirectInput
{
	// Properties
	static std::shared_ptr<CDirectInput>	g_instance;
	LPDIRECTINPUT8			m_pDinput;		//!<  DirectInput �I�u�W�F�N�g
	LPDIRECTINPUTDEVICE8	m_pKeyDevice;	//!<  �L�[�{�[�h				
	LPDIRECTINPUTDEVICE8	m_pMouseDevice;	//!<  �}�E�X		
	LPDIRECTINPUTDEVICE8	m_pController;	// �R���g���[���[
	DIDEVCAPS				m_devCaps;

	BYTE m_keyboardInput[BUFFER_SIZE];		//!<  �L�[�{�[�h�̓��͏��	
	BYTE m_keyboardInputPrev[BUFFER_SIZE];	//!<  �O��̃L�[�{�[�h�̓��͏��

	BOOL m_pressed[BUFFER_SIZE];
	BOOL m_pressedOnce[BUFFER_SIZE];
	DIMOUSESTATE2	m_diMouseState2;		//!<  �}�E�X�̓��͏��	
	DIMOUSESTATE2	m_mouseInputPrev;		//!<  �O��̃}�E�X�̓��͏��

	DIJOYSTATE m_controllerButton;			// �R���g���[���[�̓��͏��
	DIJOYSTATE m_controllerButtonPrev;		// �O��̃R���g���[���[�̓��͏��
	FLOAT m_lXValue;						// �A�i���O�X�e�B�b�N��x��
	FLOAT m_lYValue;						// �A�i���O�X�e�B�b�N��y��

	// Methods
	CDirectInput();							//!<  �y�V���O���g���E�p�^�[���z
	HRESULT init(HWND inHWnd);
	HRESULT initKeyboard(HWND inHWnd);
	HRESULT initMouse(HWND inHWnd);
	static BOOL CALLBACK enumJoystickCallback(LPCDIDEVICEINSTANCE pdidInstance, LPVOID pContext);
	static BOOL CALLBACK enumAxesCallback(LPCDIDEVICEOBJECTINSTANCE pdidoi, LPVOID pContext);
	HRESULT initController(HWND inHWnd);
	VOID free();
public:
	~CDirectInput();						//!<  �X�}�[�g�|�C���^�@�Ɓ@�y�V���O���g���E�p�^�[���z
	// Methids
	static std::shared_ptr<CDirectInput> getInstance(HWND inHWnd);	//!<  �C���X�^���X���擾
	static VOID releaseInstance();					//!<  �C���X�^���X�����

	HRESULT getState();

	BOOL isKeyPressed(BYTE aKey);
	BOOL isPressedOnce(BYTE aKey);
	BOOL isKeyTrigger(BYTE aKey);
	BOOL isKeyReleased(BYTE aKey);

	BOOL isLeftButtonClicked();
	BOOL isRightButtonClicked();
	BOOL isCenterButtonClicked();

	FLOAT getHorizontal()const { return m_lXValue; }
	FLOAT getVertical()const { return m_lYValue; }
};