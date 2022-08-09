#include "stdafx.h"
#include "..\public\PartilceCreateMgr.h"
#include "..\public\NonInstanceMeshEffect_TT.h"

IMPLEMENT_SINGLETON(CPartilceCreateMgr);

// #define RIMCOLOR(x,x,x);

CPartilceCreateMgr::CPartilceCreateMgr()
{}

/*
//	Mesh Effect USE

	// USE LOAD
			STATIC_EFFECTLOAD(Prototype_Mesh_circle);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_Bow_Em_01);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_Bow_Em_02);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_circle);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_4E_IceSpike_01);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_Bow_Em_Wing_T);
		STATIC_EFFECTLOAD(Prototype_Mesh_mySphere);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_sphere_melon);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_CoreRing_3);
		STATIC_EFFECTLOAD(Prototype_Mesh_MS_ST3_Portal_00);
		STATIC_EFFECTLOAD(Prototype_Mesh_ice2);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_Ring_03);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_ky_windSmokeDist);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_L_DS_Dashcombo_Y_L);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_4E_ImpactFX_02);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_ice2);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_ICE_01);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_SS_Trail_02);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_ArrowBurstTrail_01_New);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_SpinningSlash_O_R_DX);
		STATIC_EFFECTLOAD(Prototype_Mesh_Sample_Mesh_Fake_Mesh_Lighting);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_DS_Rage_Buff_L_D);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_DS_Rage_Buff_L_X);
		STATIC_EFFECTLOAD(Prototype_Mesh_02_L_Upper_SM);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_ky_tornado3);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_06_06_SM002);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_SG_tornado);
		STATIC_EFFECTLOAD(Prototype_Mesh_mySphere);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_SP_End_X_L);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_SP_End_Z_R);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_Meteo_Sphere3);
		STATIC_EFFECTLOAD(Prototype_Mesh_MS_Shot_Cylinder_00);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_DW_Weapon);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_Baren_Weapon);
		STATIC_EFFECTLOAD(Prototype_Mesh_Mesh_Leap_Ground_Light);
		STATIC_EFFECTLOAD(Prototype_Mesh_circle_inv);
		STATIC_EFFECTLOAD(Prototype_Mesh_MS_Tornado_Cyl_2side);
		STATIC_EFFECTLOAD(Prototype_Mesh_Mesh_HFB_Grand_Git);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_4E_DarkTails_01);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_rockRing);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_EM_Copycat_03);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_ky_lans_rot);
		STATIC_EFFECTLOAD(Prototype_Mesh_doomBvert);
		STATIC_EFFECTLOAD(Prototype_Mesh_ConeMesh);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_Crack_OutLine);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_BBT_Paw);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_AOG_Ring);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_SS_Aura_01);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_Sphere_RPG);
		STATIC_EFFECTLOAD(Prototype_Mesh_MS_Trail_Twist_00);
		STATIC_EFFECTLOAD(Prototype_Mesh_Sample_Mesh_WP_Cylinder_Z);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_AXTrail_01);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_ky_sphere_dist_wind);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_DivineLaserBeam_02);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_TS_Basic_combo_EX2_Sword);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_LB_Invisibility_02);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_Helix_Crystal);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_TS_Double_Slash_01);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_GT_SphereHarf);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_GN_Wind01);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_DS_Basic_01_2_Trail_X);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_4E_LightningBolt_01);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_DS_Switching_L_D_Plane);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_IceTrap_Ice);
		STATIC_EFFECTLOAD(Prototype_Mesh_MS_Tornado_Cyl_2side_03);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_4E_IceShards_01);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_4E_IceShards_02);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_4E_IceShards_03);



		// Bullet
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_Monster_Bullet_Vayusura_Leader);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_Monster_Bullet_Tezabsura_Minion);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_Monster_Bullet_Tezabsura_Purple);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_Monster_Bullet_Tezabsura_Bomber);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_Monster_Bullet_Tezabsura_Landmine);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_Ninjasura_Knife);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_sinkhole);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_Gadasura_Terrain_Bullet);


	*/

HRESULT CPartilceCreateMgr::Initialize_ParticleMgr()
{
	FAILED_CHECK(Ready_MeshEffect());
	FAILED_CHECK(Ready_TextureEffect());
	FAILED_CHECK(Ready_MeshInstanceEffect());

	return S_OK;
}

HRESULT CPartilceCreateMgr::Create_Texture_Effect(E_TEXTURE_EFFECTJ type, CTransform * parentTransform)
{
	INSTPARTICLEDESC texdesc = Get_TypeDesc_TextureInstance(type);
	texdesc.FollowingTarget = parentTransform;

	_uint SceneNum = GetSingle(CGameInstance)->Get_NowSceneNum();
	FAILED_CHECK(GetSingle(CUtilityMgr)->Create_TextureInstance(SceneNum,texdesc));

	return S_OK;
}

HRESULT CPartilceCreateMgr::Create_Texture_Effect_World(E_TEXTURE_EFFECTJ type, _float3 WorldPos)
{
	INSTPARTICLEDESC texdesc = Get_TypeDesc_TextureInstance(type);
	texdesc.vFixedPosition = WorldPos;

	_uint SceneNum = GetSingle(CGameInstance)->Get_NowSceneNum();
	FAILED_CHECK(GetSingle(CUtilityMgr)->Create_TextureInstance(SceneNum, texdesc));

	return S_OK;
}

HRESULT CPartilceCreateMgr::Create_MeshInst_Effect(E_MESHINST_EFFECTJ type, CTransform * parentTransform)
{
	INSTMESHDESC texdesc = Get_TypeDesc_MeshInstance(type);
	texdesc.FollowingTarget = parentTransform;

	_uint SceneNum = GetSingle(CGameInstance)->Get_NowSceneNum();
	FAILED_CHECK(GetSingle(CUtilityMgr)->Create_MeshInstance(SceneNum, texdesc));

	return S_OK;
}

HRESULT CPartilceCreateMgr::Create_MeshInst_Effect_World(E_MESHINST_EFFECTJ type, _float3 WorldPos)
{
	INSTMESHDESC texdesc = Get_TypeDesc_MeshInstance(type);
	texdesc.vFixedPosition = WorldPos;

	_uint SceneNum = GetSingle(CGameInstance)->Get_NowSceneNum();
	FAILED_CHECK(GetSingle(CUtilityMgr)->Create_MeshInstance(SceneNum, texdesc));

	return S_OK;
}


HRESULT CPartilceCreateMgr::Create_Texture_Effect_Desc(INSTPARTICLEDESC desc, _uint scene)
{

	FAILED_CHECK(GetSingle(CUtilityMgr)->Create_TextureInstance(scene, desc));

	return S_OK;
}

INSTPARTICLEDESC CPartilceCreateMgr::Get_EffectSetting_Tex(E_TEXTURE_EFFECTJ e, _float TotalTime, _float EachTime, _float4 Color1, _float4 Color2, _uint colorFrequency, _float3 Size1, _float3 Size2, _uint sizeFrequency)
{
	INSTPARTICLEDESC desc = GETPARTICLE->Get_TypeDesc_TextureInstance(e);
	if (TotalTime != 0)
		desc.TotalParticleTime = TotalTime;
	if (EachTime != 0)
		desc.EachParticleLifeTime = EachTime;

	if (colorFrequency != 0)
	{
		desc.TargetColor = Color1;
		desc.TargetColor2 = Color2;
		desc.ColorChageFrequency = colorFrequency;
	}

	if (sizeFrequency != 0)
	{
		desc.ParticleSize = Size1;
		desc.ParticleSize2 = Size2;
		desc.SizeChageFrequency = sizeFrequency;
	}

	return desc;
}

HRESULT CPartilceCreateMgr::Create_MeshInst_DESC(INSTMESHDESC desc, _uint scene)
{

	FAILED_CHECK(GetSingle(CUtilityMgr)->Create_MeshInstance(scene, desc));

	return S_OK;
}

INSTMESHDESC CPartilceCreateMgr::Get_EffectSetting_Mesh(E_MESHINST_EFFECTJ e, COMPONENTPROTOTYPEID meshtype, _float TotalTime, _float EachTime, _float4 Color1, _float4 Color2, _uint colorFrequency, _float3 Size1, _float3 Size2, _uint sizeFrequency)
{

	INSTMESHDESC desc = GETPARTICLE->Get_TypeDesc_MeshInstance(e);
	if (meshtype != 0)
		lstrcpy(desc.szModelMeshProtoTypeTag, TAG_CP(meshtype));

	if (TotalTime != 0)
		desc.TotalParticleTime = TotalTime;
	if (EachTime != 0)
		desc.EachParticleLifeTime = EachTime;

	if (colorFrequency != 0)
	{
		desc.TargetColor = Color1;
		desc.TargetColor2 = Color2;
		desc.ColorChageFrequency = colorFrequency;
	}

	if (sizeFrequency != 0)
	{
		desc.ParticleSize = Size1;
		desc.ParticleSize2 = Size2;
		desc.SizeChageFrequency = sizeFrequency;
	}

	return desc;
}



HRESULT CPartilceCreateMgr::Create_MeshEffect(E_MESH_EFFECTJ type, CTransform * parentTransform,_float3 Offset)
{

	NONINSTNESHEFTDESC meshDesc = Get_TypeDesc_NonInstacne(type);
	meshDesc.vPosition = Offset;
	meshDesc.vLookDir = parentTransform->Get_MatrixState(CTransform::STATE_LOOK);

	_uint SceneNum = GetSingle(CGameInstance)->Get_NowSceneNum();
	CGameObject* obj  = g_pGameInstance->Add_GameObject_GetObject(SceneNum, TAG_LAY(Layer_PlayerEffect), TAG_OP(Prototype_NonInstanceMeshEffect_TT), &meshDesc);
	NULL_CHECK_BREAK(obj);
	CNonInstanceMeshEffect_TT * meshobj = static_cast<CNonInstanceMeshEffect_TT*>(obj);
	NULL_CHECK_BREAK(meshobj);
	meshobj->Set_ParentTransform(parentTransform);


	//mListEffects.push_back(meshobj);


	return S_OK;
}

HRESULT CPartilceCreateMgr::Create_MeshEffect_World(E_MESH_EFFECTJ type, _float3 Postion, _float3 LookDir)
{
	 NONINSTNESHEFTDESC meshDesc = Get_TypeDesc_NonInstacne(type);
	meshDesc.vPosition = Postion;
	meshDesc.vLookDir = LookDir;


	_uint SceneNum = GetSingle(CGameInstance)->Get_NowSceneNum();
	g_pGameInstance->Add_GameObject_To_Layer(SceneNum, TAG_LAY(Layer_PlayerEffect), TAG_OP(Prototype_NonInstanceMeshEffect_TT), &meshDesc);


	return S_OK;
}

HRESULT CPartilceCreateMgr::Create_MeshEffectDesc(NONINSTNESHEFTDESC desc, MESHADDDATA desc2, CTransform * parentTransform, MESHAEASING* easingdesc,  _uint MoveCount)
{

	_uint SceneNum = GetSingle(CGameInstance)->Get_NowSceneNum();
	CGameObject* obj = g_pGameInstance->Add_GameObject_GetObject(SceneNum, TAG_LAY(Layer_PlayerEffect), TAG_OP(Prototype_NonInstanceMeshEffect_TT), &desc);
	NULL_CHECK_BREAK(obj);
	CNonInstanceMeshEffect_TT * meshobj = static_cast<CNonInstanceMeshEffect_TT*>(obj);
	NULL_CHECK_BREAK(meshobj);
	meshobj->Set_AddDesc(desc2);
	meshobj->Set_ParentTransform(parentTransform);

	if (easingdesc)
	{
		meshobj->Set_EasingMoveDesc(easingdesc, MoveCount);
		Safe_Delete_Array(easingdesc);

	}

	return S_OK;
}

HRESULT CPartilceCreateMgr::Create_MeshEffectDesc_Hard(E_MESH_EFFECTJ type, CTransform* Transfom)
{
	if (type >= MESHEFFECT_END)
		return E_FAIL;
	// EXAMPEL
/*

	  if(type == MESHEFFECT_MONSTER_TEST)
	  {
		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_SM_Sphere_RPG;
	
		// Time
		MeshDesc.fMaxTime_Duration = 1.0f;
		MeshDesc.fAppearTime = 0.5f;
		AddDesc.bAfterApperTime = false;
	
		// Tex
		MeshDesc.iDiffuseTextureIndex = 299;
		MeshDesc.MaskTextureIndex = NONNMASK;
		MeshDesc.NoiseTextureIndex = 54;
		// Noise
		MeshDesc.noisingdir = _float2(0, 1).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		// Color
		MeshDesc.vLimLight = _float4(0.35f, 0.16f, 0.0f, 0.0f);
		MeshDesc.vEmissive = _float4(1.0f, 1.0f, 1.0f, 1.f);
	
		// Transform_Base
		MeshDesc.vPosition = _float3(0, 0.0f, 0);
		//	MeshDesc.vPosition = _float3(0.0f, 0, 1.5f);
		MeshDesc.vSize = _float3(1.f);
	
	
	
		// Move
		MeshDesc.MoveDir = FollowingDir_Look;
		MeshDesc.MoveSpeed = 0;
		AddDesc.AccMoveSpeed = 0;
	
		// Rot
		AddDesc.LookRotAxis = FollowingDir_Right;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);
	
		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 0;
		AddDesc.AccRotSpeed = 0;
		AddDesc.InitRot = _float3(0, 0, 0.0f);
	
		// Scale
		AddDesc.AccScaleSpeed = 0.0f;
		AddDesc.ScaleMax = 0.0f;
	
		AddDesc.ScaleReFlag = false;
	
		AddDesc.bLockScale[0] = true;
		AddDesc.bLockScale[1] = true;
		AddDesc.bLockScale[2] = true;
	
	
		// Fix
		AddDesc.FixFlag_Move = true;
		AddDesc.FixFlag_Rot = true;
		AddDesc.FollowTarget = Transfom;
	
		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
		MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå
	
		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);
		}
*/

	Create_MeshEffectDesc_Hard_MONSTER(type,Transfom);
	Create_MeshEffectDesc_Hard_BOSS(type, Transfom);

	return S_OK;
}




HRESULT CPartilceCreateMgr::Update_MeshEffect(_double timer)
{

	return S_OK;
}

HRESULT CPartilceCreateMgr::Clear_MeshEffect()
{
	return S_OK;
}

#define MESHIMPORT(x,y)						MeshEffectDESC.eMeshType = x; mVecMeshEffectDesc[y] = MeshEffectDESC;

HRESULT CPartilceCreateMgr::Ready_MeshEffect()
{
	// #MESHPARTILCEINIT
	mVecMeshEffectDesc.resize((int)MESHEFFECT_END);



	return S_OK;
}

HRESULT CPartilceCreateMgr::Ready_TextureEffect()
{
	// #TEXTUREPARTILCEINIT
	
	/*
	// ÅØ½ºÃ³ ÆÄÆ¼Å¬ »ý¼º ¿¹Á¦ÄÚµå
	instanceDesc = GETPARTICLE->Get_TypeDesc_TextureInstance(CPartilceCreateMgr::TEXTURE_EFFECTJ_Bow_R_FlyBall);
	instanceDesc.FollowingTarget = m_pTextureParticleTransform;
	GETPARTICLE->Create_Texture_Effect_Desc(instanceDesc, m_eNowSceneNum);

	*/
	mVecTextureEffectDesc.resize((int)TEXTURE_EFFECTJ_END);

	auto  pUtil = GetSingle(CUtilityMgr);
	
	mVecTextureEffectDesc[TEXTURE_EFFECTJ_Bow_Default] = pUtil->Get_TextureParticleDesc(TEXT("Bow_Default"));
	mVecTextureEffectDesc[TEXTURE_EFFECTJ_Bow_ArrowHit] = pUtil->Get_TextureParticleDesc(TEXT("Bow_ArrowHit"));
	mVecTextureEffectDesc[TEXTURE_EFFECTJ_Bow_Bow_ArrowTrail] = pUtil->Get_TextureParticleDesc(TEXT("Bow_ArrowTrail"));
	mVecTextureEffectDesc[TEXTURE_EFFECTJ_Bow_Charze_ArrowHead] = pUtil->Get_TextureParticleDesc(TEXT("Bow_Charze_ArrowHead"));
	mVecTextureEffectDesc[TEXTURE_EFFECTJ_Bow_Charze_Circle] = pUtil->Get_TextureParticleDesc(TEXT("Bow_Charze_Circle"));
	mVecTextureEffectDesc[TEXTURE_EFFECTJ_Bow_Charze_Dash] = pUtil->Get_TextureParticleDesc(TEXT("Bow_Charze_Dash"));
	mVecTextureEffectDesc[TEXTURE_EFFECTJ_Bow_Charze_Long] = pUtil->Get_TextureParticleDesc(TEXT("Bow_Charze_Long"));
	mVecTextureEffectDesc[TEXTURE_EFFECTJ_Bow_Charze_Suck] = pUtil->Get_TextureParticleDesc(TEXT("Bow_Charze_Suck"));

	mVecTextureEffectDesc[TEXTURE_EFFECTJ_Bow_Shift_Image] = pUtil->Get_TextureParticleDesc(TEXT("Bow_Shift_Image"));
	mVecTextureEffectDesc[TEXTURE_EFFECTJ_Bow_R_FlyBall] = pUtil->Get_TextureParticleDesc(TEXT("Bow_R_FlyBall"));
	mVecTextureEffectDesc[TEXTURE_EFFECTJ_Bow_R_FlyFire] = pUtil->Get_TextureParticleDesc(TEXT("Bow_R_FlyFire"));
	mVecTextureEffectDesc[TEXTURE_EFFECTJ_Bow_Q_Ball] = pUtil->Get_TextureParticleDesc(TEXT("Bow_Q_Ball"));



	// Universal
	mVecTextureEffectDesc[TEXTURE_EFFECTJ_Universal_Ball] = pUtil->Get_TextureParticleDesc(TEXT("Ball_Universal"));
	mVecTextureEffectDesc[TEXTURE_EFFECTJ_Universal_Suck] = pUtil->Get_TextureParticleDesc(TEXT("Suck_Universal"));
	mVecTextureEffectDesc[TEXTURE_EFFECTJ_Universal_Spread] = pUtil->Get_TextureParticleDesc(TEXT("Spread_Universal"));


	// SameName
	mVecTextureEffectDesc[JY_TextureEft_1] = pUtil->Get_TextureParticleDesc(TEXT("JY_TextureEft_1"));
	mVecTextureEffectDesc[JY_TextureEft_2] = pUtil->Get_TextureParticleDesc(TEXT("JY_TextureEft_2"));
	mVecTextureEffectDesc[JY_TextureEft_3] = pUtil->Get_TextureParticleDesc(TEXT("JY_TextureEft_3"));
	mVecTextureEffectDesc[JY_TextureEft_4] = pUtil->Get_TextureParticleDesc(TEXT("JY_TextureEft_4"));
	mVecTextureEffectDesc[JY_TextureEft_5] = pUtil->Get_TextureParticleDesc(TEXT("JY_TextureEft_5"));
	mVecTextureEffectDesc[JY_TextureEft_6] = pUtil->Get_TextureParticleDesc(TEXT("JY_TextureEft_6"));
	mVecTextureEffectDesc[JY_TextureEft_7] = pUtil->Get_TextureParticleDesc(TEXT("JY_TextureEft_7"));
	mVecTextureEffectDesc[JY_TextureEft_8] = pUtil->Get_TextureParticleDesc(TEXT("JY_TextureEft_8"));
	mVecTextureEffectDesc[Spear_ThrowAttack] = pUtil->Get_TextureParticleDesc(TEXT("Spear_ThrowAttack"));
	mVecTextureEffectDesc[SpearNormalAttack] = pUtil->Get_TextureParticleDesc(TEXT("SpearNormalAttack"));


	mVecTextureEffectDesc[Um_Hit_1] = pUtil->Get_TextureParticleDesc(TEXT("Um_Hit_1"));
	mVecTextureEffectDesc[Um_Hit_2] = pUtil->Get_TextureParticleDesc(TEXT("Um_Hit_2"));
	mVecTextureEffectDesc[Um_Hit_3] = pUtil->Get_TextureParticleDesc(TEXT("Um_Hit_3"));
	mVecTextureEffectDesc[Um_Hit_4] = pUtil->Get_TextureParticleDesc(TEXT("Um_Hit_4"));
	mVecTextureEffectDesc[Um_Dust_1] = pUtil->Get_TextureParticleDesc(TEXT("Um_Dust_1"));
	mVecTextureEffectDesc[Um_Dust_2] = pUtil->Get_TextureParticleDesc(TEXT("Um_Dust_2"));
	mVecTextureEffectDesc[Um_Dust_2_FounTain] = pUtil->Get_TextureParticleDesc(TEXT("Um_Dust_2_FounTain"));
	mVecTextureEffectDesc[Um_Roll_1] = pUtil->Get_TextureParticleDesc(TEXT("Um_Roll_1"));
	mVecTextureEffectDesc[Um_Up_1] = pUtil->Get_TextureParticleDesc(TEXT("Um_Up_1"));
	mVecTextureEffectDesc[Um_Imgae_1] = pUtil->Get_TextureParticleDesc(TEXT("Um_Imgae_1"));
	mVecTextureEffectDesc[Um_Imgae_2] = pUtil->Get_TextureParticleDesc(TEXT("Um_Imgae_2"));
	mVecTextureEffectDesc[Um_Sunder_1] = pUtil->Get_TextureParticleDesc(TEXT("Um_Sunder_1"));
	mVecTextureEffectDesc[Um_FireMask_1] = pUtil->Get_TextureParticleDesc(TEXT("Um_FireMask_1"));
	
	
	
	// DefaultSetting
	mVecTextureEffectDesc[TEXTURE_EFFECTJ_Bow_Default].TotalParticleTime = 99999.f;

	mVecTextureEffectDesc[TEXTURE_EFFECTJ_Bow_Charze_Long].ParticleSize = _float3(0.08f, 0.3f, 0.08f);
	mVecTextureEffectDesc[TEXTURE_EFFECTJ_Bow_Charze_Long].ParticleSize2 = _float3(0.08f, 0.8f, 0.08f);
	mVecTextureEffectDesc[TEXTURE_EFFECTJ_Bow_Charze_Long].iFollowingDir = FollowingDir_Look;


	mVecTextureEffectDesc[TEXTURE_EFFECTJ_Bow_R_FlyBall].TotalParticleTime = 0.2f;

	return S_OK;
}

HRESULT CPartilceCreateMgr::Ready_MeshInstanceEffect()
{
	mVecMeshInstDesc.resize((int)MESHINST_EFFECTJ_END);

	auto  pUtil = GetSingle(CUtilityMgr);


	mVecMeshInstDesc[MESHINST_EFFECTJ_BOW_Q_ICE] = pUtil->Get_MeshParticleDesc(TEXT("Bow_Q_Ice"));
	mVecMeshInstDesc[MESHINST_EFFECTJ_BOW_Q_ICE2] = pUtil->Get_MeshParticleDesc(TEXT("Bow_Q_Ice2"));
	mVecMeshInstDesc[MESHINST_EFFECTJ_BOW_Q_PLANE] = pUtil->Get_MeshParticleDesc(TEXT("Bow_Q_Plane"));
	mVecMeshInstDesc[Um_MeshBase] = pUtil->Get_MeshParticleDesc(TEXT("Um_MeshBase"));

	return S_OK;
}


MESHAEASING CPartilceCreateMgr::CreateEasingDesc(EasingTypeID id, _float3 endpos, _float timemax)
{
	MESHAEASING desc;
	desc.EasingID = id;
	desc.EndPos = endpos;
	desc.MaxTime = timemax;

	return desc;
}

HRESULT CPartilceCreateMgr::Create_MeshEffectDesc_Hard_MONSTER(E_MESH_EFFECTJ type, CTransform * Transfom)
{
	NONINSTNESHEFTDESC	MeshDesc;
	MeshDesc.eMeshType = Prototype_Mesh_SM_circle;

	MESHADDDATA			AddDesc;
	MESHAEASING*		pEaseDesc = nullptr;

	// RANDOM
	static bool bRandomVal = true;
	bRandomVal = !bRandomVal;




#pragma region BOW

	// ÇÏµå ÄÚµùÀ¸·Î Á¦ÀÛ
	if (type == MESHEFFECT_ARROW_HEAD)
	{
		MeshDesc.eMeshType = Prototype_Mesh_SM_4E_ImpactFX_02;

		MeshDesc.fMaxTime_Duration = 3.0f;

		MeshDesc.iDiffuseTextureIndex = 365;
		MeshDesc.MaskTextureIndex = 31;
		MeshDesc.NoiseTextureIndex = 325;

		MeshDesc.noisingdir = _float2(0.5f, 0.5f).Get_Nomalize();
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		MeshDesc.fAppearTime = 1.0f;
		MeshDesc.fDistortionNoisingPushPower = 5.0f;

		MeshDesc.vEmissive = _float4(0.3f, 0.3f, 1.0f, 1.0f);
		MeshDesc.vLimLight = _float4(0.0f, 0.4f, 1, 1);


		MeshDesc.vLookDir = _float3(0, 1, 0);
		MeshDesc.vSize = _float3(0.3f);

		MeshDesc.RotAxis = FollowingDir_Right;
		MeshDesc.RotationSpeedPerSec = 10.0f;

		//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
		//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
		MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå
		AddDesc.LookRotAxis = FollowingDir_Look;
		//	MeshDesc.vLookDir = m_pTransformCom->Get_MatrixScale(CTransform::STATE_RIGHT);

		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

	}

	if (type == MESHEFFECT_ARROW_BOW1)
	{
		MeshDesc.eMeshType = Prototype_Mesh_SM_Bow_Em_01;


		MeshDesc.fMaxTime_Duration = 2.0f;

		MeshDesc.iDiffuseTextureIndex = 335;
		MeshDesc.MaskTextureIndex = 31;
		MeshDesc.NoiseTextureIndex = 325;

		MeshDesc.noisingdir = _float2(0.5f, 0.5f).Get_Nomalize();
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		MeshDesc.fAppearTime = 0.5f;
		MeshDesc.fDistortionNoisingPushPower = 5.0f;

		MeshDesc.vEmissive = _float4(0.3f, 0.3f, 0.5f, 0.8f);
		MeshDesc.vLimLight = _float4(0.1f, 0.76f, 0.77f, 0.2f);


		MeshDesc.vPosition = _float3(-0.0f, 0.0f, -0.0f);
		MeshDesc.vSize = _float3(1.3f);

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 0;

		//	_float3 Pos = Transfom->Get_MatrixState(CTransform::STATE_POS);
		//	_float3 dir =  Transfom->Get_MatrixState(CTransform::STATE_UP);

		MeshDesc.vLookDir = _float3(0, 1, 0);


		//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
		//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
		MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		AddDesc.LookRotAxis = FollowingDir_Up;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);

		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);


	}
	if (type == MESHEFFECT_ARROW_BOW_UP)
	{
		MeshDesc.eMeshType = Prototype_Mesh_SM_Bow_Em_01;

		MeshDesc.fMaxTime_Duration = 3.0f;

		MeshDesc.iDiffuseTextureIndex = 330;
		MeshDesc.MaskTextureIndex = 8;
		MeshDesc.NoiseTextureIndex = 328;

		MeshDesc.noisingdir = _float2(0.0f, 1.0f).Get_Nomalize();
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		MeshDesc.fAppearTime = 1.5f;
		MeshDesc.fDistortionNoisingPushPower = 3.0f;

		MeshDesc.vLimLight = _float4(0.0f, 0.40f, 0.89f, 1.0f);
		MeshDesc.vEmissive = _float4(0.6f, 0.5f, 0.0f, 0.0f);


		MeshDesc.vPosition = _float3(-0.8f, 0.0f, 0.0f);
		MeshDesc.vSize = _float3(1.5f);

		MeshDesc.RotAxis = FollowingDir_Up;
		MeshDesc.RotationSpeedPerSec = 0;

		//	_float3 Pos = Transfom->Get_MatrixState(CTransform::STATE_POS);
		//	_float3 dir =  Transfom->Get_MatrixState(CTransform::STATE_UP);

		MeshDesc.vLookDir = _float3(0, 0, 1);


		//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
		//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
		MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		AddDesc.LookRotAxis = FollowingDir_Up;
		AddDesc.vAddDirectAngle = _float3(0, 270, 0);

		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);
	}


	if (type == MESHEFFECT_ARROW_BOW_SHIFT_PLANE)
	{
		MeshDesc.eMeshType = Prototype_Mesh_SM_circle;

		MeshDesc.fMaxTime_Duration = 3.0f;

		MeshDesc.iDiffuseTextureIndex = 236;
		MeshDesc.MaskTextureIndex = 31;
		MeshDesc.NoiseTextureIndex = 325;

		MeshDesc.noisingdir = _float2(0.5f, 0.5f).Get_Nomalize();
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		MeshDesc.fAppearTime = 0.5f;
		MeshDesc.fDistortionNoisingPushPower = 5.0f;

		MeshDesc.vLimLight = _float4(0.03f, 0.06f, 0.82f, 1.f);
		MeshDesc.vEmissive = _float4(0.03f, 0.06f, 0.82f, 1.f);


		//	MeshDesc.vRotAxis = _float3(1, 0, 0);
		MeshDesc.vPosition = _float3(-0.0f, 0, -0.0f);
		MeshDesc.vSize = _float3(45.0f);

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 0;

		//	_float3 Pos = Transfom->Get_MatrixState(CTransform::STATE_POS);
		//	_float3 dir =  Transfom->Get_MatrixState(CTransform::STATE_UP);

		MeshDesc.vLookDir = _float3(1, 0, 0);


		//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
		MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		AddDesc.LookRotAxis = FollowingDir_Right;

		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);
	}

	if (type == MESHEFFECT_ARROW_BOW_SHIFT_ICE)
	{
		MeshDesc.eMeshType = Prototype_Mesh_SM_ICE_01;

		MeshDesc.fMaxTime_Duration = 1.5f;

		MeshDesc.iDiffuseTextureIndex = 363;
		MeshDesc.MaskTextureIndex = 0;
		MeshDesc.NoiseTextureIndex = 285;

		MeshDesc.noisingdir = _float2(0.5f, 1.5f).Get_Nomalize();
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		MeshDesc.fAppearTime = 1.0f;
		MeshDesc.fDistortionNoisingPushPower = 10.0f;

		MeshDesc.vLimLight = _float4(0.11f, 0.26f, 0.64f, 0.3f);
		MeshDesc.vEmissive = _float4(1.0f, 0.f, 0.f, 0.f);


		//	MeshDesc.vRotAxis = _float3(1, 0, 0);
		MeshDesc.vPosition = _float3(-0.0f, 0, -0.0f);
		MeshDesc.vSize = _float3(3.0f);

		MeshDesc.RotAxis = FollowingDir_Up;
		MeshDesc.RotationSpeedPerSec = 30.0f;

		//	_float3 Pos = Transfom->Get_MatrixState(CTransform::STATE_POS);s
		//	_float3 dir =  Transfom->Get_MatrixState(CTransform::STATE_UP);

		MeshDesc.vLookDir = _float3(1, 0, 0);


		//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
		//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
		MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		AddDesc.LookRotAxis = FollowingDir_Right;
		AddDesc.AccRotSpeed = -28.0f;

		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);
	}

	if (type == MESHEFFECT_ARROW_BOW_R)
	{
		MeshDesc.eMeshType = Prototype_Mesh_SM_Bow_Em_02;

		MeshDesc.fMaxTime_Duration = 0.18f;

		MeshDesc.iDiffuseTextureIndex = 364;
		MeshDesc.MaskTextureIndex = 57;
		MeshDesc.NoiseTextureIndex = 366;

		MeshDesc.noisingdir = _float2(0.5f, 1.5f).Get_Nomalize();
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		MeshDesc.fAppearTime = 0.0f;
		MeshDesc.fDistortionNoisingPushPower = 1.0f;



		MeshDesc.vLimLight = bRandomVal ? _float4(0.56f, 0.97f, 0.98f, 1.0f) : _float4(0.58f, 0.99f, 1.00f, 1.0f);
		MeshDesc.vEmissive = _float4(1.0f, 0.f, 0.f, 0.f);


		//	MeshDesc.vRotAxis = _float3(1, 0, 0);
		MeshDesc.vPosition = _float3(-0.0f, 0, -0.0f);
		MeshDesc.vSize = _float3(1.0f);

		MeshDesc.RotAxis = FollowingDir_Up;
		MeshDesc.RotationSpeedPerSec = 0;

		MeshDesc.vLookDir = _float3(0, 0, 1);


		//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
			//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
		//	MeshDesc.m_iPassIndex = 18; // DisCard
		MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		AddDesc.LookRotAxis = FollowingDir_Up;
		// AddDesc.AccRotSpeed = -28.0f;
		AddDesc.vAddDirectAngle = _float3(0, -30, 0);

		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

	}



	if (type == MESHEFFECT_ARROW_BOW_R_JUMP_WING1)
	{
		MeshDesc.eMeshType = Prototype_Mesh_SM_Bow_Em_Wing_T;

		MeshDesc.fMaxTime_Duration = 1.0f;

		MeshDesc.iDiffuseTextureIndex = 364;
		MeshDesc.MaskTextureIndex = 57;
		MeshDesc.NoiseTextureIndex = 366;

		MeshDesc.noisingdir = _float2(1, 0).Get_Nomalize();
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		MeshDesc.fAppearTime = 0.4f;
		MeshDesc.fDistortionNoisingPushPower = 5.0f;

		MeshDesc.vLimLight = _float4(0.10f, 0.21f, 0.50f, 1.0f);
		MeshDesc.vEmissive = _float4(0.0f, 0.5f, 0.5f, 0.f);


		//	MeshDesc.vRotAxis = _float3(1, 0, 0);
		MeshDesc.vPosition = _float3(0.0f, 0.0f, -0.3f);
		MeshDesc.vSize = _float3(2.5f);

		MeshDesc.RotAxis = FollowingDir_Up;
		MeshDesc.RotationSpeedPerSec = 0;

		MeshDesc.vLookDir = _float3(1, 0, 0);


		MeshDesc.m_iPassIndex = bRandomVal ? 17 : 19; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		AddDesc.LookRotAxis = FollowingDir_Right;
		AddDesc.AccRotSpeed = 0;
		AddDesc.vAddDirectAngle = _float3(0, 90, 0);

		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

	}
	if (type == MESHEFFECT_ARROW_BOW_R_JUMP_WING2)
	{
		MeshDesc.eMeshType = Prototype_Mesh_SM_Bow_Em_Wing_T;

		MeshDesc.fMaxTime_Duration = 1.0f;

		MeshDesc.iDiffuseTextureIndex = 364;
		MeshDesc.MaskTextureIndex = 57;
		MeshDesc.NoiseTextureIndex = 366;

		MeshDesc.noisingdir = _float2(1, 0).Get_Nomalize();
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		MeshDesc.fAppearTime = 0.4f;
		MeshDesc.fDistortionNoisingPushPower = 5.0f;

		MeshDesc.vLimLight = _float4(0.10f, 0.21f, 0.50f, 1.0f);
		MeshDesc.vEmissive = _float4(0.0f, 0.5f, 0.5f, 0.f);


		//	MeshDesc.vRotAxis = _float3(1, 0, 0);
		MeshDesc.vPosition = _float3(0.0f, 0.0f, -0.3f);
		MeshDesc.vSize = _float3(2.5f);

		MeshDesc.RotAxis = FollowingDir_Up;
		MeshDesc.RotationSpeedPerSec = 0;

		MeshDesc.vLookDir = _float3(1, 0, 0);


		MeshDesc.m_iPassIndex = bRandomVal ? 17 : 19; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		AddDesc.LookRotAxis = FollowingDir_Right;
		AddDesc.AccRotSpeed = 0;
		AddDesc.vAddDirectAngle = _float3(0, -90, 0);

		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

	}


	if (type == MESHEFFECT_ARROW_BOW_SP_MOVE_SPEHERE)
	{
		MeshDesc.eMeshType = Prototype_Mesh_mySphere;

		pEaseDesc = NEW MESHAEASING[2];

		MeshDesc.fMaxTime_Duration = 3.0f;

		MeshDesc.iDiffuseTextureIndex = 364;
		MeshDesc.MaskTextureIndex = 60;
		MeshDesc.NoiseTextureIndex = 283;

		MeshDesc.noisingdir = _float2(0.1f, -0.5f).Get_Nomalize();
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		MeshDesc.fAppearTime = 0.0f;
		MeshDesc.fDistortionNoisingPushPower = 1.0f;

		MeshDesc.vLimLight = _float4(0.75f, 0.29f, 0.66f, 1.0f);
		MeshDesc.vEmissive = _float4(1.0f, 0.f, 0.f, 0.f);


		//	MeshDesc.vRotAxis = _float3(1, 0, 0);
		MeshDesc.vPosition = _float3(-0.0f, 0.5f, -0.0f);
		MeshDesc.vSize = _float3(5.0f);

		MeshDesc.RotAxis = FollowingDir_Up;
		MeshDesc.RotationSpeedPerSec = 30;

		MeshDesc.vLookDir = _float3(0, 0, 1);

		MeshDesc.MoveDir = FollowingDir_Look;
		MeshDesc.MoveSpeed = 30;



		//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
		//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
		//	MeshDesc.m_iPassIndex = 18; // DisCard
		MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		AddDesc.LookRotAxis = FollowingDir_Look;
		// AddDesc.AccRotSpeed = -28.0f;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);
		AddDesc.AccMoveSpeed = 100;


		pEaseDesc[0] = CreateEasingDesc(TYPE_Linear, _float3(0, 5.2f, 0), 0.2f);
		pEaseDesc[1] = CreateEasingDesc(TYPE_Linear, _float3(0, 5.2f, 0), 3.0f);
		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom, pEaseDesc, 2);
	}

	if (type == MESHEFFECT_ARROW_BOW_SP_BOM_SPEHERE)
	{
		MeshDesc.eMeshType = Prototype_Mesh_SM_sphere_melon;

		// MAX

		MeshDesc.fMaxTime_Duration = 7.0f;

		MeshDesc.iDiffuseTextureIndex = 44;
		MeshDesc.MaskTextureIndex = 20;
		MeshDesc.NoiseTextureIndex = 30;

		MeshDesc.noisingdir = _float2(0.f, -1.0f).Get_Nomalize();
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		MeshDesc.fAppearTime = 0.3f;
		MeshDesc.fDistortionNoisingPushPower = 3.0f;



		MeshDesc.vLimLight = _float4(0.87f, 0.55f, 0.82f, 1.0f);
		MeshDesc.vEmissive = _float3(0.0f, 0.5f, 0.f);


		//	MeshDesc.vRotAxis = _float3(1, 0, 0);
		MeshDesc.vPosition = _float3(0.2f, 4.0f, 0.2f);
		MeshDesc.vSize = _float3(8.0f);

		MeshDesc.RotAxis = FollowingDir_Up;
		MeshDesc.RotationSpeedPerSec = 50.0f;

		MeshDesc.vLookDir = _float3(0, 0, 1);


		//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
			//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
		//	MeshDesc.m_iPassIndex = 18; // DisCard
		MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.FixFlag_Move = true;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);

		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

	}

	if (type == MESHEFFECT_ARROW_BOW_SP_PLANE)
	{
		MeshDesc.eMeshType = Prototype_Mesh_SM_CoreRing_3;

		MeshDesc.fMaxTime_Duration = 5.0f;

		MeshDesc.iDiffuseTextureIndex = 236;
		MeshDesc.iDiffuseTextureIndex = 33;
		MeshDesc.MaskTextureIndex = 31;
		MeshDesc.NoiseTextureIndex = 325;

		MeshDesc.noisingdir = _float2(0.0f, 1.0f).Get_Nomalize();
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		MeshDesc.fAppearTime = 0.5f;
		MeshDesc.fDistortionNoisingPushPower = 5.0f;

		MeshDesc.vLimLight = _float4(0.03f, 0.06f, 0.82f, 1.f);
		MeshDesc.vEmissive = _float4(0.03f, 0.06f, 0.82f, 1.f);


		//	MeshDesc.vRotAxis = _float3(1, 0, 0);
		MeshDesc.vPosition = _float3(-0.0f, 0, -0.0f);
		MeshDesc.vSize = _float3(3.5f);

		MeshDesc.RotAxis = FollowingDir_Up;
		MeshDesc.RotationSpeedPerSec = 13;

		//	_float3 Pos = Transfom->Get_MatrixState(CTransform::STATE_POS);
		//	_float3 dir =  Transfom->Get_MatrixState(CTransform::STATE_UP);

		MeshDesc.vLookDir = _float3(1, 0, 0);


		//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
		MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		AddDesc.LookRotAxis = FollowingDir_Right;
		AddDesc.FixFlag_Move = true;
		AddDesc.AccRotSpeed = -5.0f;

		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

	}

	if (type == MESHEFFECT_ARROW_BOW_SP_PLANE2)
	{
		MeshDesc.eMeshType = Prototype_Mesh_SM_circle;

		MeshDesc.fMaxTime_Duration = 2.0f;

		MeshDesc.iDiffuseTextureIndex = 36;
		MeshDesc.MaskTextureIndex = 21;
		MeshDesc.NoiseTextureIndex = 357;

		MeshDesc.noisingdir = _float2(0.0f, 1.0f).Get_Nomalize();
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		MeshDesc.fAppearTime = 1.0f;
		MeshDesc.fDistortionNoisingPushPower = 10.0f;

		MeshDesc.vLimLight = _float4(0.02f, 0.16f, 0.90f, 1.f);
		MeshDesc.vEmissive = _float4(0.7f, 0.9f, 0.5f, 1.f);


		//	MeshDesc.vRotAxis = _float3(1, 0, 0);
		MeshDesc.vPosition = _float3(-0.0f, 0, -0.0f);
		MeshDesc.vSize = _float3(70.0f);

		MeshDesc.RotAxis = FollowingDir_Up;
		MeshDesc.RotationSpeedPerSec = 40.0f;

		//	_float3 Pos = Transfom->Get_MatrixState(CTransform::STATE_POS);
		//	_float3 dir =  Transfom->Get_MatrixState(CTransform::STATE_UP);

		MeshDesc.vLookDir = _float3(1, 0, 0);


		//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
		//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
		//	MeshDesc.m_iPassIndex = 18; // DisCard
		MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		AddDesc.LookRotAxis = FollowingDir_Right;
		AddDesc.FixFlag_Move = true;

		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

	}


	if (type == MESHEFFECT_ARROW_BOW_SP_PLANE3)
	{
		MeshDesc.eMeshType = Prototype_Mesh_MS_ST3_Portal_00;

		MeshDesc.fMaxTime_Duration = 4.0f;

		MeshDesc.iDiffuseTextureIndex = 236;
		MeshDesc.iDiffuseTextureIndex = 33;
		MeshDesc.MaskTextureIndex = 31;
		MeshDesc.NoiseTextureIndex = 325;

		MeshDesc.noisingdir = _float2(0.0f, 1.0f).Get_Nomalize();
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		MeshDesc.fAppearTime = 0.2f;
		MeshDesc.fDistortionNoisingPushPower = 5.0f;

		MeshDesc.vLimLight = _float4(0.03f, 0.06f, 0.82f, 1.f);
		MeshDesc.vEmissive = _float4(0.03f, 0.06f, 0.82f, 1.f);


		//	MeshDesc.vRotAxis = _float3(1, 0, 0);
		MeshDesc.vPosition = _float3(-0.0f, -1.0f, -0.0f);
		MeshDesc.vSize = _float3(10.5f);

		MeshDesc.RotAxis = FollowingDir_Up;
		MeshDesc.RotationSpeedPerSec = 15.0f;

		//	_float3 Pos = Transfom->Get_MatrixState(CTransform::STATE_POS);
		//	_float3 dir =  Transfom->Get_MatrixState(CTransform::STATE_UP);

		MeshDesc.vLookDir = _float3(1, 0, 0);


		//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
		MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCardB
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		AddDesc.LookRotAxis = FollowingDir_Right;
		AddDesc.FixFlag_Move = true;
		AddDesc.AccRotSpeed = -10.0f;

		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

	}

	if (type == MESHEFFECT_ARROW_BOW_SP_BOW)
	{
		MeshDesc.eMeshType = Prototype_Mesh_SM_Bow_Em_02;

		MeshDesc.fMaxTime_Duration = 0.4f;

		MeshDesc.iDiffuseTextureIndex = 335;
		MeshDesc.MaskTextureIndex = 31;
		MeshDesc.NoiseTextureIndex = 325;

		MeshDesc.noisingdir = _float2(0.5f, 0.5f).Get_Nomalize();
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		MeshDesc.fAppearTime = 0.01f;
		MeshDesc.fDistortionNoisingPushPower = 5.0f;

		MeshDesc.vEmissive = _float4(0.5f, 0.1f, 0.1f, 0.8f);
		MeshDesc.vLimLight = _float4(0.1f, 0.76f, 0.77f, 0.2f);


		MeshDesc.vPosition = _float3(-0.0f, 0.0f, -0.0f);
		MeshDesc.vSize = _float3(1.3f);

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 0;

		//	_float3 Pos = Transfom->Get_MatrixState(CTransform::STATE_POS);
		//	_float3 dir =  Transfom->Get_MatrixState(CTransform::STATE_UP);

		MeshDesc.vLookDir = _float3(0, 1, 0);


		//		MeshDesc.m_iPassIndex = 16; // ¿Ö°î
		//		MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
		MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		AddDesc.LookRotAxis = FollowingDir_Up;
		AddDesc.vAddDirectAngle = _float3(0, -90, 0);

		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);


	}




	if (type == MESHEFFECT_ARROW_BOW_SP_TON)
	{
		MeshDesc.eMeshType = Prototype_Mesh_ConeMesh;

		// MAX
		MeshDesc.fMaxTime_Duration = 4.0f;

		MeshDesc.iDiffuseTextureIndex = 77;
		MeshDesc.MaskTextureIndex = 51;
		MeshDesc.NoiseTextureIndex = 75;

		MeshDesc.noisingdir = _float2(0.0f, 1.0f).Get_Nomalize();
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		MeshDesc.fAppearTime = 0.3f;
		MeshDesc.fDistortionNoisingPushPower = 3.0f;

		MeshDesc.vLimLight = _float4(0.09f, 0.09f, 0.75f, 1.0f);
		MeshDesc.vEmissive = _float3(1.0f, 0.5f, 0.5f);


		//	MeshDesc.vRotAxis = _float3(1, 0, 0);
		MeshDesc.vPosition = _float3(0.0f, 1.0f, 0.0f);
		MeshDesc.vSize = _float3(1.2f, 0.8f, 1.2f);

		MeshDesc.RotAxis = FollowingDir_Up;
		MeshDesc.RotationSpeedPerSec = 50.0f;

		MeshDesc.vLookDir = _float3(0, 0, 1);


		//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
			//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
		//	MeshDesc.m_iPassIndex = 18; // DisCard
		MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.FixFlag_Move = true;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);
		AddDesc.AccRotSpeed = 5.0f;

		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

	}

	if (type == MESHEFFECT_ARROW_BOW_SP_ICES)
	{
		MeshDesc.eMeshType = Prototype_Mesh_SM_ice2;

		// MAX

		MeshDesc.fMaxTime_Duration = 1.0f;

		MeshDesc.iDiffuseTextureIndex = 44;
		MeshDesc.MaskTextureIndex = 20;
		MeshDesc.NoiseTextureIndex = 30;

		MeshDesc.noisingdir = _float2(0.f, -1.0f).Get_Nomalize();
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		MeshDesc.fAppearTime = 0.3f;
		MeshDesc.fDistortionNoisingPushPower = 3.0f;



		MeshDesc.vLimLight = _float4(0.12f, 0.26f, 0.98f, 1.0f);
		MeshDesc.vEmissive = _float3(0.0f, 0.5f, 0.f);


		//	MeshDesc.vRotAxis = _float3(1, 0, 0);
		MeshDesc.vPosition = _float3(0.0f, 1.0f, -0.5f);
		MeshDesc.vSize = _float3(2.0f);

		MeshDesc.RotAxis = FollowingDir_Up;
		MeshDesc.RotationSpeedPerSec = 10.0f;

		MeshDesc.vLookDir = _float3(0, 0, 1);


		//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
		//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
		MeshDesc.m_iPassIndex = 18; // DisCard
	//MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.FixFlag_Move = true;

		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);


	}

#pragma endregion BOW
#pragma region CREATE

	if (type == MESHEFFECT_MONSTER_CREATE1)
	{
		MeshDesc.eMeshType = Prototype_Mesh_SM_Ring_03;


		MeshDesc.fMaxTime_Duration = 1.0f;

		MeshDesc.iDiffuseTextureIndex = 372;
		MeshDesc.MaskTextureIndex = 55;
		MeshDesc.NoiseTextureIndex = 374;

		MeshDesc.noisingdir = _float2(0.0f, 1.f).Get_Nomalize();
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		MeshDesc.fAppearTime = 0.0f;
		MeshDesc.fDistortionNoisingPushPower = 10.0f;

		MeshDesc.vLimLight = _float4(1, 0, 0, 1.f);
		MeshDesc.vEmissive = _float4(1, 1.0f, 1.f, 1.f);


		//	MeshDesc.vRotAxis = _float3(1, 0, 0);
		MeshDesc.vPosition = _float3(-0.0f, 0, -0.0f);
		MeshDesc.vSize = _float3(3.0f);

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 0.0f;

		//	_float3 Pos = Transfom->Get_MatrixState(CTransform::STATE_POS);
		//	_float3 dir =  Transfom->Get_MatrixState(CTransform::STATE_UP);

		MeshDesc.vLookDir = _float3(1, 0, 0);


		//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
		//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
		//	MeshDesc.m_iPassIndex = 18; // DisCard
		MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		AddDesc.LookRotAxis = FollowingDir_Right;

		AddDesc.vAddDirectAngle = _float3(90, 0, 00);
		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

	}

	if (type == MESHEFFECT_MONSTER_CREATE2)
	{
		MeshDesc.eMeshType = Prototype_Mesh_SM_Ring_03;

		MeshDesc.fMaxTime_Duration = 1.0f;

		MeshDesc.iDiffuseTextureIndex = 372;
		MeshDesc.MaskTextureIndex = 55;
		MeshDesc.NoiseTextureIndex = 374;

		MeshDesc.noisingdir = _float2(0.0f, 1.f).Get_Nomalize();
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		MeshDesc.fAppearTime = 0.0f;
		MeshDesc.fDistortionNoisingPushPower = 10.0f;

		MeshDesc.vLimLight = _float4(0, 0, 1, 1.f);
		MeshDesc.vEmissive = _float4(1, 1.0f, 1.f, 1.f);


		//	MeshDesc.vRotAxis = _float3(1, 0, 0);
		MeshDesc.vPosition = _float3(-0.0f, 0, -0.0f);
		MeshDesc.vSize = _float3(3.0f);

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 0.0f;

		//	_float3 Pos = Transfom->Get_MatrixState(CTransform::STATE_POS);
		//	_float3 dir =  Transfom->Get_MatrixState(CTransform::STATE_UP);

		MeshDesc.vLookDir = _float3(1, 0, 0);


		//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
		//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
		//	MeshDesc.m_iPassIndex = 18; // DisCard
		MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		AddDesc.LookRotAxis = FollowingDir_Right;

		AddDesc.vAddDirectAngle = _float3(90, 0, 00);
		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

	}

#pragma endregion CREATE

#pragma region MONSTER_MM

	if (type == MESHEFFECT_MONSTER_MM_TAIL)
	{
		MeshDesc.eMeshType = Prototype_Mesh_SM_LB_Invisibility_02;

		// Time
		MeshDesc.fMaxTime_Duration = 0.5f;
		MeshDesc.fAppearTime = 0.3f;
		AddDesc.bAfterApperTime = false;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 311;
		MeshDesc.MaskTextureIndex = 106;
		MeshDesc.NoiseTextureIndex = 334;

		// Noise
		MeshDesc.noisingdir = _float2(1, 0).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 50.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		// Color
		MeshDesc.vLimLight = _float4(0.2f, 0.2f, 0.2f, 0.0);
		MeshDesc.vEmissive = _float4(0.5f, 0.0f, 0, 1.f);
		MeshDesc.vEmissive = _float4(1.f);

		// Transform_Base
		MeshDesc.vPosition = _float3(0, -0.5f, 0);
		MeshDesc.vSize = _float3(1.2f, 0.65f, 1.2f);
		MeshDesc.vSize = _float3(1.2f, 0.65f, 1.2f).XMVector()*1.2f;


		// Move
		//MeshDesc.MoveDir = FollowingDir_Up;
		//MeshDesc.MoveSpeed = 10.0f;
		//AddDesc.AccMoveSpeed = 110.0f;

		// Rot
		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);
		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 35;
		AddDesc.AccRotSpeed = 0;
		AddDesc.InitRot = _float3(0, -0, -0);

		// Scale
		AddDesc.AccScaleSpeed = 1.0f;
		AddDesc.ScaleReFlag = false;

		AddDesc.bLockScale[0] = true;
		AddDesc.bLockScale[1] = true;
		AddDesc.bLockScale[2] = true;

		// Fix
		AddDesc.FixFlag_Move = false;
		AddDesc.FixFlag_Rot = false;
		AddDesc.FollowTarget = Transfom;

		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
		MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

		MeshDesc.iDiffuseTextureIndex = 323;
		MeshDesc.MaskTextureIndex = 101;
		MeshDesc.NoiseTextureIndex = 334;
		MeshDesc.noisingdir = _float2(-1, 0).Get_Nomalize();
		MeshDesc.RotationSpeedPerSec = -35;

		MeshDesc.vPosition = _float3(0, 0.1f, 0);
		MeshDesc.vLimLight = _float4(0);
		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

	}


	if (type == MESHEFFECT_MONSTER_MM_TAIL2)
	{
		MeshDesc.eMeshType = Prototype_Mesh_SM_TS_Basic_combo_EX2_Sword;

		// Time
		MeshDesc.fMaxTime_Duration = 1.2f;
		MeshDesc.fAppearTime = 0.5f;
		AddDesc.bAfterApperTime = true;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 317;
		MeshDesc.iDiffuseTextureIndex = 232;
		MeshDesc.MaskTextureIndex = 72;
		MeshDesc.MaskTextureIndex = NONNMASK;
		MeshDesc.NoiseTextureIndex = 182;

		// Noise
		MeshDesc.noisingdir = _float2(1, 0).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 50.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		// Color
		MeshDesc.vLimLight = _float4(1, 1, 1, 1.f);
		MeshDesc.vEmissive = _float4(0.5f, 0.5f, 0.5f, 1.f);
		MeshDesc.vEmissive = _float4(1.f);

		// Transform_Base
		MeshDesc.vPosition = _float3(0, -0.0f, 0);
		MeshDesc.vSize = _float3(0.25f);
		MeshDesc.vSize = _float3(1.0f);
		//	MeshDesc.vSize = _float3(0.3f,0.15f, 0.3f);


			// Move
			//MeshDesc.MoveDir = FollowingDir_Up;
			//MeshDesc.MoveSpeed = 10.0f;
			//AddDesc.AccMoveSpeed = 110.0f;

			// Rot
		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.vAddDirectAngle = _float3(0, 180, 0);
		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 0;
		AddDesc.AccRotSpeed = 0;
		AddDesc.InitRot = _float3(0, -0, -0);

		// Scale
		AddDesc.AccScaleSpeed = 0.0f;
		AddDesc.ScaleReFlag = false;

		AddDesc.bLockScale[0] = false;
		AddDesc.bLockScale[1] = true;
		AddDesc.bLockScale[2] = false;


		// Fix
		AddDesc.FixFlag_Move = false;
		AddDesc.FixFlag_Rot = false;
		AddDesc.FollowTarget = Transfom;

		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
		MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

	}


	if (type == MESHEFFECT_MONSTER_MM_HAND_L)
	{
		

		MeshDesc.eMeshType = Prototype_Mesh_SM_DS_Basic_01_2_Trail_X;

		// Time
		MeshDesc.fMaxTime_Duration = 0.6f;
		MeshDesc.fAppearTime = 0.15f;
		AddDesc.bAfterApperTime = true;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 285;
		MeshDesc.iDiffuseTextureIndex = 360;
		MeshDesc.iDiffuseTextureIndex = 194;
		MeshDesc.MaskTextureIndex = 69;
		MeshDesc.MaskTextureIndex = 102;
		MeshDesc.NoiseTextureIndex = 382;

		// Noise
		MeshDesc.noisingdir = _float2(1, 0).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 20.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		// Color
		MeshDesc.vLimLight = _float4(1, 0, 0, 1.f);
		MeshDesc.vEmissive = _float4(0.5f, 0.5f, 0.5f, 1.f);
		//	MeshDesc.vEmissive = _float4(1, 0.8f, 1.f, 1.f);

			// Transform_Base
		MeshDesc.vPosition = _float3(0.5f, 0.5f, 1.5f);
		MeshDesc.vSize = _float3(0.8f);
		//	MeshDesc.vSize = _float3(0.3f, 0.8f, 0.3f).XMVector()*2.0f;


			// Move
			//MeshDesc.MoveDir = FollowingDir_Up;
			//MeshDesc.MoveSpeed = 10.0f;d
			//AddDesc.AccMoveSpeed = 110.0f;

			// Rotwww
		AddDesc.LookRotAxis = FollowingDir_Right;
		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.vAddDirectAngle = _float3(0, 0, 45);
		AddDesc.vAddDirectAngle = _float3(0, 30, -45);
		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 0;
		AddDesc.AccRotSpeed = 0;
		AddDesc.InitRot = _float3(0);

		// Scale
		AddDesc.AccScaleSpeed = 0.0f;
		AddDesc.ScaleReFlag = false;

		AddDesc.bLockScale[0] = false;
		AddDesc.bLockScale[1] = false;
		AddDesc.bLockScale[2] = false;


		// Fix
		AddDesc.FixFlag_Move = true;
		AddDesc.FixFlag_Rot = true;
		AddDesc.FollowTarget = nullptr;

		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
		MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

		MeshDesc.vPosition = _float3(0.5f, 0, 1.5f);
		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

		MeshDesc.vPosition = _float3(0.5f, -0.5f, 1.5f);
		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

	}

	if (type == MESHEFFECT_MONSTER_MM_HAND_R)
	{

		MeshDesc.eMeshType = Prototype_Mesh_SM_DS_Basic_01_2_Trail_X;

		// Time
		MeshDesc.fMaxTime_Duration = 0.3f;
		MeshDesc.fAppearTime = 0.15f;
		AddDesc.bAfterApperTime = true;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 285;
		MeshDesc.iDiffuseTextureIndex = 360;
		MeshDesc.iDiffuseTextureIndex = 194;
		MeshDesc.MaskTextureIndex = 69;
		MeshDesc.MaskTextureIndex = 102;
		MeshDesc.NoiseTextureIndex = 382;

		// Noise
		MeshDesc.noisingdir = _float2(-1, 0).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 20.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		// Color
		MeshDesc.vLimLight = _float4(1, 0, 0, 1.f);
		MeshDesc.vEmissive = _float4(0.5f, 0.5f, 0.5f, 1.f);
		//	MeshDesc.vEmissive = _float4(1, 0.8f, 1.f, 1.f);

			// Transform_Base
		MeshDesc.vPosition = _float3(0.5f, 0.5f, 1.5f);
		MeshDesc.vPosition = _float3(-0.5f, 0.5f, 1.5f);
		MeshDesc.vSize = _float3(0.8f);
		//	MeshDesc.vSize = _float3(0.3f, 0.8f, 0.3f).XMVector()*2.0f;


			// Move
			//MeshDesc.MoveDir = FollowingDir_Up;
			//MeshDesc.MoveSpeed = 10.0f;d
			//AddDesc.AccMoveSpeed = 110.0f;

			// Rotwww
		AddDesc.LookRotAxis = FollowingDir_Right;
		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.vAddDirectAngle = _float3(0, 50, 45);

		//	AddDesc.vAddDirectAngle = _float3(0, 0, 0);
		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 0;
		AddDesc.AccRotSpeed = 0;
		AddDesc.InitRot = _float3(0);

		// Scale
		AddDesc.AccScaleSpeed = 0.0f;
		AddDesc.ScaleReFlag = false;

		AddDesc.bLockScale[0] = false;
		AddDesc.bLockScale[1] = false;
		AddDesc.bLockScale[2] = false;


		// Fix
		AddDesc.FixFlag_Move = true;
		AddDesc.FixFlag_Rot = true;
		AddDesc.FollowTarget = nullptr;

		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
		MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

		MeshDesc.vPosition = _float3(-0.5f, 0, 1.5f);
		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

		MeshDesc.vPosition = _float3(-0.5f, -0.5f, 1.5f);
		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

	}

#pragma endregion MONSTER_MM

#pragma region MONSTER_ML

	if (type == MESHEFFECT_MONSTER_ML_TAIL1)
	{

		MeshDesc.eMeshType = Prototype_Mesh_SM_LB_Invisibility_02;

		// Time
		MeshDesc.fMaxTime_Duration = 0.5f;
		MeshDesc.fAppearTime = 0.3f;
		AddDesc.bAfterApperTime = false;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 201;
		MeshDesc.iDiffuseTextureIndex = 282;
		MeshDesc.MaskTextureIndex = 106;
		MeshDesc.NoiseTextureIndex = 334;

		// Noise
		MeshDesc.noisingdir = _float2(1, 0).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 50.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		// Color
		MeshDesc.vLimLight = _float4(1.0f, 1.0f, 1.0f, 1.0f);
		MeshDesc.vEmissive = _float4(0.5f, 0.0f, 0, 1.f);
		MeshDesc.vEmissive = _float4(1.f, 0, 0, 1);
		MeshDesc.vEmissive = _float4(1.f);

		// Transform_Base
		MeshDesc.vPosition = _float3(0, -0.5f, 0);
		MeshDesc.vSize = _float3(1.2f, 0.65f, 1.2f);
		MeshDesc.vSize = _float3(1.2f, 0.65f, 1.2f).XMVector()*1.5f;


		// Move
		//MeshDesc.MoveDir = FollowingDir_Up;
		//MeshDesc.MoveSpeed = 10.0f;
		//AddDesc.AccMoveSpeed = 110.0f;

		// Rot
		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);
		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 30;
		AddDesc.AccRotSpeed = 0;
		AddDesc.InitRot = _float3(0, -0, -0);

		// Scale
		AddDesc.AccScaleSpeed = 0.3f;
		AddDesc.ScaleReFlag = false;

		AddDesc.bLockScale[0] = true;
		AddDesc.bLockScale[1] = true;
		AddDesc.bLockScale[2] = true;

		// Fix
		AddDesc.FixFlag_Move = false;
		AddDesc.FixFlag_Rot = false;
		AddDesc.FollowTarget = Transfom;

		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
		MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

		//// 2
		MeshDesc.iDiffuseTextureIndex = 321;
		MeshDesc.MaskTextureIndex = 82;
		MeshDesc.NoiseTextureIndex = 328;
		MeshDesc.noisingdir = _float2(-1, 0).Get_Nomalize();
		MeshDesc.RotationSpeedPerSec = -25;

		MeshDesc.vPosition = _float3(0, 0.0f, 0);
		MeshDesc.vLimLight = _float4(0);
		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

		//// 3
		//MeshDesc.iDiffuseTextureIndex = 311;
		//MeshDesc.MaskTextureIndex = 101;
		//MeshDesc.NoiseTextureIndex = 334;
		//MeshDesc.noisingdir = _float2(0, 1).Get_Nomalize();
		//MeshDesc.RotationSpeedPerSec = 35;

		//MeshDesc.vPosition = _float3(0, 0.5f, 0);
		//MeshDesc.vLimLight = _float4(0);
		//Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);


	}

	if (type == MESHEFFECT_MONSTER_ML_CASH3)
	{
		MeshDesc.eMeshType = Prototype_Mesh_SM_Baren_Weapon;
		
		// Time
		MeshDesc.fMaxTime_Duration = 1.8f;
		MeshDesc.fAppearTime = 0.5f;
		AddDesc.bAfterApperTime = true;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 276;
		MeshDesc.MaskTextureIndex = NONNMASK;
		MeshDesc.MaskTextureIndex = 57;
		MeshDesc.MaskTextureIndex = 71;
		MeshDesc.NoiseTextureIndex = 181;
		MeshDesc.NoiseTextureIndex = 200;

		// Noise
		MeshDesc.noisingdir = _float2(1, 0).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 50.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		// Color
		MeshDesc.vLimLight = _float4(1, 0, 0, 1.0f);
		MeshDesc.vEmissive = _float4(0.5f, 0.5f, 0.5f, 1.f);
	//	MeshDesc.vEmissive = _float4(1.f);

		// Transform_Base
		MeshDesc.vPosition = _float3(0, -0.5f, 0);
		MeshDesc.vSize = _float3(1.0f);
		MeshDesc.vSize = _float3(1.0f,1.5f,1.0f).XMVector()*0.6f;

		//	MeshDesc.vSize = _float3(0.3f,0.15f, 0.3f);


			// Move
			//MeshDesc.MoveDir = FollowingDir_Up;
			//MeshDesc.MoveSpeed = 10.0f;
			//AddDesc.AccMoveSpeed = 110.0f;

			// Rot
		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);
		MeshDesc.RotAxis = FollowingDir_Right;
		MeshDesc.RotationSpeedPerSec = 0;
		AddDesc.AccRotSpeed = 0;
		AddDesc.InitRot = _float3(0, -0, -0);

		// Scale
		AddDesc.AccScaleSpeed = 0.0f;
		AddDesc.ScaleReFlag = false;

		AddDesc.bLockScale[0] = false;
		AddDesc.bLockScale[1] = true;
		AddDesc.bLockScale[2] = false;


		// Fix
		AddDesc.FixFlag_Move = false;
		AddDesc.FixFlag_Rot = false;
		AddDesc.FollowTarget = Transfom;

		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
		MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);


	}



	// ÇÒÄû±â
	if (type == MESHEFFECT_MONSTER_ML_HAND_L)
	{
	
		MeshDesc.eMeshType = Prototype_Mesh_SM_DS_Basic_01_2_Trail_X;

		// Time
		MeshDesc.fMaxTime_Duration = 0.7f;
		MeshDesc.fAppearTime = 0.15f;
		AddDesc.bAfterApperTime = true;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 376;
		MeshDesc.MaskTextureIndex = 143;
		MeshDesc.NoiseTextureIndex = 382;

		// Noise
		MeshDesc.noisingdir = _float2(1, 0).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 20.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		// Color
		MeshDesc.vLimLight = _float4(1, 0, 0, 1.f);
		MeshDesc.vEmissive = _float4(0.5f, 0.5f, 0.5f, 1.f);
		//	MeshDesc.vEmissive = _float4(1, 0.8f, 1.f, 1.f);

			// Transform_Base
		MeshDesc.vPosition = _float3(0.5f, 0.5f, 4.5f);
		MeshDesc.vSize = _float3(0.8f);


			// Move
			//MeshDesc.MoveDir = FollowingDir_Up;
			//MeshDesc.MoveSpeed = 10.0f;d
			//AddDesc.AccMoveSpeed = 110.0f;

			// Rotwww
		AddDesc.LookRotAxis = FollowingDir_Right;
		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.vAddDirectAngle = _float3(0, 0, 45);
		AddDesc.vAddDirectAngle = _float3(0, 30, -45);
		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 0;
		AddDesc.AccRotSpeed = 0;
		AddDesc.InitRot = _float3(0);

		// Scale
		AddDesc.AccScaleSpeed = 0.0f;
		AddDesc.ScaleReFlag = false;

		AddDesc.bLockScale[0] = false;
		AddDesc.bLockScale[1] = false;
		AddDesc.bLockScale[2] = false;


		// Fix
		AddDesc.FixFlag_Move = true;
		AddDesc.FixFlag_Rot = true;
		AddDesc.FollowTarget = nullptr;

		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
		MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

		MeshDesc.vPosition = _float3(0.5f, 0, 4.5f);
		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

		MeshDesc.vPosition = _float3(0.5f, -0.5f, 4.5f);
		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

	}

	if (type == MESHEFFECT_MONSTER_ML_HAND_R)
	{

		MeshDesc.eMeshType = Prototype_Mesh_SM_DS_Basic_01_2_Trail_X;

		// Time
		MeshDesc.fMaxTime_Duration = 0.3f;
		MeshDesc.fAppearTime = 0.15f;
		AddDesc.bAfterApperTime = true;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 376;
		MeshDesc.MaskTextureIndex = 143;
		MeshDesc.NoiseTextureIndex = 382;

		// Noise
		MeshDesc.noisingdir = _float2(-1, 0).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 20.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		// Color
		MeshDesc.vLimLight = _float4(1, 0, 0, 1.f);
		MeshDesc.vEmissive = _float4(0.5f, 0.5f, 0.5f, 1.f);
		//	MeshDesc.vEmissive = _float4(1, 0.8f, 1.f, 1.f);

			// Transform_Base
		MeshDesc.vPosition = _float3(-0.5f, 0.5f, 1.5f);
		MeshDesc.vSize = _float3(0.8f);


			// Move
			//MeshDesc.MoveDir = FollowingDir_Up;
			//MeshDesc.MoveSpeed = 10.0f;d
			//AddDesc.AccMoveSpeed = 110.0f;

			// Rotwww
		AddDesc.LookRotAxis = FollowingDir_Right;
		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.vAddDirectAngle = _float3(0, 50, 45);

		//	AddDesc.vAddDirectAngle = _float3(0, 0, 0);
		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 0;
		AddDesc.AccRotSpeed = 0;
		AddDesc.InitRot = _float3(0);

		// Scale
		AddDesc.AccScaleSpeed = 0.0f;
		AddDesc.ScaleReFlag = false;

		AddDesc.bLockScale[0] = false;
		AddDesc.bLockScale[1] = false;
		AddDesc.bLockScale[2] = false;


		// Fix
		AddDesc.FixFlag_Move = true;
		AddDesc.FixFlag_Rot = true;
		AddDesc.FollowTarget = nullptr;

		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
		MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

		MeshDesc.vPosition = _float3(-0.5f, 0, 1.5f);
		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

		MeshDesc.vPosition = _float3(-0.5f, -0.5f, 1.5f);
		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

	}




	if (type == MESHEFFECT_MONSTER_ML_CASH1)
	{

		MeshDesc.eMeshType = Prototype_Mesh_SM_DS_Basic_01_2_Trail_X;

		// Time
		MeshDesc.fMaxTime_Duration = 0.3f;
		MeshDesc.fAppearTime = 0.15f;
		AddDesc.bAfterApperTime = true;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 376;
		MeshDesc.MaskTextureIndex = 143;
		MeshDesc.NoiseTextureIndex = 382;

		// Noise
		MeshDesc.noisingdir = _float2(-1, 0).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 20.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		// Color
		MeshDesc.vLimLight = _float4(1, 0, 0, 1.f);
		MeshDesc.vEmissive = _float4(0.5f, 0.5f, 0.5f, 1.f);
		//	MeshDesc.vEmissive = _float4(1, 0.8f, 1.f, 1.f);

			// Transform_Base
		MeshDesc.vPosition = _float3(-0.5f, 0, 0.5f);
		MeshDesc.vSize = _float3(0.8f);


		// Move
		//MeshDesc.MoveDir = FollowingDir_Up;
		//MeshDesc.MoveSpeed = 10.0f;d
		//AddDesc.AccMoveSpeed = 110.0f;

		// Rotwww
		AddDesc.LookRotAxis = FollowingDir_Right;
		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.vAddDirectAngle = _float3(0, 50, 45);

		//	AddDesc.vAddDirectAngle = _float3(0, 0, 0);
		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 0;
		AddDesc.AccRotSpeed = 0;
		AddDesc.InitRot = _float3(0);

		// Scale
		AddDesc.AccScaleSpeed = 0.0f;
		AddDesc.ScaleReFlag = false;

		AddDesc.bLockScale[0] = false;
		AddDesc.bLockScale[1] = false;
		AddDesc.bLockScale[2] = false;


		// Fix
		AddDesc.FixFlag_Move = true;
		AddDesc.FixFlag_Rot = true;
		AddDesc.FollowTarget = nullptr;

		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
		MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

		MeshDesc.vPosition = _float3(-0.5f, -0.5f, 0.5f);
		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

		MeshDesc.vPosition = _float3(-0.5f, -1.0f, 0.5f);
		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

		// 2
		MeshDesc.noisingdir = _float2(1, 0).Get_Nomalize();

		MeshDesc.vPosition = _float3(-0.5f, 0.0f, 0.5f);
		AddDesc.vAddDirectAngle = _float3(0, 50, -45);

		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

		MeshDesc.vPosition = _float3(-0.5f, -0.5f, 0.5f);
		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

		MeshDesc.vPosition = _float3(-0.5f, -1.0f, 0.5f);
		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);
	}


	// ²¿¸® Âî¸£±â
	if (type == MESHEFFECT_MONSTER_ML_TAIL2)// 1
	{
	
		MeshDesc.eMeshType = Prototype_Mesh_Sample_Mesh_Fake_Mesh_Lighting;

		// Time
		MeshDesc.fMaxTime_Duration = 0.5f;
		MeshDesc.fAppearTime = 0.2f;
		AddDesc.bAfterApperTime = true;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 276;
		MeshDesc.MaskTextureIndex = NONNMASK;
		MeshDesc.NoiseTextureIndex = 319;

		// Noise
		MeshDesc.noisingdir = _float2(-1, -1).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);

		// Color
		MeshDesc.vLimLight = _float4(1.0f, 0.00f, 0.00f, 1.0f);
		MeshDesc.vEmissive = _float4(0.1f, 0.1f, 0.5f, 1.f);
		MeshDesc.vEmissive = _float4(0);


		// Transform_Base
		MeshDesc.vPosition = _float3(-0.0f, 0, 0.0f);
		MeshDesc.vSize = _float3(0.3f);


		// Move
		//MeshDesc.MoveDir = FollowingDir_Up;
		//MeshDesc.MoveSpeed = 10.0f;d
		//AddDesc.AccMoveSpeed = 110.0f;

		// Rotwww

		AddDesc.LookRotAxis = FollowingDir_Right;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);

		//	AddDesc.vAddDirectAngle = _float3(0, 0, 0);
		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 0;
		AddDesc.AccRotSpeed = 0;
		AddDesc.InitRot = _float3(0);

		// Scale
		AddDesc.AccScaleSpeed = 0.0f;
		AddDesc.ScaleReFlag = false;

		AddDesc.bLockScale[0] = true;
		AddDesc.bLockScale[1] = true;
		AddDesc.bLockScale[2] = true;


		// Fix
		AddDesc.FixFlag_Move = true;
		AddDesc.FixFlag_Rot = true;
		AddDesc.FollowTarget = nullptr;

		// Shader
		MeshDesc.m_iPassIndex = 16; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		MeshDesc.vSize = _float3(0.5f);
		MeshDesc.iDiffuseTextureIndex = 305;
		AddDesc.vAddDirectAngle = _float3(0, 0, GetSingle(CUtilityMgr)->RandomFloat(0, 360));
		MeshDesc.vLimLight = _float4(0.98f, 0.07f, 0.01f, 1.0f);
		MeshDesc.vEmissive = _float4(1);


		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

		AddDesc.DealyTime = 0.3f;
		MeshDesc.vSize = _float3(1.5f);
		MeshDesc.iDiffuseTextureIndex = 321;
		AddDesc.vAddDirectAngle = _float3(0, 0, GetSingle(CUtilityMgr)->RandomFloat(0, 360));
		MeshDesc.vLimLight = _float4(0.98f, 0.07f, 0.01f, 1.0f);
		MeshDesc.vEmissive = _float4(0);
		MeshDesc.m_iPassIndex = 17;
		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

		AddDesc.DealyTime = 0.6f;
		MeshDesc.vSize = _float3(2.5f);
		MeshDesc.iDiffuseTextureIndex = 276;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);
		MeshDesc.vLimLight = _float4(0.98f, 0.07f, 0.01f, 0.0f);

		MeshDesc.m_iPassIndex = 17;
		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

	}

	if (type == MESHEFFECT_MONSTER_ML_TAIL3)
	{
		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_SM_Helix_Crystal;

		// Time
		MeshDesc.fMaxTime_Duration = 0.4f;
		MeshDesc.fAppearTime = 0.2f;
		AddDesc.bAfterApperTime = true;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 317;
		MeshDesc.iDiffuseTextureIndex = 302;

		MeshDesc.MaskTextureIndex = 72;

		MeshDesc.NoiseTextureIndex = 182;


		// Noise
		MeshDesc.noisingdir = _float2(0, 1).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 50.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);

		// Color
		MeshDesc.vLimLight = _float4(0.69f, 0.04f, 0.87f, 1.0f);
		MeshDesc.vEmissive = _float4(0.8f, 0.5f, 1.0f, 1.f);

		// Transform_Base
		MeshDesc.vPosition = _float3(0.0f, 1.5f, 1.0f);
		MeshDesc.vSize = _float3(0.25f);

		// Move
		MeshDesc.MoveDir = FollowingDir_Look;
		MeshDesc.MoveSpeed = 0;
		AddDesc.AccMoveSpeed = 0;

		// RotS
		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.vAddDirectAngle = _float3(90, 0, 0);
		AddDesc.LookRotSpeed = 0;

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 0.0f;
		AddDesc.AccRotSpeed = 0;
		AddDesc.InitRot = _float3(0, 0, 0.0f);

		// Scale
		AddDesc.AccScaleSpeed = 0;
		AddDesc.ScaleReFlag = false;

		AddDesc.bLockScale[0] = false;
		AddDesc.bLockScale[1] = false;
		AddDesc.bLockScale[2] = true;


		// Fix
		AddDesc.FixFlag_Move = true;
		AddDesc.FixFlag_Rot = true;
		AddDesc.FollowTarget = nullptr;

		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
		MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå
		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);
	}


#pragma endregion MONSTER_MM

#pragma region MONSTER_GM


	if (type == MESHEFFECT_MONSTER_GM_ATT0)
	{
		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_SM_06_06_SM002;

		// Time
		MeshDesc.fMaxTime_Duration = 0.4f;
		MeshDesc.fAppearTime = 0.2f;
		AddDesc.bAfterApperTime = true;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 317;
		MeshDesc.MaskTextureIndex = 98;
		MeshDesc.MaskTextureIndex = 202;
		MeshDesc.NoiseTextureIndex = 360;
		// Noise
		MeshDesc.noisingdir = _float2(1.0f, 0).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 50.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		// Color
		MeshDesc.vLimLight = _float4(0.0f, 0.f, 1.0f, 0.0f);
		MeshDesc.vEmissive = _float4(1.f, 1.f, 1.f, 1.f);

		// Transform_Base
		MeshDesc.vPosition = _float3(0.0f, 0.0f, 0.0f);
		//	MeshDesc.vPosition = _float3(0.0f, 0, 1.5f);
		MeshDesc.vSize = _float3(2.5f);

		// Move
		//MeshDesc.MoveDir = FollowingDir_Up;
		//MeshDesc.MoveSpeed = 10.0f;
		//AddDesc.AccMoveSpeed = 110.0f;

		// Rot
		AddDesc.LookRotAxis = FollowingDir_Up;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 0.0f;
		AddDesc.AccRotSpeed = 0.0f;
		AddDesc.InitRot = _float3(0.0f, 0.f, 0.0f);

		// Scale
		AddDesc.AccScaleSpeed = 0.0f;
		AddDesc.ScaleReFlag = false;

		AddDesc.bLockScale[0] = false;
		AddDesc.bLockScale[1] = false;
		AddDesc.bLockScale[2] = false;


		// Fix
		AddDesc.FixFlag_Move = false;

		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
		MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå


		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);
	}

	if (type == MESHEFFECT_MONSTER_GM_Cash2)
	{
		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_SM_06_06_SM002;

		// Time
		MeshDesc.fMaxTime_Duration = 0.4f;
		MeshDesc.fAppearTime = 0.2f;
		AddDesc.bAfterApperTime = true;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 317;
		MeshDesc.MaskTextureIndex = 98;
		MeshDesc.MaskTextureIndex = 202;
		MeshDesc.NoiseTextureIndex = 360;
		// Noise
		MeshDesc.noisingdir = _float2(-1.0f, 0).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 50.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		// Color
		MeshDesc.vLimLight = _float4(0.0f, 0.f, 1.0f, 0.0f);
		MeshDesc.vEmissive = _float4(1.f, 1.f, 1.f, 1.f);

		// Transform_Base
		MeshDesc.vPosition = _float3(0.0f, 0.0f, 0.0f);
		//	MeshDesc.vPosition = _float3(0.0f, 0, 1.5f);
		MeshDesc.vSize = _float3(2.5f);

		// Move
		//MeshDesc.MoveDir = FollowingDir_Up;
		//MeshDesc.MoveSpeed = 10.0f;
		//AddDesc.AccMoveSpeed = 110.0f;

		// Rot
		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.vAddDirectAngle = _float3(0, 0, -45);

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 0.0f;
		AddDesc.AccRotSpeed = 0.0f;
		AddDesc.InitRot = _float3(0.0f, 0.f, 0.0f);

		// Scale
		AddDesc.AccScaleSpeed = 0.0f;
		AddDesc.ScaleReFlag = false;

		AddDesc.bLockScale[0] = false;
		AddDesc.bLockScale[1] = false;
		AddDesc.bLockScale[2] = false;


		// Fix
		AddDesc.FixFlag_Move = false;

		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
		MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå


		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);
	}

	if (type == MESHEFFECT_MONSTER_GM_Cash0)
	{
		MeshDesc.eMeshType = Prototype_Mesh_SM_sphere_melon;
		
		
		// Time
		MeshDesc.fMaxTime_Duration = 1.5f;
		MeshDesc.fAppearTime = 0.5f;
		AddDesc.bAfterApperTime = true;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 317;
		MeshDesc.MaskTextureIndex = 186;
		MeshDesc.NoiseTextureIndex = 31;

		// Noise
		MeshDesc.noisingdir = _float2(0, 1).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 50.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);

		// Color
		MeshDesc.vLimLight = _float4(1.0f, 0.00f, 0.00f, 0.0f);
		MeshDesc.vEmissive = _float4(1);


		// Transform_Base
		MeshDesc.vPosition = _float3(0,-1.8f,0);
		MeshDesc.vSize = _float3(18.0f);


		// Move
		//MeshDesc.MoveDir = FollowingDir_Up;
		//MeshDesc.MoveSpeed = 10.0f;d
		//AddDesc.AccMoveSpeed = 110.0f;

		// Rotwww

		AddDesc.LookRotAxis = FollowingDir_Right;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);

		//	AddDesc.vAddDirectAngle = _float3(0, 0, 0);
		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 50.0f;
		AddDesc.AccRotSpeed = 0;
		AddDesc.InitRot = _float3(0);

		// Scale
		AddDesc.AccScaleSpeed = 1.0f;
		AddDesc.ScaleReFlag = true;

		AddDesc.bLockScale[0] = true;
		AddDesc.bLockScale[1] = true;
		AddDesc.bLockScale[2] = true;


		// Fix
		AddDesc.FixFlag_Move = true;
		AddDesc.FixFlag_Rot = false;
		AddDesc.FollowTarget = Transfom;

		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
		MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

	}

	if (type == MESHEFFECT_MONSTER_GM_Cash1)
	{
		MeshDesc.eMeshType = Prototype_Mesh_SM_4E_LightningBolt_01;
		
		// Time
		MeshDesc.fMaxTime_Duration = 0.8f;
		MeshDesc.fAppearTime = 0.5f;
		AddDesc.bAfterApperTime = true;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 317;
		MeshDesc.MaskTextureIndex = 186;
		MeshDesc.NoiseTextureIndex = 289;

		// Noise
		MeshDesc.noisingdir = _float2(0, 1).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 100.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);

		// Color
		MeshDesc.vLimLight = _float4(0.5f, 0.00f, 1.0f, 1.0f);
		MeshDesc.vLimLight = _float4(1.0f, 0.8f, 0.0f, 1.0f);
		MeshDesc.vEmissive = _float4(1);


		// Transform_Base
		MeshDesc.vPosition = _float3(0, 0.0f, 3.0f);
		MeshDesc.vSize = _float3(8,1.5,8);


		// Move
		//MeshDesc.MoveDir = FollowingDir_Up;
		//MeshDesc.MoveSpeed = 10.0f;d
		//AddDesc.AccMoveSpeed = 110.0f;

		// Rotwww

		AddDesc.LookRotAxis = FollowingDir_Right;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);

		//	AddDesc.vAddDirectAngle = _float3(0, 0, 0);
		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 50;
		AddDesc.AccRotSpeed = -30;
		AddDesc.InitRot = _float3(0);

		// Scale
		AddDesc.AccScaleSpeed = 0.5f;
		AddDesc.ScaleReFlag = false;

		AddDesc.bLockScale[0] = true;
		AddDesc.bLockScale[1] = true;
		AddDesc.bLockScale[2] = true;


		// Fix
		AddDesc.FixFlag_Move = false;
		AddDesc.FixFlag_Rot = false;
		AddDesc.FollowTarget = Transfom;

		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
		MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

	}
	

	if (type == MESHEFFECT_MONSTER_GM_SKILLSMASH0)
	{
		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_SM_DS_Switching_L_D_Plane;
		
		// Time
		MeshDesc.fMaxTime_Duration = 0.4f;
		MeshDesc.fAppearTime = 0.2f;
		AddDesc.bAfterApperTime = true;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 317;
		MeshDesc.iDiffuseTextureIndex = 278;
		MeshDesc.MaskTextureIndex = NONNMASK;
		MeshDesc.MaskTextureIndex = 143;
		MeshDesc.NoiseTextureIndex = 281;




		// Noise
		MeshDesc.noisingdir = _float2(-1.0f, 0).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		// Color
		MeshDesc.vLimLight = _float4(0.0f, 0.f, 0.0f, 0.0f);
		MeshDesc.vLimLight = _float4(1.f, 0.f, 0.0f, 1.0f);
		MeshDesc.vEmissive = _float4(1.f, 1.f, 1.f, 1.f);

		// Transform_Base
		MeshDesc.vPosition = _float3(0.0f, 1.5f, 1.0f);
		//	MeshDesc.vPosition = _float3(0.0f, 0, 1.5f);
		MeshDesc.vSize = _float3(2.5f);

		// Move
		//MeshDesc.MoveDir = FollowingDir_Up;
		//MeshDesc.MoveSpeed = 10.0f;
		//AddDesc.AccMoveSpeed = 110.0f;

		// Rot
		AddDesc.LookRotAxis = FollowingDir_Right;
		AddDesc.vAddDirectAngle = _float3(90, 90, 0);

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 0.0f;
		AddDesc.AccRotSpeed = 0.0f;
		AddDesc.InitRot = _float3(0.0f, 0.f, 0.0f);

		// Scale
		AddDesc.AccScaleSpeed = 0.0f;
		AddDesc.ScaleReFlag = false;

		AddDesc.bLockScale[0] = false;
		AddDesc.bLockScale[1] = false;
		AddDesc.bLockScale[2] = false;


		// Fix
		AddDesc.FixFlag_Move = false;

		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
		MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå


		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);
	}

	if (type == MESHEFFECT_MONSTER_GM_SKILLSMASH1)
	{
		MeshDesc.eMeshType = Prototype_Mesh_SM_sphere_melon;

		// Time
		MeshDesc.fMaxTime_Duration = 1.5f;
		MeshDesc.fAppearTime = 0.5f;
		AddDesc.bAfterApperTime = true;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 317;
		MeshDesc.MaskTextureIndex = 186;
		MeshDesc.NoiseTextureIndex = 31;

		// Noise
		MeshDesc.noisingdir = _float2(0, 1).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 50.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);

		// Color
		MeshDesc.vLimLight = _float4(1.0f, 0.00f, 0.00f, 0.0f);
		MeshDesc.vEmissive = _float4(1);


		// Transform_Base
		MeshDesc.vPosition = _float3(0, -1.8f, 0);
		MeshDesc.vSize = _float3(18.0f);


		// Move
		//MeshDesc.MoveDir = FollowingDir_Up;
		//MeshDesc.MoveSpeed = 10.0f;d
		//AddDesc.AccMoveSpeed = 110.0f;

		// Rotwww

		AddDesc.LookRotAxis = FollowingDir_Right;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);

		//	AddDesc.vAddDirectAngle = _float3(0, 0, 0);
		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 50.0f;
		AddDesc.AccRotSpeed = 0;
		AddDesc.InitRot = _float3(0);

		// Scale
		AddDesc.AccScaleSpeed = 1.0f;
		AddDesc.ScaleReFlag = true;

		AddDesc.bLockScale[0] = true;
		AddDesc.bLockScale[1] = true;
		AddDesc.bLockScale[2] = true;


		// Fix
		AddDesc.FixFlag_Move = true;
		AddDesc.FixFlag_Rot = false;
		AddDesc.FollowTarget = Transfom;

		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
		MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);
	}


	if (type == MESHEFFECT_MONSTER_GM_SKILLSMASH2)
	{
		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_mySphere;

		// Time
		MeshDesc.fMaxTime_Duration = 0.5f;
		MeshDesc.fAppearTime = 0.2f;
		AddDesc.bAfterApperTime = false;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 423;
		MeshDesc.iDiffuseTextureIndex = 321;
		MeshDesc.MaskTextureIndex = 58;
		MeshDesc.NoiseTextureIndex = 357;
		// Noise
		MeshDesc.noisingdir = _float2(0, 1).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		// Color
		MeshDesc.vLimLight = _float4(1, 1, 1, 1.f);
		MeshDesc.vEmissive = _float4(1.f, 1.f, 1.f, 1.f);

		// Transform_Base
		MeshDesc.vPosition = _float3(0);
		//	MeshDesc.vPosition = _float3(0.0f, 0, 1.5f);
		MeshDesc.vSize = _float3(50);

		//	MeshDesc.vSize = _float3(5,10,5);


		// Move
		//MeshDesc.MoveDir = FollowingDir_Up;
		//MeshDesc.MoveSpeed = 10.0f;
		//AddDesc.AccMoveSpeed = 110.0f;

		// Rot
		AddDesc.LookRotAxis = FollowingDir_Right;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 0.0f;
		AddDesc.AccRotSpeed = 5;
		AddDesc.InitRot = _float3(0, 0, 0.0f);

		// Scale
		AddDesc.AccScaleSpeed = -4;
		AddDesc.ScaleReFlag = false;

		AddDesc.bLockScale[0] = true;
		AddDesc.bLockScale[1] = true;
		AddDesc.bLockScale[2] = true;


		// Fix
		AddDesc.FixFlag_Move = false;
		AddDesc.FixFlag_Rot = false;
		AddDesc.FollowTarget = Transfom;

		// Shader
		MeshDesc.m_iPassIndex = 16; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå


		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);
	}



	if (type == MESHEFFECT_MONSTER_GM_SKILLRUN1)
	{

		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_SM_SP_End_Z_R;

		// Time
		MeshDesc.fMaxTime_Duration = 0.3f;
		MeshDesc.fAppearTime = 0.2f;
		AddDesc.bAfterApperTime = true;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 416;
		MeshDesc.MaskTextureIndex = 91;
		MeshDesc.NoiseTextureIndex = 272;
		// Noise
		MeshDesc.noisingdir = _float2(1, 0).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		// Color
		MeshDesc.vLimLight = _float4(0.97f, 0.05f, 0.10f, 1.f);
		MeshDesc.vEmissive = _float4(1.f, 1.f, 1.f, 1.f);

		// Transform_Base
		MeshDesc.vPosition = _float3(-1, 3, 0);
		//	MeshDesc.vPosition = _float3(0.0f, 0, 1.5f);
		MeshDesc.vSize = _float3(1.5f);

		//	MeshDesc.vSize = _float3(5,10,5);


		// Move
		//MeshDesc.MoveDir = FollowingDir_Up;
		//MeshDesc.MoveSpeed = 10.0f;
		//AddDesc.AccMoveSpeed = 110.0f;

		// Rot
		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.vAddDirectAngle = _float3(0, 100, 0);

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 0.0f;
		AddDesc.AccRotSpeed = 0;
		AddDesc.InitRot = _float3(0, 0, 0.0f);

		// Scale
		AddDesc.AccScaleSpeed = 0;
		AddDesc.ScaleReFlag = false;

		AddDesc.bLockScale[0] = true;
		AddDesc.bLockScale[1] = true;
		AddDesc.bLockScale[2] = true;


		// Fix
		AddDesc.FixFlag_Move = true;
		AddDesc.FixFlag_Rot = true;
		AddDesc.FollowTarget = nullptr;

		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
		MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);
	}

	if (type == MESHEFFECT_MONSTER_GM_SKILLRUN0)
	{
		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_SM_SG_tornado;

		// Time
		MeshDesc.fMaxTime_Duration = 0.6f;
		MeshDesc.fAppearTime = 0.3f;
		AddDesc.bAfterApperTime = true;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 394;
		MeshDesc.MaskTextureIndex = 101;
		MeshDesc.NoiseTextureIndex = 211;
		// Noise
		MeshDesc.noisingdir = _float2(0, 1).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		// Color
		MeshDesc.vLimLight = _float4(0.31f, 0.29f, 0.89f, 1.f);
		MeshDesc.vLimLight = _float4(0.71f, 0.29f, 0.98f, 1.f);
		MeshDesc.vEmissive = _float4(1.f, 1.f, 1.f, 1.f);

		// Transform_Base
		MeshDesc.vPosition = _float3(0.0f, 0.0f, 4.00f);
		//	MeshDesc.vPosition = _float3(0.0f, 0, 1.5f);
		MeshDesc.vSize = _float3(3.0f);
		MeshDesc.vSize = _float3(3.0f, 2.0f, 3.0f).XMVector()*0.5f;


		// Move
		//MeshDesc.MoveDir = FollowingDir_Up;
		//MeshDesc.MoveSpeed = 10.0f;
		//AddDesc.AccMoveSpeed = 110.0f;

		// Rot
		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0.0f);

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 45;
		AddDesc.AccRotSpeed = 10;
		AddDesc.InitRot = _float3(0, 0, 0.0f);

		// Scale
		AddDesc.AccScaleSpeed = 5.0f;
		AddDesc.ScaleReFlag = false;

		AddDesc.bLockScale[0] = false;
		AddDesc.bLockScale[1] = true;
		AddDesc.bLockScale[2] = false;


		// Fix
		AddDesc.FixFlag_Move = false;
		AddDesc.FixFlag_Rot = false;

		AddDesc.FollowTarget = Transfom;

		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
		MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå


		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);	
	}


	if (type == MESHEFFECT_MONSTER_GM_SKILLBOUND0)
	{
		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_MS_Shot_Cylinder_00;

		// Time
		MeshDesc.fMaxTime_Duration = 1.0f;
		MeshDesc.fAppearTime = 0.5f;
		AddDesc.bAfterApperTime = false;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 317;
		MeshDesc.MaskTextureIndex = 103;
		MeshDesc.NoiseTextureIndex = 272;
		// Noise
		MeshDesc.noisingdir = _float2(0, 1).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		// Color
		MeshDesc.vLimLight = _float4(0.97f, 0.05f, 0.10f, 1.f);
		MeshDesc.vEmissive = _float4(1.f, 1.f, 1.f, 1.f);

		// Transform_Base
		MeshDesc.vPosition = _float3(0, 1, 0);
		//	MeshDesc.vPosition = _float3(0.0f, 0, 1.5f);
		MeshDesc.vSize = _float3(4.5f);

		//	MeshDesc.vSize = _float3(5,10,5);


		// Move
		MeshDesc.MoveDir = FollowingDir_Look;
		MeshDesc.MoveSpeed = 4.0f;
		AddDesc.AccMoveSpeed = 1.0f;

		// Rot
		AddDesc.LookRotAxis = FollowingDir_Right;
		AddDesc.vAddDirectAngle = _float3(90, 0, 0);

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 30;
		AddDesc.AccRotSpeed = 0;
		AddDesc.InitRot = _float3(0, 0, 0.0f);

		// Scale
		AddDesc.AccScaleSpeed = 0.7f;
		AddDesc.ScaleReFlag = true;

		AddDesc.bLockScale[0] = true;
		AddDesc.bLockScale[1] = true;
		AddDesc.bLockScale[2] = true;


		// Fix
		AddDesc.FixFlag_Move = false;
		AddDesc.FixFlag_Rot = false;
		AddDesc.FollowTarget = nullptr;

		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
		MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);


		//pEaseDesc = NEW MESHAEASING[1];
		//pEaseDesc[0] = CreateEasingDesc(TYPE_Linear, _float3(0, 5, 0), 0.5f);
		//
		//GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom, pEaseDesc, 1);
	}

	if (type == MESHEFFECT_MONSTER_GM_SKILLBOUND1)
	{
		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_SM_DW_Weapon;


		// Time
		MeshDesc.fMaxTime_Duration = 2.0f;
		MeshDesc.fAppearTime = 0.5f;
		AddDesc.bAfterApperTime = true;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 396;
		MeshDesc.MaskTextureIndex = 80;
		MeshDesc.NoiseTextureIndex = 319;
		// Noise
		MeshDesc.noisingdir = _float2(0, 0).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		// Color
		MeshDesc.vLimLight = _float4(0.26f, 0.10f, 0.04f, 1.f);
		MeshDesc.vLimLight = _float4(0.57f, 0.57f, 1.00f, 1.f);
		MeshDesc.vEmissive = _float4(1.f, 0.1f, 0.5f, 1.f);

		// Transform_Base
		MeshDesc.vPosition = _float3(0, 50, 0);
		//	MeshDesc.vPosition = _float3(0.0f, 0, 1.5f);

		MeshDesc.vSize = _float3(8, 11, 8);


		// Move
		MeshDesc.MoveDir = FollowingDir_Look;
		MeshDesc.MoveSpeed = 4.0f;
		AddDesc.AccMoveSpeed = 1.0f;

		// Rot
		AddDesc.LookRotAxis = FollowingDir_Right;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 0;
		AddDesc.AccRotSpeed = 0;
		AddDesc.InitRot = _float3(0, 0, 0.0f);

		// Scale
		AddDesc.AccScaleSpeed = 0.0f;
		AddDesc.ScaleReFlag = false;

		AddDesc.bLockScale[0] = true;
		AddDesc.bLockScale[1] = false;
		AddDesc.bLockScale[2] = true;


		// Fix
		AddDesc.FixFlag_Move = false;
		AddDesc.FixFlag_Rot = false;
		AddDesc.FollowTarget = nullptr;

		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
		MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		//GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);


		pEaseDesc = NEW MESHAEASING[2];
		pEaseDesc[0] = CreateEasingDesc(TYPE_SinInOut, _float3(0, 0, 3), 0.6f);
		pEaseDesc[1] = CreateEasingDesc(TYPE_SinInOut, _float3(0, 0, 3), 1.0f);

		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom, pEaseDesc, 2);
	}
	

	if (type == MESHEFFECT_MONSTER_GM_Cash3)
	{


		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_SM_sinkhole;

		// Time
		MeshDesc.fMaxTime_Duration = 0.7f;
		MeshDesc.fAppearTime = 0.2f;
		AddDesc.bAfterApperTime = true;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 276;
		MeshDesc.MaskTextureIndex = NONNMASK;
		MeshDesc.NoiseTextureIndex = 357;

		// Noise
		MeshDesc.noisingdir = _float2(0,-1).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);

		// Color
		MeshDesc.vLimLight = _float4(1.0f, 0.00f, 0.00f, 1.0f);
		MeshDesc.vEmissive = _float4(0.1f, 0.1f, 0.5f, 1.f);
		MeshDesc.vEmissive = _float4(0);


		// Transform_Base
		MeshDesc.vPosition = _float3(-0.0f, 0, 2.f);
		MeshDesc.vSize = _float3(1);


		// Move
		//MeshDesc.MoveDir = FollowingDir_Up;
		//MeshDesc.MoveSpeed = 10.0f;d
		//AddDesc.AccMoveSpeed = 110.0f;

		// Rotwww

		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);

		//	AddDesc.vAddDirectAngle = _float3(0, 0, 0);
		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 0;
		AddDesc.AccRotSpeed = 0;
		AddDesc.InitRot = _float3(0);

		// Scale
		AddDesc.AccScaleSpeed = 0.0f;
		AddDesc.ScaleReFlag = false;

		AddDesc.bLockScale[0] = true;
		AddDesc.bLockScale[1] = true;
		AddDesc.bLockScale[2] = true;


		// Fix
		AddDesc.FixFlag_Move = true;
		AddDesc.FixFlag_Rot = true;
		AddDesc.FollowTarget = nullptr;

		// Shader
		MeshDesc.m_iPassIndex = 16; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		_float size = 4;
		MeshDesc.vSize = _float3(size);
		MeshDesc.iDiffuseTextureIndex = 317;
		MeshDesc.vLimLight = _float4(0.98f, 0.07f, 0.01f, 1.0f);
		MeshDesc.vEmissive = _float4(0);


		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

		//	AddDesc.DealyTime = 0.2f;
		//	MeshDesc.vSize = _float3(size * 2.0f);
		//	MeshDesc.iDiffuseTextureIndex = 416;
		//	MeshDesc.vLimLight = _float4(0.98f, 0.07f, 0.01f, 1.0f);
		//	MeshDesc.vEmissive = _float4(0);
		//	MeshDesc.m_iPassIndex = 17;
	//	Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

		AddDesc.DealyTime = 0.2f;
		MeshDesc.vSize = _float3(size*3.0f);
		MeshDesc.iDiffuseTextureIndex = 366;
		MeshDesc.vLimLight = _float4(0.98f, 0.07f, 0.01f, 0.0f);

		MeshDesc.m_iPassIndex = 17;
		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

	}




#pragma endregion MONSTER_GM

#pragma region MONSTER_GL

	if (type == MESHEFFECT_MONSTER_GL_SKILLBOUND0)
	{
		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_MS_Shot_Cylinder_00;

		// Time
		MeshDesc.fMaxTime_Duration = 1.0f;
		MeshDesc.fAppearTime = 0.5f;
		AddDesc.bAfterApperTime = false;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 298;
		MeshDesc.MaskTextureIndex = 101;
		MeshDesc.NoiseTextureIndex = 272;
		// Noise
		MeshDesc.noisingdir = _float2(0, 1).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		// Color
		MeshDesc.vLimLight = _float4(0.97f, 0.05f, 0.10f, 1.f);
		MeshDesc.vEmissive = _float4(1.f, 1.f, 1.f, 1.f);

		// Transform_Base
		MeshDesc.vPosition = _float3(0, 1, 0);
		//	MeshDesc.vPosition = _float3(0.0f, 0, 1.5f);
		MeshDesc.vSize = _float3(4.5f);

		//	MeshDesc.vSize = _float3(5,10,5);


		// Move
		MeshDesc.MoveDir = FollowingDir_Look;
		MeshDesc.MoveSpeed = 4.0f;
		AddDesc.AccMoveSpeed = 1.0f;

		// Rot
		AddDesc.LookRotAxis = FollowingDir_Right;
		AddDesc.vAddDirectAngle = _float3(90, 0, 0);

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 30;
		AddDesc.AccRotSpeed = 0;
		AddDesc.InitRot = _float3(0, 0, 0.0f);

		// Scale
		AddDesc.AccScaleSpeed = 0.7f;
		AddDesc.ScaleReFlag = true;

		AddDesc.bLockScale[0] = true;
		AddDesc.bLockScale[1] = true;
		AddDesc.bLockScale[2] = true;


		// Fix
		AddDesc.FixFlag_Move = false;
		AddDesc.FixFlag_Rot = false;
		AddDesc.FollowTarget = nullptr;

		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
		MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);


		//pEaseDesc = NEW MESHAEASING[1];
		//pEaseDesc[0] = CreateEasingDesc(TYPE_Linear, _float3(0, 5, 0), 0.5f);
		//
		//GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom, pEaseDesc, 1);
	}

	if (type == MESHEFFECT_MONSTER_GL_SKILLBOUND1)
	{
		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_SM_Baren_Weapon;

		// Time
		MeshDesc.fMaxTime_Duration = 2.0f;
		MeshDesc.fAppearTime = 0.7f;
		AddDesc.bAfterApperTime = true;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 298;
		MeshDesc.MaskTextureIndex = 51;
		MeshDesc.NoiseTextureIndex = 306;
		// Noise
		MeshDesc.noisingdir = _float2(1, 0).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		// Color
		MeshDesc.vLimLight = _float4(1, 0, 0, 1.f);
		MeshDesc.vEmissive = _float4(1.f, 1.f, 1.f, 1.f);

		// Transform_Base
		MeshDesc.vPosition = _float3(0, 20, 0);
		//	MeshDesc.vPosition = _float3(0.0f, 0, 1.5f);
		MeshDesc.vSize = _float3(2.0f);

		//	MeshDesc.vSize = _float3(5,10,5);


		// Move
		MeshDesc.MoveDir = FollowingDir_Look;
		MeshDesc.MoveSpeed = 4.0f;
		AddDesc.AccMoveSpeed = 1.0f;

		// Rot
		AddDesc.LookRotAxis = FollowingDir_Right;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 35;
		AddDesc.AccRotSpeed = 10;
		AddDesc.InitRot = _float3(0, 0, 0.0f);

		// Scale
		AddDesc.AccScaleSpeed = 1.0f;
		AddDesc.ScaleReFlag = false;

		AddDesc.bLockScale[0] = true;
		AddDesc.bLockScale[1] = true;
		AddDesc.bLockScale[2] = true;


		// Fix
		AddDesc.FixFlag_Move = false;
		AddDesc.FixFlag_Rot = false;
		AddDesc.FollowTarget = nullptr;

		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
		MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		//GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);


		pEaseDesc = NEW MESHAEASING[2];
		pEaseDesc[0] = CreateEasingDesc(TYPE_SinInOut, _float3(0, 0, 3), 0.7f);
		pEaseDesc[1] = CreateEasingDesc(TYPE_SinInOut, _float3(0, 0, 3), 1.0f);

		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom, pEaseDesc, 2);
	}


	if (type == MESHEFFECT_MONSTER_GL_SKILLRUN0)
	{
		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_SM_ky_tornado3;

		// Time
		MeshDesc.fMaxTime_Duration = 0.5f;
		MeshDesc.fAppearTime = 0.3f;
		AddDesc.bAfterApperTime = true;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 103;
		MeshDesc.MaskTextureIndex = 101;
		MeshDesc.NoiseTextureIndex = 360;
		// Noise
		MeshDesc.noisingdir = _float2(0, 1).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		// Color
		MeshDesc.vLimLight = _float4(0.0f, 0.f, 1.0f, 1.f);
		MeshDesc.vEmissive = _float4(1.f, 1.f, 1.f, 1.f);

		// Transform_Base
		MeshDesc.vPosition = _float3(0.0f, -1.5f, 0.0f);
		//	MeshDesc.vPosition = _float3(0.0f, 0, 1.5f);
		MeshDesc.vSize = _float3(3);
		MeshDesc.vSize = _float3(8);


		// Move
		//MeshDesc.MoveDir = FollowingDir_Up;
		//MeshDesc.MoveSpeed = 10.0f;
		//AddDesc.AccMoveSpeed = 110.0f;

		// Rot
		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 0.0f;
		AddDesc.AccRotSpeed = 0;
		AddDesc.InitRot = _float3(0, 0, 0.0f);

		// Scale
		AddDesc.AccScaleSpeed = 5.0f;
		AddDesc.ScaleReFlag = false;

		AddDesc.bLockScale[0] = true;
		AddDesc.bLockScale[1] = false;
		AddDesc.bLockScale[2] = true;


		// Fix
		AddDesc.FixFlag_Move = false;

		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
		MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå


		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);
	}

	if (type == MESHEFFECT_MONSTER_GL_SKILLRUN1)
	{
		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_SM_SG_tornado;

		// Time
		MeshDesc.fMaxTime_Duration = 0.6f;
		MeshDesc.fAppearTime = 0.3f;
		AddDesc.bAfterApperTime = true;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 302;
		MeshDesc.MaskTextureIndex = 101;
		MeshDesc.NoiseTextureIndex = 211;
		// Noise
		MeshDesc.noisingdir = _float2(0, 1).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		// Color
		MeshDesc.vLimLight = _float4(0.31f, 0.29f, 0.29f, 1.f);
		MeshDesc.vEmissive = _float4(1.f, 1.f, 1.f, 1.f);

		// Transform_Base
		MeshDesc.vPosition = _float3(0.0f, 0.0f, 4.00f);
		//	MeshDesc.vPosition = _float3(0.0f, 0, 1.5f);
		MeshDesc.vSize = _float3(3.0f);
		MeshDesc.vSize = _float3(3.0f, 2.0f, 3.0f).XMVector()*0.5f;


		// Move
		//MeshDesc.MoveDir = FollowingDir_Up;
		//MeshDesc.MoveSpeed = 10.0f;
		//AddDesc.AccMoveSpeed = 110.0f;

		// Rot
		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0.0f);

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 45;
		AddDesc.AccRotSpeed = 10;
		AddDesc.InitRot = _float3(0, 0, 0.0f);

		// Scale
		AddDesc.AccScaleSpeed = 5.0f;
		AddDesc.ScaleReFlag = false;

		AddDesc.bLockScale[0] = false;
		AddDesc.bLockScale[1] = true;
		AddDesc.bLockScale[2] = false;


		// Fix
		AddDesc.FixFlag_Move = false;
		AddDesc.FixFlag_Rot = false;

		AddDesc.FollowTarget = Transfom;

		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
		MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå


		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);
	}


	if (type == MESHEFFECT_MONSTER_GL_SKILLTRIPLE3)
	{
		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_SM_SG_tornado;

		// Time
		MeshDesc.fMaxTime_Duration = 0.6f;
		MeshDesc.fAppearTime = 0.3f;
		AddDesc.bAfterApperTime = true;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 302;
		MeshDesc.MaskTextureIndex = 101;
		MeshDesc.NoiseTextureIndex = 211;
		// Noise
		MeshDesc.noisingdir = _float2(0, 1).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		// Color
		MeshDesc.vLimLight = _float4(0.31f, 0.29f, 0.29f, 1.f);
		MeshDesc.vEmissive = _float4(1.f, 1.f, 1.f, 1.f);

		// Transform_Base
		MeshDesc.vPosition = _float3(0.0f, 0.0f, 4.00f);
		//	MeshDesc.vPosition = _float3(0.0f, 0, 1.5f);
		MeshDesc.vSize = _float3(3.0f);
		MeshDesc.vSize = _float3(3.0f, 2.0f, 3.0f).XMVector();


		// Move
		//MeshDesc.MoveDir = FollowingDir_Up;
		//MeshDesc.MoveSpeed = 10.0f;
		//AddDesc.AccMoveSpeed = 110.0f;

		// Rot
		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0.0f);

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 30;
		AddDesc.AccRotSpeed = 10;
		AddDesc.InitRot = _float3(0, 0, 0.0f);

		// Scale
		AddDesc.AccScaleSpeed = 3.0f;
		AddDesc.ScaleReFlag = false;

		AddDesc.bLockScale[0] = true;
		AddDesc.bLockScale[1] = false;
		AddDesc.bLockScale[2] = true;


		// Fix
		AddDesc.FixFlag_Move = false;
		AddDesc.FixFlag_Rot = false;


		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
		MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå


		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);
	}

	if (type == MESHEFFECT_MONSTER_GL_SKILLTRIPLE0)
	{

		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_SM_SP_End_X_L;

		// Time
		MeshDesc.fMaxTime_Duration = 0.4f;
		MeshDesc.fAppearTime = 0.2f;
		AddDesc.bAfterApperTime = false;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 320;
		MeshDesc.MaskTextureIndex = 93;
		MeshDesc.NoiseTextureIndex = 105;
		// Noise
		MeshDesc.noisingdir = _float2(1, 0).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		// Color
		MeshDesc.vLimLight = _float4(1, 0, 0, 1.f);

		MeshDesc.vEmissive = _float4(0.1f, 0, 0, 1.f);

		// Transform_Base
		MeshDesc.vPosition = _float3(0, 5, 0);
		//	MeshDesc.vPosition = _float3(0.0f, 0, 1.5f);
		MeshDesc.vSize = _float3(1.f);

		//	MeshDesc.vSize = _float3(5,10,5);


		// Move
		//MeshDesc.MoveDir = FollowingDir_Up;
		//MeshDesc.MoveSpeed = 10.0f;
		//AddDesc.AccMoveSpeed = 110.0f;

		// Rot
		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 0.0f;
		AddDesc.AccRotSpeed = 0;
		AddDesc.InitRot = _float3(0, 0, 0.0f);

		// Scale
		AddDesc.AccScaleSpeed = 2;
		AddDesc.ScaleReFlag = false;

		AddDesc.bLockScale[0] = true;
		AddDesc.bLockScale[1] = true;
		AddDesc.bLockScale[2] = true;


		// Fix
		AddDesc.FixFlag_Move = true;
		AddDesc.FixFlag_Rot = true;
		AddDesc.FollowTarget = nullptr;

		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
		MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);
	}

	if (type == MESHEFFECT_MONSTER_GL_SKILLTRIPLE1)
	{
		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_SM_06_06_SM002;

		// Time
		MeshDesc.fMaxTime_Duration = 0.5f;
		MeshDesc.fAppearTime = 0.3f;
		AddDesc.bAfterApperTime = true;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 103;
		MeshDesc.MaskTextureIndex = 101;
		MeshDesc.NoiseTextureIndex = 360;
		// Noise
		MeshDesc.noisingdir = _float2(1.0f, 0).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		// Color
		MeshDesc.vLimLight = _float4(0.0f, 0.f, 1.0f, 1.f);
		MeshDesc.vEmissive = _float4(1.f, 1.f, 1.f, 1.f);

		// Transform_Base
		MeshDesc.vPosition = _float3(0.0f, 3.0f, 0.0f);
		//	MeshDesc.vPosition = _float3(0.0f, 0, 1.5f);
		MeshDesc.vSize = _float3(1);

		// Move
		//MeshDesc.MoveDir = FollowingDir_Up;
		//MeshDesc.MoveSpeed = 10.0f;
		//AddDesc.AccMoveSpeed = 110.0f;

		// Rot
		AddDesc.LookRotAxis = FollowingDir_Up;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 0.0f;
		AddDesc.AccRotSpeed = 0.0f;
		AddDesc.InitRot = _float3(0.0f, 0.f, 0.0f);

		// Scale
		AddDesc.AccScaleSpeed = 0.0f;
		AddDesc.ScaleReFlag = false;

		AddDesc.bLockScale[0] = false;
		AddDesc.bLockScale[1] = false;
		AddDesc.bLockScale[2] = false;


		// Fix
		AddDesc.FixFlag_Move = false;
		AddDesc.FixFlag_Rot = false;

		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
		MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå


		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);
	}

	if (type == MESHEFFECT_MONSTER_GL_SKILLTRIPLE2)
	{

		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_SM_Meteo_Sphere3;

		// Time
		MeshDesc.fMaxTime_Duration = 1.5f;
		MeshDesc.fAppearTime = 0.5f;
		AddDesc.bAfterApperTime = true;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 298;
		MeshDesc.MaskTextureIndex = 103;
		MeshDesc.NoiseTextureIndex = 80;
		// Noise
		MeshDesc.noisingdir = _float2(0, 1).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		// Color
		MeshDesc.vLimLight = _float4(0.90f, 0.0f, 0.0f, 1.f);
		MeshDesc.vEmissive = _float4(1.0f, 0.5f, 0.1f, 1.f);

		// Transform_Base
		MeshDesc.vPosition = _float3(0, 0, 0);
		//	MeshDesc.vPosition = _float3(0.0f, 0, 1.5f);
		MeshDesc.vSize = _float3(5);
		MeshDesc.vSize = _float3(1.5f, 9.0f, 1.5f).XMVector()*1.6f;

		//	MeshDesc.vSize = _float3(5,10,5);


		// Move
		//MeshDesc.MoveDir = FollowingDir_Up;
		//MeshDesc.MoveSpeed = 10.0f;
		//AddDesc.AccMoveSpeed = 110.0f;

		// Rot
		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 30.0f;
		AddDesc.AccRotSpeed = 0;
		AddDesc.InitRot = _float3(0, 0, 0.0f);

		// Scale
		AddDesc.AccScaleSpeed = -1.0f;
		AddDesc.ScaleReFlag = false;

		AddDesc.bLockScale[0] = true;
		AddDesc.bLockScale[1] = false;
		AddDesc.bLockScale[2] = true;


		// Fix
		AddDesc.FixFlag_Move = false;
		AddDesc.FixFlag_Rot = false;
		AddDesc.FollowTarget = nullptr;

		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
		MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);


		//	pEaseDesc = NEW MESHAEASING[2];
		//	pEaseDesc[0] = CreateEasingDesc(TYPE_SinInOut, _float3(0, 5.0f, 0), 0.3f);
		//	pEaseDesc[1] = CreateEasingDesc(TYPE_Linear, _float3(0, 0, 0), 1.0f);
		//
		//	GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom, pEaseDesc, 2);
		//

	}


	if (type == MESHEFFECT_MONSTER_GL_SKILLHOLOGRAM2) // plane
	{
		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_Mesh_Leap_Ground_Light;

		// Time
		MeshDesc.fMaxTime_Duration = 6.0f;
		MeshDesc.fAppearTime = 2.0f;
		AddDesc.bAfterApperTime = true;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 322;
		MeshDesc.MaskTextureIndex = 78;
		MeshDesc.MaskTextureIndex = 10;
		MeshDesc.MaskTextureIndex = 103;
		MeshDesc.NoiseTextureIndex = NONNMASK;
		// Noise
		MeshDesc.noisingdir = _float2(-1.0f, 0).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		// Color
		MeshDesc.vLimLight = _float4(1.0f, 0.13f, 0.13f, 1.f);
		MeshDesc.vLimLight = _float4(1.0f, 0.0f, 0.0f, 1.f);

		MeshDesc.vEmissive = _float4(1.f, 1.f, 1.f, 1.f);

		// Transform_Base
		MeshDesc.vPosition = _float3(0.0f, 0.0f, 10.0f);
		//	MeshDesc.vPosition = _float3(0.0f, 0, 1.5f);
		MeshDesc.vSize = _float3(3);
		MeshDesc.vSize = _float3(10.f, 3.f, 3.f).XMVector()*0.5f;

		// Move
		//MeshDesc.MoveDir = FollowingDir_Up;
		//MeshDesc.MoveSpeed = 10.0f;
		//AddDesc.AccMoveSpeed = 110.0f;

		// Rot
		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.vAddDirectAngle = _float3(0, 90, 0);

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 0.0f;
		AddDesc.AccRotSpeed = 0.0f;
		AddDesc.InitRot = _float3(0.0f, 0.f, 0.0f);

		// Scale
		AddDesc.AccScaleSpeed = 0.0f;
		AddDesc.ScaleReFlag = false;

		AddDesc.bLockScale[0] = false;
		AddDesc.bLockScale[1] = false;
		AddDesc.bLockScale[2] = false;


		// Fix
		AddDesc.FixFlag_Move = true;
		AddDesc.FixFlag_Rot = true;

		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
		MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå


		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);
	}


	if (type == MESHEFFECT_MONSTER_GL_SKILLHOLOGRAM1) // door
	{
		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_SM_Crack_OutLine;

		// Time
		MeshDesc.fMaxTime_Duration = 5.0f;
		MeshDesc.fAppearTime = 0.5f;
		AddDesc.bAfterApperTime = true;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 321;
		MeshDesc.MaskTextureIndex = 73;
		MeshDesc.NoiseTextureIndex = 80;
		// Noise
		MeshDesc.noisingdir = _float2(0, 1).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		// Color
		MeshDesc.vLimLight = _float4(1.0f, 0.0f, 0.0f, 1.f);
		MeshDesc.vEmissive = _float4(0.8f, 0.1f, 0.1f, 1.f);

		// Transform_Base
		MeshDesc.vPosition = _float3(-0, 0.0f, 0.0f);
		//	MeshDesc.vPosition = _float3(0.0f, 0, 1.5f);
		MeshDesc.vSize = _float3(1, 1, 1).XMVector()*0.2f;
		MeshDesc.vSize = _float3(2, 0.5, 2).XMVector()*0.3f;
		MeshDesc.vSize = _float3(2, 1.0, 2).XMVector()*0.35f;

		// Move
		//MeshDesc.MoveDir = FollowingDir_Up;
		//MeshDesc.MoveSpeed = 10.0f;
		//AddDesc.AccMoveSpeed = 110.0f;

		// Rot
		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 30;
		AddDesc.AccRotSpeed = 0.0f;
		AddDesc.InitRot = _float3(0.0f, 0.f, 0.0f);

		// Scale
		AddDesc.AccScaleSpeed = 3.0f;
		AddDesc.ScaleReFlag = false;

		AddDesc.bLockScale[0] = false;
		AddDesc.bLockScale[1] = true;
		AddDesc.bLockScale[2] = false;


		// Fix
		AddDesc.FixFlag_Move = false;
		AddDesc.FixFlag_Rot = true;

		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
		MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå


		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

		//MeshDesc.noisingdir = _float2(1.0f, 0).Get_Nomalize();
		//MeshDesc.vPosition = _float3(2.0f, 0.0f, 0.0f);
		//MeshDesc.vSize = _float3(0.8f, 0.8f, 2).XMVector()*0.8f;


		//GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

	}


	if (type == MESHEFFECT_MONSTER_GL_SKILLHOLOGRAM0) // 
	{
		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_SM_Crack_OutLine;

		// Time
		MeshDesc.fMaxTime_Duration = 3.0f;
		MeshDesc.fAppearTime = 0.5f;
		AddDesc.bAfterApperTime = true;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 321;
		MeshDesc.MaskTextureIndex = 73;
		MeshDesc.NoiseTextureIndex = 80;
		// Noise
		MeshDesc.noisingdir = _float2(0, 1).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 30.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		// Color
		MeshDesc.vLimLight = _float4(1.0f, 0.0f, 0.0f, 1.f);
		MeshDesc.vEmissive = _float4(0.8f, 0.1f, 1.0f, 1.f);

		// Transform_Base
		MeshDesc.vPosition = _float3(-0, 0.0f, 0.0f);
		//	MeshDesc.vPosition = _float3(0.0f, 0, 1.5f);
		MeshDesc.vSize = _float3(2, 1.0, 2).XMVector()*0.35f;

		// Move
		//MeshDesc.MoveDir = FollowingDir_Up;
		//MeshDesc.MoveSpeed = 10.0f;
		//AddDesc.AccMoveSpeed = 110.0f;

		// Rot
		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 30;
		AddDesc.AccRotSpeed = 0.0f;
		AddDesc.InitRot = _float3(0.0f, 0.f, 0.0f);

		// Scale
		AddDesc.AccScaleSpeed = 0.0f;
		AddDesc.ScaleReFlag = false;

		AddDesc.bLockScale[0] = false;
		AddDesc.bLockScale[1] = true;
		AddDesc.bLockScale[2] = false;


		// Fix
		AddDesc.FixFlag_Move = false;
		AddDesc.FixFlag_Rot = false;

		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
		MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå


		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

		//MeshDesc.noisingdir = _float2(1.0f, 0).Get_Nomalize();
		//MeshDesc.vPosition = _float3(2.0f, 0.0f, 0.0f);
		//MeshDesc.vSize = _float3(0.8f, 0.8f, 2).XMVector()*0.8f;


		//GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

	}
	if (type == MESHEFFECT_MONSTER_GL_Attack1)
	{
		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_SM_SP_End_X_L;

		// Time
		MeshDesc.fMaxTime_Duration = 0.4f;
		MeshDesc.fAppearTime = 0.2f;
		AddDesc.bAfterApperTime = false;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 419;
		MeshDesc.MaskTextureIndex = 93;
		MeshDesc.NoiseTextureIndex = 105;
		// Noise
		MeshDesc.noisingdir = _float2(1, 0).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		// Color
		MeshDesc.vLimLight = _float4(1.00f, 0.44f, 0.11f, 1.f);
		MeshDesc.vLimLight = _float4(1, 0, 0, 1.f);
		MeshDesc.vEmissive = _float4(0.1f, 0, 0, 1.f);

		// Transform_Base
		MeshDesc.vPosition = _float3(0, 5, 0);
		//	MeshDesc.vPosition = _float3(0.0f, 0, 1.5f);
		MeshDesc.vSize = _float3(1.f);

		//	MeshDesc.vSize = _float3(5,10,5);


		// Move
		//MeshDesc.MoveDir = FollowingDir_Up;
		//MeshDesc.MoveSpeed = 10.0f;
		//AddDesc.AccMoveSpeed = 110.0f;

		// Rot
		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);
		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 0.0f;
		AddDesc.AccRotSpeed = 0;
		AddDesc.InitRot = _float3(0, -0, 0.0f);

		// Scale
		AddDesc.AccScaleSpeed = 2;
		AddDesc.ScaleReFlag = false;

		AddDesc.bLockScale[0] = true;
		AddDesc.bLockScale[1] = true;
		AddDesc.bLockScale[2] = true;


		// Fix
		AddDesc.FixFlag_Move = true;
		AddDesc.FixFlag_Rot = true;
		AddDesc.FollowTarget = nullptr;

		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
		MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);
	}

	if (type == MESHEFFECT_MONSTER_GL_Attack0)
	{
		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_SM_SP_End_X_L;

		// Time
		MeshDesc.fMaxTime_Duration = 0.4f;
		MeshDesc.fAppearTime = 0.2f;
		AddDesc.bAfterApperTime = false;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 419;
		MeshDesc.MaskTextureIndex = 93;
		MeshDesc.NoiseTextureIndex = 105;
		// Noise
		MeshDesc.noisingdir = _float2(1, 0).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		// Color
		MeshDesc.vLimLight = _float4(1.00f, 0.44f, 0.11f, 1.f);
		MeshDesc.vLimLight = _float4(1, 0, 0, 1.f);
		MeshDesc.vEmissive = _float4(0.1f, 0, 0, 1.f);

		// Transform_Base
		MeshDesc.vPosition = _float3(0.5f, 1, 0);
		//	MeshDesc.vPosition = _float3(0.0f, 0, 1.5f);
		MeshDesc.vSize = _float3(1.f);

		//	MeshDesc.vSize = _float3(5,10,5);


		// Move
		//MeshDesc.MoveDir = FollowingDir_Up;
		//MeshDesc.MoveSpeed = 10.0f;
		//AddDesc.AccMoveSpeed = 110.0f;

		// Rot
		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.vAddDirectAngle = _float3(0, 180, -0);
		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 0.0f;
		AddDesc.AccRotSpeed = 0;
		AddDesc.InitRot = _float3(0, -0, -0);

		// Scale
		AddDesc.AccScaleSpeed = 2;
		AddDesc.ScaleReFlag = false;

		AddDesc.bLockScale[0] = true;
		AddDesc.bLockScale[1] = true;
		AddDesc.bLockScale[2] = true;


		// Fix
		AddDesc.FixFlag_Move = true;
		AddDesc.FixFlag_Rot = false;
		AddDesc.FollowTarget = nullptr;

		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
		MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå



		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);
	}

	if (type == MESHEFFECT_MONSTER_GL_Attack2)
	{
		// #TODO ¿©±âºÎÅÍ
		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_SM_SP_End_X_L;

		// Time
		MeshDesc.fMaxTime_Duration = 0.5f;
		MeshDesc.fAppearTime = 0.25f;
		AddDesc.bAfterApperTime = false;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 419;
		MeshDesc.MaskTextureIndex = 93;
		MeshDesc.NoiseTextureIndex = 105;
		// Noise
		MeshDesc.noisingdir = _float2(1, 0).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		// Color
		MeshDesc.vLimLight = _float4(1.00f, 0.44f, 0.11f, 1.f);
		MeshDesc.vLimLight = _float4(1, 0, 0, 1.f);
		MeshDesc.vEmissive = _float4(0.1f, 0, 0, 1.f);

		// Transform_Base
		MeshDesc.vPosition = _float3(0.5f, 1, 0);
		//	MeshDesc.vPosition = _float3(0.0f, 0, 1.5f);
		MeshDesc.vSize = _float3(1.f);

		//	MeshDesc.vSize = _float3(5,10,5);


		// Move
		//MeshDesc.MoveDir = FollowingDir_Up;
		//MeshDesc.MoveSpeed = 10.0f;
		//AddDesc.AccMoveSpeed = 110.0f;

		// Rot
		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.vAddDirectAngle = _float3(0, 150, -0);
		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 0.0f;
		AddDesc.AccRotSpeed = 0;
		AddDesc.InitRot = _float3(0, -0, -0);

		// Scale
		AddDesc.AccScaleSpeed = 2;
		AddDesc.ScaleReFlag = false;

		AddDesc.bLockScale[0] = true;
		AddDesc.bLockScale[1] = true;
		AddDesc.bLockScale[2] = true;


		// Fix
		AddDesc.FixFlag_Move = true;
		AddDesc.FixFlag_Rot = false;
		AddDesc.FollowTarget = nullptr;

		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
		MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå



		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);
	}

	if (type == MESHEFFECT_MONSTER_GL_Attack0) // wind
	{
		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_SM_ky_tornado3;

		// Time
		MeshDesc.fMaxTime_Duration = 1.0f;
		MeshDesc.fAppearTime = 0.6f;
		AddDesc.bAfterApperTime = true;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 103;
		MeshDesc.MaskTextureIndex = 101;
		MeshDesc.NoiseTextureIndex = 360;
		// Noise
		MeshDesc.noisingdir = _float2(0, 1).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		// Color
		MeshDesc.vLimLight = _float4(1.0f, 0.f, 0.0f, 1.f);
		MeshDesc.vEmissive = _float4(1.f, 1.f, 1.f, 1.f);

		// Transform_Base
		MeshDesc.vPosition = _float3(0.0f, -1.5f, 3.0f);
		//	MeshDesc.vPosition = _float3(0.0f, 0, 1.5f);
		MeshDesc.vSize = _float3(10);


		// Move
		//MeshDesc.MoveDir = FollowingDir_Up;
		//MeshDesc.MoveSpeed = 10.0f;
		//AddDesc.AccMoveSpeed = 110.0f;

		// Rot
		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 0.0f;
		AddDesc.AccRotSpeed = 0;
		AddDesc.InitRot = _float3(0, 0, 0.0f);

		// Scale
		AddDesc.AccScaleSpeed = 5.0f;
		AddDesc.ScaleReFlag = false;

		AddDesc.bLockScale[0] = true;
		AddDesc.bLockScale[1] = false;
		AddDesc.bLockScale[2] = true;


		// Fix
		AddDesc.FixFlag_Move = false;

		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
		MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå


		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

	}

	if (type == MESHEFFECT_MONSTER_GL_Cash0)
	{
		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_SM_sinkhole;

		// Time
		MeshDesc.fMaxTime_Duration = 0.7f;
		MeshDesc.fAppearTime = 0.0f;
		AddDesc.bAfterApperTime = true;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 276;
		MeshDesc.MaskTextureIndex = NONNMASK;
		MeshDesc.NoiseTextureIndex = 357;

		// Noise
		MeshDesc.noisingdir = _float2(0, -1).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);

		// Color
		MeshDesc.vLimLight = _float4(1.0f, 0.00f, 0.00f, 1.0f);
		MeshDesc.vEmissive = _float4(0.1f, 0.1f, 0.5f, 1.f);
		MeshDesc.vEmissive = _float4(0);


		// Transform_Base
		MeshDesc.vPosition = _float3(-0.0f, 0, 3.0f);
		MeshDesc.vSize = _float3(1);


		// Move
		//MeshDesc.MoveDir = FollowingDir_Up;
		//MeshDesc.MoveSpeed = 10.0f;d
		//AddDesc.AccMoveSpeed = 110.0f;

		// Rotwww

		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);

		//	AddDesc.vAddDirectAngle = _float3(0, 0, 0);
		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 0;
		AddDesc.AccRotSpeed = 0;
		AddDesc.InitRot = _float3(0);

		// Scale
		AddDesc.AccScaleSpeed = 0.0f;
		AddDesc.ScaleReFlag = false;

		AddDesc.bLockScale[0] = true;
		AddDesc.bLockScale[1] = true;
		AddDesc.bLockScale[2] = true;


		// Fix
		AddDesc.FixFlag_Move = true;
		AddDesc.FixFlag_Rot = true;
		AddDesc.FollowTarget = nullptr;

		// Shader
		MeshDesc.m_iPassIndex = 16; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå



		MeshDesc.vSize = _float3(10);
		MeshDesc.iDiffuseTextureIndex = 366;
		MeshDesc.vLimLight = _float4(0.98f, 0.07f, 0.01f, 0.0f);

		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

		AddDesc.DealyTime = 0.1f;
		MeshDesc.fMaxTime_Duration = 0.6f;
		MeshDesc.vPosition = _float3(-3.0f, 0, 3.0f);
		MeshDesc.vSize = _float3(5);
		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);
		MeshDesc.vPosition = _float3(3.0f, 0, 3.0f);
		MeshDesc.vSize = _float3(5);
		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

	}


#pragma endregion MONSTER_GL

#pragma region MONSTER_VM

	if (type == MESHEFFECT_MONSTER_VM_Test)
	{
		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_SM_SS_Aura_01;

		// Time
		MeshDesc.fMaxTime_Duration = 0.7f;
		MeshDesc.fAppearTime = 0.3f;
		AddDesc.bAfterApperTime = true;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 321;
		MeshDesc.MaskTextureIndex = 13;
		MeshDesc.NoiseTextureIndex = 35;

		// Noise
		MeshDesc.noisingdir = _float2(0, 1).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);

		// Color
		MeshDesc.vLimLight = _float4(0.35f, 0.16f, 0.0f, 0.0f);
		MeshDesc.vEmissive = _float4(1.0f, 1.0f, 1.0f, 1.f);
	//	MeshDesc.vEmissive = _float4(0.1f, 0.3f, 0.1f, 1.f);

		// Transform_Base
		MeshDesc.vPosition = _float3(0, -3.f, 0);
		//	MeshDesc.vPosition = _float3(0.0f, 0, 1.5f);
		MeshDesc.vSize = _float3(1.0f);



		// Move
		MeshDesc.MoveDir = FollowingDir_Look;
		MeshDesc.MoveSpeed = 0;
		AddDesc.AccMoveSpeed = 0;

		// Rot
		AddDesc.LookRotAxis = FollowingDir_Right;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);
		AddDesc.LookRotSpeed = 0;
		
		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 30;
		AddDesc.AccRotSpeed = 0;
		AddDesc.InitRot = _float3(0, 0, 0.0f);

		// Scale
		AddDesc.AccScaleSpeed = 0.0f;
		AddDesc.ScaleReFlag = false;

		AddDesc.bLockScale[0] = true;
		AddDesc.bLockScale[1] = true;
		AddDesc.bLockScale[2] = true;


		// Fix
		AddDesc.FixFlag_Move = false;
		AddDesc.FixFlag_Rot = false;
		AddDesc.FollowTarget = Transfom;

		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
		MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);


	}

	if (type == MESHEFFECT_MONSTER_VM_Plane)
	{
		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_Sample_Mesh_Fake_Mesh_Lighting;

		// Time
		MeshDesc.fMaxTime_Duration = 0.5f;
		MeshDesc.fAppearTime = 0.3f;
		AddDesc.bAfterApperTime = true;
		AddDesc.bAfterApperTime = false;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 418;
		MeshDesc.MaskTextureIndex = 107;
		MeshDesc.NoiseTextureIndex = 180;
		// Noise
		MeshDesc.noisingdir = _float2(0, -1).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		// Color
		MeshDesc.vLimLight = _float4(0.35f, 0.16f, 0.0f, 0.0f);
		MeshDesc.vEmissive = _float4(1.0f, 1.0f, 1.0f, 1.f);
		MeshDesc.vEmissive = _float4(0.1f, 0.3f, 0.1f, 1.f);

		// Transform_Base
		MeshDesc.vPosition = _float3(0, 0.5f, 0);
		//	MeshDesc.vPosition = _float3(0.0f, 0, 1.5f);
		_float rand = GetSingle(CUtilityMgr)->RandomFloat(0.7f, 1.25f);
		MeshDesc.vSize = _float3(rand);



		// Move
		MeshDesc.MoveDir = FollowingDir_Look;
		MeshDesc.MoveSpeed = 0;
		AddDesc.AccMoveSpeed = 0;

		// Rot
		AddDesc.LookRotAxis = FollowingDir_Right;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 0;
		AddDesc.AccRotSpeed = 0;
		AddDesc.InitRot = _float3(0, 0, 0.0f);

		// Scale
		AddDesc.AccScaleSpeed = 1.25f;
		AddDesc.ScaleReFlag = false;

		AddDesc.bLockScale[0] = true;
		AddDesc.bLockScale[1] = true;
		AddDesc.bLockScale[2] = true;


		// Fix
		AddDesc.FixFlag_Move = true;
		AddDesc.FixFlag_Rot = true;
		AddDesc.FollowTarget = nullptr;

		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
		MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

	}
	


#pragma endregion MONSTER_VM

#pragma region MONSTER_VL

	if (type == MESHEFFECT_MONSTER_VL_Test)
	{




	}

	if (type == MESHEFFECT_MONSTER_VL_Cash0)
	{
		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_SM_Sphere_RPG;

		// Time
		MeshDesc.fMaxTime_Duration = 8.0f;
		MeshDesc.fAppearTime = 0.5f;
		AddDesc.bAfterApperTime = false;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 299;
		MeshDesc.MaskTextureIndex = NONNMASK;
		MeshDesc.NoiseTextureIndex = 237;
		MeshDesc.NoiseTextureIndex = 289;
		MeshDesc.NoiseTextureIndex = 54;
		// Noise
		MeshDesc.noisingdir = _float2(0, 1).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		// Color
		MeshDesc.vLimLight = _float4(0.35f, 0.16f, 0.0f, 0.0f);
		MeshDesc.vLimLight = _float4(0.96f, 0.45f, 0.09f, 0.0f);
		MeshDesc.vLimLight = _float4(0.96f, 0.45f, 0.09f, 1.0f);
		MeshDesc.vEmissive = _float4(1.0f, 1.0f, 1.0f, 1.f);
		MeshDesc.vEmissive = _float4(0.1f, 0.3f, 0.1f, 1.f);

		// Transform_Base
		MeshDesc.vPosition = _float3(0, 0.0f, 0);
		//	MeshDesc.vPosition = _float3(0.0f, 0, 1.5f);
		MeshDesc.vSize = _float3(0.1f);



		// Move
		MeshDesc.MoveDir = FollowingDir_Look;
		MeshDesc.MoveSpeed = 0;
		AddDesc.AccMoveSpeed = 0;

		// Rot
		AddDesc.LookRotAxis = FollowingDir_Right;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 30;
		AddDesc.AccRotSpeed = 0;
		AddDesc.InitRot = _float3(0, 0, 0.0f);

		// Scale
		AddDesc.AccScaleSpeed = 0.8f;
		AddDesc.ScaleMax = 0.6f;

		AddDesc.ScaleReFlag = false;

		AddDesc.bLockScale[0] = true;
		AddDesc.bLockScale[1] = true;
		AddDesc.bLockScale[2] = true;


		// Fix
		AddDesc.FixFlag_Move = true;
		AddDesc.FixFlag_Rot = true;
		AddDesc.FollowTarget = Transfom;

		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
		MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);
	}

	if (type == MESHEFFECT_MONSTER_VL_Cash1)
	{

		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_MS_Trail_Twist_00;

		// Time
		MeshDesc.fMaxTime_Duration = 1.0f;
		MeshDesc.fAppearTime = 0.2f;
		AddDesc.bAfterApperTime = false;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 299;
		MeshDesc.MaskTextureIndex = NONNMASK;
		MeshDesc.NoiseTextureIndex = 54;
		// Noise
		MeshDesc.noisingdir = _float2(0, 1).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		// Color
		MeshDesc.vLimLight = _float4(0.35f, 0.16f, 0.0f, 0.0f);
		MeshDesc.vEmissive = _float4(1.0f, 1.0f, 1.0f, 1.f);

		// Transform_Base
		MeshDesc.vPosition = _float3(0, 0.0f, 0);
		//	MeshDesc.vPosition = _float3(0.0f, 0, 1.5f);
		MeshDesc.vSize = _float3(2.0f);
		MeshDesc.vSize = _float3(3.f, 5.0f, 3.0f);



		// Move
		MeshDesc.MoveDir = FollowingDir_Look;
		MeshDesc.MoveSpeed = 0;
		AddDesc.AccMoveSpeed = 0;

		// Rot
		AddDesc.LookRotAxis = FollowingDir_Right;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 30;
		AddDesc.AccRotSpeed = 0;
		AddDesc.InitRot = _float3(0, 0, 0.0f);

		// Scale
	//	AddDesc.AccScaleSpeed = 1.0f;
	//	AddDesc.ScaleMax = 3.0f;

		AddDesc.ScaleReFlag = false;

		AddDesc.bLockScale[0] = true;
		AddDesc.bLockScale[1] = true;
		AddDesc.bLockScale[2] = true;


		// Fix
		AddDesc.FixFlag_Move = false;
		AddDesc.FixFlag_Rot = false;
		AddDesc.FollowTarget = nullptr;

		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
		MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);
	}

	if (type == MESHEFFECT_MONSTER_VL_Cash2)
	{
		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_Sample_Mesh_WP_Cylinder_Z;

		// Time
		MeshDesc.fMaxTime_Duration = 1.2f;
		MeshDesc.fAppearTime = 0.4f;
		AddDesc.bAfterApperTime = false;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 299;
		MeshDesc.MaskTextureIndex = NONNMASK;
		MeshDesc.NoiseTextureIndex = 54;

		MeshDesc.iDiffuseTextureIndex = 299;
		MeshDesc.MaskTextureIndex = NONNMASK;
		MeshDesc.NoiseTextureIndex = 54;

		// Noise
		MeshDesc.noisingdir = _float2(0, 1).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		// Color
		MeshDesc.vLimLight = _float4(0.35f, 0.16f, 0.0f, 0.0f);
		MeshDesc.vEmissive = _float4(1.0f, 1.0f, 1.0f, 1.f);

		// Transform_Base
		MeshDesc.vPosition = _float3(0, 0.0f, 0);
		//	MeshDesc.vPosition = _float3(0.0f, 0, 1.5f);
		MeshDesc.vSize = _float3(12.0f);



		// Move
		MeshDesc.MoveDir = FollowingDir_Look;
		MeshDesc.MoveSpeed = 0;
		AddDesc.AccMoveSpeed = 0;

		// Rot
		AddDesc.LookRotAxis = FollowingDir_Right;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 20;
		AddDesc.AccRotSpeed = 10;
		AddDesc.InitRot = _float3(0, 0, 0.0f);

		// Scale
	//	AddDesc.AccScaleSpeed = 1.0f;
	//	AddDesc.ScaleMax = 3.0f;

		AddDesc.ScaleReFlag = false;

		AddDesc.bLockScale[0] = true;
		AddDesc.bLockScale[1] = true;
		AddDesc.bLockScale[2] = true;


		// Fix
		AddDesc.FixFlag_Move = false;
		AddDesc.FixFlag_Rot = false;
		AddDesc.FollowTarget = nullptr;

		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
		MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå
		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);
	}

#pragma endregion MONSTER_VL

#pragma region MONSTER_NM

	if (type == MESHEFFECT_MONSTER_NM_Test)
	{

		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_SM_DivineLaserBeam_02;

		// Time
		MeshDesc.fMaxTime_Duration = 0.2f;
		MeshDesc.fAppearTime = 0.1f;
		AddDesc.bAfterApperTime = true;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 317;

		MeshDesc.MaskTextureIndex = 72;

		MeshDesc.NoiseTextureIndex = 182;


		// Noise
		MeshDesc.noisingdir = _float2(0, 1).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 50.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);

		// Color
		MeshDesc.vLimLight = _float4(0.69f, 0.04f, 0.87f, 1.0f);
		MeshDesc.vLimLight = _float4(0.01f, 0.71f, 0.96f, 1.0f);
		MeshDesc.vEmissive = _float4(0.8f, 0.5f, 1.0f, 1.f);
		MeshDesc.vEmissive = _float4(1);

		// Transform_Base
		MeshDesc.vPosition = _float3(0.0f, -0.5f, -7.0f);
		MeshDesc.vSize = _float3(0.3f, 0.5f, 0.3f);

		// Move
		MeshDesc.MoveDir = FollowingDir_Look;
		MeshDesc.MoveSpeed = 0;
		AddDesc.AccMoveSpeed = 0;

		// RotS
		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.vAddDirectAngle = _float3(70, 0, 0);
		AddDesc.LookRotSpeed = 0;

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 0.0f;
		AddDesc.AccRotSpeed = 0;
		AddDesc.InitRot = _float3(0, 0, 0.0f);

		// Scale
		AddDesc.AccScaleSpeed = -3.0f;
		AddDesc.ScaleReFlag = false;

		AddDesc.bLockScale[0] = false;
		AddDesc.bLockScale[1] = false;
		AddDesc.bLockScale[2] = true;


		// Fix
		AddDesc.FixFlag_Move = true;
		AddDesc.FixFlag_Rot = true;
		AddDesc.FollowTarget = nullptr;

		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
		MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå
		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

		MeshDesc.vPosition = _float3(0.0f, 2.0f, -7.0f);
		AddDesc.vAddDirectAngle = _float3(100, 0, 0);
		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

	}




	if (type == MESHEFFECT_MONSTER_NM_Cash2)
	{
		
		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_SM_ky_sphere_dist_wind;

		// Time
		MeshDesc.fMaxTime_Duration = 1.0f;
		MeshDesc.fAppearTime = 0.5f;
		AddDesc.bAfterApperTime = true;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 317;
		MeshDesc.iDiffuseTextureIndex = 301;
		MeshDesc.MaskTextureIndex = 51;
		MeshDesc.MaskTextureIndex = 63;
		MeshDesc.NoiseTextureIndex = 160;

		// Noise
		MeshDesc.noisingdir = _float2(-1, 0).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);

		// Color
		MeshDesc.vLimLight = _float4(0.35f, 0.16f, 0.0f, 0.0f);
		MeshDesc.vEmissive = _float4(1.0f, 1.0f, 1.0f, 1.f);
		//	MeshDesc.vEmissive = _float4(0.1f, 0.3f, 0.1f, 1.f);

			// Transform_Base
		MeshDesc.vPosition = _float3(0, 0.5f, 0);
		//	MeshDesc.vPosition = _float3(0.0f, 0, 1.5f);
		MeshDesc.vSize = _float3(20.0f);

		// Move
		MeshDesc.MoveDir = FollowingDir_Look;
		MeshDesc.MoveSpeed = 0;
		AddDesc.AccMoveSpeed = 0;

		// Rot
		AddDesc.LookRotAxis = FollowingDir_Right;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);
		AddDesc.LookRotSpeed = 0;

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 30.0f;
		AddDesc.AccRotSpeed = 0;
		AddDesc.InitRot = _float3(0, 0, 0.0f);

		// Scale
		AddDesc.AccScaleSpeed = 0.0f;
		AddDesc.ScaleReFlag = false;

		AddDesc.bLockScale[0] = true;
		AddDesc.bLockScale[1] = true;
		AddDesc.bLockScale[2] = true;


		// Fix
		AddDesc.FixFlag_Move = false;
		AddDesc.FixFlag_Rot = false;
		AddDesc.FollowTarget = Transfom;

		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
		MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);
	}

	if (type == MESHEFFECT_MONSTER_NM_Cash3)
	{
		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_SM_AXTrail_01;

		// Time
		MeshDesc.fMaxTime_Duration = 0.5f;
		MeshDesc.fAppearTime = 0.2f;
		AddDesc.bAfterApperTime = true;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 278;
		MeshDesc.MaskTextureIndex = 106;
		MeshDesc.MaskTextureIndex = 71;
		MeshDesc.NoiseTextureIndex = 188;


		// Noise
		MeshDesc.noisingdir = _float2(1, 0).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);

		// Color
		MeshDesc.vLimLight = _float4(0.35f, 0.16f, 0.0f, 0.0f);
		MeshDesc.vLimLight = _float4(0.1f, 0.4f, 0.96f, 1.0f);
		MeshDesc.vEmissive = _float4(0.3f, 0.1f, 0.1f, 1.f);
		//	MeshDesc.vEmissive = _float4(0.1f, 0.3f, 0.1f, 1.f);

			// Transform_Base
		MeshDesc.vPosition = _float3(0);
		//	MeshDesc.vPosition = _float3(0.0f, 0, 1.5f);
		MeshDesc.vSize = _float3(0.4f, 0.2f, 0.4f);

		// Move
		MeshDesc.MoveDir = FollowingDir_Look;
		MeshDesc.MoveSpeed = 0;
		AddDesc.AccMoveSpeed = 0;

		// RotS
		AddDesc.LookRotAxis = FollowingDir_Right;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);
		AddDesc.LookRotSpeed = 0;

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 20.0f;
		AddDesc.AccRotSpeed = 0;
		AddDesc.InitRot = _float3(0, 0, 0.0f);

		// Scale
		AddDesc.AccScaleSpeed = 0.0f;
		AddDesc.ScaleReFlag = false;

		AddDesc.bLockScale[0] = true;
		AddDesc.bLockScale[1] = true;
		AddDesc.bLockScale[2] = true;


		// Fix
		AddDesc.FixFlag_Move = false;
		AddDesc.FixFlag_Rot = false;
		AddDesc.FollowTarget = Transfom;

		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
		MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

		MeshDesc.vSize = _float3(1.0f, 0.2f, 1.0f);
		MeshDesc.vLimLight = _float4(1.0f, 0.0f, 0.96f, 1.0f);
		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);
	}

	if (type == MESHEFFECT_MONSTER_NM_Cash4)
	{
		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_SM_DivineLaserBeam_02;

		// Time
		MeshDesc.fMaxTime_Duration = 0.2f;
		MeshDesc.fAppearTime = 0.1f;
		AddDesc.bAfterApperTime = true;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 317;

		MeshDesc.MaskTextureIndex = 72;

		MeshDesc.NoiseTextureIndex = 182;


		// Noise
		MeshDesc.noisingdir = _float2(0, 1).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 50.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);

		// Color
		MeshDesc.vLimLight = _float4(0.69f, 0.04f, 0.87f, 1.0f);
		MeshDesc.vLimLight = _float4(0.01f, 0.71f, 0.96f, 1.0f);
		MeshDesc.vEmissive = _float4(0.8f, 0.5f, 1.0f, 1.f);
		MeshDesc.vEmissive = _float4(1);

		// Transform_Base
		MeshDesc.vPosition = _float3(0.0f, -0.5f, -7.0f);
		MeshDesc.vSize = _float3(0.3f, 0.5f, 0.3f);

		// Move
		MeshDesc.MoveDir = FollowingDir_Look;
		MeshDesc.MoveSpeed = 0;
		AddDesc.AccMoveSpeed = 0;

		// RotS
		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.vAddDirectAngle = _float3(70, 0, 0);
		AddDesc.LookRotSpeed = 0;

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 0.0f;
		AddDesc.AccRotSpeed = 0;
		AddDesc.InitRot = _float3(0, 0, 0.0f);

		// Scale
		AddDesc.AccScaleSpeed = -3.0f;
		AddDesc.ScaleReFlag = false;

		AddDesc.bLockScale[0] = false;
		AddDesc.bLockScale[1] = false;
		AddDesc.bLockScale[2] = true;


		// Fix
		AddDesc.FixFlag_Move = true;
		AddDesc.FixFlag_Rot = true;
		AddDesc.FollowTarget = nullptr;

		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
		MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå
		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

		MeshDesc.vPosition = _float3(0.0f, 2.0f, -7.0f);
		AddDesc.vAddDirectAngle = _float3(100, 0, 0);
		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);
	}

	// ÀÏ¼¶ Åä³×ÀÌµµ ÀúÀå
	if (type == MESHEFFECT_MONSTER_NM_Cash5)
	{
	// Mesh
	MeshDesc.eMeshType = Prototype_Mesh_SM_Helix_Crystal;

	// Time
	MeshDesc.fMaxTime_Duration = 0.4f;
	MeshDesc.fAppearTime = 0.2f;
	AddDesc.bAfterApperTime = true;

	// Tex
	MeshDesc.iDiffuseTextureIndex = 317;

	MeshDesc.MaskTextureIndex = 72;

	MeshDesc.NoiseTextureIndex = 182;


	// Noise
	MeshDesc.noisingdir = _float2(0, 1).Get_Nomalize();
	MeshDesc.fDistortionNoisingPushPower = 50.0f;
	MeshDesc.vColor = _float4(1, 1, 1, 1);

	// Color
	MeshDesc.vLimLight = _float4(0.69f, 0.04f, 0.87f, 1.0f);
	MeshDesc.vEmissive = _float4(0.8f, 0.5f, 1.0f, 1.f);

	// Transform_Base
	MeshDesc.vPosition = _float3(0.0f, 1.0f, -7.0f);
	MeshDesc.vSize = _float3(0.3f);

	// Move
	MeshDesc.MoveDir = FollowingDir_Look;
	MeshDesc.MoveSpeed = 0;
	AddDesc.AccMoveSpeed = 0;

	// RotS
	AddDesc.LookRotAxis = FollowingDir_Look;
	AddDesc.vAddDirectAngle = _float3(90, 0, 0);
	AddDesc.LookRotSpeed = 0;

	MeshDesc.RotAxis = FollowingDir_Look;
	MeshDesc.RotationSpeedPerSec = 0.0f;
	AddDesc.AccRotSpeed = 0;
	AddDesc.InitRot = _float3(0, 0, 0.0f);

	// Scale
	AddDesc.AccScaleSpeed = 0;
	AddDesc.ScaleReFlag = false;

	AddDesc.bLockScale[0] = false;
	AddDesc.bLockScale[1] = false;
	AddDesc.bLockScale[2] = true;


	// Fix
	AddDesc.FixFlag_Move = true;
	AddDesc.FixFlag_Rot = true;
	AddDesc.FollowTarget = nullptr;

	// Shader
//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
//	MeshDesc.m_iPassIndex = 18; // DisCard
//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå
	Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);
	}


#pragma endregion MONSTER_NM

	
#pragma region MONSTER_NL

	if (type == MESHEFFECT_MONSTER_NL_Test)
	{

		// Plane
		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_SM_TS_Double_Slash_01;

		// Time
		MeshDesc.fMaxTime_Duration = 0.5f;
		MeshDesc.fAppearTime = 0.2f;
		AddDesc.bAfterApperTime = true;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 278;
		MeshDesc.MaskTextureIndex = NONNMASK;
		MeshDesc.MaskTextureIndex = 34;
		MeshDesc.NoiseTextureIndex = 54;


		// Noise
		MeshDesc.noisingdir = _float2(1, 1).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 50.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);

		// Color
		MeshDesc.vLimLight = _float4(0.35f, 0.16f, 0.0f, 0.0f);
		MeshDesc.vLimLight = _float4(0.1f, 0.4f, 0.96f, 1.0f);
		MeshDesc.vEmissive = _float4(0.3f, 0.1f, 0.1f, 1.f);
		MeshDesc.vEmissive = _float4(1);
		//	MeshDesc.vEmissive = _float4(0.1f, 0.3f, 0.1f, 1.f);

			// Transform_Base
		MeshDesc.vPosition = _float3(0.0f, -0.4f, 0);
		MeshDesc.vSize = _float3(0.5f);

		// Move
		MeshDesc.MoveDir = FollowingDir_Look;
		MeshDesc.MoveSpeed = 0;
		AddDesc.AccMoveSpeed = 0;

		// RotS
		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.vAddDirectAngle = _float3(0, 0, 90);
		AddDesc.LookRotSpeed = 0;

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 30;
		AddDesc.AccRotSpeed = 0;
		AddDesc.InitRot = _float3(0, 0, 0.0f);

		// Scale
		AddDesc.AccScaleSpeed = 0.0f;
		AddDesc.ScaleReFlag = false;

		AddDesc.bLockScale[0] = true;
		AddDesc.bLockScale[1] = true;
		AddDesc.bLockScale[2] = true;


		// Fix
		AddDesc.FixFlag_Move = false;
		AddDesc.FixFlag_Rot = false;
		AddDesc.FollowTarget = Transfom;

		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
		MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

	}

	if (type == MESHEFFECT_MONSTER_NL_Cash0)
	{
		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_SM_AXTrail_01;

		// Time
		MeshDesc.fMaxTime_Duration = 0.5f;
		MeshDesc.fAppearTime = 0.2f;
		AddDesc.bAfterApperTime = true;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 278;
		MeshDesc.MaskTextureIndex = 106;
		MeshDesc.MaskTextureIndex = 71;
		MeshDesc.NoiseTextureIndex = 188;


		// Noise
		MeshDesc.noisingdir = _float2(1, 0).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);

		// Color
		MeshDesc.vLimLight = _float4(0.35f, 0.16f, 0.0f, 0.0f);
		MeshDesc.vLimLight = _float4(0.1f, 0.4f, 0.96f, 1.0f);
		MeshDesc.vEmissive = _float4(0.3f, 0.1f, 0.1f, 1.f);
		//	MeshDesc.vEmissive = _float4(0.1f, 0.3f, 0.1f, 1.f);

			// Transform_Base
		MeshDesc.vPosition = _float3(0);
		//	MeshDesc.vPosition = _float3(0.0f, 0, 1.5f);
		MeshDesc.vSize = _float3(0.4f, 0.2f, 0.4f);

		// Move
		MeshDesc.MoveDir = FollowingDir_Look;
		MeshDesc.MoveSpeed = 0;
		AddDesc.AccMoveSpeed = 0;

		// RotS
		AddDesc.LookRotAxis = FollowingDir_Right;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);
		AddDesc.LookRotSpeed = 0;

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 20.0f;
		AddDesc.AccRotSpeed = 0;
		AddDesc.InitRot = _float3(0, 0, 0.0f);

		// Scale
		AddDesc.AccScaleSpeed = 0.0f;
		AddDesc.ScaleReFlag = false;

		AddDesc.bLockScale[0] = true;
		AddDesc.bLockScale[1] = true;
		AddDesc.bLockScale[2] = true;


		// Fix
		AddDesc.FixFlag_Move = false;
		AddDesc.FixFlag_Rot = false;
		AddDesc.FollowTarget = Transfom;

		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
		MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

		MeshDesc.vSize = _float3(1.0f, 0.2f, 1.0f);
		MeshDesc.vLimLight = _float4(1.0f, 0.0f, 0.96f, 1.0f);
		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);
	}

	if (type == MESHEFFECT_MONSTER_NL_Cash1)
	{
		// Plane
		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_SM_TS_Double_Slash_01;

		// Time
		MeshDesc.fMaxTime_Duration = 0.5f;
		MeshDesc.fAppearTime = 0.2f;
		AddDesc.bAfterApperTime = true;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 278;
		MeshDesc.MaskTextureIndex = NONNMASK;
		MeshDesc.MaskTextureIndex = 34;
		MeshDesc.NoiseTextureIndex = 54;


		// Noise
		MeshDesc.noisingdir = _float2(1, 1).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 50.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);

		// Color
		MeshDesc.vLimLight = _float4(0.35f, 0.16f, 0.0f, 0.0f);
		MeshDesc.vLimLight = _float4(0.1f, 0.4f, 0.96f, 1.0f);
		MeshDesc.vEmissive = _float4(0.3f, 0.1f, 0.1f, 1.f);
		MeshDesc.vEmissive = _float4(1);
		//	MeshDesc.vEmissive = _float4(0.1f, 0.3f, 0.1f, 1.f);

			// Transform_Base
		MeshDesc.vPosition = _float3(0.0f, -0.4f, 0);
		MeshDesc.vSize = _float3(0.5f);

		// Move
		MeshDesc.MoveDir = FollowingDir_Look;
		MeshDesc.MoveSpeed = 0;
		AddDesc.AccMoveSpeed = 0;

		// RotS
		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.vAddDirectAngle = _float3(0, 0, 90);
		AddDesc.LookRotSpeed = 0;

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 30;
		AddDesc.AccRotSpeed = 0;
		AddDesc.InitRot = _float3(0, 0, 0.0f);

		// Scale
		AddDesc.AccScaleSpeed = 0.0f;
		AddDesc.ScaleReFlag = false;

		AddDesc.bLockScale[0] = true;
		AddDesc.bLockScale[1] = true;
		AddDesc.bLockScale[2] = true;


		// Fix
		AddDesc.FixFlag_Move = false;
		AddDesc.FixFlag_Rot = false;
		AddDesc.FollowTarget = Transfom;

		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
		MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);
	}

	if (type == MESHEFFECT_MONSTER_NL_Cash2)
	{

		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_SM_GN_Wind01;

		// Time
		MeshDesc.fMaxTime_Duration = 0.6f;
		MeshDesc.fAppearTime = 0.3f;
		AddDesc.bAfterApperTime = true;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 278;
		MeshDesc.iDiffuseTextureIndex = 370;
		MeshDesc.MaskTextureIndex = NONNMASK;
		MeshDesc.NoiseTextureIndex = 384;


		// Noise
		MeshDesc.noisingdir = _float2(0, 1).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);

		// Color
		MeshDesc.vLimLight = _float4(1);
		MeshDesc.vEmissive = _float4(1);

		// Transform_Base
		MeshDesc.vPosition = _float3(-1.0f, 1.0f, -0.2f);
		MeshDesc.vSize = _float3(1.3f);

		// Move
		MeshDesc.MoveDir = FollowingDir_Look;
		MeshDesc.MoveSpeed = 0;
		AddDesc.AccMoveSpeed = 0;

		// RotS
		AddDesc.LookRotAxis = FollowingDir_Right;
		AddDesc.vAddDirectAngle = _float3(90, 0, 0);
		AddDesc.LookRotSpeed = 0;

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 30;
		AddDesc.AccRotSpeed = 0;
		AddDesc.InitRot = _float3(0, 0, 0.0f);

		// Scale
		AddDesc.AccScaleSpeed = 0.0f;
		AddDesc.ScaleReFlag = false;

		AddDesc.bLockScale[0] = true;
		AddDesc.bLockScale[1] = true;
		AddDesc.bLockScale[2] = true;


		// Fix
		AddDesc.FixFlag_Move = false;
		AddDesc.FixFlag_Rot = false;
		AddDesc.FollowTarget = Transfom;

		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
		MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

		// 2
		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_SM_GT_SphereHarf;

		// Time
		MeshDesc.fMaxTime_Duration = 0.6f;
		MeshDesc.fAppearTime = 0.3f;
		AddDesc.bAfterApperTime = false;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 317;
		MeshDesc.MaskTextureIndex = NONNMASK;
		MeshDesc.NoiseTextureIndex = 381;


		// Noise
		MeshDesc.noisingdir = _float2(0, 0).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 50.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);

		// Color
		MeshDesc.vLimLight = _float4(0.35f, 0.16f, 0.0f, 0.0f);
		MeshDesc.vLimLight = _float4(0.1f, 0.4f, 0.96f, 1.0f);
		MeshDesc.vEmissive = _float4(0.3f, 0.1f, 0.1f, 1.f);
		// MeshDesc.vEmissive = _float4(1);
		//	MeshDesc.vEmissive = _float4(0.1f, 0.3f,s 0.1f, 1.f);

			// Transform_Base
		MeshDesc.vPosition = _float3(-1.0f, 1.0f, -0.2f);
		MeshDesc.vSize = _float3(0.2f);

		// Move
		MeshDesc.MoveDir = FollowingDir_Look;
		MeshDesc.MoveSpeed = 0;
		AddDesc.AccMoveSpeed = 0;

		// RotS
		AddDesc.LookRotAxis = FollowingDir_Right;
		AddDesc.vAddDirectAngle = _float3(90, 0, 0);
		AddDesc.LookRotSpeed = 0;

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = -50;
		AddDesc.AccRotSpeed = 0;
		AddDesc.InitRot = _float3(0, 0, 0.0f);

		// Scale
		AddDesc.AccScaleSpeed = 1.0f;
		AddDesc.ScaleReFlag = false;

		AddDesc.bLockScale[0] = true;
		AddDesc.bLockScale[1] = true;
		AddDesc.bLockScale[2] = true;


		// Fix
		AddDesc.FixFlag_Move = false;
		AddDesc.FixFlag_Rot = false;
		AddDesc.FollowTarget = Transfom;

		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
		MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);





	}

	if (type == MESHEFFECT_MONSTER_NL_Cash3)
	{

	}

#pragma endregion MONSTER_NL



	return S_OK;
}



HRESULT CPartilceCreateMgr::Create_MeshEffectDesc_Hard_BOSS(E_MESH_EFFECTJ type, CTransform * Transfom)
{
	NONINSTNESHEFTDESC	MeshDesc;
	MeshDesc.eMeshType = Prototype_Mesh_SM_circle;

	MESHADDDATA			AddDesc;
	MESHAEASING*		pEaseDesc = nullptr;

	// RANDOM
	static bool bRandomVal_BOSS = true;
	bRandomVal_BOSS = !bRandomVal_BOSS;

#pragma region BOSS_Snake

	if (type == MESHEFFECT_BOSS_SNAKE_0)
	{
		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_circle_inv;
		// Time
		MeshDesc.fMaxTime_Duration = 1.0f;
		MeshDesc.fAppearTime = 0.0f;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 357;
		MeshDesc.MaskTextureIndex = 80;
		MeshDesc.NoiseTextureIndex = 323;
		// Noise
		MeshDesc.noisingdir = _float2(0.0f, 1.f).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		// Color
		MeshDesc.vLimLight = _float4(1.00f, 0.80f, 1.00f, 1.f);
		MeshDesc.vEmissive = _float4(1, 0.5f, 1.f, 1.f);

		// Transform_Base
		MeshDesc.vPosition = _float3(-0.0f, 30.0f, -3.0f);
		MeshDesc.vSize = _float3(1300);
		//		MeshDesc.vSize = _float3(30.0f);

		// Move
		//MeshDesc.MoveDir = FollowingDir_Up;
		//MeshDesc.MoveSpeed = 10.0f;
		//AddDesc.AccMoveSpeed = 110.0f;

		// Rot
		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 0.0f;
		AddDesc.AccRotSpeed = 0.0f;

		// Scale
	//	AddDesc.AccScale = 20.0f;
		AddDesc.AccScaleSpeed = 0.0f;
		AddDesc.ScaleReFlag = false;

		// Fix
		AddDesc.FixFlag_Move = true;
		AddDesc.FixFlag_Rot = false;

		// Shader
		//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
		MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);
	}


	if (type == MESHEFFECT_BOSS_SNAKE_1)
	{
		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_Sample_Mesh_Fake_Mesh_Lighting;
		// Time
		MeshDesc.fMaxTime_Duration = 0.4f;
		MeshDesc.fAppearTime = 0.2f;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 293;
		MeshDesc.MaskTextureIndex = 79;
		MeshDesc.NoiseTextureIndex = 294;
		// Noise
		MeshDesc.noisingdir = _float2(0.0f, 1.f).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		// Color
		MeshDesc.vLimLight = _float4(1, 1, 1, 1.f);
		MeshDesc.vEmissive = _float4(1, 0.5f, 1.f, 1.f);

		// Transform_Base
		MeshDesc.vPosition = _float3(-0.0f, 0.0f, -0.0f);
		MeshDesc.vSize = _float3(5.0f);

		// Move
		//MeshDesc.MoveDir = FollowingDir_Up;
		//MeshDesc.MoveSpeed = 10.0f;
		//AddDesc.AccMoveSpeed = 110.0f;

		// Rot
		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 0.0f;
		AddDesc.AccRotSpeed = 0.0f;

		// Scale
		AddDesc.AccScaleSpeed = 15.0f;
		AddDesc.ScaleReFlag = false;

		// Fix
		AddDesc.FixFlag_Move = true;

		// Shader
		//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
		MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

	}

	if (type == MESHEFFECT_BOSS_SNAKE_2)
	{
		//	// Mesh
		//	MeshDesc.eMeshType = Prototype_Mesh_Mesh_HFB_Grand_Git;
		//	// Time
		//	MeshDesc.fMaxTime_Duration = 1.0f;
		//	MeshDesc.fAppearTime = 0.2f;

		//	// Tex
		//	MeshDesc.iDiffuseTextureIndex = 335;
		//	MeshDesc.MaskTextureIndex = 33;
		//	MeshDesc.NoiseTextureIndex = 337;
		//	// Noise
		//	MeshDesc.noisingdir = _float2(0.5f, 0.5f).Get_Nomalize();
		//	MeshDesc.fDistortionNoisingPushPower = 0.0f;
		//	MeshDesc.vColor = _float4(1, 1, 1, 1);
		//	// Color
		//	MeshDesc.vLimLight = _float4(0.55f, 0.87f, 0.97f, 1.0f);
		////	MeshDesc.vLimLight = _float4(0, 0, 0, 1.0f);
		//	MeshDesc.vEmissive = _float4(1, 0.5f, 1.f, 1.f);

		//	// Transform_Base
		//	MeshDesc.vPosition = _float3(-0.0f, 0.0f, -0.0f);
		//	MeshDesc.vSize = _float3(20, 20, 800.0f);
		//	MeshDesc.vSize = _float3(200, 200, 200);

		//	// Move
		//	//MeshDesc.MoveDir = FollowingDir_Up;
		//	//MeshDesc.MoveSpeed = 10.0f;
		//	//AddDesc.AccMoveSpeed = 110.0f;

		//	// Rot
		//	AddDesc.LookRotAxis = FollowingDir_Look;
		//	AddDesc.vAddDirectAngle = _float3(90, 0, 0);

		//	MeshDesc.RotAxis = FollowingDir_Look;
		//	MeshDesc.RotationSpeedPerSec = 0.0f;
		//	AddDesc.AccRotSpeed = 0.0f;

		//	// Scale
		//	AddDesc.AccScale = 0.0;
		//	AddDesc.ScaleReFlag = false;

		//	// Fix
		//	AddDesc.FixFlag = true;

		//	// Shader
		//	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
		////	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
		////	MeshDesc.m_iPassIndex = 18; // DisCard
		//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå


		//	
		////	pEaseDesc = NEW MESHAEASING[1];
		////	pEaseDesc[0] = CreateEasingDesc(TYPE_SinInOut, _float3(0, -5.0f, 0), 0.5f);
		//	pEaseDesc[1] = CreateEasingDesc(TYPE_Linear, _float3(0, 0, 0), 1.0f);
		//	GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);
		////	GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom, pEaseDesc,1);

	}

	if (type == MESHEFFECT_BOSS_SNAKE_3)
	{

		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_Mesh_HFB_Grand_Git;
		// Time
		MeshDesc.fMaxTime_Duration = 1.0f;
		MeshDesc.fAppearTime = 0.2f;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 335;
		MeshDesc.MaskTextureIndex = 33;
		MeshDesc.NoiseTextureIndex = 337;
		// Noise
		MeshDesc.noisingdir = _float2(0.5f, 0.5f).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 0.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		// Color
		MeshDesc.vLimLight = _float4(0.55f, 0.87f, 0.97f, 1.0f);
		//	MeshDesc.vLimLight = _float4(0, 0, 0, 1.0f);
		MeshDesc.vEmissive = _float4(1, 0.5f, 1.f, 1.f);

		// Transform_Base
		MeshDesc.vPosition = _float3(-0.0f, 0.0f, -0.0f);
		MeshDesc.vSize = _float3(20, 20, 800.0f);
		MeshDesc.vSize = _float3(200, 200, 200);

		// Move
		//MeshDesc.MoveDir = FollowingDir_Up;
		//MeshDesc.MoveSpeed = 10.0f;
		//AddDesc.AccMoveSpeed = 110.0f;

		// Rot
		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.vAddDirectAngle = _float3(90, 0, 0);

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 0.0f;
		AddDesc.AccRotSpeed = 0.0f;

		// Scale
		AddDesc.AccScaleSpeed = 0.0;
		AddDesc.ScaleReFlag = false;

		// Fix
		AddDesc.FixFlag_Move = true;

		// Shader
		//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
		MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå



	//	pEaseDesc = NEW MESHAEASING[1];
	//	pEaseDesc[0] = CreateEasingDesc(TYPE_SinInOut, _float3(0, -5.0f, 0), 0.5f);
		pEaseDesc[1] = CreateEasingDesc(TYPE_Linear, _float3(0, 0, 0), 1.0f);
		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);
		//	GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom, pEaseDesc,1);

	}


#pragma endregion BOSS_Snake


#pragma region BOSS_Mahabalasura


	if (type == MESHEFFECT_BOSS_Mahabalasura_SKILLSPEAR_0)
	{

		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_MS_Tornado_Cyl_2side;

		// Time
		MeshDesc.fMaxTime_Duration = 0.5f;
		MeshDesc.fAppearTime = 0.3f;
		AddDesc.bAfterApperTime = true;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 302;
		MeshDesc.MaskTextureIndex = 75;
		MeshDesc.NoiseTextureIndex = 182;
		// Noise
		MeshDesc.noisingdir = _float2(0, -1).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 30.f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		// Color
		MeshDesc.vLimLight = _float4(0.42f, 0.84f, 0.81f, 1.0f);
		MeshDesc.vLimLight = _float4(1.0f,0.0f, 0.0f, 1.0f);
		//	MeshDesc.vLimLight = _float4(0, 0, 0, 1.0f);
		MeshDesc.vEmissive = _float4(0.8f, 1.f, 1.f, 1.f);

		// Transform_Base
		MeshDesc.vPosition = _float3(0);
		MeshDesc.vSize = _float3(15.0f, 30.0f, 15.f).XMVector()*0.4f;

		// Move
		//MeshDesc.MoveDir = FollowingDir_Up;
		//MeshDesc.MoveSpeed = 10.0f;
		//AddDesc.AccMoveSpeed = 110.0f;

		// Rot
		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 10.0f;
		AddDesc.InitRot = _float3(0, 90, 0);
		AddDesc.AccRotSpeed = 0.0f;


		// Scale
		AddDesc.AccScaleSpeed = 1.5f;
		AddDesc.ScaleReFlag = false;

		AddDesc.bAfterApperTime = false;
		AddDesc.bLockScale[0] = true;
		AddDesc.bLockScale[1] = false;
		AddDesc.bLockScale[2] = true;
		// Fix
		AddDesc.FixFlag_Move = true;

		// Shader
		//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
		MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå



	//	pEaseDesc = NEW MESHAEASING[1];
	//	pEaseDesc[0] = CreateEasingDesc(TYPE_SinInOut, _float3(0, -5.0f, 0), 0.5f);
	//	pEaseDesc[1] = CreateEasingDesc(TYPE_Linear, _float3(0, 0, 0), 1.0f);
		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);
		//	GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom, pEaseDesc,1);

	}


	if (type == MESHEFFECT_BOSS_Mahabalasura_SKILLSPEAR_1)
	{

		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_SM_4E_DarkTails_01;

		// Time
		MeshDesc.fMaxTime_Duration = 0.4f;
		MeshDesc.fAppearTime = 0.2f;
		AddDesc.bAfterApperTime = true;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 357;
		MeshDesc.MaskTextureIndex = 10;
		MeshDesc.NoiseTextureIndex = 346;
		// Noise
		MeshDesc.noisingdir = _float2(-1, 0).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		// Color
		MeshDesc.vLimLight = _float4(1.0f, 0.50f, 0.00f, 1.0f);
		//	MeshDesc.vLimLight = _float4(0, 0, 0, 1.0f);
		MeshDesc.vEmissive = _float4(1.0f, 0.7f, 1.f, 1.f);

		// Transform_Base
		MeshDesc.vPosition = _float3(-0.0f, 0.0f, -0.0f);
		MeshDesc.vSize = _float3(10.f, 40.0f, 10.f);

		// Move
		//MeshDesc.MoveDir = FollowingDir_Up;
		//MeshDesc.MoveSpeed = 10.0f;
		//AddDesc.AccMoveSpeed = 110.0f;

		// Rot
		AddDesc.LookRotAxis = FollowingDir_Up;
		AddDesc.vAddDirectAngle = _float3(0, 0, 90);

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 18.0f;
		AddDesc.AccRotSpeed = 0.0f;
		AddDesc.InitRot = _float3(0);

		// Scale
		AddDesc.AccScaleSpeed = -10.0f;
		AddDesc.ScaleReFlag = false;

		AddDesc.bLockScale[0] = true;
		AddDesc.bLockScale[1] = false;
		AddDesc.bLockScale[2] = true;

		// Fix
		AddDesc.FixFlag_Move = true;

		// Shader
		//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
		MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå



	//	pEaseDesc = NEW MESHAEASING[1];
	//	pEaseDesc[0] = CreateEasingDesc(TYPE_SinInOut, _float3(0, -5.0f, 0), 0.5f);
	//	pEaseDesc[1] = CreateEasingDesc(TYPE_Linear, _float3(0, 0, 0), 1.0f);
		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);
		//	GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom, pEaseDesc,1);

	}

	if (type == MESHEFFECT_BOSS_Mahabalasura_SKILLSPEAR_2)
	{
		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_SM_rockRing;
		// Time
		MeshDesc.fMaxTime_Duration = 0.5f;
		MeshDesc.fAppearTime = 0.3f;
		AddDesc.bAfterApperTime = true;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 276;
		MeshDesc.MaskTextureIndex = 74;
		MeshDesc.NoiseTextureIndex = 236;
		// Noise
		MeshDesc.noisingdir = _float2(1.0f, 0).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		// Color
		MeshDesc.vLimLight = _float4(0, 0.0f, 0.0f, 1.f);
		MeshDesc.vEmissive = _float4(1, 0.5f, 1.f, 1.f);

		// Transform_Base
		MeshDesc.vPosition = _float3(0.0f, -0.0f, 0.0f);
		MeshDesc.vSize = _float3(6);

		// Move
		//MeshDesc.MoveDir = FollowingDir_Up;
		//MeshDesc.MoveSpeed = 10.0f;
		//AddDesc.AccMoveSpeed = 110.0f;

		// Rot
		AddDesc.LookRotAxis = FollowingDir_Right;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 0.0f;
		AddDesc.AccRotSpeed = 0.0f;
		AddDesc.InitRot = _float3(0.0f, 0.f, 0.0f);

		// Scale
		AddDesc.AccScaleSpeed = 5.0f;
		AddDesc.ScaleReFlag = false;

		AddDesc.bLockScale[0] = true;
		AddDesc.bLockScale[1] = false;
		AddDesc.bLockScale[2] = true;


		// Fix
		AddDesc.FixFlag_Move = true;

		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
		MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå


		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);
	}


	if (type == MESHEFFECT_BOSS_Mahabalasura_0)
	{
		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_SM_rockRing;
		// Time
		MeshDesc.fMaxTime_Duration = 0.5f;
		MeshDesc.fAppearTime = 0.3f;
		AddDesc.bAfterApperTime = true;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 276;
		MeshDesc.MaskTextureIndex = 74;
		MeshDesc.NoiseTextureIndex = 236;
		// Noise
		MeshDesc.noisingdir = _float2(1.0f, 0).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		// Color
		MeshDesc.vLimLight = _float4(0, 0.0f, 0.0f, 1.f);
		MeshDesc.vEmissive = _float4(1, 0.5f, 1.f, 1.f);

		// Transform_Base
		MeshDesc.vPosition = _float3(0.0f, -0.0f, 0.0f);
		MeshDesc.vSize = _float3(6);

		// Move
		//MeshDesc.MoveDir = FollowingDir_Up;
		//MeshDesc.MoveSpeed = 10.0f;
		//AddDesc.AccMoveSpeed = 110.0f;

		// Rot
		AddDesc.LookRotAxis = FollowingDir_Right;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 0.0f;
		AddDesc.AccRotSpeed = 0.0f;
		AddDesc.InitRot = _float3(0.0f, 0.f, 0.0f);

		// Scale
		AddDesc.AccScaleSpeed = 5.0f;
		AddDesc.ScaleReFlag = false;

		AddDesc.bLockScale[0] = true;
		AddDesc.bLockScale[1] = false;
		AddDesc.bLockScale[2] = true;


		// Fix
		AddDesc.FixFlag_Move = true;

		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
		MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå


		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);
	}

	if (type == MESHEFFECT_BOSS_Mahabalasura_SKILLHAND_0)
	{
		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_SM_rockRing;

		// Time
		MeshDesc.fMaxTime_Duration = 10.0f;
		MeshDesc.fAppearTime = 5.0f;
		AddDesc.bAfterApperTime = true;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 321;
		MeshDesc.MaskTextureIndex = 31;
		MeshDesc.NoiseTextureIndex = 358;
		// Noise
		MeshDesc.noisingdir = _float2(0, 1).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		// Color
		MeshDesc.vLimLight = _float4(1.0f, 0.50f, 0.00f, 1.0f);
		//	MeshDesc.vLimLight = _float4(0, 0, 0, 1.0f);
		MeshDesc.vEmissive = _float4(1.0f, 0.7f, 1.f, 1.f);

		// Transform_Base
		MeshDesc.vPosition = _float3(-0.0f, -0, 1.3f);
		MeshDesc.vSize = _float3(15);

		// Move
		//MeshDesc.MoveDir = FollowingDir_Up;
		//MeshDesc.MoveSpeed = 10.0f;
		//AddDesc.AccMoveSpeed = 110.0f;

		// Rot
		AddDesc.LookRotAxis = FollowingDir_Up;
		AddDesc.vAddDirectAngle = _float3(0, 0, 90);

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 20.0f;
		AddDesc.AccRotSpeed = 0.0f;
		AddDesc.InitRot = _float3(0);

		// Scale
		AddDesc.AccScaleSpeed = 0.0f;
		AddDesc.ScaleReFlag = false;

		AddDesc.bLockScale[0] = true;
		AddDesc.bLockScale[1] = false;
		AddDesc.bLockScale[2] = true;

		// Fix
		AddDesc.FixFlag_Move = true;

		// Shader
		//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
		MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå



	//	pEaseDesc = NEW MESHAEASING[1];
	//	pEaseDesc[0] = CreateEasingDesc(TYPE_SinInOut, _float3(0, -5.0f, 0), 0.5f);
	//	pEaseDesc[1] = CreateEasingDesc(TYPE_Linear, _float3(0, 0, 0), 1.0f);
		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);
		//	GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom, pEaseDesc,1);

	}

	// Pattern 3
	if (type == MESHEFFECT_BOSS_Mahabalasura_SKILLRAIN_2)
	{
		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_SM_ky_tornado3;

		// Time
		MeshDesc.fMaxTime_Duration = 0.4f;
		MeshDesc.fAppearTime = 0.2f;
		AddDesc.bAfterApperTime = true;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 321;
		MeshDesc.MaskTextureIndex = 1;
		MeshDesc.NoiseTextureIndex = 318;
		// Noise
		MeshDesc.noisingdir = _float2(0, 1).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		// Color
		MeshDesc.vLimLight = _float4(1.0f, 0.50f, 0.00f, 1.0f);
		//	MeshDesc.vLimLight = _float4(0, 0, 0, 1.0f);
		MeshDesc.vEmissive = _float4(1.0f, 0.7f, 1.f, 1.f);

		// Transform_Base
		MeshDesc.vPosition = _float3(0, 0, 3);
		MeshDesc.vSize = _float3(30);

		// Move
		//MeshDesc.MoveDir = FollowingDir_Up;
		//MeshDesc.MoveSpeed = 10.0f;
		//AddDesc.AccMoveSpeed = 110.0f;

		// Rot
		AddDesc.LookRotAxis = FollowingDir_Up;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 40;
		AddDesc.AccRotSpeed = 0.0f;
		AddDesc.InitRot = _float3(0);

		// Scale
		AddDesc.AccScaleSpeed = 0.0f;
		AddDesc.ScaleReFlag = false;

		AddDesc.bLockScale[0] = false;
		AddDesc.bLockScale[1] = false;
		AddDesc.bLockScale[2] = false;

		// Fix
		AddDesc.FixFlag_Move = true;

		// Shader
		//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
		MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå



	//	pEaseDesc = NEW MESHAEASING[1];
	//	pEaseDesc[0] = CreateEasingDesc(TYPE_SinInOut, _float3(0, -5.0f, 0), 0.5f);
	//	pEaseDesc[1] = CreateEasingDesc(TYPE_Linear, _float3(0, 0, 0), 1.0f);
		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);
		//	GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom, pEaseDesc,1);

	}

	if (type == MESHEFFECT_BOSS_Mahabalasura_SKILLRAIN_1)
	{
		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_SM_EM_Copycat_03;
		//	Prototype_Mesh_SM_ky_windLine27midPoly

			// Time
		MeshDesc.fMaxTime_Duration = 0.6f;
		MeshDesc.fAppearTime = 0.3f;
		AddDesc.bAfterApperTime = true;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 292;
		MeshDesc.MaskTextureIndex = 31;
		MeshDesc.NoiseTextureIndex = 337;
		// Noise
		MeshDesc.noisingdir = _float2(-1, 0).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		// Color
		MeshDesc.vLimLight = _float4(1.0f, 0.50f, 0.00f, 1.0f);
		//	MeshDesc.vLimLight = _float4(0, 0, 0, 1.0f);
		MeshDesc.vEmissive = _float4(1.0f, 0.7f, 1.f, 1.f);

		// Transform_Base
		MeshDesc.vPosition = _float3(-0.0f, -3, 0.0f);
		MeshDesc.vSize = _float3(50);
		MeshDesc.vSize = _float3(50, 30, 30).XMVector();

		// Move
		//MeshDesc.MoveDir = FollowingDir_Up;
		//MeshDesc.MoveSpeed = 10.0f;
		//AddDesc.AccMoveSpeed = 110.0f;

		// Rot
		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.vAddDirectAngle = _float3(30, 0, 0);

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 0;
		AddDesc.AccRotSpeed = 0.0f;
		AddDesc.InitRot = _float3(0);

		// Scale
		AddDesc.AccScaleSpeed = 0.0f;
		AddDesc.ScaleReFlag = false;

		AddDesc.bLockScale[0] = false;
		AddDesc.bLockScale[1] = false;
		AddDesc.bLockScale[2] = false;

		// Fix
		AddDesc.FixFlag_Move = true;
		AddDesc.FixFlag_Rot = true;

		// Shader
		//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
		MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå



	//	pEaseDesc = NEW MESHAEASING[1];
	//	pEaseDesc[0] = CreateEasingDesc(TYPE_SinInOut, _float3(0, -5.0f, 0), 0.5f);
	//	pEaseDesc[1] = CreateEasingDesc(TYPE_Linear, _float3(0, 0, 0), 1.0f);
		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);
		//	GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom, pEaseDesc,1);

	}

	if (type == MESHEFFECT_BOSS_Mahabalasura_SKILLRAIN_0)
	{
		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_circle_inv;
		// Time
		MeshDesc.fMaxTime_Duration = 2.0f;
		MeshDesc.fAppearTime = 0.3f;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 302;
		MeshDesc.MaskTextureIndex = 80;
		MeshDesc.NoiseTextureIndex = 236;
		// Noise
		MeshDesc.noisingdir = _float2(0.0f, 1.f).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		// Color
		MeshDesc.vLimLight = _float4(0.65f, 0.6f, 0.3f, 1.f);
		MeshDesc.vEmissive = _float4(1, 0.5f, 1.f, 1.f);

		// Transform_Base
		MeshDesc.vPosition = _float3(-0.0f, 0, -3.1f);
		MeshDesc.vSize = _float3(80.0f);
		//		MeshDesc.vSize = _float3(30.0f);

		// Move
		//MeshDesc.MoveDir = FollowingDir_Up;
		//MeshDesc.MoveSpeed = 10.0f;
		//AddDesc.AccMoveSpeed = 110.0f;

		// Rot
		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.vAddDirectAngle = _float3(0, 90, 0);

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 0.0f;
		AddDesc.AccRotSpeed = 0.0f;

		// Scale
	//	AddDesc.AccScale = 20.0f;
		AddDesc.AccScaleSpeed = 0;
		AddDesc.ScaleReFlag = false;

		// Fix
		AddDesc.FixFlag_Move = true;

		// Shader
		//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
		MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);
	}

	// 4 
	if (type == MESHEFFECT_BOSS_Mahabalasura_SKILLCOPY_0)
	{
		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_circle_inv;
		// Time
		MeshDesc.fMaxTime_Duration = 0.7f;
		MeshDesc.fAppearTime = 0.2f;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 302;
		MeshDesc.MaskTextureIndex = 80;
		MeshDesc.NoiseTextureIndex = 236;
		// Noise
		MeshDesc.noisingdir = _float2(0.0f, 1.f).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		// Color
		MeshDesc.vLimLight = _float4(0.65f, 0.6f, 0.3f, 1.f);
		MeshDesc.vEmissive = _float4(1, 0.5f, 1.f, 1.f);

		// Transform_Base
		MeshDesc.vPosition = _float3(-0.0f, 0, -3.1f);
		MeshDesc.vSize = _float3(100);
		//		MeshDesc.vSize = _float3(30.0f);

		// Move
		//MeshDesc.MoveDir = FollowingDir_Up;
		//MeshDesc.MoveSpeed = 10.0f;
		//AddDesc.AccMoveSpeed = 110.0f;

		// Rot
		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.vAddDirectAngle = _float3(0, 90, 0);

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 0.0f;
		AddDesc.AccRotSpeed = 0.0f;

		// Scale
	//	AddDesc.AccScale = 20.0f;
		AddDesc.AccScaleSpeed = 0;
		AddDesc.ScaleReFlag = false;

		// Fix
		AddDesc.FixFlag_Move = true;

		// Shader
		//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
		MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);
	}


	if (type == MESHEFFECT_BOSS_Mahabalasura_SKILLCOPY_1)
	{
		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_SM_ky_lans_rot;
		// Time
		MeshDesc.fMaxTime_Duration = 0.3f;
		MeshDesc.fAppearTime = 0.3f;
		AddDesc.bAfterApperTime = true;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 321;
		MeshDesc.MaskTextureIndex = 65;
		MeshDesc.NoiseTextureIndex = 335;
		// Noise
		MeshDesc.noisingdir = _float2(0.0f, 1.f).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		// Color
		MeshDesc.vLimLight = _float4(0.65f, 1.0f, 1.0f, 1.f);
		MeshDesc.vEmissive = _float4(1, 0.5f, 1.f, 1.f);

		// Transform_Base
		MeshDesc.vPosition = _float3(0.0f, 0.f, 0.0f);
		//	MeshDesc.vPosition = _float3(0.0f, 0, 1.5f);
		MeshDesc.vSize = _float3(50);
		MeshDesc.vSize = _float3(65.0f, 60, 1.f).XMVector() * 1;

		// Move
		//MeshDesc.MoveDir = FollowingDir_Up;
		//MeshDesc.MoveSpeed = 10.0f;
		//AddDesc.AccMoveSpeed = 110.0f;

		// Rot
		AddDesc.LookRotAxis = FollowingDir_Up;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 0.0f;
		AddDesc.AccRotSpeed = 0.0f;
		AddDesc.InitRot = _float3(0.0f, 0.f, 0.0f);

		// Scale
		AddDesc.AccScaleSpeed = 0.0f;
		AddDesc.ScaleReFlag = false;

		AddDesc.bLockScale[0] = true;
		AddDesc.bLockScale[1] = false;
		AddDesc.bLockScale[2] = true;


		// Fix
		AddDesc.FixFlag_Move = true;

		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
		MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå


		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);
	}
	


	if (type == MESHEFFECT_BOSS_Mahabalasura_PLANE1)
	{

	}

	if (type == MESHEFFECT_BOSS_Mahabalasura_PLANE2)
	{
		MeshDesc.eMeshType = Prototype_Mesh_SM_circle;

		MeshDesc.fMaxTime_Duration = 5.0f;

		MeshDesc.iDiffuseTextureIndex = 36;
		MeshDesc.MaskTextureIndex = 21;
		MeshDesc.NoiseTextureIndex = 357;

		MeshDesc.noisingdir = _float2(0.0f, 1.0f).Get_Nomalize();
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		MeshDesc.fAppearTime = 1.0f;
		MeshDesc.fDistortionNoisingPushPower = 10.0f;

		MeshDesc.vLimLight = _float4(0.02f, 0.16f, 0.90f, 1.f);
		MeshDesc.vEmissive = _float4(0.7f, 0.9f, 0.5f, 1.f);


		//	MeshDesc.vRotAxis = _float3(1, 0, 0);
		MeshDesc.vPosition = _float3(-0.0f, 0.15f, -0.0f);
		MeshDesc.vSize = _float3(70.0f*0.5f);

		MeshDesc.RotAxis = FollowingDir_Up;
		MeshDesc.RotationSpeedPerSec = 40.0f;

		//	_float3 Pos = Transfom->Get_MatrixState(CTransform::STATE_POS);
		//	_float3 dir =  Transfom->Get_MatrixState(CTransform::STATE_UP);

		MeshDesc.vLookDir = _float3(1, 0, 0);


		//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
		//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
		//	MeshDesc.m_iPassIndex = 18; // DisCard
		MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		AddDesc.LookRotAxis = FollowingDir_Right;
		AddDesc.FixFlag_Move = true;
		AddDesc.FollowTarget = Transfom;

		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

	}


	if (type == MESHEFFECT_BOSS_Mahabalasura_PLANE3)
	{
		MeshDesc.eMeshType = Prototype_Mesh_MS_ST3_Portal_00;

		MeshDesc.fMaxTime_Duration = 10.0f;

		MeshDesc.iDiffuseTextureIndex = 236;
		MeshDesc.iDiffuseTextureIndex = 33;
		MeshDesc.MaskTextureIndex = 31;
		MeshDesc.NoiseTextureIndex = 325;

		MeshDesc.noisingdir = _float2(0.0f, 1.0f).Get_Nomalize();
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		MeshDesc.fAppearTime = 1.f;
		MeshDesc.fDistortionNoisingPushPower = 5.0f;

		MeshDesc.vLimLight = _float4(0.03f, 0.06f, 0.82f, 1.f);
		MeshDesc.vEmissive = _float4(0.03f, 0.06f, 0.82f, 1.f);


		//	MeshDesc.vRotAxis = _float3(1, 0, 0);
		MeshDesc.vPosition = _float3(-0.0f, 0.15f, -0.0f);
		MeshDesc.vSize = _float3(10.5f*0.5f);

		MeshDesc.RotAxis = FollowingDir_Up;
		MeshDesc.RotationSpeedPerSec = 15.0f;

		//	_float3 Pos = Transfom->Get_MatrixState(CTransform::STATE_POS);
		//	_float3 dir =  Transfom->Get_MatrixState(CTransform::STATE_UP);

		MeshDesc.vLookDir = _float3(1, 0, 0);


		//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
		MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
									//	MeshDesc.m_iPassIndex = 18; // DisCardB
									//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		AddDesc.LookRotAxis = FollowingDir_Right;
		AddDesc.FixFlag_Move = true;
		AddDesc.AccRotSpeed = -10.0f;
		AddDesc.FollowTarget = Transfom;

		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

	}


#pragma endregion BOSS_Mahabalasura


#pragma region BOSS_Rangda 



	if (type == MESHEFFECT_BOSS_Rangda_0)
	{
	
		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_SM_BBT_Paw;

		// Time
		MeshDesc.fMaxTime_Duration = 2.4f;
		MeshDesc.fAppearTime = 1.2f;
		AddDesc.bAfterApperTime = true;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 302;
		MeshDesc.MaskTextureIndex = 103;
		MeshDesc.NoiseTextureIndex = 184;
		// Noise
		MeshDesc.noisingdir = _float2(0, 1).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		// Color
		MeshDesc.vLimLight = _float4(1.0f, 0.0f, 0.00f, 1.0f);
		//	MeshDesc.vLimLight = _float4(0, 0, 0, 1.0f);
		MeshDesc.vEmissive = _float4(1.0f, 0.7f, 1.f, 1.f);

		// Transform_Base
		MeshDesc.vPosition = _float3(0, -1, 13);
		MeshDesc.vSize = _float3(18);

		// Move
		//MeshDesc.MoveDir = FollowingDir_Up;
		//MeshDesc.MoveSpeed = 10.0f;
		//AddDesc.AccMoveSpeed = 110.0f;

		// Rot
		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.vAddDirectAngle = _float3(0, 30, 0);

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 0;
		AddDesc.AccRotSpeed = 0.0f;
		//	AddDesc.InitRot = _float3(0, 32, 0);


			// Scale
		AddDesc.AccScaleSpeed = 0;
		AddDesc.ScaleReFlag = false;

		AddDesc.bLockScale[0] = true;
		AddDesc.bLockScale[1] = true;
		AddDesc.bLockScale[2] = true;

		// Fix
		AddDesc.FixFlag_Move = true;
		AddDesc.FixFlag_Rot = false;

		AddDesc.FollowTarget = nullptr;

		// Shader
		//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
		MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå


		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

		AddDesc.vAddDirectAngle = _float3(0, 210, 0);
		MeshDesc.vPosition = _float3(-2, -1, 10);
		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);


	}

	if (type == MESHEFFECT_BOSS_Rangda_1)
	{
		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_SM_4E_ImpactFX_02;
		
		// Time
		MeshDesc.fMaxTime_Duration = 2.4f;
		MeshDesc.fAppearTime = 1.2f;
		AddDesc.bAfterApperTime = true;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 54;
		MeshDesc.MaskTextureIndex = 101;
		MeshDesc.NoiseTextureIndex = 211;
		// Noise
		MeshDesc.noisingdir = _float2(0, 1).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		// Color
		MeshDesc.vLimLight = _float4(1.0f, 0.0f, 0.00f, 1.0f);
		//	MeshDesc.vLimLight = _float4(0, 0, 0, 1.0f);
		MeshDesc.vEmissive = _float4(1.0f, 0.7f, 1.f, 1.f);

		// Transform_Base
		MeshDesc.vPosition = _float3(0, -1, 0);
		MeshDesc.vSize = _float3(10);

		// Move
		//MeshDesc.MoveDir = FollowingDir_Up;
		//MeshDesc.MoveSpeed = 10.0f;
		//AddDesc.AccMoveSpeed = 110.0f;

		// Rot
		AddDesc.LookRotAxis = FollowingDir_Right;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 30;
		AddDesc.AccRotSpeed = 0.0f;
		//	AddDesc.InitRot = _float3(0, 32, 0);


			// Scale
		AddDesc.AccScaleSpeed = 0.0f;
		AddDesc.ScaleReFlag = false;

		AddDesc.bLockScale[0] = false;
		AddDesc.bLockScale[1] = false;
		AddDesc.bLockScale[2] = false;

		// Fix
		AddDesc.FixFlag_Move = false;
		AddDesc.FixFlag_Rot = false;

		AddDesc.FollowTarget = Transfom;

		// Shader
		//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
		MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå



	//	pEaseDesc = NEW MESHAEASING[1];
	//	pEaseDesc[0] = CreateEasingDesc(TYPE_SinInOut, _float3(0, -5.0f, 0), 0.5f);
	//	pEaseDesc[1] = CreateEasingDesc(TYPE_Linear, _float3(0, 0, 0), 1.0f);
		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);
		//	GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom, pEaseDesc,1);

	}

	if (type == MESHEFFECT_BOSS_Rangda_2)
	{

		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_MS_Tornado_Cyl_2side;
		
		// Time
		MeshDesc.fMaxTime_Duration = 2.0f;
		MeshDesc.fAppearTime = 1.0f;
		AddDesc.bAfterApperTime = true;
		MeshDesc.fMaxTime_Duration = 0.8f;
		MeshDesc.fAppearTime = 0.4f;
		AddDesc.bAfterApperTime = false;
		// Tex
		MeshDesc.iDiffuseTextureIndex = 331;
		MeshDesc.MaskTextureIndex = 103;
		MeshDesc.NoiseTextureIndex = 211;

		// Noise
		MeshDesc.noisingdir = _float2(0, 1).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		// Color
		MeshDesc.vLimLight = _float4(1.0f, 0.0f, 0.00f, 1.0f);
		//	MeshDesc.vLimLight = _float4(0, 0, 0, 1.0f);
		MeshDesc.vEmissive = _float4(1.0f, 0.7f, 1.f, 1.f);

		// Transform_Base
		MeshDesc.vPosition = _float3(0, -3, 12);
		MeshDesc.vSize = _float3(5);

		// Move
		//MeshDesc.MoveDir = FollowingDir_Up;
		//MeshDesc.MoveSpeed = 10.0f;
		//AddDesc.AccMoveSpeed = 110.0f;

		// Rot
		AddDesc.LookRotAxis = FollowingDir_Right;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 40;
		AddDesc.AccRotSpeed = 0.0f;
		//	AddDesc.InitRot = _float3(0, 32, 0);


			// Scale
		AddDesc.AccScaleSpeed = 3.0f;
		AddDesc.ScaleReFlag = false;

		AddDesc.bLockScale[0] = true;
		AddDesc.bLockScale[1] = false;
		AddDesc.bLockScale[2] = true;

		// Fix
		AddDesc.FixFlag_Move = true;
		AddDesc.FixFlag_Rot = false;

	//	AddDesc.FollowTarget = Transfom;

		// Shader
		MeshDesc.m_iPassIndex = 16; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
		MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå



	//	pEaseDesc = NEW MESHAEASING[1];
	//	pEaseDesc[0] = CreateEasingDesc(TYPE_SinInOut, _float3(0, -5.0f, 0), 0.5f);
	//	pEaseDesc[1] = CreateEasingDesc(TYPE_Linear, _float3(0, 0, 0), 1.0f);
		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);
		//	GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom, pEaseDesc,1);

	}



	if (type == MESHEFFECT_BOSS_Rangda_3)
	{

		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_circle;

		// Time
		MeshDesc.fMaxTime_Duration = 1.4f;
		MeshDesc.fAppearTime = 0.7f;
		AddDesc.bAfterApperTime = true;
		// Tex
		MeshDesc.iDiffuseTextureIndex = 302;
		MeshDesc.MaskTextureIndex = NONNMASK;
		MeshDesc.NoiseTextureIndex = 211;

		// Noise
		MeshDesc.noisingdir = _float2(0, 1).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		// Color
		MeshDesc.vLimLight = _float4(1.0f, 0.0f, 0.00f, 1.0f);
		//	MeshDesc.vLimLight = _float4(0, 0, 0, 1.0f);
		MeshDesc.vEmissive = _float4(1.0f, 0.7f, 1.f, 1.f);

		// Transform_Base
		MeshDesc.vPosition = _float3(0, 0, 0);
		MeshDesc.vSize = _float3(10);

		// Move
		//MeshDesc.MoveDir = FollowingDir_Up;
		//MeshDesc.MoveSpeed = 10.0f;
		//AddDesc.AccMoveSpeed = 110.0f;

		// Rot
		AddDesc.LookRotAxis = FollowingDir_Right;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 35;
		AddDesc.AccRotSpeed = 0.0f;
		//	AddDesc.InitRot = _float3(0, 32, 0);


			// Scale
		AddDesc.AccScaleSpeed = 0.7f;
		AddDesc.ScaleReFlag = false;

		AddDesc.bLockScale[0] = true;
		AddDesc.bLockScale[1] = false;
		AddDesc.bLockScale[2] = true;

		// Fix
		AddDesc.FixFlag_Move = true;
		AddDesc.FixFlag_Rot = false;

		//	AddDesc.FollowTarget = Transfom;

			// Shader
		MeshDesc.m_iPassIndex = 16; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
		MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);


	}

	if (type == MESHEFFECT_BOSS_Rangda_4)

	{
		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_SM_AOG_Ring;

		// Time
		MeshDesc.fMaxTime_Duration = 1.5f;
		MeshDesc.fAppearTime = 0.75f;
		AddDesc.bAfterApperTime = true;


		// Tex
		MeshDesc.iDiffuseTextureIndex = 301;
		MeshDesc.MaskTextureIndex = NONNMASK;
		MeshDesc.NoiseTextureIndex = NONNOISE;

		// Noise
		MeshDesc.noisingdir = _float2(0, 1).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		// Color
		MeshDesc.vLimLight = _float4(1.0f, 0.0f, 0.00f, 1.0f);
		//	MeshDesc.vLimLight = _float4(0, 0, 0, 1.0f);
		MeshDesc.vEmissive = _float4(1.0f, 0.7f, 1.f, 1.f);

		// Transform_Base
		MeshDesc.vPosition = _float3(0, 0, 0);
		MeshDesc.vSize = _float3(1);

		// Move
		//MeshDesc.MoveDir = FollowingDir_Up;
		//MeshDesc.MoveSpeed = 10.0f;
		//AddDesc.AccMoveSpeed = 110.0f;

		// Rot
		AddDesc.LookRotAxis = FollowingDir_Right;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 30;
		AddDesc.AccRotSpeed = -50;
		//	AddDesc.InitRot = _float3(0, 32, 0);


			// Scale
		AddDesc.AccScaleSpeed = 0;
		AddDesc.ScaleReFlag = false;

		AddDesc.bLockScale[0] = true;
		AddDesc.bLockScale[1] = false;
		AddDesc.bLockScale[2] = true;

		// Fix
		AddDesc.FixFlag_Move = true;
		AddDesc.FixFlag_Rot = false;

		//	AddDesc.FollowTarget = Transfom;

			// Shader
		MeshDesc.m_iPassIndex = 16; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå


		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);


	}


#pragma endregion BOSS_Rangda 

#pragma region BOSS_Chiedtian  



#pragma endregion BOSS_Chiedtian  








	return S_OK;
}


void CPartilceCreateMgr::Free()
{
	

}
