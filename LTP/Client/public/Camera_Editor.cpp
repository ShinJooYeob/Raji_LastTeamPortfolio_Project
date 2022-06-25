#include "stdafx.h"
#include "..\Public\Camera_Editor.h"



CCamera_Editor::CCamera_Editor(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CCamera(pDevice,pDeviceContext)
{
}

CCamera_Editor::CCamera_Editor(const CCamera_Editor & rhs)
	:CCamera(rhs)
{
}
//1
HRESULT CCamera_Editor::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CCamera_Editor::Initialize_Clone(void * pArg)
{

	FAILED_CHECK(__super::Initialize_Clone(pArg));


	if (FAILED(SetUp_Components()))
		return E_FAIL;

	return S_OK;
}

_int CCamera_Editor::Update(_double fDeltaTime)
{
	__super::Update(fDeltaTime);

	if (g_pGameInstance->Get_DIKeyState(DIK_W) & DIS_Press)
	{
		m_pTransform->Move_Forward(fDeltaTime);
	}
	if (g_pGameInstance->Get_DIKeyState(DIK_A) & DIS_Press)
	{
		m_pTransform->Move_Left(fDeltaTime);
	}
	if (g_pGameInstance->Get_DIKeyState(DIK_S) & DIS_Press)
	{
		m_pTransform->Move_Backward(fDeltaTime);
	}
	if (g_pGameInstance->Get_DIKeyState(DIK_D) & DIS_Press)
	{
		m_pTransform->Move_Right(fDeltaTime);
	}


	FAILED_CHECK(Update_CamAction(fDeltaTime));

	return _int();
}

_int CCamera_Editor::LateUpdate(_double fDeltaTime)
{	

	__super::LateUpdate(fDeltaTime);


	return _int();
}

_int CCamera_Editor::Render()
{



	return _int();
}

_int CCamera_Editor::LateRender()
{


	return _int();
}

_bool CCamera_Editor::CamActionStart(CAMERAACTION Act)
{
	m_bCamActionStart = true;
	m_tCamAction = Act;
	m_ActionPassedTime = 0;
	m_tCamAction.TotalTime = 0;

	_float TotalTime = 0;
	for (auto& iter : m_tCamAction.vecCamPos)
		TotalTime += iter.fDuration;
	if (m_tCamAction.TotalTime < TotalTime)m_tCamAction.TotalTime = TotalTime;
	TotalTime = 0;
	for (auto& iter : m_tCamAction.vecLookAt)
		TotalTime += iter.fDuration;
	if (m_tCamAction.TotalTime < TotalTime)m_tCamAction.TotalTime = TotalTime;


	m_ReturnPos = m_pTransform->Get_MatrixState_Float3(CTransform::STATE_POS);
	m_ReturnLook = m_ReturnPos.XMVector() + m_pTransform->Get_MatrixState(CTransform::STATE_LOOK);
	m_iNowPosIndex = 0;
	m_iNowLookIndex = 0;



	return _bool();
}


HRESULT CCamera_Editor::SetUp_Components()
{

	//if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_ComRenderer)))
	//	return E_FAIL;

	//if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Texture_Blank), TAG_COM(Com_Texture), (CComponent**)&m_ComTexture)))
	//	return E_FAIL;

	//if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Rect), TAG_COM(Com_VIBuffer), (CComponent**)&m_ComVIBuffer)))
	//	return E_FAIL;



	return S_OK;
}

//#define EASINGTYPE TYPE_QuadInOut
#define EASINGTYPE TYPE_SinInOut

HRESULT CCamera_Editor::Update_CamAction(_double fDeltaTime)
{

	if (!m_bCamActionStart)return S_FALSE;

	_Matrix NewCamMat = XMMatrixIdentity();
	CGameInstance* pInstance = g_pGameInstance;

	

	m_ActionPassedTime += fDeltaTime;

	if (m_ActionPassedTime > m_tCamAction.TotalTime)
		//마지막 프레임까지 돌았다면 원래 위치로 되돌려라
	{

		_float3 EasedPos;
		_float3 EasedLookAt;


		_float3 Old = m_tCamAction.vecCamPos[m_tCamAction.vecCamPos.size()-1].vPosition;
		_float3 Now = m_ReturnPos;

		EasedPos = pInstance->Easing_Vector(EASINGTYPE, Old, Now, _float(m_ActionPassedTime - m_tCamAction.TotalTime) , 0.5f);

		Old = m_tCamAction.vecLookAt[m_tCamAction.vecLookAt.size() - 1].vPosition;
		Now = m_ReturnLook;

		EasedLookAt = pInstance->Easing_Vector(EASINGTYPE, Old, Now, _float(m_ActionPassedTime - m_tCamAction.TotalTime), 0.5f);

		if (m_ActionPassedTime > m_tCamAction.TotalTime + 0.5f)
		{
			EasedPos = m_ReturnPos;
			EasedLookAt = m_ReturnLook;
			m_ActionPassedTime = 0;
			m_bCamActionStart = false;
		}

		m_pTransform->Set_MatrixState(CTransform::STATE_POS, EasedPos);
		m_pTransform->LookAt(EasedLookAt.XMVector());
	}
	else
	//마지막 프레임까지 돌지 않았다면 보간하면서 라업룩포를 구해라
	{
		_float3 EasedPos;
		_float3 EasedLookAt;


		_float3 Old = m_ReturnPos;
		_float3 Now = m_tCamAction.vecCamPos[0].vPosition;
		_float	Rate = 0;
		_float	TargetTime = 0;
		_uint	iTargetIndex = _uint(m_tCamAction.vecCamPos.size() - 1);
		
		for (_uint i =0 ; i< m_tCamAction.vecCamPos.size(); i++)
		{
			if (TargetTime + m_tCamAction.vecCamPos[i].fDuration > m_ActionPassedTime)
			{
				iTargetIndex = i;
				break;
			}

			TargetTime += m_tCamAction.vecCamPos[i].fDuration;
			Old = m_tCamAction.vecCamPos[i].vPosition;
		}

		Now = m_tCamAction.vecCamPos[iTargetIndex].vPosition;
		_float TempPassedTime = min(_float(m_ActionPassedTime - TargetTime), m_tCamAction.vecCamPos[iTargetIndex].fDuration);

		EasedPos = pInstance->Easing_Vector(EASINGTYPE, Old, Now, TempPassedTime, m_tCamAction.vecCamPos[iTargetIndex].fDuration);


		//////////////////////////////////////////////////////////////////////////
		Old = m_ReturnLook;
		Now = m_tCamAction.vecLookAt[0].vPosition;
		Rate = 0;
		TargetTime = 0;
		iTargetIndex = _uint(m_tCamAction.vecLookAt.size() - 1);
		TempPassedTime = 0;

		for (_uint i = 0; i < m_tCamAction.vecLookAt.size(); i++)
		{
			if (TargetTime + m_tCamAction.vecLookAt[i].fDuration > m_ActionPassedTime)
			{
				iTargetIndex = i;
				break;
			}

			TargetTime += m_tCamAction.vecLookAt[i].fDuration;
			Old = m_tCamAction.vecLookAt[i].vPosition;
		}

		Now = m_tCamAction.vecLookAt[iTargetIndex].vPosition;
		TempPassedTime = min(_float(m_ActionPassedTime - TargetTime), m_tCamAction.vecLookAt[iTargetIndex].fDuration);

		EasedLookAt = pInstance->Easing_Vector(EASINGTYPE, Old, Now, TempPassedTime, m_tCamAction.vecLookAt[iTargetIndex].fDuration);



		m_pTransform->Set_MatrixState(CTransform::STATE_POS, EasedPos);
		m_pTransform->LookAt(EasedLookAt.XMVector());

	}


	return S_OK;
}



CCamera_Editor * CCamera_Editor::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CCamera_Editor* pInstance = new CCamera_Editor(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg))) {
		MSGBOX("Failed to Create CCamera_Editor");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject * CCamera_Editor::Clone(void * pArg)
{
	CCamera_Editor* pInstance = new CCamera_Editor(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg))) {
		MSGBOX("Failed to Create CCamera_Editor");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CCamera_Editor::Free()
{
	__super::Free();


}
