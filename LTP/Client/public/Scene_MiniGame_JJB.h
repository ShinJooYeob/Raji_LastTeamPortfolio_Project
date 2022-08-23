#pragma once
#include "Scene.h"
#include "Taiko_Monster.h"

BEGIN(Client)

class CScene_MiniGame_JJB final : public CScene
{
private:
	explicit CScene_MiniGame_JJB(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	virtual ~CScene_MiniGame_JJB() = default;


public:
	virtual HRESULT		Initialize();
	virtual _int		Update(_double fDeltaTime)override;
	virtual _int		LateUpdate(_double fDeltaTime)override;
	virtual _int		Render()override;
	virtual _int		LateRender()override;


private:
	HRESULT				Ready_Light();
	HRESULT				Ready_Layer_MainCamera(const _tchar* pLayerTag);
	HRESULT				Ready_Layer_Objects(const _tchar * pLayerTag);
	HRESULT				Ready_Layer_Player(const _tchar* pLayerTag);
	HRESULT				Ready_Layer_Monster(const _tchar* pLayerTag);
	HRESULT				Ready_Layer_GoalCollider(const _tchar* pLayerTag);

	HRESULT				Ready_Layer_UI(const _tchar* pLayerTag);



	HRESULT				Ready_PostPorcessing();

private:
	list<CTaiko_Monster*>		m_NoteSmallRedMonsterList;
	list<CTaiko_Monster*>		m_NoteBicRedMonsterList;
	list<CTaiko_Monster*>		m_NoteSmallBlueMonsterList;
	list<CTaiko_Monster*>		m_NoteBicBlueMonsterList;

	_float						m_AccumulateTime = 0.f;



private:
	class CCamera_Main*				m_pMainCam = nullptr;
	CUtilityMgr*					m_pUtilMgr = nullptr;

public:
	static CScene_MiniGame_JJB*	Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	virtual void				Free() override;

};
END
