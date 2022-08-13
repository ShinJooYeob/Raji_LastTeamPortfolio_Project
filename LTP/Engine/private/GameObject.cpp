#include "..\Public\GameObject.h"
#include "GameInstance.h"
#include "Shader.h"

/*
1. Main Cam -> FocusTarget Settomg
2. Target Obj->Set_AttachCamPos()
3. Main Cam->Lock_CamLook(true, Look)
4. Main Cam->Set_TargetArmLength, Max, Min, Or Cur
*/

CGameObject::CGameObject(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: m_pDevice(pDevice)
	, m_pDeviceContext(pDeviceContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
}

CGameObject::CGameObject(const CGameObject & rhs)
	: m_pDevice(rhs.m_pDevice)
	, m_pDeviceContext(rhs.m_pDeviceContext),
	m_bIsClone(true), 
	m_bIsDead(false),
	m_szNameTag(nullptr)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
	m_eNowSceneNum = GetSingle(CGameInstance)->Get_TargetSceneNum();

	m_bIsOwnerDead = false;
}

HRESULT CGameObject::Initialize_Prototype(void * pArg)
{
	return S_OK;
}

HRESULT CGameObject::Initialize_Clone(void * pArg)
{
	return S_OK;
}

_int CGameObject::Update(_double fDeltaTime)
{
	m_bIsOnScreen = true;
	return _int();
}

_int CGameObject::LateUpdate(_double fDeltaTime)
{
	return _int();
}

_int CGameObject::Render()
{
	if (!m_pEngineShader)
	{
		m_pEngineShader = (CShader*)(Get_Component(L"Com_Shader"));
	}

	if (m_pEngineShader)
	{
		FAILED_CHECK(m_pEngineShader->Set_RawValue("g_vLimLight", &m_vLimLight, sizeof(_float4)));
		FAILED_CHECK(m_pEngineShader->Set_RawValue("g_fEmissive", &m_fEmissiveIntensive, sizeof(_float4)));

		_float DissolveValue = 0;
		FAILED_CHECK(m_pEngineShader->Set_RawValue("g_fDissolveValue", &(DissolveValue), sizeof(_float)));
	}



	return _int();
}

_int CGameObject::LateRender()
{
	return _int();
}

_float CGameObject::Take_Damage(CGameObject * pTargetObject, _float fDamageAmount, _fVector vDamageDir, _bool bKnockback, _float fKnockbackPower)
{
	return _float();
}


_fVector CGameObject::Get_BonePos(const char * pBoneName)
{
	return _fVector();
}

_fMatrix CGameObject::Get_BoneMatrix(const char * pBoneName)
{
	return _fMatrix();
}

_float CGameObject::Compute_RenderSortValue()
{
	CGameInstance* pGameInstance = GetSingle(CGameInstance);

	_Vector CamPos = pGameInstance->Get_TargetPostion_Vector(PLV_CAMERA);
	_Vector ObjPos = ((CTransform*)(this->Get_Component(TEXT("Com_Transform"))))->Get_MatrixState(CTransform::STATE_POS);

	m_fRenderSortValue = XMVectorGetX(XMVector3Length(CamPos - ObjPos));

	return m_fRenderSortValue;
}



void CGameObject::Set_LimLight_N_Emissive(_float4 vLimLight, _float4 fEmissive)
{
	m_vLimLight = vLimLight;
	m_fEmissiveIntensive = fEmissive;
}

CComponent* CGameObject::Get_Component(const _tchar * tagComponent)
{
	return Find_Components(tagComponent);
}


HRESULT CGameObject::Add_Component(_uint iScenenNum, const _tchar* tagPrototype, const _tchar* tagComponent, CComponent** ppOut, void* pArg)
{
	if (Find_Components(tagComponent) != nullptr)
	{
		__debugbreak();
		return E_FAIL;
	}

	CComponent* pCloneComponent = GetSingle(CGameInstance)->Clone_Component(iScenenNum, tagPrototype, pArg);

	if (pCloneComponent == nullptr)
	{
		pCloneComponent = GetSingle(CGameInstance)->Clone_Component(0, tagPrototype, pArg);
 		NULL_CHECK_BREAK(pCloneComponent);
	}

	(*ppOut) = pCloneComponent;
	Safe_AddRef(pCloneComponent);
	m_mapComponets.emplace(tagComponent, ppOut);


	return S_OK;
}

HRESULT CGameObject::Change_Component_by_NewAssign(_uint iScenenNum, const _tchar * tagPrototype, const _tchar * tagComponent, void * pArg)
{

	CComponent** ppSecValue = nullptr;

	auto iter = find_if(m_mapComponets.begin(), m_mapComponets.end(), CTagFinder(tagComponent));

	if (iter == m_mapComponets.end())
	{
		ppSecValue = nullptr;
		OutputDebugString(TEXT("Components Not Exist\n"));
		NULL_CHECK_RETURN(ppSecValue, E_FAIL);
	}

	ppSecValue = iter->second;


	CComponent* pCloneComponent = GetSingle(CGameInstance)->Clone_Component(iScenenNum, tagPrototype, pArg);
	if (pCloneComponent == nullptr)
	{
		pCloneComponent = GetSingle(CGameInstance)->Clone_Component(0, tagPrototype, pArg);
		NULL_CHECK_RETURN(pCloneComponent,E_FAIL);

	}



	// 기존 컴포넌트 삭제
	CComponent* pObjMemberPointer = *ppSecValue;
	Safe_Release(pObjMemberPointer);	
	pObjMemberPointer = *ppSecValue;
	Safe_Release(pObjMemberPointer);




	*ppSecValue = pCloneComponent;
	Safe_AddRef(pCloneComponent);

	return S_OK;
}

HRESULT CGameObject::Change_Component_by_Parameter(CComponent * NewComponent, const _tchar * tagComponent,  void * pArg)
{
	CComponent** ppSecValue = nullptr;

	NULL_CHECK_RETURN(NewComponent, E_FAIL);

	auto iter = find_if(m_mapComponets.begin(), m_mapComponets.end(), CTagFinder(tagComponent));

	if (iter == m_mapComponets.end())
	{
		ppSecValue = nullptr;
		OutputDebugString(TEXT("Components Not Exist\n"));
		NULL_CHECK_RETURN(ppSecValue, E_FAIL);
	}


	ppSecValue = iter->second;


	// 기존 컴포넌트 삭제
	CComponent* pObjMemberPointer = *ppSecValue;
	Safe_Release(pObjMemberPointer);
	pObjMemberPointer = *ppSecValue;
	Safe_Release(pObjMemberPointer);




	CComponent* pCloneComponent = NewComponent;

	*ppSecValue = pCloneComponent;
	Safe_AddRef(pCloneComponent);
	return S_OK;
}

_fVector CGameObject::Get_AttachCamPos()
{
	return XMVectorSetW(m_fAttachCamPos.XMVector(), 1.f);
}

_fVector CGameObject::Get_AttachCamPosOffset()
{
	return m_fAttachCamPos_Offset.XMVector();
}

_fVector CGameObject::Get_AttachCamLook()
{
	return m_fAttachCamLook.XMVector();
}

void CGameObject::Set_AttachCamPos(_float3 fAttachCamPos)
{
	m_fAttachCamPos = fAttachCamPos;
}

void CGameObject::Set_AttachCamPosOffset(_float3 fAttachCamPosOffset)
{
	m_fAttachCamPos_Offset = fAttachCamPosOffset;
}

void CGameObject::Update_AttachCamPos()
{
	
}

_float CGameObject::Add_NowHP(_float fHP)
{
	m_fHP += fHP;

	if (0.f > m_fHP)
	{
		m_fHP = 0.f;
	}

	return m_fHP;
}

CComponent * CGameObject::Find_Components(const _tchar * tagComponent)
{
	auto iter = find_if(m_mapComponets.begin(), m_mapComponets.end(), CTagFinder(tagComponent));

	if (iter == m_mapComponets.end())
		return nullptr;

	return *(iter->second);
}

//void CGameObject::Set_LoadData(OUTPUT_OBJECTINFO * pInfo)
//{
//	// OUTPUT_OBJECTINFO를 포인터 대입받음
//	m_pLoadInfoData = pInfo;
//}


void CGameObject::Free()
{
	CComponent* pForRelease;
	for (auto& ComponentPair : m_mapComponets)
	{
		pForRelease = *(ComponentPair.second);
		pForRelease->Set_IsOwnerDead(true);
		Safe_Release(pForRelease);
	}


	m_mapComponets.clear();

	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);

}
