#pragma once
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CScene abstract :public CBase
{
protected:
	explicit CScene(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CScene() = default;

public:
	virtual HRESULT Initialize();
	virtual _int Update(_double fDeltaTime);
	virtual _int LateUpdate(_double fDeltaTime);
	virtual _int Render();
	virtual _int LateRender();

	virtual _int Change_to_NextScene() { return 0; };
	_bool	Get_bIsNeedToSceneChange() { return m_bIsNeedToSceneChange; }
	HRESULT Set_SceneChanging(_uint _INextScene);

protected:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;

	_bool			m_bIsNeedToSceneChange = false;
	_uint			m_eNextScene;
public:
	virtual void Free()override;
};

END