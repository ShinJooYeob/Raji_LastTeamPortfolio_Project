#pragma once

#include "Scene.h"

BEGIN(Client)
class CScene_Loading final : public CScene
{
public:
	explicit CScene_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CScene_Loading() = default;


public:
	virtual HRESULT Initialize(SCENEID eSceneID);
	virtual _int Update(_double fDeltaTime)override;
	virtual _int LateUpdate(_double fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

	HRESULT Ready_Layer_LoadingUI(const _tchar* pLayerTag);




private:
	SCENEID m_eNextSceneIndex = SCENEID::SCENE_END;
	class CLoader*		m_pLoader;

	_double				m_FadePassedTime = 0;


public:
	static CScene_Loading* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, SCENEID eTargetSceneID);
	virtual void Free() override;
};

END
