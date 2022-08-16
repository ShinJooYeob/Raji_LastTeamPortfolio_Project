#include "stdafx.h"
#include "..\public\PathArrow.h"
#include "Player.h"
#include "Golu.h"

CPathArrow::CPathArrow(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CPathArrow::CPathArrow(const CPathArrow & rhs)
	: CGameObject(rhs)
{
}

HRESULT CPathArrow::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CPathArrow::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg)); 

	FAILED_CHECK(SetUp_Components());
	FAILED_CHECK(SetUp_Etc());

	return S_OK;
}

_int CPathArrow::Update(_double fDeltaTime)
{
	if (false == m_pDissolveCom->Get_IsFadeIn() && 1.f <= m_pDissolveCom->Get_DissolvingRate())
		return 0;

	if (__super::Update(fDeltaTime) < 0)	return -1;

	
	_Vector vPlayerPos = m_pPlayerTransform->Get_MatrixState(CTransform::TransformState::STATE_POS);
	_Vector vGoluPos = m_pGoluTransform->Get_MatrixState(CTransform::TransformState::STATE_POS);
	
	_Vector vLookDir = XMVector3Normalize(vGoluPos - vPlayerPos);
	vLookDir = XMVectorSetY(vLookDir, 0.f);

	vPlayerPos = XMVectorSetY(vPlayerPos, XMVectorGetY(vPlayerPos) + 0.5f);

	if (m_fMin_Dist >= m_fCur_Dist && 0 > m_fMovDir)
	{
		m_fMovDir *= -1.f;
	}
	else if (m_fMax_Dist <= m_fCur_Dist && 0 < m_fMovDir)
	{
		m_fMovDir *= -1.f;
	}
	m_fCur_Dist += m_fMovDir;
	
	m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, vPlayerPos + vLookDir * m_fCur_Dist);
	m_pTransformCom->LookDir(vLookDir);
	FAILED_CHECK(m_pDissolveCom->Update_Dissolving(fDeltaTime));
	return _int();
}

_int CPathArrow::LateUpdate(_double fDeltaTime)
{
	if (false == m_pDissolveCom->Get_IsFadeIn() && 1.f <= m_pDissolveCom->Get_DissolvingRate())
		return 0;

	if (__super::LateUpdate(fDeltaTime) < 0)return -1;

	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	return _int();
}

_int CPathArrow::Render()
{
	if (__super::Render() < 0)		return -1;

	NULL_CHECK_RETURN(m_pModel, E_FAIL);

	CGameInstance* pInstance = GetSingle(CGameInstance);

	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));
	FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));

	FAILED_CHECK(m_pDissolveCom->Render(10));

	return _int();
}

_int CPathArrow::LateRender()
{
	return _int();
}

void CPathArrow::Set_Appear(_bool bAppear)
{
	if (true == bAppear)
	{
		m_pDissolveCom->Set_DissolveOn(true, 1.f);		// Appear
	}
	else if (false == bAppear)
	{
		m_pDissolveCom->Set_DissolveOn(false, 1.f);	// Disappear
	}
}

HRESULT CPathArrow::SetUp_Components()
{
	CTransform::TRANSFORMDESC tDesc = {};
	tDesc.fMovePerSec = 5;
	tDesc.fRotationPerSec = XMConvertToRadians(60);
	tDesc.fScalingPerSec = 1;
	tDesc.vPivot = _float3(0, 0, 0);
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VNAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));
	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_ArrowDir), TAG_COM(Com_Model), (CComponent**)&m_pModel));


	CDissolve::DISSOLVEDESC	tDissolveDesc;
	tDissolveDesc.eDissolveModelType = CDissolve::DISSOLVE_NONANIM;
	tDissolveDesc.pModel = m_pModel;
	tDissolveDesc.pShader = m_pShaderCom;
	tDissolveDesc.RampTextureIndex = 1;
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Dissolve), TAG_COM(Com_Dissolve), (CComponent**)&m_pDissolveCom, &tDissolveDesc));

	return S_OK;
}

HRESULT CPathArrow::SetUp_Etc()
{
	m_pTransformCom->Scaled_All(_float3(1.5f));
	Set_LimLight_N_Emissive(_float4(1.f), _float4(1.f));

	CGameObject* pPlayer = (CPlayer*)(g_pGameInstance->Get_GameObject_By_LayerIndex(SCENE_STAGE3, TAG_LAY(Layer_Player)));
	m_pPlayerTransform = (CTransform*)pPlayer->Get_Component(TAG_COM(Com_Transform));

	CGameObject* pGolu = (CGolu*)(g_pGameInstance->Get_GameObject_By_LayerIndex(SCENE_STAGE3, TAG_LAY(Layer_NPC)));
	m_pGoluTransform = (CTransform*)pGolu->Get_Component(TAG_COM(Com_Transform));

	m_pDissolveCom->Set_DissolveOn(false, 0.f);

	m_fMovDir = 0.02f;
	return S_OK;
}

CPathArrow * CPathArrow::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CPathArrow*	pInstance = NEW CPathArrow(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CPathArrow");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CPathArrow::Clone(void * pArg)
{
	CPathArrow*	pInstance = NEW CPathArrow(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CPathArrow");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPathArrow::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pModel);
	Safe_Release(m_pDissolveCom);
}
