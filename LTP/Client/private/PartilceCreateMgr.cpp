#include "stdafx.h"
#include "..\public\PartilceCreateMgr.h"


IMPLEMENT_SINGLETON(CPartilceCreateMgr);

CPartilceCreateMgr::CPartilceCreateMgr()
{}

HRESULT CPartilceCreateMgr::Initialize_ParticleMgr()
{
	// 
	mVecMeshEffectDesc.resize((int)MESHEFFECT_END);

	// BASE TYPE
	CNonInstanceMeshEffect::NONINSTNESHEFTDESC tNIMEDesc;
	tNIMEDesc.eMeshType = Prototype_Mesh_ConeMesh;
	tNIMEDesc.fMaxTime_Duration = 10.f;
	mVecMeshEffectDesc[MESHEFFECT_PRE_CONMESH] = tNIMEDesc;

	tNIMEDesc.eMeshType = Prototype_Mesh_CIRCLE;
	mVecMeshEffectDesc[MESHEFFECT_PRE_CIRCLE] = tNIMEDesc;

	tNIMEDesc.eMeshType = Prototype_Mesh_CIRCLE_DIST5;
	mVecMeshEffectDesc[MESHEFFECT_PRE_CIRCLE_DIST4] = tNIMEDesc;

	tNIMEDesc.eMeshType = Prototype_Mesh_CIRCLE_DIST4;
	mVecMeshEffectDesc[MESHEFFECT_PRE_CIRCLE_DIST5] = tNIMEDesc;

	tNIMEDesc.eMeshType = Prototype_Mesh_IMPECTFX_02;
	mVecMeshEffectDesc[MESHEFFECT_PRE_IMPECTFX_02] = tNIMEDesc;

	tNIMEDesc.eMeshType = Prototype_Mesh_RING;
	mVecMeshEffectDesc[MESHEFFECT_PRE_RING] = tNIMEDesc;

	tNIMEDesc.eMeshType = Prototype_Mesh_LOVE;
	mVecMeshEffectDesc[MESHEFFECT_PRE_LOVE] = tNIMEDesc;

	tNIMEDesc.eMeshType = Prototype_Mesh_WING;
	mVecMeshEffectDesc[MESHEFFECT_PRE_WING] = tNIMEDesc;

	tNIMEDesc.eMeshType = Prototype_Mesh_BOW1;
	mVecMeshEffectDesc[MESHEFFECT_PRE_BOW1] = tNIMEDesc;

	tNIMEDesc.eMeshType = Prototype_Mesh_BOW2;
	mVecMeshEffectDesc[MESHEFFECT_PRE_BOW2] = tNIMEDesc;

	// Define MeshEffect
	//tNIMEDesc.eMeshType = Prototype_Mesh_CIRCLE;
	//tNIMEDesc.fMaxTime_Duration = 2.0f;
	//tNIMEDesc.iDiffuseTextureIndex = 48;
	//tNIMEDesc.NoiseTextureIndex = 0;
	//tNIMEDesc.MaskTextureIndex = 0;
	//mVecMeshEffectDesc[MESHEFFECT_ARROW_HEAD] = tNIMEDesc;


	//tNIMEDesc.eMeshType = Prototype_Mesh_CIRCLE;
	//tNIMEDesc.fMaxTime_Duration = 2.0f;
	//tNIMEDesc.iDiffuseTextureIndex = 48;
	//tNIMEDesc.NoiseTextureIndex = 0;
	//tNIMEDesc.MaskTextureIndex = 0;
	//mVecMeshEffectDesc[MESHEFFECT_ARROW_END] = tNIMEDesc;


	return S_OK;
}

HRESULT CPartilceCreateMgr::Create_MeshEffect(E_MESHEFFECT type, CTransform * parentTransform,_float3 Offset)
{

	CNonInstanceMeshEffect::NONINSTNESHEFTDESC meshDesc = Get_TypeDesc(type);
	meshDesc.vPosition = Offset;
	meshDesc.vLookDir = parentTransform->Get_MatrixState(CTransform::STATE_LOOK);

	_uint SceneNum = GetSingle(CGameInstance)->Get_NowSceneNum();
	CGameObject* obj  = g_pGameInstance->Add_GameObject_GetObject(SceneNum, TAG_LAY(Layer_PlayerEffect), TAG_OP(Prototype_NonInstanceMeshEffect), &meshDesc);
	NULL_CHECK_BREAK(obj);
	CNonInstanceMeshEffect * meshobj = static_cast<CNonInstanceMeshEffect*>(obj);
	NULL_CHECK_BREAK(meshobj);
	meshobj->Set_Base();
	meshobj->Set_ParentTransform(parentTransform);


	//mListEffects.push_back(meshobj);


	return S_OK;
}

HRESULT CPartilceCreateMgr::Create_MeshEffect_World(E_MESHEFFECT type, _float3 Postion, _float3 LookDir)
{
	CNonInstanceMeshEffect::NONINSTNESHEFTDESC meshDesc = Get_TypeDesc(type);
	meshDesc.vPosition = Postion;
	meshDesc.vLookDir = LookDir;


	_uint SceneNum = GetSingle(CGameInstance)->Get_NowSceneNum();
	g_pGameInstance->Add_GameObject_To_Layer(SceneNum, TAG_LAY(Layer_PlayerEffect), TAG_OP(Prototype_NonInstanceMeshEffect), &meshDesc);


	return S_OK;
}

HRESULT CPartilceCreateMgr::Create_MeshEffectDesc(CNonInstanceMeshEffect::NONINSTNESHEFTDESC desc, CTransform * parentTransform, _float3 Offset)
{
	desc.vPosition = Offset;

	_uint SceneNum = GetSingle(CGameInstance)->Get_NowSceneNum();
	CGameObject* obj = g_pGameInstance->Add_GameObject_GetObject(SceneNum, TAG_LAY(Layer_PlayerEffect), TAG_OP(Prototype_NonInstanceMeshEffect), &desc);
	NULL_CHECK_BREAK(obj);
	CNonInstanceMeshEffect * meshobj = static_cast<CNonInstanceMeshEffect*>(obj);
	NULL_CHECK_BREAK(meshobj);
	meshobj->Set_Base();
	meshobj->Set_ParentTransform(parentTransform);


	return S_OK;
}

HRESULT CPartilceCreateMgr::Create_MeshEffectDesc_World(CNonInstanceMeshEffect::NONINSTNESHEFTDESC desc, _float3 Postion, _float3 LookDir)
{

	desc.vPosition = Postion;
	desc.vLookDir = LookDir;
	_uint SceneNum = GetSingle(CGameInstance)->Get_NowSceneNum();
	CGameObject* obj = g_pGameInstance->Add_GameObject_GetObject(SceneNum, TAG_LAY(Layer_PlayerEffect), TAG_OP(Prototype_NonInstanceMeshEffect), &desc);
	NULL_CHECK_BREAK(obj);
	CNonInstanceMeshEffect * meshobj = static_cast<CNonInstanceMeshEffect*>(obj);
	NULL_CHECK_BREAK(meshobj);
	meshobj->Set_Base();

	return S_OK;
}

HRESULT CPartilceCreateMgr::Create_MeshEffectDesc_Hard(E_MESHEFFECT type , CTransform* Transfom)
{
	if (type <= MESHEFFECT_PRE_END)
		return E_FAIL;

	// ÇÏµå ÄÚµùÀ¸·Î Á¦ÀÛ
	if (type == MESHEFFECT_ARROW_HEAD)
	{
		CNonInstanceMeshEffect::NONINSTNESHEFTDESC ArrowDesc = GetSingle(CPartilceCreateMgr)->Get_TypeDesc(CPartilceCreateMgr::MESHEFFECT_PRE_IMPECTFX_02);

		ArrowDesc.fMaxTime_Duration = 3.0f;

		ArrowDesc.iDiffuseTextureIndex = 365;
		ArrowDesc.MaskTextureIndex = 31;
		ArrowDesc.NoiseTextureIndex = 325;

		ArrowDesc.noisingdir = _float2(0.5f, 0.5f).Get_Nomalize();
		ArrowDesc.vColor = _float4(1, 1, 1, 1);
		ArrowDesc.fAppearTime = 1.0f;
		ArrowDesc.fDistortionNoisingPushPower = 5.0f;

		ArrowDesc.vEmissive = _float4(0.3f, 0.3f, 1.0f, 1.0f);
		ArrowDesc.vLimLight = _float4(0.0f, 0.4f, 1, 1);


		ArrowDesc.vLookDir = _float3(0, 1, 0);
		//	ArrowDesc.vRotAxis = _float3(1, 0, 0);
		_float3 offset = _float3(0, 0, 1.2f);
		ArrowDesc.vScale = _float3(0.3f);

		ArrowDesc.eRUL = CTransform::STATE_RIGHT;
		ArrowDesc.fRotSpeed = 10.0f;

		//	ArrowDesc.m_iPassIndex = 16; // ¿Ö°î
		//	ArrowDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
		ArrowDesc.m_iPassIndex = 18; // DisCard
	//	ArrowDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

	//	ArrowDesc.vLookDir = m_pTransformCom->Get_MatrixScale(CTransform::STATE_RIGHT);

		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(ArrowDesc, Transfom, offset);

	}

	if (type == MESHEFFECT_ARROW_WING)
	{
		CNonInstanceMeshEffect::NONINSTNESHEFTDESC ArrowDesc = GetSingle(CPartilceCreateMgr)->Get_TypeDesc(CPartilceCreateMgr::MESHEFFECT_PRE_WING);

		ArrowDesc.fMaxTime_Duration = 3.0f;

		ArrowDesc.iDiffuseTextureIndex = 365;
		ArrowDesc.MaskTextureIndex = 31;
		ArrowDesc.NoiseTextureIndex = 325;

		ArrowDesc.noisingdir = _float2(0.5f, 0.5f).Get_Nomalize();
		ArrowDesc.vColor = _float4(1, 1, 1, 1);
		ArrowDesc.fAppearTime = 1.0f;
		ArrowDesc.fDistortionNoisingPushPower = 5.0f;

		ArrowDesc.vEmissive = _float4(0.3f, 0.3f, 1.0f, 1.0f);
		ArrowDesc.vLimLight = _float4(0.0f, 0.4f, 1, 1);


		ArrowDesc.vLookDir = _float3(0, 1, 0);
		//	ArrowDesc.vRotAxis = _float3(1, 0, 0);
		_float3 offset = _float3(0, 0, 1.2f);
		ArrowDesc.vScale = _float3(0.3f);

		ArrowDesc.eRUL = CTransform::STATE_RIGHT;
		ArrowDesc.fRotSpeed = 10.0f;

		//	ArrowDesc.m_iPassIndex = 16; // ¿Ö°î
		//	ArrowDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
		ArrowDesc.m_iPassIndex = 18; // DisCard
	//	ArrowDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

	//	ArrowDesc.vLookDir = m_pTransformCom->Get_MatrixScale(CTransform::STATE_RIGHT);

		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(ArrowDesc, Transfom, offset);

	}

	if (type == MESHEFFECT_ARROW_BOW1)
	{
		CNonInstanceMeshEffect::NONINSTNESHEFTDESC ArrowDesc = GetSingle(CPartilceCreateMgr)->Get_TypeDesc(CPartilceCreateMgr::MESHEFFECT_PRE_BOW1);

		ArrowDesc.fMaxTime_Duration = 3.0f;

		ArrowDesc.iDiffuseTextureIndex = 365;
		ArrowDesc.MaskTextureIndex = 31;
		ArrowDesc.NoiseTextureIndex = 325;

		ArrowDesc.noisingdir = _float2(0.5f, 0.5f).Get_Nomalize();
		ArrowDesc.vColor = _float4(1, 1, 1, 1);
		ArrowDesc.fAppearTime = 1.0f;
		ArrowDesc.fDistortionNoisingPushPower = 5.0f;

		ArrowDesc.vEmissive = _float4(0.3f, 0.3f, 1.0f, 1.0f);
		ArrowDesc.vLimLight = _float4(0.0f, 0.4f, 1, 1);


		ArrowDesc.vLookDir = _float3(0, 1, 0);
		//	ArrowDesc.vRotAxis = _float3(1, 0, 0);
		_float3 offset = _float3(0, 0, 1.2f);
		ArrowDesc.vScale = _float3(0.3f);

		ArrowDesc.eRUL = CTransform::STATE_RIGHT;
		ArrowDesc.fRotSpeed = 10.0f;

		//	ArrowDesc.m_iPassIndex = 16; // ¿Ö°î
		//	ArrowDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
		ArrowDesc.m_iPassIndex = 18; // DisCard
	//	ArrowDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

	//	ArrowDesc.vLookDir = m_pTransformCom->Get_MatrixScale(CTransform::STATE_RIGHT);

		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(ArrowDesc, Transfom, offset);


	}

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


void CPartilceCreateMgr::Free()
{
	

}
