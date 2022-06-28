#include "stdafx.h"
#include "..\Public\Loader.h"



//////////////////////////////////////////////////////////////////////////

////로비씬//////////////////////////////////////////////////////////////////////
#include "TestObject.h"
#include "AnimationClip.h"
#include "Navigation.h"
#include "TestStaticObject.h"

//////////////////////////////////////////////////////////////////////////



////에디터씬///////////////////////////////////////////////////////////////////////
#include "ESCursor.h"
#include "EditorTerrain.h"
#include "Camera_Editor.h"
#include "StaticMapObject.h"


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
	//TransformMatrix = XMMatrixScaling(0.0001f, 0.0001f, 0.0001f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	//FAILED_CHECK(pGameInstance->Add_Component_Prototype(SCENEID::SCENE_LOBY, TAG_CP(Prototype_Mesh_TestObject),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "TestObject", "Alice.FBX", TransformMatrix)));

	TransformMatrix = XMMatrixScaling(0.0001f, 0.0001f, 0.0001f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	FAILED_CHECK(pGameInstance->Add_Component_Prototype(SCENEID::SCENE_LOBY, TAG_CP(Prototype_Mesh_SkyBox),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "SkyBox", "SkyBox_0.FBX", TransformMatrix)));


#pragma endregion

#pragma  region PROTOTYPE_GAMEOBJECT

	
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
	FAILED_CHECK(pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STAGE4, TAG_CP(Prototype_Mesh_TestObject_Himeko),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "Himeko", "new_Himeko.fbx", TransformMatrix)));

	/* For.Prototype_Component_Navigation */
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE_STAGE4, TEXT("Prototype_Component_Navigation"),
		CNavigation::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/data/NaviMesh/NaviData_Stage_2.dat")))))
		return E_FAIL;

#pragma endregion

#pragma  region PROTOTYPE_GAMEOBJECT

	//FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_TestObject_Himeko), CTestObject::Create(m_pDevice, m_pDeviceContext)));

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

HRESULT CLoader::Load_Scene_Stage6(_bool * _IsClientQuit, CRITICAL_SECTION * _CriSec)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

#pragma region PROTOTYPE_COMPONENT


	_Matrix			TransformMatrix;
	TransformMatrix = XMMatrixScaling(1, 1, 1);// * XMMatrixRotationY(XMConvertToRadians(180));

	// Assimp static
//	FAILED_CHECK(pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Mesh_TestObject),
//		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "city_gate", "city_gate.fbx", TransformMatrix)))

	// Assimp Dynamic
//	FAILED_CHECK(pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Mesh_Player),
//		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "crea_Snot_a", "crea_Snot_a.FBX", TransformMatrix)));

	// NoAssimp
	FAILED_CHECK(Load_Model_DatFile());

#pragma endregion

#pragma  region PROTOTYPE_GAMEOBJECT

	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_TestObject), CTestStaticObject::Create(m_pDevice, m_pDeviceContext)));

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
	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	/* 테스트 알게락 */
	FAILED_CHECK(pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Mesh_AlgaeRock_Ledge),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "TestObject", "AlgaeRock_Ledge.FBX", TransformMatrix)));



#pragma endregion

#pragma  region PROTOTYPE_GAMEOBJECT

	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_EditorCursor), CESCursor::Create(m_pDevice, m_pDeviceContext)));

	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_Camera_Editor), CCamera_Editor::Create(m_pDevice, m_pDeviceContext)));
	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_EditorTerrain), CEditorTerrain::Create(m_pDevice, m_pDeviceContext)));
	//FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(L"ProtoType_EditRendererUI", CRendererEditSceneUI::Create(m_pDevice, m_pDeviceContext)));

	FAILED_CHECK(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_StaticMapObject), 	CStaticMapObject::Create(m_pDevice, m_pDeviceContext)));
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
	

	// 데이터 파일로 assimp 없이 모델 초기화 / 데이터 초기화
	CGameInstance* pGameInstance = GetSingle(CGameInstance);
	_Matrix TransformMatrix = XMMatrixScaling(1, 1, 1);

	// 파일 경로
	auto static_dat = GetSingle(CGameInstance)->Load_ExtensionList(STR_FILEPATH_RESOURCE_FBXDAT_L, "stc");
	auto dynamic_dat = GetSingle(CGameInstance)->Load_ExtensionList(STR_FILEPATH_RESOURCE_FBXDAT_L, "dyn");


	// MODELDESC / LOAD 함수
	list<MODELDESC*> List_ModelCreateTest;
//	Load_Model(static_dat, List_ModelCreateTest);
	Load_Model(dynamic_dat, List_ModelCreateTest);

	// 모델 컴포넌트 생성
	// For. Test

	for (auto& modeldesc : List_ModelCreateTest)
	{
		FAILED_CHECK(pGameInstance->Add_Component_Prototype(
			SCENEID::SCENE_STATIC,
			TAG_CP(Prototype_Mesh_Player),
			CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, modeldesc, TransformMatrix)));

		//CModel* DebugModel = CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, modeldesc, TransformMatrix);


		//FAILED_CHECK(pGameInstance->Add_Component_Prototype(
		//	SCENEID::SCENE_STATIC,
		//	TAG_CP(Prototype_Mesh_TEST_STATIC),
		//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, modeldesc, TransformMatrix)));

		// CModel* DebugModel = CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, modeldesc, TransformMatrix);
		int Debug = 5;
	}

	return S_OK;

}

HRESULT CLoader::Load_Model(const list<MYFILEPATH*>& pathlist, list<MODELDESC*>& List_Modeldesc)
{
	// #LOAD 클라이언트 로드 함수

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
			modelDesc = NEW MODELDESC;

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

			// 할당
			if (modelDesc->mNumMeshes != 0)
				modelDesc->mMeshDesc = NEW MESHDESC[modelDesc->mNumMeshes];

			if (modelDesc->mNumMaterials != 0)
				modelDesc->mMaterials = NEW MATDESC[modelDesc->mNumMaterials];

			if (ModelType == CModel::TYPE_ANIM)
			{
				if (modelDesc->mNumBones != 0)
					modelDesc->mBones = NEW BONEDESC[modelDesc->mNumBones];

				if (modelDesc->mNumAnimations != 0)
					modelDesc->mAnimations = NEW ANIDESC[modelDesc->mNumAnimations];
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

				meshdesc->mVertices = NEW _float3[meshdesc->mNumVertices];
				meshdesc->mNormals = NEW _float3[meshdesc->mNumVertices];
				meshdesc->mTangents = NEW _float3[meshdesc->mNumVertices];
				meshdesc->mUV = NEW _float2[meshdesc->mNumVertices];
				meshdesc->mFaces = NEW FACEINDICES32[meshdesc->mNumFaces];

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

				// UV 추가
				ReadFile(hFile, meshdesc->mUV, sizeof(_float2)*meshdesc->mNumVertices, &dwByte, nullptr);
				if (dwByte == 0)
					return E_FAIL;

				// 여러개 UV 추가시 
				//for (_uint j = 0; j < AI_MAX_NUMBER_OF_TEXTURECOORDS; ++j)
				//{
				//	_float3* cash = NEW _float3[meshdesc->mNumVertices];


				//	ReadFile(hFile, cash, sizeof(_float3)*meshdesc->mNumVertices, &dwByte, nullptr);
				//	if (cash->x == 0)
				//	{
				//		meshdesc->mTextureCoords[j] = nullptr;
				//		Safe_Delete_Array(cash);
				//	}
				//	else
				//	{
				//		meshdesc->mTextureCoords[j] = NEW _float3[meshdesc->mNumVertices];
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
						// 뼈 인덱스 리스트
						meshdesc->mAffectingBones = NEW _uint[NumAffectingBones];
						meshdesc->mMeshBones = NEW MESHBONEDESC[NumAffectingBones];

						ReadFile(hFile, meshdesc->mAffectingBones, sizeof(_uint)*NumAffectingBones, &dwByte, nullptr);
						// 뼈 Weight
						for (int bone = 0; bone < NumAffectingBones; ++bone)
						{
							ReadFile(hFile, &meshdesc->mMeshBones[bone].mNumWeights, sizeof(_uint), &dwByte, nullptr);
							_uint NumWeight = meshdesc->mMeshBones[bone].mNumWeights;
							if (NumWeight == 0)
								continue;

							meshdesc->mMeshBones[bone].mAiWeights = NEW aiVertexWeight[NumWeight];

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
					anidesc->mAniBones = NEW ANIBONES[anidesc->mNumAniBones];
					for (_uint j = 0; j < anidesc->mNumAniBones; ++j)
					{
						ANIBONES* anibone = &anidesc->mAniBones[j];


						ReadFile(hFile, anibone->mBoneName, sizeof(char)*MAX_PATH, &dwByte, nullptr);
						ReadFile(hFile, &anibone->mHierarchyNodeIndex, sizeof(_int), &dwByte, nullptr);
						ReadFile(hFile, &anibone->mNumKeyFrames, sizeof(_uint), &dwByte, nullptr);
						anibone->mKeyFrames = NEW KEYFRAME[anibone->mNumKeyFrames];
						ReadFile(hFile, anibone->mKeyFrames, sizeof(KEYFRAME)* anibone->mNumKeyFrames, &dwByte, nullptr);

						if (dwByte == 0)
							return E_FAIL;

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
 