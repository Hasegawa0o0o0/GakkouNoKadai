/**
 * @file CDirectInput.cpp
 * @brief DirectInputキーボードのクラス
 * @author 織戸　喜隆
 * @date 日付（2017.02.14）
 */

#include "stdafx.h"
#include "CDirectInput.h"

std::shared_ptr<CDirectInput> CDirectInput::g_instance = NULL;	 //!< 【クラス変数】

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
 * @brief 初期化
 * @param hWnd_		WindowsAPIウインドウハンドル
 * @return			WindowsAPI 実行結果
 */
HRESULT CDirectInput::init(HWND inHWnd)
{
	m_pDinput = NULL;		 //!< DirectInput オブジェクト
	m_pKeyDevice = NULL;	 //!< キーボード
	m_pMouseDevice = NULL;	 //!< マウス
	m_pController = NULL;	 // コントローラー

	ZeroMemory(&m_keyboardInput, sizeof(BYTE) * BUFFER_SIZE);		 //!< キーボードの入力情報
	ZeroMemory(&m_keyboardInputPrev, sizeof(BYTE) * BUFFER_SIZE);	 //!< 前回のキーボードの入力情報

	ZeroMemory(&m_pressed, sizeof(BYTE) * BUFFER_SIZE);		 //!< 押された
	ZeroMemory(&m_pressedOnce, sizeof(BYTE) * BUFFER_SIZE);	 //!< 押され続けた

	ZeroMemory(&m_diMouseState2, sizeof(DIMOUSESTATE2));	 //!< マウスの入力情報

	ZeroMemory(&m_controllerButton, sizeof(DIJOYSTATE));		// コントローラーの入力情報
	ZeroMemory(&m_controllerButtonPrev, sizeof(DIJOYSTATE));	// 前回のコントローラーの入力情報

	/**
	 * DirectInputオブジェクトの生成
	 */
	if (FAILED(DirectInput8Create(GetModuleHandle(NULL),
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(VOID**)&m_pDinput,
		NULL))) {
		return E_FAIL;
	}
	/**
	 *キーボードの初期化
	 *キーボードのデバイスオブジェクトの生成
	 */
	initKeyboard(inHWnd);
	
	// マウスのデバイスオブジェクトを作成
	initMouse(inHWnd);

	// コントローラーのデバイスオブジェクトを作成
	initController(inHWnd);

	return S_OK;
}

/**
* @fn HRESULT CDirectInput::initKeyboard(HWND inHWnd)
* @brief キーボードの初期化
* @param hWnd_		WindowsAPIウインドウハンドル
* @return			WindowsAPI 実行結果
*/
HRESULT CDirectInput::initKeyboard(HWND inHWnd)
{
	if (FAILED(m_pDinput->CreateDevice(GUID_SysKeyboard, &m_pKeyDevice, NULL))) {
		return E_FAIL;
	}
	/**
	 *デバイスをキーボードに設定
	 */
	if (FAILED(m_pKeyDevice->SetDataFormat(&c_dfDIKeyboard))) {
		return E_FAIL;
	}
	/**
	 *協調レベルの設定
	 */
	if (FAILED(m_pKeyDevice->SetCooperativeLevel(inHWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND))) {
		return E_FAIL;
	}
	/**
	 *入力制御開始
	 */
	m_pKeyDevice->Acquire();
	return S_OK;
}

/**
* @fn HRESULT CDirectInput::initMouse(HWND inHWnd)
* @brief マウスの初期化
* @param hWnd_		WindowsAPIウインドウハンドル
* @return			WindowsAPI 実行結果
*/
HRESULT CDirectInput::initMouse(HWND inHWnd)
{
	if (FAILED(m_pDinput->CreateDevice(GUID_SysMouse, &m_pMouseDevice, NULL))) {
		return false;
	}
	// データフォーマットを設定
	// マウス用のデータ・フォーマットを設定
	if (FAILED(m_pMouseDevice->SetDataFormat(&c_dfDIMouse2))) {
		return E_FAIL;
	}
	// モードを設定（フォアグラウンド＆非排他モード）
	if (FAILED(m_pMouseDevice->SetCooperativeLevel(inHWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND))) {
		return E_FAIL;
	}
	//// デバイスの設定
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;	// 相対値モードで設定（絶対値はDIPROPAXISMODE_ABS）

	if (FAILED(m_pMouseDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph))) {
		return E_FAIL;
	}
	// 入力制御開始
	m_pMouseDevice->Acquire();
	return S_OK;
}

// コントローラー生成のためのコールバックメソッド
BOOL CALLBACK CDirectInput::enumJoystickCallback(LPCDIDEVICEINSTANCE pdidInstance, LPVOID pContext)
{
	if (FAILED(CDirectInput::g_instance->m_pDinput->CreateDevice(pdidInstance->guidInstance, &CDirectInput::g_instance->m_pController, NULL)))
	{
		return DIENUM_CONTINUE;
	}
	return DIENUM_STOP;
}

// 十字キーの範囲をしたいするときのコールバックメソッド
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
// コントローラーの初期化
// hWnd		WindowsAPIウィンドウハンドル
// return	WindowsAPI実行結果
HRESULT CDirectInput::initController(HWND inHWnd)
{
	if (FAILED(m_pDinput->EnumDevices(DI8DEVCLASS_GAMECTRL, enumJoystickCallback, NULL, DIEDFL_ATTACHEDONLY)))
	{
		return E_FAIL;
	}
	// コントローラーが接続されていなかったら終了
	if (!m_pController)
	{
		return E_FAIL;
	}
	// データフォーマットを作成
	if (FAILED(m_pController->SetDataFormat(&c_dfDIJoystick)))
	{
		return E_FAIL;
	}
	// 協調レベルを設定
	if (FAILED(m_pController->SetCooperativeLevel(inHWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
	{
		return E_FAIL;
	}
	// デバイスの能力を取得
	//if (FAILED(m_pController->GetCapabilities(&m_devCaps)))
	//{
	//	return E_FAIL;
	//}
	// 十字キーの範囲を指定する
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
 * @brief DirectInputのオブジェクトの開放
 * @param	無し
 * @return	無し
 */
VOID CDirectInput::free()
{
	if (m_pMouseDevice)
		m_pMouseDevice->Unacquire();
	SAFE_RELEASE(m_pMouseDevice);	 //!< マウス

	if (m_pKeyDevice)
		m_pKeyDevice->Unacquire();
	SAFE_RELEASE(m_pKeyDevice);		//!< キーボード

	if (m_pController)
		m_pController->Unacquire();
	SAFE_RELEASE(m_pController);	// コントローラー

	SAFE_RELEASE(m_pDinput);		//!< DirectInput オブジェクト
}

//__________________________________________________________________
// Public Methods
//__________________________________________________________________

/**
 * @fn std::shared_ptr<CDirectInput> CDirectInput::getInstance(HWND inHWnd)
 * @brief インスタンスを取得 【クラスメソッド】<Singleton-pattern>
 * @param inHWnd	方向の配列
 * @return		インスタンス
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
 * @brief インスタンスを解放 【クラスメソッド】<Singleton-pattern>
 * @param	無し
 * @return	無し
 */
VOID CDirectInput::releaseInstance()
{
	g_instance->free();
}

/**
 * @fn HRESULT CDirectInput::getState()
 * @brief キーボード・マウスのステータスの取得
 * @param	無し
 * @return   WindowsAPI 実行結果
 */
//
HRESULT CDirectInput::getState()
{
	/**
	 *キーボードの更新
	 */
	HRESULT hr = m_pKeyDevice->Acquire();

	if ((hr == DI_OK) || (hr == S_FALSE)) {
		memcpy(m_keyboardInputPrev, m_keyboardInput, sizeof(BYTE) * BUFFER_SIZE);	 //!<前回の状態を退避

		m_pKeyDevice->GetDeviceState(sizeof(m_keyboardInput), &m_keyboardInput);	 //!<キーボードの状態を取得
		for (int i = 0; i < BUFFER_SIZE; i++) {
			if (IS_KEY_PRESSED(m_keyboardInput[i])) {
				if ((!m_pressedOnce[i]) && (!m_pressed[i])) {
 					m_pressedOnce[i] = TRUE;		 //!< 初めて押された
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
	*マウスの状態を取得します
	*/
	hr = m_pMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &m_diMouseState2);
	if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED) {
		hr = m_pMouseDevice->Acquire();
		while (hr == DIERR_INPUTLOST)
			hr = m_pMouseDevice->Acquire();
	}

	// コントローラーが接続されていなければ終了
	if (!m_pController)
	{
		return S_OK;
	}
	// コントローラーの状態を取得
	hr = m_pController->Acquire();
	if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED) {
		hr = m_pController->Acquire();
		while (hr == DIERR_INPUTLOST)
			hr = m_pController->Acquire();
	}
	memcpy(m_controllerButtonPrev.rgbButtons, m_controllerButton.rgbButtons, sizeof(m_controllerButtonPrev.rgbButtons));	 //!<前回の状態を退避
	hr = m_pController->GetDeviceState(sizeof(DIJOYSTATE), &m_controllerButton);	 // コントローラーの状態を取得
	if (hr == DI_OK)
	{
		for (int i = 0; i < (int)size(m_controllerButton.rgbButtons); i++) {
			if (IS_KEY_PRESSED(m_controllerButton.rgbButtons[i])) {
				if ((!m_pressedOnce[i]) && (!m_pressed[i])) {
					m_pressedOnce[i] = TRUE;		 //!< 初めて押された
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
 * @brief キーボードの押下
 * @param aKey	調べるキー
 * @return		押されたか
 */
BOOL CDirectInput::isKeyPressed(BYTE aKey)
{
	/**
	 *現在押している
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
 * @brief 指定されたキーの押下が一度だけか
 * @param aKey	調べるキー
 * @return		押されたか
 */
BOOL CDirectInput::isPressedOnce(BYTE aKey)
{
	return m_pressedOnce[aKey];
}

/**
* @fn BOOL CDirectInput::isKeyTrigger(BYTE aKey)
* @brief  指定されたキーのトリガー
* @param aKey	調べるキー
* @return		前回押していて現在押している
*/
BOOL CDirectInput::isKeyTrigger(BYTE aKey)
{
	/**
	 *前回押していて現在押している
	 */
	if ((IS_KEY_PRESSED(m_keyboardInputPrev[aKey])) && (IS_KEY_PRESSED(m_keyboardInput[aKey])))
	{
		return TRUE;
	}
	return FALSE;
}

/**
 * @fn BOOL CDirectInput::isKeyReleased(BYTE aKey)
 * @brief  指定されたキーのチェック
 * @param aKey	調べるキー
 * @return		前回押していて現在離している
 */
BOOL CDirectInput::isKeyReleased(BYTE aKey)
{
	/**
	 *前回押していて現在離している
	 */
	if ((IS_KEY_PRESSED(m_keyboardInputPrev[aKey])) && !(IS_KEY_PRESSED(m_keyboardInput[aKey])))
	{
		return TRUE;
	}
	return FALSE;
}

/**
* @fn BOOL CDirectInput::isLeftButtonClicked()
* @brief  マウスの左ボタンのチェック
* @return		押している
*/
BOOL CDirectInput::isLeftButtonClicked()
{
	return m_diMouseState2.rgbButtons[0] & 0x80;//	1000 0000
}

/**
* @fn BOOL CDirectInput::isRightButtonClicked()
* @brief  マウスの右ボタンのチェック
* @return		押している
*/
BOOL CDirectInput::isRightButtonClicked()
{
	return m_diMouseState2.rgbButtons[1] & 0x80;
}

/**
* @fn BOOL CDirectInput::isCenterButtonClicked()
* @brief  マウスの中ボタンのチェック
* @return		押している
*/
BOOL CDirectInput::isCenterButtonClicked()
{
	return m_diMouseState2.rgbButtons[2] & 0x80;
}