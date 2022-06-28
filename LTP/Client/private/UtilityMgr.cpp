#include "stdafx.h"
#include "..\public\UtilityMgr.h"
#include "MainApp.h"

IMPLEMENT_SINGLETON(CUtilityMgr);

CUtilityMgr::CUtilityMgr()
{
}

HRESULT CUtilityMgr::Initialize_UtilityMgr(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, CMainApp * pMainApp)
{
	if (nullptr == pMainApp || nullptr == pDevice || nullptr == pDeviceContext)
		return E_FAIL;

	m_pDevice = pDevice;
	m_pDeviceContext = pDeviceContext;
	m_pMainApp = pMainApp;


	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);

	ZeroMemory(m_StartTime, sizeof(_double)*E_DEBUGTIMER::DEBUGTIMER_END);

	return S_OK;
}

_float CUtilityMgr::RandomFloat(_float Min, _float Max)
{
	if (Min >= Max) // bad input
	{
		_float temp = Min;
		Min = Max;
		Max = temp;
	}


	_uint RandNum = rand();
	_uint iDigit = CountDigit(RandNum);
	while (!iDigit)
	{
		RandNum = rand();
		iDigit = CountDigit(RandNum);
	}
	_uint Power = _uint(pow(10, iDigit - 1));
	//_float f = (RandNum) / _float(Power);
	_float f = (RandNum % Power) / _float(Power);
	return (f * (Max - Min)) + Min;
}

_float3 CUtilityMgr::RandomFloat3(_float Min, _float Max)
{
	if (Min >= Max) // bad input
	{
		_float temp = Min;
		Min = Max;
		Max = temp;
	}
	return _float3(RandomFloat(Min, Max), RandomFloat(Min, Max), RandomFloat(Min, Max));
}

_float3 CUtilityMgr::RandomFloat3(_float3 Min, _float3 Max)
{
	if (Min.x >= Max.x) // bad input
	{
		_float temp = Min.x;
		Min.x = Max.x;
		Max.x = temp;
	}
	if (Min.y >= Max.y) // bad input
	{
		_float temp = Min.y;
		Min.y = Max.y;
		Max.y = temp;
	}

	if (Min.z >= Max.z) // bad input
	{
		_float temp = Min.z;
		Min.z = Max.z;
		Max.z = temp;
	}

	return _float3(RandomFloat(Min.x, Max.x), RandomFloat(Min.y, Max.y), RandomFloat(Min.z, Max.z));
}

void CUtilityMgr::SlowMotionStart(_float fTargetTime, _float TargetSpeed)
{
	NULL_CHECK_BREAK(m_pMainApp);
	m_pMainApp->SlowMotionStart(fTargetTime, TargetSpeed);
}

void CUtilityMgr::Start_DebugTimer(E_DEBUGTIMER type)
{
	m_StartTime[type] = clock();
	// 실행코드를 실행한다.
}

void CUtilityMgr::End_DebugTimer(E_DEBUGTIMER type, wstring debugLog)
{
	if (m_StartTime[type] == 0)
	{
#ifdef _DEBUG
		wstring log = L"__BEFORE_CALL_Start_DebugTimer_FUNC__\n";
		OutputDebugStringW(log.c_str());
		return;
#endif
	}

#ifdef _DEBUG
	{
		_double EndTimer = ((int)clock() - m_StartTime[type]) / (CLOCKS_PER_SEC / 1000);
		wstring debug = L"__ENDTIME: " + to_wstring(EndTimer) + L"ms__"
			+ debugLog + L"___\n"; ;

		OutputDebugStringW(debug.c_str());

		m_StartTime[type] = 0;
	}
#endif

}





HRESULT CUtilityMgr::Clear_RenderGroup_forSceneChange()
{
	NULL_CHECK_RETURN(m_pRenderer, E_FAIL);
	g_pGameInstance->Clear_CollisionGroup();
	FAILED_CHECK(m_pRenderer->Clear_RenderGroup_forSceneChaging());
	return  S_OK;
}

void CUtilityMgr::Set_Renderer(CRenderer * pRenderer)
{
	m_pRenderer = pRenderer;
	Safe_AddRef(m_pRenderer);
}

_uint CUtilityMgr::CountDigit(_uint iNum)
{
	string tmp;
	tmp = to_string(iNum);
	return _uint(tmp.size());
}


void CUtilityMgr::Free()
{
	Safe_Release(m_pRenderer);

	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);
}
