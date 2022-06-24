#include "..\Public\Camera.h"
#include "GameInstance.h"






CCamera::CCamera(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CCamera::CCamera(const CCamera & rhs)
	:CGameObject(rhs)
{
	m_CameraDesc = rhs.m_CameraDesc;
}

HRESULT CCamera::Initialize_Prototype(void * pArg)
{
	if (pArg != nullptr)
		memcpy(&m_CameraDesc, pArg, sizeof(CAMERADESC));

	return S_OK;
}

HRESULT CCamera::Initialize_Clone(void * pArg)
{
	if (pArg != nullptr)
		memcpy(&m_CameraDesc, pArg, sizeof(CAMERADESC));

	if (m_pDevice == nullptr || m_pDeviceContext == nullptr)
		return E_FAIL;

	m_pTransform = CTransform::Create(m_pDevice, m_pDeviceContext);

	if (nullptr == m_pTransform)
		return E_FAIL;

	m_pTransform->Set_TransformDesc(m_CameraDesc.TransformDesc);


	_Vector vRight, vUp, vLook;

	vLook = XMVector3Normalize((m_CameraDesc.vAt).XMVector() - (m_CameraDesc.vEye).XMVector());
	vRight = XMVector3Normalize((m_CameraDesc.vAxisY.Get_Cross(vLook)));
	vUp = XMVector3Normalize(XMVector3Cross(vLook, vRight));


	m_pTransform->Set_MatrixState(CTransform::STATE_RIGHT, vRight);
	m_pTransform->Set_MatrixState(CTransform::STATE_UP, vUp);
	m_pTransform->Set_MatrixState(CTransform::STATE_LOOK, vLook);
	m_pTransform->Set_MatrixState(CTransform::STATE_POS, m_CameraDesc.vEye);


	return S_OK;
}

_int CCamera::Update(_double fDeltaTime, _bool bOrtho)
{
	if (m_pDevice == nullptr || m_pDeviceContext == nullptr)
		return -1;

	Set_ViewMatrix();
	Set_ProjectMatrix();
	//if (FAILED(m_pGraphicDevice->SetTransform(D3DTS_VIEW, &(m_pTransform->Get_InverseWorldMatrix()))))
	//	return -1;

	//_float4x4 matProj;
	//

	//if (FAILED(Set_ProjectMatrix(m_bIsOrtho)))
	//	return E_FAIL;


	return _int();
}

_int CCamera::LateUpdate(_double fDeltaTime)
{
	return _int();
}

_int CCamera::Render()
{
	return _int();
}

_int CCamera::LateRender()
{


	return _int();
}

HRESULT CCamera::Set_ViewMatrix()
{
	CGameInstance* pIsntance =  GetSingle(CGameInstance);

	pIsntance->Set_Transform(PLM_VIEW, m_pTransform->Get_InverseWorldMatrix());

	pIsntance->Set_TargetPostion(PLV_CAMERA, *((_float4*)(m_pTransform->Get_WorldFloat4x4().m[3])));

	pIsntance->Set_TargetPostion(PLV_CAMLOOK, *((_float4*)(m_pTransform->Get_WorldFloat4x4().m[2])));

	return S_OK;
}

HRESULT CCamera::Set_ProjectMatrix(_bool bIsOrtho)
{
	CGameInstance* pIsntance = GetSingle(CGameInstance);

	if (bIsOrtho)
		pIsntance->Set_Transform(PLM_PROJ, XMMatrixOrthographicLH(_float(m_CameraDesc.iWinCX), _float(m_CameraDesc.iWinCY), 0.f, 1.f));

	else
		pIsntance->Set_Transform(PLM_PROJ, XMMatrixPerspectiveFovLH(m_CameraDesc.fFovy,
		(_float)m_CameraDesc.fAspect, m_CameraDesc.fNear, m_CameraDesc.fFar));



	return S_OK;
}


void CCamera::Free()
{
	__super::Free();

	Safe_Release(m_pTransform);
}
