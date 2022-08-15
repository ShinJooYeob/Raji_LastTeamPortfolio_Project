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
		MESHEFFECT_MONSTER_GL_Attack2,

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
			MESHEFFECT_MONSTER_GL_Cash3,


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

		// Ohter
		JY_TextureEft_1,
		JY_TextureEft_2,
		JY_TextureEft_3,
		JY_TextureEft_4,
		JY_TextureEft_5,
		JY_TextureEft_6,
		JY_TextureEft_7,
		JY_TextureEft_8,

		JY_TextureEft_9,
		JY_TextureEft_10,
		JY_TextureEft_11,
		JY_TextureEft_12,
		JY_TextureEft_13,
		JY_TextureEft_14,
		JY_TextureEft_15,
		JY_TextureEft_16,
		JY_TextureEft_17,
		JY_TextureEft_18,
		JY_TextureEft_19,
		JY_TextureEft_20,
		JY_TextureEft_21,
		JY_TextureEft_22,
		JY_TextureEft_23,
		JY_TextureEft_24,
		JY_TextureEft_25,


		Spear_ThrowAttack,
		SpearNormalAttack,

		// My
		Um_Dust_1,
		Um_Dust_2,
		Um_Dust_2_FounTain,
		Um_Dust_3,
		Um_FireMask_1,
		Um_FireMask_2,
		Um_FireMask_2_png,
		Um_FireMask_3,
		Um_Hit_1,
		Um_Hit_2,
		Um_Hit_2_DisDiffuse,
		Um_Hit_3,
		Um_Hit_4,
		Um_Imgae_1,
		Um_Imgae_2,
		Um_Roll_1,
		Um_Sunder_1,
		Um_Up_1,

		Um_Spawn1_Image,
		Um_Spawn1_Image_suck,
		Um_Spawn2_Image,
		Um_Spawn2_Image_power,
		Um_Spawn2_Image_powerdown,
		Um_Spawn3_Imagepng,
		Um_Spawn3_Imagepng_ground,
		Um_Spawn3_Imagepng_ground2,
		Um_Spawn3_Imagepng_magic,
		Um_Spawn3_Imagepng_magic2,
		Um_Spawn3_Imagepng_Snow,
		Um_Spawn3_Imagepng_Snow2,
		Um_Spawn4_smoke,
		Um_Spawn5_Png_Ring,


		TEXTURE_EFFECTJ_END,

	};

	enum E_MESHINST_EFFECTJ
	{
		MESHINST_EFFECTJ_BOW_Q_ICE,
		MESHINST_EFFECTJ_BOW_Q_ICE2,
		MESHINST_EFFECTJ_BOW_Q_PLANE,

		Um_MeshBase,
		Um_MeshBase2,
		Um_MeshBase3,
		Um_MeshBase_Cone,
		Um_Mesh_Sword1,
		Um_Mesh_Sword2,
		Um_Mesh_MaskApper,
		Um_MeshBase4,
		Um_MeshBase4_NoOnetime,
		Um_MeshBase4_TurnAuto,
		Um_MeshBase4_TurnRight,


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

	INSTPARTICLEDESC Get_EffectSetting_Tex(E_TEXTURE_EFFECTJ e,
		_float TotalTime, _float EachTime,
		_float4 Color1, _float4 Color2, _uint colorFrequency,
		_float3 Size1, _float3 Size2, _uint sizeFrequency
		);

	void Set_CreatBound_Tex(INSTPARTICLEDESC& tex,_float3 Min, _float3 Max);

	// Meshinst
	HRESULT Create_MeshInst_Effect(E_MESHINST_EFFECTJ type, CTransform * parentTransform);
	HRESULT Create_MeshInst_Effect_World(E_MESHINST_EFFECTJ type, _float3 worldPos);
	HRESULT Create_MeshInst_DESC(INSTMESHDESC desc, _uint scene);

	INSTMESHDESC Get_EffectSetting_Mesh(E_MESHINST_EFFECTJ e,
		COMPONENTPROTOTYPEID meshType = COMPONENTPROTOTYPEID(0),
		_float TotalTime=0, _float EachTime=0,
		_float4 Color1 = _float4(1), _float4 Color2 = _float4(1), _uint colorFrequency = 0,
		_float3 Size1 = _float3(1), _float3 Size2 = _float3(1), _uint sizeFrequency=0
	);


	// MeshEffect

	HRESULT Create_MeshEffect(E_MESH_EFFECTJ type, CTransform * parentTransform, _float3 Offset);
	HRESULT Create_MeshEffect_World(E_MESH_EFFECTJ type, _float3 Postion, _float3 LookDir);


	HRESULT Create_MeshEffectDesc(NONINSTNESHEFTDESC desc, MESHADDDATA desc2, CTransform * parentTransform, MESHAEASING* easing = nullptr,_uint Count = 0);

	HRESULT Create_MeshEffectDesc_Hard(E_MESH_EFFECTJ type, CTransform* Transfomr = nullptr);

	HRESULT Update_MeshEffect(_double timer);
	HRESULT Remove_MeshEffect(enum MeshEffect);

	HRESULT Clear_MeshEffect();

	const class CNonInstanceMeshEffect_TT* GetMeshEffect() const;


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

	class CNonInstanceMeshEffect_TT*	mPreMeshEffect = nullptr;



public:
	virtual void Free()override;

};

END
