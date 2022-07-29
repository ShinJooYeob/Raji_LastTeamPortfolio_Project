#include "stdafx.h"
#include "Monster_Bullet_Universal.h"
#include "Monster_Vayusura_Leader.h"
#include "Monster_Texture_Bullet.h"

const _tchar* m_pMonster_Bullet_UniversalTag[CMonster_Bullet_Universal::MONSTER_BULLET_UNIVERSAL_END]
{
	TEXT("Monster_Bullet_Vayusura_Leader.fbx"),
	TEXT("Monster_Bullet_Tezabsura_Minion.fbx"),
	TEXT("Monster_Bullet_Tezabsura_Purple.fbx"),
	TEXT("Monster_Bullet_Tezabsura_Purple.fbx"),
	TEXT("Monster_Bullet_Tezabsura_Bomber.fbx"),
	TEXT("Monster_Bullet_Tezabsura_Bomber.fbx"),
	TEXT("Monster_Bullet_Tezabsura_Landmine.fbx"),
	TEXT("Missile.fbx"),
	TEXT("Ninjasura_Knife.fbx"),
	TEXT("sinkhole.fbx")

};

CMonster_Bullet_Universal::CMonster_Bullet_Universal(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CMonster_Bullet(pDevice, pDeviceContext)
{
}

CMonster_Bullet_Universal::CMonster_Bullet_Universal(const CMonster_Bullet_Universal & rhs)
	: CMonster_Bullet(rhs),
	m_vecJYTextureParticleDesc(rhs.m_vecJYTextureParticleDesc),
	m_vecJYMeshParticleDesc(rhs.m_vecJYMeshParticleDesc),
	m_vecJYNonMeshParticleDesc(rhs.m_vecJYNonMeshParticleDesc)
{

}

HRESULT CMonster_Bullet_Universal::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	CUtilityMgr* pUtil = GetSingle(CUtilityMgr);

#pragma region Tezabsura_Minion
	//0
	m_vecJYTextureParticleDesc.push_back(pUtil->Get_TextureParticleDesc(L"Monster_Tazabsura_Minon_1"));
	m_vecJYTextureParticleDesc[m_vecJYTextureParticleDesc.size() - 1].iFollowingDir = FollowingDir_Look;
	m_vecJYTextureParticleDesc[m_vecJYTextureParticleDesc.size() - 1].TotalParticleTime = 15.f;
	m_vecJYTextureParticleDesc[m_vecJYTextureParticleDesc.size() - 1].Particle_Power = 5.f;
	m_vecJYTextureParticleDesc[m_vecJYTextureParticleDesc.size() - 1].EachParticleLifeTime = 0.15f;
	m_vecJYTextureParticleDesc[m_vecJYTextureParticleDesc.size() - 1].eParticleTypeID = InstanceEffect_Ball;

	//1
	m_vecJYTextureParticleDesc.push_back(pUtil->Get_TextureParticleDesc(L"Monster_Tazabsura_Minon_2"));

#pragma  endregion Tezabsura_Minion

#pragma region Tezabsura_LandMine
	//2
	m_vecJYTextureParticleDesc.push_back(pUtil->Get_TextureParticleDesc(L"Monster_LandMine_1"));
	m_vecJYTextureParticleDesc[m_vecJYTextureParticleDesc.size() - 1].TotalParticleTime = 1.f;
	m_vecJYTextureParticleDesc[m_vecJYTextureParticleDesc.size() - 1].iFollowingDir = FollowingDir_Look;
	m_vecJYTextureParticleDesc[m_vecJYTextureParticleDesc.size() - 1].Particle_Power = 5.f;
	m_vecJYTextureParticleDesc[m_vecJYTextureParticleDesc.size() - 1].EachParticleLifeTime = 0.15f;
	m_vecJYTextureParticleDesc[m_vecJYTextureParticleDesc.size() - 1].eParticleTypeID = InstanceEffect_Ball;
	//3
	m_vecJYTextureParticleDesc.push_back(pUtil->Get_TextureParticleDesc(L"Monster_LandMine_2"));
	//4
	m_vecJYTextureParticleDesc.push_back(pUtil->Get_TextureParticleDesc(L"Monster_LandMine_3"));
	//5
	m_vecJYTextureParticleDesc.push_back(pUtil->Get_TextureParticleDesc(L"Monster_LandMine_4"));
	m_vecJYTextureParticleDesc[m_vecJYTextureParticleDesc.size() - 1].TotalParticleTime = 1.f;
	m_vecJYTextureParticleDesc[m_vecJYTextureParticleDesc.size() - 1].iFollowingDir = FollowingDir_Look;
	m_vecJYTextureParticleDesc[m_vecJYTextureParticleDesc.size() - 1].Particle_Power = 5.f;
	m_vecJYTextureParticleDesc[m_vecJYTextureParticleDesc.size() - 1].EachParticleLifeTime = 0.15f;
	m_vecJYTextureParticleDesc[m_vecJYTextureParticleDesc.size() - 1].eParticleTypeID = InstanceEffect_Ball;

	//6
	m_vecJYTextureParticleDesc.push_back(pUtil->Get_TextureParticleDesc(L"Monster_LandMine_5"));

#pragma  endregion Tezabsura_LandMine




#pragma region Mesh


	{
		NONINSTNESHEFTDESC tNIMEDesc;

		tNIMEDesc.eMeshType = Prototype_Mesh_Tornado;
		tNIMEDesc.fAppearTime = 0.25f;
		tNIMEDesc.fMaxTime_Duration = tNIMEDesc.fAppearTime*2.f;

		tNIMEDesc.noisingdir = _float2(0, 1);

		tNIMEDesc.fDistortionNoisingPushPower = 20.f;
		tNIMEDesc.NoiseTextureIndex = 6;
		tNIMEDesc.MaskTextureIndex = 81;
		tNIMEDesc.iDiffuseTextureIndex = 299;
		tNIMEDesc.m_iPassIndex = 19;
		tNIMEDesc.vEmissive = _float4(1, 0.5f, 1.f, 0);
		tNIMEDesc.vLimLight = _float4(0.35f, 0.85f, 0.35f, 1);
		tNIMEDesc.NoiseTextureIndex = 381;
		tNIMEDesc.vColor = _float4(1, 1, 1, 1);

		tNIMEDesc.RotAxis = FollowingDir_Up;
		tNIMEDesc.RotationSpeedPerSec = -1080.f;
		tNIMEDesc.vSize = _float3(1.f, 1.5f, 1.f);
		tNIMEDesc.vLimLight = _float4(_float3(vOldRimLightColor), 1);

		tNIMEDesc.MoveDir = FollowingDir_Look;
		tNIMEDesc.MoveSpeed = 0.f;

		m_vecJYNonMeshParticleDesc.push_back(tNIMEDesc);
	}
	{
		NONINSTNESHEFTDESC tNIMEDesc;

		tNIMEDesc.eMeshType = Prototype_Mesh_Tornado2;
		tNIMEDesc.fAppearTime = 0.25f;
		tNIMEDesc.fMaxTime_Duration = tNIMEDesc.fAppearTime*2.f;

		tNIMEDesc.noisingdir = _float2(1, 0);

		tNIMEDesc.fDistortionNoisingPushPower = 20.f;
		tNIMEDesc.NoiseTextureIndex = 6;
		tNIMEDesc.MaskTextureIndex = 81;
		tNIMEDesc.iDiffuseTextureIndex = 299;
		tNIMEDesc.m_iPassIndex = 19;
		tNIMEDesc.vEmissive = _float4(1, 0.5f, 1.f, 0);
		tNIMEDesc.vLimLight = _float4(0.35f, 0.85f, 0.35f, 1);
		tNIMEDesc.NoiseTextureIndex = 381;
		tNIMEDesc.vColor = _float4(1, 1, 1, 1);

		tNIMEDesc.RotAxis = FollowingDir_Up;
		tNIMEDesc.RotationSpeedPerSec = -1080.f;
		tNIMEDesc.vSize = _float3(1.f, 1.5f, 1.f);
		tNIMEDesc.vLimLight = _float4(_float3(vOldRimLightColor), 1);

		tNIMEDesc.MoveDir = FollowingDir_Look;
		tNIMEDesc.MoveSpeed = 0.f;
		m_vecJYNonMeshParticleDesc.push_back(tNIMEDesc);
	}

	m_vecJYNonMeshParticleDesc[1].vLookDir = m_vecJYNonMeshParticleDesc[0].vLookDir
		= _float3(1.f, 0, 0);
	m_vecJYTextureParticleDesc[4].vPowerDirection = _float3(0.00001f, 1.f, 0);
	m_vecJYTextureParticleDesc[3].vPowerDirection = _float3(1.f, 0, 0);
	m_vecJYTextureParticleDesc[4].EachParticleLifeTime = m_vecJYTextureParticleDesc[3].EachParticleLifeTime = 0.35f;
	m_vecJYTextureParticleDesc[4].Particle_Power = 20.f;
	m_vecJYTextureParticleDesc[4].TotalParticleTime = m_vecJYTextureParticleDesc[3].TotalParticleTime = 0.1f;
#pragma endregion



	return S_OK;
}

HRESULT CMonster_Bullet_Universal::Initialize_Clone(void * pArg)
{
	if (nullptr != pArg)
	{
		memcpy(&m_Monster_Bullet_UniversalDesc, pArg, sizeof(MONSTER_BULLET_UNIVERSALDESC));
	}
	else {
		MSGBOX("Monster_Bullet의 pArg가 Nullptr입니다.");
	}


	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());


	SetUp_Info();
	SetUp_BoneMatrix();


	FAILED_CHECK(Ready_JYParticleDesc());

	return S_OK;
}

_int CMonster_Bullet_Universal::Update(_double dDeltaTime)
{
	if (__super::Update(dDeltaTime) < 0)return -1;

	m_dDeltaTime += dDeltaTime;

	if (m_Monster_Bullet_UniversalDesc.dDuration <= m_dDeltaTime)
	{
		Set_IsDead();
	}

	SetUp_Fire(dDeltaTime);

	FAILED_CHECK(Update_JYParticleTransform(dDeltaTime));


	Update_Collider(dDeltaTime);

	return _int();
}

_int CMonster_Bullet_Universal::LateUpdate(_double dDeltaTime)
{
	if (__super::LateUpdate(dDeltaTime) < 0)return -1;

	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_NONANIMMODEL, this, m_pTransformCom, m_pShaderCom, m_pModel));


#ifdef _DEBUG
	FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pColliderCom));
#endif

	return _int();
}

_int CMonster_Bullet_Universal::Render()
{
	if (__super::Render() < 0)
		return -1;
	NULL_CHECK_RETURN(m_pModel, E_FAIL);

	CGameInstance* pInstance = GetSingle(CGameInstance);

	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));

	FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));

	_uint iNumMaterials = m_pModel->Get_NumMaterial();

	for (_uint i = 0; i < iNumMaterials; ++i)
	{
		m_pModel->Bind_OnShader(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture");

		m_pModel->Render(m_pShaderCom, 3, i);
	}

	return _int();
}

_int CMonster_Bullet_Universal::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;


	return _int();
}

void CMonster_Bullet_Universal::Set_IsDead()
{
	switch (m_Monster_Bullet_UniversalDesc.iBulletMeshNumber)
	{
	case TEZABSURA_MINION_BULLET:
	{
		g_pGameInstance->Play3D_Sound(TEXT("EH_Tezaabsura_Tezaab_Hit_01.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_SUBEFFECT, 0.5f);
		break;
	}
	case TEZABSURA_LANDMINE_DEFAULT_BULLET:
	{
		g_pGameInstance->Play3D_Sound(TEXT("EH_Tezaabsura_Tezaab_Hit_02.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_SUBEFFECT, 0.5f);
		break;
	}
	case TEZABSURA_LANDMINE_INSTALL:
	{
		g_pGameInstance->Play3D_Sound(TEXT("EH_M1_1038.mp3"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_SUBEFFECT, 0.5f);
		break;
	}
	default:
		break;
	}

	m_bIsDead = true;
}

void CMonster_Bullet_Universal::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	if (CollisionTypeID::CollisionType_Player == eConflictedObjCollisionType)
	{
		_Vector vDamageDir = XMVector3Normalize(pConflictedCollider->Get_ColliderPosition(iConflictedObjColliderIndex).XMVector() - m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS));
		pConflictedObj->Take_Damage(this, 1.f, vDamageDir, m_bOnKnockbackCol, m_fKnockbackColPower);
		pConflictedCollider->Set_Conflicted(1.f);


		switch (m_Monster_Bullet_UniversalDesc.iBulletMeshNumber)
		{
		case TEZABSURA_MINION_BULLET:
		{
			m_vecJYTextureParticleDesc[1].vFixedPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
			GetSingle(CUtilityMgr)->Create_TextureInstance(m_eNowSceneNum, m_vecJYTextureParticleDesc[1]);
			Set_IsDead();
			break;
		}
		case TEZABSURA_LANDMINE_DEFAULT_BULLET:
		{
			m_vecJYTextureParticleDesc[6].vFixedPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
			GetSingle(CUtilityMgr)->Create_TextureInstance(m_eNowSceneNum, m_vecJYTextureParticleDesc[6]);
			Set_IsDead();
			break;
		}
		case TEZABSURA_LANDMINE_INSTALL:
		{
			m_vecJYNonMeshParticleDesc[1].vPosition = m_vecJYNonMeshParticleDesc[0].vPosition
				= m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);


			g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_PlayerEffect), TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecJYNonMeshParticleDesc[0]);
			g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_PlayerEffect), TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecJYNonMeshParticleDesc[1]);

			m_vecJYTextureParticleDesc[4].vFixedPosition = m_vecJYTextureParticleDesc[3].vFixedPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);


			GetSingle(CUtilityMgr)->Create_TextureInstance(m_eNowSceneNum, m_vecJYTextureParticleDesc[3]);
			GetSingle(CUtilityMgr)->Create_TextureInstance(m_eNowSceneNum, m_vecJYTextureParticleDesc[4]);
			Set_IsDead();
			break;
		}
		default:
			break;
		}
	}
}

_float CMonster_Bullet_Universal::Take_Damage(CGameObject * pTargetObject, _float fDamageAmount, _fVector vDamageDir, _bool bKnockback, _float fKnockbackPower)
{
	return _float();
}

HRESULT CMonster_Bullet_Universal::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VNAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, m_pMonster_Bullet_UniversalTag[m_Monster_Bullet_UniversalDesc.iBulletMeshNumber], TAG_COM(Com_Model), (CComponent**)&m_pModel));

	CTransform::TRANSFORMDESC tDesc = {};

	tDesc.fMovePerSec = m_Monster_Bullet_UniversalDesc.fSpeedPerSec;
	tDesc.fRotationPerSec = XMConvertToRadians(60);
	tDesc.vPivot = _float3(0, 0, 0);
	tDesc.fScalingPerSec = 1;


	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));


	SetUp_Collider();
	return S_OK;
}

HRESULT CMonster_Bullet_Universal::SetUp_BoneMatrix()
{
	if (m_Monster_Bullet_UniversalDesc.bBornAttachOn)
	{
		m_AttachedDesc.Initialize_AttachedDesc(static_cast<CGameObject*>(m_Monster_Bullet_UniversalDesc.Object),
			m_Monster_Bullet_UniversalDesc.pBoneName, 
			m_Monster_Bullet_UniversalDesc.fScale, 
			_float3(0.f, 0.f, 0.f), m_Monster_Bullet_UniversalDesc.fPositioning);
	}

	return S_OK;
}

HRESULT CMonster_Bullet_Universal::SetUp_Info()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_pPlayerTransform = static_cast<CTransform*>(pGameInstance->Get_Commponent_By_LayerIndex(m_eNowSceneNum, TAG_LAY(Layer_Player), TAG_COM(Com_Transform)));

	if (nullptr != m_Monster_Bullet_UniversalDesc.Object_Transform)
	{
		m_pTransformCom->Scaled_All(XMLoadFloat3(&m_Monster_Bullet_UniversalDesc.fScale));

		_Vector vPosition = m_Monster_Bullet_UniversalDesc.Object_Transform->Get_MatrixState(CTransform::STATE_POS);

		vPosition += XMVector3Normalize(m_Monster_Bullet_UniversalDesc.Object_Transform->Get_MatrixState(CTransform::STATE_RIGHT)) *m_Monster_Bullet_UniversalDesc.fPositioning.x;
		vPosition += XMVector3Normalize(m_Monster_Bullet_UniversalDesc.Object_Transform->Get_MatrixState(CTransform::STATE_UP)) * m_Monster_Bullet_UniversalDesc.fPositioning.y;
		vPosition += XMVector3Normalize(m_Monster_Bullet_UniversalDesc.Object_Transform->Get_MatrixState(CTransform::STATE_LOOK)) * m_Monster_Bullet_UniversalDesc.fPositioning.z;

		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, vPosition);

		_float3 PlayerLookAtPibut = m_pPlayerTransform->Get_MatrixState_Float3(CTransform::STATE_POS);

		PlayerLookAtPibut.y += 1.f;

		m_pTransformCom->LookAt(XMLoadFloat3(&PlayerLookAtPibut));


		XMStoreFloat4(&m_fTempPos, m_pTransformCom->Get_MatrixState(CTransform::STATE_POS));
		XMStoreFloat3(&m_fTempLook, m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK));

		m_fTempPlayerPos = m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS);








		switch (m_Monster_Bullet_UniversalDesc.iBulletMeshNumber)
		{
		case Client::CMonster_Bullet_Universal::VAYUSURA_LEADER_BULLET:
			break;
		case Client::CMonster_Bullet_Universal::TEZABSURA_MINION_BULLET:
		{
			Set_LimLight_N_Emissive(_float4(0.5f, 0.8f, 0.5f, 1.f), _float4(1.f, 0.5f, 1.f, 1.f));
			m_pTransformCom->LookDir_ver2(m_Monster_Bullet_UniversalDesc.Object_Transform->Get_MatrixState(CTransform::STATE_LOOK));
		}
		break;
		case Client::CMonster_Bullet_Universal::TEZABSURA_PURPLE_DEFAULT_BULLET:
			break;
		case Client::CMonster_Bullet_Universal::TEZABSURA_PURPLE_PRIMARY_BULLET:
			break;
		case Client::CMonster_Bullet_Universal::TEZABSURA_BOMBER_DEFAULT_BULLET:
			break;
		case Client::CMonster_Bullet_Universal::TEZABSURA_BOMBER_HOWITZER_BULLET:
			break;
		case Client::CMonster_Bullet_Universal::TEZABSURA_LANDMINE_DEFAULT_BULLET:
		{
			Set_LimLight_N_Emissive(_float4(_float3(0.98046875f, 0.03359375f, 0.09140625f), 1.f), _float4(1.f, 1.f, 1.f, 1.f));

		}

			break;
		case Client::CMonster_Bullet_Universal::TEZABSURA_LANDMINE_INSTALL:
		{

			Set_LimLight_N_Emissive(_float4(_float3(0.98046875f, 0.03359375f, 0.09140625f), 1.f), _float4(1.f, 1.f, 1.f, 1.f));
			m_pTransformCom->LookDir_ver2(m_Monster_Bullet_UniversalDesc.Object_Transform->Get_MatrixState(CTransform::STATE_LOOK));
		}
			break;
		case Client::CMonster_Bullet_Universal::NINJASURA_KNIFE:
			break;
		case Client::CMonster_Bullet_Universal::GADASURA_SINKHOLE:
			break;
		case Client::CMonster_Bullet_Universal::MONSTER_BULLET_UNIVERSAL_END:
			break;
		default:
			break;
		}


	}
	else {
		MSGBOX("CMonster_Bullet_Universal Not Obejct_Transform");
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CMonster_Bullet_Universal::SetUp_Fire(_double dDeltaTime)
{
	switch (m_Monster_Bullet_UniversalDesc.iBulletMeshNumber)
	{
	case VAYUSURA_LEADER_BULLET:
		Vayusura_Leader_Bullet(dDeltaTime);
		break;
	case TEZABSURA_MINION_BULLET:
		Tezabsura_Minion_Bullet(dDeltaTime);
		break;
	case TEZABSURA_PURPLE_DEFAULT_BULLET:
		Tezabsura_Purple_Default_Bullet(dDeltaTime);
		break;
	case TEZABSURA_PURPLE_PRIMARY_BULLET:
		Tezabsura_Purple_Primary_Bullet(dDeltaTime);
		break;
	case TEZABSURA_BOMBER_DEFAULT_BULLET:
		Tezabsura_Bomber_Default_Bullet(dDeltaTime);
		break;
	case TEZABSURA_BOMBER_HOWITZER_BULLET:
		Tezabsura_Bomber_Howitzer_Bullet(dDeltaTime);
		break;
	case TEZABSURA_LANDMINE_DEFAULT_BULLET:
		Tezabsura_Landmine_Default_Bullet(dDeltaTime);
		break;
	case TEZABSURA_LANDMINE_INSTALL:
		Tezabsura_Landmine_Install(dDeltaTime);
		break;
	case NINJASURA_KNIFE:
		Ninjasura_Knife(dDeltaTime);
		break;
	case GADASURA_SINKHOLE:
		Gadasura_Black(dDeltaTime);
		break;
	default:
		MSGBOX("Not BulletMeshNumber");
		break;
	}
	return S_OK;
}

HRESULT CMonster_Bullet_Universal::SetUp_Collider()
{
	switch (m_Monster_Bullet_UniversalDesc.iBulletMeshNumber)
	{
	case TEZABSURA_MINION_BULLET:
	{
		FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider), (CComponent**)&m_pColliderCom));

		/////////////////m_pColliderCom!@!@#$@!#$@#$@$!@%#$%@#$%%^^W@!
		COLLIDERDESC			ColliderDesc;
		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(1.5f, 1.5f, 1.5f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
		FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		break;
	}
	case TEZABSURA_LANDMINE_INSTALL:
	{
		FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider), (CComponent**)&m_pColliderCom));

		COLLIDERDESC			ColliderDesc;
		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(100.f, 100.f, 100.f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
		FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));


		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(30.f, 30.f, 90.f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
		FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_OBB, &ColliderDesc));
		m_pColliderCom->Set_ParantBuffer();
		break;
	}
	case TEZABSURA_LANDMINE_DEFAULT_BULLET:
	{
		FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider), (CComponent**)&m_pColliderCom));

		/////////////////m_pColliderCom!@!@#$@!#$@#$@$!@%#$%@#$%%^^W@!
		COLLIDERDESC			ColliderDesc;
		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(1.5f, 1.5f, 1.5f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
		FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		break;
		break;
	}
	default:
		break;
	}

	return S_OK;
}

HRESULT CMonster_Bullet_Universal::Update_Collider(_double dDeltaTime)
{
	if (m_pColliderCom == nullptr)
		return S_OK;

	m_pColliderCom->Update_ConflictPassedTime(dDeltaTime);

	//Collider

	switch (m_Monster_Bullet_UniversalDesc.iBulletMeshNumber)
	{
	case TEZABSURA_MINION_BULLET:
	{
		_uint	iNumCollider = m_pColliderCom->Get_NumColliderBuffer();
		for (_uint i = 0; i < iNumCollider; i++)
			m_pColliderCom->Update_Transform(i, m_pTransformCom->Get_WorldMatrix());
		break;
	}
	default:
	{
		_uint	iNumCollider = m_pColliderCom->Get_NumColliderBuffer();
		for (_uint i = 0; i < iNumCollider; i++)
			m_pColliderCom->Update_Transform(i, m_pTransformCom->Get_WorldMatrix());
		break;
	}
	}

	FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_MonsterWeapon, this, m_pColliderCom));

	return S_OK;
}

_Vector CMonster_Bullet_Universal::Bezier(_Vector StartPoint, _Vector LastPoint, _double dDeltaTime)
{
	if (m_dBezierTime <= 1)
	{
		m_dBezierTime += dDeltaTime;
	}
	else {
		Set_IsDead();
	}

	_Vector  CenterPos = (StartPoint - LastPoint);

	CenterPos = StartPoint + (CenterPos * 0.1f); //베지어 곡선은 스타트 지점에 가까울수록 날카로운 곡선이 아닌 둥글 둥글한 포물선이 되는듯

	_float4 TempCenter;
	XMStoreFloat4(&TempCenter, CenterPos);

	TempCenter.y = TempCenter.y + 10;

	_Vector BezierPos = ((1 - (_float)m_dBezierTime)*(1 - (_float)m_dBezierTime)*StartPoint) + (2 * (_float)m_dBezierTime*(1 - (_float)m_dBezierTime) *XMLoadFloat4(&TempCenter)) + ((_float)m_dBezierTime*(_float)m_dBezierTime)*LastPoint;

	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, BezierPos);


	//_Vector  CenterPos = (StartPoint - LastPoint) ;

	//CenterPos = StartPoint + (CenterPos * 0.5);

	//_float4 TempCenter;
	//XMStoreFloat4(&TempCenter, CenterPos);

	//TempCenter.y = TempCenter.y + 10;

	//_Vector BezierPos = ((1 - m_dBezierTime)*(1 - m_dBezierTime)*StartPoint) + (2 * m_dBezierTime*(1 - m_dBezierTime) *XMLoadFloat4(&TempCenter)) + (m_dBezierTime*m_dBezierTime)*LastPoint;

	//m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, BezierPos)

	return _Vector();
}

HRESULT CMonster_Bullet_Universal::Vayusura_Leader_Bullet(_double dDeltaTime)
{
	//_Vector	vPosition = m_AttachedDesc.Get_AttachedBoneWorldPosition(); 이것도 됨

	//vPosition += XMVector3Normalize(m_Monster_Bullet_UniversalDesc.Object_Transform->Get_MatrixState(CTransform::STATE_RIGHT)) *m_Monster_Bullet_UniversalDesc.fPositioning.x;
	//vPosition += XMVector3Normalize(m_Monster_Bullet_UniversalDesc.Object_Transform->Get_MatrixState(CTransform::STATE_UP)) * m_Monster_Bullet_UniversalDesc.fPositioning.y;
	//vPosition += XMVector3Normalize(m_Monster_Bullet_UniversalDesc.Object_Transform->Get_MatrixState(CTransform::STATE_LOOK)) * m_Monster_Bullet_UniversalDesc.fPositioning.z;
	CMonster_Vayusura_Leader* Monster_Object = static_cast<CMonster_Vayusura_Leader*>(m_Monster_Bullet_UniversalDesc.Object);

	if(false == Monster_Object->Get_AttackAttackFrieOn() && !m_bIsFired)
	{

		m_PassedTime += dDeltaTime;
		//m_pTransformCom->Scaling_All(dDeltaTime);
		m_pTransformCom->Scaled_All(_float3(_float(m_PassedTime) * 1.f ));
		 

		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, m_AttachedDesc.Get_AttachedBoneWorldPosition());


		_float3 fPlayerPos = m_pPlayerTransform->Get_MatrixState_Float3(CTransform::STATE_POS);
		fPlayerPos.y += 0.5f;

		XMStoreFloat3(&m_fTempLook, XMVector3Normalize(XMLoadFloat3(&fPlayerPos) - m_pTransformCom->Get_MatrixState(CTransform::STATE_POS)));
	}
	else {
		m_bIsFired = true;
		_Vector vPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
		_Vector vLook = XMLoadFloat3(&m_fTempLook);

		vPosition += XMVector3Normalize(vLook) * m_Monster_Bullet_UniversalDesc.fSpeedPerSec * (_float)dDeltaTime;

		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, vPosition);
	}

	if(true == Monster_Object->Get_AttackCanceOn())
	{
		Set_IsDead();
	}



	return S_OK;
}

HRESULT CMonster_Bullet_Universal::Tezabsura_Minion_Bullet(_double dDeltaTime)
{
	//_Vector vPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
	//_Vector vLook = XMLoadFloat3(&m_fTempLook);

	//vPosition += XMVector3Normalize(vLook) * m_Monster_Bullet_UniversalDesc.fSpeedPerSec * (_float)dDeltaTime;

	//m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, vPosition);

	if (m_bOnceSwtich == false)
	{
		CTransform* Monster_Transform = static_cast<CTransform*>(static_cast<CGameObject*>(m_Monster_Bullet_UniversalDesc.Object)->Get_Component(TAG_COM(Com_Transform)));

		m_pTransformCom->Set_MatrixState(CTransform::STATE_LOOK, Monster_Transform->Get_MatrixState(CTransform::STATE_LOOK));

		
		m_bOnceSwtich = true;
	}

	if (m_dDeltaTime < 1.5f)
	{
		
		if (m_dDeltaTime > 0.65f)
		{
			_Vector NewLook =  XMVector3Normalize(m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS) - m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) );
			NewLook = m_pTransformCom->Get_MatrixState_Normalized(CTransform::STATE_LOOK) * 0.85f + NewLook * 0.15f;
			m_pTransformCom->LookDir_ver2(NewLook);
		}
		else
		{

			m_pTransformCom->Set_TurnSpeed(XMConvertToRadians(680.f * (_float)m_dDeltaTime));
			m_pTransformCom->Turn_CCW(m_pTransformCom->Get_MatrixState(CTransform::STATE_RIGHT), dDeltaTime);
		}
	}


	m_pTransformCom->Move_Forward((1+ m_dDeltaTime * 2.f)*dDeltaTime);

	if (m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS).y < m_pPlayerTransform->Get_MatrixState_Float3(CTransform::STATE_POS).y)
	{
		m_vecJYTextureParticleDesc[1].vFixedPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
		GetSingle(CUtilityMgr)->Create_TextureInstance(m_eNowSceneNum, m_vecJYTextureParticleDesc[1]);

		////////////////////////////////EH

		CMonster_Texture_Bullet::MONSTER_TEXTURE_BULLETDESC Monster_Texture_BulletDesc;

		Monster_Texture_BulletDesc.iBulletTextureNumber = CMonster_Texture_Bullet::NONTEXTURE_SPHERE;
		Monster_Texture_BulletDesc.fSpeedPerSec = 0;
		Monster_Texture_BulletDesc.fScale = _float3(5.f, 5.f, 5.f);

		Monster_Texture_BulletDesc.Object_Transform = m_pTransformCom;
		Monster_Texture_BulletDesc.fPositioning = _float3(0.f, 0.f, 0.f);


		Monster_Texture_BulletDesc.Object = this;

		Monster_Texture_BulletDesc.dDuration = 0.2;

		FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_MonsterBullet), TAG_OP(Prototype_Object_Monster_Texture_Bullet), &Monster_Texture_BulletDesc));
		///////////////////////////////////

		Set_IsDead();
	}
	


	return S_OK;
}

HRESULT CMonster_Bullet_Universal::Tezabsura_Purple_Default_Bullet(_double dDeltaTime)
{
	_Vector vPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
	_Vector vLook = XMLoadFloat3(&m_Monster_Bullet_UniversalDesc.fLook);

	vPosition += XMVector3Normalize(vLook) * m_Monster_Bullet_UniversalDesc.fSpeedPerSec * (_float)dDeltaTime;

	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, vPosition);

	return S_OK;
}

HRESULT CMonster_Bullet_Universal::Tezabsura_Purple_Primary_Bullet(_double dDeltaTime)
{
	_Vector vPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
	_Vector vLook = XMLoadFloat3(&m_fTempLook);

	vPosition += XMVector3Normalize(vLook) * m_Monster_Bullet_UniversalDesc.fSpeedPerSec * (_float)dDeltaTime;

	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, vPosition);

	return S_OK;
}

HRESULT CMonster_Bullet_Universal::Tezabsura_Bomber_Default_Bullet(_double dDeltaTime)
{
	/*
	_Vector vPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
	_Vector vLook = XMLoadFloat3(&m_fTempLook);

	vPosition += XMVector3Normalize(vLook) * m_Monster_Bullet_UniversalDesc.fSpeedPerSec * (_float)dDeltaTime;

	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, vPosition);
	*/
	_Vector vPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
	_Vector vLook = XMLoadFloat3(&m_Monster_Bullet_UniversalDesc.fLook);

	vPosition += XMVector3Normalize(vLook) * m_Monster_Bullet_UniversalDesc.fSpeedPerSec * (_float)dDeltaTime;

	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, vPosition);

	return S_OK;
}

HRESULT CMonster_Bullet_Universal::Tezabsura_Bomber_Howitzer_Bullet(_double dDeltaTime)
{
	Bezier(XMLoadFloat4(&m_fTempPos), XMLoadFloat4(&m_fTempPlayerPos), dDeltaTime);
	//_Vector vPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
	//_Vector vLook = XMLoadFloat3(&m_fTempLook);

	//vPosition += XMVector3Normalize(vLook) * m_Monster_Bullet_UniversalDesc.fSpeedPerSec * (_float)dDeltaTime;

	//m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, vPosition);

	return S_OK;
}

HRESULT CMonster_Bullet_Universal::Tezabsura_Landmine_Default_Bullet(_double dDeltaTime)
{

	if (m_bOnceSwtich == false)
	{
		CTransform* Monster_Transform = static_cast<CTransform*>(static_cast<CGameObject*>(m_Monster_Bullet_UniversalDesc.Object)->Get_Component(TAG_COM(Com_Transform)));

		m_pTransformCom->Set_MatrixState(CTransform::STATE_LOOK, Monster_Transform->Get_MatrixState(CTransform::STATE_LOOK));


		m_bOnceSwtich = true;
	}

	if (m_dDeltaTime < 1.5f)
	{

		if (m_dDeltaTime > 0.65f)
		{
			_Vector NewLook = XMVector3Normalize(m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS) - m_pTransformCom->Get_MatrixState(CTransform::STATE_POS));
			NewLook = m_pTransformCom->Get_MatrixState_Normalized(CTransform::STATE_LOOK) * 0.85f + NewLook * 0.15f;
			m_pTransformCom->LookDir_ver2(NewLook);
		}
		else
		{

			m_pTransformCom->Set_TurnSpeed(XMConvertToRadians(680.f * (_float)m_dDeltaTime));
			m_pTransformCom->Turn_CCW(m_pTransformCom->Get_MatrixState(CTransform::STATE_RIGHT), dDeltaTime);
		}
	}


	m_pTransformCom->Move_Forward((1 + m_dDeltaTime * 2.f)*dDeltaTime);

	if (m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS).y < m_pPlayerTransform->Get_MatrixState_Float3(CTransform::STATE_POS).y)
	{
		m_vecJYTextureParticleDesc[6].vFixedPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
		GetSingle(CUtilityMgr)->Create_TextureInstance(m_eNowSceneNum, m_vecJYTextureParticleDesc[6]);

		////////////////////////////////EH

		CMonster_Texture_Bullet::MONSTER_TEXTURE_BULLETDESC Monster_Texture_BulletDesc;

		Monster_Texture_BulletDesc.iBulletTextureNumber = CMonster_Texture_Bullet::NONTEXTURE_SPHERE;
		Monster_Texture_BulletDesc.fSpeedPerSec = 0;
		Monster_Texture_BulletDesc.fScale = _float3(5.f, 5.f, 5.f);

		Monster_Texture_BulletDesc.Object_Transform = m_pTransformCom;
		Monster_Texture_BulletDesc.fPositioning = _float3(0.f, 0.f, 0.f);


		Monster_Texture_BulletDesc.Object = this;

		Monster_Texture_BulletDesc.dDuration = 0.2;

		FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_MonsterBullet), TAG_OP(Prototype_Object_Monster_Texture_Bullet), &Monster_Texture_BulletDesc));
		///////////////////////////////////
		Set_IsDead();
	}

	return S_OK;
}

HRESULT CMonster_Bullet_Universal::Tezabsura_Landmine_Install(_double dDeltaTime)
{
	if (m_dBezierTime < 1)
	{
		if (m_iBoolInt == 0)
		{
			m_fTempPlayerPos.y += 0.5;
			m_iBoolInt += 1;
		}
		m_dBezierTime += dDeltaTime;
		_Vector  CenterPos = (XMLoadFloat4(&m_fTempPos) - XMLoadFloat4(&m_fTempPlayerPos));
		CenterPos = XMLoadFloat4(&m_fTempPos) + (CenterPos * .1f); //베지어 곡선은 스타트 지점에 가까울수록 날카로운 곡선이 아닌 둥글 둥글한 포물선이 되는듯
		_float4 TempCenter;
		XMStoreFloat4(&TempCenter, CenterPos);
		TempCenter.y = TempCenter.y + 10;
		_Vector BezierPos = ((1 - (_float)m_dBezierTime)*(1 - (_float)m_dBezierTime)*XMLoadFloat4(&m_fTempPos)) + (2 * (_float)m_dBezierTime*(1 - (_float)m_dBezierTime) *XMLoadFloat4(&TempCenter)) + ((_float)m_dBezierTime* (_float)m_dBezierTime)*XMLoadFloat4(&m_fTempPlayerPos);
		_Vector OldPos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, BezierPos);
		m_pTransformCom->LookDir(XMVector3Normalize(OldPos - BezierPos ));

		if (m_dBezierTime > 1)
		{

		}
	}
	else
	{
		if (m_iBoolInt == 1)
		{
			g_pGameInstance->Play3D_Sound(TEXT("EH_M1_1117.mp3"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_SUBEFFECT, 0.5f);
			m_iBoolInt += 1;
		}
		m_dBezierTime += dDeltaTime;
		if (m_dBezierTime > 1.3f) m_dBezierTime = 1.;

		m_pTransformCom->Turn_CW(m_pTransformCom->Get_MatrixState_Normalized(CTransform::STATE_LOOK), dDeltaTime * 4);

		_float Value = g_pGameInstance->Easing_Return(TYPE_Linear, TYPE_Linear, 0, 1, (_float)m_dBezierTime - 1.f, 0.3f);
		Value = max(min(Value, 1.f), 0.f);
		Set_LimLight_N_Emissive(_float4(_float3(0.98046875f, 0.03359375f, 0.09140625f), Value), _float4(1.f, 1.f, min(Value + 0.001f,1.f), 1.f));


	}

	if (m_dDeltaTime > 4)
	{

		m_vecJYNonMeshParticleDesc[1].vPosition =m_vecJYNonMeshParticleDesc[0].vPosition 
			= m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);


		g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_PlayerEffect), TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecJYNonMeshParticleDesc[0]);
		g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_PlayerEffect), TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecJYNonMeshParticleDesc[1]);

		m_vecJYTextureParticleDesc[4].vFixedPosition =m_vecJYTextureParticleDesc[3].vFixedPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);


		GetSingle(CUtilityMgr)->Create_TextureInstance(m_eNowSceneNum, m_vecJYTextureParticleDesc[3]);
		GetSingle(CUtilityMgr)->Create_TextureInstance(m_eNowSceneNum, m_vecJYTextureParticleDesc[4]);


		////////////////////////////////EH

		CMonster_Texture_Bullet::MONSTER_TEXTURE_BULLETDESC Monster_Texture_BulletDesc;

		Monster_Texture_BulletDesc.iBulletTextureNumber = CMonster_Texture_Bullet::NONTEXTURE_SPHERE;
		Monster_Texture_BulletDesc.fSpeedPerSec = 0;
		Monster_Texture_BulletDesc.fScale = _float3(7.f, 7.f, 7.f);

		Monster_Texture_BulletDesc.Object_Transform = m_pTransformCom;
		Monster_Texture_BulletDesc.fPositioning = _float3(0.f, 0.f, 0.f);


		Monster_Texture_BulletDesc.Object = this;

		Monster_Texture_BulletDesc.dDuration = 0.3;

		FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_MonsterBullet), TAG_OP(Prototype_Object_Monster_Texture_Bullet), &Monster_Texture_BulletDesc));

		///////////////////////////////////

		Set_IsDead();
	}


	return S_OK;
}

HRESULT CMonster_Bullet_Universal::Ninjasura_Knife(_double dDeltaTime)
{

	_float fPosY = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS).y;
	
	if (m_fTempPlayerPos.y <= fPosY)
	{
		_Vector vPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
		_Vector vLook = XMLoadFloat3(&m_Monster_Bullet_UniversalDesc.fLook);

		vPosition += XMVector3Normalize(vLook) * m_Monster_Bullet_UniversalDesc.fSpeedPerSec * (_float)dDeltaTime;

		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, vPosition);
	}
	return S_OK;
}

HRESULT CMonster_Bullet_Universal::Gadasura_Black(_double dDeltaTime)
{
	//if (m_bIsFired == false)
	//{
	//	_float4x4 fWorldMatrix = m_pTransformCom->Get_WorldFloat4x4();

	//	fWorldMatrix = XMMatrixIdentity();

	//	m_pTransformCom->Set_Matrix(XMLoadFloat4x4(&fWorldMatrix));


	//	m_pTransformCom->Scaled_All(m_Monster_Bullet_UniversalDesc.fScale);

	//	m_bIsFired = true;
	//}

	m_pTransformCom->LookDir(XMVectorSet(1.f, 0.f, 0.f, 0.f));
	
	return S_OK;
}

HRESULT CMonster_Bullet_Universal::Ready_JYParticleDesc()
{
	CGameInstance* pInstance = g_pGameInstance;
	CUtilityMgr* pUtil = GetSingle(CUtilityMgr);

#pragma region DescCreate
	m_pJYTextureParticleTransform = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
	m_pJYMeshParticleTransform		= (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
	NULL_CHECK_RETURN(m_pJYTextureParticleTransform, E_FAIL);
	NULL_CHECK_RETURN(m_pJYMeshParticleTransform, E_FAIL);

	m_pJYTextureParticleTransform->Set_TurnSpeed(1);
	m_pJYMeshParticleTransform->Set_TurnSpeed(XMConvertToRadians(90));



#pragma  endregion 


#pragma region Init


	switch (m_Monster_Bullet_UniversalDesc.iBulletMeshNumber)
	{
	case Client::CMonster_Bullet_Universal::VAYUSURA_LEADER_BULLET:
		break;
	case Client::CMonster_Bullet_Universal::TEZABSURA_MINION_BULLET:
	{
		m_pTransformCom->Set_TurnSpeed(XMConvertToRadians(180));
		m_vecJYTextureParticleDesc[0].FollowingTarget = m_pTransformCom;
		pUtil->Create_TextureInstance(m_eNowSceneNum, m_vecJYTextureParticleDesc[0]);
		m_StartY = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS).y;
		m_vecJYTextureParticleDesc[0].FollowingTarget = nullptr;
	}
	break;
	case Client::CMonster_Bullet_Universal::TEZABSURA_PURPLE_DEFAULT_BULLET:
		break;
	case Client::CMonster_Bullet_Universal::TEZABSURA_PURPLE_PRIMARY_BULLET:
		break;
	case Client::CMonster_Bullet_Universal::TEZABSURA_BOMBER_DEFAULT_BULLET:
		break;
	case Client::CMonster_Bullet_Universal::TEZABSURA_BOMBER_HOWITZER_BULLET:
		break;
	case Client::CMonster_Bullet_Universal::TEZABSURA_LANDMINE_DEFAULT_BULLET:
		m_pTransformCom->Set_TurnSpeed(XMConvertToRadians(180));
		m_vecJYTextureParticleDesc[5].FollowingTarget = m_pTransformCom;
		pUtil->Create_TextureInstance(m_eNowSceneNum, m_vecJYTextureParticleDesc[5]);
		m_vecJYTextureParticleDesc[5].FollowingTarget = nullptr;

		break;
	case Client::CMonster_Bullet_Universal::TEZABSURA_LANDMINE_INSTALL:
		m_pTransformCom->Scaled_All(_float3(0.05f));
		m_vecJYTextureParticleDesc[2].FollowingTarget = m_pTransformCom;
		pUtil->Create_TextureInstance(m_eNowSceneNum, m_vecJYTextureParticleDesc[2]);
		m_vecJYTextureParticleDesc[2].FollowingTarget = nullptr;

		break;
	case Client::CMonster_Bullet_Universal::NINJASURA_KNIFE:
		break;
	case Client::CMonster_Bullet_Universal::GADASURA_SINKHOLE:
		break;
	case Client::CMonster_Bullet_Universal::MONSTER_BULLET_UNIVERSAL_END:
		break;
	default:
		break;
	}



#pragma  endregion

	return S_OK;
}

HRESULT CMonster_Bullet_Universal::Update_JYParticleTransform(_double fDeltaTime)
{
	switch (m_Monster_Bullet_UniversalDesc.iBulletMeshNumber)
	{
	case Client::CMonster_Bullet_Universal::VAYUSURA_LEADER_BULLET:
		break;
	case Client::CMonster_Bullet_Universal::TEZABSURA_MINION_BULLET:

		break;
	case Client::CMonster_Bullet_Universal::TEZABSURA_PURPLE_DEFAULT_BULLET:
		break;
	case Client::CMonster_Bullet_Universal::TEZABSURA_PURPLE_PRIMARY_BULLET:
		break;
	case Client::CMonster_Bullet_Universal::TEZABSURA_BOMBER_DEFAULT_BULLET:
		break;
	case Client::CMonster_Bullet_Universal::TEZABSURA_BOMBER_HOWITZER_BULLET:
		break;
	case Client::CMonster_Bullet_Universal::TEZABSURA_LANDMINE_DEFAULT_BULLET:
		break;
	case Client::CMonster_Bullet_Universal::TEZABSURA_LANDMINE_INSTALL:
		break;
	case Client::CMonster_Bullet_Universal::NINJASURA_KNIFE:
		break;
	case Client::CMonster_Bullet_Universal::GADASURA_SINKHOLE:
		break;
	case Client::CMonster_Bullet_Universal::MONSTER_BULLET_UNIVERSAL_END:
		break;
	default:
		break;
	}



	return S_OK;
}

CMonster_Bullet_Universal * CMonster_Bullet_Universal::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CMonster_Bullet_Universal*	pInstance = NEW CMonster_Bullet_Universal(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CMonster_Bullet_Universal");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMonster_Bullet_Universal::Clone(void * pArg)
{
	CMonster_Bullet_Universal*	pInstance = NEW CMonster_Bullet_Universal(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CMonster_Bullet_Universal");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMonster_Bullet_Universal::Free()
{

	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
	Safe_Release(m_pColliderCom);


	Safe_Release(m_pJYTextureParticleTransform);
	Safe_Release(m_pJYMeshParticleTransform);
}