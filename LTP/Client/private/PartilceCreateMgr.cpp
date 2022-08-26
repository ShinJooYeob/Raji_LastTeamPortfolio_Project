#include "stdafx.h"
#include "..\public\PartilceCreateMgr.h"
#include "..\public\NonInstanceMeshEffect_TT.h"
#include "..\public\NonInstanceMeshEffect_TT_Fix.h"
#include "..\public\Camera_Main.h"

#include "Player.h"

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
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_meteo);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_half_ball_closed_1);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_RainDrop);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_DS_Basic_03_3_X_L);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_Sphere_Plane_01);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_ky_rock07);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_ControlPointMatch_Square_02);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_Box_Basic);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_GT_Cyinder_01);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_ENV_DT_StatueFemale_Pose01);

		

		// Bullet
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_Monster_Bullet_Vayusura_Leader);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_Monster_Bullet_Tezabsura_Minion);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_Monster_Bullet_Tezabsura_Purple);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_Monster_Bullet_Tezabsura_Bomber);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_Monster_Bullet_Tezabsura_Landmine);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_Ninjasura_Knife);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_sinkhole);
		STATIC_EFFECTLOAD(Prototype_Mesh_SM_Gadasura_Terrain_Bullet);

		// ALLLOAD
		// for (_uint i = (_uint)Prototype_Mesh_KurtzpelStart + 1; i < (_uint)Prototype_Mesh_KurtzpelEnd; i++)
		// 	STATIC_EFFECTLOAD((COMPONENTPROTOTYPEID)i);


	*/

HRESULT CPartilceCreateMgr::Initialize_ParticleMgr()
{
	FAILED_CHECK(Ready_MeshEffect());
	FAILED_CHECK(Ready_TextureEffect());
	FAILED_CHECK(Ready_MeshInstanceEffect());

	FAILED_CHECK(ReadyParticle_ALL_Stages());
	return S_OK;
}

_int CPartilceCreateMgr::Update_EffectMgr(_double Timer)
{
	// Effect Timer
	if (mListMeshInstDesc_Delay.empty() == false)
	{
		mListMeshInstDesc_Delay.begin();
		for (auto iter = mListMeshInstDesc_Delay.begin(); iter != mListMeshInstDesc_Delay.end();)
		{

			iter->timer -= Timer;
			if (iter->timer <= 0)
			{
				FAILED_CHECK(GetSingle(CUtilityMgr)->Create_MeshInstance(iter->scene, iter->data));
				iter = mListMeshInstDesc_Delay.erase(iter);
			}
			else
				iter++;
		}
	}



	if (mListParticleDesc_Delay.empty() == false)
	{
		mListParticleDesc_Delay.begin();
		for (auto iter = mListParticleDesc_Delay.begin(); iter != mListParticleDesc_Delay.end();)
		{
			
			iter->timer -= Timer;
			if (iter->timer <= 0)
			{
				FAILED_CHECK(GetSingle(CUtilityMgr)->Create_TextureInstance(iter->scene, iter->data));
				iter = mListParticleDesc_Delay.erase(iter);
			}
			else
				iter++;
		}

	}


	// Map Effect
//	Update_MapEffect(Timer);
	Update_MapEffect_Sound(Timer);

	Update_DebugParticle(Timer);

	return 0;
}

_int CPartilceCreateMgr::Update_DebugParticle(_double Timer)
{

#ifdef _DEBUG

	if (
		mCurrentScene != SCENE_STAGE1 &&
		mCurrentScene != SCENE_STAGE2 &&
		mCurrentScene != SCENE_STAGE3 &&
		mCurrentScene != SCENE_STAGE4 &&
		mCurrentScene != SCENE_STAGE5 &&
		mCurrentScene != SCENE_STAGE6 &&
		mCurrentScene != SCENE_STAGE7
		)
		return _int();


	CPlayer* player = ((CPlayer*)(g_pGameInstance->Get_GameObject_By_LayerIndex(g_pGameInstance->Get_NowSceneNum(), TAG_LAY(Layer_Player))));
	if (player == nullptr)
		return 0;

	CTransform* m_pTransformCom = player->Get_Transform();
	
	_uint m_eNowSceneNum = g_pGameInstance->Get_NowSceneNum();
	if (KEYDOWN(DIK_C))
	{
		// Tex

		{
			INSTPARTICLEDESC desctex = GETPARTICLE->Get_EffectSetting_Tex(CPartilceCreateMgr::TEXTURE_EFFECTJ_Bow_ArrowHit,
				0,
				0.5f,
				_float4(0.03f, 0.25f, 0.65f, 1),
				_float4(0.13f, 0.35f, 0.75f, 1),
				0,
				_float3(1.1f),
				_float3(0.3f),
				1);
			desctex.eInstanceCount = Prototype_VIBuffer_Point_Instance_32;
			//	desctex.vEmissive_SBB = _float3(1,0.3f,0.1f);
			desctex.Particle_Power = 6.5f;
			//	desctex.Particle_Power = 4.0f;

			//	desctex.TempBuffer_0.z = 0;
			//	desctex.TempBuffer_0.w = FollowingDir_Look;
				//	desctex.FollowingTarget = m_pTransformCom;
				//	desctex.iFollowingDir = FollowingDir_Up;

			desctex.vFixedPosition =
				m_pTransformCom->Get_WorldMatrix().r[3]
				+ m_pTransformCom->Get_WorldMatrix().r[1] * 2.0f;

			_float val = 0.5f;
			desctex.ParticleStartRandomPosMin = _float3(-val, 0, -val);
			desctex.ParticleStartRandomPosMax = _float3(val, 0, val);

			//	desc.AlphaBlendON = true;
			//	desc.vEmissive_SBB = _float3(1, 0, 0);

		//	GETPARTICLE->Create_Texture_Effect_Desc(desctex, m_eNowSceneNum);


		}
		{
			INSTPARTICLEDESC desctex = GETPARTICLE->Get_EffectSetting_Tex(CPartilceCreateMgr::Um_FireMask_2_png,
				0,
				0.8f,
				_float4(0.75f, 0.26f, 0.05f, 1),
				_float4(0.95f, 0.13f, 0.2f, 0.5f),
				0,
				_float3(1.0f),
				_float3(0.1f),
				1);

			desctex.vEmissive_SBB = _float3(1, 0.5f, 0.0f);
			desctex.Particle_Power = 4.0f;
			// desctex.iTextureLayerIndex = ;

			desctex.vFixedPosition =
				m_pTransformCom->Get_WorldMatrix().r[3]
				+ m_pTransformCom->Get_WorldMatrix().r[1] * 2.0f;

			//	desctex.AlphaBlendON = true;
			//	desctex.vEmissive_SBB = _float3(1, 0, 0);

			//	GETPARTICLE->Create_Texture_Effect_Desc(desctex, m_eNowSceneNum);
		}

		{
			
			INSTPARTICLEDESC desctex = GETPARTICLE->Get_EffectSetting_Tex(CPartilceCreateMgr::Um_Dust_2_FounTain,
				2,
				0.4f,
				_float4(0.04f, 0.78f, 0.98f, 1.0f),
				_float4(0.04f, 0.78f, 0.98f, 1.0f),
				1,
				_float3(0.2f),
				_float3(0.1f),
				1);

			desctex.Particle_Power = 7.0f;


		//	desctex = GetSingle(CUtilityMgr)->Get_TextureParticleDesc(TEXT("Jino_Stage1_WaterFallParticle_1"));
		//	desctex = GetSingle(CUtilityMgr)->Get_TextureParticleDesc(TEXT("Jino_Stage1_FountainParticle_0"));
		//	desctex = GetSingle(CUtilityMgr)->Get_TextureParticleDesc(TEXT("Jino_Stage1_FountainParticle_1"));
		//	desctex = GetSingle(CUtilityMgr)->Get_TextureParticleDesc(TEXT("Jino_Stage1_Rain_0"));

		//	desctex.TotalParticleTime = 1.0f;
		//	desctex.EachParticleLifeTime = 3.0f;


			desctex.vFixedPosition =
				m_pTransformCom->Get_WorldMatrix().r[3]
				+ m_pTransformCom->Get_WorldMatrix().r[1] * 2.0f;

			//	desctex.AlphaBlendON = true;
			//	desctex.vEmissive_SBB = _float3(1, 0, 0);

			GETPARTICLE->Create_Texture_Effect_Desc(desctex, m_eNowSceneNum);
		}

		


	}

	if (KEYDOWN(DIK_V))
	{
		// MESHINST
		{
			// MESHINST_EFFECTJ_BOW_Q_ICE2
			INSTMESHDESC descmesh = GETPARTICLE->Get_EffectSetting_Mesh(CPartilceCreateMgr::MESHINST_EFFECTJ_BOW_Q_ICE2,
				//	Prototype_Mesh_SM_ICE_01,
				COMPONENTPROTOTYPEID(0),
				3,
				1.0f,
				_float4(0.25f, 0.86f, 1.0f, 1),
				_float4(0.25f, 0.86f, 1.0f, 0.8f),
				0,
				_float3(0.8f),
				_float3(1.0f),
				0);


			// mesh
			_Vector FixPos = m_pTransformCom->Get_WorldMatrix().r[3];

			_float val = 5.0f;
			descmesh.ParticleStartRandomPosMin = _float3(-val, 0, -val);
			descmesh.ParticleStartRandomPosMax = _float3(val, 0, val);

			descmesh.vFixedPosition = FixPos;
			//	GETPARTICLE->Create_MeshInst_DESC(descmesh, m_eNowSceneNum);


		}

	}


	if (KEYDOWN(DIK_B))
	{
		// MESH Effect
		_float3 FixPos = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS);
		_uint id = g_pGameInstance->Get_NowSceneNum();

		//	GETPARTICLE->Create_MeshEffectDesc_Map_Pos(CPartilceCreateMgr::MESHEFFECT_MAP_TEST0, FixPos);
		//	GETPARTICLE->Create_MeshEffectDesc_Map_Pos(CPartilceCreateMgr::MESHEFFECT_MAP_TEST1, FixPos);
		//	GETPARTICLE->Create_MeshEffectDesc_Map_Pos(CPartilceCreateMgr::MESHEFFECT_MAP_TEST2, FixPos);

		// _float3(10.0f, 10.0f, 10.0f);
		wstring tt = L"\nPOSITION: _float3(" + to_wstring(FixPos.x) + L"f, " +
			to_wstring(FixPos.y) + L"f, " +
			to_wstring(FixPos.z) + L"f)" +
			L"\n";
		OutputDebugStringW(tt.c_str());


		NONINSTNESHEFTDESC desc1;
		MESHADDDATA desc2;
		MESHLIGHTDATA desc3;


	//	GETPARTICLE->Get_MeshEffectDesc_Map_FreeSet(CPartilceCreateMgr::MESHEFFECT_MAP_OBJ_Lotas, desc1, desc2, desc3);
	//	GETPARTICLE->Create_MeshEffectDesc_MAP(id ,desc1, desc2, FixPos, desc3);

	//	GETPARTICLE->Get_MeshEffectDesc_Map_FreeSet(CPartilceCreateMgr::MESHEFFECT_MAP_OBJ_Freeset0, desc1, desc2, desc3);
	//	GETPARTICLE->Create_MeshEffectDesc_MAP(id, desc1, desc2, FixPos, desc3);


	//	GETPARTICLE->Get_MeshEffectDesc_Map_FreeSet(CPartilceCreateMgr::MESHEFFECT_MAP_OBJ_Freeset1, desc1, desc2, desc3);
	//	GETPARTICLE->Create_MeshEffectDesc_MAP(id, desc1, desc2, FixPos, desc3);

		{
			// FixPos = (_float3(460.5f, 13.5f, 149.0f));

			GETPARTICLE->Get_MeshEffectDesc_Map_FreeSet(CPartilceCreateMgr::MESHEFFECT_MAP_OBJ_Lotas, desc1, desc2, desc3);
			desc1.fMaxTime_Duration = 3.0f;
			desc1.vPosition = _float3(-0.0f, -1.1f, 0.0f);
			desc2.DealyTime = GETUTIL->RandomFloat(0.1f, 2);
			desc2.AccRotSpeed = 0.0001f;
			desc1.iDiffuseTextureIndex = 275;
			desc1.vSize = desc1.vSize.XMVector()*1.2f;
		//	GETPARTICLE->Create_MeshEffectDesc_MAP(id, desc1, desc2, FixPos, desc3);

		}

		//GETPARTICLE->Get_MeshEffectDesc_Map_FreeSet(CPartilceCreateMgr::MESHEFFECT_MAP_OBJ_Freeset2, desc1, desc2, desc3);
		//desc1.fMaxTime_Duration = 1.0f;
		//desc2.DealyTime = 0.1f;

		//desc1.vPosition = _float3(0, -0.1f, 0);
		//desc1.vSize = _float3(0.35f, 0.25f, 0.35f);

		//GETPARTICLE->Create_MeshEffectDesc_MAP(id, desc1, desc2, FixPos, desc3);


		//GETPARTICLE->Get_MeshEffectDesc_Map_FreeSet(CPartilceCreateMgr::MESHEFFECT_MAP_OBJ_Freeset4, desc1, desc2, desc3);
		//desc1.fMaxTime_Duration = 3.0f;
		//desc2.DealyTime = 0.1f;
		//desc1.vLookDir = _float3(0, 0, 1);
		//GETPARTICLE->Create_MeshEffectDesc_MAP(id, desc1, desc2, FixPos, desc3);


	//	GETPARTICLE->Get_MeshEffectDesc_Map_FreeSet(CPartilceCreateMgr::MESHEFFECT_MAP_OBJ_BoxBar, desc1, desc2, desc3);
	//	GETPARTICLE->Create_MeshEffectDesc_MAP(id, desc1, desc2, FixPos, desc3);

		// GETPARTICLE->GetMeshEffect_Fix();


	//	 Mesh Effect
	//	GETPARTICLE->Create_MeshEffectDesc_Hard();

	// Texture Particle Effect
	//	GETPARTICLE->Get_EffectSetting_Tex();
	//	GETPARTICLE->Create_Texture_Effect_Desc();

	// Mesh Particle Effect
	//	GETPARTICLE->Get_EffectSetting_Mesh();
	//	GETPARTICLE->Create_MeshInst_DESC();

		

	}


	if (KEYDOWN(DIK_H))
	{
		// ÇØ´ç ¿µ¿ª¿¡ º¸³¿

		CGameObject* pPlayer = (CPlayer*)(g_pGameInstance->Get_GameObject_By_LayerIndex(g_pGameInstance->Get_NowSceneNum(), TAG_LAY(Layer_Player)));
		NULL_CHECK_BREAK(pPlayer);
		CTransform* PlayerTransform = (CTransform*)pPlayer->Get_Component(TAG_COM(Com_Transform));
		CNavigation* PlayerNavi = (CNavigation*)pPlayer->Get_Component(TAG_COM(Com_Navaigation));


		// pick pos
		POINT ptMouse;
		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);

		_Vector vCursorPos = XMVectorSet(
			(_float(ptMouse.x) / (g_iWinCX * 0.5f)) - 1.f,
			(_float(ptMouse.y) / -(g_iWinCY * 0.5f)) + 1.f,
			0, 1.f);

		_Matrix InvProjMat = XMMatrixInverse(nullptr, GetSingle(CGameInstance)->Get_Transform_Matrix(PLM_PROJ));
		_Matrix InvViewMat = XMMatrixInverse(nullptr, GetSingle(CGameInstance)->Get_Transform_Matrix(PLM_VIEW));

		_Vector vRayDir = XMVector4Transform(vCursorPos, InvProjMat) - XMVectorSet(0, 0, 0, 1);

		vRayDir = XMVector3TransformNormal(vRayDir, InvViewMat);


		_Vector vCamPos = GETUTIL->Get_MainCamera()->Get_Camera_Transform()->Get_MatrixState(CTransform::STATE_POS);
		_Vector vOldPos = vCamPos;
		_Vector vNewPos;
		vNewPos = vOldPos + vRayDir;

		static_cast<CTransform*>(pPlayer->Get_Component(TAG_COM(Com_Transform)))->Set_MatrixState(CTransform::STATE_POS, vNewPos);
		PlayerNavi->FindCellIndex(PlayerTransform->Get_MatrixState(CTransform::TransformState::STATE_POS));

	}


#endif // _DEBUG

	return _int();
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


HRESULT CPartilceCreateMgr::Create_Texture_Effect_Desc(INSTPARTICLEDESC desc, _uint scene,_double timer)
{

	if (timer != 0)
	{
		DELAYTEX tex;
		tex.data = desc;
		tex.scene = scene;
		tex.timer = timer;

		mListParticleDesc_Delay.push_back(tex);
	}
	else
	{
		FAILED_CHECK(GetSingle(CUtilityMgr)->Create_TextureInstance(scene, desc));
	}


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

void CPartilceCreateMgr::Set_CreatBound_Tex(INSTPARTICLEDESC & tex, _float3 Min, _float3 Max)
{
	tex.ParticleStartRandomPosMin = Min;
	tex.ParticleStartRandomPosMax = Max;

}

HRESULT CPartilceCreateMgr::Create_MeshInst_DESC(INSTMESHDESC desc, _uint scene, _double timer)
{
	if (timer != 0)
	{
		DELAYMESH mesh;
		mesh.data = desc;
		mesh.scene = scene;
		mesh.timer = timer;

		mListMeshInstDesc_Delay.push_back(mesh);
	}
	else
	{
		FAILED_CHECK(GetSingle(CUtilityMgr)->Create_MeshInstance(scene, desc));
	}

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
	mPreMeshEffect = meshobj;

	return S_OK;
}

HRESULT CPartilceCreateMgr::Create_MeshEffectDesc_MAP(_uint SceneId, NONINSTNESHEFTDESC desc, MESHADDDATA desc2,_float3 pos, MESHLIGHTDATA desc3)
{

	_uint SceneNum = SceneId;
	CGameObject* obj = g_pGameInstance->Add_GameObject_GetObject(SceneNum, 
		TAG_LAY(Layer_PlayerEffect), TAG_OP(Prototype_NonInstanceMeshEffect_TT_Fix), &desc);
	NULL_CHECK_BREAK(obj);

	CNonInstanceMeshEffect_TT_Fix * meshobj = static_cast<CNonInstanceMeshEffect_TT_Fix*>(obj);
	NULL_CHECK_BREAK(meshobj);

	meshobj->Set_Position(pos);
	meshobj->Set_AddDesc(desc2);
	meshobj->Set_LightDesc(desc3);

	mPreMeshEffect_Fix = meshobj;
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


bool CPartilceCreateMgr::Get_MeshEffectDesc_Map_FreeSet(E_MESH_EFFECTJ type, NONINSTNESHEFTDESC& desc1, MESHADDDATA& desc2, MESHLIGHTDATA& desc3)
{
	mPreMeshEffect_Fix = nullptr;

	if (type <= MESHEFFECT_MAP_NONE)
	{
		DEBUGBREAK;
		return nullptr;
	}

	NONINSTNESHEFTDESC	MeshDesc;
	MESHADDDATA			AddDesc;
	MESHLIGHTDATA		LightDesc;
	_float3 Pos = _float3(0, 0, 0);
	MeshDesc.eMeshType = Prototype_Mesh_SM_circle;

#pragma region TEST


	if (type == MESHEFFECT_MAP_TEST0)
	{
		MeshDesc.eMeshType = Prototype_Mesh_Sample_Mesh_Fake_Mesh_Lighting;
		MeshDesc.eMeshType = Prototype_Mesh_MS_Shot_Cylinder_00;
		MeshDesc.eMeshType = Prototype_Mesh_SM_GT_SphereHarf;
		MeshDesc.eMeshType = Prototype_Mesh_SM_ky_sphere_dist_wind;
		MeshDesc.eMeshType = Prototype_Mesh_SM_Sphere_RPG;

		// Time
		MeshDesc.fMaxTime_Duration = 3.0f;
		MeshDesc.fAppearTime = 0.5f;
		AddDesc.bAfterApperTime = false;



		// Tex
		MeshDesc.iDiffuseTextureIndex = NONNOISE;
		MeshDesc.MaskTextureIndex = NONNMASK;
		MeshDesc.NoiseTextureIndex = NONNOISE;


		// Noise
		MeshDesc.noisingdir = _float2(0, 1).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		MeshDesc.fAlphaTestValue = 1.0f;

		// Color
		MeshDesc.vLimLight = _float4(1.0f, 0.00f, 0.00f, 1.0f);
		MeshDesc.vEmissive = _float4(0);


		// Transform_Base
		// LocalPos
		MeshDesc.vLookDir = _float3(0, 0, 1);
		MeshDesc.vPosition = _float3(0.0f, 1.0f, 0.0f);
		MeshDesc.vSize = _float3(2.0f);


		// Move
		MeshDesc.MoveDir = FollowingDir_Up;
		AddDesc.AccMoveSpeed = 0.0f;

		// Rot
		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.vAddDirectAngle = _float3(0, 0, 90);
		AddDesc.InitRot = _float3(0);
		MeshDesc.RotAxis = FollowingDir_Up;
		AddDesc.AccRotSpeed = 45;
		AddDesc.LookRotSpeed = 0;

		// Scale
		AddDesc.AccScaleSpeed = 0.0f;
		AddDesc.bLockScale[0] = true;
		AddDesc.bLockScale[1] = true;
		AddDesc.bLockScale[2] = true;


		// Fix
		AddDesc.FixFlag_Move = false;
		AddDesc.FixFlag_Rot = false;
		AddDesc.DealyTime = 0.1f;

		LightDesc.bEmsiive = false;
		LightDesc.bRim = true;
		LightDesc.Ambiant = 0.5f;
		LightDesc.SpeedTime_Rim = 5.0f;
		LightDesc.SpeedTime_Emsive = 1.0f;

		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
		MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

	}

	if (type == MESHEFFECT_MAP_TEST1)
	{
		MeshDesc.eMeshType = Prototype_Mesh_SM_Sphere_RPG;

		// Time
		MeshDesc.fMaxTime_Duration = 3.0f;
		MeshDesc.fAppearTime = 0.5f;
		AddDesc.bAfterApperTime = false;



		// Tex
		MeshDesc.iDiffuseTextureIndex = 321;
		MeshDesc.MaskTextureIndex = NONNMASK;
		MeshDesc.MaskTextureIndex = 3;
		MeshDesc.NoiseTextureIndex = NONNOISE;


		// Noise
		MeshDesc.noisingdir = _float2(0, 1).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		MeshDesc.fAlphaTestValue = 1.0f;

		// Color
		MeshDesc.vLimLight = _float4(1.0f, 0.00f, 0.00f, 1.0f);
		MeshDesc.vLimLight = _float4(1.0f, 0.00f, 0.00f, 0.1f);
		MeshDesc.vEmissive = _float4(1);


		// Transform_Base
		// LocalPos
		MeshDesc.vLookDir = _float3(0, 0, 1);
		MeshDesc.vPosition = _float3(0.0f, 1.0f, 0.0f);
		MeshDesc.vSize = _float3(0.5f);


		// Move
		MeshDesc.MoveDir = FollowingDir_Up;
		AddDesc.AccMoveSpeed = 0.0f;

		// Rot
		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.vAddDirectAngle = _float3(0, 0, 90);
		AddDesc.InitRot = _float3(0);
		MeshDesc.RotAxis = FollowingDir_Up;
		AddDesc.AccRotSpeed = 0;
		AddDesc.LookRotSpeed = 0;

		// Scale
		AddDesc.AccScaleSpeed = 0.0f;
		AddDesc.bLockScale[0] = false;
		AddDesc.bLockScale[1] = false;
		AddDesc.bLockScale[2] = false;


		// Fix
		AddDesc.FixFlag_Move = false;
		AddDesc.FixFlag_Rot = false;
		AddDesc.DealyTime = 0.1f;

		LightDesc.bEmsiive = true;
		LightDesc.bRim = true;
		LightDesc.Ambiant = 0.5f;
		LightDesc.SpeedTime_Rim = 10.0f;
		LightDesc.SpeedTime_Emsive = 10.0f;

		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
		MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå


	}

	if (type == MESHEFFECT_MAP_TEST2)
	{

		MeshDesc.eMeshType = Prototype_Mesh_SM_ENV_DT_Diya_02_01;
		MeshDesc.eMeshType = Prototype_Mesh_SM_ENV_DT_Bull;
		



		// Time
		MeshDesc.fMaxTime_Duration = 3.0f;
		MeshDesc.fAppearTime = 0.5f;
		AddDesc.bAfterApperTime = false;

		AddDesc.DealyTime = 0.1f;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 321;
		MeshDesc.MaskTextureIndex = NONNMASK;
		MeshDesc.MaskTextureIndex = 3;
		MeshDesc.NoiseTextureIndex = NONNOISE;


		// Noise
		MeshDesc.noisingdir = _float2(0, 1).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		MeshDesc.fAlphaTestValue = 1.0f;

		// Color
		MeshDesc.vLimLight = _float4(1.0f, 0.00f, 0.00f, 1.0f);
		MeshDesc.vLimLight = _float4(1.0f, 0.00f, 0.00f, 0.1f);
		MeshDesc.vEmissive = _float4(1);


		// Transform_Base
		// LocalPos
		MeshDesc.vLookDir = _float3(0, 0, 1);
		MeshDesc.vPosition = _float3(0.0f, 1.0f, 0.0f);
		MeshDesc.vSize = _float3(0.5f);


		// Move
		MeshDesc.MoveDir = FollowingDir_Up;
		AddDesc.AccMoveSpeed = 0.0f;

		// Rot
		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.vAddDirectAngle = _float3(0, 0, 90);
		AddDesc.InitRot = _float3(0);
		MeshDesc.RotAxis = FollowingDir_Up;
		AddDesc.AccRotSpeed = 0;
		AddDesc.LookRotSpeed = 0;

		// Scale
		AddDesc.AccScaleSpeed = 0.0f;
		AddDesc.bLockScale[0] = false;
		AddDesc.bLockScale[1] = false;
		AddDesc.bLockScale[2] = false;


		// Fix
		AddDesc.FixFlag_Move = false;
		AddDesc.FixFlag_Rot = false;
		AddDesc.DealyTime = 0.1f;

		LightDesc.bEmsiive = true;
		LightDesc.bRim = true;
		LightDesc.Ambiant = 0.5f;
		LightDesc.SpeedTime_Rim = 10.0f;
		LightDesc.SpeedTime_Emsive = 10.0f;

		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
		MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå


	}
#pragma endregion TEST

	// Freeset OBJ Effect ¼öÁ¤ÇÏ¸é¼­ ¹èÄ¡ ÁøÇà

	if (type == MESHEFFECT_MAP_OBJ_SphereLight)
	{
		// light
		MeshDesc.eMeshType = Prototype_Mesh_SM_Sphere_RPG;

		// Time
		MeshDesc.fMaxTime_Duration = 5.0f;
		MeshDesc.fAppearTime = 0.5f;
		AddDesc.bAfterApperTime = false;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 394;
		MeshDesc.iDiffuseTextureIndex = NODIFF;
		MeshDesc.iDiffuseTextureIndex = 275;
		MeshDesc.iDiffuseTextureIndex = 271;
		MeshDesc.MaskTextureIndex = NONNMASK;
		MeshDesc.NoiseTextureIndex = NONNOISE;


		// Noise
		MeshDesc.noisingdir = _float2(0, 0).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 0.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		MeshDesc.fAlphaTestValue = 1.0f;

		// Color
		MeshDesc.vLimLight = _float4(1.0f, 1, 1, 0.0f);
		MeshDesc.vLimLight = _float4(0.3f, 0.3f, 0.3f, 1.0f);
		MeshDesc.vEmissive = _float4(1);


		// Transform_Base
		// LocalPos
		MeshDesc.vLookDir = _float3(0, 0, 1);
		MeshDesc.vPosition = _float3(-0.0f, 2.0f, 0.0f);
		MeshDesc.vSize = _float3(0.15f);


		// Move
		MeshDesc.MoveDir = FollowingDir_Up;
		AddDesc.AccMoveSpeed = 0.0f;

		// Rot
		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.vAddDirectAngle = _float3(0, 0, 90);
		AddDesc.InitRot = _float3(0);
		MeshDesc.RotAxis = FollowingDir_Up;
		AddDesc.AccRotSpeed = 4.5f;
		AddDesc.LookRotSpeed = 0;

		// Scale
		AddDesc.AccScaleSpeed = 0.0f;
		AddDesc.bLockScale[0] = false;
		AddDesc.bLockScale[1] = false;
		AddDesc.bLockScale[2] = false;


		// Fix
		AddDesc.FixFlag_Move = false;
		AddDesc.FixFlag_Rot = false;
		AddDesc.DealyTime = 0.1f;

		LightDesc.bEmsiive = true;
		LightDesc.bRim = true;
		LightDesc.Ambiant = 0.5f;
		_float Time = 2.0f;
		LightDesc.SpeedTime_Rim = Time;
		LightDesc.SpeedTime_Emsive = Time;

		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
		MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå
		
	}

	if (type == MESHEFFECT_MAP_OBJ_BoxBar)
	{
		// light
		MeshDesc.eMeshType = Prototype_Mesh_SM_Box_Basic;

		// Time
		MeshDesc.fMaxTime_Duration = 5.0f;
		MeshDesc.fAppearTime = 0.5f;
		AddDesc.bAfterApperTime = false;



		// Tex
		MeshDesc.iDiffuseTextureIndex = 397;
		MeshDesc.iDiffuseTextureIndex = 306;
		MeshDesc.MaskTextureIndex = NONNMASK;
		MeshDesc.NoiseTextureIndex = NONNOISE;


		// Noise
		MeshDesc.noisingdir = _float2(0, 0).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 0.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		MeshDesc.fAlphaTestValue = 1.0f;

		// Color
		MeshDesc.vLimLight = _float4(0.0f, 0.0f, 0.5f, 1.0f);
		MeshDesc.vLimLight = _float4(1.0f, 0.00f, 0.00f, 0.0f);
		MeshDesc.vEmissive = _float4(0);


		// Transform_Base
		// LocalPos
		MeshDesc.vLookDir = _float3(0, 0, 1);
		MeshDesc.vPosition = _float3(0.0f, 0, 0.0f);
		MeshDesc.vSize = _float3(1.0f,10, 1.0f);


		// Move
		MeshDesc.MoveDir = FollowingDir_Up;
		AddDesc.AccMoveSpeed = 0.0f;

		// Rot
		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);
		AddDesc.InitRot = _float3(0);
		MeshDesc.RotAxis = FollowingDir_Up;
		AddDesc.AccRotSpeed = 0.0f;
		AddDesc.LookRotSpeed = 0;

		// Scale
		AddDesc.AccScaleSpeed = 0.0f;
		AddDesc.bLockScale[0] = false;
		AddDesc.bLockScale[1] = false;
		AddDesc.bLockScale[2] = false;


		// Fix
		AddDesc.FixFlag_Move = false;
		AddDesc.FixFlag_Rot = false;
		AddDesc.DealyTime = 0.1f;

		LightDesc.bEmsiive = false;
		LightDesc.bRim = false;
		LightDesc.Ambiant = 0.5f;
		_float Time = 3.0f;
		LightDesc.SpeedTime_Rim = Time;
		LightDesc.SpeedTime_Emsive = Time;

		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
		MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

	}

	if (type == MESHEFFECT_MAP_OBJ_Bull)
	{
		// light
		MeshDesc.eMeshType = Prototype_Mesh_SM_ENV_DT_Bull;

		// Time
		MeshDesc.fMaxTime_Duration = 5.0f;
		MeshDesc.fAppearTime = 0.5f;
		AddDesc.bAfterApperTime = false;



		// Tex
		MeshDesc.iDiffuseTextureIndex = 276;
		MeshDesc.MaskTextureIndex = NONNMASK;
		MeshDesc.NoiseTextureIndex = NONNOISE;


		// Noise
		MeshDesc.noisingdir = _float2(0, 1).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		MeshDesc.fAlphaTestValue = 1.0f;

		// Color
		MeshDesc.vLimLight = _float4(1.0f, 0.00f, 0.00f, 0.0f);
		MeshDesc.vLimLight = _float4(1.0f, 0.00f, 0.00f, 1.0f);
		MeshDesc.vEmissive = _float4(1);


		// Transform_Base
		// LocalPos
		MeshDesc.vLookDir = _float3(0, 0, 1);
		MeshDesc.vPosition = _float3(0.0f, 1.0f, 0.0f);
		MeshDesc.vSize = _float3(0.3f);


		// Move
		MeshDesc.MoveDir = FollowingDir_Up;
		AddDesc.AccMoveSpeed = 0.0f;

		// Rot
		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.vAddDirectAngle = _float3(180, 0, 0);
		AddDesc.InitRot = _float3(0);
		MeshDesc.RotAxis = FollowingDir_Up;
		AddDesc.AccRotSpeed = 0.0f;
		AddDesc.LookRotSpeed = 0;

		// Scale
		AddDesc.AccScaleSpeed = 0.0f;
		AddDesc.bLockScale[0] = false;
		AddDesc.bLockScale[1] = false;
		AddDesc.bLockScale[2] = false;


		// Fix
		AddDesc.FixFlag_Move = false;
		AddDesc.FixFlag_Rot = false;
		AddDesc.DealyTime = 0.1f;

		LightDesc.bEmsiive = true;
		LightDesc.bRim = true;
		LightDesc.Ambiant = 0.5f;
		_float Time = 3.0f;
		LightDesc.SpeedTime_Rim = Time;
		LightDesc.SpeedTime_Emsive = Time;

		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
		MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå
	}


	if (type == MESHEFFECT_MAP_OBJ_Lotas)
	{
		// light
		MeshDesc.eMeshType = Prototype_Mesh_SM_Lotus_01_flower;

		// Time
		MeshDesc.fMaxTime_Duration = 5.0f;
		MeshDesc.fAppearTime = 0.5f;
		AddDesc.bAfterApperTime = false;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 394;
		MeshDesc.iDiffuseTextureIndex = NODIFF;
		MeshDesc.iDiffuseTextureIndex = 275;
		MeshDesc.iDiffuseTextureIndex = 271;
		MeshDesc.MaskTextureIndex = NONNMASK;
		MeshDesc.NoiseTextureIndex = NONNOISE;


		// Noise
		MeshDesc.noisingdir = _float2(0, 0).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 0.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		MeshDesc.fAlphaTestValue = 1.0f;

		// Color
		MeshDesc.vLimLight = _float4(1.0f, 1, 1, 0.0f);
		MeshDesc.vLimLight = _float4(0.3f, 0.3f, 0.3f, 1.0f);
		MeshDesc.vEmissive = _float4(1);


		// Transform_Base
		// LocalPos
		MeshDesc.vLookDir = _float3(0, 0, 1);
		MeshDesc.vPosition = _float3(-0.0f, 0.7f, 0.0f);
		MeshDesc.vSize = _float3(0.45f);


		// Move
		MeshDesc.MoveDir = FollowingDir_Up;
		AddDesc.AccMoveSpeed = 0.0f;

		// Rot
		AddDesc.LookRotAxis = FollowingDir_Up;
		AddDesc.vAddDirectAngle = _float3(0, 0, 90);
		AddDesc.InitRot = _float3(0);
		MeshDesc.RotAxis = FollowingDir_Up;
		AddDesc.AccRotSpeed = 1.0f;
		AddDesc.LookRotSpeed = 0;

		// Scale
		AddDesc.AccScaleSpeed = 0.0f;
		AddDesc.bLockScale[0] = false;
		AddDesc.bLockScale[1] = false;
		AddDesc.bLockScale[2] = false;


		// Fix
		AddDesc.FixFlag_Move = false;
		AddDesc.FixFlag_Rot = false;
		AddDesc.DealyTime = 0.1f;

		LightDesc.bEmsiive = true;
		LightDesc.bRim = true;
		LightDesc.Ambiant = 0.5f;
		_float Time = 2.0f;
		LightDesc.SpeedTime_Rim = Time;
		LightDesc.SpeedTime_Emsive = Time;

		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
		MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

	}


	if (type == MESHEFFECT_MAP_OBJ_Freeset0)
	{
		// light
		MeshDesc.eMeshType = Prototype_Mesh_SM_Statue_Female_ReBirth_Body;

		// Time
		MeshDesc.fMaxTime_Duration = 5.0f;
		MeshDesc.fAppearTime = 0.5f;
		AddDesc.bAfterApperTime = false;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 394;
		MeshDesc.iDiffuseTextureIndex = 275;
		MeshDesc.iDiffuseTextureIndex = NODIFF;
		MeshDesc.iDiffuseTextureIndex = NONNOISE;
		MeshDesc.MaskTextureIndex = NONNMASK;
		MeshDesc.NoiseTextureIndex = NONNOISE;


		// Noise
		MeshDesc.noisingdir = _float2(0, 0).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 0.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		MeshDesc.fAlphaTestValue = 1.0f;

		// Color
		MeshDesc.vLimLight = _float4(0.3f, 0, 0, 0.0f);
		MeshDesc.vLimLight = _float4(0.57f, 0.33f, 0.15f, 1.0f);
		MeshDesc.vEmissive = _float4(1);
		MeshDesc.vEmissive = _float4(1,0.1f,1,1);


		// Transform_Base
		// LocalPos
		MeshDesc.vPosition = _float3(-0.0f, 3.0f, 0.0f);
		MeshDesc.vSize = _float3(10.0f);


		// Move
		MeshDesc.MoveDir = FollowingDir_Up;
		AddDesc.AccMoveSpeed = 0.0f;

		// Rot
		MeshDesc.vLookDir = _float3(0, 0, -1);
		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);
		AddDesc.InitRot = _float3(0);
		MeshDesc.RotAxis = FollowingDir_Up;
		AddDesc.AccRotSpeed = 0.0f;
		AddDesc.LookRotSpeed = 0;
		// Scale
		AddDesc.AccScaleSpeed = 0.0f;
		AddDesc.bLockScale[0] = false;
		AddDesc.bLockScale[1] = false;
		AddDesc.bLockScale[2] = false;


		// Fix
		AddDesc.FixFlag_Move = false;
		AddDesc.FixFlag_Rot = false;
		AddDesc.DealyTime = 0.1f;

		LightDesc.bEmsiive = true;
		LightDesc.bRim = true;
		LightDesc.Ambiant = 0.1f;
		_float Time = 2.5f;
		LightDesc.SpeedTime_Rim = Time;
		LightDesc.SpeedTime_Emsive = Time;

		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
		MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

	}

	if (type == MESHEFFECT_MAP_OBJ_Freeset1)
	{

		// light
		MeshDesc.eMeshType = Prototype_Mesh_doomBvert;

		// Time
		MeshDesc.fMaxTime_Duration = 5.0f;
		MeshDesc.fAppearTime = 0.5f;
		AddDesc.bAfterApperTime = false;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 394;
		MeshDesc.iDiffuseTextureIndex = NODIFF;
		MeshDesc.iDiffuseTextureIndex = 275;
		MeshDesc.iDiffuseTextureIndex = 271;
		MeshDesc.MaskTextureIndex = NONNMASK;
		MeshDesc.NoiseTextureIndex = NONNOISE;


		// Noise
		MeshDesc.noisingdir = _float2(0, 0).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 0.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		MeshDesc.fAlphaTestValue = 1.0f;

		// Color
		MeshDesc.vLimLight = _float4(1.0f, 1, 1, 0.0f);
		MeshDesc.vLimLight = _float4(0.3f, 0.3f, 0.3f, 1.0f);
		MeshDesc.vEmissive = _float4(1);


		// Transform_Base
		// LocalPos
		MeshDesc.vLookDir = _float3(0, 0, 1);
		MeshDesc.vPosition = _float3(-0.0f, 0.0f, 0.0f);
		MeshDesc.vSize = _float3(5.0f);


		// Move
		MeshDesc.MoveDir = FollowingDir_Look;
		AddDesc.AccMoveSpeed = 0.0f;

		// Rot
		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);
		AddDesc.InitRot = _float3(0,0,0);
		MeshDesc.RotAxis = FollowingDir_Up;
		AddDesc.AccRotSpeed = 0;
		AddDesc.LookRotSpeed = 0;

		// Scale
		AddDesc.AccScaleSpeed = 0.0f;
		AddDesc.bLockScale[0] = false;
		AddDesc.bLockScale[1] = false;
		AddDesc.bLockScale[2] = false;


		// Fix
		AddDesc.FixFlag_Move = false;
		AddDesc.FixFlag_Rot = false;
		AddDesc.DealyTime = 0.1f;

		LightDesc.bEmsiive = true;
		LightDesc.bRim = true;
		LightDesc.Ambiant = 0.2f;
		_float Time = 3.0f;
		LightDesc.SpeedTime_Rim = Time;
		LightDesc.SpeedTime_Emsive = Time;

		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
		MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

	}

	if (type == MESHEFFECT_MAP_OBJ_Freeset2)
	{

		// light
		MeshDesc.eMeshType = Prototype_Mesh_SM_GT_Cyinder_01;

		// Time
		MeshDesc.fMaxTime_Duration = 5.0f;
		MeshDesc.fAppearTime = 1.0f;
		AddDesc.bAfterApperTime = false;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 365;
		MeshDesc.iDiffuseTextureIndex = 278;
		
		MeshDesc.MaskTextureIndex = 57;
		MeshDesc.MaskTextureIndex = 109;



		MeshDesc.NoiseTextureIndex = 360;
		MeshDesc.NoiseTextureIndex = 92;
		MeshDesc.NoiseTextureIndex = 180;


		// Noise
		MeshDesc.noisingdir = _float2(0, -1).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		MeshDesc.fAlphaTestValue = 1.0f;

		// Color
		MeshDesc.vLimLight = _float4(1.0f, 1, 1, 0.0f);
		MeshDesc.vLimLight = _float4(0.3f, 0.3f, 0.3f, 1.0f);
		MeshDesc.vEmissive = _float4(1);


		// Transform_Base
		// LocalPos
		MeshDesc.vLookDir = _float3(0, 0, 1);
		MeshDesc.vPosition = _float3(0.0f, 4.0f, 0.0f); // 1
		MeshDesc.vSize = _float3(0.5f, 0.3f, 0.5f); // 1


		// Move
		MeshDesc.MoveDir = FollowingDir_Look;
		AddDesc.AccMoveSpeed = 0.0f;

		// Rot
		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);
		AddDesc.InitRot = _float3(0, 0, 0);
		MeshDesc.RotAxis = FollowingDir_Up;
		AddDesc.AccRotSpeed = 0;
		AddDesc.LookRotSpeed = 0;

		// Scale
		AddDesc.AccScaleSpeed = 0.0f;
		AddDesc.bLockScale[0] = false;
		AddDesc.bLockScale[1] = false;
		AddDesc.bLockScale[2] = false;


		// Fix
		AddDesc.FixFlag_Move = false;
		AddDesc.FixFlag_Rot = false;
		AddDesc.DealyTime = 0.1f;

		LightDesc.bEmsiive = false;
		LightDesc.bRim = false;
		LightDesc.Ambiant = 0.2f;
		_float Time = 3.0f;
		LightDesc.SpeedTime_Rim = Time;
		LightDesc.SpeedTime_Emsive = Time;

		// Shader
		MeshDesc.m_iPassIndex = 16; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå
	}
	

	if (type == MESHEFFECT_MAP_OBJ_Freeset3)
	{

		// light
		MeshDesc.eMeshType = Prototype_Mesh_SM_circle;

		// Time
		MeshDesc.fMaxTime_Duration = 5.0f;
		MeshDesc.fAppearTime = 1.0f;
		AddDesc.bAfterApperTime = false;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 365;
		MeshDesc.iDiffuseTextureIndex = 278;
		MeshDesc.MaskTextureIndex = 57;
		MeshDesc.NoiseTextureIndex = 360;


		// Noise
		MeshDesc.noisingdir = _float2(0, 1).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		MeshDesc.fAlphaTestValue = 0.1f;

		// Color
		MeshDesc.vLimLight = _float4(1.0f, 1, 1, 0.0f);
		MeshDesc.vLimLight = _float4(0.3f, 0.3f, 0.3f, 1.0f);
		MeshDesc.vEmissive = _float4(1);


		// Transform_Base
		// LocalPos
		MeshDesc.vLookDir = _float3(0, 0, 1);

		MeshDesc.vPosition = _float3(0.0f, 6.0f, 0.0f); // 1
		MeshDesc.vSize = _float3(10); // 1



		// Move
		MeshDesc.MoveDir = FollowingDir_Look;
		AddDesc.AccMoveSpeed = 0.0f;

		// Rot
		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);
		AddDesc.InitRot = _float3(0, 0, 0);
		MeshDesc.RotAxis = FollowingDir_Up;
		AddDesc.AccRotSpeed = 0;
		AddDesc.LookRotSpeed = 0;

		// Scale
		AddDesc.AccScaleSpeed = 0.0f;
		AddDesc.bLockScale[0] = false;
		AddDesc.bLockScale[1] = false;
		AddDesc.bLockScale[2] = false;


		// Fix
		AddDesc.FixFlag_Move = false;
		AddDesc.FixFlag_Rot = false;
		AddDesc.DealyTime = 0.1f;

		LightDesc.bEmsiive = false;
		LightDesc.bRim = false;
		LightDesc.Ambiant = 0.2f;
		_float Time = 3.0f;
		LightDesc.SpeedTime_Rim = Time;
		LightDesc.SpeedTime_Emsive = Time;

		// Shader
		MeshDesc.m_iPassIndex = 16; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

	}


	if (type == MESHEFFECT_MAP_OBJ_Freeset4)
	{
		
		// light
		MeshDesc.eMeshType = Prototype_Mesh_SM_ENV_DT_StatueFemale_Pose01;

		// Time
		MeshDesc.fMaxTime_Duration = 3.0f;
		MeshDesc.fAppearTime = 0.5f;
		AddDesc.bAfterApperTime = false;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 394;
		MeshDesc.iDiffuseTextureIndex = 275;
		MeshDesc.iDiffuseTextureIndex = NODIFF;
		MeshDesc.iDiffuseTextureIndex = NONNOISE;
		MeshDesc.MaskTextureIndex = NONNMASK;
		MeshDesc.NoiseTextureIndex = NONNOISE;


		// Noise
		MeshDesc.noisingdir = _float2(0, 1).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		MeshDesc.fAlphaTestValue = 0.3f;

		// Color
		MeshDesc.vLimLight = _float4(0.3f, 0, 0, 0.0f);
		MeshDesc.vLimLight = _float4(0.04f, 0.68f, 1.0f, 1.0f);
		MeshDesc.vEmissive = _float4(1);
		MeshDesc.vEmissive = _float4(1, 0.1f, 1, 1);


		// Transform_Base
		// LocalPos
		MeshDesc.vPosition = _float3(-0.0f, 1.0f, 0.0f);
		MeshDesc.vSize = _float3(1.0f);


		// Move
		MeshDesc.MoveDir = FollowingDir_Up;
		AddDesc.AccMoveSpeed = 0.0f;

		// Rot
		MeshDesc.vLookDir = _float3(0, 0, 1);
		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);
		AddDesc.InitRot = _float3(0);
		MeshDesc.RotAxis = FollowingDir_Up;
		AddDesc.AccRotSpeed = 0.0f;
		AddDesc.LookRotSpeed = 0;

		// Scale
		AddDesc.AccScaleSpeed = 0.0f;
		AddDesc.bLockScale[0] = false;
		AddDesc.bLockScale[1] = false;
		AddDesc.bLockScale[2] = false;


		// Fix
		AddDesc.FixFlag_Move = false;
		AddDesc.FixFlag_Rot = false;
		AddDesc.DealyTime = 0.1f;

		LightDesc.bEmsiive = true;
		LightDesc.bRim = true;
		LightDesc.Ambiant = 0.1f;
		_float Time = 4.0f;
		LightDesc.SpeedTime_Rim = Time;
		LightDesc.SpeedTime_Emsive = Time;

		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
		MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

	}



	desc1 = MeshDesc;
	desc2 = AddDesc;
	desc3 = LightDesc;

	return true;
}



CNonInstanceMeshEffect_TT * CPartilceCreateMgr::GetMeshEffect() const
{
	return mPreMeshEffect;
}

CNonInstanceMeshEffect_TT_Fix * CPartilceCreateMgr::GetMeshEffect_Fix() const
{
	return mPreMeshEffect_Fix;
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
	// #EFFECT

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
	mVecTextureEffectDesc[TEXTURE_EFFECTJ_Bow_ArrowHit2] = pUtil->Get_TextureParticleDesc(TEXT("Bow_ArrowHit2"));
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
	mVecTextureEffectDesc[TEXTURE_EFFECTJ_Bow_Q_Snow] = pUtil->Get_TextureParticleDesc(TEXT("Bow_Q_Snow"));



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
	mVecTextureEffectDesc[JY_TextureEft_9] = pUtil->Get_TextureParticleDesc(TEXT("JY_TextureEft_9"));
	mVecTextureEffectDesc[JY_TextureEft_10] = pUtil->Get_TextureParticleDesc(TEXT("JY_TextureEft_10"));
	mVecTextureEffectDesc[JY_TextureEft_11] = pUtil->Get_TextureParticleDesc(TEXT("JY_TextureEft_11"));
	mVecTextureEffectDesc[JY_TextureEft_12] = pUtil->Get_TextureParticleDesc(TEXT("JY_TextureEft_12"));
	mVecTextureEffectDesc[JY_TextureEft_13] = pUtil->Get_TextureParticleDesc(TEXT("JY_TextureEft_13"));
	mVecTextureEffectDesc[JY_TextureEft_14] = pUtil->Get_TextureParticleDesc(TEXT("JY_TextureEft_14"));
	mVecTextureEffectDesc[JY_TextureEft_15] = pUtil->Get_TextureParticleDesc(TEXT("JY_TextureEft_15"));
	mVecTextureEffectDesc[JY_TextureEft_16] = pUtil->Get_TextureParticleDesc(TEXT("JY_TextureEft_16"));
	mVecTextureEffectDesc[JY_TextureEft_17] = pUtil->Get_TextureParticleDesc(TEXT("JY_TextureEft_17"));
	mVecTextureEffectDesc[JY_TextureEft_18] = pUtil->Get_TextureParticleDesc(TEXT("JY_TextureEft_18"));
	mVecTextureEffectDesc[JY_TextureEft_19] = pUtil->Get_TextureParticleDesc(TEXT("JY_TextureEft_19"));
	//mVecTextureEffectDesc[JY_TextureEft_20] = pUtil->Get_TextureParticleDesc(TEXT("JY_TextureEft_20"));
	//mVecTextureEffectDesc[JY_TextureEft_21] = pUtil->Get_TextureParticleDesc(TEXT("JY_TextureEft_21"));
	//mVecTextureEffectDesc[JY_TextureEft_22] = pUtil->Get_TextureParticleDesc(TEXT("JY_TextureEft_22"));
	//mVecTextureEffectDesc[JY_TextureEft_23] = pUtil->Get_TextureParticleDesc(TEXT("JY_TextureEft_23"));
	//mVecTextureEffectDesc[JY_TextureEft_24] = pUtil->Get_TextureParticleDesc(TEXT("JY_TextureEft_24"));
	//mVecTextureEffectDesc[JY_TextureEft_25] = pUtil->Get_TextureParticleDesc(TEXT("JY_TextureEft_25"));



	mVecTextureEffectDesc[Spear_ThrowAttack] = pUtil->Get_TextureParticleDesc(TEXT("Spear_ThrowAttack"));
	mVecTextureEffectDesc[SpearNormalAttack] = pUtil->Get_TextureParticleDesc(TEXT("SpearNormalAttack"));

	mVecTextureEffectDesc[Um_Dust_1] = pUtil->Get_TextureParticleDesc(TEXT("Um_Dust_1"));
	mVecTextureEffectDesc[Um_Dust_2] = pUtil->Get_TextureParticleDesc(TEXT("Um_Dust_2"));
	mVecTextureEffectDesc[Um_Dust_2_FounTain] = pUtil->Get_TextureParticleDesc(TEXT("Um_Dust_2_FounTain"));
	mVecTextureEffectDesc[Um_Dust_3] = pUtil->Get_TextureParticleDesc(TEXT("Um_Dust_3"));
	mVecTextureEffectDesc[Um_FireMask_1] = pUtil->Get_TextureParticleDesc(TEXT("Um_FireMask_1"));
	mVecTextureEffectDesc[Um_FireMask_2] = pUtil->Get_TextureParticleDesc(TEXT("Um_FireMask_2"));
	mVecTextureEffectDesc[Um_FireMask_2_png] = pUtil->Get_TextureParticleDesc(TEXT("Um_FireMask_2_png"));
	mVecTextureEffectDesc[Um_FireMask_3] = pUtil->Get_TextureParticleDesc(TEXT("Um_FireMask_3"));
	mVecTextureEffectDesc[Um_Hit_1] = pUtil->Get_TextureParticleDesc(TEXT("Um_Hit_1"));
	mVecTextureEffectDesc[Um_Hit_2] = pUtil->Get_TextureParticleDesc(TEXT("Um_Hit_2"));
	mVecTextureEffectDesc[Um_Hit_2_DisDiffuse] = pUtil->Get_TextureParticleDesc(TEXT("Um_Hit_2_DisDiffuse"));
	mVecTextureEffectDesc[Um_Hit_3] = pUtil->Get_TextureParticleDesc(TEXT("Um_Hit_3"));
	mVecTextureEffectDesc[Um_Hit_4] = pUtil->Get_TextureParticleDesc(TEXT("Um_Hit_4"));
	mVecTextureEffectDesc[Um_Imgae_1] = pUtil->Get_TextureParticleDesc(TEXT("Um_Imgae_1"));
	mVecTextureEffectDesc[Um_Imgae_2] = pUtil->Get_TextureParticleDesc(TEXT("Um_Imgae_2"));
	mVecTextureEffectDesc[Um_Roll_1] = pUtil->Get_TextureParticleDesc(TEXT("Um_Roll_1"));
	mVecTextureEffectDesc[Um_Sunder_1] = pUtil->Get_TextureParticleDesc(TEXT("Um_Sunder_1"));
	mVecTextureEffectDesc[Um_Up_1] = pUtil->Get_TextureParticleDesc(TEXT("Um_Up_1"));

	mVecTextureEffectDesc[Um_Spawn1_Image] = pUtil->Get_TextureParticleDesc(TEXT("Um_Spawn1_Image"));
	mVecTextureEffectDesc[Um_Spawn1_Image_suck] = pUtil->Get_TextureParticleDesc(TEXT("Um_Spawn1_Image_suck"));
	mVecTextureEffectDesc[Um_Spawn2_Image] = pUtil->Get_TextureParticleDesc(TEXT("Um_Spawn2_Image"));
	mVecTextureEffectDesc[Um_Spawn2_Image_power] = pUtil->Get_TextureParticleDesc(TEXT("Um_Spawn2_Image_power"));
	mVecTextureEffectDesc[Um_Spawn2_Image_powerdown] = pUtil->Get_TextureParticleDesc(TEXT("Um_Spawn2_Image_powerdown"));
	mVecTextureEffectDesc[Um_Spawn3_Imagepng] = pUtil->Get_TextureParticleDesc(TEXT("Um_Spawn3_Imagepng"));
	mVecTextureEffectDesc[Um_Spawn3_Imagepng_ground] = pUtil->Get_TextureParticleDesc(TEXT("Um_Spawn3_Imagepng_ground"));
	mVecTextureEffectDesc[Um_Spawn3_Imagepng_ground2] = pUtil->Get_TextureParticleDesc(TEXT("Um_Spawn3_Imagepng_ground2"));
	mVecTextureEffectDesc[Um_Spawn3_Imagepng_magic] = pUtil->Get_TextureParticleDesc(TEXT("Um_Spawn3_Imagepng_magic"));
	mVecTextureEffectDesc[Um_Spawn3_Imagepng_magic2] = pUtil->Get_TextureParticleDesc(TEXT("Um_Spawn3_Imagepng_magic2"));
	mVecTextureEffectDesc[Um_Spawn3_Imagepng_Snow] = pUtil->Get_TextureParticleDesc(TEXT("Um_Spawn3_Imagepng_Snow"));
	mVecTextureEffectDesc[Um_Spawn3_Imagepng_Snow2] = pUtil->Get_TextureParticleDesc(TEXT("Um_Spawn3_Imagepng_Snow2"));
	mVecTextureEffectDesc[Um_Spawn4_smoke] = pUtil->Get_TextureParticleDesc(TEXT("Um_Spawn4_smoke"));
	mVecTextureEffectDesc[Um_Spawn5_Png_Ring] = pUtil->Get_TextureParticleDesc(TEXT("Um_Spawn5_Png_Ring"));

	
	
	
	
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
	mVecMeshInstDesc[Um_MeshBase2] = pUtil->Get_MeshParticleDesc(TEXT("Um_MeshBase2"));
	mVecMeshInstDesc[Um_MeshBase3] = pUtil->Get_MeshParticleDesc(TEXT("Um_MeshBase3"));
	mVecMeshInstDesc[Um_MeshBase_Cone] = pUtil->Get_MeshParticleDesc(TEXT("Um_MeshBase_Cone"));
	mVecMeshInstDesc[Um_Mesh_Sword1] = pUtil->Get_MeshParticleDesc(TEXT("Um_Mesh_Sword1"));
	mVecMeshInstDesc[Um_Mesh_Sword2] = pUtil->Get_MeshParticleDesc(TEXT("Um_Mesh_Sword2"));
	mVecMeshInstDesc[Um_Mesh_MaskApper] = pUtil->Get_MeshParticleDesc(TEXT("Um_Mesh_MaskApper"));

	mVecMeshInstDesc[Um_MeshBase4] = pUtil->Get_MeshParticleDesc(TEXT("Um_MeshBase4"));
	mVecMeshInstDesc[Um_MeshBase4_NoOnetime] = pUtil->Get_MeshParticleDesc(TEXT("Um_MeshBase4_NoOnetime"));
	mVecMeshInstDesc[Um_MeshBase4_TurnAuto] = pUtil->Get_MeshParticleDesc(TEXT("Um_MeshBase4_TurnAuto"));
	mVecMeshInstDesc[Um_MeshBase4_TurnRight] = pUtil->Get_MeshParticleDesc(TEXT("Um_MeshBase4_TurnRight"));

	

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


		MeshDesc.fMaxTime_Duration = 30;
		MeshDesc.fAppearTime = 0.8f;


		MeshDesc.iDiffuseTextureIndex = 335;
		MeshDesc.iDiffuseTextureIndex = 302;
		MeshDesc.MaskTextureIndex = NONNMASK;
		MeshDesc.NoiseTextureIndex = 323;

		MeshDesc.noisingdir = _float2(0, 1.0f).Get_Nomalize();
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		MeshDesc.fDistortionNoisingPushPower = 5.0f;

		MeshDesc.vLimLight = _float4(0.1f, 0.76f, 0.77f, 1.0f);
		MeshDesc.vEmissive = _float4(0.f, 0.1f, 0.1f, 1.0f);


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

		MeshDesc.fMaxTime_Duration = 999;
		MeshDesc.fAppearTime = 0.4f;

		MeshDesc.iDiffuseTextureIndex = 278;
		MeshDesc.iDiffuseTextureIndex = 411;
		MeshDesc.MaskTextureIndex = NONNMASK;
		MeshDesc.NoiseTextureIndex = NONNOISE;

		MeshDesc.noisingdir = _float2(0.0f, 1.0f).Get_Nomalize();
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		MeshDesc.fDistortionNoisingPushPower = 3.0f;

		MeshDesc.vLimLight = _float4(0.0f, 0.40f, 0.89f, 1.0f);
		MeshDesc.vEmissive = _float4(1.0f, 0.1f, 0.1f, 0.0f);


		MeshDesc.vPosition = _float3(0, 0.0f, 0.0f);
		MeshDesc.vSize = _float3(2.5f);

		MeshDesc.RotAxis = FollowingDir_Up;
		MeshDesc.RotationSpeedPerSec = 0;
		AddDesc.vAddDirectAngle = _float3(0,0,90);
		//	_float3 Pos = Transfom->Get_MatrixState(CTransform::STATE_POS);
		//	_float3 dir =  Transfom->Get_MatrixState(CTransform::STATE_UP);

		MeshDesc.vLookDir = _float3(0, 0, 1);


		//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
			MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		AddDesc.LookRotAxis = FollowingDir_Up;
		AddDesc.vAddDirectAngle = _float3(0, 270, 0);

		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);
	}


	if (type == MESHEFFECT_ARROW_BOW_SHIFT_PLANE)
	{
		

		
			MeshDesc.eMeshType = Prototype_Mesh_SM_circle;
			MeshDesc.eMeshType = Prototype_Mesh_SM_ControlPointMatch_Square_02;


			MeshDesc.fMaxTime_Duration = 999;
			MeshDesc.fAppearTime = 0.2f;

			MeshDesc.iDiffuseTextureIndex = 395;

			MeshDesc.MaskTextureIndex = 31;
			MeshDesc.MaskTextureIndex = 71;
			MeshDesc.MaskTextureIndex = 179;
			MeshDesc.MaskTextureIndex = NONNMASK;
			MeshDesc.NoiseTextureIndex = NONNOISE;
			MeshDesc.NoiseTextureIndex = 325;
			MeshDesc.NoiseTextureIndex = 294;

			MeshDesc.noisingdir = _float2(1, 1).Get_Nomalize();
			MeshDesc.vColor = _float4(1, 1, 1, 1);
			MeshDesc.fDistortionNoisingPushPower = 5.0f;
			MeshDesc.fAlphaTestValue = 1.0f;

			MeshDesc.vLimLight = _float4(0.03f, 0.06f, 0.82f, 1.0f);
			MeshDesc.vEmissive = _float4(1.0f, 0.8f, 0.5f, 1.f);


			//	MeshDesc.vRotAxis = _float3(1, 0, 0);
			MeshDesc.vPosition = _float3(0, 0.2f, 0);
			MeshDesc.vSize = _float3(1.3f);

			MeshDesc.RotAxis = FollowingDir_Up;
			MeshDesc.RotationSpeedPerSec = 20;

			//	_float3 Pos = Transfom->Get_MatrixState(CTransform::STATE_POS);
			//	_float3 dir =  Transfom->Get_MatrixState(CTransform::STATE_UP);

			MeshDesc.vLookDir = _float3(1, 0, 0);


			//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
			//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
			//	MeshDesc.m_iPassIndex = 18; // DisCard
			MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

			AddDesc.LookRotAxis = FollowingDir_Right;

			GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);
		
	}

	if (type == MESHEFFECT_ARROW_BOW_SHIFT_ICE)
	{
		// demo
		{
			MeshDesc.eMeshType = Prototype_Mesh_SM_circle;

			MeshDesc.fMaxTime_Duration = 999;
			MeshDesc.fAppearTime = 0.5f;

			MeshDesc.iDiffuseTextureIndex = 365;
			MeshDesc.iDiffuseTextureIndex = 317;
			MeshDesc.iDiffuseTextureIndex = 278;
			MeshDesc.MaskTextureIndex = 179;
			MeshDesc.MaskTextureIndex = 65;
			MeshDesc.NoiseTextureIndex = 294;
			MeshDesc.NoiseTextureIndex = 237;

			MeshDesc.noisingdir = _float2(0.0f, 1.0f).Get_Nomalize();
			MeshDesc.vColor = _float4(1, 1, 1, 1);
			MeshDesc.fDistortionNoisingPushPower = 5.0f;

			MeshDesc.vLimLight = _float4(0.02f, 0.16f, 0.90f, 1.f);
			MeshDesc.vEmissive = _float4(0.7f, 0.9f, 0.5f, 1.f);


			//	MeshDesc.vRotAxis = _float3(1, 0, 0);
			MeshDesc.vPosition = _float3(-0.0f, 0, -0.0f);
			MeshDesc.vSize = _float3(70);

			MeshDesc.RotAxis = FollowingDir_Up;
			MeshDesc.RotationSpeedPerSec = 10;

			//	_float3 Pos = Transfom->Get_MatrixState(CTransform::STATE_POS);
			//	_float3 dir =  Transfom->Get_MatrixState(CTransform::STATE_UP);

			MeshDesc.vLookDir = _float3(1, 0, 0);


			//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
			MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
		//	MeshDesc.m_iPassIndex = 18; // DisCard
		//  MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

			AddDesc.LookRotAxis = FollowingDir_Right;
			AddDesc.FixFlag_Move = true;

			GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

		}

		{
			// ori
			MeshDesc.eMeshType = Prototype_Mesh_SM_circle;


			MeshDesc.fMaxTime_Duration = 999;
			MeshDesc.fAppearTime = 0.3f;

			MeshDesc.iDiffuseTextureIndex = 365;

			MeshDesc.MaskTextureIndex = 31;
			MeshDesc.MaskTextureIndex = NONNMASK;
			MeshDesc.MaskTextureIndex = 179;
			MeshDesc.NoiseTextureIndex = NONNOISE;
			MeshDesc.NoiseTextureIndex = 325;
			MeshDesc.NoiseTextureIndex = 294;

			MeshDesc.noisingdir = _float2(1, 1).Get_Nomalize();
			MeshDesc.vColor = _float4(1, 1, 1, 1);
			MeshDesc.fDistortionNoisingPushPower = 5.0f;
			MeshDesc.fAlphaTestValue = 1.0f;

			MeshDesc.vLimLight = _float4(0.03f, 0.06f, 0.82f, 1.0f);
			MeshDesc.vEmissive = _float4(1.0f, 0.8f, 0.5f, 1.f);


			//	MeshDesc.vRotAxis = _float3(1, 0, 0);
			MeshDesc.vPosition = _float3(0);
			MeshDesc.vSize = _float3(92);
			MeshDesc.vSize = _float3(70);

			MeshDesc.RotAxis = FollowingDir_Up;
			MeshDesc.RotationSpeedPerSec = 20;

			//	_float3 Pos = Transfom->Get_MatrixState(CTransform::STATE_POS);
			//	_float3 dir =  Transfom->Get_MatrixState(CTransform::STATE_UP);

			MeshDesc.vLookDir = _float3(1, 0, 0);


			//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
			//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
			//	MeshDesc.m_iPassIndex = 18; // DisCard
			MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

			AddDesc.LookRotAxis = FollowingDir_Right;

		//	GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);
		}
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
		MeshDesc.fAppearTime = 0.4f;
		AddDesc.bAfterApperTime = false;

		MeshDesc.iDiffuseTextureIndex = 364;
		MeshDesc.MaskTextureIndex = 57;
		MeshDesc.MaskTextureIndex = NONNMASK;
		MeshDesc.NoiseTextureIndex = 366;

		MeshDesc.noisingdir = _float2(1, 0).Get_Nomalize();
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		MeshDesc.fDistortionNoisingPushPower = 5.0f;

		MeshDesc.vLimLight = _float4(0.10f, 0.21f, 0.50f, 1.0f);
		MeshDesc.vEmissive = _float4(1.f, 0.1f, 0.1f, 0.f);


		//	MeshDesc.vRotAxis = _float3(1, 0, 0);
		MeshDesc.vPosition = _float3(0.0f, 0.0f, -0.3f);
		MeshDesc.vSize = _float3(2.5f);

		MeshDesc.RotAxis = FollowingDir_Up;
		MeshDesc.RotationSpeedPerSec = 10;

		MeshDesc.vLookDir = _float3(0, 0, 1);

		MeshDesc.fAlphaTestValue = 0.1f;
		MeshDesc.m_iPassIndex = bRandomVal ? 17 : 19; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		AddDesc.LookRotAxis = FollowingDir_Right;
		AddDesc.AccRotSpeed = -10;
		AddDesc.vAddDirectAngle = _float3(0,90, 0);

		// Scale
		AddDesc.AccScaleSpeed = 0;
		AddDesc.ScaleMax = 0;
		AddDesc.ScaleReFlag = false;

		AddDesc.bLockScale[0] = true;
		AddDesc.bLockScale[1] = true;
		AddDesc.bLockScale[2] = true;


		// Fix
		AddDesc.FixFlag_Move = false;
		AddDesc.FixFlag_Rot = false;
		AddDesc.FollowTarget = nullptr;

		MeshDesc.vPosition = _float3(0.3f, 0.7f, -0.5f);
		MeshDesc.vSize = _float3(3.0f);
		AddDesc.AccScaleSpeed = 6.0f;
		AddDesc.ScaleMax = 3.2f;

		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

		MeshDesc.vPosition = _float3(0.3f, 0.5f, -0.4f);
		MeshDesc.vSize = _float3(2.5f);
		AddDesc.AccScaleSpeed = 3.0f;
		AddDesc.ScaleMax = 2.3f;

		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);


		MeshDesc.vPosition = _float3(0.3f, 0.4f, -0.3f);
		MeshDesc.vSize = _float3(1.3f);
		AddDesc.AccScaleSpeed = 0.0f;
		AddDesc.ScaleMax = 0.0f;

		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);


	}
	if (type == MESHEFFECT_ARROW_BOW_R_JUMP_WING2)
	{
		MeshDesc.eMeshType = Prototype_Mesh_SM_Bow_Em_Wing_T;

		MeshDesc.fMaxTime_Duration = 1.0f;
		MeshDesc.fAppearTime = 0.4f;
		AddDesc.bAfterApperTime = false;

		MeshDesc.iDiffuseTextureIndex = 364;
		MeshDesc.MaskTextureIndex = 57;
		MeshDesc.MaskTextureIndex = NONNMASK;
		MeshDesc.NoiseTextureIndex = 366;

		MeshDesc.noisingdir = _float2(1, 0).Get_Nomalize();
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		MeshDesc.fDistortionNoisingPushPower = 5.0f;

		MeshDesc.vLimLight = _float4(0.10f, 0.21f, 0.50f, 1.0f);
		MeshDesc.vEmissive = _float4(1.f, 0.1f, 0.1f, 0.f);


		//	MeshDesc.vRotAxis = _float3(1, 0, 0);
		MeshDesc.vPosition = _float3(0.0f, 0.0f, -0.3f);
		MeshDesc.vSize = _float3(2.5f);

		MeshDesc.RotAxis = FollowingDir_Up;
		MeshDesc.RotationSpeedPerSec = 10;

		MeshDesc.vLookDir = _float3(0, 0, 1);

		MeshDesc.fAlphaTestValue = 0.1f;
		MeshDesc.m_iPassIndex = bRandomVal ? 17 : 19; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.AccRotSpeed = -45;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);

		// Scale
		AddDesc.AccScaleSpeed = 0;
		AddDesc.ScaleMax = 0;
		AddDesc.ScaleReFlag = false;

		AddDesc.bLockScale[0] = true;
		AddDesc.bLockScale[1] = true;
		AddDesc.bLockScale[2] = true;


		// Fix
		AddDesc.FixFlag_Move = false;
		AddDesc.FixFlag_Rot = false;
		AddDesc.FollowTarget = nullptr;

		MeshDesc.vPosition = _float3(-0.3f, 0.7f, -0.5f);
		MeshDesc.vSize = _float3(3.0f);
		AddDesc.AccScaleSpeed = 6.0f;
		AddDesc.ScaleMax = 3.2f;

		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

		MeshDesc.vPosition = _float3(-0.3f, 0.5f, -0.4f);
		MeshDesc.vSize = _float3(2.5f);
		AddDesc.AccScaleSpeed = 3.0f;
		AddDesc.ScaleMax = 2.3f;

		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);


		MeshDesc.vPosition = _float3(-0.3f, 0.4f, -0.3f);
		MeshDesc.vSize = _float3(1.3f);
		AddDesc.AccScaleSpeed = 0.0f;
		AddDesc.ScaleMax = 0.0f;

		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

	}


	if (type == MESHEFFECT_ARROW_BOW_SP_MOVE_SPEHERE)
	{
		MeshDesc.eMeshType = Prototype_Mesh_mySphere;
		MeshDesc.eMeshType = Prototype_Mesh_SM_ice2;
		

		MeshDesc.fMaxTime_Duration = 3.0f;

		MeshDesc.iDiffuseTextureIndex = 365;
		MeshDesc.iDiffuseTextureIndex = 317;
		MeshDesc.MaskTextureIndex = NONNMASK;
		MeshDesc.MaskTextureIndex = 33;
		MeshDesc.NoiseTextureIndex = NONNOISE;
		MeshDesc.NoiseTextureIndex = 258;

		MeshDesc.noisingdir = _float2(1, 1).Get_Nomalize();
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		MeshDesc.fAppearTime = 0.0f;
		MeshDesc.fDistortionNoisingPushPower = 10.0f;

		MeshDesc.vLimLight = _float4(0, 0, 1, 1.0f);
		MeshDesc.vEmissive = _float4(0.0f, 0.f, 0.f, 0.f);

		//	MeshDesc.vRotAxis = _float3(1, 0, 0);
		MeshDesc.vPosition = _float3(0, 0.5f, -0.0f);
		MeshDesc.vSize = _float3(10.0f);

		MeshDesc.RotAxis = FollowingDir_Up;
		MeshDesc.RotationSpeedPerSec = 0;

		MeshDesc.vLookDir = _float3(0, 0, 1);

		//	MeshDesc.MoveDir = FollowingDir_Look;
		//	MeshDesc.MoveSpeed = 30;

		MeshDesc.fAlphaTestValue = 0.0f;

		//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
		//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
		//	MeshDesc.m_iPassIndex = 18; // DisCard
		MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		AddDesc.LookRotAxis = FollowingDir_Look;
		// AddDesc.AccRotSpeed = -28.0f;
		
		
		// Scale
		AddDesc.AccScaleSpeed = 3.0f;
		AddDesc.ScaleMax = 30.0f;

		AddDesc.ScaleReFlag = false;

		AddDesc.bLockScale[0] = true;
		AddDesc.bLockScale[1] = true;
		AddDesc.bLockScale[2] = true;


		AddDesc.bEmissive = true;

		pEaseDesc = NEW MESHAEASING[2];
		pEaseDesc[0] = CreateEasingDesc(TYPE_Linear, _float3(0, 5.2f, 0), 0.2f);
		pEaseDesc[1] = CreateEasingDesc(TYPE_Linear, _float3(0, 5.2f, 0), 2.0f);
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
		MeshDesc.fAppearTime = 0.5f;

		MeshDesc.iDiffuseTextureIndex = 365;
		MeshDesc.iDiffuseTextureIndex = 317;
		MeshDesc.iDiffuseTextureIndex = 278;
		MeshDesc.MaskTextureIndex = 179;
		MeshDesc.MaskTextureIndex = 65;
		MeshDesc.NoiseTextureIndex = 294;
		MeshDesc.NoiseTextureIndex = 237;

		MeshDesc.noisingdir = _float2(0.0f, 1.0f).Get_Nomalize();
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		MeshDesc.fDistortionNoisingPushPower = 5.0f;

		MeshDesc.vLimLight = _float4(0.02f, 0.16f, 0.90f, 1.f);
		MeshDesc.vEmissive = _float4(0.7f, 0.9f, 0.5f, 1.f);


		//	MeshDesc.vRotAxis = _float3(1, 0, 0);
		MeshDesc.vPosition = _float3(-0.0f, 0, -0.0f);
		MeshDesc.vSize = _float3(50.0f);

		MeshDesc.RotAxis = FollowingDir_Up;
		MeshDesc.RotationSpeedPerSec = 10;

		//	_float3 Pos = Transfom->Get_MatrixState(CTransform::STATE_POS);
		//	_float3 dir =  Transfom->Get_MatrixState(CTransform::STATE_UP);

		MeshDesc.vLookDir = _float3(1, 0, 0);


		//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
			MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
		//	MeshDesc.m_iPassIndex = 18; // DisCard
		//  MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		AddDesc.LookRotAxis = FollowingDir_Right;
		AddDesc.FixFlag_Move = true;

		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

	}


	if (type == MESHEFFECT_ARROW_BOW_SP_PLANE3)
	{
		MeshDesc.eMeshType = Prototype_Mesh_MS_ST3_Portal_00;

		MeshDesc.fMaxTime_Duration = 4.0f;
		MeshDesc.fAppearTime = 0.35f;

		MeshDesc.iDiffuseTextureIndex = 236;
		MeshDesc.iDiffuseTextureIndex = 300;
		MeshDesc.MaskTextureIndex = NONNMASK;
		MeshDesc.MaskTextureIndex = 178;
		MeshDesc.MaskTextureIndex = 13;
		MeshDesc.NoiseTextureIndex = 294;



		MeshDesc.noisingdir = _float2(0.0f, 1.0f).Get_Nomalize();
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		MeshDesc.fDistortionNoisingPushPower = 10.0f;

		MeshDesc.vLimLight = _float4(0.13f, 0.16f, 0.82f, 1.f);
		MeshDesc.vEmissive = _float4(0.8f, 0.4f, 0.82f, 1.f);


		//	MeshDesc.vRotAxis = _float3(1, 0, 0);
		MeshDesc.vPosition = _float3(-0.0f, -0.8f, -0.0f);
		MeshDesc.vSize = _float3(8.0f);
		MeshDesc.vSize = _float3(8.0f,5.0f,8.0f);

		MeshDesc.RotAxis = FollowingDir_Up;
		MeshDesc.RotationSpeedPerSec = 45;

		//	_float3 Pos = Transfom->Get_MatrixState(CTransform::STATE_POS);
		//	_float3 dir =  Transfom->Get_MatrixState(CTransform::STATE_UP);

		MeshDesc.vLookDir = _float3(1, 0, 0);


		//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCardB
		MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		AddDesc.LookRotAxis = FollowingDir_Right;
		AddDesc.FixFlag_Move = true;
		AddDesc.AccRotSpeed = -10.0f;

		MeshDesc.vLimLight = _float4(0.13f, 0.16f, 0.82f, 1.f);
		MeshDesc.fAlphaTestValue = 0.6f;
		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

		MeshDesc.vPosition = _float3(-0.0f, -1.0f, -0.0f);
		MeshDesc.vLimLight = _float4(0.03f, 0.06f, 0.92f, 1.f);
		MeshDesc.fAlphaTestValue = 0.0f;

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
		MeshDesc.eMeshType = Prototype_Mesh_SM_ky_tornado3;
		MeshDesc.eMeshType = Prototype_Mesh_SM_SG_tornado;

		// MAX
		MeshDesc.fMaxTime_Duration = 4.0f;
		MeshDesc.fAppearTime = 0.35f;

		MeshDesc.iDiffuseTextureIndex = 317;
		MeshDesc.iDiffuseTextureIndex = 278;
		MeshDesc.MaskTextureIndex = NONNMASK;
		MeshDesc.MaskTextureIndex = 29;
		MeshDesc.MaskTextureIndex = 109;
		MeshDesc.NoiseTextureIndex = NONNOISE;
		MeshDesc.NoiseTextureIndex = 362;

		MeshDesc.noisingdir = _float2(0.0f, 1.0f).Get_Nomalize();
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		MeshDesc.fDistortionNoisingPushPower = 10.0f;

		MeshDesc.vLimLight = _float4(0.09f, 0.09f, 0.75f, 1.0f);
		MeshDesc.vEmissive = _float3(0.8f, 0.6f, 0.1f);
		MeshDesc.fAlphaTestValue = 0.55f;


		//	MeshDesc.vRotAxis = _float3(1, 0, 0);
		MeshDesc.vPosition = _float3(0.0f, -0.5f, 0.0f);
		MeshDesc.vSize = _float3(10.0f, 30.0f, 10.0f);
		MeshDesc.vSize = _float3(1.5f, 2.5f, 1.5f);

		MeshDesc.RotAxis = FollowingDir_Up;
		MeshDesc.RotationSpeedPerSec = 40.0f;

		MeshDesc.vLookDir = _float3(0, 0, 1);

		// MeshDesc.m_iPassIndex = 16; // ¿Ö°î
		MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	   // MeshDesc.m_iPassIndex = 18; // DisCard
		MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.FixFlag_Move = true;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);
		AddDesc.AccRotSpeed = 5.0f;
		MeshDesc.vPosition = _float3(0.0f, -0.7f, 0.0f);

		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

		MeshDesc.eMeshType = Prototype_Mesh_SM_Helix_Crystal;
		MeshDesc.vSize = _float3(0.2f, 0.4f, 0.2f);
		MeshDesc.MaskTextureIndex = 59;
		MeshDesc.NoiseTextureIndex = NONNOISE;

		MeshDesc.fAlphaTestValue = 0.3f;
		MeshDesc.vPosition = _float3(0.0f, -0.5f, 0.0f);

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

	if (type == MESHEFFECT_MONSTER_CREATE3)
	{

		// Plane
		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_MS_Shot_Cylinder_00;

		// Time
		MeshDesc.fMaxTime_Duration = 0.7f;
		MeshDesc.fAppearTime = 0.4f;
		AddDesc.bAfterApperTime = true;

		// Tex
		MeshDesc.iDiffuseTextureIndex = bRandomVal ? 375 : 365;

		MeshDesc.MaskTextureIndex = NONNMASK;
		MeshDesc.MaskTextureIndex = 13;
		MeshDesc.NoiseTextureIndex = NONNOISE;
		MeshDesc.NoiseTextureIndex = 54;
		MeshDesc.NoiseTextureIndex = 383;


		// Noise
		MeshDesc.noisingdir = _float2(0, 1).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);

		// Color
		MeshDesc.vLimLight = _float4(0.1f, 0.4f, 0.96f, 0.3f);
		MeshDesc.vEmissive = _float4(1);
		//	MeshDesc.vEmissive = _float4(0.1f, 0.3f, 0.1f, 1.f);

			// Transform_Base
		MeshDesc.vPosition = _float3(0.0f, 1.5f, 0);
		MeshDesc.vSize = _float3(6.0f);

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
		AddDesc.FollowTarget = nullptr;

		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
		MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

	}

	if (type == MESHEFFECT_MONSTER_SPAWN_TM)
	{


		// Plane
		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_ice2;
		//	MeshDesc.eMeshType = Prototype_Mesh_Plat_Wall;
	//	MeshDesc.eMeshType = Prototype_Mesh_ice2;
	//	MeshDesc.eMeshType = Prototype_Mesh_SM_ice2;
	//	MeshDesc.eMeshType = Prototype_Mesh_SM_ICE_01;

		// Time
		MeshDesc.fMaxTime_Duration = 2.0f;
		MeshDesc.fAppearTime = 0.8f;
		AddDesc.bAfterApperTime = true;

		// Tex
		MeshDesc.iDiffuseTextureIndex = bRandomVal ? 375 : 365;
		MeshDesc.iDiffuseTextureIndex = 274;

		MeshDesc.MaskTextureIndex = NONNMASK;
		MeshDesc.MaskTextureIndex = 31;
		MeshDesc.NoiseTextureIndex = NONNOISE;
		MeshDesc.NoiseTextureIndex = 101;


		// Noise
		MeshDesc.noisingdir = _float2(0, 1).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);

		// Color
		MeshDesc.vLimLight = _float4(0.1f, 0.4f, 0.96f, 0);
		MeshDesc.vEmissive = _float4(1,0.2f,0.0f,0);
		//	MeshDesc.vEmissive = _float4(0.1f, 0.3f, 0.1f, 1.f);
		MeshDesc.fAlphaTestValue = 1.0f;

			// Transform_Base
		MeshDesc.vPosition = _float3(0.0f, 25.0f, 0);
		MeshDesc.vSize = _float3(6.0f);
		MeshDesc.vSize = _float3(10.0f, 6.0f, 10.0f);

		// Move
		MeshDesc.MoveDir = FollowingDir_Look;
		MeshDesc.MoveSpeed = 0;
		AddDesc.AccMoveSpeed = 0;

		// RotS
		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);
		AddDesc.LookRotSpeed = 0;

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 35;
		MeshDesc.RotationSpeedPerSec = 0;
		AddDesc.AccRotSpeed = 0;
		AddDesc.InitRot = _float3(0, 0, 0.0f);

		// Scale
		AddDesc.AccScaleSpeed = 0;
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

	//	Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

	pEaseDesc = NEW MESHAEASING[2];
	pEaseDesc[0] = CreateEasingDesc(TYPE_QuadIn, _float3(0, 1.0f, 0), 0.8f);
	pEaseDesc[1] = CreateEasingDesc(TYPE_Linear, _float3(0, 1.0f, 0), 1.0f);
	
	GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom, pEaseDesc, 2);

	}


	if (type == MESHEFFECT_MONSTER_SPAWN_TL)
	{


		// Plane
		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_ice2;
		//	MeshDesc.eMeshType = Prototype_Mesh_Plat_Wall;
	//	MeshDesc.eMeshType = Prototype_Mesh_ice2;
	//	MeshDesc.eMeshType = Prototype_Mesh_SM_ice2;
	//	MeshDesc.eMeshType = Prototype_Mesh_SM_ICE_01;





		// Time
		MeshDesc.fMaxTime_Duration = 2.0f;
		MeshDesc.fAppearTime = 0.8f;
		AddDesc.bAfterApperTime = true;

		// Tex
		MeshDesc.iDiffuseTextureIndex = bRandomVal ? 375 : 365;
		MeshDesc.iDiffuseTextureIndex = 375;
		MeshDesc.iDiffuseTextureIndex = 276;

		MeshDesc.MaskTextureIndex = NONNMASK;
		MeshDesc.MaskTextureIndex = 31;
		MeshDesc.NoiseTextureIndex = NONNOISE;
		MeshDesc.NoiseTextureIndex = 101;


		// Noise
		MeshDesc.noisingdir = _float2(0, 1).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);

		// Color
		MeshDesc.vLimLight = _float4(0.1f, 0.4f, 0.96f, 0);
		MeshDesc.vEmissive = _float4(1, 0.2f, 0.0f, 0);
		//	MeshDesc.vEmissive = _float4(0.1f, 0.3f, 0.1f, 1.f);
		MeshDesc.fAlphaTestValue = 1.0f;

		// Transform_Base
		MeshDesc.vPosition = _float3(0.0f, 25.0f, 0);
		MeshDesc.vSize = _float3(6.0f);
		MeshDesc.vSize = _float3(10.0f, 6.0f, 10.0f);

		// Move
		MeshDesc.MoveDir = FollowingDir_Look;
		MeshDesc.MoveSpeed = 0;
		AddDesc.AccMoveSpeed = 0;

		// RotS
		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);
		AddDesc.LookRotSpeed = 0;

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 35;
		MeshDesc.RotationSpeedPerSec = 0;
		AddDesc.AccRotSpeed = 0;
		AddDesc.InitRot = _float3(0, 0, 0.0f);

		// Scale
		AddDesc.AccScaleSpeed = 0;
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

	//	Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

		pEaseDesc = NEW MESHAEASING[2];
		pEaseDesc[0] = CreateEasingDesc(TYPE_QuadIn, _float3(0, 1.0f, 0), 0.8f);
		pEaseDesc[1] = CreateEasingDesc(TYPE_Linear, _float3(0, 1.0f, 0), 1.0f);

		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom, pEaseDesc, 2);

	}


	if (type == MESHEFFECT_MONSTER_SPAWN_TP)
	{
		// Plane
		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_ice2;
		//	MeshDesc.eMeshType = Prototype_Mesh_Plat_Wall;
	//	MeshDesc.eMeshType = Prototype_Mesh_ice2;
	//	MeshDesc.eMeshType = Prototype_Mesh_SM_ice2;
	//	MeshDesc.eMeshType = Prototype_Mesh_SM_ICE_01;

		// Time
		MeshDesc.fMaxTime_Duration = 2.0f;
		MeshDesc.fAppearTime = 0.8f;
		AddDesc.bAfterApperTime = true;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 271;

		MeshDesc.MaskTextureIndex = NONNMASK;
		MeshDesc.MaskTextureIndex = 31;
		MeshDesc.NoiseTextureIndex = NONNOISE;
		MeshDesc.NoiseTextureIndex = 101;


		// Noise
		MeshDesc.noisingdir = _float2(0, 1).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);

		// Color
		MeshDesc.vLimLight = _float4(0.1f, 0.4f, 0.96f, 0);
		MeshDesc.vEmissive = _float4(1, 0.2f, 0.0f, 0);
		//	MeshDesc.vEmissive = _float4(0.1f, 0.3f, 0.1f, 1.f);
		MeshDesc.fAlphaTestValue = 1.0f;

		// Transform_Base
		MeshDesc.vPosition = _float3(0.0f, 25.0f, 0);
		MeshDesc.vSize = _float3(6.0f);
		MeshDesc.vSize = _float3(10.0f, 6.0f, 10.0f);

		// Move
		MeshDesc.MoveDir = FollowingDir_Look;
		MeshDesc.MoveSpeed = 0;
		AddDesc.AccMoveSpeed = 0;

		// RotS
		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);
		AddDesc.LookRotSpeed = 0;

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 35;
		MeshDesc.RotationSpeedPerSec = 0;
		AddDesc.AccRotSpeed = 0;
		AddDesc.InitRot = _float3(0, 0, 0.0f);

		// Scale
		AddDesc.AccScaleSpeed = 0;
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

	//	Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

		pEaseDesc = NEW MESHAEASING[2];
		pEaseDesc[0] = CreateEasingDesc(TYPE_QuadIn, _float3(0, 1.0f, 0), 0.8f);
		pEaseDesc[1] = CreateEasingDesc(TYPE_Linear, _float3(0, 1.0f, 0), 1.0f);

		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom, pEaseDesc, 2);

	}


	if (type == MESHEFFECT_MONSTER_SPAWN_TB)
	{


		// Plane
		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_ice2;
		//	MeshDesc.eMeshType = Prototype_Mesh_Plat_Wall;
	//	MeshDesc.eMeshType = Prototype_Mesh_ice2;
	//	MeshDesc.eMeshType = Prototype_Mesh_SM_ice2;
	//	MeshDesc.eMeshType = Prototype_Mesh_SM_ICE_01;





		// Time
		MeshDesc.fMaxTime_Duration = 2.0f;
		MeshDesc.fAppearTime = 0.8f;
		AddDesc.bAfterApperTime = true;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 427;

		MeshDesc.MaskTextureIndex = NONNMASK;
		MeshDesc.MaskTextureIndex = 80;
		MeshDesc.NoiseTextureIndex = 5;
		MeshDesc.NoiseTextureIndex = NONNOISE;


		// Noise
		MeshDesc.noisingdir = _float2(0, 1).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);

		// Color
		MeshDesc.vLimLight = _float4(0.1f, 0.4f, 0.96f, 1);
		MeshDesc.vEmissive = _float4(1, 0.2f, 0.0f, 0);
		//	MeshDesc.vEmissive = _float4(0.1f, 0.3f, 0.1f, 1.f);
		MeshDesc.fAlphaTestValue = 1.0f;

		// Transform_Base
		MeshDesc.vPosition = _float3(0.0f, 25.0f, 0);
		MeshDesc.vSize = _float3(6.0f);
		MeshDesc.vSize = _float3(10.0f, 6.0f, 10.0f);

		// Move
		MeshDesc.MoveDir = FollowingDir_Look;
		MeshDesc.MoveSpeed = 0;
		AddDesc.AccMoveSpeed = 0;

		// RotS
		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);
		AddDesc.LookRotSpeed = 0;

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 35;
		MeshDesc.RotationSpeedPerSec = 0;
		AddDesc.AccRotSpeed = 0;
		AddDesc.InitRot = _float3(0, 0, 0.0f);

		// Scale
		AddDesc.AccScaleSpeed = 0;
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

	//	Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

		pEaseDesc = NEW MESHAEASING[2];
		pEaseDesc[0] = CreateEasingDesc(TYPE_QuadIn, _float3(0, 1.0f, 0), 0.8f);
		pEaseDesc[1] = CreateEasingDesc(TYPE_Linear, _float3(0, 1.0f, 0), 1.0f);

		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom, pEaseDesc, 2);

	}



	if (type == MESHEFFECT_MONSTER_Tezabsura_Cash0)
	{
		// Plane
		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_Plat_Wall;

		// Time
		MeshDesc.fMaxTime_Duration = 2.0f;
		MeshDesc.fAppearTime = 0.8f;
		AddDesc.bAfterApperTime = true;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 92;
		MeshDesc.iDiffuseTextureIndex = 414;

		MeshDesc.MaskTextureIndex = NONNMASK;
		MeshDesc.MaskTextureIndex = 80;
		MeshDesc.NoiseTextureIndex = 221;
		MeshDesc.NoiseTextureIndex = NONNOISE;


		// Noise
		MeshDesc.noisingdir = _float2(0, 1).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);

		// Color
		MeshDesc.vLimLight = _float4(1,1,1,0.1f);
		MeshDesc.vEmissive = _float4(1,0.1f,0.1f,0);
		//	MeshDesc.vEmissive = _float4(0.1f, 0.3f, 0.1f, 1.f);
		MeshDesc.fAlphaTestValue = 0.1f;

		// Transform_Base
		MeshDesc.vPosition = _float3(0.0f, 0.0f, 2.0f);
		MeshDesc.vSize = _float3(0.1001f);

		// Move
		MeshDesc.MoveDir = FollowingDir_Look;
		MeshDesc.MoveSpeed = 0;
		AddDesc.AccMoveSpeed = 0;

		// RotS
		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);
		AddDesc.LookRotSpeed = 0;

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 0;
		AddDesc.AccRotSpeed = 0;
		AddDesc.InitRot = _float3(0, 0, 0.0f);

		// Scale
		AddDesc.AccScaleSpeed = 0;
		AddDesc.ScaleReFlag = false;

		AddDesc.bLockScale[0] = true;
		AddDesc.bLockScale[1] = true;
		AddDesc.bLockScale[2] = true;


		// Fix
		AddDesc.FixFlag_Move = false;
		AddDesc.FixFlag_Rot = true;
		AddDesc.FollowTarget = nullptr;

		// Shader
		MeshDesc.m_iPassIndex = 16; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

	//	Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

		pEaseDesc = NEW MESHAEASING[2];
		pEaseDesc[0] = CreateEasingDesc(TYPE_QuadIn, _float3(0.0f, 0.0f, 4.5f) , 0.3f);
		pEaseDesc[1] = CreateEasingDesc(TYPE_Linear, _float3(0.0f, 0.0f, 4.5f) , 0.2f);

		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom, pEaseDesc, 2);

	}

	if (type == MESHEFFECT_MONSTER_Tezabsura_Cash1)
	{

		// Plane
		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_SM_half_ball_closed_1;
		MeshDesc.eMeshType = Prototype_Mesh_MS_Shot_Cylinder_00;
		

		// Time
		MeshDesc.fMaxTime_Duration = 2.0f;
		MeshDesc.fAppearTime = 0.5f;
		AddDesc.bAfterApperTime = true;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 423;
		MeshDesc.MaskTextureIndex = NONNMASK;
		MeshDesc.MaskTextureIndex = 185;
		MeshDesc.NoiseTextureIndex = NONNOISE;
		MeshDesc.NoiseTextureIndex = 360;


		// Noise
		MeshDesc.noisingdir = _float2(0, 1).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 30.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);

		// Color
		MeshDesc.vLimLight = _float4(0.8f, 1.0f, 1.0f, 0.0f);
		MeshDesc.vEmissive = _float4(0.5f,0.5f,0.1f,1);
		//	MeshDesc.vEmissive = _float4(0.1f, 0.3f, 0.1f, 1.f);

			// Transform_Base
		MeshDesc.vPosition = _float3(0.0f, 1.0f, 0);
		MeshDesc.vSize = _float3(2.0f);
		MeshDesc.vSize = _float3(6.5f);

		MeshDesc.fAlphaTestValue = 0.5f;

		// Move
		MeshDesc.MoveDir = FollowingDir_Look;
		MeshDesc.MoveSpeed = 0;
		AddDesc.AccMoveSpeed = 0;

		// RotS
		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.vAddDirectAngle = _float3(0, 0, 90);
		AddDesc.LookRotSpeed = 0;

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 45;
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
		AddDesc.FollowTarget = nullptr;

		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
		MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

	}

	if (type == MESHEFFECT_MONSTER_Tezabsura_Cash2)
	{

		// Plane
		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_SM_meteo;


		// Time
		MeshDesc.fMaxTime_Duration = 5.0f;
		MeshDesc.fAppearTime = 0.5f;
		AddDesc.bAfterApperTime = false;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 372;
		MeshDesc.MaskTextureIndex = 178;
		MeshDesc.MaskTextureIndex = NONNMASK;
		MeshDesc.NoiseTextureIndex = NONNOISE;
		MeshDesc.NoiseTextureIndex = 385;


		// Noise
		MeshDesc.noisingdir = _float2(0, 1).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 30.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);

		// Color
		MeshDesc.vLimLight = _float4(0.5f, 0.0f, 0.3f, 0.0f);
		MeshDesc.vEmissive = _float4(0.3f, 0.8f, 0.1f, 1);
		//	MeshDesc.vEmissive = _float4(0.1f, 0.3f, 0.1f, 1.f);

			// Transform_Base
		MeshDesc.vPosition = _float3(0.0f, 0.0f, 0);
		MeshDesc.vSize = _float3(6.5f);

		MeshDesc.fAlphaTestValue = 1.0f;

		// Move
		MeshDesc.MoveDir = FollowingDir_Look;
		MeshDesc.MoveSpeed = 0;
		AddDesc.AccMoveSpeed = 0;

		// RotS
		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);
		AddDesc.LookRotSpeed = 0;

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 20;
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
		AddDesc.FollowTarget = nullptr;

		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
		MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

	}

	if (type == MESHEFFECT_MONSTER_Tezabsura_Cash3)
	{

		// Plane
		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_MS_Shot_Cylinder_00;
		MeshDesc.eMeshType = Prototype_Mesh_SM_sphere_melon;


		// Time
		MeshDesc.fMaxTime_Duration = 2.0f;
		MeshDesc.fAppearTime = 0.2f;
		AddDesc.bAfterApperTime = false;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 373;
		MeshDesc.MaskTextureIndex = NONNMASK;
		MeshDesc.MaskTextureIndex = 120;
		MeshDesc.NoiseTextureIndex = NONNOISE;
		MeshDesc.NoiseTextureIndex = 334;


		// Noise
		MeshDesc.noisingdir = _float2(0, 1).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);

		// Color
		MeshDesc.vLimLight = _float4(0.8f, 1.0f, 1.0f, 0.0f);
		MeshDesc.vEmissive = _float4(0.5f, 0.5f, 0.1f, 1);
		//	MeshDesc.vEmissive = _float4(0.1f, 0.3f, 0.1f, 1.f);

			// Transform_Base
		MeshDesc.vPosition = _float3(0.0f, 1.0f, 0);
		MeshDesc.vSize = _float3(5.0f);

		MeshDesc.fAlphaTestValue = 1.0f;

		// Move
		MeshDesc.MoveDir = FollowingDir_Look;
		MeshDesc.MoveSpeed = 0;
		AddDesc.AccMoveSpeed = 0;

		// RotS
		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);
		AddDesc.LookRotSpeed = 0;

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 45;
		AddDesc.AccRotSpeed = 0;
		AddDesc.InitRot = _float3(0, 0, 0.0f);

		// Scale
		AddDesc.AccScaleSpeed = 0.3f;
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

	//	Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

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
		MeshDesc.vSize = _float3(1.0f, 0.4f, 1.0f).XMVector()*1.2f;
		MeshDesc.fAlphaTestValue = 0.2f;


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
		MeshDesc.vSize = _float3(1.2f, 0.4f, 1.2f).XMVector()*1.5f;


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


	if (type == MESHEFFECT_MONSTER_ML_CASH4)
	{
		MeshDesc.eMeshType = Prototype_Mesh_SM_Baren_Weapon;

		// Time
		MeshDesc.fMaxTime_Duration = 1.5f;
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
		MeshDesc.vPosition = _float3(0, 0, 0);
		MeshDesc.vSize = _float3(0.8f);

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
		MeshDesc.MaskTextureIndex = 100;
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
		MeshDesc.fAlphaTestValue = 0.2f;

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
		MeshDesc.MaskTextureIndex = 100;
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
		MeshDesc.MaskTextureIndex = 100;
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
		MeshDesc.eMeshType = Prototype_Mesh_SM_SG_tornado;

		// Time
		MeshDesc.fMaxTime_Duration = 1.3f;
		MeshDesc.fAppearTime = 0.3f;
		AddDesc.bAfterApperTime = false;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 298;
		MeshDesc.MaskTextureIndex = 101;
		MeshDesc.NoiseTextureIndex = 272;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 302;
		MeshDesc.MaskTextureIndex = NONNMASK;
		MeshDesc.NoiseTextureIndex = 211;

		// Noise
		MeshDesc.noisingdir = _float2(0, -1).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		// Color
		MeshDesc.vLimLight = _float4(0.97f, 0.15f, 0.02f, 1.f);
		MeshDesc.vEmissive = _float4(1.f, 0.0f, 0.0f, 1.f);

		// Transform_Base
		MeshDesc.vPosition = _float3(0, 0, 0);
		//	MeshDesc.vPosition = _float3(0.0f, 0, 1.5f);
		MeshDesc.vSize = _float3(3.5f);
		MeshDesc.vSize = _float3(4,3,4);
		MeshDesc.fAlphaTestValue = 0.8f;

		//	MeshDesc.vSize = _float3(5,10,5);


		// Move
		//MeshDesc.MoveDir = FollowingDir_Look;
		//MeshDesc.MoveSpeed = 4.0f;
		//AddDesc.AccMoveSpeed = 1.0f;

		// Rot
		AddDesc.LookRotAxis = FollowingDir_Right;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 40;
		AddDesc.AccRotSpeed = 0;
		AddDesc.InitRot = _float3(0, 0, 0.0f);

		// Scale
		AddDesc.AccScaleSpeed = 0;
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

		//pEaseDesc = NEW MESHAEASING[1];
		//pEaseDesc[0] = CreateEasingDesc(TYPE_Linear, _float3(0, 5, 0), 0.5f);
		//
		//GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom, pEaseDesc, 1);

		MeshDesc.noisingdir = _float2(0, 1).Get_Nomalize();
		AddDesc.DealyTime = 3.f;
		MeshDesc.vPosition = _float3(0, 0, 2.f);
		MeshDesc.vSize = _float3(2, 1, 2);
		MeshDesc.fMaxTime_Duration = 1.3f;
		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);


		
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
		MeshDesc.vLimLight = _float4(0.31f, 0.29f, 0.29f, 0.0f);
		MeshDesc.vEmissive = _float4(1.f, 1.f, 1.f, 1.f);

		// Transform_Base
		MeshDesc.vPosition = _float3(0.0f, 0.0f, 4.00f);
		//	MeshDesc.vPosition = _float3(0.0f, 0, 1.5f);
		MeshDesc.vSize = _float3(3.0f);
		MeshDesc.vSize = _float3(3.0f, 2.0f, 3.0f).XMVector()*0.5f;
		MeshDesc.fAlphaTestValue = 0.9f;


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
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
		MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå


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
		MeshDesc.vLimLight = _float4(0);
		MeshDesc.vEmissive = _float4(1.f, 1.f, 1.f, 1.f);
		MeshDesc.vEmissive = _float4(1.0f,0.5f,0.0f,1.0f);

		// Transform_Base
		MeshDesc.vPosition = _float3(0.0f, 0.0f, 4.00f);
		//	MeshDesc.vPosition = _float3(0.0f, 0, 1.5f);
		MeshDesc.vSize = _float3(3.0f);
		MeshDesc.vSize = _float3(3.0f, 2.0f, 3.0f).XMVector();
		MeshDesc.fAlphaTestValue = 0.8f;

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
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
		MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå


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
		MeshDesc.noisingdir = _float2(-1, 0).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		// Color
		MeshDesc.vLimLight = _float4(1.00f, 0.44f, 0.11f, 1.f);
		MeshDesc.vLimLight = _float4(1, 0, 0, 1.f);
		MeshDesc.vEmissive = _float4(0.1f, 0, 0, 1.f);

		// Transform_Base
		MeshDesc.vPosition = _float3(0.5f, 1, 0);
		//	MeshDesc.vPosition = _float3(0.0f, 0, 1.5f);
		MeshDesc.vSize = _float3(0.8f);

		//	MeshDesc.vSize = _float3(5,10,5);


		// Move
		//MeshDesc.MoveDir = FollowingDir_Up;
		//MeshDesc.MoveSpeed = 10.0f;
		//AddDesc.AccMoveSpeed = 110.0f;

		// Rot
		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.vAddDirectAngle = _float3(0, 0, -45);
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

	if (type == MESHEFFECT_MONSTER_GL_Cash1)
	{
		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_SM_SG_tornado;

		// Time
		MeshDesc.fMaxTime_Duration = 1.0f;
		MeshDesc.fAppearTime = 0.8f;
		AddDesc.bAfterApperTime = false;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 302;
		MeshDesc.MaskTextureIndex = 0;
		MeshDesc.MaskTextureIndex = 80;
		MeshDesc.NoiseTextureIndex = 218;

		// Noise
		MeshDesc.noisingdir = _float2(0, 1).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 100.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);

		// Color
		MeshDesc.vLimLight = _float4(1.0f, 0.00f, 0.00f, 0.0f);
		MeshDesc.vEmissive = _float4(1.0f, 0.0f, 0.1f, 1.f);
	//	MeshDesc.vEmissive = _float4(0);

		// Transform_Base
		MeshDesc.vPosition = _float3(-0.0f, -1, 0.0f);
		MeshDesc.vSize = _float3(3.5f,2.0f, 3.5f);
		MeshDesc.fAlphaTestValue = 0.55f;

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

		AddDesc.bLockScale[0] = false;
		AddDesc.bLockScale[1] = false;
		AddDesc.bLockScale[2] = false;


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

	if (type == MESHEFFECT_MONSTER_GL_Cash2) //meteo
	{
		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_SM_meteo;

		// Time
		MeshDesc.fMaxTime_Duration = 2.f;
		MeshDesc.fAppearTime = 0.5f;
		AddDesc.bAfterApperTime = true;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 302;
		MeshDesc.iDiffuseTextureIndex = 321;
		MeshDesc.MaskTextureIndex = 5;
		MeshDesc.NoiseTextureIndex = 5;

		// Noise
		MeshDesc.noisingdir = _float2(0, 1).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 100.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);

		// Color
		MeshDesc.vLimLight = _float4(1.0f, 0.00f, 0.00f, 1.0f);
		MeshDesc.vEmissive = _float4(0.1f, 0.1f, 0.5f, 1.f);
		//	MeshDesc.vEmissive = _float4(0);


			// Transform_Base
		MeshDesc.vPosition = _float3(0.0f, 20.0f, 0.0f);
		MeshDesc.vSize = _float3(20);


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
		AddDesc.FixFlag_Move = false;
		AddDesc.FixFlag_Rot = false;
		AddDesc.FollowTarget = nullptr;

		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
		MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå


		pEaseDesc = NEW MESHAEASING[2];
		pEaseDesc[0] = CreateEasingDesc(TYPE_CircularIn, _float3(0, 0.0f, 0), 0.8f);
		pEaseDesc[1] = CreateEasingDesc(TYPE_Linear, _float3(0, 0, 0), 0.1f);
		
		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom, pEaseDesc,2);
		


	//	Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);
	}


	if (type == MESHEFFECT_MONSTER_GL_Cash3)
	{
		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_SM_sinkhole;

		// Time
		MeshDesc.fMaxTime_Duration = 1.0f;
		MeshDesc.fAppearTime = 0.1f;
		AddDesc.bAfterApperTime = false;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 0;
		MeshDesc.MaskTextureIndex = NONNMASK;
		MeshDesc.NoiseTextureIndex = 357;

		// Noise
		MeshDesc.noisingdir = _float2(0, 1).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);

		// Color
		MeshDesc.vLimLight = _float4(1.0f, 0.00f, 0.00f, 1.0f);
		MeshDesc.vEmissive = _float4(0.1f, 0.1f, 0.5f, 1.f);
		MeshDesc.vEmissive = _float4(0);


		// Transform_Base
		MeshDesc.vPosition = _float3(-0.0f, -1, 0.0f);
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
		MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå



		MeshDesc.vSize = _float3(17);
		MeshDesc.iDiffuseTextureIndex = 322;
		MeshDesc.iDiffuseTextureIndex = 370;
		MeshDesc.MaskTextureIndex = NONNMASK;
		MeshDesc.NoiseTextureIndex = 198;

		MeshDesc.vLimLight = _float4(0.98f, 0.07f, 0.01f, 0.0f);

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
		MeshDesc.fMaxTime_Duration = 1.0f;
		MeshDesc.fAppearTime = 0.3f;
		AddDesc.bAfterApperTime = true;

		// Tex
		MeshDesc.iDiffuseTextureIndex = bRandomVal ? 424 : 416;
		MeshDesc.iDiffuseTextureIndex = bRandomVal ? 410 : 413;
		MeshDesc.MaskTextureIndex = 107;
		MeshDesc.MaskTextureIndex = NONNMASK;
		MeshDesc.NoiseTextureIndex = 180;
		// Noise
		MeshDesc.noisingdir = _float2(0, -1).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		// Color
		MeshDesc.vLimLight = _float4(0.35f, 0.16f, 0.0f, 0.0f);
		MeshDesc.vEmissive = _float4(1.0f, 1.0f, 1.0f, 1.f);
		MeshDesc.vEmissive = _float4(1.0f, 0.0f, 0.0f, 1.f);
	//	MeshDesc.vEmissive = _float4(0.5f, 0.5f, 0.1f, 1.f);

		// Transform_Base
		MeshDesc.vPosition = _float3(0, 0.5f, 0);
		//	MeshDesc.vPosition = _float3(0.0f, 0, 1.5f);
		_float rand = GetSingle(CUtilityMgr)->RandomFloat(1.0f, 2.0f);
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
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
		MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

	}

	if (type == MESHEFFECT_MONSTER_VM_Cash2)
	{
	

		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_SM_half_ball_closed_1;
		MeshDesc.eMeshType = Prototype_Mesh_SM_sphere_melon;
		MeshDesc.eMeshType = Prototype_Mesh_mySphere;

		
		// Time
		MeshDesc.fMaxTime_Duration = 0.5f;
		// 2.0f;
		MeshDesc.fAppearTime = 0.2f;
		AddDesc.bAfterApperTime = true;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 320;
		MeshDesc.iDiffuseTextureIndex = 321;
		MeshDesc.MaskTextureIndex = NONNMASK;
		MeshDesc.MaskTextureIndex = 69;
		MeshDesc.MaskTextureIndex = 77;
		MeshDesc.MaskTextureIndex = 107;
		MeshDesc.NoiseTextureIndex = 248;
		MeshDesc.NoiseTextureIndex = 221;

		// Noise
		MeshDesc.noisingdir = _float2(0, 1).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		// Color
		MeshDesc.vLimLight = _float4(1.00f, 0.0f, 0.0f, 1.0f);
		MeshDesc.vEmissive = _float4(1.0f, 0.0f, 0.0f, 1.f);

			// Transform_Base
		MeshDesc.vPosition = _float3(0, 0.0f, -1);
		MeshDesc.vSize = _float3(1, 1.0f, 1).XMVector() * 30;
		MeshDesc.vSize = _float3(3.f, 3.0f, 1.f).XMVector() * 10;
		MeshDesc.fAlphaTestValue = 1.0f;


		// Move
		MeshDesc.MoveDir = FollowingDir_Right;
		MeshDesc.MoveSpeed = 0;
		AddDesc.AccMoveSpeed = 0;

		// Rot
		AddDesc.LookRotAxis = FollowingDir_Up;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);

		MeshDesc.RotAxis = FollowingDir_Up;
		MeshDesc.RotationSpeedPerSec = 35;
		MeshDesc.RotationSpeedPerSec = 0;
		AddDesc.AccRotSpeed = 0;
		AddDesc.InitRot = _float3(0, 0, 0.0f);

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

#pragma endregion MONSTER_VM

#pragma region MONSTER_VL

	// ¹° ÆøÅº

	if (type == MESHEFFECT_MONSTER_VL_Cash0)
	{
		// Mesh
	//	MeshDesc.eMeshType = Prototype_Mesh_SM_sphere_melon;
		MeshDesc.eMeshType = Prototype_Mesh_SM_Sphere_RPG;

		// Time
		MeshDesc.fMaxTime_Duration = 15.f;
		MeshDesc.fAppearTime = 0.5f;
		AddDesc.bAfterApperTime = false;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 278;
		MeshDesc.MaskTextureIndex = 208;
		MeshDesc.NoiseTextureIndex = 289;
		// Noise
		MeshDesc.noisingdir = _float2(0, 1).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		// Color
		MeshDesc.vLimLight = _float4(0.96f, 0.45f, 0.09f, 1.0f);
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
	//	AddDesc.AccScaleSpeed = 2.0f;
		AddDesc.ScaleMax = 0.6f;
	//	AddDesc.ScaleMax = 10.0f;

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
		MeshDesc.iDiffuseTextureIndex = 278;
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
		MeshDesc.vSize = _float3(3.0f, 6.f, 3.0f);



		// Move
		MeshDesc.MoveDir = FollowingDir_Look;
		MeshDesc.MoveSpeed = 0;
		AddDesc.AccMoveSpeed = 0;

		// Rot
		AddDesc.LookRotAxis = FollowingDir_Right;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 35;
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
		MeshDesc.fMaxTime_Duration = 2.0f;
		MeshDesc.fAppearTime = 0.4f;
		AddDesc.bAfterApperTime = false;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 278;
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
		MeshDesc.vSize = _float3(20.0f);



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


	if (type == MESHEFFECT_MONSTER_VL_Cash3)
	{
		MeshDesc.eMeshType = Prototype_Mesh_SM_Sphere_RPG;

		// Time
		MeshDesc.fMaxTime_Duration = 2.0f;
		MeshDesc.fAppearTime = 0.0f;
		AddDesc.bAfterApperTime = false;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 278;
		MeshDesc.MaskTextureIndex = 208;
		MeshDesc.NoiseTextureIndex = 289;
		// Noise
		MeshDesc.noisingdir = _float2(0, 1).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		// Color
		MeshDesc.vLimLight = _float4(0.96f, 0.45f, 0.09f, 1.0f);
		MeshDesc.vEmissive = _float4(0.1f, 0.3f, 0.1f, 1.f);

		// Transform_Base
		MeshDesc.vPosition = _float3(0, 0.0f, 0);
		//	MeshDesc.vPosition = _float3(0.0f, 0, 1.5f);
		MeshDesc.vSize = _float3(2.0f);



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
		AddDesc.AccScaleSpeed = 3.5f;
		//	AddDesc.AccScaleSpeed = 2.0f;
		AddDesc.ScaleMax = 0.0f;
		//	AddDesc.ScaleMax = 10.0f;

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

		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);
	}


	
	if (type == MESHEFFECT_MONSTER_VL_Cash4)
	{

	}
	if (type == MESHEFFECT_MONSTER_VL_Cash5)
	{

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
		MeshDesc.iDiffuseTextureIndex = 285;
		MeshDesc.MaskTextureIndex = 106;
		MeshDesc.MaskTextureIndex = 71;
		MeshDesc.NoiseTextureIndex = 188;


		// Noise
		MeshDesc.noisingdir = _float2(1, 0).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);

		// Color
		MeshDesc.vLimLight = _float4(0.35f, 0.16f, 0.0f, 0.0f);
		MeshDesc.vLimLight = _float4(0.01f, 0.05f, 1.0f, 1.0f);
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
		MeshDesc.vLimLight = _float4(0.0f, 0.8f, 0.12f, 1.0f);
		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);
	}

	if (type == MESHEFFECT_MONSTER_NL_Cash1)
	{
		// Plane
		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_Tornado2;

		// Time
		MeshDesc.fMaxTime_Duration = 0.8f;
		MeshDesc.fAppearTime = 0.4f;
		AddDesc.bAfterApperTime = true;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 278;
		MeshDesc.MaskTextureIndex = NONNMASK;
		MeshDesc.MaskTextureIndex = 84;
		MeshDesc.NoiseTextureIndex = 54;


		// Noise
		MeshDesc.noisingdir = _float2(0, -1).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 50.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		
		// Color
		MeshDesc.vLimLight = _float4(0.15f, 0.38f, 1, 0.0f);
		MeshDesc.vEmissive = _float4(1);
		//	MeshDesc.vEmissive = _float4(0.1f, 0.3f, 0.1f, 1.f);

			// Transform_Base
		MeshDesc.vPosition = _float3(0.0f, -1.0f, 0);
		MeshDesc.vSize = _float3(0.5f, 1.0f, 0.5f);

		// Move
		MeshDesc.MoveDir = FollowingDir_Look;
		MeshDesc.MoveSpeed = 0;
		AddDesc.AccMoveSpeed = 0;

		// RotS
		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);
		AddDesc.LookRotSpeed = 0;

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 0.0f;
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
		MeshDesc.fAlphaTestValue = 0.6f;
		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
		MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);
	}

	if (type == MESHEFFECT_MONSTER_NL_Cash2)
	{

		MeshDesc.eMeshType = Prototype_Mesh_SM_Ninjasura_Knife;

		// Time
		MeshDesc.fMaxTime_Duration = 10;
		MeshDesc.fAppearTime = 0.3f;
		AddDesc.bAfterApperTime = false;

		// Tex
		MeshDesc.iDiffuseTextureIndex = 271;
		MeshDesc.MaskTextureIndex = 82;
		MeshDesc.NoiseTextureIndex = 221;


		// Noise
		MeshDesc.noisingdir = _float2(1, 0).Get_Nomalize();
		MeshDesc.fDistortionNoisingPushPower = 10.0f;
		MeshDesc.vColor = _float4(1, 1, 1, 1);


		// Color
		MeshDesc.vLimLight = _float4(0.15f, 0.38f, 1, 1.0f);
		MeshDesc.vEmissive = _float4(0.3f, 0.1f, 0.1f, 1.f);
		// MeshDesc.vEmissive = _float4(1);
		MeshDesc.vEmissive = _float4(0.1f, 0.3f,0.1f, 1.f);

		// Transform_Base
		MeshDesc.vPosition = _float3(0,0,-0.5f);
		MeshDesc.vSize = _float3(3);

		// Move
		//MeshDesc.MoveDir = FollowingDir_Right;
		//MeshDesc.MoveSpeed = 0;
		//AddDesc.AccMoveSpeed = 0;

		// RotS
		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.vAddDirectAngle = _float3(0, 0, 0);
		AddDesc.LookRotSpeed = 0;

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 0;
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
		AddDesc.FollowTarget = nullptr;

	//	MeshDesc.fAlphaTestValue = 0.9f;
		// Shader
	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
		MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå


			
		//pEaseDesc = NEW MESHAEASING[2];
		//pEaseDesc[0] = CreateEasingDesc(TYPE_SinInOut, _float3(0, -5.0f, 0), 0.5f);
		//pEaseDesc[1] = CreateEasingDesc(TYPE_Linear, _float3(0, 0, 0), 1.0f);
		//GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);
		//GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom, pEaseDesc,1);


		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);


		


	}

	if (type == MESHEFFECT_MONSTER_NL_Cash3)
	{
		// 2
		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_SM_Ninjasura_Knife;

		// Time
		MeshDesc.fMaxTime_Duration = 1.0f;
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
		MeshDesc.vPosition = _float3(0,3,-3);
		MeshDesc.vSize = _float3(2.0f);

		// Move
		//MeshDesc.MoveDir = FollowingDir_Look;
		//MeshDesc.MoveSpeed = 0;
		//AddDesc.AccMoveSpeed = 0;

		// RotS
		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.vAddDirectAngle = _float3(20, 0, 0);
		AddDesc.LookRotSpeed = 0;

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 0;
		AddDesc.AccRotSpeed = 0;
		AddDesc.InitRot = _float3(0, 0, 0.0f);

		// Scale
		AddDesc.AccScaleSpeed = 2.0f;
		AddDesc.ScaleMax = 3.5f;
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
		MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

	//	Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

		pEaseDesc = NEW MESHAEASING[2];
		pEaseDesc[0] = CreateEasingDesc(TYPE_QuadOut, _float3(0, 1.f, 1.f), 0.5f);
		pEaseDesc[1] = CreateEasingDesc(TYPE_Linear, _float3(0, -2.0f, 4.f), 0.2f);

		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom, pEaseDesc, 2);


	}
	if (type == MESHEFFECT_MONSTER_NL_Cash4)
	{
		// 2
		// Mesh
		MeshDesc.eMeshType = Prototype_Mesh_SM_Ninjasura_Knife;

		// Time
		MeshDesc.fMaxTime_Duration = 1.0f;
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
		MeshDesc.vPosition = _float3(0, 3, -3);
		MeshDesc.vSize = _float3(2.0f);

		// Move
		//MeshDesc.MoveDir = FollowingDir_Look;
		//MeshDesc.MoveSpeed = 0;
		//AddDesc.AccMoveSpeed = 0;

		// RotS
		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.vAddDirectAngle = _float3(20, 0, 0);
		AddDesc.LookRotSpeed = 0;

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 0;
		AddDesc.AccRotSpeed = 0;
		AddDesc.InitRot = _float3(0, 0, 0.0f);

		// Scale
		AddDesc.AccScaleSpeed = 2.0f;
		AddDesc.ScaleMax = 3.5f;
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
		MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

	//	pEaseDesc = NEW MESHAEASING[2];
	//	pEaseDesc[0] = CreateEasingDesc(TYPE_QuadOut, _float3(0, 1.f, 1.f), 0.5f);
	//	pEaseDesc[1] = CreateEasingDesc(TYPE_Linear, _float3(0, -2.0f, 4.f), 0.2f);
	//
	//	Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom, pEaseDesc, 2);


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


HRESULT CPartilceCreateMgr::Ready_MapParticle_Stage(SCENEID id)
{
	if (SCENEID::SCENE_END <= id)
		return E_FAIL;

	mCurrentScene = id;
	mbInitScene = false;
	//GETINSTANCE->Play3D_Sound(
//	TEXT("UM_Spawn_04.ogg"),
//	Fixpos,
//	CHANNELID::CHANNEL_MAPOBJECT, 1.0f);

// GETINSTANCE->Stop_ChannelSound(CHANNEL_MAPOBJECT);

	// °¢ ¸ÊÀÇ Tex / Mesh Instance Effect

	for (auto val : mListTexParticleMap[mCurrentScene])
	{
		GetSingle(CUtilityMgr)->Create_TextureInstance(mCurrentScene, val);
	}

	for (auto val : mListMeshParticleMap[mCurrentScene])
	{
		GetSingle(CUtilityMgr)->Create_MeshInstance(mCurrentScene, val);
	}

	// Soound
	for (auto& val : mListSound3DMap[mCurrentScene])
	{
		val.mIsSoundPlay = false;
	}

	// Map MeshhObject
	Create_MapMeshParticle(mCurrentScene);


	return S_OK;
}

//_int CPartilceCreateMgr::Update_MapEffect(_double timer)
//{
//	if (
//		mCurrentScene != SCENE_STAGE1 &&
//		mCurrentScene != SCENE_STAGE2 &&
//		mCurrentScene != SCENE_STAGE3 &&
//		mCurrentScene != SCENE_STAGE4 &&
//		mCurrentScene != SCENE_STAGE5 &&
//		mCurrentScene != SCENE_STAGE6 &&
//		mCurrentScene != SCENE_STAGE7
//		)
//		return _int();
//
//	if(mbInitScene == false)
//	{
//		mbInitScene = true;
//		return _int();
//	}
//
//	// Map Effect Create / Sound 
//	mMapUpdateLate_Timer -= timer;
//	_bool bParticleUpate = false;
//
//	if (mMapUpdateLate_Timer<=0)
//	{
//		mMapUpdateLate_Timer = mMapUpdateLate;
//		bParticleUpate = true;
//	}
//
//	if (bParticleUpate == false)
//		return 0;
//
//
//	CPlayer* player = ((CPlayer*)(g_pGameInstance->Get_GameObject_By_LayerIndex(mCurrentScene, TAG_LAY(Layer_Player))));
//
//	if (player)
//	{
//
//		for (MAPEFFECT& mesheffect : mVecCurrentObjects)
//		{
//			_float PlyDistance = player->Get_Transform()->Get_MatrixState_Float3(CTransform::STATE_POS).Get_Distance(mesheffect.mFixpos.XMVector());
//
//			if (mesheffect.mIsCreate)
//			{
//				if (mesheffect.mRange+10 < PlyDistance)
//				{
//					// Map Particle OFF 
//					if (mesheffect.mObject)
//					{
//						CGameObject* obj = ((CGameObject*)mesheffect.mObject);
//						if (obj)
//							obj->Set_IsDead();
//						Safe_Release(mesheffect.mObject);
//					//	mesheffect.mObject = nullptr;
//					}
//					mesheffect.mIsCreate = false;
//				}
//
//			}
//
//			else
//			{
//				if (mesheffect.mRange > PlyDistance)
//				{
//					// Map Particle ON 
//					mesheffect.mIsCreate = true;
//
//					if (mesheffect.mParticleType == PARTICLEDESC_TYPE_TEX)
//					{
//						GETUTIL->Create_TextureInstance(mCurrentScene, mesheffect.mParticleDesc);
//						mTestObj = GETINSTANCE->Get_GameObject_By_LayerIndex(mCurrentScene, TAG_LAY(Layer_Particle));
//						mesheffect.mObject = mTestObj;
//						Safe_AddRef(mesheffect.mObject);
//
//						// PlaySound
//						if (mesheffect.mSoundFileName.empty() == false)
//						{
//							GETINSTANCE->Play3D_Sound(
//							(TCHAR*)mesheffect.mSoundFileName.c_str(), // TEXT("UM_Spawn_04.ogg"),
//								mesheffect.mFixpos,
//							CHANNELID::CHANNEL_MAPOBJECT, 1.0f);
//						}
//					}
//
//					//if (mesheffect.mParticleType == PARTICLEDESC_TYPE_MESH)
//					//{
//					//	GETUTIL->Create_MeshInstance(mCurrentScene, mesheffect.mMeshParticleDesc);
//					//	//mesheffect.mObject = (GETINSTANCE->Get_GameObject_By_LayerIndex(mCurrentScene, TAG_LAY(Layer_Particle)));
//
//					//	if (mesheffect.mSoundFileName.empty() == false)
//					//	{
//					//		GETINSTANCE->Play3D_Sound(
//					//			(TCHAR*)mesheffect.mSoundFileName.c_str(), // TEXT("UM_Spawn_04.ogg"),
//					//			mesheffect.mFixpos,
//					//			CHANNELID::CHANNEL_MAPOBJECT, 1.0f);
//					//	}
//
//
//					//}
//				}
//			}
//
//			
//		}
//	}
//	return 0;
//}


_int CPartilceCreateMgr::Update_MapEffect_Sound(_double timer)
{
	if (
		mCurrentScene != SCENE_STAGE1 &&
		mCurrentScene != SCENE_STAGE2 &&
		mCurrentScene != SCENE_STAGE3 &&
		mCurrentScene != SCENE_STAGE4 &&
		mCurrentScene != SCENE_STAGE5 &&
		mCurrentScene != SCENE_STAGE6 &&
		mCurrentScene != SCENE_STAGE7
		)
		return _int();

	if (mbInitScene == false)
	{
		mbInitScene = true;
		return _int();
	}

	// Map Effect Create / Sound 
	mMapUpdateLate_Timer -= timer;
	_bool bParticleUpate = false;

	if (mMapUpdateLate_Timer <= 0)
	{
		mMapUpdateLate_Timer = mMapUpdateLate;
		bParticleUpate = true;
	}

	if (bParticleUpate == false)
		return 0;


	CPlayer* player = ((CPlayer*)(g_pGameInstance->Get_GameObject_By_LayerIndex(mCurrentScene, TAG_LAY(Layer_Player))));

	if (player)
	{
		for (MAPSOUND& mesheffect : mListSound3DMap[mCurrentScene])
		{
			_float PlyDistance = player->Get_Transform()->Get_MatrixState_Float3(CTransform::STATE_POS).Get_Distance(mesheffect.mFixpos.XMVector());

			if (mesheffect.mIsSoundPlay)
			{
				if (mesheffect.mRangeOff < PlyDistance)
				{
					// Sound Off
					g_pGameInstance->Stop_ChannelSound(CHANNELID::CHANNEL_MAPOBJECT);

					for (auto& val : mListSound3DMap[mCurrentScene])
					{
						val.mIsSoundPlay = false;
					}				
				
				}
			}

			else
			{
				if (mesheffect.mRange > PlyDistance)
				{
					// Sound On
					mesheffect.mIsSoundPlay = true;

					// PlaySound
					PlaySound_Map(mesheffect.mFixpos, mesheffect.mSoundType, mesheffect.mVol);
				}
			}
		}
	}
	return 0;
}

HRESULT CPartilceCreateMgr::AddSoundDesc(SCENEID id, _float3 worldpos, E_PARTICLSOUND_TYPE mSoundType, _float vol, _float Range, _float RangeMax)
{
	MAPSOUND sound;
	sound.mFixpos = worldpos;
	sound.mSoundType = mSoundType;
	sound.mRange = Range;
	sound.mRangeOff = RangeMax;
	sound.mVol = vol;
	mListSound3DMap[id].push_back(sound);
	return S_OK;
}

void CPartilceCreateMgr::PlaySound_Map(_float3  pos, E_PARTICLSOUND_TYPE type, _float vol)
{
	wstring wstr;
	_int random = 0;
	// GETUTIL->RandomFloat(0, 3);
//	wstr = TEXT("UM_FireLoop.ogg");

	switch (type)
	{

	case Client::UM_Env_Fire:
		wstr = TEXT("UM_Env_Fire.ogg");
		break;
	case Client::UM_Env_Fire_bonefire:
		wstr = TEXT("UM_Env_Fire_bonefire.ogg");
		break;
	case Client::UM_Env_Water_Cave:
		wstr = TEXT("UM_Env_Water_Cave.ogg");
		break;
	case Client::UM_Env_Water_Wet:
		wstr = TEXT("UM_Env_Water_Wet.ogg");

	case Client::UM_Env_Water_Foutain1:
		wstr = TEXT("UM_Env_Water_Foutain1.ogg");
	case Client::UM_Env_Water_Foutain2:
		wstr = TEXT("UM_Env_Water_Foutain2.ogg");
		break;

	default:
		return;

	}
	if (wstr.empty())
		return;

	g_pGameInstance->Play3D_Sound(
		(TCHAR*)wstr.c_str(),
		pos,
		CHANNELID::CHANNEL_MAPOBJECT, vol);
}

void CPartilceCreateMgr::Create_MapMeshParticle(SCENEID id)
{
	_float3 FixPos = _float3(-999, -999, -999);
	NONINSTNESHEFTDESC desc1;
	MESHADDDATA desc2;
	MESHLIGHTDATA desc3;

	_uint count = 0;

	switch (id)
	{

	case SCENE_STAGE1:
	{
		list<_float3> listFloat3_NomalLight;


		listFloat3_NomalLight.push_back(_float3(58.639412f, 34.096962f, 47.311424f));
		listFloat3_NomalLight.push_back(_float3(58.588211f, 34.096603f, 52.187607f));
	//	listFloat3_NomalLight.push_back(_float3(78.068604f, 33.403080f, 50.104527f));

		listFloat3_NomalLight.push_back(_float3(63.581612f, 32.160011f, 43.371586f));
	//	listFloat3_NomalLight.push_back(_float3(76.306778f, 32.160011f, 57.359707f));
		listFloat3_NomalLight.push_back(_float3(63.697540f, 32.160007f, 57.658871f));
		listFloat3_NomalLight.push_back(_float3(128.296295f, 33.799999f, 45.668304f));
		listFloat3_NomalLight.push_back(_float3(136.963242f, 33.799999f, 45.711121f));
		listFloat3_NomalLight.push_back(_float3(146.675217f, 33.799999f, 46.061680f));
		listFloat3_NomalLight.push_back(_float3(152.966187f, 33.799999f, 66.874329f));

	//	list<_float3> listFloat3_DownLight;


		// ´Ù¸¥ ´À³¦À¸·Î ±³Ã¼
		listFloat3_NomalLight.push_back(_float3(238.355499f, 21.240009f, 176.354553f));
		listFloat3_NomalLight.push_back(_float3(248.099197f, 21.240009f, 176.378082f));
		listFloat3_NomalLight.push_back(_float3(253.676987f, 18.900009f, 173.714569f));
		listFloat3_NomalLight.push_back(_float3(260.460571f, 18.900009f, 173.714569f));
		listFloat3_NomalLight.push_back(_float3(275.256653f, 22.850010f, 172.504914f));
		listFloat3_NomalLight.push_back(_float3(288.565094f, 22.850010f, 172.504914f));
		listFloat3_NomalLight.push_back(_float3(299.868561f, 22.850010f, 172.504914f));
		listFloat3_NomalLight.push_back(_float3(308.263000f, 22.850010f, 172.504914f));
		listFloat3_NomalLight.push_back(_float3(310.254395f, 44.150013f, 263.728302f));
		listFloat3_NomalLight.push_back(_float3(320.024963f, 44.150013f, 263.973053f));
		listFloat3_NomalLight.push_back(_float3(299.426697f, 38.750011f, 213.826035f));
		listFloat3_NomalLight.push_back(_float3(303.848389f, 38.750011f, 217.508209f));
		listFloat3_NomalLight.push_back(_float3(338.354218f, 43.600010f, 262.804962f));
		listFloat3_NomalLight.push_back(_float3(338.336151f, 43.600010f, 268.335358f));


		//for (_float3 pos : listFloat3_NomalLight)
		//{
		//	FixPos = pos;

		//	GETPARTICLE->Get_MeshEffectDesc_Map_FreeSet(CPartilceCreateMgr::MESHEFFECT_MAP_OBJ_Lotas, desc1, desc2, desc3);
		//	desc1.fMaxTime_Duration = 9999.0f;
		//	desc2.DealyTime = GETUTIL->RandomFloat(0.1f, 2);

		//	GETPARTICLE->Create_MeshEffectDesc_MAP(id, desc1, desc2, FixPos, desc3);

		//	GETPARTICLE->Get_MeshEffectDesc_Map_FreeSet(CPartilceCreateMgr::MESHEFFECT_MAP_OBJ_BoxBar, desc1, desc2, desc3);
		//	//	desc1.iDiffuseTextureIndex = 0;
		//	desc1.fMaxTime_Duration = 9999.0f;
		//	GETPARTICLE->Create_MeshEffectDesc_MAP(id, desc1, desc2, FixPos, desc3);
		//}

		for (_float3 pos : listFloat3_NomalLight)
		{
			FixPos = pos;

			GETPARTICLE->Get_MeshEffectDesc_Map_FreeSet(CPartilceCreateMgr::MESHEFFECT_MAP_OBJ_Lotas, desc1, desc2, desc3);
			desc1.fMaxTime_Duration = 9999.0f;
			desc1.vPosition = _float3(-0.0f, -1.1f, 0.0f);
			desc2.DealyTime = GETUTIL->RandomFloat(0.1f, 2);
			desc2.AccRotSpeed = 0.0001f;
			GETPARTICLE->Create_MeshEffectDesc_MAP(id, desc1, desc2, FixPos, desc3);
		}

		{
			FixPos = _float3(78.068604f, 33.403080f, 50.104527f); // Çô

			GETPARTICLE->Get_MeshEffectDesc_Map_FreeSet(CPartilceCreateMgr::MESHEFFECT_MAP_OBJ_Lotas, desc1, desc2, desc3);
			desc1.fMaxTime_Duration = 9999.0f;
			desc1.vPosition = _float3(-0.0f, -1.1f, 0.0f);
			desc1.iDiffuseTextureIndex = 276;
			desc2.DealyTime = GETUTIL->RandomFloat(0.1f, 2);
			desc2.AccRotSpeed = 0.0001f;

			GETPARTICLE->Create_MeshEffectDesc_MAP(id, desc1, desc2, FixPos, desc3);
		}


		listFloat3_NomalLight.clear();
		
		

	}
	break;
	case SCENE_STAGE2:
	{
		list<_float3> listFloat3;

		listFloat3.push_back(_float3(474.500646f, 7.311449f, 91.291466f));
		listFloat3.push_back(_float3(474.500409f, 7.261961f, 85.622421f));
		listFloat3.push_back(_float3(458.395782f, 13.650010f, 119.169800f));

		//	Briage
			listFloat3.push_back(_float3(460.2f, 13.8f, 149.0f));
		//	listFloat3.push_back(_float3(444.670258f, 10.656261f, 159.772964f));
		//	listFloat3.push_back(_float3(440.548309f, 4.600010f, 198.594711f));
		//	listFloat3.push_back(_float3(430.246429f, 4.600010f, 211.354660f));
		//	listFloat3.push_back(_float3(415.853699f, 4.600010f, 222.872742f));
		listFloat3.push_back(_float3(399.194153f, 4.400010f, 223.655167f));

		listFloat3.push_back(_float3(407.185669f, 4.400010f, 233.373367f));
		listFloat3.push_back(_float3(404.093170f, 6.537241f, 259.133728f));
		listFloat3.push_back(_float3(399.451324f, 6.496566f, 258.920837f));
		listFloat3.push_back(_float3(386.009735f, 11.800004f, 305.576477f));
		listFloat3.push_back(_float3(377.783661f, 11.800006f, 300.690399f));
		listFloat3.push_back(_float3(374.133606f, 11.612940f, 320.554993f));

		for (_float3 pos : listFloat3)
		{
			FixPos = pos;

			GETPARTICLE->Get_MeshEffectDesc_Map_FreeSet(CPartilceCreateMgr::MESHEFFECT_MAP_OBJ_Lotas, desc1, desc2, desc3);
			desc1.fMaxTime_Duration = 9999.0f;
			desc1.vPosition = _float3(-0.0f, -1.1f, 0.0f);
			desc2.DealyTime = GETUTIL->RandomFloat(0.1f, 2);
			desc2.AccRotSpeed = 0.0001f;
			desc1.iDiffuseTextureIndex = 275;
			GETPARTICLE->Create_MeshEffectDesc_MAP(id, desc1, desc2, FixPos, desc3);
		}

		listFloat3.clear();

		// dark	
		listFloat3.push_back(_float3(383.272278f, 16.500010f, 354.304535f));
		listFloat3.push_back(_float3(386.138580f, 16.500010f, 350.624939f));

		for (_float3 pos : listFloat3)
		{
			FixPos = pos;

			GETPARTICLE->Get_MeshEffectDesc_Map_FreeSet(CPartilceCreateMgr::MESHEFFECT_MAP_OBJ_Lotas, desc1, desc2, desc3);
			desc1.fMaxTime_Duration = 9999.0f;
			desc1.vPosition = _float3(-0.0f, -1.1f, 0.0f);
			desc2.DealyTime = GETUTIL->RandomFloat(0.1f, 2);
			desc2.AccRotSpeed = 0.0001f;
			desc1.iDiffuseTextureIndex = 275;
			desc1.vSize = desc1.vSize.XMVector()*1.2f;
			GETPARTICLE->Create_MeshEffectDesc_MAP(id, desc1, desc2, FixPos, desc3);

			{
				auto texDesc = GETPARTICLE->Get_EffectSetting_Tex(CPartilceCreateMgr::Um_FireMask_2_png,
					9999.0f,
					0.8f,
					_float4(0.75f, 0.26f, 0.05f, 1),
					_float4(0.95f, 0.13f, 0.2f, 0.5f),
					1,
					_float3(1.0f),
					_float3(0.1f),
					1);

				texDesc.vEmissive_SBB = _float3(1, 0.5f, 0.5f);
				texDesc.Particle_Power = 4.0f;

				GetMeshEffect_Fix()->Set_Particle(texDesc);

			}

		}
		listFloat3.clear();



		// Fire Mesh														
		listFloat3.push_back(_float3(594.999634f, 21.800009f, 408.885498f));
		listFloat3.push_back(_float3(590.230042f, 21.800009f, 416.497437f));
		listFloat3.push_back(_float3(584.636230f, 21.800009f, 413.633392f));
		listFloat3.push_back(_float3(589.038147f, 21.800009f, 406.860748f));

		for (_float3 pos : listFloat3)
		{
			FixPos = pos;

			GETPARTICLE->Get_MeshEffectDesc_Map_FreeSet(CPartilceCreateMgr::MESHEFFECT_MAP_OBJ_Lotas, desc1, desc2, desc3);
			desc1.fMaxTime_Duration = 9999.0f;
			desc1.vPosition = _float3(-0.0f, -1.1f, 0.0f);
			desc2.DealyTime = GETUTIL->RandomFloat(0.1f, 2);
			desc2.AccRotSpeed = 0.0001f;
			desc1.iDiffuseTextureIndex = 275;
			GETPARTICLE->Create_MeshEffectDesc_MAP(id, desc1, desc2, FixPos, desc3);

			{
				auto texDesc = GETPARTICLE->Get_EffectSetting_Tex(CPartilceCreateMgr::Um_FireMask_2_png,
					9999.0f,
					0.8f,
					_float4(0.75f, 0.26f, 0.05f, 1),
					_float4(0.95f, 0.13f, 0.2f, 0.5f),
					0,
					_float3(1.0f),
					_float3(0.1f),
					1);

				texDesc.vEmissive_SBB = _float3(1, 0.5f, 0.5f);
				texDesc.Particle_Power = 4.0f;

				GetMeshEffect_Fix()->Set_Particle(texDesc);

			}

		}

		listFloat3.clear();

		{
			FixPos = _float3(483.217959f, 7.100010f, 116.793f); // ¿©½Å»ó

			GETPARTICLE->Get_MeshEffectDesc_Map_FreeSet(CPartilceCreateMgr::MESHEFFECT_MAP_OBJ_Freeset0, desc1, desc2, desc3);
			desc1.fMaxTime_Duration = 9999.0f;
			desc1.vPosition = _float3(-0.0f, 1.0f, 0.0f);

			GETPARTICLE->Create_MeshEffectDesc_MAP(id, desc1, desc2, FixPos, desc3);
		}		


	}
		break;
	case SCENE_STAGE3:

	{
		list<_float3> listFloat3;

		for (_uint i = 104; i < 210; i+=7)
		{
			_float zpos = _float(i);
			listFloat3.push_front(_float3(207.612289f, 96.400009f,zpos));
			listFloat3.push_front(_float3(96.927811f, 96.400009f, zpos) );
		}

		_uint cc = 0;
		_bool bRed = true;

		for (_float3 pos : listFloat3)
		{

			FixPos = pos;
			FixPos.y -= 6.5f;
			GETPARTICLE->Get_MeshEffectDesc_Map_FreeSet(CPartilceCreateMgr::MESHEFFECT_MAP_OBJ_Lotas, desc1, desc2, desc3);
			desc1.fMaxTime_Duration = 9999.0f;
			desc1.vPosition = _float3(-0.0f, 0, 0.0f);
			desc1.vSize = _float3(3.0f);
			desc2.DealyTime = GETUTIL->RandomFloat(0.1f, 2);
			desc2.AccRotSpeed = 0.0001f;

			if (cc >= 2)
			{
				bRed = !bRed;
				cc = 0;
			}
			cc++;

			if(bRed)
				desc1.iDiffuseTextureIndex = 401;
			else
				desc1.iDiffuseTextureIndex = 278;


			GETPARTICLE->Create_MeshEffectDesc_MAP(id, desc1, desc2, FixPos, desc3);

		}

		listFloat3.clear();
	}



		break;
	case SCENE_STAGE4:
		break;
	case SCENE_STAGE5:
		break;
	case SCENE_STAGE6: // STAGE 6 

	{
		list<_float3> listFloat3;


		//fire
		listFloat3.push_back(_float3(7.957f,53.3f, 9.123f));
		listFloat3.push_back(_float3(11.5f, 53.3f, 9.123f));
		listFloat3.push_back(_float3(22.5f, 53.3f, 9.0f));
		listFloat3.push_back(_float3(25.2f, 53.3f, 9.0f));

		
		for (_float3 pos : listFloat3)
		{
			count++;

			FixPos = pos;

			GETPARTICLE->Get_MeshEffectDesc_Map_FreeSet(CPartilceCreateMgr::MESHEFFECT_MAP_OBJ_Lotas, desc1, desc2, desc3);
			desc1.vSize = _float3(0.01f);
			desc1.fMaxTime_Duration = 9999.0f;
			desc2.AccRotSpeed = 0.0001f;
			desc1.vEmissive = _float3(0);
			desc1.iDiffuseTextureIndex = 365;
			GETPARTICLE->Create_MeshEffectDesc_MAP(id, desc1, desc2, FixPos, desc3);

			{
				auto texDesc = GETPARTICLE->Get_EffectSetting_Tex(CPartilceCreateMgr::Um_FireMask_2_png,
					9999.0f,
					0.8f,
					_float4(0.78f, 0.35f, 0.1f, 1),
					_float4(0, 0, 0, 0.3f),
					1,
					_float3(0.7f),
					_float3(0.1f),
					1);

				if (count == 1 || count == 4)
				{
					texDesc.TargetColor = _float4(0.68f, 0.55f, 0.2f, 1);

				}

				texDesc.vEmissive_SBB = _float3(1, 0.8f, 0.0f);
				texDesc.Particle_Power = 3.0f;

				texDesc.iNoiseTextureIndex = 360;
				texDesc.iMaskingTextureIndex = 106;
				GetMeshEffect_Fix()->Set_Particle(texDesc);

			}


		}
		listFloat3.clear();


		listFloat3.push_back(_float3(22.247766f, 50.000000f, 23.085331f));
		listFloat3.push_back(_float3(13.913488f, 50.000000f, 23.085331f));
		listFloat3.push_back(_float3(59.685730f, 67.530006f, 75.403770f));
		listFloat3.push_back(_float3(67.879105f, 67.530006f, 75.403770f));



		listFloat3.push_back(_float3(96.381218f, 45.000011f, 74.160652f));
		listFloat3.push_back(_float3(82.843170f, 45.000011f, 74.245422f));
		listFloat3.push_back(_float3(109.301048f, 43.050011f, 73.086113f));
		listFloat3.push_back(_float3(108.924980f, 43.050011f, 55.893261f));

		listFloat3.push_back(_float3(112.721992f, 43.050011f, 74.876572f));
		listFloat3.push_back(_float3(105.345367f, 43.050011f, 74.876572f));
		listFloat3.push_back(_float3(105.345367f, 43.050011f, 49.274857f));
		listFloat3.push_back(_float3(112.721992f, 43.050011f, 49.356396f));

		listFloat3.push_back(_float3(143.656448f, 44.600010f, 58.098022f));
		listFloat3.push_back(_float3(143.577911f, 44.600010f, 62.294209f));
		listFloat3.push_back(_float3(140.214310f, 44.600010f, 58.031128f));
		listFloat3.push_back(_float3(140.252289f, 44.600010f, 62.757294f));
		listFloat3.push_back(_float3(172.994125f, 44.600010f, 118.799599f));
		listFloat3.push_back(_float3(167.615372f, 44.600010f, 118.722282f));


		listFloat3.push_back(_float3(103.320984f, 32.200001f, 288.545959f));
		listFloat3.push_back(_float3(89.704994f, 32.200001f, 288.571747f));
		listFloat3.push_back(_float3(80.716476f, 32.000011f, 308.908661f));
		listFloat3.push_back(_float3(57.225475f, 32.000011f, 234.781067f));
		listFloat3.push_back(_float3(57.545902f, 32.000011f, 248.421188f));
		listFloat3.push_back(_float3(71.133125f, 32.065060f, 321.413269f));
		listFloat3.push_back(_float3(58.152012f, 32.065975f, 321.413269f));

		

		


		for (_float3 pos : listFloat3)
		{
			FixPos = pos;

			GETPARTICLE->Get_MeshEffectDesc_Map_FreeSet(CPartilceCreateMgr::MESHEFFECT_MAP_OBJ_Lotas, desc1, desc2, desc3);
			desc1.fMaxTime_Duration = 9999.0f;
			desc1.vPosition = _float3(-0.0f, -1.1f, 0.0f);
			desc2.DealyTime = GETUTIL->RandomFloat(0.1f, 2);
			desc2.AccRotSpeed = 0.0001f;
			desc1.iDiffuseTextureIndex = 365;
			GETPARTICLE->Create_MeshEffectDesc_MAP(id, desc1, desc2, FixPos, desc3);

			{
				auto texDesc = GETPARTICLE->Get_EffectSetting_Tex(CPartilceCreateMgr::Um_Spawn3_Imagepng_Snow,
					9999.0f,
					1.0f,
					_float4(1),
					_float4(0),
					1,
					_float3(0.2f),
					_float3(0.01f),
					1);

				texDesc.eParticleTypeID = InstanceEffect_Ball;
				texDesc.eInstanceCount = Prototype_VIBuffer_Point_Instance_16;
				texDesc.ParticleStartRandomPosMin = _float3(-0.3f,1,-0.3f);
				texDesc.ParticleStartRandomPosMax = _float3(0.3f,1, 0.3f);
				texDesc.vEmissive_SBB = _float3(1, 0.4f, 0.0f);
				texDesc.Particle_Power = 3.0f;

			//	texDesc.iNoiseTextureIndex = 360;
			//	texDesc.iMaskingTextureIndex = 106;
				GetMeshEffect_Fix()->Set_Particle(texDesc);

			}

		}
		listFloat3.clear();


		// fotaion

	//	151.909 / 7.225 362.594
	//	152.167 7.404 392.032
	
		listFloat3.push_back(_float3(151.909f, 7.325f, 362.594f));
		listFloat3.push_back(_float3(152.167f, 7.504f, 392.032f));


		for (_float3 pos : listFloat3)
		{
			FixPos = pos;

			GETPARTICLE->Get_MeshEffectDesc_Map_FreeSet(CPartilceCreateMgr::MESHEFFECT_MAP_OBJ_Freeset2, desc1, desc2, desc3);
			desc1.fMaxTime_Duration = 9999.0f;
			desc2.DealyTime = 0.1f;

			desc1.vPosition = _float3(0, -0.1f, 0);
			desc1.vSize = _float3(0.35f, 0.25f, 0.35f);

			GETPARTICLE->Create_MeshEffectDesc_MAP(id, desc1, desc2, FixPos, desc3);


			GETPARTICLE->Get_MeshEffectDesc_Map_FreeSet(CPartilceCreateMgr::MESHEFFECT_MAP_OBJ_Freeset3, desc1, desc2, desc3);
			desc1.fMaxTime_Duration = 9999.0f;
			desc2.DealyTime = 0.1f;

			desc1.vPosition = _float3(0, 1.25f, 0);
			desc1.vSize = _float3(6.0f);

			GETPARTICLE->Create_MeshEffectDesc_MAP(id, desc1, desc2, FixPos, desc3);
			
			{
				INSTPARTICLEDESC texDesc = GETPARTICLE->Get_EffectSetting_Tex(CPartilceCreateMgr::Um_Dust_2_FounTain,
					9999.f,
					0.3f,
					_float4(0.04f, 0.78f, 0.98f, 1.0f),
					_float4(0.04f, 0.78f, 0.98f, 0.8f),
					1,
					_float3(0.2f),
					_float3(0.1f),
					1);

				texDesc.ParticleStartRandomPosMin = _float3(0,0,0);
				texDesc.ParticleStartRandomPosMax = _float3(0,0,0);
				texDesc.Particle_Power = 3.0f;

				GetMeshEffect_Fix()->Set_Particle(texDesc);

			}

		}

		listFloat3.clear();


		// Door
		listFloat3.push_back(_float3(107.620811f, 3.450010f, 445.574219f));
		listFloat3.push_back(_float3(111.792023f, 3.450010f, 445.574219f));
		
		count = 0;

		for (_float3 pos : listFloat3)
		{
			count++;

			FixPos = pos;
		
			GETPARTICLE->Get_MeshEffectDesc_Map_FreeSet(CPartilceCreateMgr::MESHEFFECT_MAP_OBJ_Freeset4, desc1, desc2, desc3);
			desc1.fMaxTime_Duration = 9999.0f;
			desc2.DealyTime = 0.1f;
			if (count == 1)
				desc1.vLookDir = _float3(1, 0, 1);
			if (count == 2)
				desc1.vLookDir = _float3(-1, 0, 1);

			GETPARTICLE->Create_MeshEffectDesc_MAP(id, desc1, desc2, FixPos, desc3);

			//{
			//	auto texDesc = GETPARTICLE->Get_EffectSetting_Tex(CPartilceCreateMgr::Um_Spawn3_Imagepng_Snow2,
			//		9999.0f,
			//		0.3f,
			//		_float4(0.75f, 0.25f, 0.15f, 1),
			//		_float4(1, 0.1f, 0, 0.1f),
			//		1,
			//		_float3(0.4f),
			//		_float3(0.1f),
			//		1);

			//	texDesc.iTextureLayerIndex = 15;
			//	texDesc.vEmissive_SBB = _float3(1, 0.4f, 0.0f);
			//	//	texDesc.Particle_Power = 3.0f;

			//	GetMeshEffect_Fix()->Set_Particle(texDesc);

			//}

		}
		

		listFloat3.clear();



		// one

		// ½Ã°£ÀÖÀ¸¸é Ãß°¡
//		listFloat3.push_back(_float3(181.994354f, 32.450012f, 287.644196f)); // new Mesh

		{
			FixPos = (_float3(170.167926f, 41.050011f, 177.772952f));

			// MESHEFFECT_MAP_OBJ_Freeset2
			// MESHEFFECT_MAP_OBJ_Freeset3
			GETPARTICLE->Get_MeshEffectDesc_Map_FreeSet(CPartilceCreateMgr::MESHEFFECT_MAP_OBJ_Freeset2, desc1, desc2, desc3);
			desc1.fMaxTime_Duration = 9999.0f;
			desc2.DealyTime = GETUTIL->RandomFloat(0.1f, 2);
			GETPARTICLE->Create_MeshEffectDesc_MAP(id, desc1, desc2, FixPos, desc3);

			desc1.vPosition = _float3(0, 0.5f, 0.0f);//2
			desc1.vSize = _float3(0.9f, 0.7f, 0.9f); // 2
			GETPARTICLE->Create_MeshEffectDesc_MAP(id, desc1, desc2, FixPos, desc3);


			GETPARTICLE->Get_MeshEffectDesc_Map_FreeSet(CPartilceCreateMgr::MESHEFFECT_MAP_OBJ_Freeset3, desc1, desc2, desc3);

			desc1.fMaxTime_Duration = 9999.0f;
			GETPARTICLE->Create_MeshEffectDesc_MAP(id, desc1, desc2, FixPos, desc3);

			desc1.vPosition = _float3(0, 4.5f, 0.0f); // 2
			desc1.vSize = _float3(14.0f); // 2
			GETPARTICLE->Create_MeshEffectDesc_MAP(id, desc1, desc2, FixPos, desc3);

			desc1.vPosition = _float3(0.0f, 0.8f, 0.0f); // 3
			desc1.vSize = _float3(25.0f); // 3
			GETPARTICLE->Create_MeshEffectDesc_MAP(id, desc1, desc2, FixPos, desc3);

			{
				INSTPARTICLEDESC texDesc = GETPARTICLE->Get_EffectSetting_Tex(CPartilceCreateMgr::Um_Dust_2_FounTain,
					9999.f,
					0.4f,
					_float4(0.04f, 0.78f, 0.98f, 1.0f),
					_float4(0.04f, 0.78f, 0.98f, 0.8f),
					1,
					_float3(0.5f),
					_float3(0.2f),
					1);

				texDesc.Particle_Power = 8.0f;

				GetMeshEffect_Fix()->Set_Particle(texDesc);
			}
		}


		{
			FixPos = _float3(41.784278f, 73.228539f, 23.181898f); // ¿©½Å»ó

			GETPARTICLE->Get_MeshEffectDesc_Map_FreeSet(CPartilceCreateMgr::MESHEFFECT_MAP_OBJ_Freeset0, desc1, desc2, desc3);
			desc1.fMaxTime_Duration = 9999.0f;
			desc1.vPosition = _float3(-0.0f, 1.0f, 0.0f);
			desc1.vSize = _float3(2.5f);

			GETPARTICLE->Create_MeshEffectDesc_MAP(id, desc1, desc2, FixPos, desc3);

			{
				auto texDesc = GETPARTICLE->Get_EffectSetting_Tex(CPartilceCreateMgr::Um_Spawn3_Imagepng_Snow2,
					9999.0f,
					0,
					_float4(0.35f, 0.25f, 0.8f, 1),
					_float4(1, 1, 1, 0.3f),
					1,
					_float3(0.7f),
					_float3(0.1f),
					0);

				texDesc.vEmissive_SBB = _float3(1, 0.4f, 0.0f);
				//	texDesc.Particle_Power = 3.0f;

				GetMeshEffect_Fix()->Set_Particle(texDesc);

			}

		}


	}

		break;
	case SCENE_STAGE7:
		break;
	default:
		break;
	}

}


HRESULT CPartilceCreateMgr::ReadyParticle_ALL_Stages()
{
	INSTPARTICLEDESC texDesc;
	INSTMESHDESC	 meshDesc;

	for (auto& cc : mListTexParticleMap)
	{
		cc.clear();
	}

#pragma region SCENE_STAGE1
 
	texDesc = GetSingle(CUtilityMgr)->Get_TextureParticleDesc(TEXT("Jino_Stage1_FireParticle_0"));
	texDesc.FollowingTarget = nullptr;

	texDesc.vFixedPosition = _float3(32.2f, 42.8f, 65.4f);
	mListTexParticleMap[SCENE_STAGE1].push_back(texDesc);
	AddSoundDesc(SCENE_STAGE1,texDesc.vFixedPosition, UM_Env_Fire,0.4f);

//	GetSingle(CUtilityMgr)->Create_TextureInstance(SCENE_STAGE1, texDesc);


	texDesc = GetSingle(CUtilityMgr)->Get_TextureParticleDesc(TEXT("Jino_Stage1_WaterFallParticle_0"));
	texDesc.eInstanceCount = Prototype_VIBuffer_Point_Instance_4;
	texDesc.FollowingTarget = nullptr;
	texDesc.bBillboard = true;
	texDesc.TextureChageFrequency = 1;

	texDesc.vFixedPosition = _float3(95.110130f, 35.100010f, 40.533039f);
	mListTexParticleMap[SCENE_STAGE1].push_back(texDesc);
	AddSoundDesc(SCENE_STAGE1, texDesc.vFixedPosition, UM_Env_Water_Cave);

	texDesc.vFixedPosition = _float3(101.110130f, 35.100010f, 42.033039f);
	mListTexParticleMap[SCENE_STAGE1].push_back(texDesc);
	AddSoundDesc(SCENE_STAGE1, texDesc.vFixedPosition, UM_Env_Water_Cave);

	texDesc.vFixedPosition = _float3(117.710f, 36.6f, 39.433f);
	mListTexParticleMap[SCENE_STAGE1].push_back(texDesc);
	AddSoundDesc(SCENE_STAGE1, texDesc.vFixedPosition, UM_Env_Water_Cave);

//	GetSingle(CUtilityMgr)->Create_TextureInstance(SCENE_STAGE1, texDesc2, &vParticlesPos);


	texDesc = GetSingle(CUtilityMgr)->Get_TextureParticleDesc(TEXT("Jino_Stage1_WaterFallParticle_1"));
	texDesc.FollowingTarget = nullptr;
	texDesc.vFixedPosition = _float3(256.512f, 33.f, 191.703f);
	mListTexParticleMap[SCENE_STAGE1].push_back(texDesc);
	AddSoundDesc(SCENE_STAGE1, texDesc.vFixedPosition, UM_Env_Water_Cave);

//	GetSingle(CUtilityMgr)->Create_TextureInstance(SCENE_STAGE1, texDesc);

	texDesc = GetSingle(CUtilityMgr)->Get_TextureParticleDesc(TEXT("Jino_Stage1_FountainParticle_0"));
	texDesc.FollowingTarget = nullptr;
	texDesc.vFixedPosition = _float3(257.012054f, 24.5f, 180.302515f);
	mListTexParticleMap[SCENE_STAGE1].push_back(texDesc);
	AddSoundDesc(SCENE_STAGE1, texDesc.vFixedPosition, UM_Env_Water_Wet);

//	GetSingle(CUtilityMgr)->Create_TextureInstance(SCENE_STAGE1, texDesc);

	texDesc = GetSingle(CUtilityMgr)->Get_TextureParticleDesc(TEXT("Jino_Stage1_FountainParticle_1"));
	texDesc.FollowingTarget = nullptr;
	texDesc.vFixedPosition = _float3(256.811218f, 21.f, 191.899643f);
	texDesc.TotalParticleTime = 999999999999.f;
	mListTexParticleMap[SCENE_STAGE1].push_back(texDesc);
	AddSoundDesc(SCENE_STAGE1, texDesc.vFixedPosition, PARTICLSOUND_TYPE_NONE);

//	GetSingle(CUtilityMgr)->Create_TextureInstance(SCENE_STAGE1, texDesc);

	texDesc = GetSingle(CUtilityMgr)->Get_TextureParticleDesc(TEXT("Jino_Stage1_Rain_0"));
	texDesc.FollowingTarget = nullptr;
	texDesc.vFixedPosition = _float3(256.811218f, 27.5f, 178.358f);
	mListTexParticleMap[SCENE_STAGE1].push_back(texDesc);
	AddSoundDesc(SCENE_STAGE1, texDesc.vFixedPosition, PARTICLSOUND_TYPE_NONE);


//	GetSingle(CUtilityMgr)->Create_TextureInstance(SCENE_STAGE1, texDesc);

#pragma endregion SCENE_STAGE1

#pragma region SCENE_STAGE2

	// ¸ð´ÚºÒ
	texDesc = GetSingle(CUtilityMgr)->Get_TextureParticleDesc(TEXT("Jino_Stage2_CampFireParticle_1"));
	texDesc.FollowingTarget = nullptr;
	texDesc.vFixedPosition = _float3(395.f, 4.400f, 227.5f);
	mListTexParticleMap[SCENE_STAGE2].push_back(texDesc);
	AddSoundDesc(SCENE_STAGE2, texDesc.vFixedPosition, UM_Env_Fire_bonefire);


	{
		texDesc = GETPARTICLE->Get_EffectSetting_Tex(CPartilceCreateMgr::Um_FireMask_2_png,
			9999.0f,
			0.8f,
			_float4(0.75f, 0.26f, 0.05f, 1),
			_float4(0.95f, 0.13f, 0.2f, 0.5f),
			0,
			_float3(1.0f),
			_float3(0.1f),
			1);

		texDesc.vEmissive_SBB = _float3(1, 0.5f, 0.0f);
		texDesc.Particle_Power = 4.0f;
		texDesc.TargetColor = _float4(0.97f, 0.25f, 0.01f, 0.5f);
		texDesc.TargetColor2 = _float4(0.97f, 0.25f, 0.01f, 0.1f);
		// desctex.iTextureLayerIndex = ;
		texDesc.vFixedPosition = _float3(395.f, 4.400f, 227.5f);

		mListTexParticleMap[SCENE_STAGE2].push_back(texDesc);
		AddSoundDesc(SCENE_STAGE2, texDesc.vFixedPosition, PARTICLSOUND_TYPE_NONE);

	}
//	GetSingle(CUtilityMgr)->Create_TextureInstance(SCENE_STAGE2, texDesc);


	meshDesc = GetSingle(CUtilityMgr)->Get_MeshParticleDesc(TEXT("Jino_Stage2_Mesh_PinkBall"));
	meshDesc.FollowingTarget = nullptr;
	meshDesc.vFixedPosition = _float3(473.970f, 4.900f, 414.755f);;
	meshDesc.EachParticleLifeTime = 999999999.f;
	mListMeshParticleMap[SCENE_STAGE2].push_back(meshDesc);
	AddSoundDesc(SCENE_STAGE2, texDesc.vFixedPosition, PARTICLSOUND_TYPE_NONE);

	//	GetSingle(CUtilityMgr)->Create_MeshInstance(SCENE_STAGE2, meshDesc);
	mListMeshParticleMap[SCENE_STAGE2].push_back(meshDesc);
	AddSoundDesc(SCENE_STAGE2, texDesc.vFixedPosition, PARTICLSOUND_TYPE_NONE);

	meshDesc.vFixedPosition = _float3(495.42f, 4.61f, 420.7f);
	//	GetSingle(CUtilityMgr)->Create_MeshInstance(SCENE_STAGE2, meshDesc);
	mListMeshParticleMap[SCENE_STAGE2].push_back(meshDesc);
	AddSoundDesc(SCENE_STAGE2, texDesc.vFixedPosition, PARTICLSOUND_TYPE_NONE);

	meshDesc.vFixedPosition = _float3(485.67f, 4.91f, 420.289f);
	//	GetSingle(CUtilityMgr)->Create_MeshInstance(SCENE_STAGE2, meshDesc);
	mListMeshParticleMap[SCENE_STAGE2].push_back(meshDesc);
	AddSoundDesc(SCENE_STAGE2, texDesc.vFixedPosition, PARTICLSOUND_TYPE_NONE);

	meshDesc.vFixedPosition = _float3(475.605f, 4.900f, 420.541f);
	mListMeshParticleMap[SCENE_STAGE2].push_back(meshDesc);
	AddSoundDesc(SCENE_STAGE2, texDesc.vFixedPosition, PARTICLSOUND_TYPE_NONE);

//	GetSingle(CUtilityMgr)->Create_MeshInstance(SCENE_STAGE2, meshDesc);


	texDesc = GetSingle(CUtilityMgr)->Get_TextureParticleDesc(TEXT("Jino_Stage2_FireParticle_0"));
	texDesc.FollowingTarget = nullptr;
	texDesc.vFixedPosition = _float3(568.f, 24.300f, 403.5f);
	mListTexParticleMap[SCENE_STAGE2].push_back(texDesc);
	AddSoundDesc(SCENE_STAGE2, texDesc.vFixedPosition, UM_Env_Fire, 0.4f);

//	GetSingle(CUtilityMgr)->Create_TextureInstance(SCENE_STAGE2, texDesc);

	texDesc = GetSingle(CUtilityMgr)->Get_TextureParticleDesc(TEXT("Jino_Stage2_FireParticle_0"));
	texDesc.FollowingTarget = nullptr;
	texDesc.vFixedPosition = _float3(573.5f, 24.000f, 395.3f);
	mListTexParticleMap[SCENE_STAGE2].push_back(texDesc);
	AddSoundDesc(SCENE_STAGE2, texDesc.vFixedPosition, UM_Env_Fire, 0.4f);

	AddSoundDesc(SCENE_STAGE2, _float3(383.272278f, 16.500010f, 354.304535f), UM_Env_Fire, 0.2f);
	AddSoundDesc(SCENE_STAGE2, _float3(594.999634f, 21.800009f, 408.885498f), UM_Env_Fire, 0.2f);

//	GetSingle(CUtilityMgr)->Create_TextureInstance(SCENE_STAGE2, texDesc);

#pragma endregion SCENE_STAGE2


#pragma region SCENE_STAGE3

#pragma endregion SCENE_STAGE3


#pragma region SCENE_STAGE4
#pragma endregion SCENE_STAGE4


#pragma region SCENE_STAGE5
#pragma endregion SCENE_STAGE5


#pragma region SCENE_STAGE6

	// Only Sound

	AddSoundDesc(SCENE_STAGE6, _float3(170.167926f, 41.050011f, 177.772952f), UM_Env_Water_Foutain1, 0.7f,15.0f,17.0f);
	AddSoundDesc(SCENE_STAGE6, _float3(151.909f, 7.325f, 362.594f), UM_Env_Water_Foutain2, 0.5f, 5.0f,11);
	AddSoundDesc(SCENE_STAGE6, _float3(152.167f, 7.504f, 392.032f), UM_Env_Water_Foutain2, 0.5f, 5.0f,11);


#pragma endregion SCENE_STAGE6


#pragma region SCENE_STAGE7
#pragma endregion SCENE_STAGE7

	return S_OK;
}



void CPartilceCreateMgr::Free()
{
}
