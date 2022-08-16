#pragma once

#include "Scene.h"
#include "NormalMonkey.h"

BEGIN(Client)
class CMiniGame_Jino_Player;
class CScene_MiniGame_Jino final : public CScene
{
public:
	enum	ESPAWN_PHASE { PHASE_1, PHASE_2, PHASE_3, PHASE_END };

private:
	explicit CScene_MiniGame_Jino(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	virtual ~CScene_MiniGame_Jino() = default;

public:
	virtual HRESULT		Initialize();
	virtual _int		Update(_double fDeltaTime)override;
	virtual _int		LateUpdate(_double fDeltaTime)override;
	virtual _int		Render()override;
	virtual _int		LateRender()override;

public:
	void				Spawn_Monster();
	void				GameClear();
	void				GameOver();

public: // Object Pool
	void				Push_NormalMonkey(CNormalMonkey* pNormalMonkey);
	void				Pop_NormalMonkey(CMiniGame_Jino_Monster::EMONSTER_TYPE eMonkeyType, _float3 vSpawnPos);

private:
	HRESULT				Ready_Light();
	HRESULT				Ready_Layer_MainCamera(const _tchar* pLayerTag);
	HRESULT				Ready_Layer_Player(const _tchar * pLayerTag);
	HRESULT				Ready_Layer_Objects(const _tchar * pLayerTag);
	HRESULT				Ready_MapData(const _tchar* szMapDataFileName, SCENEID eSceneID, const _tchar* pLayerTag);

	HRESULT				Ready_TriggerObject(const _tchar * szTriggerDataName, SCENEID eSceneID, const _tchar * pLayerTag);
	HRESULT				Ready_PostPorcessing();

	HRESULT				Ready_ObjectPool_NormalMonkey();

private:
	_bool							m_bGameClear = false;
	_float							m_fDelayTime_Spawn = 0.f;
	ESPAWN_PHASE					m_eSpawnPhase = PHASE_END;

private:
	list<CNormalMonkey*>			m_ObjectPool_NormalMonkey;

private:
	CTransform*						m_pPlayerTransform = nullptr;
	CMiniGame_Jino_Player*			m_pPlayer = nullptr;

private:
	class CCamera_Main*				m_pMainCam = nullptr;
	CUtilityMgr*					m_pUtilMgr = nullptr;

public:
	static CScene_MiniGame_Jino*	Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	virtual void					Free() override;
};

END