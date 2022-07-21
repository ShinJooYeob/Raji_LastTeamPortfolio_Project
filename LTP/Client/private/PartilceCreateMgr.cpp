#include "stdafx.h"
#include "..\public\PartilceCreateMgr.h"
#include "..\public\NonInstanceMeshEffect_TT.h"

#define  NONNOISE	388
#define  NONNMASK	50
IMPLEMENT_SINGLETON(CPartilceCreateMgr);

CPartilceCreateMgr::CPartilceCreateMgr()
{}

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

HRESULT CPartilceCreateMgr::Create_MeshInst_DESC(INSTMESHDESC desc, _uint scene)
{

	FAILED_CHECK(GetSingle(CUtilityMgr)->Create_MeshInstance(scene, desc));

	return S_OK;
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

HRESULT CPartilceCreateMgr::Create_MeshEffectDesc_Hard(E_MESH_EFFECTJ type , CTransform* Transfom)
{
	if (type <= MESHEFFECT_PRE_END)
		return E_FAIL;

	MESHADDDATA			AddDesc;
	MESHAEASING*		pEaseDesc = nullptr;

	// RANDOM
	static bool bRandomVal  = true;
	static bool bRandomVal1 = true;
	static bool bRandomVal2 = true;
	static bool bRandomVal3 = true;
	static bool bRandomVal4 = true;
	static bool bRandomVal5 = true;
	static bool bRandomVal6 = true;

	bRandomVal = !bRandomVal;
	bRandomVal1 = !bRandomVal1;
	bRandomVal2 = !bRandomVal2;
	bRandomVal3 = !bRandomVal3;
	bRandomVal4 = !bRandomVal4;
	bRandomVal5 = !bRandomVal5;


#pragma region BOW

	// ÇÏµå ÄÚµùÀ¸·Î Á¦ÀÛ
	if (type == MESHEFFECT_ARROW_HEAD)
	{
		NONINSTNESHEFTDESC MeshDesc = GetSingle(CPartilceCreateMgr)->Get_TypeDesc_NonInstacne(CPartilceCreateMgr::MESHEFFECT_PRE_IMPECTFX_02);

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

		MeshDesc.RotAxis= FollowingDir_Right;
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
		NONINSTNESHEFTDESC MeshDesc = GetSingle(CPartilceCreateMgr)->Get_TypeDesc_NonInstacne(CPartilceCreateMgr::MESHEFFECT_PRE_BOW1);

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
		AddDesc.vAddDirectAngle = _float3(0,0,0); 

		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);


	}
	if (type == MESHEFFECT_ARROW_BOW_UP)
	{

		NONINSTNESHEFTDESC	MeshDesc = GetSingle(CPartilceCreateMgr)->Get_TypeDesc_NonInstacne(CPartilceCreateMgr::MESHEFFECT_PRE_BOW1);

		MeshDesc.fMaxTime_Duration = 3.0f;

		MeshDesc.iDiffuseTextureIndex = 330;
		MeshDesc.MaskTextureIndex = 8;
		MeshDesc.NoiseTextureIndex = 328;

		MeshDesc.noisingdir = _float2(0.0f, 1.0f).Get_Nomalize();
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		MeshDesc.fAppearTime = 1.5f;
		MeshDesc.fDistortionNoisingPushPower = 3.0f;

		MeshDesc.vLimLight = _float4(0.0f, 0.40f, 0.89f, 1.0f);
		MeshDesc.vEmissive = _float4(0.6f, 0.5f, 0.0f,0.0f);


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
		AddDesc.vAddDirectAngle = _float3(0,270,0);

		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc,Transfom);
	}


	if (type == MESHEFFECT_ARROW_BOW_SHIFT_PLANE)
	{

		NONINSTNESHEFTDESC	MeshDesc = GetSingle(CPartilceCreateMgr)->Get_TypeDesc_NonInstacne(CPartilceCreateMgr::MESHEFFECT_PRE_CIRCLE);

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

		NONINSTNESHEFTDESC	MeshDesc = GetSingle(CPartilceCreateMgr)->Get_TypeDesc_NonInstacne(CPartilceCreateMgr::MESHEFFECT_PRE_ICE);

		MeshDesc.fMaxTime_Duration = 1.5f;

		MeshDesc.iDiffuseTextureIndex = 363;
		MeshDesc.MaskTextureIndex = 0;
		MeshDesc.NoiseTextureIndex = 285;

		MeshDesc.noisingdir = _float2(0.5f, 1.5f).Get_Nomalize();
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		MeshDesc.fAppearTime = 1.0f;
		MeshDesc.fDistortionNoisingPushPower = 10.0f;

		MeshDesc.vLimLight = _float4(0.11f, 0.26f, 0.64f, 0.3f);
		MeshDesc.vEmissive = _float4(1.0f, 0.f, 0.f,0.f);


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
		NONINSTNESHEFTDESC	MeshDesc = GetSingle(CPartilceCreateMgr)->Get_TypeDesc_NonInstacne(CPartilceCreateMgr::MESHEFFECT_PRE_BOW2);

		MeshDesc.fMaxTime_Duration = 0.18f;

		MeshDesc.iDiffuseTextureIndex = 364;
		MeshDesc.MaskTextureIndex = 57;
		MeshDesc.NoiseTextureIndex = 366;

		MeshDesc.noisingdir = _float2(0.5f, 1.5f).Get_Nomalize();
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		MeshDesc.fAppearTime = 0.0f;
		MeshDesc.fDistortionNoisingPushPower = 1.0f;



		MeshDesc.vLimLight = bRandomVal?_float4(0.56f, 0.97f, 0.98f, 1.0f):_float4(0.58f, 0.99f, 1.00f, 1.0f);
		MeshDesc.vEmissive = _float4(1.0f, 0.f, 0.f, 0.f);


		//	MeshDesc.vRotAxis = _float3(1, 0, 0);
		MeshDesc.vPosition= _float3(-0.0f, 0, -0.0f);
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
		AddDesc.vAddDirectAngle = _float3(0,-30,0);
		
		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

	}

	

	if (type == MESHEFFECT_ARROW_BOW_R_JUMP_WING1)
	{
		NONINSTNESHEFTDESC	MeshDesc = GetSingle(CPartilceCreateMgr)->Get_TypeDesc_NonInstacne(CPartilceCreateMgr::MESHEFFECT_PRE_WING);

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
		NONINSTNESHEFTDESC	MeshDesc = GetSingle(CPartilceCreateMgr)->Get_TypeDesc_NonInstacne(CPartilceCreateMgr::MESHEFFECT_PRE_WING);

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
		NONINSTNESHEFTDESC	MeshDesc = GetSingle(CPartilceCreateMgr)->Get_TypeDesc_NonInstacne(CPartilceCreateMgr::MESHEFFECT_PRE_Sphere);
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
		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom, pEaseDesc,2);
	}

	if (type == MESHEFFECT_ARROW_BOW_SP_BOM_SPEHERE)
	{
		NONINSTNESHEFTDESC	MeshDesc = GetSingle(CPartilceCreateMgr)->Get_TypeDesc_NonInstacne(CPartilceCreateMgr::MESHEFFECT_PRE_SM_sphere_melon);

		// MAX

		MeshDesc.fMaxTime_Duration = 7.0f;

		MeshDesc.iDiffuseTextureIndex = 44;
		MeshDesc.MaskTextureIndex = 20;
		MeshDesc.NoiseTextureIndex = 30;

		MeshDesc.noisingdir = _float2(0.f,-1.0f).Get_Nomalize();
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
		AddDesc.FixFlag = true;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);

		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

	}

	if (type == MESHEFFECT_ARROW_BOW_SP_PLANE)
	{
		NONINSTNESHEFTDESC	MeshDesc = GetSingle(CPartilceCreateMgr)->Get_TypeDesc_NonInstacne(CPartilceCreateMgr::MESHEFFECT_PRE_RING);

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
		AddDesc.FixFlag = true;
		AddDesc.AccRotSpeed = -5.0f;

		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

	}

	if (type == MESHEFFECT_ARROW_BOW_SP_PLANE2)
	{
		NONINSTNESHEFTDESC	MeshDesc = GetSingle(CPartilceCreateMgr)->Get_TypeDesc_NonInstacne(CPartilceCreateMgr::MESHEFFECT_PRE_CIRCLE);

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
		AddDesc.FixFlag = true;

		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

	}


	if (type == MESHEFFECT_ARROW_BOW_SP_PLANE3)
	{
		NONINSTNESHEFTDESC	MeshDesc = GetSingle(CPartilceCreateMgr)->Get_TypeDesc_NonInstacne(CPartilceCreateMgr::MESHEFFECT_PRE_POTAL);

		MeshDesc.fMaxTime_Duration = 6.0f;

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
		AddDesc.FixFlag = true;
		AddDesc.AccRotSpeed = -10.0f;

		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

	}

	if (type == MESHEFFECT_ARROW_BOW_SP_BOW)
	{
		NONINSTNESHEFTDESC	MeshDesc = GetSingle(CPartilceCreateMgr)->Get_TypeDesc_NonInstacne(CPartilceCreateMgr::MESHEFFECT_PRE_BOW2);

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
		NONINSTNESHEFTDESC	MeshDesc = GetSingle(CPartilceCreateMgr)->Get_TypeDesc_NonInstacne(CPartilceCreateMgr::MESHEFFECT_PRE_CONMESH);

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
		MeshDesc.vSize = _float3(1.2f,0.8f,1.2f);

		MeshDesc.RotAxis = FollowingDir_Up;
		MeshDesc.RotationSpeedPerSec = 50.0f;

		MeshDesc.vLookDir = _float3(0, 0, 1);


		//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
			//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
		//	MeshDesc.m_iPassIndex = 18; // DisCard
		MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.FixFlag = true;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);
		AddDesc.AccRotSpeed = 5.0f;

		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

	}

	if (type == MESHEFFECT_ARROW_BOW_SP_ICES)
	{
		NONINSTNESHEFTDESC	MeshDesc = GetSingle(CPartilceCreateMgr)->Get_TypeDesc_NonInstacne(CPartilceCreateMgr::MESHEFFECT_PRE_Ice2);

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
		AddDesc.FixFlag = true;

		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);


	}

#pragma endregion BOW

#pragma region MONSTER

	if (type == MESHEFFECT_MONSTER_CREATE1)
	{
		NONINSTNESHEFTDESC	MeshDesc = GetSingle(CPartilceCreateMgr)->Get_TypeDesc_NonInstacne(CPartilceCreateMgr::MESHEFFECT_PRE_SM_Ring_03);

		MeshDesc.fMaxTime_Duration = 0.8f;

		MeshDesc.iDiffuseTextureIndex = 372;
		MeshDesc.MaskTextureIndex = 55;
		MeshDesc.NoiseTextureIndex = 374;

		MeshDesc.noisingdir = _float2(0.0f, 1.f).Get_Nomalize();
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		MeshDesc.fAppearTime = 0.2f;
		MeshDesc.fDistortionNoisingPushPower = 10.0f;

		MeshDesc.vLimLight = _float4(1, 1, 1, 1.f);
		MeshDesc.vEmissive = _float4(1, 1, 1, 1.f);


		//	MeshDesc.vRotAxis = _float3(1, 0, 0);
		MeshDesc.vPosition = _float3(-0.0f, 0, -0.0f);
		MeshDesc.vSize = _float3(1.0f);

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
		NONINSTNESHEFTDESC	MeshDesc = GetSingle(CPartilceCreateMgr)->Get_TypeDesc_NonInstacne(CPartilceCreateMgr::MESHEFFECT_PRE_SM_Ring_03);

		MeshDesc.fMaxTime_Duration = 0.8f;

		MeshDesc.iDiffuseTextureIndex = 372;
		MeshDesc.MaskTextureIndex = 55;
		MeshDesc.NoiseTextureIndex = 374;

		MeshDesc.noisingdir = _float2(0.0f, 1.f).Get_Nomalize();
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		MeshDesc.fAppearTime = 0.2f;
		MeshDesc.fDistortionNoisingPushPower = 10.0f;

		MeshDesc.vLimLight = _float4(1, 1, 1, 1.f);
		MeshDesc.vEmissive = _float4(1, 1, 1, 1.f);


		//	MeshDesc.vRotAxis = _float3(1, 0, 0);
		MeshDesc.vPosition = _float3(-0.0f, 0, -0.0f);
		MeshDesc.vSize = _float3(1.0f);

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







#pragma endregion MONSTER

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

#define MESHINPORT(x,y)						MeshEffectDESC.eMeshType = x; mVecMeshEffectDesc[y] = MeshEffectDESC;

HRESULT CPartilceCreateMgr::Ready_MeshEffect()
{
	// #MESHPARTILCEINIT
	mVecMeshEffectDesc.resize((int)MESHEFFECT_END);

	NONINSTNESHEFTDESC MeshEffectDESC;
	MESHINPORT(Prototype_Mesh_ConeMesh, MESHEFFECT_PRE_CONMESH);
	MESHINPORT(Prototype_Mesh_CIRCLE, MESHEFFECT_PRE_CIRCLE);
	MESHINPORT(Prototype_Mesh_CIRCLE_DIST5, MESHEFFECT_PRE_CIRCLE_DIST4);
	MESHINPORT(Prototype_Mesh_CIRCLE_DIST4, MESHEFFECT_PRE_CIRCLE_DIST5);
	MESHINPORT(Prototype_Mesh_IMPECTFX_02, MESHEFFECT_PRE_IMPECTFX_02);
	MESHINPORT(Prototype_Mesh_RING, MESHEFFECT_PRE_RING);


	MESHINPORT(Prototype_Mesh_LOVE, MESHEFFECT_PRE_LOVE);
	MESHINPORT(Prototype_Mesh_WING, MESHEFFECT_PRE_WING);
	MESHINPORT(Prototype_Mesh_BOW1, MESHEFFECT_PRE_BOW1);
	MESHINPORT(Prototype_Mesh_BOW2, MESHEFFECT_PRE_BOW2);
	MESHINPORT(Prototype_Mesh_ICE, MESHEFFECT_PRE_ICE);
	MESHINPORT(Prototype_Mesh_Sphere, MESHEFFECT_PRE_Sphere);
	MESHINPORT(Prototype_Mesh_SM_sphere_melon, MESHEFFECT_PRE_SM_sphere_melon);
	MESHINPORT(Prototype_Mesh_SM_4E_IceSpike_01, MESHEFFECT_PRE_SM_4E_IceSpike_01);
	MESHINPORT(Prototype_Mesh_MoonStone00, MESHEFFECT_PRE_MoonStone00);
	MESHINPORT(Prototype_Mesh_MoonStone02, MESHEFFECT_PRE_MoonStone04);
	MESHINPORT(Prototype_Mesh_MoonStone04, MESHEFFECT_PRE_MoonStone04);
	MESHINPORT(Prototype_Mesh_PurpCrystal01, MESHEFFECT_PRE_PurpCrystal01);
	MESHINPORT(Prototype_Mesh_Rock001, MESHEFFECT_PRE_Rock001);
	MESHINPORT(Prototype_Mesh_Rock002, MESHEFFECT_PRE_Rock002);
	MESHINPORT(Prototype_Mesh_Rock003, MESHEFFECT_PRE_Rock003);
	MESHINPORT(Prototype_Mesh_Rock004, MESHEFFECT_PRE_Rock004);
	MESHINPORT(Prototype_Mesh_Rock005, MESHEFFECT_PRE_Rock005);
	MESHINPORT(Prototype_Mesh_SM_4E_Ice2, MESHEFFECT_PRE_Ice2);
	MESHINPORT(Prototype_Mesh_POTAL, MESHEFFECT_PRE_POTAL);
	MESHINPORT(Prototype_Mesh_SM_Ring_03, MESHEFFECT_PRE_SM_Ring_03);
	MESHINPORT(Prototype_Mesh_SM_Helix_Crystal,  MESHEFFECT_PRE_SM_Helix_Crystal);
	MESHINPORT(Prototype_Mesh_SM_Karin_Weapon_03 , MESHEFFECT_PRE_SM_Karin_Weapon_03);
	MESHINPORT(Prototype_Mesh_SM_ky_tornado6,  MESHEFFECT_PRE_SM_ky_tornado6);
	MESHINPORT(Prototype_Mesh_SM_ky_sphere_dist_wind,  MESHEFFECT_PRE_SM_ky_sphere_dist_wind);
	MESHINPORT(Prototype_Mesh_SM_L_DS_Dashcombo_Y_L,  MESHEFFECT_PRE_SM_L_DS_Dashcombo_Y_L);
	MESHINPORT(Prototype_Mesh_SM_Reorgea_Hand_R_New_3 , MESHEFFECT_PRE_SM_Reorgea_Hand_R_New_3);




	return S_OK;
}

HRESULT CPartilceCreateMgr::Ready_TextureEffect()
{
	// #TEXTUREPARTILCEINIT
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


void CPartilceCreateMgr::Free()
{
	

}
