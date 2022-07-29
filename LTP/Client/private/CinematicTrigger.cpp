#include "stdafx.h"
#include "..\public\CinematicTrigger.h"
#include "Player.h"
#include "Camera_Main.h"

CCinematicTrigger::CCinematicTrigger(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CTriggerObject(pDevice, pDeviceContext)
{
}

CCinematicTrigger::CCinematicTrigger(const CCinematicTrigger & rhs)
	: CTriggerObject(rhs)
{
}

HRESULT CCinematicTrigger::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CCinematicTrigger::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(nullptr));

	FAILED_CHECK(SetUp_Components());

	FAILED_CHECK(SetUp_EtcInfo());


	return S_OK;
}

HRESULT CCinematicTrigger::After_Initialize()
{
	SCENEID		eSceneID = static_cast<SCENEID>((_int)m_fValueMat._11);
	_tchar szPath[MAX_PATH] = L"";

	m_tCamActionDesc.vecCamPos.clear();
	m_tCamActionDesc.vecLookAt.clear();

	switch (eSceneID)
	{
	case SCENEID::SCENE_STAGE1:
	{
		swprintf_s(szPath, L"Stage1_%d.dat", (_int)m_fValueMat._12);
	}
	break;
	}


	_tchar szFullPath[MAX_PATH] = L"../bin/Resources/Data/CameraAction/";
	lstrcat(szFullPath, szPath);
	HANDLE hFile = ::CreateFileW(szFullPath, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, NULL);


	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD    dwByte = 0;

	CGameInstance* pInstance = g_pGameInstance;

	_uint iCount = 0;
	ReadFile(hFile, &(iCount), sizeof(_uint), &dwByte, nullptr);

	CAMACTDESC tDesc;

	for (_uint i = 0; i < iCount; i++)
	{
		ReadFile(hFile, &(tDesc.fDuration), sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, &(tDesc.vPosition), sizeof(_float3), &dwByte, nullptr);

		m_tCamActionDesc.vecCamPos.push_back(tDesc);

	}


	iCount = 0;
	ReadFile(hFile, &(iCount), sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < iCount; i++)
	{
		ReadFile(hFile, &(tDesc.fDuration), sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, &(tDesc.vPosition), sizeof(_float3), &dwByte, nullptr);

		m_tCamActionDesc.vecLookAt.push_back(tDesc);
	}


	CloseHandle(hFile);
	
	return S_OK;
}

_int CCinematicTrigger::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0) return -1;

	if (false == m_bActive)
	{
		FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_NPC, this, m_pColliderCom));
	}

	return _int();
}

_int CCinematicTrigger::LateUpdate(_double fDeltaTimer)
{
	if (__super::LateUpdate(fDeltaTimer) < 0) return -1;

#ifdef _DEBUG
	FAILED_CHECK(GetSingle(CUtilityMgr)->Get_Renderer()->Add_DebugGroup(m_pColliderCom));
#endif // _DEBUG

	return _int();
}

_int CCinematicTrigger::Render()
{
	return _int();
}

_int CCinematicTrigger::LateRender()
{
	return _int();
}

_int CCinematicTrigger::Active_Trigger(CGameObject * pTarget, _double fDeltaTime)
{
	return _int();
}

_int CCinematicTrigger::DeActive_Trigger(CGameObject * pTarget, _double fDeltaTime)
{
	return _int();
}

CCinematicTrigger::EParkourTriggerType CCinematicTrigger::Get_ParkourTriggerType()
{
	return EParkourTriggerType::TRIGGER_CINEMATIC;
}

void CCinematicTrigger::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	if (CollisionTypeID::CollisionType_Player == eConflictedObjCollisionType)
	{
		GetSingle(CUtilityMgr)->Get_MainCamera()->CamActionStart(m_tCamActionDesc);
		m_bActive = true;
		Set_IsDead();
	}


}

HRESULT CCinematicTrigger::SetUp_Components()
{
	return S_OK;
}

HRESULT CCinematicTrigger::SetUp_EtcInfo()
{
	return S_OK;
}

CCinematicTrigger * CCinematicTrigger::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CCinematicTrigger*	pInstance = NEW CCinematicTrigger(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CCinematicTrigger");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCinematicTrigger::Clone(void * pArg)
{
	CCinematicTrigger*	pInstance = NEW CCinematicTrigger(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CCinematicTrigger");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCinematicTrigger::Free()
{
	__super::Free();
}
