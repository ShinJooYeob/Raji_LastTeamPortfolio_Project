#include "stdafx.h"
#include "AssimpCreateMgr.h"


IMPLEMENT_SINGLETON(CAssimpCreateMgr);

CAssimpCreateMgr::CAssimpCreateMgr()
{
}

HRESULT CAssimpCreateMgr::Initalize(ID3D11Device* d, ID3D11DeviceContext* c)
{
	m_pDevice = d;
	m_pDeviceContext = c;
	Safe_AddRef(d);
	Safe_AddRef(c);



	return S_OK;
}

HRESULT CAssimpCreateMgr::Load_ALL_Modele()
{
	// ��� �� ������Ʈ ����
	FAILED_CHECK(Load_Model_DatFile());
	return S_OK;
}

const wchar_t * CAssimpCreateMgr::GetName_Iter_Plus()
{
	++mCurrent_NameIter;
	if (mCurrent_NameIter == mList_Name_ModelCom.end())
		mCurrent_NameIter = mList_Name_ModelCom.begin();

	return (*mCurrent_NameIter);
}

HRESULT CAssimpCreateMgr::Load_Model_DatFile()
{
	// ���� ����Ʈ
	mFile_StaticList = GetSingle(CGameInstance)->Load_ExtensionList(STR_FILEPATH_RESOURCE_FBXDAT_L, "stc");
	mFile_DynamicList = GetSingle(CGameInstance)->Load_ExtensionList(STR_FILEPATH_RESOURCE_FBXDAT_L, "dyn");


	GetSingle(CUtilityMgr)->Start_DebugTimer(CUtilityMgr::DEBUGTIMER_1);

	// Load ModelDesc
	FAILED_CHECK(Load_ModelMap(mFile_StaticList, mMap_StaticModelDesc));
	FAILED_CHECK(Load_ModelMap(mFile_DynamicList, mMap_DynamicModelDesc));

	// Create ModelCom
	_Matrix			TransformMatrix_dynamic;
	TransformMatrix_dynamic = XMMatrixScaling(0.0001f, 0.0001f, 0.0001f) *XMMatrixRotationY(XMConvertToRadians(180.0f));

	_Matrix			TransformMatrix_static;
	TransformMatrix_static = XMMatrixScaling(0.01f, 0.01f, 0.01f);


	SCENEID sceneID = SCENE_STATIC;
	FAILED_CHECK(Create_ModelCom(mMap_StaticModelDesc, sceneID, CModel::TYPE_NONANIM, TransformMatrix_static));
	FAILED_CHECK(Create_ModelCom(mMap_DynamicModelDesc, sceneID, CModel::TYPE_ANIM, TransformMatrix_dynamic));

	mCurrent_NameIter = mList_Name_ModelCom.begin();

	GetSingle(CUtilityMgr)->End_DebugTimer(CUtilityMgr::DEBUGTIMER_1, L"Assimp ModelLoad");
	
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

			// 
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

			// 
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
			Map_Modeldesc.emplace(modelDesc->mFBXFileName,modelDesc);
		}
		CloseHandle(hFile);
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

		mList_Name_ModelCom.push_front(pair.first);
	}
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


void CAssimpCreateMgr::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);

	Safe_Delete_List(mFile_StaticList);
	Safe_Delete_List(mFile_DynamicList);

	//Safe_Delete_List(mList_ModelStatic);
	//Safe_Delete_List(mList_ModelDynamic);

	Safe_Delete_Map(mMap_StaticModelDesc);
	Safe_Delete_Map(mMap_DynamicModelDesc);


}
