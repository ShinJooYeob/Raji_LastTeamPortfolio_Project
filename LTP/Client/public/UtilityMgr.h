#pragma once

#include "Base.h"
BEGIN(Client)

class CUtilityMgr final :public CBase
{
	DECLARE_SINGLETON(CUtilityMgr)
public:
	enum E_DEBUGTIMER
	{
		DEBUGTIMER_1,
		DEBUGTIMER_2,
		DEBUGTIMER_3,
		DEBUGTIMER_END,
	};
private:
	explicit CUtilityMgr();
	virtual ~CUtilityMgr() = default;

public:
	HRESULT Initialize_UtilityMgr(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, class CMainApp*	pMainApp);


	/*For Random*/
	_float	RandomFloat(_float Min, _float Max);
	_float3	RandomFloat3(_float Min, _float Max);
	_float3	RandomFloat3(_float3 Min, _float3 Max);


	/*For SlowMotion*/
	void SlowMotionStart(_float fTargetTime = 0.5f, _float TargetSpeed = 0.0f, _float TargetCurveRate = 0.5f);
	_float Get_SlowMotionSpeed();


	/*For Debug_Timer*/
	void Start_DebugTimer(E_DEBUGTIMER type);
	void End_DebugTimer(E_DEBUGTIMER type, wstring debugLog = L"");


	/*For DebugMode*/
	CRenderer* Get_Renderer() { return m_pRenderer; }


	/*For Particle*/
	HRESULT Create_TextureInstance(_uint eSceneID, INSTPARTICLEDESC& tParticleDesc);
	HRESULT Create_MeshInstance(_uint eSceneID, INSTMESHDESC& tParticleDesc);



public:
	HRESULT Clear_RenderGroup_forSceneChange();
	void Set_Renderer(CRenderer* pRenderer);

private:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;
	class CMainApp*			m_pMainApp = nullptr;
	CRenderer*				m_pRenderer = nullptr;

private:
	clock_t m_StartTime[E_DEBUGTIMER::DEBUGTIMER_END];


private:
	_uint CountDigit(_uint iNum);


public:
	virtual void Free()override;

};

END