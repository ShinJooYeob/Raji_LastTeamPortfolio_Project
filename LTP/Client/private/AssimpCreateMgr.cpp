#include "stdafx.h"
#include "AssimpCreateMgr.h"

IMPLEMENT_SINGLETON(CAssimpCreateMgr);

static _bool				mThreadLoaderCheck = false;


_uint CALLBACK StaticLoadThread(void* _Prameter)
{
	// ÄÄÆ÷³ÍÆ® »ý¼ºÀ» ³ª´²¼­ ÁøÇà½ÃÅ²´Ù.
	// 1. ÀüÃ¼ÀÇ ¹ÝÀ» ³ª´²¼­ ÁøÇà

	THREADARG tThreadArg{};
	memcpy(&tThreadArg, _Prameter, sizeof(THREADARG));
	delete _Prameter;
	CAssimpCreateMgr* pAssimpCreateMgr = (CAssimpCreateMgr*)tThreadArg.pArg;
	CAssimpCreateMgr::ASTHREADDESC* AssimpDesc =  (CAssimpCreateMgr::ASTHREADDESC*)tThreadArg.Desc;
	
	FAILED_CHECK(pAssimpCreateMgr->Processing_LoadStatic(tThreadArg.IsClientQuit, tThreadArg.CriSec, *AssimpDesc));

	return 0;
}

CAssimpCreateMgr::CAssimpCreateMgr()
{
}

HRESULT CAssimpCreateMgr::Initalize(ID3D11Device* d, ID3D11DeviceContext* c)
{
	m_pDevice = d;
	m_pDeviceContext = c;
	Safe_AddRef(d);
	Safe_AddRef(c);

	Init_ModelName_FileList();
	return S_OK;
}

HRESULT CAssimpCreateMgr::Load_ALL_Model(_fMatrix staticDefault, _fMatrix dynamicDefault)
{
	// ¸ðµç ¸ðµ¨ ÄÄÆ÷³ÍÆ® »ý¼º
	FAILED_CHECK(Load_Model_DatFile_All(staticDefault,dynamicDefault));


//	FAILED_CHECK(Free_VertexData());

	return S_OK;
}



HRESULT CAssimpCreateMgr::Load_Model_One_ByFBXName(const wchar_t * fbxName, _fMatrix DefaultMat)
{
	FAILED_CHECK(Load_Model_DatFile_One(fbxName,DefaultMat));
	return S_OK;
}

const wchar_t * CAssimpCreateMgr::GetName_Iter_Plus()
{
	++mCurrent_NameIter;
	if (mCurrent_NameIter == mList_CreateModelName.end())
		mCurrent_NameIter = mList_CreateModelName.begin();

	return (*mCurrent_NameIter);
}

HRESULT CAssimpCreateMgr::Load_Model_DatFile_One(const wchar_t* fbxName, _fMatrix DefaultMat)
{
	// »ý¼ºÇÒ FBX ÀÌ¸§À¸·Î »ý¼º
	for (auto iter: mList_CreateModelName)
	{
		if (lstrcmp(iter, fbxName) == 0)
		{
			
#ifdef _DEBUG
			wstring log = L"AlReady Compoenet: fbxName" + wstring(fbxName) + L"\n";
			OutputDebugStringW(log.c_str());
#endif
			return S_FALSE;
		}
	}
	GetSingle(CUtilityMgr)->Start_DebugTimer(CUtilityMgr::DEBUGTIMER_1);

	// Æ¯Á¤ ¸ðµ¨ Desc¿¡ ÀúÀå ÈÄ ¸ðµ¨ »ý¼º
	FAILED_CHECK(Load_ModelFBXName_CreateModel(fbxName, DefaultMat));
	mCurrent_NameIter = mList_CreateModelName.begin();
//	GetSingle(CUtilityMgr)->End_DebugTimer(CUtilityMgr::DEBUGTIMER_1, L"Assimp ModelLoad");

	return S_OK;
}

HRESULT CAssimpCreateMgr::Init_ModelName_FileList()
{
	// ÆÄÀÏ ¸®½ºÆ® ÃÊ±âÈ­
	mList_DataFIle_Static = GetSingle(CGameInstance)->Load_ExtensionList(STR_FILEPATH_RESOURCE_FBXDAT_L, "stc");
	mList_DataFIle_Dynamic = GetSingle(CGameInstance)->Load_ExtensionList(STR_FILEPATH_RESOURCE_FBXDAT_L, "dyn");
	return S_OK;
}

HRESULT CAssimpCreateMgr::Load_Model_DatFile_All(_fMatrix staticDefault, _fMatrix dynamicDefault)
{
	// ¸ðµç ¸ðµ¨ ·Îµå
	GetSingle(CUtilityMgr)->Start_DebugTimer(CUtilityMgr::DEBUGTIMER_1);

	// Load ModelDesc
	FAILED_CHECK(Load_ModelMap(mList_DataFIle_Static, mMap_StaticModelDesc));
	FAILED_CHECK(Load_ModelMap(mList_DataFIle_Dynamic, mMap_DynamicModelDesc));

	SCENEID sceneID = SCENE_STATIC;
	FAILED_CHECK(Create_ModelCom(mMap_StaticModelDesc, sceneID, CModel::TYPE_NONANIM, staticDefault));
	FAILED_CHECK(Create_ModelCom(mMap_DynamicModelDesc, sceneID, CModel::TYPE_ANIM, dynamicDefault));

//	mCurrent_NameIter = mList_CreateModelName.begin();

	GetSingle(CUtilityMgr)->End_DebugTimer(CUtilityMgr::DEBUGTIMER_1, L"Assimp ModelLoad");


	return S_OK;

}


HRESULT CAssimpCreateMgr::Load_Model_DatFile_All_Thread(_fMatrix staticDefault, _fMatrix dynamicDefault)
{
	// #Threading 
	// ¸ðµç ¸ðµ¨ ·Îµå


//	GetSingle(CUtilityMgr)->Start_DebugTimer(CUtilityMgr::DEBUGTIMER_1);
//
//	// Load ModelDesc
//	FAILED_CHECK(Load_ModelMap(mList_DataFIle_Static, mMap_StaticModelDesc));
//	FAILED_CHECK(Load_ModelMap(mList_DataFIle_Dynamic, mMap_DynamicModelDesc));
//	GetSingle(CUtilityMgr)->End_DebugTimer(CUtilityMgr::DEBUGTIMER_1, L"Assimp FileLoad");
//
//	SCENEID sceneID = SCENE_STATIC;
//	// Á¤Á¡À» »ý¼ºÇÏ´Â ºÎºÐÀÌ ¿À·¡°É¸°´Ù.
//	GetSingle(CUtilityMgr)->Start_DebugTimer(CUtilityMgr::DEBUGTIMER_1);
//
//	// Thread
//	
////	_uint count = 100;
//
//	//for (_uint startPos = 0; startPos < mMap_StaticModelDesc.size(); startPos += count)
//	//{
//	//	ASTHREADDESC asimpDesc;// = NEW ASTHREADDESC;
//	//	asimpDesc.mapdesc = &mMap_StaticModelDesc;
//	//	asimpDesc.DefaultMat = staticDefault;
//	//	asimpDesc.Count = count-1;
//
//	//	asimpDesc.startPos = startPos;
//	//	GetSingle(CGameInstance)->PlayThread(StaticLoadThread, this, &asimpDesc);
//	//}
//
//	_uint half = mMap_StaticModelDesc.size() / 2;
//
//	ASTHREADDESC asimpDesc1;// = NEW ASTHREADDESC;
//	asimpDesc1.mapdesc = &mMap_StaticModelDesc;
//	asimpDesc1.DefaultMat = staticDefault;
//	asimpDesc1.Count = half;
//
//	asimpDesc1.startPos = 0;
//	GetSingle(CGameInstance)->PlayThread(StaticLoadThread, this, &asimpDesc1);
//	
//	ASTHREADDESC asimpDesc2;// = NEW ASTHREADDESC;
//	asimpDesc2.mapdesc = &mMap_StaticModelDesc;
//	asimpDesc2.DefaultMat = staticDefault;
//	asimpDesc2.Count = half;
//
//	asimpDesc2.startPos = half;
//	GetSingle(CGameInstance)->PlayThread(StaticLoadThread, this, &asimpDesc2);
//
//
//
//
////	asimpDesc.startPos = 0;
////	GetSingle(CGameInstance)->PlayThread(StaticLoadThread, this, &asimpDesc);
////	asimpDesc.startPos = 1001;
////	GetSingle(CGameInstance)->PlayThread(StaticLoadThread, this, &asimpDesc);
//
//
//
//	// Base
////	FAILED_CHECK(Create_ModelCom(mMap_StaticModelDesc, sceneID, CModel::TYPE_NONANIM, staticDefault));
//
//	while (mThreadLoaderCheck == false)
//	{
//		// ·ÎµùÁß
//		if (mList_CreateModelName.size() >= mMap_StaticModelDesc.size()-1)
//			mThreadLoaderCheck = true;
//	}
//	FAILED_CHECK(Create_ModelCom(mMap_DynamicModelDesc, sceneID, CModel::TYPE_ANIM, dynamicDefault));
//	mList_CreateModelName;
//	GetSingle(CUtilityMgr)->End_DebugTimer(CUtilityMgr::DEBUGTIMER_1, L"Assimp ComLoad");

	return S_OK;

}




HRESULT CAssimpCreateMgr::Load_ModelList(const list<MYFILEPATH*>& pathlist, list<MODELDESC*>& List_Modeldesc)
{
	// #LOAD For NoAssimp

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

			// 
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

				// UV ì¶”ê?
				ReadFile(hFile, meshdesc->mUV, sizeof(_float2)*meshdesc->mNumVertices, &dwByte, nullptr);
				if (dwByte == 0)
					return E_FAIL;

				// ?¬ëŸ¬ê°?UV ì¶”ê???
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
						// ë¼??¸ë±??ë¦¬ìŠ¤??
						meshdesc->mAffectingBones = NEW _uint[NumAffectingBones];
						meshdesc->mMeshBones = NEW MESHBONEDESC[NumAffectingBones];

						ReadFile(hFile, meshdesc->mAffectingBones, sizeof(_uint)*NumAffectingBones, &dwByte, nullptr);
						// ë¼?Weight
						for (_uint bone = 0; bone < NumAffectingBones; ++bone)
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

HRESULT CAssimpCreateMgr::Load_ModelMap(const list<MYFILEPATH*>& pathlist, map<const wchar_t*, MODELDESC*>& Map_Modeldesc)
{

	// #LOAD For NoAssimp

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

			// 
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

				// UV ì¶”ê?
				ReadFile(hFile, meshdesc->mUV, sizeof(_float2)*meshdesc->mNumVertices, &dwByte, nullptr);
				if (dwByte == 0)
					return E_FAIL;

				// ?¬ëŸ¬ê°?UV ì¶”ê???
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
						// ë¼??¸ë±??ë¦¬ìŠ¤??
						meshdesc->mAffectingBones = NEW _uint[NumAffectingBones];
						meshdesc->mMeshBones = NEW MESHBONEDESC[NumAffectingBones];

						ReadFile(hFile, meshdesc->mAffectingBones, sizeof(_uint)*NumAffectingBones, &dwByte, nullptr);
						// ë¼?Weight
						for (_uint bone = 0; bone < NumAffectingBones; ++bone)
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

						// #TEST
						//Safe_Delete_Array(anibone->mKeyFrames);
					}

				}
			}
			Map_Modeldesc.emplace(modelDesc->mFBXFileName,modelDesc);
		}
		CloseHandle(hFile);
	}
	return S_OK;
}

HRESULT CAssimpCreateMgr::Load_ModelFBXName_CreateModel(const wchar_t * fbxName, _fMatrix Default)
{
	// Æ¯Á¤ ¸ðµ¨ ÄÄÆ÷³ÍÆ® ÃÊ±âÈ­

	wstring fullpath = L"";
	wstring FileName = L"";
	wstring SearchName = fbxName;

	MODELDESC* modelDesc = NEW MODELDESC;
	// 1.ÆÄÀÏ Ã£±â

	bool isSearch = false;
	_ulong			dwByte = 0;
	for (auto& path : mList_DataFIle_Dynamic)
	{
		if (isSearch)
			break;

		fullpath = path->FullPath;
		FileName = path->FileName;
		HANDLE			hFile = CreateFile(fullpath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (0 == hFile)
			return E_FAIL;

		// UINT
		ReadFile(hFile, modelDesc->mFBXFullPath, sizeof(wchar_t)*MAX_PATH, &dwByte, nullptr);
		ReadFile(hFile, modelDesc->mFBXFileName, sizeof(wchar_t)*MAX_PATH, &dwByte, nullptr);

		if (lstrcmp(modelDesc->mFBXFileName, fbxName) == 0)
		{
			isSearch = true;
		}
		CloseHandle(hFile);
	}

	if (isSearch == false)
	{
		for (auto& path : mList_DataFIle_Static)
		{
			if (isSearch)
				break;

			fullpath = path->FullPath;
			FileName = path->FileName;
			HANDLE			hFile = CreateFile(fullpath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
			if (0 == hFile)
				return E_FAIL;

			// UINT
			ReadFile(hFile, modelDesc->mFBXFullPath, sizeof(wchar_t)*MAX_PATH, &dwByte, nullptr);
			ReadFile(hFile, modelDesc->mFBXFileName, sizeof(wchar_t)*MAX_PATH, &dwByte, nullptr);

			if (lstrcmp(modelDesc->mFBXFileName, fbxName) == 0)
			{
				isSearch = true;
			}
			CloseHandle(hFile);
		}

	}

	if (isSearch == false)
		return E_FAIL;


	HANDLE			hFile = CreateFile(fullpath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;
	{

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

		// 
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

			// UV ì¶”ê?
			ReadFile(hFile, meshdesc->mUV, sizeof(_float2)*meshdesc->mNumVertices, &dwByte, nullptr);
			if (dwByte == 0)
				return E_FAIL;

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
					// ë¼??¸ë±??ë¦¬ìŠ¤??
					meshdesc->mAffectingBones = NEW _uint[NumAffectingBones];
					meshdesc->mMeshBones = NEW MESHBONEDESC[NumAffectingBones];

					ReadFile(hFile, meshdesc->mAffectingBones, sizeof(_uint)*NumAffectingBones, &dwByte, nullptr);
					// ë¼?Weight
					for (_uint bone = 0; bone < NumAffectingBones; ++bone)
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

				}

			}
		}

		CloseHandle(hFile);

		if (modelDesc->mModelType == CModel::TYPE_NONANIM)
		{
			mList_CreateModelName.push_front(modelDesc->mFBXFileName);
			mMap_StaticModelDesc.emplace(modelDesc->mFBXFileName, modelDesc);

			FAILED_CHECK(GetSingle(CGameInstance)->Add_Component_Prototype(
				SCENEID::SCENE_STATIC,
				modelDesc->mFBXFileName,
				CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, modelDesc, Default)));
		}

		else if (modelDesc->mModelType == CModel::TYPE_ANIM)
		{
			mList_CreateModelName.push_front(modelDesc->mFBXFileName);
			mMap_DynamicModelDesc.emplace(modelDesc->mFBXFileName, modelDesc);

			FAILED_CHECK(GetSingle(CGameInstance)->Add_Component_Prototype(
				SCENEID::SCENE_STATIC,
				modelDesc->mFBXFileName,
				CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, modelDesc, Default)));

		}
	}

	return S_OK;
}



HRESULT CAssimpCreateMgr::Create_ModelCom(map<const wchar_t*, MODELDESC*>& map, SCENEID sceneid, CModel::MODELTYPE type, _Matrix defaultMat)
{
	if (map.empty())
		return S_FALSE;

	if (m_pDevice == nullptr || m_pDeviceContext == nullptr)
		return E_FAIL;

	// #CREATE ModelCom 

	for (auto& pair : map)
	{
		FAILED_CHECK(GetSingle(CGameInstance)->Add_Component_Prototype(
			sceneid,
			pair.first,
			CModel::Create(m_pDevice, m_pDeviceContext, type, pair.second, defaultMat)));

		mList_CreateModelName.push_front(pair.first);
	}
	return S_OK;
}

HRESULT CAssimpCreateMgr::Create_ModelCom_StaticThread(map<const wchar_t*, MODELDESC*>& modelMap, SCENEID sceneid,_Matrix defaultMat, _uint start, _uint count, CRITICAL_SECTION*_CriSec)
{

	if (modelMap.empty())
		return S_FALSE;

	if (m_pDevice == nullptr || m_pDeviceContext == nullptr)
		return E_FAIL;
	if (start >= modelMap.size())
		return E_FAIL;
	if (count == 0)
		return S_OK;
	
	if((count + start)>= modelMap.size())
	{
		count = _uint(modelMap.size() - start);
	}

	   
	// ½ÃÀÛ À§Ä¡¿¡¼­ count ¸¸Å­¸¸ ·Îµå
	int startPos = 0;
	auto iter_Begin = modelMap.begin();
	map<const wchar_t*, MODELDESC*>::iterator startIter = modelMap.begin();

	for (; iter_Begin != modelMap.end();iter_Begin++, startPos++)
	{
		if (startPos == start)
		{
			startIter = iter_Begin;
			break;
		}
	}

	if (iter_Begin == modelMap.end())
		return S_OK;


	// ÇØ´ç °³¼ö¸¸Å­¸¸ ·Îµå
	for (_uint itercount = 0; startIter!=modelMap.end(); startIter++, itercount++)
	{
		if (itercount >= count)
		{
			break;
		}

		FAILED_CHECK(GetSingle(CGameInstance)->Add_Component_Prototype(
			sceneid,
			startIter->first,
			CModel::Create(m_pDevice, m_pDeviceContext, CModel::MODELTYPE::TYPE_NONANIM, startIter->second, defaultMat)));

		mList_CreateModelName.push_front(startIter->first);
	}

	/*for (auto& pair : modelMap)
	{
		FAILED_CHECK(GetSingle(CGameInstance)->Add_Component_Prototype(
			sceneid,
			pair.first,
			CModel::Create(m_pDevice, m_pDeviceContext, CModel::MODELTYPE::TYPE_NONANIM, pair.second, defaultMat)));

		mList_CreateModelName.push_front(pair.first);
	}*/
	return S_OK;
}

HRESULT CAssimpCreateMgr::Free_VertexData()
{
	for (auto desc: mMap_StaticModelDesc)
	{
		Safe_Delete_Array(desc.second->mMeshDesc);
		desc.second->mNumMeshes = 0;

	}
	for (auto desc: mMap_DynamicModelDesc)
	{
		Safe_Delete_Array(desc.second->mMeshDesc);
		desc.second->mNumMeshes = 0;
	}

	return S_OK;
}

HRESULT CAssimpCreateMgr::Free_VertexData_STATIC()
{
	// TEST
	for (auto desc : mMap_StaticModelDesc)
	{
		Safe_Delete_Array(desc.second->mMeshDesc);
		desc.second->mNumMeshes = 0;

	}
	mMap_StaticModelDesc.clear();

	return S_OK;

}
//HRESULT CAssimpCreateMgr::Create_ModelCom(const list<MYFILEPATH*>& NameList, const list<MODELDESC*>& DescList,
//	SCENEID sceneid, CModel::MODELTYPE type, _Matrix defaultMat)
//{
//	if (m_pDevice == nullptr || m_pDeviceContext == nullptr)
//		return E_FAIL;
//	if (NameList.empty() || DescList.empty())
//		return S_FALSE;
//
//	// #CREATE ModelCom 
//	auto dynamiciter = NameList.begin();
//	for (auto& modeldesc : DescList)
//	{
//		FAILED_CHECK(GetSingle(CGameInstance)->Add_Component_Prototype(
//			sceneid,
//			(*dynamiciter)->FileName,
//			CModel::Create(m_pDevice, m_pDeviceContext, type, modeldesc, defaultMat)));
//
//		dynamiciter++;
//		if (dynamiciter == NameList.end())
//			break;
//	}
//
//	/*auto staticiter = static_dat.begin();
//	for (auto& modeldesc : List_ModelStatic)
//	{
//		FAILED_CHECK(pGameInstance->Add_Component_Prototype(
//			SCENEID::SCENE_STATIC,
//			(*staticiter)->FileName,
//			CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, modeldesc, TransformMatrix)));
//
//		staticiter++;
//		if (staticiter == static_dat.end())
//			break;
//	}*/
//}


HRESULT CAssimpCreateMgr::Processing_LoadStatic(_bool * _IsClientQuit, CRITICAL_SECTION * _CriSec, ASTHREADDESC desc)
{

	// Static °´Ã¼ ·Îµå 
	if (desc.mapdesc->empty())
		return S_FALSE;

	if (m_pDevice == nullptr || m_pDeviceContext == nullptr)
		return E_FAIL;

	// #CREATE ModelCom 

	// ÇØ´ç °³¼ö¸¸ ·Îµå
	EnterCriticalSection(_CriSec);
	FAILED_CHECK(Create_ModelCom_StaticThread(*desc.mapdesc, SCENE_STATIC, desc.DefaultMat.XMatrix(),
		desc.startPos,desc.Count,_CriSec));
	LeaveCriticalSection(_CriSec);


	desc.isOk = true;

	return S_OK;
}

HRESULT CAssimpCreateMgr::DataFree_TEST()
{

	for (auto desc : mMap_StaticModelDesc)
	{
		Safe_Delete_Array(desc.second->mMeshDesc);
		desc.second->mNumMeshes = 0;
	}

	mMap_StaticModelDesc.clear();

	Safe_Delete_List(mList_DataFIle_Static);
	Safe_Delete_Map(mMap_StaticModelDesc);


	return S_OK;

}

void CAssimpCreateMgr::Free()
{

	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);

	Free_VertexData();

	Safe_Delete_List(mList_DataFIle_Static);
	Safe_Delete_List(mList_DataFIle_Dynamic);

	//Safe_Delete_List(mList_ModelStatic);
	//Safe_Delete_List(mList_ModelDynamic);

	Safe_Delete_Map(mMap_StaticModelDesc);
	Safe_Delete_Map(mMap_DynamicModelDesc);


}
