#include "stdafx.h"
#include "..\public\DonkeyKong_Bullet.h"
#include "PartilceCreateMgr.h"
#include "MiniGame_KongGolu.h"


CDonkeyKong_Bullet::CDonkeyKong_Bullet(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CDonkeyKong_Bullet::CDonkeyKong_Bullet(const CDonkeyKong_Bullet & rhs)
	: CGameObject(rhs)
{
}


HRESULT CDonkeyKong_Bullet::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CDonkeyKong_Bullet::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());

	if (pArg != nullptr)
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, *((_float3*)pArg));



	//m_pNavigationCom->FindCellIndex(m_pTransformCom->Get_MatrixState(CTransform::STATE_POS));


	SetUp_Info();

	m_pTransformCom->Rotation_CW(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(90));
	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, XMVectorSet(33.033f, 45.49f, 40.f, 1.f));
	m_pTransformCom->Scaled_All(_float3(0.3f, 0.3f, 0.3f));


	//씬에 있는 레이어 순서 확인할것!
	m_pKongGolu = static_cast<CMiniGame_KongGolu*>(g_pGameInstance->Get_GameObject_By_LayerIndex(m_eNowSceneNum, TAG_LAY(Layer_MiniGame_DonkeyKong)));

	return S_OK;
}

_int CDonkeyKong_Bullet::Update(_double dDeltaTime)
{
	if (__super::Update(dDeltaTime) < 0)return -1;

	Play_MiniGame(dDeltaTime);


	m_bIsOnScreen = g_pGameInstance->IsNeedToRender(m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS), m_fFrustumRadius);

	Update_Collider(dDeltaTime);

	return _int();
}

_int CDonkeyKong_Bullet::LateUpdate(_double dDeltaTime)
{
	if (__super::LateUpdate(dDeltaTime) < 0)return -1;

	if (m_bIsOnScreen)
	{
		FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	}

	FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_NONANIMMODEL, this, m_pTransformCom, m_pShaderCom, m_pModel, nullptr, m_pDissolve));

#ifdef _DEBUG
	FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pColliderCom));
#endif

	return _int();
}

_int CDonkeyKong_Bullet::Render()
{
	if (__super::Render() < 0)
		return -1;

	NULL_CHECK_RETURN(m_pModel, E_FAIL);

	CGameInstance* pInstance = GetSingle(CGameInstance);
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));

	FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));

	//_uint iNumMaterials = m_pModel->Get_NumMaterial();

	//for (_uint i = 0; i < iNumMaterials; ++i)
	//{
	//	m_pModel->Bind_OnShader(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture");

	//	m_pModel->Render(m_pShaderCom, 3, i);
	//}

	FAILED_CHECK(m_pDissolve->Render(3)); //디졸브 내부에서 밑의 머테리얼을 찾아주고 있음


	return _int();
}

_int CDonkeyKong_Bullet::LateRender()
{
	return _int();
}

void CDonkeyKong_Bullet::Set_IsDead()
{
	__super::Set_IsDead();

	//g_pGameInstance->Play3D_Sound(TEXT("EH_DonkeyKong_Bullet_Delete.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.3f);

	if(m_pKongGolu->Get_bClear() == false)
		g_pGameInstance->PlaySound(TEXT("EH_DonkeyKong_Bullet_Delete.wav"), CHANNELID::CHANNEL_PLAYER, 0.3f);

	// Fragment
	INSTMESHDESC ParticleMesh = GETPARTICLE->Get_EffectSetting_Mesh(CPartilceCreateMgr::E_MESHINST_EFFECTJ::Um_MeshBase4_TurnAuto,
		Prototype_Mesh_SM_4E_IceShards_01, //FBX
		0.01f, //파티클 전체의 지속시간 한번만 재생시키기 위해 짧음
		0.3f, //파티클 지속시간
		_float4(0.28f, 0.29f, 0.95f, 0.f), //색깔1
		_float4(0), //색깔2 색깔1~2끼리 움직이면서 함 즉, 바꾸지 않게 하려면 같은 색을 넣고
		1, //여기에 1을 넣으면 됨
		_float3(2), //사이즈
		_float3(1.f), //사이즈2 이것도 위에랑 마찬가지
		1);
	ParticleMesh.eParticleTypeID = InstanceEffect_Ball; //퍼지는 타입
	ParticleMesh.eInstanceCount = Prototype_ModelInstance_32; //인스턴스 갯수
	ParticleMesh.ePassID = MeshPass_BrightColor; //노이즈

												 //범위
	_float val = 0.0f;
	ParticleMesh.ParticleStartRandomPosMin = _float3(-val, -0, -val);
	ParticleMesh.ParticleStartRandomPosMax = _float3(val, -0, val);

	//디퓨즈 텍스쳐
	ParticleMesh.TempBuffer_0.w = 398;

	ParticleMesh.iMaskingTextureIndex = 122;
	ParticleMesh.iMaskingTextureIndex = NONNMASK;//노이즈 타입이 아니면 동작하지 않음
	ParticleMesh.iNoiseTextureIndex = 289;
	ParticleMesh.vEmissive_SBB = _float3(0.f, 0.f, 0.f);
	ParticleMesh.Particle_Power = 10.0f;

	ParticleMesh.SubPowerRandomRange_RUL = _float3(1, 1, 1);
	ParticleMesh.fRotSpeed_Radian = XMConvertToRadians(max(1080, 0));


	ParticleMesh.TempBuffer_0.z = 1; //한번에 파티클이 생성됨

	//위치지정
	_Matrix mat = m_pTransformCom->Get_WorldMatrix();
	_Vector pos = mat.r[3];
	ParticleMesh.vFixedPosition = pos;

	//위치지정
	// ParticleMesh.FollowingTarget = m_pTransformCom;
	// ParticleMesh.iFollowingDir = FollowingDir_Up;

	GETPARTICLE->Create_MeshInst_DESC(ParticleMesh, m_eNowSceneNum);

	ParticleMesh.TempBuffer_0.w = 404;
	GETPARTICLE->Create_MeshInst_DESC(ParticleMesh, m_eNowSceneNum);

	ParticleMesh.TempBuffer_0.w = 405;
	GETPARTICLE->Create_MeshInst_DESC(ParticleMesh, m_eNowSceneNum);
}

void CDonkeyKong_Bullet::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	//Take_Damage만 호출해줄려고 인자값에 아무거나 때려박음
	pConflictedObj->Take_Damage(this,1,XMVectorSet(0.f,0.f,0.f,0.f),true,1.f);
}

_float CDonkeyKong_Bullet::Take_Damage(CGameObject * pTargetObject, _float fDamageAmount, _fVector vDamageDir, _bool bKnockback, _float fKnockbackPower)
{
	//m_pHPUI->Set_ADD_HitCount((_int)fDamageAmount);
	//m_fHP += -fDamageAmount;


	if (0 >= m_fHP)
	{
		return -1.f;
	}

	return _float();
}

HRESULT CDonkeyKong_Bullet::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VNAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	//FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_MONSTER_BULLET(Prototype_Mesh_Monster_Bullet_Tezabsura_Landmine), TAG_COM(Com_Model), (CComponent**)&m_pModel));
	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_PM_PowerUpFood), TAG_COM(Com_Model), (CComponent**)&m_pModel));
	CTransform::TRANSFORMDESC tDesc = {};

	tDesc.fMovePerSec = 5;
	tDesc.fRotationPerSec = XMConvertToRadians(60);
	tDesc.vPivot = _float3(0, 0, 0);
	tDesc.fScalingPerSec = 1;

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));

	CDissolve::DISSOLVEDESC DissolveDesc;
	DissolveDesc.pModel = m_pModel;
	DissolveDesc.eDissolveModelType = CDissolve::DISSOLVE_NONANIM;
	DissolveDesc.pShader = m_pShaderCom;
	DissolveDesc.RampTextureIndex = 4;
	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Dissolve), TAG_COM(Com_Dissolve), (CComponent**)&m_pDissolve, &DissolveDesc));


	CNavigation::NAVIDESC NaviDesc;
	NaviDesc.iCurrentIndex = 0;
	if (FAILED(__super::Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Navigation), TAG_COM(Com_Navaigation), (CComponent**)&m_pNavigationCom, &NaviDesc)))
		return E_FAIL;


	SetUp_Collider();

	return S_OK;
}

HRESULT CDonkeyKong_Bullet::SetUp_Info()
{
	return S_OK;
}

HRESULT CDonkeyKong_Bullet::Play_MiniGame(_double dDeltaTime)
{
	Play_Bullet(dDeltaTime);

	return S_OK;
}

HRESULT CDonkeyKong_Bullet::Play_Bullet(_double dDeltaTime)
{
	m_pNavigationCom->Set_CurNavCellIndex(m_iNaviIndex);
	m_fNaviHeight = m_pNavigationCom->Get_NaviHeight(m_pTransformCom->Get_MatrixState(CTransform::STATE_POS));
	m_fMyPos = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS);

	if (m_fNaviHeight + 0.3f <= m_fMyPos.y && m_bMovoToHeightOn == true)
	{
		//m_pTransformCom->Move_Down(dDeltaTime);
		m_pTransformCom->MovetoDir(XMVectorSet(0.f, -1.f, 0.f, 0.f), dDeltaTime,m_pNavigationCom);

		if (m_iSoundIndex == 0 && m_pKongGolu->Get_bClear() == false)
		{
			//g_pGameInstance->Play3D_Sound(TEXT("EH_DonkeyKong_Descent.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.3f);
			g_pGameInstance->PlaySound(TEXT("EH_DonkeyKong_Descent.wav"), CHANNELID::CHANNEL_PLAYER, 0.3f);
			m_iSoundIndex++;
		}
	}
	else {
		m_bMoveToWidthOn = true;
		m_iSoundIndex = 0;
	}



	if (m_iMoveToDirIndex == RIGHT && m_bMoveToWidthOn == true)
	{
		m_pTransformCom->MovetoDir(XMVectorSet(1.f, 0.f, 0.f, 0.f),dDeltaTime,m_pNavigationCom);
		m_bMovoToHeightOn = false;

		_Vector vTempPos;
		vTempPos = m_pNavigationCom->Get_NaviPosition(m_pTransformCom->Get_MatrixState(CTransform::STATE_POS));
		XMStoreFloat3(&m_fPivotPos, vTempPos);
		m_fPivotPos.y += 0.5;
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, m_fPivotPos);

		m_pTransformCom->Turn_CCW(XMVectorSet(0.f, 0.f, 1.f, 0.f), dDeltaTime*4.235801032);
	}
	else if (m_iMoveToDirIndex == LEFT && m_bMoveToWidthOn == true)
	{
		m_pTransformCom->MovetoDir(XMVectorSet(-1.f, 0.f, 0.f, 0.f), dDeltaTime, m_pNavigationCom);
		m_bMovoToHeightOn = false;

		_Vector vTempPos;
		vTempPos = m_pNavigationCom->Get_NaviPosition(m_pTransformCom->Get_MatrixState(CTransform::STATE_POS));
		XMStoreFloat3(&m_fPivotPos, vTempPos);
		m_fPivotPos.y += 0.5;
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, m_fPivotPos);

		m_pTransformCom->Turn_CW(XMVectorSet(0.f, 0.f, 1.f, 0.f), dDeltaTime*4.235801032);
	}
	

	Play_Random(dDeltaTime);

	return S_OK;
}

HRESULT CDonkeyKong_Bullet::Play_Collider(_double dDeltaTime)
{
	return S_OK;
}

HRESULT CDonkeyKong_Bullet::Play_Random(_double dDeltaTime)
{
	if (m_iCurColliderIndex != m_iNewColliderIndex)
	{
		_uint iRandom;

		iRandom = rand() % 2;

		if (iRandom == 0)
		{
			m_bDescent = true;
		}
		else {
			m_bDescent = false;
		}

		m_iCurColliderIndex = m_iNewColliderIndex;
	}
	return S_OK;
}

HRESULT CDonkeyKong_Bullet::SetUp_Collider()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider), (CComponent**)&m_pColliderCom));


	COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(2.5f, 2.5f, 2.5f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));



	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(2.f, 2.f, 2.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	return S_OK;
}

HRESULT CDonkeyKong_Bullet::Update_Collider(_double dDeltaTime)
{
	m_pColliderCom->Update_ConflictPassedTime(dDeltaTime);

	//Collider
	_uint	iNumCollider = m_pColliderCom->Get_NumColliderBuffer();
	for (_uint i = 0; i < iNumCollider; i++)
		m_pColliderCom->Update_Transform(i, m_pTransformCom->Get_WorldMatrix());

	FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_Monster, this, m_pColliderCom));
	FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_MonsterWeapon, this, m_pColliderCom));

	return S_OK;
}

CDonkeyKong_Bullet * CDonkeyKong_Bullet::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CDonkeyKong_Bullet*	pInstance = NEW CDonkeyKong_Bullet(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CDonkeyKong_Bullet");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CDonkeyKong_Bullet::Clone(void * pArg)
{
	CDonkeyKong_Bullet*	pInstance = NEW CDonkeyKong_Bullet(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CDonkeyKong_Bullet");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CDonkeyKong_Bullet::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
	Safe_Release(m_pDissolve);

	Safe_Release(m_pColliderCom);
	Safe_Release(m_pNavigationCom);
}
