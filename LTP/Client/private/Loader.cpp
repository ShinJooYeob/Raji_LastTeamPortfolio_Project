#include "stdafx.h"
#include "..\Public\Loader.h"



//////////////////////////////////////////////////////////////////////////

///??/////////////////////////////////////////////////////////////////////
#include "TestObject.h"
#include "AnimationClip.h"
#include "Navigation.h"
#include "TestStaticObject.h"

//////////////////////////////////////////////////////////////////////////

////STA0GE_4//////////////////////////////////////////////////////////////////
//Boss
#include "Rangda.h"
#include "Chiedtian.h"
#include "Snake.h"
#include "Mahabalasura.h"

//Weapons
#include "Chiedtuan_Weapon.h"
#include "Rangda_Finger.h"
#include "Mahabalasura_Arms.h"
#include "Mahabalasura_AttackArms.h"
#include "Mahabalasura_Weapon.h"
#include "CopyMahabalasura.h"

//TestEffect
#include "Rangda_MagicCircle.h"
//////////////////////////////////////////////////////////////////////////////

////STA0GE_5//////////////////////////////////////////////////////////////////
// Player
#include "Player.h"
// Player_Weapon
#include "PlayerWeapon_Spear.h"
#include "PlayerWeapon_Bow.h"
#include "PlayerWeapon_Sword.h"
#include "PlayerWeapon_Shield.h"
#include "PlayerWeapon_Chakra.h"
#include "PlayerWeapon_Arrow.h"
//////////////////////////////////////////////////////////////////////////////
////STA0GE_6//////////////////////////////////////////////////////////////////
#include "TestObject_PhysX.h"

///////////////////////////////////////////////////////////////////////////
#include "ESCursor.h"
#include "EditorTerrain.h"
#include "Camera_Editor.h"
#include "StaticMapObject.h"
#include "NaviPoint.h"
#include "AssimpCreateMgr.h"


////����//////////////////////////////////////////////////////////////////////
#include "Monster_Mahinasura_Minion.h"
#include "Monster_Mahinasura_Leader.h"
#include "Monster_Vayusura_Minion.h"
#include "Monster_Vayusura_Leader.h"
#include "Monster_Tezabsura_Minion.h"
#include "Monster_Tezabsura_Purple.h"
#include "Monster_Tezabsura_Bomber.h"
#include "Monster_Tezabsura_Landmine.h"
#include "Monster_Jalsura.h"
#include "Monster_Ninjasura.h"
#include "Monster_Ninjasura_Minion.h"
#include "Monster_Gadasura_Black.h"
#include "Monster_Gadasura_Rage.h"
////////////////////////////////////////////////////////////////////////////////

//Monster_Bullet////////////////////////////////////////////////////////////////
#include "Monster_Bullet_Universal.h"
#include "Monster_Texture_Bullet.h"
#include "Monster_Weapon_Universal.h"
#include "Gadasura_Rage_Hollogram.h"
////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
#include "TestNonAnimInstancing.h"
//////////////////////////////////////////////////////////////////////////

_uint CALLBACK LoadingThread(void* _Prameter)
{
	THREADARG tThreadArg{};
	memcpy(&tThreadArg, _Prameter, sizeof(THREADARG));
	delete _Prameter;


	CLoader* pLoader = (CLoader*)(tThreadArg.pArg);
	 

	switch (pLoader->Get_SceneID())
	{
	case SCENEID::SCENE_LOBY:
		pLoader->Load_Scene_Loby(tThreadArg.IsClientQuit, tThreadArg.CriSec);
		break;
	case SCENEID::SCENE_STAGESELECT:
		pLoader->Load_Scene_StageSelect(tThreadArg.IsClientQuit, tThreadArg.CriSec);
		break;
	case SCENEID::SCENE_STAGE1:
		pLoader->Load_Scene_Stage1(tThreadArg.IsClientQuit, tThreadArg.CriSec);
		break;
	case SCENEID::SCENE_STAGE2:
		pLoader->Load_Scene_Stage2(tThreadArg.IsClientQuit, tThreadArg.CriSec);
		break;
	case SCENEID::SCENE_STAGE3:
		pLoader->Load_Scene_Stage3(tThreadArg.IsClientQuit, tThreadArg.CriSec);
		break;

	case SCENEID::SCENE_STAGE4:
		pLoader->Load_Scene_Stage4(tThreadArg.IsClientQuit, tThreadArg.CriSec);
		break;

	case SCENEID::SCENE_STAGE5:
		pLoader->Load_Scene_Stage5(tThreadArg.IsClientQuit, tThreadArg.CriSec);
		break;

	case SCENEID::SCENE_STAGE6:
		pLoader->Load_Scene_Stage6(tThreadArg.IsClientQuit, tThreadArg.CriSec);
		break;

	case SCENEID::SCENE_STAGE7:
		pLoader->Load_Scene_Stage7(tThreadArg.IsClientQuit, tThreadArg.CriSec);
		break;

	case SCENEID::SCENE_BOSS:
		pLoader->Load_Scene_Boss(tThreadArg.IsClientQuit, tThreadArg.CriSec);
		break;

	case SCENEID::SCENE_ENDING:
		pLoader->Load_Scene_Ending(tThreadArg.IsClientQuit, tThreadArg.CriSec);
		break;
		
	case SCENEID::SCENE_EDIT:

		pLoader->Load_Scene_Edit(tThreadArg.IsClientQuit, tThreadArg.CriSec);
		break;

	default:
		MSGBOX("Failed to Create Scene");
		break;
	}

	return 0;
}


CLoader::CLoader()
{
}



HRESULT CLoader::Initialize(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, SCENEID eSceneID)
{
	m_eSceneID = eSceneID;
	m_bIsLoadingFinished = false;
	m_pDevice = pDevice;
	m_pDeviceContext = pDeviceContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);

	g_pGameInstance->PlayThread(LoadingThread, this);

	return S_OK;
}

HRESULT CLoader::Load_Scene_Loby(_bool * _IsClientQuit, CRITICAL_SECTION * _CriSec)
{
	CGameInstance* pGameInstance = GetSingle(CGameInstance);

#pragma region PROTOTYPE_COMPONENT

	_Matrix			TransformMatrix;
//	TransformMatrix = XMMatrixScaling(0.0001f, 0.0001f, 0.0001f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
//	FAILED_CHECK(pGameInstance->Add_Component_Prototype(SCENEID::SCENE_LOBY, TAG_CP(Prototype_Mesh_TestObject),
//		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "PlayerMesh", "Player.FBX", TransformMatrix)));
//

	TransformMatrix = XMMatrixScaling(0.0001f, 0.0001f, 0.0001f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	FAILED_CHECK(pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Mesh_SkyBox),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "SkyBox", "SkyBox_0.FBX", TransformMatrix)));


	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	///* Å×½ºÆ® ¾Ë°Ô¶ô */
	FAILED_CHECK(pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Mesh_AlgaeRock_Ledge),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "TestObject", "AlgaeRock_Ledge.FBX", TransformMatrix)));



	// #TEST Create NoAssimp 

	static _bool bisLoaded = false; 
	if (!bisLoaded)
	{
		bisLoaded = true;

		//// #LOAD ALLMODEL
		//// ��� �� �ε� TEST
		TransformMatrix = XMMatrixScaling(0.0001f, 0.0001f, 0.0001f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
		GetSingle(CAssimpCreateMgr)->Load_Model_One_ByFBXName(TAG_CP(Prototype_Mesh_Player), TransformMatrix);


		// Player Weapons //
		TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
		GetSingle(CAssimpCreateMgr)->Load_Model_One_ByFBXName(TAG_CP(Prototype_Mesh_PlayerWeapon_Spear), TransformMatrix);
		TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
		GetSingle(CAssimpCreateMgr)->Load_Model_One_ByFBXName(TAG_CP(Prototype_Mesh_PlayerWeapon_Bow), TransformMatrix);
		TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
		GetSingle(CAssimpCreateMgr)->Load_Model_One_ByFBXName(TAG_CP(Prototype_Mesh_PlayerWeapon_Sword), TransformMatrix);
		TransformMatrix = XMMatrixScaling(1.f, 1.f, 1.f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
		GetSingle(CAssimpCreateMgr)->Load_Model_One_ByFBXName(TAG_CP(Prototype_Mesh_PlayerWeapon_Shield), TransformMatrix);
		TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
		GetSingle(CAssimpCreateMgr)->Load_Model_One_ByFBXName(TAG_CP(Prototype_Mesh_PlayerWeapon_Chakra), TransformMatrix);
		TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
		GetSingle(CAssimpCreateMgr)->Load_Model_One_ByFBXName(TAG_CP(Prototype_Mesh_PlayerWeapon_Arrow), TransformMatrix);
		//


	//	TransformMatrix = XMMatrixScaling(0.0001f, 0.0001f, 0.0001f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	//	GetSingle(CAssimpCreateMgr)->Load_Model_One_ByFBXName(TAG_CP(Prototype_Mesh_Boss_Chieftian), TransformMatrix);


	//	TransformMatrix = XMMatrixScaling(0.0001f, 0.0001f, 0.0001f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	//	GetSingle(CAssimpCreateMgr)->Load_Model_One_ByFBXName(TAG_CP(Prototype_Mesh_Player), TransformMatrix);

	//	TransformMatrix = XMMatrixScaling(0.0001f, 0.0001f, 0.0001f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	//	GetSingle(CAssimpCreateMgr)->Load_Model_One_ByFBXName(TAG_CP(Prototype_Mesh_Boss_ChieftianWeapon), TransformMatrix);



		//TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
		//GetSingle(CAssimpCreateMgr)->Load_Model_One_ByFBXName(TAG_MONSTER_BULLET(Prototype_Mesh_Monster_Bullet_Tezabsura_Minion), TransformMatrix);


		//ALL Load
	//	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	//	GetSingle(CAssimpCreateMgr)->Load_ALL_Model(TransformMatrix, TransformMatrix);


		//// #LOAD OneLOAD
	}


#pragma endregion

#pragma  region PROTOTYPE_GAMEOBJECT
	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_StaticMapObject), CStaticMapObject::Create(m_pDevice, m_pDeviceContext)));
	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_TestObject), CTestObject::Create(m_pDevice, m_pDeviceContext)));

	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_Player), CPlayer::Create(m_pDevice, m_pDeviceContext)));
	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_PlayerWeapon_Spear), CPlayerWeapon_Spear::Create(m_pDevice, m_pDeviceContext)));
	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_PlayerWeapon_Bow), CPlayerWeapon_Bow::Create(m_pDevice, m_pDeviceContext)));
	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_PlayerWeapon_Sword), CPlayerWeapon_Sword::Create(m_pDevice, m_pDeviceContext)));
	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_PlayerWeapon_Shield), CPlayerWeapon_Shield::Create(m_pDevice, m_pDeviceContext)));
	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_PlayerWeapon_Chakra), CPlayerWeapon_Chakra::Create(m_pDevice, m_pDeviceContext)));
	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_PlayerWeapon_Arrow), CPlayerWeapon_Arrow::Create(m_pDevice, m_pDeviceContext)));

#pragma endregion

	EnterCriticalSection(_CriSec);
	m_iLoadingMaxCount = 1;
	m_iLoadingProgressCount = 0;
	LeaveCriticalSection(_CriSec);

	EnterCriticalSection(_CriSec);
	m_bIsLoadingFinished = true;
	LeaveCriticalSection(_CriSec);

	return S_OK;
}

HRESULT CLoader::Load_Scene_StageSelect(_bool * _IsClientQuit, CRITICAL_SECTION * _CriSec)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

#pragma region PROTOTYPE_COMPONENT


#pragma endregion

#pragma  region PROTOTYPE_GAMEOBJECT

#pragma endregion

	RELEASE_INSTANCE(CGameInstance);
	EnterCriticalSection(_CriSec);
	m_iLoadingMaxCount = 1;
	m_iLoadingProgressCount = 0;
	LeaveCriticalSection(_CriSec);

	for (int i = 0; i < m_iLoadingMaxCount; ++i)
	{
		EnterCriticalSection(_CriSec);
		m_iLoadingProgressCount = i;
		LeaveCriticalSection(_CriSec);
	}

	EnterCriticalSection(_CriSec);
	m_bIsLoadingFinished = true;
	LeaveCriticalSection(_CriSec);
	m_bIsLoadingFinished = true;
	return S_OK;
}

HRESULT CLoader::Load_Scene_Stage1(_bool * _IsClientQuit, CRITICAL_SECTION * _CriSec)
{

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

#pragma region PROTOTYPE_COMPONENT

#pragma endregion

#pragma  region PROTOTYPE_GAMEOBJECT
	
#pragma endregion


	RELEASE_INSTANCE(CGameInstance);


	EnterCriticalSection(_CriSec);
	m_iLoadingMaxCount = 1;
	m_iLoadingProgressCount = 0;
	LeaveCriticalSection(_CriSec);

	for (int i = 0; i < m_iLoadingMaxCount; ++i)
	{
		EnterCriticalSection(_CriSec);
		m_iLoadingProgressCount = i;
		LeaveCriticalSection(_CriSec);
	}

	EnterCriticalSection(_CriSec);
	m_bIsLoadingFinished = true;
	LeaveCriticalSection(_CriSec);
	m_bIsLoadingFinished = true;
	return S_OK;
}

HRESULT CLoader::Load_Scene_Stage2(_bool * _IsClientQuit, CRITICAL_SECTION * _CriSec)
{


	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

#pragma region PROTOTYPE_COMPONENT


#pragma endregion

#pragma  region PROTOTYPE_GAMEOBJECT

#pragma endregion


	RELEASE_INSTANCE(CGameInstance);
	EnterCriticalSection(_CriSec);
	m_iLoadingMaxCount = 1;
	m_iLoadingProgressCount = 0;
	LeaveCriticalSection(_CriSec);

	EnterCriticalSection(_CriSec);
	m_bIsLoadingFinished = true;
	LeaveCriticalSection(_CriSec);


	return S_OK;
}

HRESULT CLoader::Load_Scene_Stage3(_bool * _IsClientQuit, CRITICAL_SECTION * _CriSec)
{

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

#pragma region PROTOTYPE_COMPONENT


#pragma endregion

#pragma  region PROTOTYPE_GAMEOBJECT

#pragma endregion


	RELEASE_INSTANCE(CGameInstance);
	EnterCriticalSection(_CriSec);
	m_iLoadingMaxCount = 1;
	m_iLoadingProgressCount = 0;
	LeaveCriticalSection(_CriSec);

	EnterCriticalSection(_CriSec);
	m_bIsLoadingFinished = true;
	LeaveCriticalSection(_CriSec);


	return S_OK;
}

HRESULT CLoader::Load_Scene_Stage4(_bool * _IsClientQuit, CRITICAL_SECTION * _CriSec)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

#pragma region PROTOTYPE_COMPONENT


	_Matrix			TransformMatrix;
	TransformMatrix = XMMatrixScaling(1.f, 1.f, 1.f) * XMMatrixRotationY(XMConvertToRadians(180.0f));

	/* For.Prototype_Component_Navigation */
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE_STAGE4, TEXT("Prototype_Component_Navigation"),
		CNavigation::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/data/NaviMesh/NaviData_Stage_2.dat")))))
		return E_FAIL;

	/*TransformMatrix = XMMatrixScaling(0.0001f, 0.0001f, 0.0001f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	FAILED_CHECK(pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STAGE4, TAG_CP(Prototype_Mesh_Boss_Rangda),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "Boss_RangDa", "Boss_Randa.FBX", TransformMatrix)));

	TransformMatrix = XMMatrixScaling(0.0001f, 0.0001f, 0.0001f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	FAILED_CHECK(pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STAGE4, TAG_CP(Prototype_Mesh_Player),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "PlayerMesh", "Player.FBX", TransformMatrix, 2)));*/




	TransformMatrix = XMMatrixScaling(0.0001f, 0.0001f, 0.0001f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	FAILED_CHECK(pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Mesh_SkyBox),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "SkyBox", "SkyBox_Boss.FBX", TransformMatrix)));

	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	/* �׽�Ʈ �˰Զ� */
	FAILED_CHECK(pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Mesh_AlgaeRock_Ledge),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "TestObject", "AlgaeRock_Ledge.FBX", TransformMatrix)));


	TransformMatrix = XMMatrixScaling(0.0001f, 0.0001f, 0.0001f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	GetSingle(CAssimpCreateMgr)->Load_Model_One_ByFBXName(TAG_CP(Prototype_Mesh_Boss_Rangda), TransformMatrix);

	TransformMatrix = XMMatrixScaling(0.0001f, 0.0001f, 0.0001f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	GetSingle(CAssimpCreateMgr)->Load_Model_One_ByFBXName(TAG_CP(Prototype_Mesh_Boss_Rangda_Finger), TransformMatrix);

	TransformMatrix = XMMatrixScaling(0.0001f, 0.0001f, 0.0001f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	GetSingle(CAssimpCreateMgr)->Load_Model_One_ByFBXName(TAG_CP(Prototype_Mesh_Boss_Chieftian), TransformMatrix);

	TransformMatrix = XMMatrixScaling(0.0001f, 0.0001f, 0.0001f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	GetSingle(CAssimpCreateMgr)->Load_Model_One_ByFBXName(TAG_CP(Prototype_Mesh_Boss_ChieftianWeapon), TransformMatrix);

	TransformMatrix = XMMatrixScaling(0.0001f, 0.0001f, 0.0001f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	GetSingle(CAssimpCreateMgr)->Load_Model_One_ByFBXName(TAG_CP(Prototype_Mesh_Boss_ChieftianWeapon2), TransformMatrix);

	TransformMatrix = XMMatrixScaling(0.0001f, 0.0001f, 0.0001f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	GetSingle(CAssimpCreateMgr)->Load_Model_One_ByFBXName(TAG_CP(Prototype_Mesh_Boss_ChieftianWeapon3), TransformMatrix);

	TransformMatrix = XMMatrixScaling(0.0001f, 0.0001f, 0.0001f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	GetSingle(CAssimpCreateMgr)->Load_Model_One_ByFBXName(TAG_CP(Prototype_Mesh_Boss_ChieftianWeapon4), TransformMatrix);

	TransformMatrix = XMMatrixScaling(0.0001f, 0.0001f, 0.0001f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	GetSingle(CAssimpCreateMgr)->Load_Model_One_ByFBXName(TAG_CP(Prototype_Mesh_Boss_Snake), TransformMatrix);

	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	GetSingle(CAssimpCreateMgr)->Load_Model_One_ByFBXName(TAG_CP(Prototype_Mesh_Boss_Mahabalasura), TransformMatrix);

	TransformMatrix = XMMatrixScaling(0.0001f, 0.0001f, 0.0001f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	GetSingle(CAssimpCreateMgr)->Load_Model_One_ByFBXName(TAG_CP(Prototype_Mesh_Boss_MahabalasuraArms), TransformMatrix);

	TransformMatrix = XMMatrixScaling(0.0001f, 0.0001f, 0.0001f) * XMMatrixRotationY(XMConvertToRadians(270.f));
	GetSingle(CAssimpCreateMgr)->Load_Model_One_ByFBXName(TAG_CP(Prototype_Mesh_Boss_MahabalasuraAttackArms), TransformMatrix);

	TransformMatrix = XMMatrixScaling(0.0001f, 0.0001f, 0.0001f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	GetSingle(CAssimpCreateMgr)->Load_Model_One_ByFBXName(TAG_CP(Prototype_Mesh_Boss_MahabalasurWeapon), TransformMatrix);

	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	GetSingle(CAssimpCreateMgr)->Load_Model_One_ByFBXName(TAG_CP(Prototype_Mesh_Boss_MahabalasurCopy), TransformMatrix);


	TransformMatrix = XMMatrixScaling(0.0001f, 0.0001f, 0.0001f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	GetSingle(CAssimpCreateMgr)->Load_Model_One_ByFBXName(TAG_CP(Prototype_Mesh_PlayerWeapon_Spear), TransformMatrix);
	TransformMatrix = XMMatrixScaling(0.0001f, 0.0001f, 0.0001f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	GetSingle(CAssimpCreateMgr)->Load_Model_One_ByFBXName(TAG_CP(Prototype_Mesh_PlayerWeapon_Bow), TransformMatrix);
	TransformMatrix = XMMatrixScaling(0.0001f, 0.0001f, 0.0001f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	GetSingle(CAssimpCreateMgr)->Load_Model_One_ByFBXName(TAG_CP(Prototype_Mesh_PlayerWeapon_Sword), TransformMatrix);





#pragma endregion

#pragma  region PROTOTYPE_GAMEOBJECT

	//FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_TestObject_Himeko), CTestObject::Create(m_pDevice, m_pDeviceContext)));

	//Boss
	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_Object_Boss_Rangda), CRangda::Create(m_pDevice, m_pDeviceContext)));
	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_Object_Boss_Rangda_Finger), CRangda_Finger::Create(m_pDevice, m_pDeviceContext)));
	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_Object_Boss_Chiedtian), CChiedtian::Create(m_pDevice, m_pDeviceContext)));
	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_Object_Boss_ChiedtianWeapon), CChiedtuan_Weapon::Create(m_pDevice, m_pDeviceContext)));
	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_Object_Boss_Snake), CSnake::Create(m_pDevice, m_pDeviceContext)));
	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_Object_Boss_Mahabalasura), CMahabalasura::Create(m_pDevice, m_pDeviceContext)));
	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_Object_Boss_MahabalasuraArms), CMahabalasura_Arms::Create(m_pDevice, m_pDeviceContext)));
	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_Object_Boss_MahabalasuraAttackArms), CMahabalasura_AttackArms::Create(m_pDevice, m_pDeviceContext)));
	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_Object_Boss_MahabalasuraWeapon), CMahabalasura_Weapon::Create(m_pDevice, m_pDeviceContext)));
	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_Object_Boss_MahabalasuraCopy), CCopyMahabalasura::Create(m_pDevice, m_pDeviceContext)));

	//Test
	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_StaticMapObject), CStaticMapObject::Create(m_pDevice, m_pDeviceContext)));
	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_Player), CPlayer::Create(m_pDevice, m_pDeviceContext)));

	//EffectTest
	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_Object_Effect_MagicCircle), CRangda_MagicCircle::Create(m_pDevice, m_pDeviceContext)));
	
#pragma endregion


	RELEASE_INSTANCE(CGameInstance);
	EnterCriticalSection(_CriSec);
	m_iLoadingMaxCount = 1;
	m_iLoadingProgressCount = 0;
	LeaveCriticalSection(_CriSec);

	EnterCriticalSection(_CriSec);
	m_bIsLoadingFinished = true;
	LeaveCriticalSection(_CriSec);
	 

	return S_OK;
}

HRESULT CLoader::Load_Scene_Stage5(_bool * _IsClientQuit, CRITICAL_SECTION * _CriSec)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

#pragma region PROTOTYPE_COMPONENT

	_Matrix			TransformMatrix;
	

	TransformMatrix = XMMatrixScaling(0.0001f, 0.0001f, 0.0001f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	FAILED_CHECK(pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STAGE7, TAG_CP(Prototype_Mesh_SkyBox),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "SkyBox", "SkyBox_0.FBX", TransformMatrix)));


	// Player Weapons //
	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	GetSingle(CAssimpCreateMgr)->Load_Model_One_ByFBXName(TAG_CP(Prototype_Mesh_PlayerWeapon_Spear), TransformMatrix);
	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	GetSingle(CAssimpCreateMgr)->Load_Model_One_ByFBXName(TAG_CP(Prototype_Mesh_PlayerWeapon_Bow), TransformMatrix);
	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	GetSingle(CAssimpCreateMgr)->Load_Model_One_ByFBXName(TAG_CP(Prototype_Mesh_PlayerWeapon_Sword), TransformMatrix);
	TransformMatrix = XMMatrixScaling(1.f, 1.f, 1.f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	GetSingle(CAssimpCreateMgr)->Load_Model_One_ByFBXName(TAG_CP(Prototype_Mesh_PlayerWeapon_Shield), TransformMatrix);
	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	GetSingle(CAssimpCreateMgr)->Load_Model_One_ByFBXName(TAG_CP(Prototype_Mesh_PlayerWeapon_Chakra), TransformMatrix);
	//


	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	/* �׽�Ʈ �˰Զ� */
	FAILED_CHECK(pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Mesh_AlgaeRock_Ledge),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "TestObject", "AlgaeRock_Ledge.FBX", TransformMatrix)));
	
#pragma endregion

#pragma  region PROTOTYPE_GAMEOBJECT
	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(L"TestNonAnimInstance", CTestNonAnimInstancing::Create(m_pDevice, m_pDeviceContext)));
	
	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_StaticMapObject), CStaticMapObject::Create(m_pDevice, m_pDeviceContext)));
	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_Player), CPlayer::Create(m_pDevice, m_pDeviceContext)));
	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_PlayerWeapon_Spear), CPlayerWeapon_Spear::Create(m_pDevice, m_pDeviceContext)));
	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_PlayerWeapon_Bow), CPlayerWeapon_Bow::Create(m_pDevice, m_pDeviceContext)));
	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_PlayerWeapon_Sword), CPlayerWeapon_Sword::Create(m_pDevice, m_pDeviceContext)));
	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_PlayerWeapon_Shield), CPlayerWeapon_Shield::Create(m_pDevice, m_pDeviceContext)));
	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_PlayerWeapon_Chakra), CPlayerWeapon_Chakra::Create(m_pDevice, m_pDeviceContext)));
#pragma endregion


	RELEASE_INSTANCE(CGameInstance);
	EnterCriticalSection(_CriSec);
	m_iLoadingMaxCount = 1;
	m_iLoadingProgressCount = 0;
	LeaveCriticalSection(_CriSec);

	EnterCriticalSection(_CriSec);
	m_bIsLoadingFinished = true;
	LeaveCriticalSection(_CriSec);


	return S_OK;
}

HRESULT CLoader::Load_Scene_Stage6(_bool * _IsClientQuit, CRITICAL_SECTION * _CriSec)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

#pragma region PROTOTYPE_COMPONENT

	_Matrix			TransformMatrix;

	// Player Weapons //
	TransformMatrix = XMMatrixScaling(0.0001f, 0.0001f, 0.0001f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	GetSingle(CAssimpCreateMgr)->Load_Model_One_ByFBXName(TAG_CP(Prototype_Mesh_PlayerWeapon_Spear), TransformMatrix);
	TransformMatrix = XMMatrixScaling(0.0001f, 0.0001f, 0.0001f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	GetSingle(CAssimpCreateMgr)->Load_Model_One_ByFBXName(TAG_CP(Prototype_Mesh_PlayerWeapon_Bow), TransformMatrix);
	TransformMatrix = XMMatrixScaling(0.0001f, 0.0001f, 0.0001f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	GetSingle(CAssimpCreateMgr)->Load_Model_One_ByFBXName(TAG_CP(Prototype_Mesh_PlayerWeapon_Sword), TransformMatrix);

	TransformMatrix = XMMatrixScaling(1, 1, 1);
	GetSingle(CAssimpCreateMgr)->Load_Model_One_ByFBXName(TAG_MONSTER_BULLET(Prototype_Mesh_Monster_Bullet_Vayusura_Leader), TransformMatrix);

	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	GetSingle(CAssimpCreateMgr)->Load_Model_One_ByFBXName(TAG_CP(Prototype_Mesh_Monster_Ninjasura_Minion), TransformMatrix);


	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	/* �׽�Ʈ �˰Զ� */
	FAILED_CHECK(pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Mesh_AlgaeRock_Ledge),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "TestObject", "AlgaeRock_Ledge.FBX", TransformMatrix)));

#pragma endregion

#pragma  region PROTOTYPE_GAMEOBJECT
	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_StaticMapObject), CStaticMapObject::Create(m_pDevice, m_pDeviceContext)));
	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_Player), CPlayer::Create(m_pDevice, m_pDeviceContext)));
	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_PlayerWeapon_Spear), CPlayerWeapon_Spear::Create(m_pDevice, m_pDeviceContext)));
	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_PlayerWeapon_Bow), CPlayerWeapon_Bow::Create(m_pDevice, m_pDeviceContext)));
	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_PlayerWeapon_Sword), CPlayerWeapon_Sword::Create(m_pDevice, m_pDeviceContext)));
	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_Object_Static_PhysX), CTestObject_PhysX::Create(m_pDevice, m_pDeviceContext)));
	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_Object_Dynamic_PhysX), CTestObject_PhysX::Create(m_pDevice, m_pDeviceContext)));
	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_Object_Joint_PhysX), CTestObject_PhysX::Create(m_pDevice, m_pDeviceContext)));
	
#pragma endregion



	RELEASE_INSTANCE(CGameInstance);
	EnterCriticalSection(_CriSec);
	m_iLoadingMaxCount = 1;
	m_iLoadingProgressCount = 0;
	LeaveCriticalSection(_CriSec);

	EnterCriticalSection(_CriSec);
	m_bIsLoadingFinished = true;
	LeaveCriticalSection(_CriSec);


	return S_OK;
}

HRESULT CLoader::Load_Scene_Stage7(_bool * _IsClientQuit, CRITICAL_SECTION * _CriSec)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

#pragma region PROTOTYPE_COMPONENT

	_Matrix			TransformMatrix;
	TransformMatrix = XMMatrixScaling(0.0001f, 0.0001f, 0.0001f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	FAILED_CHECK(pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STAGE7, TAG_CP(Prototype_Mesh_SkyBox),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "SkyBox", "SkyBox_0.FBX", TransformMatrix)));

	////////Monster_Mesh
	TransformMatrix = XMMatrixScaling(0.015f, 0.015f, 0.015f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	GetSingle(CAssimpCreateMgr)->Load_Model_One_ByFBXName(TAG_CP(Prototype_Mesh_Monster_Mahinasura_Minion), TransformMatrix);

	TransformMatrix = XMMatrixScaling(0.025f, 0.025f, 0.025f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	GetSingle(CAssimpCreateMgr)->Load_Model_One_ByFBXName(TAG_CP(Prototype_Mesh_Monster_Mahinasura_Leader), TransformMatrix);

	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	GetSingle(CAssimpCreateMgr)->Load_Model_One_ByFBXName(TAG_CP(Prototype_Mesh_Monster_Vayusura_Minion), TransformMatrix);

	TransformMatrix = XMMatrixScaling(0.03f, 0.03f, 0.03f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	GetSingle(CAssimpCreateMgr)->Load_Model_One_ByFBXName(TAG_CP(Prototype_Mesh_Monster_Vayusura_Leader), TransformMatrix);

	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	GetSingle(CAssimpCreateMgr)->Load_Model_One_ByFBXName(TAG_CP(Prototype_Mesh_Monster_Tezabsura_Minion), TransformMatrix);

	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	GetSingle(CAssimpCreateMgr)->Load_Model_One_ByFBXName(TAG_CP(Prototype_Mesh_Monster_Tezabsura_Purple), TransformMatrix);

	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	GetSingle(CAssimpCreateMgr)->Load_Model_One_ByFBXName(TAG_CP(Prototype_Mesh_Monster_Tezabsura_Bomber), TransformMatrix);

	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	GetSingle(CAssimpCreateMgr)->Load_Model_One_ByFBXName(TAG_CP(Prototype_Mesh_Monster_Tezabsura_Landmine), TransformMatrix);

	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f)) * XMMatrixTranslation(0.f, 1.5f, 0.f);
	GetSingle(CAssimpCreateMgr)->Load_Model_One_ByFBXName(TAG_CP(Prototype_Mesh_Monster_Jalsura), TransformMatrix);

	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	GetSingle(CAssimpCreateMgr)->Load_Model_One_ByFBXName(TAG_CP(Prototype_Mesh_Monster_Ninjasura), TransformMatrix);

	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	GetSingle(CAssimpCreateMgr)->Load_Model_One_ByFBXName(TAG_CP(Prototype_Mesh_Monster_Ninjasura_Minion), TransformMatrix);

	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	GetSingle(CAssimpCreateMgr)->Load_Model_One_ByFBXName(TAG_CP(Prototype_Mesh_Monster_Gadasura_Black), TransformMatrix);

	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	GetSingle(CAssimpCreateMgr)->Load_Model_One_ByFBXName(TAG_CP(Prototype_Mesh_Monster_Gadasura_Rage), TransformMatrix);

	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	GetSingle(CAssimpCreateMgr)->Load_Model_One_ByFBXName(TAG_CP(Prototype_Mesh_Monster_Gadasura_Rage_Hollogram), TransformMatrix);


	////////////////////////Monster_Bullet
	TransformMatrix = XMMatrixScaling(1.f, 1.f, 1.f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	GetSingle(CAssimpCreateMgr)->Load_Model_One_ByFBXName(TAG_MONSTER_BULLET(Prototype_Mesh_Monster_Bullet_Vayusura_Leader), TransformMatrix);

	TransformMatrix = XMMatrixScaling(1.f, 1.f, 1.f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	GetSingle(CAssimpCreateMgr)->Load_Model_One_ByFBXName(TAG_MONSTER_BULLET(Prototype_Mesh_Monster_Bullet_Tezabsura_Minion), TransformMatrix);

	TransformMatrix = XMMatrixScaling(1.f, 1.f, 1.f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	GetSingle(CAssimpCreateMgr)->Load_Model_One_ByFBXName(TAG_MONSTER_BULLET(Prototype_Mesh_Monster_Bullet_Tezabsura_Purple), TransformMatrix);

	TransformMatrix = XMMatrixScaling(1.f, 1.f, 1.f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	GetSingle(CAssimpCreateMgr)->Load_Model_One_ByFBXName(TAG_MONSTER_BULLET(Prototype_Mesh_Monster_Bullet_Tezabsura_Bomber), TransformMatrix);

	TransformMatrix = XMMatrixScaling(1.f, 1.f, 1.f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	GetSingle(CAssimpCreateMgr)->Load_Model_One_ByFBXName(TAG_MONSTER_BULLET(Prototype_Mesh_Monster_Bullet_Tezabsura_Landmine), TransformMatrix);

	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(0.f));
	GetSingle(CAssimpCreateMgr)->Load_Model_One_ByFBXName(TAG_MONSTER_BULLET(Prototype_Mesh_Monster_Bullet_Ninjasura_Knife), TransformMatrix);

	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	GetSingle(CAssimpCreateMgr)->Load_Model_One_ByFBXName(TAG_MONSTER_BULLET(Prototype_Mesh_Monster_Bullet_Gadasura_Sinkhole), TransformMatrix);

	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	GetSingle(CAssimpCreateMgr)->Load_Model_One_ByFBXName(TAG_MONSTER_BULLET(Prototype_Mesh_Monster_Bullet_Gadasura_Terrain), TransformMatrix);
	/////////////////////////////////////

	/////////////////////////////////////Monster_Weapon
	TransformMatrix = XMMatrixScaling(0.0001f, 0.0001f, 0.0001f) * XMMatrixRotationX(XMConvertToRadians(180.f));
	GetSingle(CAssimpCreateMgr)->Load_Model_One_ByFBXName(TAG_CP(Prototype_Mesh_Monster_Weapon_Gadasura_Black), TransformMatrix);

	TransformMatrix = XMMatrixScaling(0.0001f, 0.0001f, 0.0001f) * XMMatrixRotationX(XMConvertToRadians(180.f));
	GetSingle(CAssimpCreateMgr)->Load_Model_One_ByFBXName(TAG_CP(Prototype_Mesh_Monster_Weapon_Gadasura_Rage), TransformMatrix);

	//TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	//GetSingle(CAssimpCreateMgr)->Load_Model_One_ByFBXName(TAG_CP(Prototype_Mesh_Monster_Vayusura_Minion), TransformMatrix);
	/////////////////////////////////////


	//TransformMatrix = XMMatrixScaling(0.0001f, 0.0001f, 0.0001f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	//FAILED_CHECK(pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STAGE7, TAG_CP(Prototype_Mesh_Monster_Mahinasura_Minion),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "Monster_Mahinasura_Minion", "Monster_Mahinasura_Minion.FBX", TransformMatrix)));
	//
	//
	/////////////���Ͷ� �÷��̾� ��ȣ�ۿ� üũ�Ϸ��� �ϴ� �÷��̾� �ܾ��
	//TransformMatrix = XMMatrixScaling(0.0001f, 0.0001f, 0.0001f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	//FAILED_CHECK(pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STAGE7, TAG_CP(Prototype_Mesh_Player),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "PlayerMesh", "Player.FBX", TransformMatrix, 2)));

	//TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	//FAILED_CHECK(pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Mesh_AlgaeRock_Ledge),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "TestObject", "AlgaeRock_Ledge.FBX", TransformMatrix)));
	//

	//TransformMatrix = XMMatrixScaling(0.02f, 0.02f, 0.02f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	//GetSingle(CAssimpCreateMgr)->Load_Model_One_ByFBXName(TAG_CP(Prototype_Mesh_Monster_Vayusura_Leader), TransformMatrix);

#pragma endregion

#pragma  region PROTOTYPE_GAMEOBJECT
	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_Object_Monster_Mahinasura_Minion), CMonster_Mahinasura_Minion::Create(m_pDevice, m_pDeviceContext)));
	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_Object_Monster_Mahinasura_Leader), CMonster_Mahinasura_Leader::Create(m_pDevice, m_pDeviceContext)));
	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_Object_Monster_Vayusura_Minion), CMonster_Vayusura_Minion::Create(m_pDevice, m_pDeviceContext)));
	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_Object_Monster_Vayusura_Leader), CMonster_Vayusura_Leader::Create(m_pDevice, m_pDeviceContext)));
	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_Obejct_Monster_Tezabsura_Minion), CMonster_Tezabsura_Minion::Create(m_pDevice, m_pDeviceContext)));
	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_Obejct_Monster_Tezabsura_Purple), CMonster_Tezabsura_Purple::Create(m_pDevice, m_pDeviceContext)));
	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_Obejct_Monster_Tezabsura_Bomber), CMonster_Tezabsura_Bomber::Create(m_pDevice, m_pDeviceContext)));
	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_Obejct_Monster_Tezabsura_Landmine), CMonster_Tezabsura_Landmine::Create(m_pDevice, m_pDeviceContext)));
	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_Object_Monster_Jalsura), CMonster_Jalsura::Create(m_pDevice, m_pDeviceContext)));
	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_Object_Monster_Ninjasura), CMonster_Ninjasura::Create(m_pDevice, m_pDeviceContext)));
	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_Object_Monster_Ninjasura_Minion), CMonster_Ninjasura_Minion::Create(m_pDevice, m_pDeviceContext)));
	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_Object_Monster_Gadasura_Black), CMonster_Gadasura_Black::Create(m_pDevice, m_pDeviceContext)));
	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_Object_Monster_Gadasura_Rage), CMonster_Gadasura_Rage::Create(m_pDevice, m_pDeviceContext)));
	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_Object_Monster_Gadasura_Rage_Hollogram), CGadasura_Rage_Hollogram::Create(m_pDevice, m_pDeviceContext)));

	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_Object_Monster_Bullet_Universal), CMonster_Bullet_Universal::Create(m_pDevice, m_pDeviceContext)));
	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_Object_Monster_Texture_Bullet), CMonster_Texture_Bullet::Create(m_pDevice, m_pDeviceContext)));
	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_Object_Monster_Weapon_Universal), CMonster_Weapon_Universal::Create(m_pDevice, m_pDeviceContext)));


	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_StaticMapObject), CStaticMapObject::Create(m_pDevice, m_pDeviceContext)));
	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_Player), CPlayer::Create(m_pDevice, m_pDeviceContext)));
#pragma endregion

#pragma  region PROTOTYPE_COMPONENT
	/* For.Prototype_Component_Texture_Monster_Texture_Bullet */
	FAILED_CHECK(pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STAGE7, TAG_CP(Prototype_Texture_Monster_Bullet),
		CTexture::Create(m_pDevice, m_pDeviceContext, L"Monster_Texture_Bullet.txt")));
#pragma endregion

	RELEASE_INSTANCE(CGameInstance);
	EnterCriticalSection(_CriSec);
	m_iLoadingMaxCount = 1;
	m_iLoadingProgressCount = 0;
	LeaveCriticalSection(_CriSec);

	EnterCriticalSection(_CriSec);
	m_bIsLoadingFinished = true;
	LeaveCriticalSection(_CriSec);


	return S_OK;
}

HRESULT CLoader::Load_Scene_Boss(_bool * _IsClientQuit, CRITICAL_SECTION * _CriSec)
{



	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

#pragma region PROTOTYPE_COMPONENT


#pragma endregion

#pragma  region PROTOTYPE_GAMEOBJECT

#pragma endregion


	RELEASE_INSTANCE(CGameInstance);
	EnterCriticalSection(_CriSec);
	m_iLoadingMaxCount = 1;
	m_iLoadingProgressCount = 0;
	LeaveCriticalSection(_CriSec);

	EnterCriticalSection(_CriSec);
	m_bIsLoadingFinished = true;
	LeaveCriticalSection(_CriSec);


	return S_OK;
}

HRESULT CLoader::Load_Scene_Ending(_bool * _IsClientQuit, CRITICAL_SECTION * _CriSec)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);


#pragma region PROTOTYPE_COMPONENT


#pragma endregion

#pragma  region PROTOTYPE_GAMEOBJECT

#pragma endregion


	RELEASE_INSTANCE(CGameInstance);




	EnterCriticalSection(_CriSec);
	m_iLoadingMaxCount = 99999999;
	m_iLoadingProgressCount = 0;
	LeaveCriticalSection(_CriSec);

	for (int i = 0; i < m_iLoadingMaxCount; ++i)
	{
		EnterCriticalSection(_CriSec);
		m_iLoadingProgressCount = i;
		LeaveCriticalSection(_CriSec);
	}

	EnterCriticalSection(_CriSec);
	m_bIsLoadingFinished = true;
	LeaveCriticalSection(_CriSec);
	return S_OK;
}

HRESULT CLoader::Load_Scene_Edit(_bool * _IsClientQuit, CRITICAL_SECTION * _CriSec)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
#pragma region PROTOTYPE_COMPONENT

	FAILED_CHECK(pGameInstance->Add_Component_Prototype(SCENEID::SCENE_EDIT, TAG_CP(Prototype_VIBuffer_Terrain),
		CVIBuffer_EditorTerrain::Create(m_pDevice, m_pDeviceContext, 4, 4)));
	FAILED_CHECK(pGameInstance->Add_Component_Prototype(SCENEID::SCENE_EDIT, TAG_CP(Prototype_Texture_EditScene),
		CTexture::Create(m_pDevice, m_pDeviceContext, L"EditScene.txt")));
	FAILED_CHECK(pGameInstance->Add_Component_Prototype(SCENEID::SCENE_EDIT, TAG_CP(Prototype_Texture_Edit_Terrain),
		CTexture::Create(m_pDevice, m_pDeviceContext, L"EditTerrain.txt")));

	_Matrix			TransformMatrix;

	TransformMatrix = XMMatrixScaling(0.0001f, 0.0001f, 0.0001f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	FAILED_CHECK(pGameInstance->Add_Component_Prototype(SCENEID::SCENE_EDIT, TAG_CP(Prototype_Mesh_SkyBox),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "SkyBox", "SkyBox_0.FBX", TransformMatrix)));

	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	FAILED_CHECK(pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Mesh_AlgaeRock_Ledge),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "TestObject", "AlgaeRock_Ledge.FBX", TransformMatrix)));



	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	GetSingle(CAssimpCreateMgr)->Load_ALL_Model(TransformMatrix, TransformMatrix);

#pragma endregion

#pragma  region PROTOTYPE_GAMEOBJECT

	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_EditorCursor), CESCursor::Create(m_pDevice, m_pDeviceContext)));

	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_Camera_Editor), CCamera_Editor::Create(m_pDevice, m_pDeviceContext)));
	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_EditorTerrain), CEditorTerrain::Create(m_pDevice, m_pDeviceContext)));
	//FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(L"ProtoType_EditRendererUI", CRendererEditSceneUI::Create(m_pDevice, m_pDeviceContext)));

	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_StaticMapObject), 	CStaticMapObject::Create(m_pDevice, m_pDeviceContext)));

	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_NaviPoint),	CNaviPoint::Create(m_pDevice, m_pDeviceContext)));
#pragma endregion

	RELEASE_INSTANCE(CGameInstance);
	//EnterCriticalSection(_CriSec);
	//m_iLoadingMaxCount = 1;
	//m_iLoadingProgressCount = 0;
	//LeaveCriticalSection(_CriSec);

	//for (int i = 0; i < m_iLoadingMaxCount; ++i)
	//{
	//	EnterCriticalSection(_CriSec);
	//	m_iLoadingProgressCount = i;
	//	LeaveCriticalSection(_CriSec);
	//}

	EnterCriticalSection(_CriSec);
	m_bIsLoadingFinished = true;
	LeaveCriticalSection(_CriSec);

	return S_OK;
}

HRESULT CLoader::Load_Model_DatFile()
{
	// ?�이???�일�?assimp ?�이 모델 초기??/ ?�이??초기??
	CGameInstance* pGameInstance = GetSingle(CGameInstance);
	_Matrix TransformMatrix = XMMatrixScaling(1, 1, 1);

	// ?�일 경로
	auto static_dat = GetSingle(CGameInstance)->Load_ExtensionList(STR_FILEPATH_RESOURCE_FBXDAT_L, "stc");
	auto dynamic_dat = GetSingle(CGameInstance)->Load_ExtensionList(STR_FILEPATH_RESOURCE_FBXDAT_L, "dyn");


	// MODELDESC / LOAD ?�수
	list<MODELDESC*> List_ModelStatic;
	list<MODELDESC*> List_ModelDynamic;

	FAILED_CHECK( Load_Model(static_dat, List_ModelStatic));
	FAILED_CHECK(Load_Model(dynamic_dat, List_ModelDynamic));

#pragma region TESTCODE

	// 모델 컴포?�트 ?�성
	// For. Test
	auto dynamiciter = dynamic_dat.begin();
	
	GetSingle(CUtilityMgr)->Start_DebugTimer(CUtilityMgr::DEBUGTIMER_1);
	for (auto& modeldesc : List_ModelDynamic)
	{
		FAILED_CHECK(pGameInstance->Add_Component_Prototype(
			SCENEID::SCENE_STATIC,
			(*dynamiciter)->FileName,
			CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, modeldesc, TransformMatrix)));

		dynamiciter++;
		if (dynamiciter == dynamic_dat.end())
			break;
	}

	auto staticiter = static_dat.begin();
	for (auto& modeldesc : List_ModelStatic)
	{
		FAILED_CHECK(pGameInstance->Add_Component_Prototype(
			SCENEID::SCENE_STATIC,
			(*staticiter)->FileName,
			CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, modeldesc, TransformMatrix)));

		staticiter++;
		if (staticiter == static_dat.end())
			break;
	}
	GetSingle(CUtilityMgr)->End_DebugTimer(CUtilityMgr::DEBUGTIMER_1,L"asdasd");


#pragma endregion TESTCODE
	
	Safe_Delete_List(static_dat);
	Safe_Delete_List(dynamic_dat);

//	Safe_Delete_List(List_ModelStatic);
//	Safe_Delete_List(List_ModelDynamic);

	return S_OK;

}

HRESULT CLoader::Load_Model(const list<MYFILEPATH*>& pathlist, list<MODELDESC*>& List_Modeldesc)
{
	// #LOAD ?�라?�언??로드 ?�수

	_ulong			dwByte = 0;
	for (auto& path : pathlist)
	{
		wstring fullpath = path->FullPath;
		wstring FileName = path->FileName;
		MODELDESC* modelDesc = nullptr;
		HANDLE			hFile = CreateFile(fullpath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (0 == hFile)
			return E_FAIL;

		{
			modelDesc = new MODELDESC;

			// UINT
			ReadFile(hFile, modelDesc->mFBXFullPath, sizeof(wchar_t)*MAX_PATH, &dwByte, nullptr);
			ReadFile(hFile, modelDesc->mFBXFileName, sizeof(wchar_t)*MAX_PATH, &dwByte, nullptr);

			ReadFile(hFile, &modelDesc->mModelType, sizeof(_uint), &dwByte, nullptr);
			ReadFile(hFile, &modelDesc->mNumMeshes, sizeof(_uint), &dwByte, nullptr);
			ReadFile(hFile, &modelDesc->mNumMaterials, sizeof(_uint), &dwByte, nullptr);

			if (modelDesc->mModelType == CModel::TYPE_ANIM)
			{
				ReadFile(hFile, &modelDesc->mNumBones, sizeof(_uint), &dwByte, nullptr);
				ReadFile(hFile, &modelDesc->mNumAnimations, sizeof(_uint), &dwByte, nullptr);
			}

			CModel::MODELTYPE ModelType = (CModel::MODELTYPE)modelDesc->mModelType;

			// ?�당
			if (modelDesc->mNumMeshes != 0)
				modelDesc->mMeshDesc = new MESHDESC[modelDesc->mNumMeshes];

			if (modelDesc->mNumMaterials != 0)
				modelDesc->mMaterials = new MATDESC[modelDesc->mNumMaterials];

			if (ModelType == CModel::TYPE_ANIM)
			{
				if (modelDesc->mNumBones != 0)
					modelDesc->mBones = new BONEDESC[modelDesc->mNumBones];

				if (modelDesc->mNumAnimations != 0)
					modelDesc->mAnimations = new ANIDESC[modelDesc->mNumAnimations];
			}

			// MESH
			for (_uint i = 0; i < modelDesc->mNumMeshes; ++i)
			{
				MESHDESC* meshdesc = &modelDesc->mMeshDesc[i];

				// uint
				ReadFile(hFile, &meshdesc->mPrimitiveTypes, sizeof(_uint), &dwByte, nullptr);
				ReadFile(hFile, &meshdesc->mNumVertices, sizeof(_uint), &dwByte, nullptr);
				ReadFile(hFile, &meshdesc->mNumFaces, sizeof(_uint), &dwByte, nullptr);
				ReadFile(hFile, &meshdesc->mMaterialIndex, sizeof(_uint), &dwByte, nullptr);

				if (ModelType == CModel::TYPE_ANIM)
				{
					ReadFile(hFile, &meshdesc->mNumAffectingBones, sizeof(_uint), &dwByte, nullptr);
				}

				meshdesc->mVertices = new _float3[meshdesc->mNumVertices];
				meshdesc->mNormals = new _float3[meshdesc->mNumVertices];
				meshdesc->mTangents = new _float3[meshdesc->mNumVertices];
				meshdesc->mUV = new _float2[meshdesc->mNumVertices];
				meshdesc->mFaces = new FACEINDICES32[meshdesc->mNumFaces];

				// VTX
				ReadFile(hFile, meshdesc->mVertices, sizeof(_float3)*meshdesc->mNumVertices, &dwByte, nullptr);
				if (dwByte == 0)
					return E_FAIL;

				ReadFile(hFile, meshdesc->mNormals, sizeof(_float3) *meshdesc->mNumVertices, &dwByte, nullptr);
				if (dwByte == 0)
					return E_FAIL;

				ReadFile(hFile, meshdesc->mTangents, sizeof(_float3)*meshdesc->mNumVertices, &dwByte, nullptr);
				if (dwByte == 0)
					return E_FAIL;

				// UV 추�?
				ReadFile(hFile, meshdesc->mUV, sizeof(_float2)*meshdesc->mNumVertices, &dwByte, nullptr);
				if (dwByte == 0)
					return E_FAIL;

				// ?�러�?UV 추�???
				//for (_uint j = 0; j < AI_MAX_NUMBER_OF_TEXTURECOORDS; ++j)
				//{
				//	_float3* cash = new _float3[meshdesc->mNumVertices];


				//	ReadFile(hFile, cash, sizeof(_float3)*meshdesc->mNumVertices, &dwByte, nullptr);
				//	if (cash->x == 0)
				//	{
				//		meshdesc->mTextureCoords[j] = nullptr;
				//		Safe_Delete_Array(cash);
				//	}
				//	else
				//	{
				//		meshdesc->mTextureCoords[j] = new _float3[meshdesc->mNumVertices];
				//		for (_uint k = 0; k < meshdesc->mNumVertices; ++k)
				//		{
				//			meshdesc->mTextureCoords[j]->x = cash->x;
				//			meshdesc->mTextureCoords[j]->y = cash->y;
				//			meshdesc->mTextureCoords[j]->z = 0;
				//		}
				//		Safe_Delete_Array(cash);
				//	}
				//}

				// INDEX
				ReadFile(hFile, meshdesc->mFaces, sizeof(FACEINDICES32)*meshdesc->mNumFaces, &dwByte, nullptr);
				if (dwByte == 0)
					return E_FAIL;

				// WEIGHT
				if (modelDesc->mModelType == CModel::TYPE_ANIM)
				{
					_uint NumAffectingBones = meshdesc->mNumAffectingBones;
					if (NumAffectingBones != 0)
					{
						// �??�덱??리스??
						meshdesc->mAffectingBones = new _uint[NumAffectingBones];
						meshdesc->mMeshBones = new MESHBONEDESC[NumAffectingBones];

						ReadFile(hFile, meshdesc->mAffectingBones, sizeof(_uint)*NumAffectingBones, &dwByte, nullptr);
						// �?Weight
						for (_uint bone = 0; bone < NumAffectingBones; ++bone)
						{
							ReadFile(hFile, &meshdesc->mMeshBones[bone].mNumWeights, sizeof(_uint), &dwByte, nullptr);
							_uint NumWeight = meshdesc->mMeshBones[bone].mNumWeights;
							if (NumWeight == 0)
								continue;

							meshdesc->mMeshBones[bone].mAiWeights = new aiVertexWeight[NumWeight];

							ReadFile(hFile, meshdesc->mMeshBones[bone].mAiWeights,
								sizeof(aiVertexWeight)*NumWeight, &dwByte, nullptr);
							int debug = 5;
						}
					}
				}
			}

			// MATERIAL
			for (_uint i = 0; i < modelDesc->mNumMaterials; ++i)
			{
				MATDESC* matdesc = &modelDesc->mMaterials[i];
				ReadFile(hFile, matdesc->MatName, sizeof(wchar_t)*MAX_PATH*AI_TEXTURE_TYPE_MAX, &dwByte, nullptr);
			}

			// BONE
			if (modelDesc->mModelType == CModel::TYPE_ANIM)
			{
				for (_uint i = 0; i < modelDesc->mNumBones; ++i)
				{
					BONEDESC* bonedesc = &modelDesc->mBones[i];
					ReadFile(hFile, bonedesc->mParentBoneName, sizeof(char)*MAX_PATH, &dwByte, nullptr);
					ReadFile(hFile, bonedesc->mCurrentBoneName, sizeof(char)*MAX_PATH, &dwByte, nullptr);
					ReadFile(hFile, &bonedesc->mOffsetMat, sizeof(_float4x4), &dwByte, nullptr);
					ReadFile(hFile, &bonedesc->mTransMat, sizeof(_float4x4), &dwByte, nullptr);
					ReadFile(hFile, &bonedesc->mDepth, sizeof(_uint), &dwByte, nullptr);
				}


				// ANI
				for (_uint i = 0; i < modelDesc->mNumAnimations; ++i)
				{
					ANIDESC* anidesc = &modelDesc->mAnimations[i];
					ReadFile(hFile, anidesc->mAniName, sizeof(char)*MAX_PATH, &dwByte, nullptr);
					ReadFile(hFile, &anidesc->mDuration, sizeof(double), &dwByte, nullptr);
					ReadFile(hFile, &anidesc->mTicksPerSecond, sizeof(double), &dwByte, nullptr);
					ReadFile(hFile, &anidesc->mNumAniBones, sizeof(_uint), &dwByte, nullptr);
				}

				for (_uint i = 0; i < modelDesc->mNumAnimations; ++i)
				{
					ANIDESC* anidesc = &modelDesc->mAnimations[i];
					anidesc->mAniBones = new ANIBONES[anidesc->mNumAniBones];
					for (_uint j = 0; j < anidesc->mNumAniBones; ++j)
					{
						ANIBONES* anibone = &anidesc->mAniBones[j];


						ReadFile(hFile, anibone->mBoneName, sizeof(char)*MAX_PATH, &dwByte, nullptr);
						ReadFile(hFile, &anibone->mHierarchyNodeIndex, sizeof(_int), &dwByte, nullptr);
						ReadFile(hFile, &anibone->mNumKeyFrames, sizeof(_uint), &dwByte, nullptr);
						anibone->mKeyFrames = new KEYFRAME[anibone->mNumKeyFrames];
						ReadFile(hFile, anibone->mKeyFrames, sizeof(KEYFRAME)* anibone->mNumKeyFrames, &dwByte, nullptr);

						// #TEST
						//Safe_Delete_Array(anibone->mKeyFrames);
					}

				}
			}
			List_Modeldesc.push_front(modelDesc);
		}
		CloseHandle(hFile);
	}

	return S_OK;
}


CLoader * CLoader::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, SCENEID eSceneID)
{
	CLoader* pTempLoader = new CLoader;

	if (FAILED(pTempLoader->Initialize(pDevice,pDeviceContext,eSceneID)))
	{
		Safe_Release(pTempLoader);
		MSGBOX("Failed to Create CLoader");
		return nullptr;
	}

	return pTempLoader;

}

void CLoader::Free()
{
	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
	 
}
 