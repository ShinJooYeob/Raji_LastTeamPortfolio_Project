#include "stdafx.h"
#include "..\public\Monster_Texture_Bullet.h"
#include "Monster_Bullet_Universal.h"

const _tchar* m_pMonster_Texture_BulletTag[CMonster_Texture_Bullet::MONSTER_TEXUTRE_BULLET_END]
{
	L"Jalsura_Bullet",
	L"Gadasura_Terrain_Bullet"

};

CMonster_Texture_Bullet::CMonster_Texture_Bullet(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CMonster_Bullet(pDevice, pDeviceContext)
{
}

CMonster_Texture_Bullet::CMonster_Texture_Bullet(const CMonster_Texture_Bullet & rhs)
	: CMonster_Bullet(rhs)
{
}

HRESULT CMonster_Texture_Bullet::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CMonster_Texture_Bullet::Initialize_Clone(void * pArg)
{
	if (nullptr != pArg)
	{
		memcpy(&m_Monster_Texture_BulletDesc, pArg, sizeof(MONSTER_TEXTURE_BULLETDESC));
	}
	else {
		MSGBOX("Monster_Bullet의 pArg가 Nullptr입니다.");
	}


	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());


	SetUp_Info();



	
	switch (m_Monster_Texture_BulletDesc.iBulletTextureNumber)
	{
	case Client::CMonster_Texture_Bullet::JALSURA_BULLET:
		break;
	case Client::CMonster_Texture_Bullet::GADASURA_TERRAIN_BULLET:
	{
		Gadasura_Terrain_Particle();

		break;
	}
	default:
		break;
	}

	return S_OK;
}

_int CMonster_Texture_Bullet::Update(_double dDeltaTime)
{
	if (__super::Update(dDeltaTime) < 0)return -1;

	m_dDeltaTime += dDeltaTime;

	if (m_Monster_Texture_BulletDesc.dDuration <= m_dDeltaTime)
	{
		Set_IsDead();
	}

	SetUp_Fire(dDeltaTime);

	return _int();
}

_int CMonster_Texture_Bullet::LateUpdate(_double dDeltaTime)
{
	if (__super::LateUpdate(dDeltaTime) < 0)return -1;

	if(m_Monster_Texture_BulletDesc.iBulletTextureNumber != GADASURA_TERRAIN_BULLET)
		FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));

	return _int();
}

_int CMonster_Texture_Bullet::Render()
{
	if (__super::Render() < 0)
		return -1;
	NULL_CHECK_RETURN(m_pVIBufferCom, E_FAIL);


	FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));

	CGameInstance* pInstance = GetSingle(CGameInstance);
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));

	if (FAILED(m_pTextureCom->Bind_OnShader(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;

	m_pVIBufferCom->Render(m_pShaderCom, 2);

	return _int();
}

_int CMonster_Texture_Bullet::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;


	return _int();
}

void CMonster_Texture_Bullet::Set_IsDead()
{


	m_bIsDead = true;
}

HRESULT CMonster_Texture_Bullet::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VNAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Rect), TAG_COM(Com_VIBuffer), (CComponent**)&m_pVIBufferCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Texture_Monster_Bullet), TAG_COM(Com_Texture), (CComponent**)&m_pTextureCom));

	m_pTextureCom->Change_TextureLayer(m_pMonster_Texture_BulletTag[m_Monster_Texture_BulletDesc.iBulletTextureNumber]);

	CTransform::TRANSFORMDESC tDesc = {};

	tDesc.fMovePerSec = m_Monster_Texture_BulletDesc.fSpeedPerSec;
	tDesc.fRotationPerSec = XMConvertToRadians(60);
	tDesc.vPivot = _float3(0, 0, 0);
	tDesc.fScalingPerSec = 1;


	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));

	return S_OK;
}

HRESULT CMonster_Texture_Bullet::SetUp_Info()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_pPlayerTransform = static_cast<CTransform*>(pGameInstance->Get_Commponent_By_LayerIndex(m_eNowSceneNum, TAG_LAY(Layer_Player), TAG_COM(Com_Transform)));

	if (nullptr != m_Monster_Texture_BulletDesc.Object_Transform)
	{
		m_pTransformCom->Scaled_All(XMLoadFloat3(&m_Monster_Texture_BulletDesc.fScale));

		_Vector vPosition = m_Monster_Texture_BulletDesc.Object_Transform->Get_MatrixState(CTransform::STATE_POS);

		vPosition += XMVector3Normalize(m_Monster_Texture_BulletDesc.Object_Transform->Get_MatrixState(CTransform::STATE_RIGHT)) *m_Monster_Texture_BulletDesc.fPositioning.x;
		vPosition += XMVector3Normalize(m_Monster_Texture_BulletDesc.Object_Transform->Get_MatrixState(CTransform::STATE_UP)) * m_Monster_Texture_BulletDesc.fPositioning.y;
		vPosition += XMVector3Normalize(m_Monster_Texture_BulletDesc.Object_Transform->Get_MatrixState(CTransform::STATE_LOOK)) * m_Monster_Texture_BulletDesc.fPositioning.z;

		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, vPosition);

		_float3 PlayerLookAtPibut = m_pPlayerTransform->Get_MatrixState_Float3(CTransform::STATE_POS);

		PlayerLookAtPibut.y += 1.f;

		m_pTransformCom->LookAt(XMLoadFloat3(&PlayerLookAtPibut));


		XMStoreFloat4(&m_fTempPos, m_pTransformCom->Get_MatrixState(CTransform::STATE_POS));
		XMStoreFloat3(&m_fTempLook, m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK));

		m_fTempPlayerPos = m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS);

	}
	else {
		MSGBOX("CMonster_Bullet_Universal Not Obejct_Transform");
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CMonster_Texture_Bullet::SetUp_Fire(_double dDeltaTime)
{
	switch (m_Monster_Texture_BulletDesc.iBulletTextureNumber)
	{
	case JALSURA_BULLET:
		Jalsura_Bullet(dDeltaTime);
		break;
	case GADASURA_TERRAIN_BULLET:
		Gadasura_Terrain_Bullet(dDeltaTime);
		break;
	default:
		MSGBOX("Not BulletTextureNumber");
		break;
	}
	return S_OK;
}

HRESULT CMonster_Texture_Bullet::Jalsura_Bullet(_double dDeltaTime)
{
	if (false == m_bOnceSwtich)
	{
		_Vector vTempPos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
		_Vector vTarPos = m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS);

		vTempPos = XMVectorSetY(vTempPos, 0.f);
		vTarPos = XMVectorSetY(vTarPos, 0.f);

		_Vector vLook = vTarPos - vTempPos;

		vLook = XMVector3Normalize(vLook);

		_Vector vRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook));

		_Vector vUp = XMVector3Normalize(XMVector3Cross(vLook, vRight));

		m_pTransformCom->Set_MatrixState(CTransform::STATE_RIGHT, vLook * XMVectorGetZ(m_pTransformCom->Get_MatrixScale(CTransform::STATE_LOOK)));
		m_pTransformCom->Set_MatrixState(CTransform::STATE_UP, -vRight *XMVectorGetX(m_pTransformCom->Get_MatrixScale(CTransform::STATE_RIGHT)));
		m_pTransformCom->Set_MatrixState(CTransform::STATE_LOOK, -vUp * XMVectorGetY(m_pTransformCom->Get_MatrixScale(CTransform::STATE_UP)));

		m_bOnceSwtich = true;
	}

	///////////////////////
	_float fDistance = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS).Get_Distance(XMLoadFloat4(&m_fTempPlayerPos));

	_float fDispersec = 2;


	m_fTempDis += fDispersec;

	m_pTransformCom->Scaled(CTransform::STATE_RIGHT, m_fTempDis);

	_Vector vPosition = XMLoadFloat4(&m_fTempPos);

	vPosition += XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::STATE_RIGHT)) * (m_fTempDis * 0.5f);

	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, vPosition);
	/////////////////////////////


	//_float fDispersec = 2;

	//if(m_bHitOn == false)
	//	m_fTempDis += fDispersec;

	//m_pTransformCom->Scaled(CTransform::STATE_RIGHT, m_fTempDis);

	//_Vector vPosition = XMLoadFloat4(&m_fTempPos);

	//_Vector CollisionPos = vPosition + XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::STATE_RIGHT)) * (m_fTempDis);

	//vPosition += XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::STATE_RIGHT)) * (m_fTempDis * 0.5f);

	//m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, vPosition);

	//_float vPlayerHitPos = m_pPlayerTransform->Get_MatrixState_Float3(CTransform::STATE_POS).Get_Distance(CollisionPos);

	//if (vPlayerHitPos <= 1)
	//{
	//	m_bHitOn = true;
	//}

	return S_OK;
}

HRESULT CMonster_Texture_Bullet::Gadasura_Terrain_Bullet(_double dDeltaTime)
{
	//if (false == m_bOnceSwtich)
	//{
	//	_Vector vTempPos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
	//	_Vector vTarPos = m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS);

	//	vTempPos = XMVectorSetY(vTempPos, 0.f);
	//	vTarPos = XMVectorSetY(vTarPos, 0.f);

	//	_Vector vLook = vTarPos - vTempPos;

	//	vLook = XMVector3Normalize(vLook);

	//	_Vector vRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook));

	//	_Vector vUp = XMVector3Normalize(XMVector3Cross(vLook, vRight));

	//	m_pTransformCom->Set_MatrixState(CTransform::STATE_RIGHT, vLook * XMVectorGetZ(m_pTransformCom->Get_MatrixScale(CTransform::STATE_LOOK)));
	//	m_pTransformCom->Set_MatrixState(CTransform::STATE_UP, -vRight *XMVectorGetX(m_pTransformCom->Get_MatrixScale(CTransform::STATE_RIGHT)));
	//	m_pTransformCom->Set_MatrixState(CTransform::STATE_LOOK, -vUp * XMVectorGetY(m_pTransformCom->Get_MatrixScale(CTransform::STATE_UP)));

	//	m_bOnceSwtich = true;


	//	//위치 재지정
	//	CTransform* Monster_Transform = static_cast<CTransform*>( static_cast<CGameObject*>(m_Monster_Texture_BulletDesc.Object)->Get_Component(TAG_COM(Com_Transform)));
	//	_Vector vTempLook = XMVector3Normalize(m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS) - Monster_Transform->Get_MatrixState(CTransform::STATE_POS));

	//	XMStoreFloat3(&m_fTempLook, vTempLook);


	//	GetSingle(CUtilityMgr)->Create_MeshInstance(SCENE_EDIT, m_MeshEffectDesc);
	//}

	//m_pTransformCom->MovetoDir_bySpeed(XMLoadFloat3(&m_fTempLook), m_Monster_Texture_BulletDesc.fSpeedPerSec, dDeltaTime);



	if (false == m_bOnceSwtich)
	{
		CTransform* Monster_Transform = static_cast<CTransform*>(static_cast<CGameObject*>(m_Monster_Texture_BulletDesc.Object)->Get_Component(TAG_COM(Com_Transform)));

		m_pTransformCom->Set_MatrixState(CTransform::STATE_LOOK, Monster_Transform->Get_MatrixState(CTransform::STATE_LOOK));

		GetSingle(CUtilityMgr)->Create_MeshInstance(SCENE_STAGE7, m_MeshEffectDesc);

		m_bOnceSwtich = true;
	}

	//m_fTempDis = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS).Get_Distance(m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS));

	//if (m_fTempDis <= 0.5)
	//{
	//	m_pTransformCom->LookAtExceptY(m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS), dDeltaTime);
	//}
	//m_pTransformCom->Turn_Dir()
	if (m_dDeltaTime >= 0.21963)
	{
		_Vector vTarget = XMVector3Normalize(m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS) - m_pTransformCom->Get_MatrixState(CTransform::STATE_POS));
		m_pTransformCom->Turn_Dir(vTarget, 0.95f);
	}

	m_pTransformCom->Move_Forward(dDeltaTime);



	return S_OK;
}

HRESULT CMonster_Texture_Bullet::Gadasura_Terrain_Particle()
{
	m_MeshEffectDesc.eParticleTypeID = InstanceEffect_Cone;
	m_MeshEffectDesc.eInstanceCount = Prototype_ModelInstance_64;
	m_MeshEffectDesc.ePassID = MeshPass_BrightColor;

	/*m_MeshEffectDesc.vFixedPosition = _float3(0);
	m_MeshEffectDesc.vPowerDirection = _float3(0, 1, 0);*/

	m_MeshEffectDesc.FollowingTarget = m_pTransformCom;
	m_MeshEffectDesc.iFollowingDir = 0;

	m_MeshEffectDesc.szModelMeshProtoTypeTag = TAG_MONSTER_BULLET(Prototype_Mesh_Monster_Bullet_Gadasura_Terrain);
	m_MeshEffectDesc.szNoiseTextureLayerTag = nullptr;
	m_MeshEffectDesc.iNoiseTextureIndex = 0;
	m_MeshEffectDesc.TotalParticleTime = 5.f;
	m_MeshEffectDesc.EachParticleLifeTime = 1.f;

	m_MeshEffectDesc.SizeChageFrequency = 1;
	m_MeshEffectDesc.ParticleSize = _float3(0.1f, 0.1f, 0.1f);
	m_MeshEffectDesc.ParticleSize2 = _float3(0, 0, 0);
	m_MeshEffectDesc.ColorChageFrequency = 0; //이미시블 색깔 교체
	m_MeshEffectDesc.TargetColor = _float4(1.f, 1.f, 1.f, 0.f); //w값은 림라이트
	m_MeshEffectDesc.TargetColor2 = _float4(0.f, 1.f, .0f, 0.f); //w값은 림라이트
	m_MeshEffectDesc.fMaxBoundaryRadius = 999999.f;
	m_MeshEffectDesc.Particle_Power = 0.f;
	m_MeshEffectDesc.PowerRandomRange = _float2(0.5f, 1.5f);
	m_MeshEffectDesc.SubPowerRandomRange = _float3(1.f, 1.f, 1.f);
	m_MeshEffectDesc.ParticleStartRandomPosMin = _float3(0, 0, 0); //Positioning
	m_MeshEffectDesc.ParticleStartRandomPosMax = _float3(0, 0, 0); //Positioning

	m_MeshEffectDesc.bEmissive = false;
	m_MeshEffectDesc.bAutoTurn = false;
	m_MeshEffectDesc.bIsOclusion = true;
	return S_OK;
}

CMonster_Texture_Bullet * CMonster_Texture_Bullet::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CMonster_Texture_Bullet*	pInstance = new CMonster_Texture_Bullet(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CMonster_Texture_Bullet");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMonster_Texture_Bullet::Clone(void * pArg)
{
	CMonster_Texture_Bullet*	pInstance = new CMonster_Texture_Bullet(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CMonster_Texture_Bullet");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMonster_Texture_Bullet::Free()
{

	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}