#pragma once

#include "Scene.h"

BEGIN(Client)
class CScene_Loading final : public CScene
{
public:
	enum LOADINGKINDSID
	{
		LOADINGKINDS_NORMAL, LOADINGKINDS_NORMAL_TO_MINI, LOADINGKINDS_MINI_TO_NORMAL, LOADINGKINDS_END
	};

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



	static _uint		m_iLoadingKinds;

private:
	SCENEID m_eNextSceneIndex = SCENEID::SCENE_END;
	SCENEID m_eOldSceneIndex = SCENEID::SCENE_END;
	class CLoader*		m_pLoader;


	_uint				m_iMiniGameIndex = 0;



public:
	static CScene_Loading* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, SCENEID eTargetSceneID);
	virtual void Free() override;
};

END
