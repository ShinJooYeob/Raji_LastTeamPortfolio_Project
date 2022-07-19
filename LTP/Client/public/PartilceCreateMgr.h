#pragma once

#include "Base.h"
#include "NonInstanceMeshEffect.h"


BEGIN(Client)

// J_WHAN Effect Creater
typedef struct tag_MESHADDDATA
{
	eFollowingDirID LookRotAxis = FollowingDir_Look;
	_float AccRotSpeed = 0.0f;
	_float3 vAddDirectAngle = _float3::Zero();


}MESHADDDATA;

class CPartilceCreateMgr final :public CBase
{
	DECLARE_SINGLETON(CPartilceCreateMgr)
public:



public:
	enum E_MESH_EFFECTJ
	{
		MESHEFFECT_PRE_CONMESH,
		MESHEFFECT_PRE_CIRCLE,
		MESHEFFECT_PRE_CIRCLE_DIST4,
		MESHEFFECT_PRE_CIRCLE_DIST5,
		MESHEFFECT_PRE_IMPECTFX_02,
		MESHEFFECT_PRE_RING,
		MESHEFFECT_PRE_LOVE,
		MESHEFFECT_PRE_WING,
		MESHEFFECT_PRE_BOW1,
		MESHEFFECT_PRE_BOW2,
		MESHEFFECT_PRE_ICE,
		MESHEFFECT_PRE_END,

		
		MESHEFFECT_ARROW_HEAD,
		MESHEFFECT_ARROW_END,
		MESHEFFECT_ARROW_WING,
		MESHEFFECT_ARROW_BOW1,
		MESHEFFECT_ARROW_BOW_UP,
		MESHEFFECT_ARROW_BOW_SHIFT_FLOOR,
		MESHEFFECT_ARROW_BOW_SHIFT_ICE,
		MESHEFFECT_ARROW_BOW_R,
		MESHEFFECT_ARROW_BOW_R_JUMP_WING1,
		MESHEFFECT_ARROW_BOW_R_JUMP_WING2,


		MESHEFFECT_END,

	};

	enum E_TEXTURE_EFFECTJ
	{
		TEXTURE_EFFECTJ_Bow_Default,
		TEXTURE_EFFECTJ_Bow_ArrowHit,
		TEXTURE_EFFECTJ_Bow_Bow_ArrowTrail,
		TEXTURE_EFFECTJ_Bow_Charze_ArrowHead,
		TEXTURE_EFFECTJ_Bow_Charze_Circle,
		TEXTURE_EFFECTJ_Bow_Charze_Dash,
		TEXTURE_EFFECTJ_Bow_Charze_Long,
		TEXTURE_EFFECTJ_Bow_Charze_Suck,
		TEXTURE_EFFECTJ_END,

	};

private:
	explicit CPartilceCreateMgr();
	virtual ~CPartilceCreateMgr() = default;


public:
	HRESULT Initialize_ParticleMgr();


	NONINSTNESHEFTDESC Get_TypeDesc_NonInstacne(E_MESH_EFFECTJ e)
	{
		if (e >= MESHEFFECT_END)
			return NONINSTNESHEFTDESC();
		return mVecMeshEffectDesc[e];
	}

	INSTPARTICLEDESC Get_TypeDesc_TextureInstance(E_TEXTURE_EFFECTJ e)
	{
		if (e >= TEXTURE_EFFECTJ_END)
			return INSTPARTICLEDESC();
		return mVecTextureEffectDesc[e];
	}

public:
	HRESULT Update_EffectMgr(_double Timer);


	//HRESULT ReadyParticleData_TextureInstacnce();
	//HRESULT ReadyParticleData_MeshInstance();

	// TextureEffect
	HRESULT Create_Texture_Effect(E_TEXTURE_EFFECTJ type, CTransform * parentTransform);
	HRESULT Create_Texture_Effect_World(E_TEXTURE_EFFECTJ type, _float3 worldPos);

	HRESULT Create_Texture_Effect_Desc(INSTPARTICLEDESC desc, _uint scene);

	



	// MeshEffect

	HRESULT Create_MeshEffect(E_MESH_EFFECTJ type, CTransform * parentTransform, _float3 Offset);
	HRESULT Create_MeshEffect_World(E_MESH_EFFECTJ type, _float3 Postion, _float3 LookDir);

	HRESULT Create_MeshEffectDesc(NONINSTNESHEFTDESC desc, MESHADDDATA desc2, CTransform * parentTransform);

	HRESULT Create_MeshEffectDesc_Hard(E_MESH_EFFECTJ type, CTransform* Transfomr = nullptr);

	HRESULT Update_MeshEffect(_double timer);
	HRESULT Remove_MeshEffect(enum MeshEffect);

	HRESULT Clear_MeshEffect();


private:
	HRESULT Ready_MeshEffect();
	HRESULT Ready_TextureEffect();


private:
	vector<NONINSTNESHEFTDESC>	mVecMeshEffectDesc;
	vector<INSTPARTICLEDESC>	mVecTextureEffectDesc;

public:
	virtual void Free()override;

};

END
/*
HRESULT CPlayerWeapon_Bow::Ready_ParticleDesc()
{
	// 파티클용 Transform Create
	m_pTextureParticleTransform = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
	m_pMeshParticleTransform = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
	NULL_CHECK_RETURN(m_pTextureParticleTransform, E_FAIL);
	NULL_CHECK_RETURN(m_pMeshParticleTransform, E_FAIL);

	CUtilityMgr* pUtil = GetSingle(CUtilityMgr);

	// Bow_Default Bow_Charze Bow_Charze_ArrowHead Bow_ArrowTrail Bow_ArrowEnter

	_uint num = 0;
	m_vecTextureParticleDesc.push_back(pUtil->Get_TextureParticleDesc(L"Bow_Default"));
	m_vecTextureParticleDesc[num].FollowingTarget = nullptr;

	num = 1;
	m_vecTextureParticleDesc.push_back(pUtil->Get_TextureParticleDesc(L"Bow_Charze"));
	m_vecTextureParticleDesc[num].FollowingTarget = m_pTextureParticleTransform;

	num = 2;
	m_vecTextureParticleDesc.push_back(pUtil->Get_TextureParticleDesc(L"Bow_Default"));
	m_vecTextureParticleDesc[num].FollowingTarget = nullptr;

	return S_OK;
}

void CPlayerWeapon_Bow::Update_ParticleTransform(_double fDeltaTime)
{
	// 본체 위치에 업데이트

	_Matrix mat = m_pTransformCom->Get_WorldMatrix()  * m_tPlayerWeaponDesc.eAttachedDesc.Caculate_AttachedBoneMatrix();

	mat.r[0] = XMVector3Normalize(mat.r[0]);
	mat.r[1] = XMVector3Normalize(mat.r[1]);
	mat.r[2] = XMVector3Normalize(mat.r[2]);
	_Vector vPos = mat.r[3];

	m_pTextureParticleTransform->Set_MatrixState(CTransform::STATE_POS, vPos);

	// 활 앞 뒤 세팅
	mat.r[3] = vPos - (mat.r[2] * 0.2f + mat.r[0] * 0.03f + mat.r[1] * 0.03f);
	m_vecTextureParticleDesc[0].vFixedPosition = mat.r[3];

	mat.r[3] = vPos + (mat.r[2] * 0.65f + mat.r[0] * 0.05f + mat.r[1] * 0.05f);
	m_vecTextureParticleDesc[2].vFixedPosition = mat.r[3];


	for (auto& timer :m_fPlayParticleTimer)
	{
		timer -= fDeltaTime;
		if (timer <= -100)
			timer = -1;
	}

}

HRESULT CPlayerWeapon_Bow::Set_Play_Particle(_uint ParticleIndex,_float Timer,_float3 offset)
{

	if (PARTILCECOUNT <= ParticleIndex)
		return E_FAIL;

	if (m_vecTextureParticleDesc.size() <= ParticleIndex)
		return E_FAIL;

//	m_vecTextureParticleDesc[ParticleIndex].

	if (m_fPlayParticleTimer[ParticleIndex] <= 0.0f)
	{
		FAILED_CHECK(GetSingle(CUtilityMgr)->Create_TextureInstance(m_eNowSceneNum, m_vecTextureParticleDesc[ParticleIndex]));
		if (Timer == -1)
		{
			m_fPlayParticleTimer[ParticleIndex] = m_vecTextureParticleDesc[ParticleIndex].TotalParticleTime;
		}
		else
			m_fPlayParticleTimer[ParticleIndex] = Timer;
	}

	return S_OK;

}

HRESULT CPlayerWeapon_Bow::Set_PlayOff_ALL()
{
	for (_uint i = 0; i < PARTILCECOUNT; ++i)
	{
		ZeroMemory(m_fPlayParticleTimer, sizeof(_float) * PARTILCECOUNT);
	}

	m_pTextureParticleTransform->Set_IsOwnerDead(true);
	return S_OK;

}

*/




