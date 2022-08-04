#pragma once

#include "Scene.h"

BEGIN(Client)

class CScene_Laboratory_Jino final : public CScene
{
private:
	explicit CScene_Laboratory_Jino(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	virtual ~CScene_Laboratory_Jino() = default;

public:
	virtual HRESULT		Initialize();
	virtual _int		Update(_double fDeltaTime)override;
	virtual _int		LateUpdate(_double fDeltaTime)override;
	virtual _int		Render()override;
	virtual _int		LateRender()override;


private:
	HRESULT				Ready_Light();
	HRESULT				Ready_Layer_MainCamera(const _tchar* pLayerTag);
	HRESULT				Ready_Layer_Player(const _tchar * pLayerTag);
	HRESULT				Ready_Layer_Objects(const _tchar * pLayerTag);


	HRESULT				Ready_TriggerObject(const _tchar * szTriggerDataName, SCENEID eSceneID, const _tchar * pLayerTag);
	HRESULT				Ready_PostPorcessing();


private:
	class CCamera_Main*				m_pMainCam = nullptr;
	CUtilityMgr*					m_pUtilMgr = nullptr;
	CTransform*						m_pPlayerTransform = nullptr;


public:
	static CScene_Laboratory_Jino*	Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	virtual void					Free() override;
};

END