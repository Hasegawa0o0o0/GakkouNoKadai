/**
 * @file CDirectInput.cpp
 * @brief DirectInput�L�[�{�[�h�̃N���X
 * @author �D�ˁ@�엲
 * @date ���t�i2017.02.14�j
 */

#include "stdafx.h"
#include "CDirectInput.h"

std::shared_ptr<CDirectInput> CDirectInput::g_instance = NULL;	 //!< �y�N���X�ϐ��z

/**
 * Private Methods
 */
CDirectInput::CDirectInput()
{
}

CDirectInput::~CDirectInput()
{
}

/**
 * @fn HRESULT CDirectInput::init(HWND inHWnd)
 * @brief ������
 * @param hWnd_		WindowsAPI�E�C���h�E�n���h��
 * @return			WindowsAPI ���s����
 */
HRESULT CDirectInput::init(HWND inHWnd)
{
	m_pDinput = NULL;		 //!< DirectInput �I�u�W�F�N�g
	m_pKeyDevice = NULL;	 //!< �L�[�{�[�h
	m_pMouseDevice = NULL;	 //!< �}�E�X
	m_pController = NULL;	 // �R���g���[���[

	ZeroMemory(&m_keyboardInput, sizeof(BYTE) * BUFFER_SIZE);		 //!< �L�[�{�[�h�̓��͏��
	ZeroMemory(&m_keyboardInputPrev, sizeof(BYTE) * BUFFER_SIZE);	 //!< �O��̃L�[�{�[�h�̓��͏��

	ZeroMemory(&m_pressed, sizeof(BYTE) * BUFFER_SIZE);		 //!< �����ꂽ
	ZeroMemory(&m_pressedOnce, sizeof(BYTE) * BUFFER_SIZE);	 //!< �����ꑱ����

	ZeroMemory(&m_diMouseState2, sizeof(DIMOUSESTATE2));	 //!< �}�E�X�̓��͏��

	ZeroMemory(&m_controllerButton, sizeof(DIJOYSTATE));		// �R���g���[���[�̓��͏��
	ZeroMemory(&m_controllerButtonPrev, sizeof(DIJOYSTATE));	// �O��̃R���g���[���[�̓��͏��

	/**
	 * DirectInput�I�u�W�F�N�g�̐���
	 */
	if (FAILED(DirectInput8Create(GetModuleHandle(NULL),
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(VOID**)&m_pDinput,
		NULL))) {
		return E_FAIL;
	}
	/**
	 *�L�[�{�[�h�̏�����
	 *�L�[�{�[�h�̃f�o�C�X�I�u�W�F�N�g�̐���
	 */
	initKeyboard(inHWnd);
	
	// �}�E�X�̃f�o�C�X�I�u�W�F�N�g���쐬
	initMouse(inHWnd);

	// �R���g���[���[�̃f�o�C�X�I�u�W�F�N�g���쐬
	initController(inHWnd);

	return S_OK;
}

/**
* @fn HRESULT CDirectInput::initKeyboard(HWND inHWnd)
* @brief �L�[�{�[�h�̏�����
* @param hWnd_		WindowsAPI�E�C���h�E�n���h��
* @return			WindowsAPI ���s����
*/
HRESULT CDirectInput::initKeyboard(HWND inHWnd)
{
	if (FAILED(m_pDinput->CreateDevice(GUID_SysKeyboard, &m_pKeyDevice, NULL))) {
		return E_FAIL;
	}
	/**
	 *�f�o�C�X���L�[�{�[�h�ɐݒ�
	 */
	if (FAILED(m_pKeyDevice->SetDataFormat(&c_dfDIKeyboard))) {
		return E_FAIL;
	}
	/**
	 *�������x���̐ݒ�
	 */
	if (FAILED(m_pKeyDevice->SetCooperativeLevel(inHWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND))) {
		return E_FAIL;
	}
	/**
	 *���͐���J�n
	 */
	m_pKeyDevice->Acquire();
	return S_OK;
}

/**
* @fn HRESULT CDirectInput::initMouse(HWND inHWnd)
* @brief �}�E�X�̏�����
* @param hWnd_		WindowsAPI�E�C���h�E�n���h��
* @return			WindowsAPI ���s����
*/
HRESULT CDirectInput::initMouse(HWND inHWnd)
{
	if (FAILED(m_pDinput->CreateDevice(GUID_SysMouse, &m_pMouseDevice, NULL))) {
		return false;
	}
	// �f�[�^�t�H�[�}�b�g��ݒ�
	// �}�E�X�p�̃f�[�^�E�t�H�[�}�b�g��ݒ�
	if (FAILED(m_pMouseDevice->SetDataFormat(&c_dfDIMouse2))) {
		return E_FAIL;
	}
	// ���[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	if (FAILED(m_pMouseDevice->SetCooperativeLevel(inHWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND))) {
		return E_FAIL;
	}
	//// �f�o�C�X�̐ݒ�
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;	// ���Βl���[�h�Őݒ�i��Βl��DIPROPAXISMODE_ABS�j

	if (FAILED(m_pMouseDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph))) {
		return E_FAIL;
	}
	// ���͐���J�n
	m_pMouseDevice->Acquire();
	return S_OK;
}

// �R���g���[���[�����̂��߂̃R�[���o�b�N���\�b�h
BOOL CALLBACK CDirectInput::enumJoystickCallback(LPCDIDEVICEINSTANCE pdidInstance, LPVOID pContext)
{
	if (FAILED(CDirectInput::g_instance->m_pDinput->CreateDevice(pdidInstance->guidInstance, &CDirectInput::g_instance->m_pController, NULL)))
	{
		return DIENUM_CONTINUE;
	}
	return DIENUM_STOP;
}

// �\���L�[�͈̔͂�����������Ƃ��̃R�[���o�b�N���\�b�h
BOOL CALLBACK CDirectInput::enumAxesCallback(LPCDIDEVICEOBJECTINSTANCE pdidoi, LPVOID pContext)
{
	DIPROPRANGE diprg;
	diprg.diph.dwSize		= sizeof(DIPROPRANGE);
	diprg.diph.dwHeaderSize	= sizeof(DIPROPHEADER);
	diprg.diph.dwHow		= DIPH_BYOFFSET;
	diprg.lMin				= -1000;
	diprg.lMax				= 1000;
	diprg.diph.dwObj		= DIJOFS_X;
	if (FAILED(CDirectInput::g_instance->m_pController->SetProperty(DIPROP_RANGE, &diprg.diph)))
	{
		return DIENUM_STOP;
	}
	diprg.diph.dwObj = DIJOFS_Y;
	if (FAILED(CDirectInput::g_instance->m_pController->SetProperty(DIPROP_RANGE, &diprg.diph)))
	{
		return DIENUM_STOP;
	}
	diprg.diph.dwObj = DIJOFS_Z;
	if (FAILED(CDirectInput::g_instance->m_pController->SetProperty(DIPROP_RANGE, &diprg.diph)))
	{
		return DIENUM_STOP;
	}
	diprg.diph.dwObj = DIJOFS_RX;
	if (FAILED(CDirectInput::g_instance->m_pController->SetProperty(DIPROP_RANGE, &diprg.diph)))
	{
		return DIENUM_STOP;
	}
	diprg.diph.dwObj = DIJOFS_RY;
	if (FAILED(CDirectInput::g_instance->m_pController->SetProperty(DIPROP_RANGE, &diprg.diph)))
	{
		return DIENUM_STOP;
	}
	diprg.diph.dwObj = DIJOFS_RZ;
	if (FAILED(CDirectInput::g_instance->m_pController->SetProperty(DIPROP_RANGE, &diprg.diph)))
	{
		return DIENUM_STOP;
	}
	CDirectInput::g_instance->m_pController->Poll();
	return DIENUM_CONTINUE;
}

// HRESULT CDirectInput::initController(HWND inHWnd)
// �R���g���[���[�̏�����
// hWnd		WindowsAPI�E�B���h�E�n���h��
// return	WindowsAPI���s����
HRESULT CDirectInput::initController(HWND inHWnd)
{
	if (FAILED(m_pDinput->EnumDevices(DI8DEVCLASS_GAMECTRL, enumJoystickCallback, NULL, DIEDFL_ATTACHEDONLY)))
	{
		return E_FAIL;
	}
	// �R���g���[���[���ڑ�����Ă��Ȃ�������I��
	if (!m_pController)
	{
		return E_FAIL;
	}
	// �f�[�^�t�H�[�}�b�g���쐬
	if (FAILED(m_pController->SetDataFormat(&c_dfDIJoystick)))
	{
		return E_FAIL;
	}
	// �������x����ݒ�
	if (FAILED(m_pController->SetCooperativeLevel(inHWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
	{
		return E_FAIL;
	}
	// �f�o�C�X�̔\�͂��擾
	//if (FAILED(m_pController->GetCapabilities(&m_devCaps)))
	//{
	//	return E_FAIL;
	//}
	// �\���L�[�͈̔͂��w�肷��
	if (FAILED(m_pController->EnumObjects(enumAxesCallback, (LPVOID)inHWnd, DIDFT_AXIS)))
	{
		return E_FAIL;
	}
	m_pController->Acquire();
	m_pController->Poll();
	return S_OK;
}


/**
 * @fn VOID CDirectInput::free()
 * @brief DirectInput�̃I�u�W�F�N�g�̊J��
 * @param	����
 * @return	����
 */
VOID CDirectInput::free()
{
	if (m_pMouseDevice)
		m_pMouseDevice->Unacquire();
	SAFE_RELEASE(m_pMouseDevice);	 //!< �}�E�X

	if (m_pKeyDevice)
		m_pKeyDevice->Unacquire();
	SAFE_RELEASE(m_pKeyDevice);		//!< �L�[�{�[�h

	if (m_pController)
		m_pController->Unacquire();
	SAFE_RELEASE(m_pController);	// �R���g���[���[

	SAFE_RELEASE(m_pDinput);		//!< DirectInput �I�u�W�F�N�g
}

//__________________________________________________________________
// Public Methods
//__________________________________________________________________

/**
 * @fn std::shared_ptr<CDirectInput> CDirectInput::getInstance(HWND inHWnd)
 * @brief �C���X�^���X���擾 �y�N���X���\�b�h�z<Singleton-pattern>
 * @param inHWnd	�����̔z��
 * @return		�C���X�^���X
 */
std::shared_ptr<CDirectInput> CDirectInput::getInstance(HWND inHWnd)
{
	if (g_instance == NULL) {
		g_instance = shared_ptr<CDirectInput>(new CDirectInput());
		g_instance->init(inHWnd);
	}
	return g_instance;
}

/**
 * @fn VOID CDirectInput::releaseInstance()
 * @brief �C���X�^���X����� �y�N���X���\�b�h�z<Singleton-pattern>
 * @param	����
 * @return	����
 */
VOID CDirectInput::releaseInstance()
{
	g_instance->free();
}

/**
 * @fn HRESULT CDirectInput::getState()
 * @brief �L�[�{�[�h�E�}�E�X�̃X�e�[�^�X�̎擾
 * @param	����
 * @return   WindowsAPI ���s����
 */
//
HRESULT CDirectInput::getState()
{
	/**
	 *�L�[�{�[�h�̍X�V
	 */
	HRESULT hr = m_pKeyDevice->Acquire();

	if ((hr == DI_OK) || (hr == S_FALSE)) {
		memcpy(m_keyboardInputPrev, m_keyboardInput, sizeof(BYTE) * BUFFER_SIZE);	 //!<�O��̏�Ԃ�ޔ�

		m_pKeyDevice->GetDeviceState(sizeof(m_keyboardInput), &m_keyboardInput);	 //!<�L�[�{�[�h�̏�Ԃ��擾
		for (int i = 0; i < BUFFER_SIZE; i++) {
			if (IS_KEY_PRESSED(m_keyboardInput[i])) {
				if ((!m_pressedOnce[i]) && (!m_pressed[i])) {
 					m_pressedOnce[i] = TRUE;		 //!< ���߂ĉ����ꂽ
					m_pressed[i] = TRUE;
				}
				else {
					m_pressedOnce[i] = FALSE;
				}
			}
			else {
				m_pressedOnce[i] = FALSE;
				m_pressed[i] = FALSE;
			}
		}
	}

	/**
	*�}�E�X�̏�Ԃ��擾���܂�
	*/
	hr = m_pMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &m_diMouseState2);
	if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED) {
		hr = m_pMouseDevice->Acquire();
		while (hr == DIERR_INPUTLOST)
			hr = m_pMouseDevice->Acquire();
	}

	// �R���g���[���[���ڑ�����Ă��Ȃ���ΏI��
	if (!m_pController)
	{
		return S_OK;
	}
	// �R���g���[���[�̏�Ԃ��擾
	hr = m_pController->Acquire();
	if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED) {
		hr = m_pController->Acquire();
		while (hr == DIERR_INPUTLOST)
			hr = m_pController->Acquire();
	}
	memcpy(m_controllerButtonPrev.rgbButtons, m_controllerButton.rgbButtons, sizeof(m_controllerButtonPrev.rgbButtons));	 //!<�O��̏�Ԃ�ޔ�
	hr = m_pController->GetDeviceState(sizeof(DIJOYSTATE), &m_controllerButton);	 // �R���g���[���[�̏�Ԃ��擾
	if (hr == DI_OK)
	{
		for (int i = 0; i < (int)size(m_controllerButton.rgbButtons); i++) {
			if (IS_KEY_PRESSED(m_controllerButton.rgbButtons[i])) {
				if ((!m_pressedOnce[i]) && (!m_pressed[i])) {
					m_pressedOnce[i] = TRUE;		 //!< ���߂ĉ����ꂽ
					m_pressed[i] = TRUE;
				}
				else {
					m_pressedOnce[i] = FALSE;
				}
			}
			else {
				m_pressedOnce[i] = FALSE;
				m_pressed[i] = FALSE;
			}
		}
		m_lXValue = m_controllerButton.lX;
		m_lYValue = m_controllerButton.lY;
	}

	return S_OK;
}

/**
 * @fn BOOL CDirectInput::isKeyPressed(BYTE aKey)
 * @brief �L�[�{�[�h�̉���
 * @param aKey	���ׂ�L�[
 * @return		�����ꂽ��
 */
BOOL CDirectInput::isKeyPressed(BYTE aKey)
{
	/**
	 *���݉����Ă���
	 */
	if (IS_KEY_PRESSED(m_keyboardInput[aKey]))
	{
		return TRUE;
	}
	else if (m_pressed[aKey])
	{
		return TRUE;
	}
	return FALSE;
}

/**
 * @fn BOOL CDirectInput::isPressedOnce(BYTE aKey)
 * @brief �w�肳�ꂽ�L�[�̉�������x������
 * @param aKey	���ׂ�L�[
 * @return		�����ꂽ��
 */
BOOL CDirectInput::isPressedOnce(BYTE aKey)
{
	return m_pressedOnce[aKey];
}

/**
* @fn BOOL CDirectInput::isKeyTrigger(BYTE aKey)
* @brief  �w�肳�ꂽ�L�[�̃g���K�[
* @param aKey	���ׂ�L�[
* @return		�O�񉟂��Ă��Č��݉����Ă���
*/
BOOL CDirectInput::isKeyTrigger(BYTE aKey)
{
	/**
	 *�O�񉟂��Ă��Č��݉����Ă���
	 */
	if ((IS_KEY_PRESSED(m_keyboardInputPrev[aKey])) && (IS_KEY_PRESSED(m_keyboardInput[aKey])))
	{
		return TRUE;
	}
	return FALSE;
}

/**
 * @fn BOOL CDirectInput::isKeyReleased(BYTE aKey)
 * @brief  �w�肳�ꂽ�L�[�̃`�F�b�N
 * @param aKey	���ׂ�L�[
 * @return		�O�񉟂��Ă��Č��ݗ����Ă���
 */
BOOL CDirectInput::isKeyReleased(BYTE aKey)
{
	/**
	 *�O�񉟂��Ă��Č��ݗ����Ă���
	 */
	if ((IS_KEY_PRESSED(m_keyboardInputPrev[aKey])) && !(IS_KEY_PRESSED(m_keyboardInput[aKey])))
	{
		return TRUE;
	}
	return FALSE;
}

/**
* @fn BOOL CDirectInput::isLeftButtonClicked()
* @brief  �}�E�X�̍��{�^���̃`�F�b�N
* @return		�����Ă���
*/
BOOL CDirectInput::isLeftButtonClicked()
{
	return m_diMouseState2.rgbButtons[0] & 0x80;//	1000 0000
}

/**
* @fn BOOL CDirectInput::isRightButtonClicked()
* @brief  �}�E�X�̉E�{�^���̃`�F�b�N
* @return		�����Ă���
*/
BOOL CDirectInput::isRightButtonClicked()
{
	return m_diMouseState2.rgbButtons[1] & 0x80;
}

/**
* @fn BOOL CDirectInput::isCenterButtonClicked()
* @brief  �}�E�X�̒��{�^���̃`�F�b�N
* @return		�����Ă���
*/
BOOL CDirectInput::isCenterButtonClicked()
{
	return m_diMouseState2.rgbButtons[2] & 0x80;
}