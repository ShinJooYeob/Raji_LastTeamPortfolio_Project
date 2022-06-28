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
	void SlowMotionStart(_float fTargetTime = 0.4f, _float TargetSpeed = 0.0f);

	/*For Debug_Timer*/
	// ½ÇÇà¼Óµµ Å×½ºÆ® ÇÔ¼ö
	void Start_DebugTimer(E_DEBUGTIMER type);
	void End_DebugTimer(E_DEBUGTIMER type, wstring debugLog = L"");

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