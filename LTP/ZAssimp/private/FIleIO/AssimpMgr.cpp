#include "stdafx.h"
#include "FIleIO/AssimpMgr.h"
#include "HierarchyNode.h"
#include "AnimationClip.h"
#include "ClipBone.h"


IMPLEMENT_SINGLETON(CAssimpMgr);

HRESULT CAssimpMgr::Initialize(ID3D11Device* d, ID3D11DeviceContext* c)
{
	if (nullptr == d || nullptr == c)
		return E_FAIL;

	mDevice = d;
	mDeviceContext = c;

	Safe_AddRef(d);
	Safe_AddRef(c);

	mMap_ModelDesc.clear();
	return S_OK;
}

HRESULT CAssimpMgr::ReleaseMap()
{
	for (auto& pair : mMap_ModelDesc)
	{
		Safe_Delete(pair.second);
		
	}

	mMap_ModelDesc.clear();


	return S_OK;
}

HRESULT CAssimpMgr::Initialize_FbxSetting()
{
	// 텍스트 파일읽고 해당 경로의 모델 컴포넌트로 읽음

	// 1. 경로 받기
	CModel::MODEL_TOOLPATH_FLAG = true;

	auto Path_Static	= GetSingle(CGameInstance)->Load_ExtensionList(STR_FILEPATH_RESOURCE_STATICDAT_L, "fbx");
	auto Path_Dynamic	= GetSingle(CGameInstance)->Load_ExtensionList(STR_FILEPATH_RESOURCE_DYNAMICDAT_L, "fbx");
	
	// 2. 모델 컴포넌트 생성
	for (auto& path : Path_Static)
	{
		wstring wPath = path->FullPath;
		wstring wName = path->FileName;

		string fileFullpath = CHelperClass::Convert_Wstr2str(wPath);
		string fileName = CHelperClass::Convert_Wstr2str(wName);

		CModel* ModelCash = CModel::Create(mDevice, mDeviceContext, CModel::TYPE_NONANIM, fileFullpath.c_str(), fileName.c_str());

		// 3. 로드된 Scene으로 필요 데이터 복사
		CopyData_MODELDESC(wPath, wName, ModelCash, CModel::TYPE_NONANIM);
		Safe_Release(ModelCash);
	}

	for (auto& path : Path_Dynamic)
	{
		wstring wPath = path->FullPath;
		wstring wName = path->FileName;

		string fileFullpath = CHelperClass::Convert_Wstr2str(wPath);
		string fileName = CHelperClass::Convert_Wstr2str(wName);

		CModel* ModelCash = CModel::Create(mDevice, mDeviceContext, CModel::TYPE_ANIM, fileFullpath.c_str(), fileName.c_str());
		
		// 3. 로드된 Scene으로 필요 데이터 복사
		CopyData_MODELDESC(wPath, wName, ModelCash, CModel::TYPE_ANIM);

		Safe_Release(ModelCash);
	}

	// 4. 데이터 Dat 파일로 저장

	for (auto& pair: mMap_ModelDesc)
	{		
		wstring name = pair.first;
		_uint length = name.length();
		wstring Name = name.substr(0, length - 4);
		Save_MODELDESC(STR_FILEPATH_RESOURCE_DAT_L, Name, pair.second);
		Safe_Delete(pair.second);
	}

	Safe_Delete_List(Path_Static);
	Safe_Delete_List(Path_Dynamic);

	return S_OK;
}


HRESULT CAssimpMgr::Save_MODELDESC(wstring FolderPath, wstring filename, MODELDESC* modelDesc)
{
	// #SAVE DESC 정보 저장
	// 확장자로 나누기 // ㅇ 
	_ulong			dwByte = 0;

	//	wstring ExeName = L".dat";
	wstring path = L"";
	if (modelDesc->mModelType == CModel::TYPE_ANIM)
		path = FolderPath + L"\\" + filename + L".dyn";
	else
		path = FolderPath + L"\\" + filename + L".stc";

	HANDLE			hFile = CreateFile(path.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	{
		// UINT
		WriteFile(hFile, modelDesc->mFBXFullPath, sizeof(wchar_t)*MAX_PATH, &dwByte, nullptr);
		WriteFile(hFile, modelDesc->mFBXFileName, sizeof(wchar_t)*MAX_PATH, &dwByte, nullptr);

		WriteFile(hFile, &modelDesc->mModelType, sizeof(_uint), &dwByte, nullptr);
		WriteFile(hFile, &modelDesc->mNumMeshes, sizeof(_uint), &dwByte, nullptr);
		WriteFile(hFile, &modelDesc->mNumMaterials, sizeof(_uint), &dwByte, nullptr);

		if (modelDesc->mModelType == CModel::TYPE_ANIM)
		{
			WriteFile(hFile, &modelDesc->mNumBones, sizeof(_uint), &dwByte, nullptr);
			WriteFile(hFile, &modelDesc->mNumAnimations, sizeof(_uint), &dwByte, nullptr);
		}

		// MESH
		for (_uint i = 0; i < modelDesc->mNumMeshes; ++i)
		{
			MESHDESC* meshdesc = &modelDesc->mMeshDesc[i];

			// uint
			WriteFile(hFile, &meshdesc->mPrimitiveTypes, sizeof(_uint), &dwByte, nullptr);
			WriteFile(hFile, &meshdesc->mNumVertices, sizeof(_uint), &dwByte, nullptr);
			WriteFile(hFile, &meshdesc->mNumFaces, sizeof(_uint), &dwByte, nullptr);
			WriteFile(hFile, &meshdesc->mMaterialIndex, sizeof(_uint), &dwByte, nullptr);

			if (modelDesc->mModelType == CModel::TYPE_ANIM)
			{
				WriteFile(hFile, &meshdesc->mNumAffectingBones, sizeof(_uint), &dwByte, nullptr);
			}


			// VTX
			WriteFile(hFile, meshdesc->mVertices, sizeof(_float3)*meshdesc->mNumVertices, &dwByte, nullptr);
			if (dwByte == 0)
				return E_FAIL;

			WriteFile(hFile, meshdesc->mNormals, sizeof(_float3) *meshdesc->mNumVertices, &dwByte, nullptr);
			if (dwByte == 0)
				return E_FAIL;

			WriteFile(hFile, meshdesc->mTangents, sizeof(_float3)*meshdesc->mNumVertices, &dwByte, nullptr);
			if (dwByte == 0)
				return E_FAIL;

			// UV
			WriteFile(hFile, meshdesc->mUV, sizeof(_float2)*meshdesc->mNumVertices, &dwByte, nullptr);


			//for (_uint j = 0; j < AI_MAX_NUMBER_OF_TEXTURECOORDS; ++j)
			//{
			//	WriteFile(hFile, meshdesc->mTextureCoords[j], sizeof(_float3)*meshdesc->mNumVertices, &dwByte, nullptr);
			//}

			// INDEX
			WriteFile(hFile, meshdesc->mFaces, sizeof(FACEINDICES32)*meshdesc->mNumFaces, &dwByte, nullptr);
			if (dwByte == 0)
				return E_FAIL;


			// WEIGHT
			if (modelDesc->mModelType == CModel::TYPE_ANIM)
			{
				_uint NumAffectingBones = meshdesc->mNumAffectingBones;
				if (NumAffectingBones != 0)
				{
					// 뼈 인덱스 리스트
					WriteFile(hFile, meshdesc->mAffectingBones, sizeof(_uint)*NumAffectingBones, &dwByte, nullptr);

					// 뼈 Weight
					for (_uint bone = 0; bone < NumAffectingBones; ++bone)
					{
						_uint NumWeight = meshdesc->mMeshBones[bone].mNumWeights;
						WriteFile(hFile, &NumWeight, sizeof(_uint), &dwByte, nullptr);

						if (NumWeight == 0)
							continue;
						WriteFile(hFile, meshdesc->mMeshBones[bone].mAiWeights,
							sizeof(aiVertexWeight)*NumWeight, &dwByte, nullptr);
					}
				}
			}
		}

		// MATERIAL
		for (_uint i = 0; i < modelDesc->mNumMaterials; ++i)
		{
			MATDESC* matdesc = &modelDesc->mMaterials[i];
			WriteFile(hFile, matdesc->MatName, sizeof(wchar_t)*MAX_PATH*AI_TEXTURE_TYPE_MAX, &dwByte, nullptr);
		}

		// BONE
		if (modelDesc->mModelType == CModel::TYPE_ANIM)
		{
			for (_uint i = 0; i < modelDesc->mNumBones; ++i)
			{
				BONEDESC* bonedesc = &modelDesc->mBones[i];
				WriteFile(hFile, bonedesc->mParentBoneName, sizeof(char)*MAX_PATH, &dwByte, nullptr);
				WriteFile(hFile, bonedesc->mCurrentBoneName, sizeof(char)*MAX_PATH, &dwByte, nullptr);
				WriteFile(hFile, &bonedesc->mOffsetMat, sizeof(_float4x4), &dwByte, nullptr);
				WriteFile(hFile, &bonedesc->mTransMat, sizeof(_float4x4), &dwByte, nullptr);
				WriteFile(hFile, &bonedesc->mDepth, sizeof(_uint), &dwByte, nullptr);
			}



			// ANI
			for (_uint i = 0; i < modelDesc->mNumAnimations; ++i)
			{
				ANIDESC* anidesc = &modelDesc->mAnimations[i];
				WriteFile(hFile, anidesc->mAniName, sizeof(char)*MAX_PATH, &dwByte, nullptr);
				WriteFile(hFile, &anidesc->mDuration, sizeof(double), &dwByte, nullptr);
				WriteFile(hFile, &anidesc->mTicksPerSecond, sizeof(double), &dwByte, nullptr);
				WriteFile(hFile, &anidesc->mNumAniBones, sizeof(_uint), &dwByte, nullptr);
			}

			for (_uint i = 0; i < modelDesc->mNumAnimations; ++i)
			{
				ANIDESC* anidesc = &modelDesc->mAnimations[i];
				for (_uint j = 0; j < anidesc->mNumAniBones; ++j)
				{
					ANIBONES* anibone = &modelDesc->mAnimations[i].mAniBones[j];

					WriteFile(hFile, anibone->mBoneName, sizeof(char)*MAX_PATH, &dwByte, nullptr);
					WriteFile(hFile, &anibone->mHierarchyNodeIndex, sizeof(_int), &dwByte, nullptr);
					WriteFile(hFile, &anibone->mNumKeyFrames, sizeof(_uint), &dwByte, nullptr);
					WriteFile(hFile, anibone->mKeyFrames, sizeof(KEYFRAME)* anibone->mNumKeyFrames, &dwByte, nullptr);
				}

			}
		}
	}

	CloseHandle(hFile);

	return S_OK;
}

HRESULT CAssimpMgr::CopyData_MODELDESC(wstring fbxFullpath, wstring namepath, CModel * copymodel, _uint ModelType)
{
	// #COPY ModelCopy
	CModel* model = copymodel;
	auto Scene = model->Get_AssimpScene();
	MODELDESC* ModelDesc = NEW MODELDESC();
	ModelDesc->mModelType = ModelType;


	// 경로 추가
	lstrcpy(ModelDesc->mFBXFullPath, fbxFullpath.c_str());
	lstrcpy(ModelDesc->mFBXFileName, namepath.c_str());


	// Mesh 데이터 복사
	_uint NumMeshs = ModelDesc->mNumMeshes = Scene->mNumMeshes;
	ModelDesc->mMeshDesc = NEW MESHDESC[NumMeshs];
	ZeroMemory(ModelDesc->mMeshDesc, sizeof(MESHDESC)*NumMeshs);

	for (_uint i = 0; i < NumMeshs; ++i)
	{
		aiMesh* aimesh = Scene->mMeshes[i];


		// UINT
		ModelDesc->mMeshDesc[i].mPrimitiveTypes = aimesh->mPrimitiveTypes;
		_uint NumVertices = ModelDesc->mMeshDesc[i].mNumVertices = aimesh->mNumVertices;
		_uint NumFaces = ModelDesc->mMeshDesc[i].mNumFaces = aimesh->mNumFaces;
		ModelDesc->mMeshDesc[i].mMaterialIndex = aimesh->mMaterialIndex;
		_uint NumAffectingBones = ModelDesc->mMeshDesc[i].mNumAffectingBones = aimesh->mNumBones;

		ModelDesc->mMeshDesc[i].mVertices = NEW _float3[NumVertices];
		ModelDesc->mMeshDesc[i].mNormals = NEW _float3[NumVertices];
		ModelDesc->mMeshDesc[i].mTangents = NEW _float3[NumVertices];
		ModelDesc->mMeshDesc[i].mUV = NEW _float2[NumVertices];

		//for (_uint j = 0; j < AI_MAX_NUMBER_OF_TEXTURECOORDS; ++j)
		//{
		//	ModelDesc->mMeshDesc[i].mTextureCoords[j] = NEW _float3[NumVertices];
		//}


		// VTX
		memcpy(ModelDesc->mMeshDesc[i].mVertices, aimesh->mVertices, sizeof(_float3)*NumVertices);
		memcpy(ModelDesc->mMeshDesc[i].mNormals, aimesh->mNormals, sizeof(_float3)*NumVertices);
		memcpy(ModelDesc->mMeshDesc[i].mTangents, aimesh->mTangents, sizeof(_float3)*NumVertices);
		_float3* cash = NEW _float3[NumVertices];
		memcpy(cash, aimesh->mTextureCoords[0], sizeof(_float3)*NumVertices);
		for (_uint k=0; k< NumVertices;k++)
		{
			ModelDesc->mMeshDesc[i].mUV[k].x = cash[k].x;
			ModelDesc->mMeshDesc[i].mUV[k].y = cash[k].y;
		}
		Safe_Delete_Array(cash);


		//for (_uint j = 0; j < AI_MAX_NUMBER_OF_TEXTURECOORDS; ++j)
		//{
		//	if (aimesh->mTextureCoords[j] != nullptr)
		//	{
		//		memcpy(ModelDesc->mMeshDesc[i].mTextureCoords[j], aimesh->mTextureCoords[j], sizeof(_float3)*NumVertices);
		//	}
		//}

		ModelDesc->mMeshDesc[i].mFaces = NEW FACEINDICES32[NumFaces];

		// INDEX
		for (_uint index = 0; index < NumFaces; ++index)
		{
			ModelDesc->mMeshDesc[i].mFaces[index]._0 = aimesh->mFaces[index].mIndices[0];
			ModelDesc->mMeshDesc[i].mFaces[index]._1 = aimesh->mFaces[index].mIndices[1];
			ModelDesc->mMeshDesc[i].mFaces[index]._2 = aimesh->mFaces[index].mIndices[2];
		}

		// 영향주는 뼈 정보 복사
		if (CModel::TYPE_ANIM == ModelType)
		{
			ModelDesc->mMeshDesc[i].mAffectingBones = NEW _uint[NumAffectingBones];
			ModelDesc->mMeshDesc[i].mMeshBones = NEW MESHBONEDESC[NumAffectingBones];


			const vector<_uint>&  vecUint = model->Get_VecMeshes_AffectingBoneIndes(aimesh);
			for (_uint j = 0; j < NumAffectingBones; ++j)
			{
				aiBone*	 pAffectingBone = aimesh->mBones[j];

				// 뼈 인덱스 라스트복사
				ModelDesc->mMeshDesc[i].mAffectingBones[j] = vecUint[j];

				// 뼈 weight 복사
				_uint NumWeight = ModelDesc->mMeshDesc[i].mMeshBones[j].mNumWeights = pAffectingBone->mNumWeights;
				if (NumWeight == 0)
					continue;
				ModelDesc->mMeshDesc[i].mMeshBones[j].mAiWeights = NEW aiVertexWeight[NumWeight];

				for (_uint k = 0; k < NumWeight; k++)
				{
					ModelDesc->mMeshDesc[i].mMeshBones[j].mAiWeights[k].mVertexId = pAffectingBone->mWeights[k].mVertexId;
					ModelDesc->mMeshDesc[i].mMeshBones[j].mAiWeights[k].mWeight = pAffectingBone->mWeights[k].mWeight;
				}
			}
		}
	}

	// Mat 데이터 복사
	
	_uint NumMaterials = ModelDesc->mNumMaterials = Scene->mNumMaterials;
	ModelDesc->mMaterials = NEW MATDESC[NumMaterials];

	for (_uint i = 0; i < NumMaterials; ++i)
	{
		aiMaterial* aimat = Scene->mMaterials[i];
		for (_uint j = 1; j < AI_TEXTURE_TYPE_MAX; ++j)
		{
			aiString aistr;
			if (FAILED(aimat->GetTexture(aiTextureType(j), 0, &aistr)))
				continue;

			string str = aistr.C_Str();
			wstring wstr = CHelperClass::Convert_str2wstr(str);
			lstrcpy(ModelDesc->mMaterials[i].MatName[j], wstr.c_str());

		}
	}

	if (CModel::TYPE_ANIM == ModelType)
	{
		// 뼈 정보 복사
		// 현재 뼈 / 부모 뼈 / offset 행렬 / depth?
		aiNode* RootNode = Scene->mRootNode;
		auto Vec_HIer = model->Get_VecBones();
		vector<BONEDESC*>  VecBones;
		Ready_HierarchyNodes2(Vec_HIer, VecBones);
		sort(VecBones.begin(), VecBones.end(), [](BONEDESC* pSour, BONEDESC* pDest)
		{
			return pSour->mDepth < pDest->mDepth;
		});
		ModelDesc->mNumBones = (_uint)VecBones.size();
		ModelDesc->mBones = NEW BONEDESC[ModelDesc->mNumBones];
		_uint boneindex = 0;
		for (auto& a: VecBones)
		{
			ModelDesc->mBones[boneindex] = *a;
			boneindex++;
		}
		
		Safe_Delete_List(VecBones);




		// Ani 데이터 복사
		auto VecAni = model->Get_VecAni();
		_uint NumAnimations = ModelDesc->mNumAnimations = VecAni.size();
		
		ModelDesc->mAnimations =NEW ANIDESC[NumAnimations];

		for (_uint i = 0; i < NumAnimations; ++i)
		{
			// 애니메이션 관련 정보 복사
			CAnimationClip* ani = VecAni[i];

			ANIDESC* anidesc = NEW ANIDESC(ani->Get_Name(),
				ani->Get_Duration(),
				ani->Get_UpdatePerSecond());

			ModelDesc->mAnimations[i] = *anidesc;
			anidesc->mNumAniBones = ani->Get_NumClipBone();

			//////////////////////////////////////////////////////
			anidesc->mAniBones = NEW ANIBONES[anidesc->mNumAniBones];
			for (_uint b = 0; b < anidesc->mNumAniBones; ++b)
			{
				// 뼈 키프레임 복사
				CClipBone* aniBone = ani->Get_VecClipBones()[b];

				ANIBONES* anibone = NEW ANIBONES(aniBone->Get_Name(),
					aniBone->Get_Frames(), aniBone->Get_HierNodeIndex());
				const vector<KEYFRAME*>* keycontainer = aniBone->Get_KeyFrameContainor();
				_uint index = 0;
				for (auto& key : *keycontainer)
				{
					anibone->mKeyFrames[index] = *key;
					index++;
				}
				Safe_Delete(anibone);

			}
			//////////////////////////////////////////////////////

			ModelDesc->mAnimations[i] = *anidesc;
		}
	}

	mMap_ModelDesc.emplace(ModelDesc->mFBXFileName, ModelDesc);

	return S_OK;
}

//HRESULT CAssimpMgr::Ready_HierarchyNodes(vector<BONEDESC*>& vec, aiNode * pNode, string parentBoneName, _uint iDepth)
//{
//
//	// 뼈 정보 생성
//	NULL_CHECK_RETURN(pNode, E_FAIL);
//
//	BONEDESC* bone = NEW BONEDESC(parentBoneName.c_str(),pNode->mName.C_Str(), XMMatrixIdentity(),iDepth);
//
//	//bone->mParentBoneName;
//	//bone->mCurrentBoneName;
//	//bone->mOffsetMat;
//	//bone->mDepth = iDepth;
//
//	vec.push_back(bone);
//
//	for (_uint i = 0; i < pNode->mNumChildren; ++i)
//	{
//		FAILED_CHECK(Ready_HierarchyNodes(vec, pNode->mChildren[i], pNode->mName.data, iDepth + 1));
//	}
//	return S_OK;
//}

HRESULT CAssimpMgr::Ready_HierarchyNodes2(const vector<CHierarchyNode*>& VecHierNodes, vector<BONEDESC*>& vec)
{
	// 계층 정보 복사
	for (auto bone : VecHierNodes)
	{
		char ParentName[MAX_PATH] = "";
		if (bone->Get_ParentName())
			strcpy_s(ParentName, bone->Get_ParentName());

			
		_float4x4 offsetMat = bone->Get_OffsetMatrix();
		_float4x4 transmat = bone->Get_Transformation();

		BONEDESC* boneDesc = NEW BONEDESC(ParentName, bone->Get_Name(), transmat.XMatrix(), offsetMat.XMatrix(), bone->Get_Depth());
		vec.push_back(boneDesc);

	}

	return S_OK;
}



HRESULT CAssimpMgr::Ready_OffsetMatrix(vector<BONEDESC*>& vec)
{
	// 해당 모델에 이미 영향받는 뼈 정보와 Offset이 있다.
	// 같은 이름의 뼈를 찾아서 데이터를 복사해주면 된다.


	//계층뼈에 오프셋 매트릭스를 저장하자
	//pMeshContainer->Set_OffSetMatrix(&OffsetMatrix);
	//pMeshContainer->Add_AffectingBoneIndex(iNodeIndex);

	return S_OK;
}


void CAssimpMgr::Free()
{
	ReleaseMap();
	Safe_Release(mDevice);
	Safe_Release(mDeviceContext);

}
