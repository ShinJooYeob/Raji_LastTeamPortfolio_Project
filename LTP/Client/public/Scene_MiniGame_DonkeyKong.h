#pragma once
#include "Scene.h"

BEGIN(Client)

class Scene_MiniGame_DonkeyKong final : public CScene
{
private:
	explicit Scene_MiniGame_DonkeyKong(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	virtual ~Scene_MiniGame_DonkeyKong() = default;

public:
	virtual HRESULT Initialize();
	virtual _int Update(_double fDeltaTime)override;
	virtual _int LateUpdate(_double fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

	virtual _int Change_to_NextScene() override;



private:
	HRESULT Ready_Light();
	HRESULT Ready_Layer_MainCamera(const _tchar* pLayerTag);
	HRESULT	Ready_Layer_MiniGame_KongRaji(const _tchar* pLayerTag);
	HRESULT Ready_Layer_SkyBox(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Terrain(const _tchar* pLayerTag);

	HRESULT	Ready_MapData(const _tchar* szMapDataFileName, SCENEID eSceneID, const _tchar* pLayerTag);
	HRESULT	Ready_DonkeyKong_Dynamic(const _tchar* pLayerTag);


	HRESULT Ready_PostPorcessing();

	HRESULT Ready_MapParticle();

private:
	class CCamera_Main*				m_pMainCam = nullptr;
	CUtilityMgr*					m_pUtilMgr = nullptr;
	CTransform*						m_pJalsuraTransform = nullptr;

public:
	static Scene_MiniGame_DonkeyKong* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	virtual void Free() override;
};

END