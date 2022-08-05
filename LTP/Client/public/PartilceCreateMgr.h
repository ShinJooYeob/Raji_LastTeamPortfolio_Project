#pragma once

#include "Base.h"
#include "NonInstanceMeshEffect.h"

BEGIN(Engine)
class CTransform;
END
BEGIN(Client)

// J_WHAN Effect Creater
typedef struct tag_MESHADDDATA
{
	eFollowingDirID LookRotAxis = FollowingDir_Look;
	_float AccRotSpeed = 0.0f;
	_float AccMoveSpeed = 0.0f;
	_float AccScaleSpeed = 0.0f;
	_float ScaleMax = 0.0f;
	_float3 vAddDirectAngle = _float3::Zero();
	_float  LookRotSpeed = 0;
	_bool FixFlag_Move = false;
	_bool FixFlag_Rot = false;

	_double DealyTime = 0;

	_float3 InitRot= _float3::Zero();
	_bool ScaleReFlag = false; // 스케일이 0이 되어야사라짐
	_bool bLockScale[3] = { false, };
	_bool bAfterApperTime = false; // 생성되면 사라짐
	CTransform* FollowTarget = nullptr;


}MESHADDDATA;

typedef struct tag_EASINGDESC
{
//	_float3 StartPos = _float3();
	_float3 EndPos = _float3();
	_float	MaxTime = 1.0f;
	EasingTypeID EasingID = TYPE_Linear;

}MESHAEASING;


class CPartilceCreateMgr final :public CBase
{
	DECLARE_SINGLETON(CPartilceCreateMgr)
public:



public:
	enum E_MESH_EFFECTJ
	{
#pragma region PLAYER MESH

		// PLAY EFFECT
		MESHEFFECT_ARROW_HEAD,
		MESHEFFECT_ARROW_END,
		MESHEFFECT_ARROW_WING,
		MESHEFFECT_ARROW_BOW1,
		MESHEFFECT_ARROW_BOW_UP,
		MESHEFFECT_ARROW_BOW_SHIFT_PLANE,
		MESHEFFECT_ARROW_BOW_SHIFT_ICE,
		MESHEFFECT_ARROW_BOW_R,
		MESHEFFECT_ARROW_BOW_R_JUMP_WING1,
		MESHEFFECT_ARROW_BOW_R_JUMP_WING2,
		MESHEFFECT_ARROW_BOW_SP_MOVE_SPEHERE,
		MESHEFFECT_ARROW_BOW_SP_BOM_SPEHERE,
		MESHEFFECT_ARROW_BOW_SP_PLANE,
		MESHEFFECT_ARROW_BOW_SP_PLANE2,
		MESHEFFECT_ARROW_BOW_SP_PLANE3,
		MESHEFFECT_ARROW_BOW_SP_TON,
		MESHEFFECT_ARROW_BOW_SP_ICES,
		MESHEFFECT_ARROW_BOW_SP_BOW,

#pragma endregion PLAYER MESH



#pragma region MONSTER
		MESHEFFECT_MONSTER_TEST,
		MESHEFFECT_MONSTER_TEST2,

		// Monster EFFECT
		MESHEFFECT_MONSTER_CREATE1,
		MESHEFFECT_MONSTER_CREATE2,
		MESHEFFECT_MONSTER_CREATE3,
		MESHEFFECT_MONSTER_CREATE4,

		// M 일반 L 리더
		// ========================================

		MESHEFFECT_MONSTER_MM_TAIL,
		MESHEFFECT_MONSTER_MM_TAIL2,

		MESHEFFECT_MONSTER_MM_HAND_L,
		MESHEFFECT_MONSTER_MM_HAND_R,

		MESHEFFECT_MONSTER_MM_Cash0,
		MESHEFFECT_MONSTER_MM_Cash1,
		MESHEFFECT_MONSTER_MM_Cash2,



		// ========================================
		MESHEFFECT_MONSTER_ML_TAIL1,

		MESHEFFECT_MONSTER_ML_TAIL2,
		MESHEFFECT_MONSTER_ML_TAIL3,
		MESHEFFECT_MONSTER_ML_CASH5,

		MESHEFFECT_MONSTER_ML_HAND_L,
		MESHEFFECT_MONSTER_ML_HAND_R,

		MESHEFFECT_MONSTER_ML_CASH1,
		MESHEFFECT_MONSTER_ML_CASH2,
		MESHEFFECT_MONSTER_ML_CASH3,
		MESHEFFECT_MONSTER_ML_CASH4,


		MESHEFFECT_MONSTER_ML_CASH6,
		MESHEFFECT_MONSTER_ML_CASH7,
		MESHEFFECT_MONSTER_ML_CASH8,

		// ========================================
		MESHEFFECT_MONSTER_GM_ATT0,
		MESHEFFECT_MONSTER_GM_ATT1,

		MESHEFFECT_MONSTER_GM_SKILLRUN1,
		MESHEFFECT_MONSTER_GM_SKILLRUN0,

		MESHEFFECT_MONSTER_GM_SKILLSMASH0,
		MESHEFFECT_MONSTER_GM_SKILLSMASH1,
		MESHEFFECT_MONSTER_GM_SKILLSMASH2,


		MESHEFFECT_MONSTER_GM_SKILLBOUND0,
		MESHEFFECT_MONSTER_GM_SKILLBOUND1,

		MESHEFFECT_MONSTER_GM_Cash0,
		MESHEFFECT_MONSTER_GM_Cash1,
		MESHEFFECT_MONSTER_GM_Cash2,
		MESHEFFECT_MONSTER_GM_Cash3,
		MESHEFFECT_MONSTER_GM_Cash4,




		// ===========================================

		MESHEFFECT_MONSTER_GL_Attack1,
		MESHEFFECT_MONSTER_GL_Attack0,

		MESHEFFECT_MONSTER_GL_SKILLRUN0,
		MESHEFFECT_MONSTER_GL_SKILLRUN1,

		MESHEFFECT_MONSTER_GL_SKILLBOUND0,
		MESHEFFECT_MONSTER_GL_SKILLBOUND1,

		MESHEFFECT_MONSTER_GL_SKILLTRIPLE0,
		MESHEFFECT_MONSTER_GL_SKILLTRIPLE1,
		MESHEFFECT_MONSTER_GL_SKILLTRIPLE2,
		MESHEFFECT_MONSTER_GL_SKILLTRIPLE3,

		MESHEFFECT_MONSTER_GL_SKILLHOLOGRAM0,
		MESHEFFECT_MONSTER_GL_SKILLHOLOGRAM1,
		MESHEFFECT_MONSTER_GL_SKILLHOLOGRAM2,

		MESHEFFECT_MONSTER_GL_Cash0,
		MESHEFFECT_MONSTER_GL_Cash1,
		MESHEFFECT_MONSTER_GL_Cash2,


		// ===========================================

		MESHEFFECT_MONSTER_VM_Test,
		MESHEFFECT_MONSTER_VM_Plane,
		MESHEFFECT_MONSTER_VM_Cash2,
		MESHEFFECT_MONSTER_VM_Cash3,
		MESHEFFECT_MONSTER_VM_Cash4,
		MESHEFFECT_MONSTER_VM_Cash5,
		MESHEFFECT_MONSTER_VM_Cash6,
		MESHEFFECT_MONSTER_VM_Cash7,
		MESHEFFECT_MONSTER_VM_Cash8,
		MESHEFFECT_MONSTER_VM_Cash9,
		MESHEFFECT_MONSTER_VM_Cash10,

		// ===========================================

		MESHEFFECT_MONSTER_VL_Test,
		MESHEFFECT_MONSTER_VL_Cash0,
		MESHEFFECT_MONSTER_VL_Cash1,
		MESHEFFECT_MONSTER_VL_Cash2,
		MESHEFFECT_MONSTER_VL_Cash3,
		MESHEFFECT_MONSTER_VL_Cash4,
		MESHEFFECT_MONSTER_VL_Cash5,
		MESHEFFECT_MONSTER_VL_Cash6,
		MESHEFFECT_MONSTER_VL_Cash7,
		MESHEFFECT_MONSTER_VL_Cash8,
		MESHEFFECT_MONSTER_VL_Cash9,
		MESHEFFECT_MONSTER_VL_Cash10,
		// ===========================================

		MESHEFFECT_MONSTER_NM_Test,
		MESHEFFECT_MONSTER_NM_Plane,
		MESHEFFECT_MONSTER_NM_Cash2,
		MESHEFFECT_MONSTER_NM_Cash3,
		MESHEFFECT_MONSTER_NM_Cash4,
		MESHEFFECT_MONSTER_NM_Cash5,
		MESHEFFECT_MONSTER_NM_Cash6,
		MESHEFFECT_MONSTER_NM_Cash7,
		MESHEFFECT_MONSTER_NM_Cash8,
		MESHEFFECT_MONSTER_NM_Cash9,
		MESHEFFECT_MONSTER_NM_Cash10,
		// ===========================================

		MESHEFFECT_MONSTER_NL_Test,
		MESHEFFECT_MONSTER_NL_Cash0,
		MESHEFFECT_MONSTER_NL_Cash1,
		MESHEFFECT_MONSTER_NL_Cash2,
		MESHEFFECT_MONSTER_NL_Cash3,
		MESHEFFECT_MONSTER_NL_Cash4,
		MESHEFFECT_MONSTER_NL_Cash5,
		MESHEFFECT_MONSTER_NL_Cash6,
		MESHEFFECT_MONSTER_NL_Cash7,
		MESHEFFECT_MONSTER_NL_Cash8,
		MESHEFFECT_MONSTER_NL_Cash9,
		MESHEFFECT_MONSTER_NL_Cash10,



		//	MESHEFFECT_MONSTER_NM_ATT,
		//	MESHEFFECT_MONSTER_NL_ATT,

#pragma endregion MONSTER

#pragma region BOSS

		// BOSS


		MESHEFFECT_BOSS_SNAKE_0,
		MESHEFFECT_BOSS_SNAKE_1,
		MESHEFFECT_BOSS_SNAKE_2,
		MESHEFFECT_BOSS_SNAKE_3,
		MESHEFFECT_BOSS_SNAKE_4,
		MESHEFFECT_BOSS_SNAKE_5,
		MESHEFFECT_BOSS_SNAKE_6,
		MESHEFFECT_BOSS_SNAKE_7,
		MESHEFFECT_BOSS_SNAKE_8,
		MESHEFFECT_BOSS_SNAKE_9,


		MESHEFFECT_BOSS_Mahabalasura_SKILLSPEAR_0,
		MESHEFFECT_BOSS_Mahabalasura_SKILLSPEAR_1,
		MESHEFFECT_BOSS_Mahabalasura_SKILLSPEAR_2,
		MESHEFFECT_BOSS_Mahabalasura_SKILLHAND_0,
		MESHEFFECT_BOSS_Mahabalasura_SKILLRAIN_0,
		MESHEFFECT_BOSS_Mahabalasura_SKILLRAIN_1,
		MESHEFFECT_BOSS_Mahabalasura_SKILLRAIN_2,
		MESHEFFECT_BOSS_Mahabalasura_SKILLCOPY_0,
		MESHEFFECT_BOSS_Mahabalasura_SKILLCOPY_1,
		MESHEFFECT_BOSS_Mahabalasura_0,
		MESHEFFECT_BOSS_Mahabalasura_1,
		MESHEFFECT_BOSS_Mahabalasura_2,
		MESHEFFECT_BOSS_Mahabalasura_3,
		MESHEFFECT_BOSS_Mahabalasura_4,
		MESHEFFECT_BOSS_Mahabalasura_PLANE1,
		MESHEFFECT_BOSS_Mahabalasura_PLANE2,
		MESHEFFECT_BOSS_Mahabalasura_PLANE3,



		MESHEFFECT_BOSS_Rangda_0,
		MESHEFFECT_BOSS_Rangda_1,
		MESHEFFECT_BOSS_Rangda_2,
		MESHEFFECT_BOSS_Rangda_3,
		MESHEFFECT_BOSS_Rangda_4,
		MESHEFFECT_BOSS_Rangda_5,
		MESHEFFECT_BOSS_Rangda_6,
		MESHEFFECT_BOSS_Rangda_7,
		MESHEFFECT_BOSS_Rangda_8,
		MESHEFFECT_BOSS_Rangda_9,
		MESHEFFECT_BOSS_Rangda_10,
		MESHEFFECT_BOSS_Rangda_11,
		MESHEFFECT_BOSS_Rangda_12,



		MESHEFFECT_BOSS_Chiedtian_0,
		MESHEFFECT_BOSS_Chiedtian_1,
		MESHEFFECT_BOSS_Chiedtian_2,
		MESHEFFECT_BOSS_Chiedtian_3,
		MESHEFFECT_BOSS_Chiedtian_4,
		MESHEFFECT_BOSS_Chiedtian_5,
		MESHEFFECT_BOSS_Chiedtian_6,
		MESHEFFECT_BOSS_Chiedtian_7,
		MESHEFFECT_BOSS_Chiedtian_8,
		MESHEFFECT_BOSS_Chiedtian_9,

#pragma endregion BOSS


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
		TEXTURE_EFFECTJ_Bow_Shift_Image,
		TEXTURE_EFFECTJ_Bow_Q_Ball,
		TEXTURE_EFFECTJ_Bow_R_FlyFire,
		TEXTURE_EFFECTJ_Bow_R_FlyBall,

		TEXTURE_EFFECTJ_Universal_Ball,
		TEXTURE_EFFECTJ_Universal_Suck,
		TEXTURE_EFFECTJ_Universal_Spread,

		TEXTURE_EFFECTJ_END,

	};

	enum E_MESHINST_EFFECTJ
	{
		MESHINST_EFFECTJ_BOW_Q_ICE,
		MESHINST_EFFECTJ_BOW_Q_ICE2,
		MESHINST_EFFECTJ_BOW_Q_PLANE,
		MESHINST_EFFECTJ_END,
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
	INSTMESHDESC Get_TypeDesc_MeshInstance(E_MESHINST_EFFECTJ e)
	{
		if (e >= MESHINST_EFFECTJ_END)
			return INSTMESHDESC();
		return mVecMeshInstDesc[e];
	}


public:
	HRESULT Update_EffectMgr(_double Timer);


	//HRESULT ReadyParticleData_TextureInstacnce();
	//HRESULT ReadyParticleData_MeshInstance();

	// TextureEffect
	HRESULT Create_Texture_Effect(E_TEXTURE_EFFECTJ type, CTransform * parentTransform);
	HRESULT Create_Texture_Effect_World(E_TEXTURE_EFFECTJ type, _float3 worldPos);
	HRESULT Create_Texture_Effect_Desc(INSTPARTICLEDESC desc, _uint scene);


	// Meshinst
	HRESULT Create_MeshInst_Effect(E_MESHINST_EFFECTJ type, CTransform * parentTransform);
	HRESULT Create_MeshInst_Effect_World(E_MESHINST_EFFECTJ type, _float3 worldPos);
	HRESULT Create_MeshInst_DESC(INSTMESHDESC desc, _uint scene);


	// MeshEffect

	HRESULT Create_MeshEffect(E_MESH_EFFECTJ type, CTransform * parentTransform, _float3 Offset);
	HRESULT Create_MeshEffect_World(E_MESH_EFFECTJ type, _float3 Postion, _float3 LookDir);


	HRESULT Create_MeshEffectDesc(NONINSTNESHEFTDESC desc, MESHADDDATA desc2, CTransform * parentTransform, MESHAEASING* easing = nullptr,_uint Count = 0);

	HRESULT Create_MeshEffectDesc_Hard(E_MESH_EFFECTJ type, CTransform* Transfomr = nullptr);

	HRESULT Update_MeshEffect(_double timer);
	HRESULT Remove_MeshEffect(enum MeshEffect);

	HRESULT Clear_MeshEffect();


private:
	HRESULT			Ready_MeshEffect();
	HRESULT			Ready_TextureEffect();
	HRESULT			Ready_MeshInstanceEffect();
	
	
	MESHAEASING		CreateEasingDesc(EasingTypeID id, _float3 endpos, _float timemax);

	HRESULT Create_MeshEffectDesc_Hard_MONSTER(E_MESH_EFFECTJ type, CTransform* Transfom = nullptr);
	HRESULT Create_MeshEffectDesc_Hard_BOSS(E_MESH_EFFECTJ type, CTransform* Transfom = nullptr);


private:
	vector<NONINSTNESHEFTDESC>	mVecMeshEffectDesc;
	vector<INSTPARTICLEDESC>	mVecTextureEffectDesc;
	vector<INSTMESHDESC>		mVecMeshInstDesc;




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
