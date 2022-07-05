#include "stdafx.h"
#include "Monster_Bullet_Universal.h"
#include "Monster_Vayusura_Leader.h"

const _tchar* m_pMonster_Bullet_UniversalTag[CMonster_Bullet_Universal::MONSTER_BULLET_UNIVERSAL_END]
{
	TEXT("Monster_Bullet_Vayusura_Leader.fbx"),
	TEXT("Monster_Bullet_Tezabsura_Minion.fbx"),
	TEXT("Monster_Bullet_Tezabsura_Purple.fbx"),
	TEXT("Monster_Bullet_Tezabsura_Purple.fbx"),
	TEXT("Monster_Bullet_Tezabsura_Bomber.fbx"),
	TEXT("Monster_Bullet_Tezabsura_Bomber.fbx"),
	TEXT("Monster_Bullet_Tezabsura_Landmine.fbx"),
	TEXT("Monster_Bullet_Tezabsura_Landmine.fbx"),
	TEXT("Ninjasura_Knife.fbx")

};

CMonster_Bullet_Universal::CMonster_Bullet_Universal(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CMonster_Bullet(pDevice, pDeviceContext)
{
}

CMonster_Bullet_Universal::CMonster_Bullet_Universal(const CMonster_Bullet_Universal & rhs)
	: CMonster_Bullet(rhs)
{
}

HRESULT CMonster_Bullet_Universal::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


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

	return _int();
}

_int CMonster_Bullet_Universal::LateUpdate(_double dDeltaTime)
{
	if (__super::LateUpdate(dDeltaTime) < 0)return -1;

	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_NONANIMMODEL, this, m_pTransformCom, m_pShaderCom, m_pModel));

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


	m_bIsDead = true;
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
	default:
		MSGBOX("Not BulletMeshNumber");
		break;
	}
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
	_Vector vPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
	_Vector vLook = XMLoadFloat3(&m_fTempLook);

	vPosition += XMVector3Normalize(vLook) * m_Monster_Bullet_UniversalDesc.fSpeedPerSec * (_float)dDeltaTime;

	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, vPosition);

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
	_Vector vPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
	_Vector vLook = XMLoadFloat3(&m_fTempLook);

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
	_Vector vPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
	_Vector vLook = XMLoadFloat3(&m_fTempLook);

	vPosition += XMVector3Normalize(vLook) * m_Monster_Bullet_UniversalDesc.fSpeedPerSec * (_float)dDeltaTime;

	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, vPosition);

	return S_OK;
}

HRESULT CMonster_Bullet_Universal::Tezabsura_Landmine_Install(_double dDeltaTime)
{
	if (m_dBezierTime <= 1)
	{
		m_dBezierTime += dDeltaTime;
	}

	_Vector  CenterPos = (XMLoadFloat4(&m_fTempPos) - XMLoadFloat4(&m_fTempPlayerPos));

	CenterPos = XMLoadFloat4(&m_fTempPos) + (CenterPos * 0.1f); //베지어 곡선은 스타트 지점에 가까울수록 날카로운 곡선이 아닌 둥글 둥글한 포물선이 되는듯

	_float4 TempCenter;
	XMStoreFloat4(&TempCenter, CenterPos);

	TempCenter.y = TempCenter.y + 10;

	_Vector BezierPos = ((1 - (_float)m_dBezierTime)*(1 - (_float)m_dBezierTime)*XMLoadFloat4(&m_fTempPos)) + (2 * (_float)m_dBezierTime*(1 - (_float)m_dBezierTime) *XMLoadFloat4(&TempCenter)) + ((_float)m_dBezierTime* (_float)m_dBezierTime)*XMLoadFloat4(&m_fTempPlayerPos);

	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, BezierPos);
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

CMonster_Bullet_Universal * CMonster_Bullet_Universal::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CMonster_Bullet_Universal*	pInstance = new CMonster_Bullet_Universal(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CMonster_Bullet_Universal");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMonster_Bullet_Universal::Clone(void * pArg)
{
	CMonster_Bullet_Universal*	pInstance = new CMonster_Bullet_Universal(*this);

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
}