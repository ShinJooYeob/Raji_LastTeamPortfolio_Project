#include "stdafx.h"
#include "FIleIO/AssimpMgr.h"

IMPLEMENT_SINGLETON(CAssimpMgr);

HRESULT CAssimpMgr::Initialize(ID3D11Device* d, ID3D11DeviceContext* c)
{
	if (nullptr == d || nullptr == c)
		return E_FAIL;

	mDevice = d;
	mDeviceContext = c;

	Safe_AddRef(d);
	Safe_AddRef(c);

	mMap_ComponentModel.clear();
	mMap_ModelDesc.clear();
	return S_OK;
}

HRESULT CAssimpMgr::ReleaseMap()
{
	for (auto& model : mMap_ComponentModel)
	{
		Safe_Release(model.second);
		if (model.second != nullptr)
		{
			DEBUGBREAK;
			return E_FAIL;
		}
	}
	mMap_ComponentModel.clear();

	for (auto& model : mMap_ModelDesc)
	{
		Safe_Delete(model.second);
		if (model.second != nullptr)
		{
			DEBUGBREAK;
			return E_FAIL;
		}
	}
	mMap_ModelDesc.clear();

	


	return S_OK;
}

HRESULT CAssimpMgr::Initialize_FbxSetting()
{
	// 텍스트 파일읽고 해당 경로의 모델 컴포넌트로 읽음
	if (mMap_ComponentModel.empty() == false)
	{
		ReleaseMap();
	}

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
		mMap_ComponentModel.emplace(fileName, ModelCash);

	}

	for (auto& path : Path_Dynamic)
	{
		wstring wPath = path->FullPath;
		wstring wName = path->FileName;

		string fileFullpath = CHelperClass::Convert_Wstr2str(wPath);
		string fileName = CHelperClass::Convert_Wstr2str(wName);

		CModel* ModelCash = CModel::Create(mDevice, mDeviceContext, CModel::TYPE_ANIM, fileFullpath.c_str(), fileName.c_str());

		mMap_ComponentModel.emplace(fileName, ModelCash);

	}

	// 3. 로드된 Assimp로 데이터 저장

	for (auto&model : mMap_ComponentModel)
	{
		auto Scene =  model.second->Get_AssimpScene();
		MODELDESC* ModelDesc = NEW MODELDESC();

		// Mesh 데이터 저장
		int MeshMax = ModelDesc->mNumMeshes = Scene->mNumMeshes;

		ModelDesc->mMeshDesc = NEW MESHDESC[MeshMax];
		ZeroMemory(ModelDesc->mMeshDesc,sizeof(MESHDESC)*MeshMax);

		for (_uint i=0; i< MeshMax;++i)
		{
			aiMesh* SceneMesh = Scene->mMeshes[i];
			ModelDesc->mMeshDesc[i].mPrimitiveTypes = SceneMesh->mPrimitiveTypes;
			_uint NumVertices = ModelDesc->mMeshDesc[i].mNumVertices = SceneMesh->mNumVertices;
			_uint NumFaces = ModelDesc->mMeshDesc[i].mNumFaces = SceneMesh->mNumFaces;

			ModelDesc->mMeshDesc[i].mVertices = NEW _float3[NumVertices];
			ModelDesc->mMeshDesc[i].mNormals = NEW _float3[NumVertices];
			ModelDesc->mMeshDesc[i].mTangents = NEW _float3[NumVertices];

			// 정점 복사
			for (_uint v = 0; v < NumVertices; ++v)
			{
				memcpy(&ModelDesc->mMeshDesc[i].mVertices[v], &SceneMesh->mVertices[v], sizeof(_float3));
				memcpy(&ModelDesc->mMeshDesc[i].mNormals[v], &SceneMesh->mNormals[v], sizeof(_float3));
				memcpy(&ModelDesc->mMeshDesc[i].mTangents[v], &SceneMesh->mTangents[v], sizeof(_float3));

			}

			ModelDesc->mMeshDesc[i].mFaces = NEW FACEINDICES32[NumFaces];
			
			// 인덱스 복사
			for (_uint index = 0; index < NumFaces; ++index)
			{
				ModelDesc->mMeshDesc[i].mFaces[index]._0 = SceneMesh->mFaces[index].mIndices[0];
				ModelDesc->mMeshDesc[i].mFaces[index]._1 = SceneMesh->mFaces[index].mIndices[1];
				ModelDesc->mMeshDesc[i].mFaces[index]._2 = SceneMesh->mFaces[index].mIndices[2];

			}

			ModelDesc->mMeshDesc[i].mMaterialIndex = SceneMesh->mMaterialIndex;


		}
		
		mMap_ModelDesc.emplace(model.first, ModelDesc);

		int debug = 5;


		// Mat 데이터 저장
		// desc->mNumMaterials = assimp->mNumMaterials;
	}


	// 4. 데이터 Dat 파일로 출력

	for (auto& pair: mMap_ModelDesc)
	{		
		string name = pair.first;
		wstring wstr = CHelperClass::Convert_str2wstr(name);
		int length =  wstr.length();
		wstring Name = wstr.substr(0, length - 4);
		Save_MODELDESC(STR_FILEPATH_RESOURCE_DAT_L, Name, pair.second);
	}
	return S_OK;
}


// Model 데이터 저장
HRESULT CAssimpMgr::Save_MODELDESC(wstring FolderPath, wstring filename, MODELDESC* modelDesc)
{
	// #SAVE DESC 정보 저장
	// 확장자로 나누기 // ㅇ 
	_ulong			dwByte = 0;

	//	wstring ExeName = L".dat";
//	wstring dynamic = FolderPath + L"\\" + filename + L".dyn";

	wstring staticPath = FolderPath + L"\\" + filename + L".stc";

	HANDLE			hFile = CreateFile(staticPath.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	int id = 0;
	switch (id)
	{
	case 0:
	{

		WriteFile(hFile, &modelDesc->mModelType, sizeof(_uint), &dwByte, nullptr);
		WriteFile(hFile, &modelDesc->mNumMeshes, sizeof(_uint), &dwByte, nullptr);

		for (_uint i = 0; i < modelDesc->mNumMeshes; ++i)
		{
			MESHDESC meshdesc = modelDesc->mMeshDesc[i];

			WriteFile(hFile, &meshdesc.mPrimitiveTypes, sizeof(_uint), &dwByte, nullptr);
			WriteFile(hFile, &meshdesc.mNumVertices, sizeof(_uint), &dwByte, nullptr);
			WriteFile(hFile, &meshdesc.mNumFaces, sizeof(_uint), &dwByte, nullptr);

			// VTX
			WriteFile(hFile, meshdesc.mVertices, sizeof(_float3)*meshdesc.mNumVertices, &dwByte, nullptr);
			if (dwByte == 0)
				return E_FAIL;

			WriteFile(hFile, meshdesc.mNormals, sizeof(_float3) *meshdesc.mNumVertices, &dwByte, nullptr);
			if (dwByte == 0)
				return E_FAIL;

			WriteFile(hFile, meshdesc.mTangents, sizeof(_float3)*meshdesc.mNumVertices, &dwByte, nullptr);
			if (dwByte == 0)
				return E_FAIL;

			// INDEX
			WriteFile(hFile, meshdesc.mFaces, sizeof(FACEINDICES32)*meshdesc.mNumFaces, &dwByte, nullptr);
			if (dwByte == 0)
				return E_FAIL;

			WriteFile(hFile, &meshdesc.mMaterialIndex, sizeof(_uint), &dwByte, nullptr);

		}

	}

		break;
	case 1:
		break;
	default:
		break;
	}

	CloseHandle(hFile);

	return S_OK;
}



void CAssimpMgr::Free()
{
	ReleaseMap();
	Safe_Release(mDevice);
	Safe_Release(mDeviceContext);

}
