#include "stdafx.h"
#include "..\Public\Scene_Edit.h"
#include "Scene_Loading.h"
#include "EditorTerrain.h"
#include "Camera_Editor.h"
#include "ESCursor.h"
#include "MapObject.h"
#include "NaviPoint.h"
#include "Cell.h"
#include "CollisionMgr.h"
#include "ColliderBuffer.h"

CScene_Edit::CScene_Edit(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CScene(pDevice, pDeviceContext)
{
}

HRESULT CScene_Edit::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

#ifdef _DEBUG
	m_pGameInstance = GetSingle(CGameInstance);
	Safe_AddRef(m_pGameInstance);

	FAILED_CHECK(Ready_Layer_Player(TAG_LAY(Layer_Player)));
	FAILED_CHECK(Ready_Layer_MainCamera(TAG_LAY(Layer_Camera_Main)));
	FAILED_CHECK(Ready_Layer_RendererEditUI(TAG_LAY(Layer_UI_IMG)));
	FAILED_CHECK(Ready_CamActionCursor(L"CamCursor"));
	FAILED_CHECK(Ready_ParticleDesc());
	
	


	//터짐 방지용 빈 오브젝트
	OBJELEMENT ObjElement;


	FAILED_CHECK(m_pGameInstance->Add_GameObject_Out_of_Manager(&ObjElement.pObject, SCENE_EDIT, TAG_OP(Prototype_EditorCursor)));

	ZeroMemory(ObjElement.matSRT.m, sizeof(_float) * 16);


	ObjElement.matSRT.m[0][0] = 1.f;
	ObjElement.matSRT.m[0][1] = 1.f;
	ObjElement.matSRT.m[0][2] = 1.f;
	ObjElement.matSRT.m[1][0] = 1.f;
	ObjElement.matSRT.m[1][1] = 1.f;
	ObjElement.matSRT.m[1][2] = 1.f;

	RenewElenmetTransform(&ObjElement);

	m_vecBatchedObject.push_back(ObjElement);

	Prevent_Order = false;
	m_SelectedObjectSRT = &(m_vecBatchedObject[0].matSRT);
	m_iBatchedVecIndex = 0;
	m_iPassIndex = 0;

	ZeroMemory(m_ArrBuffer, sizeof(_float) * 4);
	ZeroMemory(bArrWindowFlag, sizeof(_bool) * 10);
	ZeroMemory(m_iSelectedObjectNMesh, sizeof(_uint) * 2);
	m_iSelectedObjectNMesh[0] = Prototype_StaticMapObject;
	m_iSelectedObjectNMesh[1] = Prototype_Mesh_None;



	m_ArrBuffer[3] = 0.1f;


#ifdef _DEBUG
	m_pVIBuffer = CVIBuffer_Triangle::Create(m_pDevice, m_pDeviceContext);
	if (nullptr == m_pVIBuffer)
	{
		MSGBOX("트라이앵글 버퍼떔에 터짐");
		return E_FAIL;
	}

	m_pShader = CShader::Create(m_pDevice, m_pDeviceContext, TEXT("Shader_Navigation.hlsl"), VTXCOL_DECLARATION::Elements, VTXCOL_DECLARATION::iNumElements);
	if (nullptr == m_pShader)
	{
		MSGBOX("쉐이더땜에 터짐");
		return E_FAIL;
	}
#endif // _DEBUG




#endif // USE_IMGUI
	return S_OK;
}

_int CScene_Edit::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)
		return -1;

	if (m_bIsNeedToSceneChange)
		return Change_to_NextScene();

#ifdef _DEBUG


	switch (m_iNowTab)
	{

	case 0://맵 배치탭
		for (auto& Element : m_vecBatchedObject)
		{
			if (Element.pObject->Update(fDeltaTime) < 0)
			{
				__debugbreak();
				return -1;
			}
		}

		if (m_pCreatedTerrain != nullptr)
			m_pCreatedTerrain->Update(fDeltaTime);

		for (auto& Element : m_vecTriggerObject)
		{
			if (Element->Update(fDeltaTime) < 0)
			{
				__debugbreak();
				return -1;
			}
		}
		break;
	case 1://UI 탭

	


		break;
	case 2://파티클 탭
		break;
	case 3://캠 액션탭

	{
		m_pCamCursor->Update(fDeltaTime);

		for (auto& iter : m_vecCamPosBatchedObj)
			iter->Update(fDeltaTime);
		for (auto& iter : m_vecLookBatchedObj)
			iter->Update(fDeltaTime);
	}


		break;
	case 4://터레인 생성 탭
		if (m_pCreatedTerrain != nullptr)
			m_pCreatedTerrain->Update(fDeltaTime);

		for (auto& Point : m_Points)
			Point->Update(fDeltaTime);
		break;
	default:
		break;
	}



	FAILED_CHECK(Input_KeyBoard(fDeltaTime));

	if (GETIMGUI->Update_ImguiMgr(fDeltaTime) >= 0)
	{
		FAILED_CHECK(Update_First_Frame(fDeltaTime, "Editor"));
		Prevent_Order = true;
	}

#endif // USE_IMGUI
	return 0;
}

_int CScene_Edit::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)
		return -1;

	if (m_bIsNeedToSceneChange)
		return Change_to_NextScene();


#ifdef _DEBUG


	switch (m_iNowTab)
	{

	case 0://맵 배치탭
		for (auto& Element : m_vecBatchedObject)
		{
			if (Element.pObject->LateUpdate(fDeltaTime) < 0)
			{
				__debugbreak();
				return -1;
			}
		}
		if (m_pCreatedTerrain != nullptr)
			m_pCreatedTerrain->LateUpdate(fDeltaTime);

		for (auto& Element : m_vecTriggerObject)
		{
			if (Element->LateUpdate(fDeltaTime) < 0)
			{
				__debugbreak();
				return -1;
			}
		}


		break;
	case 1://UI 탭



		
		break;
	case 2://파티클 탭
	{
		static _bool	 Show = true;

		if (g_pGameInstance->Get_DIKeyState(DIK_Q) & DIS_Down)Show = !Show;

		if (Show)
		{

			for (auto& Element : m_vecBatchedObject)
			{
				if (Element.pObject->LateUpdate(fDeltaTime) < 0)
				{
					__debugbreak();
					return -1;
				}
			}
			if (m_pCreatedTerrain != nullptr)
				m_pCreatedTerrain->LateUpdate(fDeltaTime);

		}
	}
		break;
	case 3://캠 액션탭
	{
		static _bool	 Show = true;

		if (g_pGameInstance->Get_DIKeyState(DIK_Q) & DIS_Down)Show = !Show;

		if (Show)
		{

			for (auto& Element : m_vecBatchedObject)
			{
				if (Element.pObject->LateUpdate(fDeltaTime) < 0)
				{
					__debugbreak();
					return -1;
				}
			}
			if (m_pCreatedTerrain != nullptr)
				m_pCreatedTerrain->LateUpdate(fDeltaTime);

		}



		if (iPickKinds == 0)
			m_pCamCursor->LateUpdate(fDeltaTime);

		_uint i = 0;
		for (auto& iter : m_vecCamPosBatchedObj)
		{
			if (i == iCamPosIndex)
				iter->Set_Color({0, 0.7f, 0, 1});
			else
				iter->Set_Color({ 0.1f, 0.25f, 0.1f, 1 });

			iter->LateUpdate(fDeltaTime);
			i++;
		}
		i = 0;
		for (auto& iter : m_vecLookBatchedObj)
		{
			if (i == iCamLookIndex)
				iter->Set_Color({ 0, 0, 1.f, 1 });
			else
				iter->Set_Color({ 0.1f, 0.1f, 0.25f, 1 });

			iter->LateUpdate(fDeltaTime);

			i++;
		}
	}
		break;
	case 4://터레인 생성 탭


	{
		static _bool	 Show = true;

		if (g_pGameInstance->Get_DIKeyState(DIK_Q) & DIS_Down)Show = !Show;

		if (Show)
		{

			for (auto& Element : m_vecBatchedObject)
			{
				if (Element.pObject->LateUpdate(fDeltaTime) < 0)
				{
					__debugbreak();
					return -1;
				}
			}

		}

	}


		if (m_pCreatedTerrain != nullptr)
			m_pCreatedTerrain->LateUpdate(fDeltaTime);
		break;
	default:
		break;
	}



#endif // USE_IMGUI
	return 0;
}

_int CScene_Edit::Render()
{
	if (__super::Render() < 0)
		return -1;

#ifdef _DEBUG
	if (nullptr == m_pVIBuffer)
	{
		MSGBOX("렌더에서 트라이앵글땜에 터짐");
		return E_FAIL;
	}

	for (auto& Point : m_Points)
	{
		if (nullptr != Point)
		{
			Point->Render();
		}
	}

	m_pShader->Set_RawValue("g_WorldMatrix", &XMMatrixIdentity(), sizeof(_float4x4));
	m_pShader->Set_RawValue("g_ViewMatrix", &g_pGameInstance->Get_Transform_Float4x4_TP(TRANSFORMSTATETYPE::PLM_VIEW), sizeof(_float4x4));
	m_pShader->Set_RawValue("g_ProjMatrix", &g_pGameInstance->Get_Transform_Float4x4_TP(TRANSFORMSTATETYPE::PLM_PROJ), sizeof(_float4x4));
	//셀 렌더
	for (auto& pCell : m_Cells)
	{
		if (nullptr != pCell)
		{
			pCell->Render(m_pVIBuffer, m_pShader, _float4(1.f, 1.f, 1.f, 1.f));
		}
	}
#endif // _DEBUG

#ifdef _DEBUG
	SetWindowText(g_hWnd, TEXT("MapEditScene"));
#endif // _DEBUG


	return 0;
}

_int CScene_Edit::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;

#ifdef _DEBUG
	if (Prevent_Order)
	{
		GETIMGUI->Render_ImguiMgr();
		Prevent_Order = false;
	}
#endif // USE_IMGUI


	return 0;
}

_int CScene_Edit::Change_to_NextScene()
{

	switch (m_eNextScene)
	{

	case SCENEID::SCENE_LOBY:
	{
		FAILED_CHECK(GetSingle(CUtilityMgr)->Clear_RenderGroup_forSceneChange());
		FAILED_CHECK(g_pGameInstance->Scene_Change(CScene_Loading::Create(m_pDevice, m_pDeviceContext, SCENEID::SCENE_LOBY), SCENEID::SCENE_LOADING));
		break;
	}


	}
	return _int();
}

#ifdef _DEBUG

#pragma region Total
HRESULT CScene_Edit::Update_First_Frame(_double fDeltatime, const char * szFrameBarName)
{
	//_bool demotrue = true;
	//ImGui::ShowDemoWindow(&demotrue);

	ImGuiWindowFlags window_flags = 0;
	if (bArrWindowFlag[0])				window_flags |= ImGuiWindowFlags_NoBackground;
	if (!bArrWindowFlag[1])				window_flags |= ImGuiWindowFlags_NoMove;
	/*
	if (bArrWindowFlag[2])				window_flags |= ImGuiWindowFlags_NoTitleBar;
	if (bArrWindowFlag[3])				window_flags |= ImGuiWindowFlags_NoScrollbar;
	if (!bArrWindowFlag[4])				window_flags |= ImGuiWindowFlags_MenuBar;
	if (bArrWindowFlag[5])				window_flags |= ImGuiWindowFlags_NoResize;
	if (bArrWindowFlag[6])				window_flags |= ImGuiWindowFlags_NoCollapse;
	if (bArrWindowFlag[7])				window_flags |= ImGuiWindowFlags_NoNav;
	if (bArrWindowFlag[8])				window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
	if (bArrWindowFlag[9])				window_flags |= ImGuiWindowFlags_UnsavedDocument;
	*/

	static _bool bFirstSetting = false;

	if (!bFirstSetting)
	{
		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowSize(ImVec2(center.x * 0.667f, center.y * 2.f), ImGuiCond_None);
		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_None, ImVec2(0.0f, 0.0f));
		bFirstSetting = true;
	}


	GETIMGUI->Begin_Update_Frame(fDeltatime, szFrameBarName, window_flags);



	ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 0.0f);

	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
	if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
	{
		if (ImGui::BeginTabItem("Map"))
		{
			m_iNowTab = 0;
			FAILED_CHECK(Update_MapTab(fDeltatime));
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("UI"))
		{
			m_iNowTab = 1;
			FAILED_CHECK(Update_UITab(fDeltatime));
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Particle"))
		{
			m_iNowTab = 2;
			FAILED_CHECK(Update_ParticleTab(fDeltatime));
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("CamAction"))
		{
			m_iNowTab = 3;
			FAILED_CHECK(Update_CameraActionTab(fDeltatime));
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("HeightMap"))
		{
			m_iNowTab = 4;
			FAILED_CHECK(Update_HeightMap(fDeltatime));
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Trigger"))
		{
			m_iNowTab = 0;
			FAILED_CHECK(Update_TriggerTab(fDeltatime));
			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	ImGui::PopStyleVar();


	GETIMGUI->End_Update_Frame();
	return S_OK;


}

void CScene_Edit::Make_HelpWidget(const char * szString)
{

	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(szString);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}

}

void CScene_Edit::Make_VerticalSpacing(_uint count)
{
	for (_uint i = 0; i < count; i++)
	{
		ImGui::Spacing();
	}

}

HRESULT CScene_Edit::Sava_Data(const char* szFileName, eDATATYPE iKinds)
{
	switch (iKinds)
	{
	case Client::CScene_Edit::Data_Map:
	{
		//../bin/Resources/Data/Map/
		_tchar szFullPath[MAX_PATH] =L"../bin/Resources/Data/Map/";
		_tchar wFileName[MAX_PATH] = L"";

		MultiByteToWideChar(CP_UTF8, 0, szFileName, -1, wFileName, sizeof(wFileName));
		//WideCharToMultiByte(CP_UTF8, 0, fd.name, -1, szFilename, sizeof(szFilename), NULL, NULL);
		lstrcat(szFullPath, wFileName);


		//HANDLE hFile = CreateFileW(szFullPath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

		HANDLE hFile = ::CreateFileW(szFullPath, GENERIC_WRITE , FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, NULL);




		if (INVALID_HANDLE_VALUE == hFile)
		{
			__debugbreak();
			return E_FAIL;
		}

		DWORD	dwByte = 0;

		//// 유니코드임을 알리는 BOM
		//BYTE wc = 0xEF;
		//WriteFile(hFile, &wc, 1, &dwByte, NULL);
		//wc = 0xBB;
		//WriteFile(hFile, &wc, 1, &dwByte, NULL);
		//wc = 0xBF;
		//WriteFile(hFile, &wc, 1, &dwByte, NULL);

		lstrcpy(m_vecBatchedObject[0].ObjectID, L"Prototype_EditorCursor");
		lstrcpy(m_vecBatchedObject[0].MeshID, L"Prototype_Mesh_None");





		_int iIDLength = 0;

		for (auto& tObjectElement : m_vecBatchedObject)
		{
			// key 값 저장


			iIDLength = lstrlen(tObjectElement.ObjectID);
			WriteFile(hFile, &(iIDLength), sizeof(_int), &dwByte, nullptr);
			WriteFile(hFile, (tObjectElement.ObjectID), sizeof(_tchar) * iIDLength, &dwByte, nullptr);

			iIDLength = lstrlen(tObjectElement.MeshID);
			WriteFile(hFile, &(iIDLength), sizeof(_int), &dwByte, nullptr);
			WriteFile(hFile, (tObjectElement.MeshID), sizeof(_tchar) * iIDLength, &dwByte, nullptr);

			WriteFile(hFile, &(tObjectElement.PassIndex), sizeof(_uint), &dwByte, nullptr);
			WriteFile(hFile, &(tObjectElement.FrustumRange), sizeof(_float), &dwByte, nullptr);
			WriteFile(hFile, &(tObjectElement.bIsOcllsuion), sizeof(_bool), &dwByte, nullptr);
			WriteFile(hFile, &(tObjectElement.matSRT.m[0][0]), sizeof(_float) * 16, &dwByte, nullptr);
			WriteFile(hFile, &(tObjectElement.matTransform.m[0][0]), sizeof(_float) * 16, &dwByte, nullptr);



		}
		CloseHandle(hFile);

	}
		break;
	case Client::CScene_Edit::Data_UI:
	{

	}
	break;
	case Client::CScene_Edit::Data_Particle_Mesh:
	{


		//../bin/Resources/Data/ParicleData/TextureParticle/
		_tchar szFullPath[MAX_PATH] = L"../bin/Resources/Data/ParicleData/MeshParticle/";
		_tchar wFileName[MAX_PATH] = L"";

		MultiByteToWideChar(CP_UTF8, 0, szFileName, -1, wFileName, sizeof(wFileName));
		//WideCharToMultiByte(CP_UTF8, 0, fd.name, -1, szFilename, sizeof(szFilename), NULL, NULL);
		lstrcat(szFullPath, wFileName);


		//HANDLE hFile = CreateFileW(szFullPath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

		HANDLE hFile = ::CreateFileW(szFullPath, GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, NULL);





		if (INVALID_HANDLE_VALUE == hFile)
		{
			__debugbreak();
			return E_FAIL;
		}

		DWORD	dwByte = 0;

		_int iIDLength = 0;


		

		WriteFile(hFile, &(m_tMeshDesc.eParticleTypeID), sizeof(eInstanceEffectID), &dwByte, nullptr);
		WriteFile(hFile, &(m_tMeshDesc.eInstanceCount), sizeof(COMPONENTPROTOTYPEID), &dwByte, nullptr);
		WriteFile(hFile, &(m_tMeshDesc.ePassID), sizeof(eMeshInstancePassID), &dwByte, nullptr);


		WriteFile(hFile, &(m_tMeshDesc.vFixedPosition), sizeof(_float3), &dwByte, nullptr);
		WriteFile(hFile, &(m_tMeshDesc.vPowerDirection), sizeof(_float3), &dwByte, nullptr);

		iIDLength = lstrlen(m_tMeshDesc.szModelMeshProtoTypeTag);
		WriteFile(hFile, &(iIDLength), sizeof(_int), &dwByte, nullptr);
		WriteFile(hFile, (m_tMeshDesc.szModelMeshProtoTypeTag), sizeof(_tchar) * iIDLength, &dwByte, nullptr);


		WriteFile(hFile, &(m_tMeshDesc.iModelAnimIndex), sizeof(_int), &dwByte, nullptr);


		WriteFile(hFile, &(m_tMeshDesc.iNoiseTextureIndex), sizeof(_int), &dwByte, nullptr);
		WriteFile(hFile, &(m_tMeshDesc.iMaskingTextureIndex), sizeof(_int), &dwByte, nullptr);
		WriteFile(hFile, &(m_tMeshDesc.vNoisePushingDir), sizeof(_float2), &dwByte, nullptr);
		WriteFile(hFile, &(m_tMeshDesc.fAppearTimer), sizeof(_float), &dwByte, nullptr);
		WriteFile(hFile, &(m_tMeshDesc.fDistortionNoisingPushPower), sizeof(_float), &dwByte, nullptr);


		WriteFile(hFile, &(m_tMeshDesc.TotalParticleTime), sizeof(_float), &dwByte, nullptr);
		WriteFile(hFile, &(m_tMeshDesc.EachParticleLifeTime), sizeof(_float), &dwByte, nullptr);


		WriteFile(hFile, &(m_tMeshDesc.SizeChageFrequency), sizeof(_uint), &dwByte, nullptr);
		WriteFile(hFile, &(m_tMeshDesc.ParticleSize), sizeof(_float3), &dwByte, nullptr);
		WriteFile(hFile, &(m_tMeshDesc.ParticleSize2), sizeof(_float3), &dwByte, nullptr);


		WriteFile(hFile, &(m_tMeshDesc.ColorChageFrequency), sizeof(_uint), &dwByte, nullptr);
		WriteFile(hFile, &(m_tMeshDesc.TargetColor), sizeof(_float4), &dwByte, nullptr);
		WriteFile(hFile, &(m_tMeshDesc.TargetColor2), sizeof(_float4), &dwByte, nullptr);



		WriteFile(hFile, &(m_tMeshDesc.fMaxBoundaryRadius), sizeof(_float), &dwByte, nullptr);


		WriteFile(hFile, &(m_tMeshDesc.Particle_Power), sizeof(_float), &dwByte, nullptr);

		WriteFile(hFile, &(m_tMeshDesc.PowerRandomRange), sizeof(_float2), &dwByte, nullptr);
		WriteFile(hFile, &(m_tMeshDesc.SubPowerRandomRange_RUL), sizeof(_float3), &dwByte, nullptr);

		WriteFile(hFile, &(m_tMeshDesc.ParticleStartRandomPosMin), sizeof(_float3), &dwByte, nullptr);
		WriteFile(hFile, &(m_tMeshDesc.ParticleStartRandomPosMax), sizeof(_float3), &dwByte, nullptr);


		WriteFile(hFile, &(m_tMeshDesc.bAutoTurn), sizeof(_bool), &dwByte, nullptr);
		WriteFile(hFile, &(m_tMeshDesc.bIsOclusion), sizeof(_bool), &dwByte, nullptr);
		WriteFile(hFile, &(m_tMeshDesc.bEmissive), sizeof(_bool), &dwByte, nullptr);


		WriteFile(hFile, &(m_tMeshDesc.vEmissive_SBB), sizeof(_float3), &dwByte, nullptr);
		WriteFile(hFile, &(m_tMeshDesc.fRotSpeed_Radian), sizeof(_float), &dwByte, nullptr);


		WriteFile(hFile, &(m_tMeshDesc.TempBuffer_0), sizeof(_float4), &dwByte, nullptr);
		WriteFile(hFile, &(m_tMeshDesc.TempBuffer_1), sizeof(_float4), &dwByte, nullptr);
		WriteFile(hFile, &(m_tMeshDesc.TempBuffer_2), sizeof(_float4), &dwByte, nullptr);
		WriteFile(hFile, &(m_tMeshDesc.TempBuffer_3), sizeof(_float4), &dwByte, nullptr);

		
		CloseHandle(hFile);


	}
	break;
	case Client::CScene_Edit::Data_Particle_Texture:
	{


		//
		_tchar szFullPath[MAX_PATH] = L"../bin/Resources/Data/ParicleData/TextureParticle/";
		_tchar wFileName[MAX_PATH] = L"";

		MultiByteToWideChar(CP_UTF8, 0, szFileName, -1, wFileName, sizeof(wFileName));
		//WideCharToMultiByte(CP_UTF8, 0, fd.name, -1, szFilename, sizeof(szFilename), NULL, NULL);
		lstrcat(szFullPath, wFileName);


		//HANDLE hFile = CreateFileW(szFullPath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

		HANDLE hFile = ::CreateFileW(szFullPath, GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, NULL);


		if (INVALID_HANDLE_VALUE == hFile)
		{
			__debugbreak();
			return E_FAIL;
		}
		DWORD	dwByte = 0;
		_int iIDLength = 0;
		


		WriteFile(hFile, &(m_tParticleDesc.eParticleTypeID), sizeof(eInstanceEffectID), &dwByte, nullptr);
		WriteFile(hFile, &(m_tParticleDesc.eInstanceCount), sizeof(COMPONENTPROTOTYPEID), &dwByte, nullptr);
		WriteFile(hFile, &(m_tParticleDesc.ePassID), sizeof(eInstancePassID), &dwByte, nullptr);

		WriteFile(hFile, &(m_tParticleDesc.bBillboard), sizeof(_bool), &dwByte, nullptr);

		WriteFile(hFile, &(m_tParticleDesc.vFixedPosition), sizeof(_float3), &dwByte, nullptr);
		WriteFile(hFile, &(m_tParticleDesc.vPowerDirection), sizeof(_float3), &dwByte, nullptr);

		iIDLength = lstrlen(m_tParticleDesc.szTextureProtoTypeTag);
		WriteFile(hFile, &(iIDLength), sizeof(_int), &dwByte, nullptr);
		WriteFile(hFile, (m_tParticleDesc.szTextureProtoTypeTag), sizeof(_tchar) * iIDLength, &dwByte, nullptr);

		iIDLength = lstrlen(m_tParticleDesc.szTextureLayerTag);
		WriteFile(hFile, &(iIDLength), sizeof(_int), &dwByte, nullptr);
		WriteFile(hFile, (m_tParticleDesc.szTextureLayerTag), sizeof(_tchar) * iIDLength, &dwByte, nullptr);

		WriteFile(hFile, &(m_tParticleDesc.iTextureLayerIndex), sizeof(_int), &dwByte, nullptr);


		WriteFile(hFile, &(m_tParticleDesc.iNoiseTextureIndex), sizeof(_int), &dwByte, nullptr);
		WriteFile(hFile, &(m_tParticleDesc.iMaskingTextureIndex), sizeof(_int), &dwByte, nullptr);
		WriteFile(hFile, &(m_tParticleDesc.vNoisePushingDir), sizeof(_float2), &dwByte, nullptr);
		WriteFile(hFile, &(m_tParticleDesc.fAppearTimer), sizeof(_float), &dwByte, nullptr);
		WriteFile(hFile, &(m_tParticleDesc.fDistortionNoisingPushPower), sizeof(_float), &dwByte, nullptr);

		WriteFile(hFile, &(m_tParticleDesc.TextureChageFrequency), sizeof(_uint), &dwByte, nullptr);
		WriteFile(hFile, &(m_tParticleDesc.vTextureXYNum), sizeof(_float2), &dwByte, nullptr);
		WriteFile(hFile, &(m_tParticleDesc.iFigureCount_In_Texture), sizeof(_int), &dwByte, nullptr);


		WriteFile(hFile, &(m_tParticleDesc.TotalParticleTime), sizeof(_float), &dwByte, nullptr);
		WriteFile(hFile, &(m_tParticleDesc.EachParticleLifeTime), sizeof(_float), &dwByte, nullptr);


		WriteFile(hFile, &(m_tParticleDesc.SizeChageFrequency), sizeof(_uint), &dwByte, nullptr);
		WriteFile(hFile, &(m_tParticleDesc.ParticleSize), sizeof(_float3), &dwByte, nullptr);
		WriteFile(hFile, &(m_tParticleDesc.ParticleSize2), sizeof(_float3), &dwByte, nullptr);


		WriteFile(hFile, &(m_tParticleDesc.ColorChageFrequency), sizeof(_uint), &dwByte, nullptr);
		WriteFile(hFile, &(m_tParticleDesc.TargetColor), sizeof(_float4), &dwByte, nullptr);
		WriteFile(hFile, &(m_tParticleDesc.TargetColor2), sizeof(_float4), &dwByte, nullptr);



		WriteFile(hFile, &(m_tParticleDesc.fMaxBoundaryRadius), sizeof(_float), &dwByte, nullptr);


		WriteFile(hFile, &(m_tParticleDesc.Particle_Power), sizeof(_float), &dwByte, nullptr);

		WriteFile(hFile, &(m_tParticleDesc.PowerRandomRange), sizeof(_float2), &dwByte, nullptr);
		WriteFile(hFile, &(m_tParticleDesc.SubPowerRandomRange_RUL), sizeof(_float3), &dwByte, nullptr);

		WriteFile(hFile, &(m_tParticleDesc.ParticleStartRandomPosMin), sizeof(_float3), &dwByte, nullptr);
		WriteFile(hFile, &(m_tParticleDesc.ParticleStartRandomPosMax), sizeof(_float3), &dwByte, nullptr);


		WriteFile(hFile, &(m_tParticleDesc.AlphaBlendON), sizeof(_bool), &dwByte, nullptr);
		WriteFile(hFile, &(m_tParticleDesc.bEmissive), sizeof(_bool), &dwByte, nullptr);
		WriteFile(hFile, &(m_tParticleDesc.vEmissive_SBB), sizeof(_float3), &dwByte, nullptr);
		WriteFile(hFile, &(m_tParticleDesc.m_fAlphaTestValue), sizeof(_float), &dwByte, nullptr);



		WriteFile(hFile, &(m_tParticleDesc.TempBuffer_0), sizeof(_float4), &dwByte, nullptr);
		WriteFile(hFile, &(m_tParticleDesc.TempBuffer_1), sizeof(_float4), &dwByte, nullptr);
		WriteFile(hFile, &(m_tParticleDesc.TempBuffer_2), sizeof(_float4), &dwByte, nullptr);
		WriteFile(hFile, &(m_tParticleDesc.TempBuffer_3), sizeof(_float4), &dwByte, nullptr);


		CloseHandle(hFile);


	}
	break;
	case Client::CScene_Edit::Data_CameraAction:
	{
		//../bin/Resources/Data/Map/
		_tchar szFullPath[MAX_PATH] = L"../bin/Resources/Data/CameraAction/";
		_tchar wFileName[MAX_PATH] = L"";

		MultiByteToWideChar(CP_UTF8, 0, szFileName, -1, wFileName, sizeof(wFileName));
		//WideCharToMultiByte(CP_UTF8, 0, fd.name, -1, szFilename, sizeof(szFilename), NULL, NULL);
		lstrcat(szFullPath, wFileName);


		//HANDLE hFile = CreateFileW(szFullPath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

		HANDLE hFile = ::CreateFileW(szFullPath, GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, NULL);





		if (INVALID_HANDLE_VALUE == hFile)
		{
			__debugbreak();
			return E_FAIL;
		};

		DWORD	dwByte = 0;

		_uint iCount = _uint(m_vecCamPositions.size());
		WriteFile(hFile, &(iCount), sizeof(_uint), &dwByte, nullptr);
		for (auto& tCamActDesc : m_vecCamPositions)
		{
			// key 값 저장
			WriteFile(hFile, &(tCamActDesc.fDuration), sizeof(_float), &dwByte, nullptr);
			WriteFile(hFile, &(tCamActDesc.vPosition), sizeof(_float3), &dwByte, nullptr);
		}

		iCount = _uint(m_vecLookPostions.size());
		WriteFile(hFile, &(iCount), sizeof(_uint), &dwByte, nullptr);

		for (auto& tCamActDesc : m_vecLookPostions)
		{
			// key 값 저장
			WriteFile(hFile, &(tCamActDesc.fDuration), sizeof(_float), &dwByte, nullptr);
			WriteFile(hFile, &(tCamActDesc.vPosition), sizeof(_float3), &dwByte, nullptr);
		}






		CloseHandle(hFile);

	}
		break;


	case Client::CScene_Edit::Data_HeightMap:
	{

	}
	break;

	case Client::CScene_Edit::Data_FilterMap:
	{

	}
	break;
	case Client::CScene_Edit::Data_Trigger:
	{
		//../bin/Resources/Data/ParicleData/TextureParticle/
		_tchar szFullPath[MAX_PATH] = L"../bin/Resources/Data/Trigger/";
		_tchar wFileName[MAX_PATH] = L"";

		MultiByteToWideChar(CP_UTF8, 0, szFileName, -1, wFileName, sizeof(wFileName));
		//WideCharToMultiByte(CP_UTF8, 0, fd.name, -1, szFilename, sizeof(szFilename), NULL, NULL);
		lstrcat(szFullPath, wFileName);


		//HANDLE hFile = CreateFileW(szFullPath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

		HANDLE hFile = ::CreateFileW(szFullPath, GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, NULL);





		if (INVALID_HANDLE_VALUE == hFile)
		{
			__debugbreak();
			return E_FAIL;
		}

		DWORD	dwByte = 0;

		_int iIDLength = 0;


		for (auto& pTrigObj : m_vecTriggerObject)
		{
			_uint eNumber = pTrigObj->Get_eNumber();
			wstring eObjectID = pTrigObj->Get_eObjectID();

			_float4x4 WorldMat = ((CTransform*)pTrigObj->Get_Component(TAG_COM(Com_Transform)))->Get_WorldFloat4x4();
			_float4x4 ValueData = pTrigObj->Get_ValueMat();


			WriteFile(hFile, &(eNumber), sizeof(_uint), &dwByte, nullptr);
			
			iIDLength = eObjectID.length();
			WriteFile(hFile, &(iIDLength), sizeof(_int), &dwByte, nullptr);
			WriteFile(hFile, eObjectID.c_str(), sizeof(_tchar) * iIDLength, &dwByte, nullptr);
			WriteFile(hFile, &(WorldMat), sizeof(_float4x4), &dwByte, nullptr);
			WriteFile(hFile, &(ValueData), sizeof(_float4x4), &dwByte, nullptr);

		}

		CloseHandle(hFile);
	}
	break;
	

	case Client::CScene_Edit::Data_Navigation:
	{
		//../bin/Resources/Data/Map/
		_tchar szFullPath[MAX_PATH] = L"../bin/Resources/Data/NaviMesh/";
		_tchar wFileName[MAX_PATH] = L"";

		MultiByteToWideChar(CP_UTF8, 0, szFileName, -1, wFileName, sizeof(wFileName));
		//WideCharToMultiByte(CP_UTF8, 0, fd.name, -1, szFilename, sizeof(szFilename), NULL, NULL);
		lstrcat(szFullPath, wFileName);

		_ulong			dwByte = 0;
		HANDLE			hFile = CreateFile(szFullPath, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

		if (0 == hFile)
		{
			MSGBOX("네비메쉬저장 실패");
			return E_FAIL;
		}

		_float3			vPoints[3];

		for (auto& Cell : m_Cells)
		{
			for (_int i = 0; i < 3; ++i)
			{
				switch (i)
				{
				case 0:
					XMStoreFloat3(&vPoints[i], Cell->Get_Point(CCell::POINT_A));
					break;
				case 1:
					XMStoreFloat3(&vPoints[i], Cell->Get_Point(CCell::POINT_B));
					break;
				case 2:
					XMStoreFloat3(&vPoints[i], Cell->Get_Point(CCell::POINT_C));
					break;
				}
			}
			WriteFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);
			CCell::CELL_OPTION CellOption = Cell->Get_CellOption();
			WriteFile(hFile, &CellOption, sizeof(CCell::CELL_OPTION),  &dwByte, nullptr);

		}

		CloseHandle(hFile);

		MSGBOX("네비메쉬저장 성공");
	}
	break;

	default:

		break;
	}


	return S_OK;
}

HRESULT CScene_Edit::Load_Data(const char * szFileName, eDATATYPE iKinds)
{
	switch (iKinds)
	{
	case Client::CScene_Edit::Data_Map:
	{


		list<wstring> MeshIDList;

		{
			for (auto& tObjElement : m_vecBatchedObject)
			{
				//오브젝트 제대로 델리트delete 해주기
				Safe_Release(tObjElement.pObject);
			}
			m_vecBatchedObject.clear();

			ZeroMemory(m_ArrBuffer, sizeof(_float) * 4);
			m_ArrBuffer[3] = 0.1f;
			ZeroMemory(m_iSelectedObjectNMesh, sizeof(_uint) * 2);
						m_iSelectedObjectNMesh[0] = Prototype_StaticMapObject;
			m_iSelectedObjectNMesh[1] = Prototype_Mesh_None;

			m_bIsModelMove = 0;
			m_iKindsOfMoving = 0;
			m_iSelectedXYZ = 0; 
			m_iPassIndex = 0;
		}



		//../bin/Resources/Data/Map/
		_tchar szFullPath[MAX_PATH] = L"../bin/Resources/Data/Map/";
		_tchar wFileName[MAX_PATH] = L"";

		MultiByteToWideChar(CP_UTF8, 0, szFileName, -1, wFileName, sizeof(wFileName));
		//WideCharToMultiByte(CP_UTF8, 0, fd.name, -1, szFilename, sizeof(szFilename), NULL, NULL);
		lstrcat(szFullPath, wFileName);



		//HANDLE hFile = CreateFileW(szFullPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);


		HANDLE hFile = ::CreateFileW(szFullPath, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, NULL);


		if (INVALID_HANDLE_VALUE == hFile)
			return E_FAIL;

		DWORD	dwByte = 0;

		CGameInstance* pInstance = g_pGameInstance;

		_uint iIDLength = 0;

		// 유니코드임을 알리는 BOM
		//DWORD wc = 0xFF;
		//ReadFile(hFile, &wc, 3, &dwByte, NULL);

		while (true)
		{
			OBJELEMENT	tData{};
			_tchar szBuffer[MAX_PATH] = L"";
			// key 값 로드
			ReadFile(hFile, &(iIDLength), sizeof(_uint), &dwByte, nullptr);
			ReadFile(hFile, (tData.ObjectID), sizeof(_tchar) * iIDLength, &dwByte, nullptr);
			//lstrcpy(tData.ObjectID, szBuffer);

			ReadFile(hFile, &(iIDLength), sizeof(_uint), &dwByte, nullptr);
			ReadFile(hFile, (tData.MeshID), sizeof(_tchar) * iIDLength, &dwByte, nullptr);
			//lstrcpy(tData.MeshID, szBuffer);

			ReadFile(hFile, &(tData.PassIndex), sizeof(_uint), &dwByte, nullptr);
			ReadFile(hFile, &(tData.FrustumRange), sizeof(_float), &dwByte, nullptr);
			ReadFile(hFile, &(tData.bIsOcllsuion), sizeof(_bool), &dwByte, nullptr);
			
			ReadFile(hFile, &(tData.matSRT.m[0][0]), sizeof(_float) * 16, &dwByte, nullptr);
			ReadFile(hFile, &(tData.matTransform.m[0][0]), sizeof(_float) * 16, &dwByte, nullptr);

			if (0 == dwByte)
				break;
			
			//객채 생성해주기

			pInstance->Add_GameObject_Out_of_Manager(&(tData.pObject), SCENE_EDIT, tData.ObjectID);

			NULL_CHECK_RETURN(tData.pObject, E_FAIL);


			if (lstrcmp(tData.MeshID, TAG_CP(Prototype_Mesh_None)))
			{
				//매쉬 바꿔주기 
				tData.pObject->Change_Component_by_NewAssign(SCENE_EDIT, tData.MeshID, TAG_COM(Com_Model));
				((CMapObject*)tData.pObject)->Set_FrustumSize(tData.FrustumRange);
				((CMapObject*)tData.pObject)->Set_IsOcllusion(tData.bIsOcllsuion);
				((CMapObject*)tData.pObject)->Set_PassIndex(tData.PassIndex);




				CDissolve::DISSOLVEDESC	tDissolveDesc;

				tDissolveDesc.eDissolveModelType = CDissolve::DISSOLVE_NONANIM;
				tDissolveDesc.pModel = (CModel*)tData.pObject->Get_Component(TAG_COM(Com_Model));
				tDissolveDesc.pShader = (CShader*)tData.pObject->Get_Component(TAG_COM(Com_Shader));
				tDissolveDesc.RampTextureIndex = 1;

				tData.pObject->Change_Component_by_NewAssign(SCENE_EDIT, TAG_CP(Prototype_Dissolve), TAG_COM(Com_Dissolve),&tDissolveDesc);

				MeshIDList.push_back(tData.MeshID);
			}

			//트렌스폼
			CTransform* pTrans = (CTransform*)(tData.pObject->Get_Component(TAG_COM(Com_Transform)));
			NULL_CHECK_RETURN(pTrans, E_FAIL);
			//tData.matTransform._42 -= 20;
			pTrans->Set_Matrix(tData.matTransform);



			m_vecBatchedObject.push_back(tData);
		}


		m_iBatchedVecIndex = 0;
		m_SelectedObjectSRT = &(m_vecBatchedObject[m_iBatchedVecIndex].matSRT);
		m_bIsOcllusion = m_vecBatchedObject[m_iBatchedVecIndex].bIsOcllsuion;
		m_fFrustumRange = (m_vecBatchedObject[m_iBatchedVecIndex].FrustumRange);
		m_iPassIndex = (m_vecBatchedObject[m_iBatchedVecIndex].PassIndex);

		CloseHandle(hFile);
	
		//wstring
		OutputDebugStringW(L"TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));\n");
		OutputDebugStringW(L"CAssimpCreateMgr* pAssimpCreateMgr = GetSingle(CAssimpCreateMgr);\n");

		for (auto MeshID : MeshIDList)
		{
			wstring Ouput;

			Ouput = L"pAssimpCreateMgr->Load_Model_One_ByFBXName(L\"" + MeshID + L"\", TransformMatrix);\n";

			OutputDebugStringW(Ouput.c_str());

		}

		//TransformMatrix = XMMatrixScaling(1, 1, 1) * XMMatrixRotationY(XMConvertToRadians(180.0f));
		//GetSingle(CAssimpCreateMgr)->Load_Model_One_ByFBXName(TAG_MONSTER_BULLET(Prototype_Mesh_Monster_Bullet_Vayusura_Leader), TransformMatrix);
	}
	break;
	case Client::CScene_Edit::Data_UI:
	{
		Safe_Release(m_TargetSRV);

		char szFileFath[MAX_PATH] = "";
		strcat_s(szFileFath, szFileName);

		_tchar szWidePath[MAX_PATH] = L"";

		MultiByteToWideChar(CP_UTF8, 0, szFileFath, -1, szWidePath, sizeof(szWidePath));

		FAILED_CHECK( LoadTextureByAssimp(szWidePath));


		if (m_TargetSRV == nullptr)			return E_FAIL;



	}
	break;
	case Client::CScene_Edit::Data_Particle_Texture:
	{
		m_tParticleDesc = INSTPARTICLEDESC();

		//../bin/Resources/Data/Map/
		_tchar szFullPath[MAX_PATH] = L"../bin/Resources/Data/ParicleData/TextureParticle/";
		_tchar wFileName[MAX_PATH] = L"";

		MultiByteToWideChar(CP_UTF8, 0, szFileName, -1, wFileName, sizeof(wFileName));
		//WideCharToMultiByte(CP_UTF8, 0, fd.name, -1, szFilename, sizeof(szFilename), NULL, NULL);
		lstrcat(szFullPath, wFileName);


		HANDLE hFile = ::CreateFileW(szFullPath, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, NULL);


		if (INVALID_HANDLE_VALUE == hFile)
			return E_FAIL;


		DWORD	dwByte = 0;
		_int iIDLength = 0;
		_tchar szTempBuffer[MAX_PATH] = L"";




		ReadFile(hFile, &(m_tParticleDesc.eParticleTypeID), sizeof(eInstanceEffectID), &dwByte, nullptr);
		ReadFile(hFile, &(m_tParticleDesc.eInstanceCount), sizeof(COMPONENTPROTOTYPEID), &dwByte, nullptr);
		ReadFile(hFile, &(m_tParticleDesc.ePassID), sizeof(eInstancePassID), &dwByte, nullptr);

		ReadFile(hFile, &(m_tParticleDesc.bBillboard), sizeof(_bool), &dwByte, nullptr);


		ReadFile(hFile, &(m_tParticleDesc.vFixedPosition), sizeof(_float3), &dwByte, nullptr);
		ReadFile(hFile, &(m_tParticleDesc.vPowerDirection), sizeof(_float3), &dwByte, nullptr);


		ZeroMemory(szTempBuffer, sizeof(_tchar) * MAX_PATH);
		ReadFile(hFile, &(iIDLength), sizeof(_int), &dwByte, nullptr);
		ReadFile(hFile, (szTempBuffer), sizeof(_tchar) * iIDLength, &dwByte, nullptr);
		lstrcpy(m_tParticleDesc.szTextureProtoTypeTag, szTempBuffer);


		ZeroMemory(szTempBuffer, sizeof(_tchar) * MAX_PATH);
		ReadFile(hFile, &(iIDLength), sizeof(_int), &dwByte, nullptr);
		ReadFile(hFile, (szTempBuffer), sizeof(_tchar) * iIDLength, &dwByte, nullptr);
		lstrcpy(m_tParticleDesc.szTextureLayerTag, szTempBuffer);


		ReadFile(hFile, &(m_tParticleDesc.iTextureLayerIndex), sizeof(_int), &dwByte, nullptr);


		ReadFile(hFile, &(m_tParticleDesc.iNoiseTextureIndex), sizeof(_int), &dwByte, nullptr);
		ReadFile(hFile, &(m_tParticleDesc.iMaskingTextureIndex), sizeof(_int), &dwByte, nullptr);
		ReadFile(hFile, &(m_tParticleDesc.vNoisePushingDir), sizeof(_float2), &dwByte, nullptr);
		ReadFile(hFile, &(m_tParticleDesc.fAppearTimer), sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, &(m_tParticleDesc.fDistortionNoisingPushPower), sizeof(_float), &dwByte, nullptr);

		ReadFile(hFile, &(m_tParticleDesc.TextureChageFrequency), sizeof(_uint), &dwByte, nullptr);
		ReadFile(hFile, &(m_tParticleDesc.vTextureXYNum), sizeof(_float2), &dwByte, nullptr);
		ReadFile(hFile, &(m_tParticleDesc.iFigureCount_In_Texture), sizeof(_int), &dwByte, nullptr);


		ReadFile(hFile, &(m_tParticleDesc.TotalParticleTime), sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, &(m_tParticleDesc.EachParticleLifeTime), sizeof(_float), &dwByte, nullptr);


		ReadFile(hFile, &(m_tParticleDesc.SizeChageFrequency), sizeof(_uint), &dwByte, nullptr);
		ReadFile(hFile, &(m_tParticleDesc.ParticleSize), sizeof(_float3), &dwByte, nullptr);
		ReadFile(hFile, &(m_tParticleDesc.ParticleSize2), sizeof(_float3), &dwByte, nullptr);


		ReadFile(hFile, &(m_tParticleDesc.ColorChageFrequency), sizeof(_uint), &dwByte, nullptr);
		ReadFile(hFile, &(m_tParticleDesc.TargetColor), sizeof(_float4), &dwByte, nullptr);
		ReadFile(hFile, &(m_tParticleDesc.TargetColor2), sizeof(_float4), &dwByte, nullptr);



		ReadFile(hFile, &(m_tParticleDesc.fMaxBoundaryRadius), sizeof(_float), &dwByte, nullptr);


		ReadFile(hFile, &(m_tParticleDesc.Particle_Power), sizeof(_float), &dwByte, nullptr);

		ReadFile(hFile, &(m_tParticleDesc.PowerRandomRange), sizeof(_float2), &dwByte, nullptr);
		ReadFile(hFile, &(m_tParticleDesc.SubPowerRandomRange_RUL), sizeof(_float3), &dwByte, nullptr);

		ReadFile(hFile, &(m_tParticleDesc.ParticleStartRandomPosMin), sizeof(_float3), &dwByte, nullptr);
		ReadFile(hFile, &(m_tParticleDesc.ParticleStartRandomPosMax), sizeof(_float3), &dwByte, nullptr);


		ReadFile(hFile, &(m_tParticleDesc.AlphaBlendON), sizeof(_bool), &dwByte, nullptr);
		ReadFile(hFile, &(m_tParticleDesc.bEmissive), sizeof(_bool), &dwByte, nullptr);
		ReadFile(hFile, &(m_tParticleDesc.vEmissive_SBB), sizeof(_float3), &dwByte, nullptr);
		ReadFile(hFile, &(m_tParticleDesc.m_fAlphaTestValue), sizeof(_float), &dwByte, nullptr);



		ReadFile(hFile, &(m_tParticleDesc.TempBuffer_0), sizeof(_float4), &dwByte, nullptr);
		ReadFile(hFile, &(m_tParticleDesc.TempBuffer_1), sizeof(_float4), &dwByte, nullptr);
		ReadFile(hFile, &(m_tParticleDesc.TempBuffer_2), sizeof(_float4), &dwByte, nullptr);
		ReadFile(hFile, &(m_tParticleDesc.TempBuffer_3), sizeof(_float4), &dwByte, nullptr);


		CloseHandle(hFile);


		
	}
	break;
	case Client::CScene_Edit::Data_Particle_Mesh:
	{
		m_tMeshDesc = INSTMESHDESC();

		//../bin/Resources/Data/Map/
		_tchar szFullPath[MAX_PATH] = L"../bin/Resources/Data/ParicleData/MeshParticle/";
		_tchar wFileName[MAX_PATH] = L"";

		MultiByteToWideChar(CP_UTF8, 0, szFileName, -1, wFileName, sizeof(wFileName));
		//WideCharToMultiByte(CP_UTF8, 0, fd.name, -1, szFilename, sizeof(szFilename), NULL, NULL);
		lstrcat(szFullPath, wFileName);


		HANDLE hFile = ::CreateFileW(szFullPath, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, NULL);


		if (INVALID_HANDLE_VALUE == hFile)
			return E_FAIL;


		DWORD	dwByte = 0;
		_int iIDLength = 0;



		ReadFile(hFile, &(m_tMeshDesc.eParticleTypeID), sizeof(eInstanceEffectID), &dwByte, nullptr);
		ReadFile(hFile, &(m_tMeshDesc.eInstanceCount), sizeof(COMPONENTPROTOTYPEID), &dwByte, nullptr);
		ReadFile(hFile, &(m_tMeshDesc.ePassID), sizeof(eMeshInstancePassID), &dwByte, nullptr);

		ReadFile(hFile, &(m_tMeshDesc.vFixedPosition), sizeof(_float3), &dwByte, nullptr);
		ReadFile(hFile, &(m_tMeshDesc.vPowerDirection), sizeof(_float3), &dwByte, nullptr);

		_tchar szTempBuffer[MAX_PATH] = L"";
		lstrcpy(szTempBuffer, L"");

		ReadFile(hFile, &(iIDLength), sizeof(_int), &dwByte, nullptr);

		ReadFile(hFile, (szTempBuffer), sizeof(_tchar) * iIDLength, &dwByte, nullptr);
		lstrcpy(m_tMeshDesc.szModelMeshProtoTypeTag, szTempBuffer);

		ReadFile(hFile, &(m_tMeshDesc.iModelAnimIndex), sizeof(_int), &dwByte, nullptr);


		ReadFile(hFile, &(m_tMeshDesc.iNoiseTextureIndex), sizeof(_int), &dwByte, nullptr);
		ReadFile(hFile, &(m_tMeshDesc.iMaskingTextureIndex), sizeof(_int), &dwByte, nullptr);
		ReadFile(hFile, &(m_tMeshDesc.vNoisePushingDir), sizeof(_float2), &dwByte, nullptr);
		ReadFile(hFile, &(m_tMeshDesc.fAppearTimer), sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, &(m_tMeshDesc.fDistortionNoisingPushPower), sizeof(_float), &dwByte, nullptr);


		ReadFile(hFile, &(m_tMeshDesc.TotalParticleTime), sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, &(m_tMeshDesc.EachParticleLifeTime), sizeof(_float), &dwByte, nullptr);


		ReadFile(hFile, &(m_tMeshDesc.SizeChageFrequency), sizeof(_uint), &dwByte, nullptr);
		ReadFile(hFile, &(m_tMeshDesc.ParticleSize), sizeof(_float3), &dwByte, nullptr);
		ReadFile(hFile, &(m_tMeshDesc.ParticleSize2), sizeof(_float3), &dwByte, nullptr);


		ReadFile(hFile, &(m_tMeshDesc.ColorChageFrequency), sizeof(_uint), &dwByte, nullptr);
		ReadFile(hFile, &(m_tMeshDesc.TargetColor), sizeof(_float4), &dwByte, nullptr);
		ReadFile(hFile, &(m_tMeshDesc.TargetColor2), sizeof(_float4), &dwByte, nullptr);



		ReadFile(hFile, &(m_tMeshDesc.fMaxBoundaryRadius), sizeof(_float), &dwByte, nullptr);


		ReadFile(hFile, &(m_tMeshDesc.Particle_Power), sizeof(_float), &dwByte, nullptr);

		ReadFile(hFile, &(m_tMeshDesc.PowerRandomRange), sizeof(_float2), &dwByte, nullptr);
		ReadFile(hFile, &(m_tMeshDesc.SubPowerRandomRange_RUL), sizeof(_float3), &dwByte, nullptr);

		ReadFile(hFile, &(m_tMeshDesc.ParticleStartRandomPosMin), sizeof(_float3), &dwByte, nullptr);
		ReadFile(hFile, &(m_tMeshDesc.ParticleStartRandomPosMax), sizeof(_float3), &dwByte, nullptr);


		ReadFile(hFile, &(m_tMeshDesc.bAutoTurn), sizeof(_bool), &dwByte, nullptr);
		ReadFile(hFile, &(m_tMeshDesc.bIsOclusion), sizeof(_bool), &dwByte, nullptr);
		ReadFile(hFile, &(m_tMeshDesc.bEmissive), sizeof(_bool), &dwByte, nullptr);


		ReadFile(hFile, &(m_tMeshDesc.vEmissive_SBB), sizeof(_float3), &dwByte, nullptr);
		ReadFile(hFile, &(m_tMeshDesc.fRotSpeed_Radian), sizeof(_float), &dwByte, nullptr);


		ReadFile(hFile, &(m_tMeshDesc.TempBuffer_0), sizeof(_float4), &dwByte, nullptr);
		ReadFile(hFile, &(m_tMeshDesc.TempBuffer_1), sizeof(_float4), &dwByte, nullptr);
		ReadFile(hFile, &(m_tMeshDesc.TempBuffer_2), sizeof(_float4), &dwByte, nullptr);
		ReadFile(hFile, &(m_tMeshDesc.TempBuffer_3), sizeof(_float4), &dwByte, nullptr);

		CloseHandle(hFile);


	}
	break;
	case Client::CScene_Edit::Data_CameraAction:
	{
		{
			for (auto& iter : m_vecCamPosBatchedObj)
				Safe_Release(iter);
			m_vecCamPosBatchedObj.clear();

			for (auto& iter : m_vecLookBatchedObj)
				Safe_Release(iter);
			m_vecLookBatchedObj.clear();

			m_vecCamPositions.clear();
			m_vecLookPostions.clear();
		}



		//../bin/Resources/Data/Map/
		_tchar szFullPath[MAX_PATH] = L"../bin/Resources/Data/CameraAction/";
		_tchar wFileName[MAX_PATH] = L"";

		MultiByteToWideChar(CP_UTF8, 0, szFileName, -1, wFileName, sizeof(wFileName));
		//WideCharToMultiByte(CP_UTF8, 0, fd.name, -1, szFilename, sizeof(szFilename), NULL, NULL);
		lstrcat(szFullPath, wFileName);

		//HANDLE hFile = CreateFileW(szFullPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);


		HANDLE hFile = ::CreateFileW(szFullPath, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, NULL);


		if (INVALID_HANDLE_VALUE == hFile)
			return E_FAIL;

		DWORD	dwByte = 0;

		CGameInstance* pInstance = g_pGameInstance;

		_uint iCount = 0;
		ReadFile(hFile, &(iCount), sizeof(_uint), &dwByte, nullptr);

		CAMACTDESC tDesc;

		for (_uint i = 0 ; i < iCount; i++)
		{
			ReadFile(hFile, &(tDesc.fDuration), sizeof(_float), &dwByte, nullptr);
			ReadFile(hFile, &(tDesc.vPosition), sizeof(_float3), &dwByte, nullptr);

			m_vecCamPositions.push_back(tDesc);

			CESCursor* pCursor = nullptr;
			FAILED_CHECK(m_pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&pCursor), SCENE_EDIT, TAG_OP(Prototype_EditorCursor)));
			NULL_CHECK_RETURN(pCursor, E_FAIL);
			pCursor->Set_Position(tDesc.vPosition);
			pCursor->Set_Color({ 0.1f, 0.25f, 0.1f, 1 });
			m_vecCamPosBatchedObj.push_back(pCursor);
		}


		iCount = 0;
		ReadFile(hFile, &(iCount), sizeof(_uint), &dwByte, nullptr);

		for (_uint i = 0; i < iCount; i++)
		{
			ReadFile(hFile, &(tDesc.fDuration), sizeof(_float), &dwByte, nullptr);
			ReadFile(hFile, &(tDesc.vPosition), sizeof(_float3), &dwByte, nullptr);

			m_vecLookPostions.push_back(tDesc);

			CESCursor* pCursor = nullptr;
			FAILED_CHECK(m_pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&pCursor), SCENE_EDIT, TAG_OP(Prototype_EditorCursor)));
			NULL_CHECK_RETURN(pCursor, E_FAIL);
			pCursor->Set_Position(tDesc.vPosition);
			pCursor->Set_Color({ 0.1f, 0.1f, 0.25f, 1 });
			m_vecLookBatchedObj.push_back(pCursor);
		}


		CloseHandle(hFile);


		iPickKinds = 0;
		iCamPosIndex = 0;
		iCamLookIndex = 0;

		CamDesc.vPosition = _float3(0);
		CamDesc.fDuration = 1;

	}
	break;
	case Client::CScene_Edit::Data_HeightMap:
	{



	}
	break;
	case Client::CScene_Edit::Data_Trigger:

	{

		CGameInstance* pInstance = g_pGameInstance;

		//../bin/Resources/Data/ParicleData/TextureParticle/
		_tchar szFullPath[MAX_PATH] = L"../bin/Resources/Data/Trigger/";
		_tchar wFileName[MAX_PATH] = L"";

		MultiByteToWideChar(CP_UTF8, 0, szFileName, -1, wFileName, sizeof(wFileName));
		//WideCharToMultiByte(CP_UTF8, 0, fd.name, -1, szFilename, sizeof(szFilename), NULL, NULL);
		lstrcat(szFullPath, wFileName);


		HANDLE hFile = ::CreateFileW(szFullPath, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, NULL);


		if (INVALID_HANDLE_VALUE == hFile)
		{
			__debugbreak();
			return E_FAIL;
		}

		DWORD	dwByte = 0;
		_int iIDLength = 0;




		while (true)
		{


			_uint eNumber = 0;
			_tchar eObjectID[MAX_PATH];
			_float4x4 WorldMat = XMMatrixIdentity();
			_float4x4 ValueData= XMMatrixIdentity();

			ZeroMemory(eObjectID, sizeof(_tchar) * MAX_PATH);

			ReadFile(hFile, &(eNumber), sizeof(_uint), &dwByte, nullptr);
			ReadFile(hFile, &(iIDLength), sizeof(_int), &dwByte, nullptr);
			ReadFile(hFile, &(eObjectID), sizeof(_tchar) * iIDLength, &dwByte, nullptr);

			ReadFile(hFile, &(WorldMat), sizeof(_float4x4), &dwByte, nullptr);
			ReadFile(hFile, &(ValueData), sizeof(_float4x4), &dwByte, nullptr);
			if (0 == dwByte) break;


			CTriggerObject* pObject = nullptr;

			FAILED_CHECK(pInstance->Add_GameObject_Out_of_Manager((CGameObject**)&pObject, SCENE_EDIT, eObjectID, &eNumber));

			NULL_CHECK_RETURN(pObject, E_FAIL);

			pObject->Set_eNumberNObjectID(eNumber, eObjectID);

			((CTransform*)pObject->Get_Component(TAG_COM(Com_Transform)))->Set_Matrix(WorldMat);

			pObject->Set_ValueMat(&ValueData);
			m_vecTriggerObject.push_back(pObject);
		}

		CloseHandle(hFile);
	} 
	break;
	case Client::CScene_Edit::Data_FilterMap:
	{



	}
	break;

	case Client::CScene_Edit::Data_Navigation:
	{
		m_iVertexCount = 0;
		m_iCellCount = 0;

		for (auto& Point : m_Points)
			Safe_Delete(Point);
		m_Points.clear();

		for (auto& Cell : m_Cells)
			Safe_Release(Cell);
		m_Cells.clear();

		for (auto& Vertex : m_vVertexs)
			Safe_Delete(Vertex);
		m_vVertexs.clear();

		for (auto& CellName : m_vCellNames)
			Safe_Delete(CellName);
		m_vCellNames.clear();

		_tchar szFullPath[MAX_PATH] = L"../bin/Resources/Data/NaviMesh/";
		_tchar wFileName[MAX_PATH] = L"";

		MultiByteToWideChar(CP_UTF8, 0, szFileName, -1, wFileName, sizeof(wFileName));
		lstrcat(szFullPath, wFileName);

		_ulong			dwByte = 0;
		HANDLE			hFile = CreateFile(szFullPath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (0 == hFile)
			return E_FAIL;

		_float3			vPoints[3];

		while (true)
		{
			ReadFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);

			CCell::CELL_OPTION CellOption = CCell::CELL_END;
			ReadFile(hFile, &CellOption, sizeof(CCell::CELL_OPTION), &dwByte, nullptr);

			if (0 == dwByte)
				break;

			CCell*		pCell = CCell::Create(m_pDevice, m_pDeviceContext, vPoints, (_uint)m_Cells.size());
			if (nullptr == pCell)
			{
				MSGBOX("Edit_Load_NaviMesh_Cell Failed");
				return E_FAIL;
			}

			++m_iCellCount;
			char sIntStr[5];
			_itoa_s(m_iCellCount, sIntStr, 10);
			char str[10] = "Cell";
			strcat_s(str, sIntStr);

			char* sCellNum = NEW char[10];

			strcpy_s(sCellNum, 10, str);

			m_vCellNames.push_back(sCellNum);

			if (m_Points.size() <= 0)
			{
				for (_int i = 0; i < 3; ++i)
				{
					Ready_Layer_LoadNaviPoint(vPoints[i]);
					++m_iVertexCount;
					char sIntStr[5];
					_itoa_s(m_iVertexCount, sIntStr, 10);
					char str[10] = "Vertex";
					char* sVertexNum = NEW char[10];
					strcat_s(str, sIntStr);
					strcpy_s(sVertexNum, 10, str);

					m_vVertexs.push_back(sVertexNum);
				}
			}
			else
			{
				_bool ISTakePoint = false;

				for (_int i = 0; i < 3; ++i)
				{
					ISTakePoint = false;
					for (_int j = 0; j < m_Points.size(); ++j)
					{
						_float3 PointPos = m_Points[j]->Get_Position();

						if (vPoints[i] == PointPos)
						{
							ISTakePoint = true;
							break;
						}
					}

					if (!ISTakePoint)
					{
						Ready_Layer_LoadNaviPoint(vPoints[i]);
						++m_iVertexCount;
						char sIntStr[5];
						_itoa_s(m_iVertexCount, sIntStr, 10);
						char str[10] = "Vertex";
						char* sVertexNum = NEW char[10];
						strcat_s(str, sIntStr);
						strcpy_s(sVertexNum, 10, str);

						m_vVertexs.push_back(sVertexNum);
					}
				}
			}
			m_Cells.push_back(pCell);

			if (nullptr != pCell)
			{
				memcpy(m_vVertexts, vPoints, sizeof(_float3) * 3);

				for (_int i = 0; i < m_Points.size(); ++i)
				{
					CTransform* PointTransform = (CTransform*)m_Points[i]->Get_Component(TAG_COM(Com_Transform));
					_float3 Pos = PointTransform->Get_MatrixState(CTransform::STATE_POS);

					for (_int j = 0; j < 3; ++j)
					{

						if (Pos == m_vVertexts[j])
						{
							m_Points[i]->Push_Cells(pCell);
							break;
						}
					}
				}

				ZeroMemory(m_vVertexts, sizeof(_float3) * 3);
			}


			pCell->Set_CellOption(CellOption);
		}

		CloseHandle(hFile);



	}
	break;

	default:

		break;
	}
	return S_OK;
}

HRESULT CScene_Edit::Input_KeyBoard(_double fDeltaTime)
{

	CGameInstance* pInstance = GetSingle(CGameInstance);

#pragma region Total

	{
		static _bool IsWheelClicked = false;
		_byte BtnState = pInstance->Get_DIMouseButtonState(CInput_Device::MBS_WHEEL);
		if (BtnState & DIS_Press)
		{
			if (!IsWheelClicked && (BtnState & DIS_Down))
				IsWheelClicked = true;
			else if (IsWheelClicked && (BtnState & DIS_Up))
				IsWheelClicked = false;
			else if (IsWheelClicked && (BtnState & DIS_Press))
			{
				if (pInstance->Get_DIKeyState(DIK_LSHIFT)&DIS_Press)
				{

					_long fWheelMove = pInstance->Get_DIMouseMoveState(CInput_Device::MMS_Y);
					CTransform* CamTransform = m_pEditorCam->Get_Camera_Transform();
					CamTransform->Turn_CW(CamTransform->Get_MatrixState(CTransform::STATE_RIGHT), fWheelMove*fDeltaTime);

					fWheelMove = pInstance->Get_DIMouseMoveState(CInput_Device::MMS_X);

					CamTransform->Turn_CW(XMVectorSet(0, 1, 0, 0), fWheelMove* fDeltaTime);


				}
				else {

					_long fWheelMove = pInstance->Get_DIMouseMoveState(CInput_Device::MMS_Y);
					CTransform* CamTransform = m_pEditorCam->Get_Camera_Transform();

					CamTransform->MovetoDir_bySpeed(
						CamTransform->Get_MatrixState(CTransform::STATE_UP), (_float)fWheelMove, fDeltaTime);

					fWheelMove = pInstance->Get_DIMouseMoveState(CInput_Device::MMS_X);

					CamTransform->MovetoDir_bySpeed(
						CamTransform->Get_MatrixState(CTransform::STATE_RIGHT), (_float)-fWheelMove, fDeltaTime);


				}


			}

		}
		if (pInstance->Get_DIKeyState(DIK_LSHIFT)&DIS_Press)
		{
			_long fWheelMove = pInstance->Get_DIMouseMoveState(CInput_Device::MMS_WHEEL);
			if (fWheelMove)
			{
				CTransform* CamTransform = m_pEditorCam->Get_Camera_Transform();

				CamTransform->MovetoDir_bySpeed(
					CamTransform->Get_MatrixState(CTransform::STATE_LOOK), (_float)fWheelMove, fDeltaTime);
			}

		}

	}
#pragma endregion Total

	if (m_iNowTab == 0) 
	{
#pragma region MapTab

		if (pInstance->Get_DIKeyState(DIK_SPACE) & DIS_Down)
		{
			if (m_bIsModelMove) m_bIsModelMove = 0;
			else
			{

				// 해당 오브젝트의 메트릭스 받아오기
				m_SelectedObjectSRT = &(m_vecBatchedObject[m_iBatchedVecIndex].matSRT);
				m_fFrustumRange = m_vecBatchedObject[m_iBatchedVecIndex].FrustumRange;
				m_bIsOcllusion = m_vecBatchedObject[m_iBatchedVecIndex].bIsOcllsuion;
				m_iPassIndex = m_vecBatchedObject[m_iBatchedVecIndex].PassIndex;


				ZeroMemory(m_ArrBuffer, sizeof(_float) * 4);
				m_ArrBuffer[3] = 0.1f;

				m_bIsModelMove = 1;

			}

		}


		if (m_bIsModelMove)
		{
			if (pInstance->Get_DIKeyState(DIK_1) & DIS_Down)		m_iKindsOfMoving = 0;
			else if (pInstance->Get_DIKeyState(DIK_2) & DIS_Down)	m_iKindsOfMoving = 1;
			else if (pInstance->Get_DIKeyState(DIK_3) & DIS_Down)	m_iKindsOfMoving = 2;



			if (pInstance->Get_DIMouseButtonState(CInput_Device::MBS_RBUTTON) & DIS_Down)
			{
				m_iSelectedXYZ++;
				if (m_iSelectedXYZ > 2)
					m_iSelectedXYZ = 0;
			}

			_long fWheelMove = pInstance->Get_DIMouseMoveState(CInput_Device::MMS_WHEEL);
			if (fWheelMove)
			{
				_float tempValue;
				memcpy(&tempValue, ((_float*)(&(m_SelectedObjectSRT->m[2 - m_iKindsOfMoving])) + m_iSelectedXYZ), sizeof(_float));

				if (fWheelMove > 0)
					tempValue += _float(m_ArrBuffer[3]);
				else
					tempValue += _float(-m_ArrBuffer[3]);
				//tempValue += _float(fWheelMove * m_ArrBuffer[3] * fDeltaTime);


				FAILED_CHECK(RenewElenmetTransform(&(m_vecBatchedObject[m_iBatchedVecIndex])));
				memcpy(((_float*)(&(m_SelectedObjectSRT->m[2 - m_iKindsOfMoving])) + m_iSelectedXYZ), &tempValue, sizeof(_float));

			}

		}
		else
		{
			if (pInstance->Get_DIMouseButtonState(CInput_Device::MBS_LBUTTON) & DIS_DoubleDown)
			{
				POINT ptMouse;
				GetCursorPos(&ptMouse);
				ScreenToClient(g_hWnd, &ptMouse);



				_Vector vCursorPos = XMVectorSet(
					(_float(ptMouse.x) / (g_iWinCX * 0.5f)) - 1.f,
					(_float(ptMouse.y) / -(g_iWinCY * 0.5f)) + 1.f,
					0, 1.f);
				//_Vector vCursorPos = XMVectorSet(	(_float(ptMouse.x) / g_iWinCX * 0.5f) - 1.f, 	(_float(ptMouse.y) / g_iWinCY * 0.5f) + 1.f ,		0, 1.f);

				_Matrix InvProjMat = XMMatrixInverse(nullptr, pInstance->Get_Transform_Matrix(PLM_PROJ));

				_Vector vRayDir = XMVector4Transform(vCursorPos, InvProjMat) - XMVectorSet(0, 0, 0, 1);

				_Matrix InvViewMat = XMMatrixInverse(nullptr, pInstance->Get_Transform_Matrix(PLM_VIEW));
				vRayDir = XMVector3TransformNormal(vRayDir, InvViewMat);


				if (m_pCreatedTerrain)
				{
					_Vector vCamPos = m_pEditorCam->Get_Camera_Transform()->Get_MatrixState(CTransform::STATE_POS);

					_Vector vOldPos = vCamPos;
					_Vector vNewPos;
					_float3 vResult;
					_bool IsPicked = false;


					for (_uint i = 0; i < 200; i++)
					{
						vNewPos = vOldPos + vRayDir;

						vResult = m_pCreatedTerrain->Pick_OnTerrain(&IsPicked, vNewPos, vOldPos);

						if (IsPicked)
						{

							//wstring ResultString = L"X : " + to_wstring(vResult.x) + L"	Y : " + to_wstring(vResult.y) + L"	Z : " + to_wstring(vResult.z) + L"\n";
							//OutputDebugStringW(ResultString.c_str());
							m_fPickingedPosition[0] = vResult.x;
							m_fPickingedPosition[1] = vResult.y;
							m_fPickingedPosition[2] = vResult.z;
							break;
						}

						vOldPos = vNewPos;
					}

					if (IsPicked && !m_iBatchedVecIndex)
					{
						memcpy(m_vecBatchedObject[0].matSRT.m[2], &vResult, sizeof(_float3));

						RenewElenmetTransform(&m_vecBatchedObject[0]);
					}
				}
				else
				{
					_Vector vCamPos = m_pEditorCam->Get_Camera_Transform()->Get_MatrixState(CTransform::STATE_POS);

					if (XMVectorGetY(vCamPos) * XMVectorGetY(vRayDir) < 0)
					{
						_float Scale = (XMVectorGetY(vCamPos)  )/ - (XMVectorGetY(vRayDir)  );

						_float3 vTargetPos = vCamPos + (Scale) * vRayDir;

						m_fPickingedPosition[0] = vTargetPos.x;
						m_fPickingedPosition[1] = vTargetPos.y;
						m_fPickingedPosition[2] = vTargetPos.z;

						if (!m_iBatchedVecIndex)
						{

							memcpy(m_vecBatchedObject[0].matSRT.m[2], &vTargetPos, sizeof(_float3));

							RenewElenmetTransform(&m_vecBatchedObject[0]);
						}

					}
				}

			}

		}



#pragma endregion MapTab
	}

	else if (m_iNowTab == 4)
	{
#pragma region HeightMap


#pragma endregion HeightMap
	}
	else if (m_iNowTab == 1)
	{
#pragma region UITOOL

		if (pInstance->Get_DIKeyState(DIK_TAB) & DIS_Down) m_bIsRect = !m_bIsRect;

		
			if (pInstance->Get_DIKeyState(DIK_LSHIFT) & DIS_Press)
			{
				if (!m_bIsRect)
				{
					if (pInstance->Get_DIMouseButtonState(CInput_Device::MBS_LBUTTON) & DIS_Press)
					{

						POINT ptMouse;
						GetCursorPos(&ptMouse);
						ScreenToClient(g_hWnd, &ptMouse);

						m_fUIDesc.fX = _float(ptMouse.x);
						m_fUIDesc.fY = _float(ptMouse.y);
					}


					static bool bIsY = false;

					if (pInstance->Get_DIMouseButtonState(CInput_Device::MBS_WHEEL) & DIS_Down)
						bIsY = !bIsY;

					_long fWheelMove = pInstance->Get_DIMouseMoveState(CInput_Device::MMS_WHEEL);

					if (fWheelMove)
					{
						if (bIsY)
							m_fUIDesc.fCY += _float(fWheelMove *0.21f * fDeltaTime);
						else
							m_fUIDesc.fCX += _float(fWheelMove *0.21f * fDeltaTime);


					}

				}
				else
				{
					if (pInstance->Get_DIMouseButtonState(CInput_Device::MBS_LBUTTON) & DIS_Down)
					{

						POINT ptMouse;
						GetCursorPos(&ptMouse);
						ScreenToClient(g_hWnd, &ptMouse);

						m_fUIRect.left = _float(ptMouse.x);
						m_fUIRect.top = _float(ptMouse.y);
					}
					if (pInstance->Get_DIMouseButtonState(CInput_Device::MBS_LBUTTON) & DIS_Press)
					{

						POINT ptMouse;
						GetCursorPos(&ptMouse);
						ScreenToClient(g_hWnd, &ptMouse);

						m_fUIRect.right = _float(ptMouse.x);
						m_fUIRect.bottom = _float(ptMouse.y);
					}

				}
			}
		
			if (pInstance->Get_DIMouseButtonState(CInput_Device::MBS_RBUTTON) & DIS_Down)
			{

				POINT ptMouse;
				GetCursorPos(&ptMouse);
				ScreenToClient(g_hWnd, &ptMouse);
				m_bModifyBatchedUI = false;


				for (_uint i = _uint(m_vecBatchedUI.size() - 1); i >= 0 ; i--)
				{
					
					if (ptMouse.x > m_vecBatchedUI[i].UIRect.left	&& ptMouse.x < m_vecBatchedUI[i].UIRect.right &&
						ptMouse.y > m_vecBatchedUI[i].UIRect.top	&& ptMouse.y < m_vecBatchedUI[i].UIRect.bottom)
					{
						m_bModifyBatchedUI = true;


						m_fUIDesc = m_vecBatchedUI[i].UIDesc;

						m_bIsRect=	m_vecBatchedUI[i].bIsRect;
						m_fUIAngle=	m_vecBatchedUI[i].fAngle ;
						m_fUIRect=	m_vecBatchedUI[i].UIRect ;



						if (m_TargetSRV != nullptr)
						{

							Safe_Release(m_TargetSRV);

							m_TargetSRV = m_vecBatchedUI[i].SRV;

							Safe_AddRef(m_TargetSRV);

						}




						m_iModifyUIIndex = i;




						break;
					}
					
				}

				

			}
	
			if (pInstance->Get_DIKeyState(DIK_R) & DIS_Down)
				m_bModifyBatchedUI = false;
			

		

			
	
#pragma endregion UITOOL
	}

	else if (m_iNowTab == 2)
	{
#pragma region Particle
		if (pInstance->Get_DIMouseButtonState(CInput_Device::MBS_RBUTTON) & DIS_DoubleDown)
		{
			POINT ptMouse;
			GetCursorPos(&ptMouse);
			ScreenToClient(g_hWnd, &ptMouse);



			_Vector vCursorPos = XMVectorSet(
				(_float(ptMouse.x) / (g_iWinCX * 0.5f)) - 1.f,
				(_float(ptMouse.y) / -(g_iWinCY * 0.5f)) + 1.f,
				0, 1.f);
			//_Vector vCursorPos = XMVectorSet(	(_float(ptMouse.x) / g_iWinCX * 0.5f) - 1.f, 	(_float(ptMouse.y) / g_iWinCY * 0.5f) + 1.f ,		0, 1.f);

			_Matrix InvProjMat = XMMatrixInverse(nullptr, pInstance->Get_Transform_Matrix(PLM_PROJ));

			_Vector vRayDir = XMVector4Transform(vCursorPos, InvProjMat) - XMVectorSet(0, 0, 0, 1);

			_Matrix InvViewMat = XMMatrixInverse(nullptr, pInstance->Get_Transform_Matrix(PLM_VIEW));
			vRayDir = XMVector3TransformNormal(vRayDir, InvViewMat);


			_Vector vCamPos = m_pEditorCam->Get_Camera_Transform()->Get_MatrixState(CTransform::STATE_POS);

			if (XMVectorGetY(vCamPos) * XMVectorGetY(vRayDir) < 0)
			{
				_float Scale = (XMVectorGetY(vCamPos)) / -(XMVectorGetY(vRayDir));

				_float3 vTargetPos = vCamPos + (Scale)* vRayDir;

				m_fPickingedPosition[0] = vTargetPos.x;
				m_fPickingedPosition[1] = vTargetPos.y;
				m_fPickingedPosition[2] = vTargetPos.z;

				if (!m_iBatchedVecIndex)
				{

					memcpy(m_vecBatchedObject[0].matSRT.m[2], &vTargetPos, sizeof(_float3));

					RenewElenmetTransform(&m_vecBatchedObject[0]);
				}

			}


		}
#pragma endregion Particle
	}
	else if (m_iNowTab == 3)
	{
#pragma region CamAction
		if (pInstance->Get_DIMouseButtonState(CInput_Device::MBS_LBUTTON) & DIS_DoubleDown)
		{
			POINT ptMouse;
			GetCursorPos(&ptMouse);
			ScreenToClient(g_hWnd, &ptMouse);


			_Vector vCursorPos = XMVectorSet(
				(_float(ptMouse.x) / (g_iWinCX * 0.5f)) - 1.f,
				(_float(ptMouse.y) / -(g_iWinCY * 0.5f)) + 1.f,
				0, 1.f);
			//_Vector vCursorPos = XMVectorSet(	(_float(ptMouse.x) / g_iWinCX * 0.5f) - 1.f, 	(_float(ptMouse.y) / g_iWinCY * 0.5f) + 1.f ,		0, 1.f);

			_Matrix InvProjMat = XMMatrixInverse(nullptr, pInstance->Get_Transform_Matrix(PLM_PROJ));

			_Vector vRayDir = XMVector4Transform(vCursorPos, InvProjMat) - XMVectorSet(0, 0, 0, 1);

			_Matrix InvViewMat = XMMatrixInverse(nullptr, pInstance->Get_Transform_Matrix(PLM_VIEW));
			vRayDir = XMVector3TransformNormal(vRayDir, InvViewMat);


			if (m_pCreatedTerrain)
			{
				_Vector vCamPos = m_pEditorCam->Get_Camera_Transform()->Get_MatrixState(CTransform::STATE_POS);

				_Vector vOldPos = vCamPos;
				_Vector vNewPos;
				_float3 vResult;
				_bool IsPicked = false;


				for (_uint i = 0; i < 200; i++)
				{
					vNewPos = vOldPos + vRayDir;

					vResult = m_pCreatedTerrain->Pick_OnTerrain(&IsPicked, vNewPos, vOldPos);

					if (IsPicked)
					{

						//wstring ResultString = L"X : " + to_wstring(vResult.x) + L"	Y : " + to_wstring(vResult.y) + L"	Z : " + to_wstring(vResult.z) + L"\n";
						//OutputDebugStringW(ResultString.c_str());
						m_fPickingedPosition[0] = vResult.x;
						m_fPickingedPosition[1] = vResult.y + 1.5f;
						m_fPickingedPosition[2] = vResult.z;
						break;
					}

					vOldPos = vNewPos;
				}

				if (IsPicked)
				{

					CamDesc.vPosition = vResult;
					CamDesc.vPosition.y += 1.5f;
				}
			}
			else
			{
				_Vector vCamPos = m_pEditorCam->Get_Camera_Transform()->Get_MatrixState(CTransform::STATE_POS);

				if (XMVectorGetY(vCamPos) * XMVectorGetY(vRayDir) < 0)
				{
					_float Scale = XMVectorGetY(vCamPos) / -XMVectorGetY(vRayDir);

					_float3 vTargetPos = vCamPos + Scale * vRayDir;

					m_fPickingedPosition[0] = vTargetPos.x;
					m_fPickingedPosition[1] = vTargetPos.y + 1.5f;;
					m_fPickingedPosition[2] = vTargetPos.z;

					CamDesc.vPosition = vTargetPos;
					CamDesc.vPosition.y += 1.5f;
				}
			}

		}
#pragma endregion CamAction
	}

#pragma region HeightMap
#pragma endregion HeightMap

	return S_OK;
}

#pragma endregion Total


#pragma region MapTab
HRESULT CScene_Edit::Update_MapTab(_double fDeltatime)
{

	FAILED_CHECK(Widget_SRT(fDeltatime));

	Make_VerticalSpacing(2);
	ImGui::Separator();
	Make_VerticalSpacing(3);

	int open_action = -1;
	if (ImGui::Button("Open all about Object"))
		open_action = 1;
	ImGui::SameLine();
	if (ImGui::Button("Close all about Object"))
		open_action = 0;
	if (open_action != -1)
		ImGui::SetNextItemOpen(open_action != 0);
	FAILED_CHECK(Widget_BatchedObjectList(fDeltatime));

	if (open_action != -1)
		ImGui::SetNextItemOpen(open_action != 0);
	FAILED_CHECK(Widget_CreateDeleteObject(fDeltatime));


	Make_VerticalSpacing(2);
	ImGui::Separator();

	Make_VerticalSpacing(3);

	//FAILED_CHECK(Widget_CreateDeleteHeightMap(fDeltatime));
	Make_VerticalSpacing(3);
	FAILED_CHECK(Widget_SaveLoadMapData(fDeltatime));

	return S_OK;
}

HRESULT CScene_Edit::Widget_SRT(_double fDeltatime)
{
	Make_VerticalSpacing(3);

	char PickingPos[128] = "";
	if (m_pCreatedTerrain)
	{
		sprintf_s(PickingPos, "Picked Position : ( %f   , %f   , %f   )\n",
			m_fPickingedPosition[0], m_fPickingedPosition[1], m_fPickingedPosition[2]);
	}
	else
	{
	sprintf_s(PickingPos, "Picked Position : ( %f   , %f   , %f   )\n", m_fPickingedPosition[0], m_fPickingedPosition[1], m_fPickingedPosition[2]);
	}
	ImGui::Text(PickingPos);

	Make_VerticalSpacing(3);

	ibClickChecker = 0;
	ImGui::RadioButton("Camera Moving", &m_bIsModelMove, 0); ImGui::SameLine();
	ImGui::RadioButton("Model Moving      ", &m_bIsModelMove, 1); ImGui::SameLine();
	ibClickChecker += _uint(ImGui::IsItemClicked());

	//if (m_iBatchedVecIndex == 0)
	//{
	//	m_bIsModelMove = 0;
	//}
	//else
	if(m_bIsModelMove)
	{
		if (ibClickChecker)
		{
			/////////////////////////////////////모델 정보 받아오기

			m_SelectedObjectSRT = &(m_vecBatchedObject[m_iBatchedVecIndex].matSRT);
			m_fFrustumRange = m_vecBatchedObject[m_iBatchedVecIndex].FrustumRange;
			m_bIsOcllusion = m_vecBatchedObject[m_iBatchedVecIndex].bIsOcllsuion;
			m_iPassIndex = m_vecBatchedObject[m_iBatchedVecIndex].PassIndex;

		}
	}




	ImGui::TableNextColumn(); ImGui::Checkbox("BG X", &bArrWindowFlag[0]); ImGui::SameLine();
	ImGui::TableNextColumn(); ImGui::Checkbox("Move X\n\n\n", &bArrWindowFlag[1]);
	/*
	ImGui::TableNextColumn(); ImGui::Checkbox("No titlebar", &bArrWindowFlag[2]);
	ImGui::TableNextColumn(); ImGui::Checkbox("No scrollbar", &bArrWindowFlag[3]);
	ImGui::TableNextColumn(); ImGui::Checkbox("No menu", &bArrWindowFlag[4]);
	ImGui::TableNextColumn(); ImGui::Checkbox("No resize", &bArrWindowFlag[5]);
	ImGui::TableNextColumn(); ImGui::Checkbox("No collapse", &bArrWindowFlag[6]);
	ImGui::TableNextColumn(); ImGui::Checkbox("No nav", &bArrWindowFlag[7]);
	ImGui::TableNextColumn(); ImGui::Checkbox("No bring to front", &bArrWindowFlag[8]);
	ImGui::TableNextColumn(); ImGui::Checkbox("Unsaved document", &bArrWindowFlag[9]);
	*/

	if (!m_bIsModelMove)
		ImGui::BeginDisabled(true);

	ibClickChecker = 0;
	ImGui::RadioButton("Position", &m_iKindsOfMoving, 0); ImGui::SameLine();
	ibClickChecker += _uint(ImGui::IsItemClicked());
	ImGui::RadioButton("Rotation", &m_iKindsOfMoving, 1); ImGui::SameLine();
	ibClickChecker += _uint(ImGui::IsItemClicked());
	ImGui::RadioButton("Scale\n\n\n", &m_iKindsOfMoving, 2);
	ibClickChecker += _uint(ImGui::IsItemClicked());

	if (ibClickChecker)
	{
		ZeroMemory(m_ArrBuffer, sizeof(_float) * 4);
		m_ArrBuffer[3] = 0.1f;
	}

	ImGui::DragFloat("Drag Speed", &(m_ArrBuffer[3]), 0.1f, FLT_MIN, FLT_MAX);
	{

		char Label[64];
		if (m_iBatchedVecIndex)
			sprintf_s(Label, "\n  Selected Object Index : %d", m_iBatchedVecIndex);
		else
			sprintf_s(Label, "\n  !!!!!!!!!!!Create New Object!!!!!!!!!");



		ImGui::Text(Label);
	}
	switch (m_iSelectedXYZ)
	{
	case 0:
		ImGui::Text("  Position		| Selected : X");
		break;

	case 1:
		ImGui::Text("  Rotation		| Selected : Y");
		break;

	case 2:
		ImGui::Text("  Scale			| Selected : Z");
		break;

	default:
		break;
	}


	switch (m_iKindsOfMoving)
	{
	case 0://포지션
	{
		memcpy(m_ArrBuffer, &(m_SelectedObjectSRT->m[2]), sizeof(_float) * 3);
		ImGui::DragFloat3(" X Y Z", m_ArrBuffer, m_ArrBuffer[3], -FLT_MAX, FLT_MAX);

		memcpy(&(m_SelectedObjectSRT->m[2]), m_ArrBuffer, sizeof(_float) * 3);
	}
	break;

	case 1://로테이션
	{
		memcpy(m_ArrBuffer, &(m_SelectedObjectSRT->m[1]), sizeof(_float) * 3);
		ImGui::DragFloat3(" X Y Z", m_ArrBuffer, m_ArrBuffer[3], -FLT_MAX, FLT_MAX);

		memcpy(&(m_SelectedObjectSRT->m[1]), m_ArrBuffer, sizeof(_float) * 3);

	}
	break;


	case 2://Scale
	{
		memcpy(m_ArrBuffer, &(m_SelectedObjectSRT->m[0]), sizeof(_float) * 3);
		ImGui::DragFloat3(" X Y Z", m_ArrBuffer, m_ArrBuffer[3], -FLT_MAX, FLT_MAX);

		memcpy(&(m_SelectedObjectSRT->m[0]), m_ArrBuffer, sizeof(_float) * 3);
	}
	break;


	default:
		break;
	}
	Make_VerticalSpacing(2);

	ImGui::Checkbox("IsOcllusion", &m_bIsOcllusion);

	ImGui::DragFloat("FrustumRange ", &m_fFrustumRange, 0.003f, 0, FLT_MAX);

	ImGui::InputInt("PassIndex ", &m_iPassIndex);

	Make_VerticalSpacing(2);

	if (m_bIsModelMove)
		FAILED_CHECK(RenewElenmetTransform(&(m_vecBatchedObject[m_iBatchedVecIndex])));



	/*회전값 용

	D3DXQUATERNION quat_x, quat_y, quat_z, quat_1, quat_2;
	_float4x4 matrix;
	D3DXVECTOR3 axis_x(1.0f, 0.0f, 0.0f);
	D3DXVECTOR3 axis_y(0.0f, 1.0f, 0.0f);
	D3DXVECTOR3 axis_z(0.0f, 0.0f, 1.0f);
	D3DXQuaternionRotationAxis(&quat_x, &axis_x, rot.x);
	D3DXQuaternionRotationAxis(&quat_y, &axis_y, rot.y);
	D3DXQuaternionRotationAxis(&quat_z, &axis_z, rot.z);

	quat_1 = quat_y * quat_x * quat_z;
	D3DXQuaternionNormalize(&quat_2, &quat_1);
	D3DXMatrixRotationQuaternion(&matrix, &quat_2);

	m_ComTransform->Set_MatrixState(CTransform::STATE_RIGHT, *(_float3*)&matrix.m[0]);
	m_ComTransform->Set_MatrixState(CTransform::STATE_UP, *(_float3*)&matrix.m[1]);
	m_ComTransform->Set_MatrixState(CTransform::STATE_LOOK, *(_float3*)&matrix.m[2]);
	*/

	Make_VerticalSpacing(5);
	if (ImGui::TreeNode("Show All"))
	{

		static bool selected[4] = {};

		if (ImGui::BeginTable("split2", 4, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders))
		{
			char HederLabel[32];
			sprintf_s(HederLabel, "SRT Type");
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Selectable(HederLabel, &selected[0], ImGuiSelectableFlags_SpanAllColumns);
			ImGui::TableNextColumn();
			ImGui::Text("X");
			ImGui::TableNextColumn();
			ImGui::Text("Y");
			ImGui::TableNextColumn();
			ImGui::Text("Z");

			for (int i = 0; i < 3; i++)
			{
				char label[32];
				switch (i)
				{
				case 0:
					sprintf_s(label, "Position");
					break;
				case 1:
					sprintf_s(label, "Rotation");
					break;
				case 2:
					sprintf_s(label, "Scale");
					break;
				default:
					sprintf_s(label, "ERROR");
					break;
				}

				_float3 vSRT = {};
				memcpy(&vSRT, &(m_SelectedObjectSRT->m[2 - i]), sizeof(_float) * 3);

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Selectable(label, &selected[i + 1], ImGuiSelectableFlags_SpanAllColumns);
				ImGui::TableNextColumn();
				ImGui::Text("%f", vSRT.x);
				ImGui::TableNextColumn();
				ImGui::Text("%f", vSRT.y);
				ImGui::TableNextColumn();
				ImGui::Text("%f", vSRT.z);
			}
			ImGui::EndTable();
		}


		ImGui::TreePop();
	}

	if (!m_bIsModelMove)
		ImGui::EndDisabled();

	return S_OK;
}



HRESULT CScene_Edit::Widget_BatchedObjectList(_double fDeltatime)
{


	Make_VerticalSpacing(3);
	if (ImGui::TreeNode("Batched Object List"))
	{
		vector<string> ObjectLabelIist;

		for (_uint i = 0; i < m_vecBatchedObject.size(); i++)
		{
			char HeaderLabel[MAX_PATH];

			sprintf_s(HeaderLabel, "%d. %ws (Mesh : %ws) (PassIndex : %d)", i,
				m_vecBatchedObject[i].ObjectID,
				m_vecBatchedObject[i].MeshID ,  
				m_vecBatchedObject[i].PassIndex
			);
			//sprintf_s(HederLabel, "%d. %ws (%d)",i, m_vecBatchedObject[i].pObject->Get_NameTag(), m_vecBatchedObject[i].ObjectID);

			ObjectLabelIist.push_back({ HeaderLabel });
		}


		{

			char Label[64];
			sprintf_s(Label, "Selected Index : %d  | Total Object Count : %zd", m_iBatchedVecIndex, ObjectLabelIist.size() - 1);

			ImGui::Text(Label);
		}
		if (ImGui::BeginListBox("##listbox 2", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
		{


			for (int n = 1; n < ObjectLabelIist.size(); n++)
			{
				const bool is_selected = (m_iBatchedVecIndex == n);
				if (ImGui::Selectable(ObjectLabelIist[n].c_str(), is_selected))
				{


					m_iBatchedVecIndex = n;
					m_SelectedObjectSRT = &(m_vecBatchedObject[m_iBatchedVecIndex].matSRT);
					m_bIsOcllusion = m_vecBatchedObject[m_iBatchedVecIndex].bIsOcllsuion;
					m_fFrustumRange = m_vecBatchedObject[m_iBatchedVecIndex].FrustumRange;
					m_iPassIndex = m_vecBatchedObject[m_iBatchedVecIndex].PassIndex;
				}


				if (m_bIsModelMove != 0 && ImGui::IsItemHovered())
				{
					m_bIsModelMove = 0;
				}

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndListBox();
		}



		Make_VerticalSpacing(2);

		if (ImGui::Button("Delete Object", ImVec2(-FLT_MIN, 0.0f)) && m_iBatchedVecIndex)
		{

			auto iter = m_vecBatchedObject.begin();

			for (_uint i = 0; i < m_iBatchedVecIndex; i++)
				iter++;

			////실제로 죽이기 없애기 delete//////////////////////////////////////////////////////////////////////
			Safe_Release(iter->pObject);

			m_vecBatchedObject.erase(iter);

			m_iBatchedVecIndex = 0;
			m_SelectedObjectSRT = &(m_vecBatchedObject[m_iBatchedVecIndex].matSRT);
			m_bIsOcllusion = m_vecBatchedObject[m_iBatchedVecIndex].bIsOcllsuion;
			m_fFrustumRange = m_vecBatchedObject[m_iBatchedVecIndex].FrustumRange;
			m_iPassIndex = m_vecBatchedObject[m_iBatchedVecIndex].PassIndex;


			m_iBatchedVecIndex = _uint(m_vecBatchedObject.size() - 1);
			m_SelectedObjectSRT = &(m_vecBatchedObject[m_iBatchedVecIndex].matSRT);
			m_bIsOcllusion = m_vecBatchedObject[m_iBatchedVecIndex].bIsOcllsuion;
			m_fFrustumRange = m_vecBatchedObject[m_iBatchedVecIndex].FrustumRange;
			m_iPassIndex = m_vecBatchedObject[m_iBatchedVecIndex].PassIndex;
		}



		ImGui::TreePop();
	}

	return S_OK;
}

HRESULT CScene_Edit::Widget_CreateDeleteObject(_double fDeltatime)
{

	Make_VerticalSpacing(5);
	if (ImGui::TreeNode("Create Object"))
	{


		ImGui::Text("Selected\n\n");

		{
			char buf[MAX_PATH];
			sprintf_s(buf, "%ws (Mesh : %ws)",
				TAG_OP(OBJECTPROTOTYPEID(m_iSelectedObjectNMesh[0])),
				TAG_CP(COMPONENTPROTOTYPEID(m_iSelectedObjectNMesh[1]))
			);

			ImGui::Text(buf);
		}


		{
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_None | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_HorizontalScrollbar;
			{

				ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
				ImGui::BeginChild("ChildL", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, 330), true, window_flags);


				if (ImGui::BeginMenuBar())
				{
					if (ImGui::BeginMenu("ObjectList"))
					{
						ImGui::EndMenu();
					}
					ImGui::EndMenuBar();

					//char buf[128];
					//sprintf_s(buf, "%ws\n", TAG_OP(OBJECTPROTOTYPEID(m_iSelectedObjectNMesh[0])));
					//ImGui::Text(buf);

				}

				if (ImGui::BeginTable("split", 1, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings))
				{
					for (int i = Prototype_StaticMapObject; i >= Prototype_StaticMapObject; i--)
					{
						
						char buf[MAX_PATH];
						sprintf_s(buf, "%ws", TAG_OP(OBJECTPROTOTYPEID(i)));
						ImGui::TableNextColumn();

						if (ImGui::Button(buf, ImVec2(-FLT_MIN, 0.0f)))
							m_iSelectedObjectNMesh[0] = i;

						if (m_iBatchedVecIndex != 0 && ImGui::IsItemHovered())
						{
							m_iBatchedVecIndex = 0;
							m_bIsModelMove = 0;
							m_SelectedObjectSRT = &(m_vecBatchedObject[m_iBatchedVecIndex].matSRT);
							m_bIsOcllusion = m_vecBatchedObject[m_iBatchedVecIndex].bIsOcllsuion;
							m_fFrustumRange = m_vecBatchedObject[m_iBatchedVecIndex].FrustumRange;
							m_iPassIndex = m_vecBatchedObject[m_iBatchedVecIndex].PassIndex;

							ZeroMemory(m_iSelectedObjectNMesh, sizeof(_uint) * 2);
							m_iSelectedObjectNMesh[0] = Prototype_StaticMapObject;
							m_iSelectedObjectNMesh[1] = Prototype_Mesh_None;

						}
					}
					ImGui::EndTable();
				}
				ImGui::EndChild();
				ImGui::PopStyleVar();
			}

			ImGui::SameLine();

			{
				ImGui::BeginChild("ChildR", ImVec2(0, 330), true, window_flags);


				if (ImGui::BeginMenuBar())
				{
					if (ImGui::BeginMenu("Mesh List"))
					{
						ImGui::EndMenu();
					}
					ImGui::EndMenuBar();

					//char buf[128];
					//sprintf_s(buf, "%ws\n", TAG_MESH(MESHTYPEID(m_iSelectedObjectNMesh[1])));
					//ImGui::Text(buf);
				}

				if (ImGui::BeginTable("split", 1, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings))
				{

					for (int i = Prototype_Mesh_None + 1; i < Prototype_Mesh_Player; i++)
					{
						char buf[MAX_PATH];
						sprintf_s(buf, "%ws", TAG_CP(COMPONENTPROTOTYPEID(i)));
						ImGui::TableNextColumn();
						if (ImGui::Button(buf, ImVec2(-FLT_MIN, 0.0f)))
							m_iSelectedObjectNMesh[1] = i;

						if (m_iBatchedVecIndex != 0 && ImGui::IsItemHovered())
						{
							m_iBatchedVecIndex = 0; m_bIsModelMove = 0;
							m_SelectedObjectSRT = &(m_vecBatchedObject[m_iBatchedVecIndex].matSRT);
							m_bIsOcllusion = m_vecBatchedObject[m_iBatchedVecIndex].bIsOcllsuion;
							m_fFrustumRange = m_vecBatchedObject[m_iBatchedVecIndex].FrustumRange;
							m_iPassIndex = m_vecBatchedObject[m_iBatchedVecIndex].PassIndex;
							ZeroMemory(m_iSelectedObjectNMesh, sizeof(_uint) * 2);
							m_iSelectedObjectNMesh[0] = Prototype_StaticMapObject;
							m_iSelectedObjectNMesh[1] = Prototype_Mesh_None;

						}
					}


					ImGui::EndTable();



					Make_VerticalSpacing(5);


				}

				Make_VerticalSpacing(1);

				ImGui::EndChild();
				Make_VerticalSpacing(1);
			}

			Make_VerticalSpacing(5);
			static ImGuiTextFilter filter;

			char	szCheckforSameFileName[256] = "";

			if (ImGui::BeginListBox("    "))
			{

				for (int i = Prototype_Mesh_None + 1; i < Prototype_Mesh_Player; i++)
				{
					const bool is_selected = false;

					char buf[MAX_PATH];
					sprintf_s(buf, "%ws", TAG_CP(COMPONENTPROTOTYPEID(i)));

					if (filter.PassFilter(buf))
					{
						if (ImGui::Selectable(buf, is_selected))
						{
							//strcpy_s(filter.InputBuf, buf);

							m_iSelectedObjectNMesh[1] = i;

						}


						if (m_iBatchedVecIndex != 0 && ImGui::IsItemHovered())
						{
							m_iBatchedVecIndex = 0; m_bIsModelMove = 0;
							m_SelectedObjectSRT = &(m_vecBatchedObject[m_iBatchedVecIndex].matSRT);
							m_bIsOcllusion = m_vecBatchedObject[m_iBatchedVecIndex].bIsOcllsuion;
							m_fFrustumRange = m_vecBatchedObject[m_iBatchedVecIndex].FrustumRange;
							m_iPassIndex = m_vecBatchedObject[m_iBatchedVecIndex].PassIndex;
							ZeroMemory(m_iSelectedObjectNMesh, sizeof(_uint) * 2);
							m_iSelectedObjectNMesh[0] = Prototype_StaticMapObject;
							m_iSelectedObjectNMesh[1] = Prototype_Mesh_None;

						}

						//if (!strcmp(buf, filter.InputBuf))
						//{
						//	strcpy_s(szCheckforSameFileName, filter.InputBuf);

						//}
					}
				}
				ImGui::EndListBox();
			}
			filter.Draw("Input FileName");
		}

		Make_VerticalSpacing(2);
		if (ImGui::Button("Create Object", ImVec2(-FLT_MIN, 0.0f)))
		{
			///오브젝트 생성 코드 집어넣기

			OBJELEMENT ObjElement;




			ZeroMemory(ObjElement.matSRT.m, sizeof(_float) * 16);

			lstrcpy(ObjElement.ObjectID, TAG_OP((OBJECTPROTOTYPEID)m_iSelectedObjectNMesh[0]));
			//ObjElement.ObjectID = TAG_OP((OBJECTPROTOTYPEID) m_iSelectedObjectNMesh[0]);

			ObjElement.matSRT = m_vecBatchedObject[m_iBatchedVecIndex].matSRT;
			ObjElement.PassIndex = m_iPassIndex;
			ObjElement.FrustumRange = m_fFrustumRange;
			ObjElement.bIsOcllsuion = m_bIsOcllusion;


			lstrcpy(ObjElement.MeshID, TAG_CP((COMPONENTPROTOTYPEID)m_iSelectedObjectNMesh[1]));
			//ObjElement.MeshID = TAG_MESH( (MESHTYPEID) m_iSelectedObjectNMesh[1]);
			/////실제 생성하기

			FAILED_CHECK(m_pGameInstance->Add_GameObject_Out_of_Manager(&(ObjElement.pObject), SCENE_EDIT, ObjElement.ObjectID));

			FAILED_CHECK(RenewElenmetTransform(&ObjElement));

			if (lstrcmp(ObjElement.MeshID, TAG_CP(Prototype_Mesh_None)))
			{
				//매쉬 바꿔주기 
				ObjElement.pObject->Change_Component_by_NewAssign(SCENE_EDIT, ObjElement.MeshID, TAG_COM(Com_Model));
				((CMapObject*)ObjElement.pObject)->Set_FrustumSize(m_fFrustumRange);
				((CMapObject*)ObjElement.pObject)->Set_PassIndex(m_iPassIndex);

				CDissolve::DISSOLVEDESC	tDissolveDesc;

				tDissolveDesc.eDissolveModelType = CDissolve::DISSOLVE_NONANIM;
				tDissolveDesc.pModel = (CModel*)ObjElement.pObject->Get_Component(TAG_COM(Com_Model));
				tDissolveDesc.pShader = (CShader*)ObjElement.pObject->Get_Component(TAG_COM(Com_Shader));
				tDissolveDesc.RampTextureIndex = 1;

				ObjElement.pObject->Change_Component_by_NewAssign(SCENE_EDIT, TAG_CP(Prototype_Dissolve), TAG_COM(Com_Dissolve),&tDissolveDesc);


			}

			m_vecBatchedObject.push_back(ObjElement);

			m_iBatchedVecIndex = _uint(m_vecBatchedObject.size() - 1);
			m_SelectedObjectSRT = &(m_vecBatchedObject[m_iBatchedVecIndex].matSRT);
			m_bIsOcllusion = m_vecBatchedObject[m_iBatchedVecIndex].bIsOcllsuion;
			m_fFrustumRange = m_vecBatchedObject[m_iBatchedVecIndex].FrustumRange;
			m_iPassIndex = m_vecBatchedObject[m_iBatchedVecIndex].PassIndex;


		}


		ImGui::TreePop();
	}


	return S_OK;
}

HRESULT CScene_Edit::Widget_SaveLoadMapData(_double fDeltatime)
{
	if (ImGui::Button("New Map"))
		ImGui::OpenPopup("New Map");
	ImGui::SameLine();
	if (ImGui::Button("Save Map"))
		ImGui::OpenPopup("Save Map");
	ImGui::SameLine();
	if (ImGui::Button("Laod Map"))
		ImGui::OpenPopup("Laod Map");
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();

	// Always center this window when appearing
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("New Map", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("!!!!!!!!!!!!!!!!Waring!!!!!!!!!!!!!!!!\n\n Delete Batched Object Without Save!!!\n\n	Please Check Save One more\n\n\n");
		ImGui::Separator();

		//static int unused_i = 0;
		//ImGui::Combo("Combo", &unused_i, "Delete\0Delete harder\0");

		//static bool dont_ask_me_next_time = false;
		//ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
		//ImGui::Checkbox("Don't ask me next time", &dont_ask_me_next_time);
		//ImGui::PopStyleVar();

		if (ImGui::Button("OK", ImVec2(130, 0))) { 

			for (auto& tObjElement : m_vecBatchedObject)
			{
				//오브젝트 제대로 델리트delete 해주기
				Safe_Release(tObjElement.pObject);
			}
			m_vecBatchedObject.clear();

			OBJELEMENT ObjElement;
			
			FAILED_CHECK(m_pGameInstance->Add_GameObject_Out_of_Manager(&ObjElement.pObject, SCENE_EDIT, TAG_OP(Prototype_EditorCursor)));

			ZeroMemory(ObjElement.matSRT.m, sizeof(_float) * 16);

			ObjElement.matSRT.m[0][0] = 1.f;
			ObjElement.matSRT.m[0][1] = 1.f;
			ObjElement.matSRT.m[0][2] = 1.f;
			ObjElement.matSRT.m[1][0] = 1.f;
			ObjElement.matSRT.m[1][1] = 1.f;
			ObjElement.matSRT.m[1][2] = 1.f;

			RenewElenmetTransform(&ObjElement);
			
		
			m_vecBatchedObject.push_back(ObjElement);



			m_SelectedObjectSRT = &(m_vecBatchedObject[0].matSRT);
			m_iBatchedVecIndex = 0;

			ZeroMemory(m_ArrBuffer, sizeof(_float) * 4);
			m_ArrBuffer[3] = 0.1f;
			ZeroMemory(m_iSelectedObjectNMesh, sizeof(_uint) * 2);
			m_iSelectedObjectNMesh[0] = Prototype_StaticMapObject;
			m_iSelectedObjectNMesh[1] = Prototype_Mesh_None;



			m_bIsModelMove = 0;
			m_iKindsOfMoving = 0;
			m_iSelectedXYZ = 0;
			m_iPassIndex = 0;
			m_fFrustumRange = 5.0f;



			ImGui::CloseCurrentPopup(); 
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(130, 0))) { ImGui::CloseCurrentPopup(); }
		ImGui::EndPopup();
	}


	//////////////////////////////////////////////////////////////////////////


	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("Save Map", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{

		if (m_FilePathList.size() == 0)
		{
			m_FilePathList.clear();
			_tfinddata64_t fd;
			__int64 handle = _tfindfirst64(TEXT("../bin/Resources/Data/Map/*.*"), &fd);
			if (handle == -1 || handle == 0)
				return E_FAIL;

			_int iResult = 0;

			//char szCurPath[128] = "../bin/Resources/Data/Map/";
			//char szFullPath[128] = "";
			char szFilename[MAX_PATH];

			while (iResult != -1)
			{
				if (!lstrcmp(fd.name, L".") || !lstrcmp(fd.name, L".."))
				{
					iResult = _tfindnext64(handle, &fd);
					continue;
				}


				WideCharToMultiByte(CP_UTF8, 0, fd.name, -1, szFilename, sizeof(szFilename), NULL, NULL);
				//strcpy_s(szFullPath, szCurPath);
				//strcat_s(szFullPath, szFilename);
				m_FilePathList.push_back({ szFilename });


				iResult = _tfindnext64(handle, &fd);
			}


			_findclose(handle);

		}

		ImGui::Text("Save Map!\n\nExist MapDataFiles");

		static ImGuiTextFilter filter;

		char	szCheckforSameFileName[256]="";

		if (ImGui::BeginListBox(" "))
		{
			auto iter = m_FilePathList.begin();


			for (; iter != m_FilePathList.end(); iter++)
			{
				const bool is_selected = false;

				if (filter.PassFilter(iter->c_str()))
				{
					if (ImGui::Selectable(iter->c_str(), is_selected))
					{
						strcpy_s(filter.InputBuf, iter->c_str());
					}

					if (!strcmp(iter->c_str(), filter.InputBuf))
						strcpy_s(szCheckforSameFileName, filter.InputBuf);
				}
			}
			ImGui::EndListBox();

		}

		filter.Draw("Input FileName");


		ImGui::Separator();
		if (ImGui::Button("Save", ImVec2(120, 0)))
		{

			if (strcmp(filter.InputBuf, ""))
			{

				if (!strcmp(szCheckforSameFileName, filter.InputBuf))
				{
					ImGui::OpenPopup("One More Check");
				}
				else 
				{
					//실제 저장

					Sava_Data(filter.InputBuf, Data_Map);

					ImGui::CloseCurrentPopup();
					m_FilePathList.clear();


				}
			}



		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			ImGui::CloseCurrentPopup();
			m_FilePathList.clear();
		}


		//서브 팝업
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		if (ImGui::BeginPopupModal("One More Check", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{

			ImGui::Text("Mapdata Already Exist\nDo you want to Override on it?");

			if (ImGui::Button("Ok", ImVec2(130, 0)))
			{

				//실제 저장
				Sava_Data(filter.InputBuf, Data_Map);

				ImGui::CloseCurrentPopup();
				m_FilePathList.clear();
			}

			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(130, 0)))
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}


		ImGui::EndPopup();
	}


	//////////////////////////////////////////////////////////////////////////

	// Always center this window when appearing
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("Laod Map", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Laod Map!\n\n");
		ImGui::Separator();


		if (m_FilePathList.size() == 0)
		{
			m_FilePathList.clear();
			_tfinddata64_t fd;
			__int64 handle = _tfindfirst64(TEXT("../bin/Resources/Data/Map/*.*"), &fd);
			if (handle == -1 || handle == 0)
				return E_FAIL;

			_int iResult = 0;

			//char szCurPath[128] = "../bin/Resources/Data/Map/";
			//char szFullPath[128] = "";
			char szFilename[MAX_PATH];

			while (iResult != -1)
			{
				if (!lstrcmp(fd.name, L".") || !lstrcmp(fd.name, L".."))
				{
					iResult = _tfindnext64(handle, &fd);
					continue;
				}


				WideCharToMultiByte(CP_UTF8, 0, fd.name, -1, szFilename, sizeof(szFilename), NULL, NULL);
				//strcpy_s(szFullPath, szCurPath);
				//strcat_s(szFullPath, szFilename);
				m_FilePathList.push_back({ szFilename });


				iResult = _tfindnext64(handle, &fd);
			}


			_findclose(handle);

		}



		static ImGuiTextFilter filter;

		char	szCheckforSameFileName[256] = "";

		if (ImGui::BeginListBox(" "))
		{
			auto iter = m_FilePathList.begin();


			for (; iter != m_FilePathList.end(); iter++)
			{
				const bool is_selected = false;

				if (filter.PassFilter(iter->c_str()))
				{
					if (ImGui::Selectable(iter->c_str(), is_selected))
					{
						strcpy_s(filter.InputBuf, iter->c_str());
					}

					if (!strcmp(iter->c_str(), filter.InputBuf))
						strcpy_s(szCheckforSameFileName, filter.InputBuf);
				}
			}
			ImGui::EndListBox();

		}

		filter.Draw("Input FileName");




		if (ImGui::Button("OK", ImVec2(120, 0)))
		{ 

			if (strcmp(filter.InputBuf, ""))
			{
				if (!strcmp(szCheckforSameFileName, filter.InputBuf))
				{
					Load_Data(filter.InputBuf, Data_Map);
					m_FilePathList.clear();
					ImGui::CloseCurrentPopup();
				}
			}
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0))) {
			m_FilePathList.clear();
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
	return S_OK;
}

HRESULT CScene_Edit::RenewElenmetTransform(OBJELEMENT * pObjElement)
{
	if (pObjElement == nullptr)
		return E_FAIL;

	_Matrix Trans = XMMatrixTranslation(pObjElement->matSRT.m[2][0], pObjElement->matSRT.m[2][1], pObjElement->matSRT.m[2][2]);
	_Matrix RotX = XMMatrixRotationX(XMConvertToRadians(pObjElement->matSRT.m[1][0]));
	_Matrix RotY = XMMatrixRotationY(XMConvertToRadians(pObjElement->matSRT.m[1][1]));
	_Matrix RotZ = XMMatrixRotationZ(XMConvertToRadians(pObjElement->matSRT.m[1][2]));
	_Matrix Scale = XMMatrixScaling((pObjElement->matSRT.m[0][0]), (pObjElement->matSRT.m[0][1]), (pObjElement->matSRT.m[0][2]));


	
	//pObjElement->matTransform = XMMatrixMultiply(XMMatrixMultiply(XMMatrixMultiply((XMMatrixMultiply(Scale, RotX)), RotY), RotZ), Trans);
	pObjElement->matTransform = Scale* RotX *RotY* RotZ* Trans;

	pObjElement->FrustumRange = m_fFrustumRange;
	pObjElement->bIsOcllsuion = m_bIsOcllusion;
	pObjElement->PassIndex = m_iPassIndex;

	if (pObjElement->pObject != nullptr)
	{
		CTransform* pTrans = (CTransform*)(pObjElement->pObject->Get_Component(TAG_COM(Com_Transform)));
		pTrans->Set_Matrix(pObjElement->matTransform);

		if (m_iBatchedVecIndex)
		{
			((CMapObject*)pObjElement->pObject)->Set_FrustumSize(m_fFrustumRange);
			((CMapObject*)pObjElement->pObject)->Set_IsOcllusion(m_bIsOcllusion);
			((CMapObject*)pObjElement->pObject)->Set_PassIndex(m_iPassIndex);
		}
	}

	return S_OK;
}
HRESULT CScene_Edit::Update_TriggerTab(_double fDeltatime)
{



	if (ImGui::BeginListBox("Trigger Object Prototype"))
	{

		for (int i = Prototype_Trigger_ChangeCameraView; i <= Prototype_PlayerSkill_ShellingArrow - 1; i++)
		{
			const bool is_selected = false;

			char buf[MAX_PATH];
			sprintf_s(buf, "%ws", TAG_OP(OBJECTPROTOTYPEID(i)));


			if (ImGui::Selectable(buf, is_selected))
			{
				eObjectID = i;

			}


		}
		ImGui::EndListBox();
	}

	{
		wstring oo = TAG_OP(OBJECTPROTOTYPEID(eObjectID));
		string ss;
		ImGui::Text(ss.assign(oo.begin(), oo.end()).c_str());
	}
	// enum EParkourTriggerType { PACUR_LEDGE, PACUR_JUMP, PACUR_END };

	Make_VerticalSpacing(1);
	ImGui::InputInt("E Number", &eNumber);
	eNumber = max(min(eNumber, (CTriggerObject::PACUR_END - 1)), CTriggerObject::PACUR_LEDGE);

	Make_VerticalSpacing(1);


	if (ImGui::Button("Create Trigger"))
	{
		CTriggerObject*	pObject; 
		g_pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)&pObject, SCENE_STATIC, TAG_OP(OBJECTPROTOTYPEID(eObjectID)),&eNumber);
		NULL_CHECK_BREAK(pObject);

		CTransform * pTrigTransform= (CTransform*)pObject->Get_Component(TAG_COM(Com_Transform));
		
		//////////////////////////////////////////////////////////////////////////메트릭스 넣기
		_float3 vv;
		m_fPickingedPosition;

		memcpy(&vv, m_fPickingedPosition, sizeof(_float3));
		pTrigTransform->Set_MatrixState(CTransform::STATE_POS, vv);

		pObject->Set_eNumberNObjectID(eNumber, TAG_OP(OBJECTPROTOTYPEID(eObjectID)) );


		m_vecTriggerObject.push_back(pObject);
	}

	Make_VerticalSpacing(1);

	if (m_vecTriggerObject.size() > 0)
	{

		ImGui::InputInt("Trigger Index", &iTriggerIndex);
		iTriggerIndex = max(min(iTriggerIndex, _int(m_vecTriggerObject.size() - 1)), 0);


		Make_VerticalSpacing(1);

		if (ImGui::Button("Delete Trigger"))
		{
			auto iter = m_vecTriggerObject.begin();
			iter = iter+iTriggerIndex;
			m_vecTriggerObject.erase(iter);
			return S_OK;
		}


		Make_VerticalSpacing(1);

		CTransform * pTrigTransform = (CTransform*)m_vecTriggerObject[iTriggerIndex]->Get_Component(TAG_COM(Com_Transform));


		Make_VerticalSpacing(2);
		ImGui::Text("Position");
		static _float DragSpeed = 0.01f;
		ImGui::DragFloat("Drag Speed", &DragSpeed, 0.013f, 0.00001f, 100.f);
		DragSpeed = min(max(DragSpeed, 0.00001f), 100.f);

		float ObjectPosition[3] = { 0,0,0 };
		_float3 vPosition = pTrigTransform->Get_MatrixState_Float3(CTransform::STATE_POS);
		memcpy(ObjectPosition, &vPosition, sizeof(_float) * 3);
		ImGui::DragFloat3("   ", ObjectPosition, DragSpeed, -FLT_MAX, FLT_MAX);

		memcpy(&vPosition, ObjectPosition, sizeof(_float) * 3);

		pTrigTransform->Set_MatrixState(CTransform::STATE_POS, vPosition);


		Make_VerticalSpacing(2);
		ImGui::Text("Rotation");
		ImGui::SameLine(250);	ImGui::Text("Scaling");

		static _float RotSpeed = 60.;
		ImGui::DragFloat("Rot Speed", &RotSpeed, 0.1f, 0.001f, 360);
		RotSpeed = min(max(RotSpeed, 0), 360);


		ImGui::Button("-", ImVec2(20, 18));
		if (ImGui::IsItemHovered())
		{
			_float OldTurnSpeed = pTrigTransform->Get_TurnSpeed();
			pTrigTransform->Set_TurnSpeed(XMConvertToRadians(RotSpeed));
			pTrigTransform->Turn_CW(XMVectorSet(1, 0, 0, 0), fDeltatime);
			pTrigTransform->Set_TurnSpeed(OldTurnSpeed);

		}
		ImGui::SameLine(0, 10);		ImGui::Text("X");			ImGui::SameLine(0, 10);

		ImGui::Button("+", ImVec2(20, 18));
		if (ImGui::IsItemHovered())
		{
			_float OldTurnSpeed = pTrigTransform->Get_TurnSpeed();
			pTrigTransform->Set_TurnSpeed(XMConvertToRadians(RotSpeed));
			pTrigTransform->Turn_CCW(XMVectorSet(1, 0, 0, 0), fDeltatime);
			pTrigTransform->Set_TurnSpeed(OldTurnSpeed);
		}


		ImGui::SameLine(250);
		_float3 OldScaled = pTrigTransform->Get_Scale();
		_float Scaled = OldScaled.x;
		ImGui::DragFloat("X ", &Scaled, 0.001f, 0.001f, FLT_MAX);
		OldScaled.x = Scaled;



		ImGui::Button("- ", ImVec2(20, 18));
		if (ImGui::IsItemHovered())
		{
			_float OldTurnSpeed = pTrigTransform->Get_TurnSpeed();
			pTrigTransform->Set_TurnSpeed(XMConvertToRadians(RotSpeed));
			pTrigTransform->Turn_CW(XMVectorSet(0, 1, 0, 0), fDeltatime);
			pTrigTransform->Set_TurnSpeed(OldTurnSpeed);
		}
		ImGui::SameLine(0, 10);		ImGui::Text("Y");			ImGui::SameLine(0, 10);
		ImGui::Button("+ ", ImVec2(20, 18));

		if (ImGui::IsItemHovered())
		{
			_float OldTurnSpeed = pTrigTransform->Get_TurnSpeed();
			pTrigTransform->Set_TurnSpeed(XMConvertToRadians(RotSpeed));
			pTrigTransform->Turn_CCW(XMVectorSet(0, 1, 0, 0), fDeltatime);
			pTrigTransform->Set_TurnSpeed(OldTurnSpeed);

		}

		ImGui::SameLine(250);
		Scaled = OldScaled.y;
		ImGui::DragFloat("Y ", &Scaled, 0.001f, 0.001f, FLT_MAX);
		OldScaled.y = Scaled;
		ImGui::Button("-  ", ImVec2(20, 18));
		if (ImGui::IsItemHovered())
		{
			_float OldTurnSpeed = pTrigTransform->Get_TurnSpeed();
			pTrigTransform->Set_TurnSpeed(XMConvertToRadians(RotSpeed));
			pTrigTransform->Turn_CW(XMVectorSet(0, 0, 1, 0), fDeltatime);
			pTrigTransform->Set_TurnSpeed(OldTurnSpeed);

		}
		ImGui::SameLine(0, 10);		ImGui::Text("Z");			ImGui::SameLine(0, 10);
		ImGui::Button("+  ", ImVec2(20, 18));
		if (ImGui::IsItemHovered())
		{
			_float OldTurnSpeed = pTrigTransform->Get_TurnSpeed();
			pTrigTransform->Set_TurnSpeed(XMConvertToRadians(RotSpeed));
			pTrigTransform->Turn_CCW(XMVectorSet(0, 0, 1, 0), fDeltatime);
			pTrigTransform->Set_TurnSpeed(OldTurnSpeed);
		}

		ImGui::SameLine(250);
		Scaled = OldScaled.z;
		ImGui::DragFloat("Z ", &Scaled, 0.001f, 0.001f, FLT_MAX);
		OldScaled.z = Scaled;

		OldScaled.x = max(OldScaled.x, 0.001f);
		OldScaled.y = max(OldScaled.y, 0.001f);
		OldScaled.z = max(OldScaled.z, 0.001f);


		pTrigTransform->Scaled_All(OldScaled);


		Make_VerticalSpacing(1);
		ImGui::SameLine(50);
		if (ImGui::Button("ReSet Rot", ImVec2(80, 25)))
		{
			pTrigTransform->Rotation_CW(XMVectorSet(0, 1, 0, 0), 0);
		}

		ImGui::SameLine(250);
		if (ImGui::Button("ReSet Scale", ImVec2(100, 25)))
		{
			pTrigTransform->Scaled_All(_float3(1, 1, 1));
		}

		Make_VerticalSpacing(5);


		_float4x4 ValueMat = m_vecTriggerObject[iTriggerIndex]->Get_ValueMat();

		_float Imguifloat4[4];
		memcpy(Imguifloat4, &(ValueMat._11), sizeof(_float4));
		ImGui::InputFloat4(": row 1", Imguifloat4);
		memcpy(&(ValueMat._11), Imguifloat4, sizeof(_float4));

		memcpy(Imguifloat4, &(ValueMat._21), sizeof(_float4));
		ImGui::InputFloat4(": row 2", Imguifloat4);
		memcpy(&(ValueMat._21), Imguifloat4, sizeof(_float4));

		memcpy(Imguifloat4, &(ValueMat._31), sizeof(_float4));
		ImGui::InputFloat4(": row 3", Imguifloat4);
		memcpy(&(ValueMat._31), Imguifloat4, sizeof(_float4));

		memcpy(Imguifloat4, &(ValueMat._41), sizeof(_float4));
		ImGui::InputFloat4(": row 4", Imguifloat4);
		memcpy(&(ValueMat._41), Imguifloat4, sizeof(_float4));

		m_vecTriggerObject[iTriggerIndex]->Set_ValueMat(&ValueMat);
	}


	FAILED_CHECK(Widget_TriggerData(fDeltatime));

	return S_OK;
}

HRESULT CScene_Edit::Widget_TriggerData(_double fDeltatime)
{
	if (ImGui::Button("New Trigger"))
		ImGui::OpenPopup("New Trigger");
	ImGui::SameLine();
	if (ImGui::Button("Save Trigger"))
		ImGui::OpenPopup("Save Trigger");
	ImGui::SameLine();
	if (ImGui::Button("Laod Trigger"))
		ImGui::OpenPopup("Laod Trigger");
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();

	// Always center this window when appearing
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("New Trigger", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("!!!!!!!!!!!!!!!!Waring!!!!!!!!!!!!!!!!\n\n Delete Batched Trigger Without Save!!!\n\n	Please Check Save One more\n\n\n");
		ImGui::Separator();

		//static int unused_i = 0;
		//ImGui::Combo("Combo", &unused_i, "Delete\0Delete harder\0");

		//static bool dont_ask_me_next_time = false;
		//ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
		//ImGui::Checkbox("Don't ask me next time", &dont_ask_me_next_time);
		//ImGui::PopStyleVar();

		if (ImGui::Button("OK", ImVec2(130, 0))) {


			for (auto& pTrigger : m_vecTriggerObject)
				Safe_Release(pTrigger);
			m_vecTriggerObject.clear();

			ImGui::CloseCurrentPopup();
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(130, 0))) { ImGui::CloseCurrentPopup(); }
		ImGui::EndPopup();
	}


	//////////////////////////////////////////////////////////////////////////


	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("Save Trigger", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{

		if (m_FilePathList.size() == 0)
		{
			m_FilePathList.clear();
			_tfinddata64_t fd;
			__int64 handle = _tfindfirst64(TEXT("../bin/Resources/Data/Trigger/*.*"), &fd);
			if (handle == -1 || handle == 0)
				return E_FAIL;

			_int iResult = 0;

			//char szCurPath[128] = "../bin/Resources/Data/Map/";
			//char szFullPath[128] = "";
			char szFilename[MAX_PATH];

			while (iResult != -1)
			{
				if (!lstrcmp(fd.name, L".") || !lstrcmp(fd.name, L".."))
				{
					iResult = _tfindnext64(handle, &fd);
					continue;
				}


				WideCharToMultiByte(CP_UTF8, 0, fd.name, -1, szFilename, sizeof(szFilename), NULL, NULL);
				//strcpy_s(szFullPath, szCurPath);
				//strcat_s(szFullPath, szFilename);
				m_FilePathList.push_back({ szFilename });


				iResult = _tfindnext64(handle, &fd);
			}


			_findclose(handle);

		}

		ImGui::Text("Save Trigger!\n\nExist Trigger DataFiles");

		static ImGuiTextFilter filter;

		char	szCheckforSameFileName[256] = "";

		if (ImGui::BeginListBox(" "))
		{
			auto iter = m_FilePathList.begin();


			for (; iter != m_FilePathList.end(); iter++)
			{
				const bool is_selected = false;

				if (filter.PassFilter(iter->c_str()))
				{
					if (ImGui::Selectable(iter->c_str(), is_selected))
					{
						strcpy_s(filter.InputBuf, iter->c_str());
					}

					if (!strcmp(iter->c_str(), filter.InputBuf))
						strcpy_s(szCheckforSameFileName, filter.InputBuf);
				}
			}
			ImGui::EndListBox();

		}

		filter.Draw("Input FileName");


		ImGui::Separator();
		if (ImGui::Button("Save", ImVec2(120, 0)))
		{

			if (strcmp(filter.InputBuf, ""))
			{

				if (!strcmp(szCheckforSameFileName, filter.InputBuf))
				{
					ImGui::OpenPopup("One More Check");
				}
				else
				{
					//실제 저장

					Sava_Data(filter.InputBuf, Data_Trigger);

					ImGui::CloseCurrentPopup();
					m_FilePathList.clear();


				}
			}



		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			ImGui::CloseCurrentPopup();
			m_FilePathList.clear();
		}


		//서브 팝업
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		if (ImGui::BeginPopupModal("One More Check", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{

			ImGui::Text("Trigger data Already Exist\nDo you want to Override on it?");

			if (ImGui::Button("Ok", ImVec2(130, 0)))
			{


				//실제 저장
				Sava_Data(filter.InputBuf, Data_Trigger);

				ImGui::CloseCurrentPopup();
				m_FilePathList.clear();
			}

			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(130, 0)))
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}


		ImGui::EndPopup();
	}


	//////////////////////////////////////////////////////////////////////////

	// Always center this window when appearing
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("Laod Trigger", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Laod Trigger!\n\n");
		ImGui::Separator();


		if (m_FilePathList.size() == 0)
		{
			m_FilePathList.clear();
			_tfinddata64_t fd;
			__int64 handle = _tfindfirst64(TEXT("../bin/Resources/Data/Trigger/*.*"), &fd);
			if (handle == -1 || handle == 0)
				return E_FAIL;

			_int iResult = 0;

			//char szCurPath[128] = "../bin/Resources/Data/Map/";
			//char szFullPath[128] = "";
			char szFilename[MAX_PATH];

			while (iResult != -1)
			{
				if (!lstrcmp(fd.name, L".") || !lstrcmp(fd.name, L".."))
				{
					iResult = _tfindnext64(handle, &fd);
					continue;
				}


				WideCharToMultiByte(CP_UTF8, 0, fd.name, -1, szFilename, sizeof(szFilename), NULL, NULL);
				//strcpy_s(szFullPath, szCurPath);
				//strcat_s(szFullPath, szFilename);
				m_FilePathList.push_back({ szFilename });


				iResult = _tfindnext64(handle, &fd);
			}


			_findclose(handle);

		}



		static ImGuiTextFilter filter;

		char	szCheckforSameFileName[256] = "";

		if (ImGui::BeginListBox(" "))
		{
			auto iter = m_FilePathList.begin();


			for (; iter != m_FilePathList.end(); iter++)
			{
				const bool is_selected = false;

				if (filter.PassFilter(iter->c_str()))
				{
					if (ImGui::Selectable(iter->c_str(), is_selected))
					{
						strcpy_s(filter.InputBuf, iter->c_str());
					}

					if (!strcmp(iter->c_str(), filter.InputBuf))
						strcpy_s(szCheckforSameFileName, filter.InputBuf);
				}
			}
			ImGui::EndListBox();

		}

		filter.Draw("Input FileName");




		if (ImGui::Button("OK", ImVec2(120, 0)))
		{

			if (strcmp(filter.InputBuf, ""))
			{
				if (!strcmp(szCheckforSameFileName, filter.InputBuf))
				{
					for (auto& pTrigger : m_vecTriggerObject)
						Safe_Release(pTrigger);
					m_vecTriggerObject.clear();


					Load_Data(filter.InputBuf, Data_Trigger);
					m_FilePathList.clear();
					ImGui::CloseCurrentPopup();
				}
			}
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0))) {
			m_FilePathList.clear();
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
	return S_OK;


	return S_OK;
}



#pragma endregion MapTab

#pragma region UITab


HRESULT CScene_Edit::Ready_Layer_RendererEditUI(const _tchar * pLayerTag)
{

	//FAILED_CHECK(m_pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&m_pRendererEditUI), SCENE_LOBY, L"ProtoType_EditRendererUI"));

	//NULL_CHECK_RETURN(m_pRendererEditUI, E_FAIL);

	//m_pRendererEditUI->Set_VecEditUI(&m_vecBatchedUI);


	//ZeroMemory(&m_fUIDesc, sizeof(UIDESC));
	//m_fUIDesc.fCX = m_fUIDesc.fCY = 10;


	//m_fUIRect.left = m_fUIDesc.fX - m_fUIDesc.fCX * 0.5f;
	//m_fUIRect.top = m_fUIDesc.fY - m_fUIDesc.fCY * 0.5f;
	//m_fUIRect.right = m_fUIDesc.fX + m_fUIDesc.fCX * 0.5f;
	//m_fUIRect.bottom = m_fUIDesc.fY + m_fUIDesc.fCY * 0.5f;

	return S_OK;
}

HRESULT CScene_Edit::Update_UITab(_double fDeltatime)
{

	//FAILED_CHECK(Widget_LoadUISRVTexture(fDeltatime));

	//FAILED_CHECK(Widget_CreateNBatchUI(fDeltatime));
	//FAILED_CHECK(Widget_DeleteBatchedUI(fDeltatime));

	return S_OK;
}


HRESULT CScene_Edit::Widget_LoadUISRVTexture(_double fDeltatime)
{
	if (ImGui::TreeNode("Texture"))
	{

		static char str0[128] = "";
		ImGui::InputText("Input Path", str0, IM_ARRAYSIZE(str0));

		if (ImGui::Button("Load Texture", ImVec2(-FLT_MIN, 30)))
		{


			char TempPath[MAX_PATH] = "";
			//char FileFullPath[MAX_PATH] = "../bin/Resources/Textures/UI/UI/";
			ZeroMemory(m_szTexturPath, sizeof(char)*MAX_PATH);
			strcpy_s(m_szTexturPath, "../bin/Resources/Textures/UI/UI/");
			strcpy_s(TempPath, "../bin/Resources/Textures/UI/UI/");


			m_FilePathList.clear();
			_tfinddata64_t fd;

			strcat_s(m_szTexturPath, str0);
			strcat_s(TempPath, str0);
			if (!strcmp(str0, ""))
			{
				strcat_s(TempPath, "*.*");
			}
			else
			{
				strcat_s(TempPath, "/*.*");
				strcat_s(m_szTexturPath, "/");
			}

			string Temp = TempPath;
			wstring wtemp;
			wtemp.assign(Temp.begin(), Temp.end());

			__int64 handle = _tfindfirst64(wtemp.c_str(), &fd);
			if (handle == -1 || handle == 0)
			{
				__debugbreak();
				return E_FAIL;
			}

			_int iResult = 0;

			//char szCurPath[128] = "../bin/Resources/Data/Map/";
			//char szFullPath[128] = "";
			char szFilename[MAX_PATH];

			while (iResult != -1)
			{
				if (!lstrcmp(fd.name, L".") || !lstrcmp(fd.name, L".."))
				{
					iResult = _tfindnext64(handle, &fd);
					continue;
				}


				WideCharToMultiByte(CP_UTF8, 0, fd.name, -1, szFilename, sizeof(szFilename), NULL, NULL);
				//strcpy_s(szFullPath, szCurPath);
				//strcat_s(szFullPath, szFilename);
				m_FilePathList.push_back({ szFilename });


				iResult = _tfindnext64(handle, &fd);
			}


			_findclose(handle);



		}

		static ImGuiTextFilter filter;
		char	szCheckforSameFileName[256] = "";

		if (ImGui::BeginListBox(" "))
		{
			auto iter = m_FilePathList.begin();


			for (; iter != m_FilePathList.end(); iter++)
			{
				const bool is_selected = false;

				if (filter.PassFilter(iter->c_str()))
				{
					if (ImGui::Selectable(iter->c_str(), is_selected))
					{
						strcpy_s(filter.InputBuf, iter->c_str());
					}

					if (!strcmp(iter->c_str(), filter.InputBuf))
						strcpy_s(szCheckforSameFileName, filter.InputBuf);
				}
			}
			ImGui::EndListBox();
			if (m_TargetSRV != nullptr)
			{

				ImTextureID my_tex_id = m_TargetSRV;
				float my_tex_w = 110;
				float my_tex_h = 120;
				{
					ImGui::SameLine();
					ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
					ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
					ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
					ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white
					ImGui::Image(my_tex_id, ImVec2(my_tex_w, my_tex_h), uv_min, uv_max, tint_col, border_col);

				}



			}
		}

		filter.Draw("Input FileName");




		if (ImGui::Button("OK", ImVec2(120, 0)))
		{

			if (strcmp(filter.InputBuf, ""))
			{
				if (!strcmp(szCheckforSameFileName, filter.InputBuf))
				{

					char TempPath[MAX_PATH] = "";

					strcpy_s(TempPath, m_szTexturPath);
					strcat_s(TempPath, filter.InputBuf);


					Load_Data(TempPath, Data_UI);
					//m_FilePathList.clear();
					ImGui::CloseCurrentPopup();
				}
			}






		}
		ImGui::TreePop();
	}

	return S_OK;
}

HRESULT CScene_Edit::Widget_CreateNBatchUI(_double fDeltatime)
{
	Make_VerticalSpacing(5);

	ImGui::Checkbox("UI Drawing by Rect?" ,&m_bIsRect);

	Make_VerticalSpacing(3);

	if (m_bIsRect)
	{
		float tempArr[2] = { m_fUIRect.left,m_fUIRect.top};

		ImGui::DragFloat2("L / T ", tempArr);

		m_fUIRect.left = tempArr[0];
		m_fUIRect.top = tempArr[1];

		tempArr[0] = m_fUIRect.right;
		tempArr[1] = m_fUIRect.bottom;

		ImGui::DragFloat2("R / B ", tempArr);

		m_fUIRect.right = tempArr[0];
		m_fUIRect.bottom = tempArr[1];

		m_fUIDesc.fCX = m_fUIRect.right - m_fUIRect.left;
		m_fUIDesc.fCY = m_fUIRect.bottom - m_fUIRect.top;
		m_fUIDesc.fX = m_fUIRect.left + m_fUIDesc.fCX * 0.5f;
		m_fUIDesc.fY = m_fUIRect.top + m_fUIDesc.fCY * 0.5f;


		

	}
	else
	{
		float tempArr[2] = {m_fUIDesc.fX,m_fUIDesc.fY };
		ImGui::DragFloat2("X / Y ", tempArr);

		m_fUIDesc.fX = tempArr[0];
		m_fUIDesc.fY = tempArr[1];

		tempArr[0] = m_fUIDesc.fCX;
		tempArr[1] = m_fUIDesc.fCY;

		ImGui::DragFloat2("CX / CY ", tempArr);


		m_fUIDesc.fCX = tempArr[0];
		m_fUIDesc.fCY = tempArr[1];


		m_fUIRect.left = m_fUIDesc.fX - m_fUIDesc.fCX * 0.5f;
		m_fUIRect.top = m_fUIDesc.fY - m_fUIDesc.fCY * 0.5f;
		m_fUIRect.right = m_fUIDesc.fX + m_fUIDesc.fCX * 0.5f;
		m_fUIRect.bottom = m_fUIDesc.fY + m_fUIDesc.fCY * 0.5f;

	}

	ImGui::DragFloat("Angle", &m_fUIAngle);
	if (!m_bModifyBatchedUI)
		ImGui::DragFloat("Depth", &m_fDepth);

	if (m_fUIAngle > 360) m_fUIAngle = _float(_int(m_fUIAngle) % 360);

	if (m_fUIAngle < 0)
	{
		m_fUIAngle *= -1;
		m_fUIAngle = _float(_int(m_fUIAngle) % 360);
		m_fUIAngle *= -1;
		m_fUIAngle += 360;
	}



	if (m_bModifyBatchedUI)
	{


		m_vecBatchedUI[m_iModifyUIIndex].UIDesc = m_fUIDesc;
		m_vecBatchedUI[m_iModifyUIIndex].bIsRect = m_bIsRect;
		m_vecBatchedUI[m_iModifyUIIndex].fAngle = m_fUIAngle;
		m_vecBatchedUI[m_iModifyUIIndex].UIRect = m_fUIRect;



		if (m_TargetSRV != nullptr)
		{

			Safe_Release(m_vecBatchedUI[m_iModifyUIIndex].SRV);

			m_vecBatchedUI[m_iModifyUIIndex].SRV = m_TargetSRV;

			Safe_AddRef(m_vecBatchedUI[m_iModifyUIIndex].SRV);

		}


	}



	if(ImGui::Button("Create UI Button",ImVec2(-FLT_MIN, 30)))
	{
		if (m_TargetSRV != nullptr)
		{
			EDITUI Temp;


			Temp.UIDesc = m_fUIDesc;
			Temp.bIsRect = m_bIsRect;
			Temp.fAngle = m_fUIAngle;
			Temp.UIRect = m_fUIRect;
			Temp.SRV    = m_TargetSRV;
			Temp.fDepth = m_fDepth;
			Safe_AddRef(Temp.SRV);

			m_vecBatchedUI.push_back(Temp);
		

			sort(m_vecBatchedUI.begin(), m_vecBatchedUI.end(), [&](EDITUI& pSour, EDITUI& pDest)
			{
				return pSour.fDepth < pDest.fDepth;
			});

			m_fDepth -= 0.001f;
		}
	}


	return S_OK;
}

HRESULT CScene_Edit::Widget_DeleteBatchedUI(_double fDeltatime)
{

	ImGui::Checkbox("Modify Batched UI", &m_bModifyBatchedUI);


	if (m_bModifyBatchedUI)
	{




	}




	return S_OK;
}

HRESULT CScene_Edit::LoadTextureByAssimp(const _tchar * FileFullPath)
{
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	_tchar		szTextureFilePath[MAX_PATH] = TEXT("");
	_tchar		szExt[MAX_PATH] = TEXT("");

	lstrcpy(szTextureFilePath, FileFullPath);


		_wsplitpath_s(szTextureFilePath, nullptr, 0, nullptr, 0, nullptr, 0, szExt, MAX_PATH);

		if (!lstrcmp(szExt, TEXT(".dds")))
		{
			FAILED_CHECK(CreateDDSTextureFromFile(m_pDevice, szTextureFilePath, nullptr, &m_TargetSRV));
		}

		else if (!lstrcmp(szExt, TEXT(".tga")))
		{
			__debugbreak();
			MSGBOX("Not Available TGA File");
			return E_FAIL;
		}

		else
		{
			FAILED_CHECK(CreateWICTextureFromFile(m_pDevice, szTextureFilePath, nullptr, &m_TargetSRV));
		}

	
		if (m_TargetSRV == nullptr)
		{
			__debugbreak();
			return E_FAIL;
		}


	return S_OK;
}


#pragma endregion UITab

#pragma region ParticleTab


#pragma endregion ParticleTab


#pragma region CamTab

HRESULT CScene_Edit::Update_ParticleTab(_double fDeltatime)
{
	static int open_action = -1;

	if (open_action != -1)	ImGui::SetNextItemOpen(open_action == 0);

	if (ImGui::TreeNode("Texture Instance Particle"))
	{

		FAILED_CHECK(Widget_SettingParticleDesc(fDeltatime));

		open_action = 0;

		FAILED_CHECK(Widget_SaveLoadTextureParticle(fDeltatime));

		
		ImGui::TreePop();
	}
	else
	{
		open_action = -1;
	}
	if (open_action != -1)	ImGui::SetNextItemOpen(open_action == 1);
	if (ImGui::TreeNode("Mesh Instance Particle"))
	{
		open_action = 1;
		FAILED_CHECK(Widget_ModelParticleDesc(fDeltatime));

		FAILED_CHECK(Widget_SaveLoadMeshParticle(fDeltatime));

		ImGui::TreePop();
	}
	else
	{
		open_action = -1;
	}
	

	

	return S_OK;
}

#define DefaultParticleNameTag "dds"

HRESULT CScene_Edit::Widget_SettingParticleDesc(_double fDeltatime)
{

	_float TempFloatArr[4];
	_int TempIntArr[4];
	ZeroMemory(TempIntArr, sizeof(_int) * 4);
	ZeroMemory(TempFloatArr, sizeof(_float) * 4);

	Make_VerticalSpacing(1);

	{


		if (ImGui::Button("-", ImVec2(20, 18)))
		{
			m_tParticleDesc.eParticleTypeID = (eInstanceEffectID)(m_tParticleDesc.eParticleTypeID - 1);
			if (m_tParticleDesc.eParticleTypeID < InstanceEffect_Ball) m_tParticleDesc.eParticleTypeID = InstanceEffect_Ball;
		}ImGui::SameLine(0, 10);
		if (ImGui::Button("+", ImVec2(20, 18)))
		{
			m_tParticleDesc.eParticleTypeID = (eInstanceEffectID)(m_tParticleDesc.eParticleTypeID + 1);
			if (m_tParticleDesc.eParticleTypeID >= InstanceEffect_End) m_tParticleDesc.eParticleTypeID = (eInstanceEffectID)(InstanceEffect_End - 1);
		}
		ImGui::SameLine(0, 10);
		ImGui::Text(TAG_TEXINSTEFFECT(m_tParticleDesc.eParticleTypeID));

		//////////////////////////////////////////////////////////////////////////
		if (ImGui::Button("- ", ImVec2(20, 18)))
		{
			m_tParticleDesc.eInstanceCount = (COMPONENTPROTOTYPEID)(m_tParticleDesc.eInstanceCount - 1);
			if (m_tParticleDesc.eInstanceCount < Prototype_VIBuffer_Point_Instance_1) m_tParticleDesc.eInstanceCount = Prototype_VIBuffer_Point_Instance_1;
		}ImGui::SameLine(0, 10);
		if (ImGui::Button("+ ", ImVec2(20, 18)))
		{
			m_tParticleDesc.eInstanceCount = (COMPONENTPROTOTYPEID)(m_tParticleDesc.eInstanceCount + 1);
			if (m_tParticleDesc.eInstanceCount > Prototype_VIBuffer_Point_Instance_512) m_tParticleDesc.eInstanceCount = (Prototype_VIBuffer_Point_Instance_512);
		}
		ImGui::SameLine(0, 10);
		{	wstring tt = TAG_CP(m_tParticleDesc.eInstanceCount);	ImGui::Text(string(tt.begin(), tt.end()).c_str()); }

		//////////////////////////////////////////////////////////////////////////
		if (ImGui::Button("-  ", ImVec2(20, 18)))
		{
			m_tParticleDesc.ePassID = (eInstancePassID)(m_tParticleDesc.ePassID - 1);
			if (m_tParticleDesc.ePassID < InstancePass_OriginColor) m_tParticleDesc.ePassID = InstancePass_OriginColor;
		}ImGui::SameLine(0, 10);
		if (ImGui::Button("+  ", ImVec2(20, 18))) 
		{
			m_tParticleDesc.ePassID = (eInstancePassID)(m_tParticleDesc.ePassID + 1);
			if (m_tParticleDesc.ePassID >= InstancePass_End) m_tParticleDesc.ePassID = (eInstancePassID)(InstancePass_End - 1);
		}
		ImGui::SameLine(0, 10);		ImGui::Text(TAG_INSTPASS(m_tParticleDesc.ePassID));
	}


	Make_VerticalSpacing(1);

	static bool	bIsFollowingTransform = false;
	{

		ImGui::Checkbox("FollowingTransform", &bIsFollowingTransform);

		if (bIsFollowingTransform)
		{
			m_tParticleDesc.FollowingTarget = (CTransform*)(m_vecBatchedObject[0].pObject->Get_Component(TAG_COM(Com_Transform)));
		}
		else
		{
			m_tParticleDesc.FollowingTarget = nullptr;
		}

	}

	if (bIsFollowingTransform)
	{
		if (ImGui::Button("-             ", ImVec2(20, 18)))
		{
			m_tParticleDesc.iFollowingDir = (eFollowingDirID)(m_tParticleDesc.iFollowingDir - 1);
			if (m_tParticleDesc.iFollowingDir < FollowingDir_Right) m_tParticleDesc.iFollowingDir = FollowingDir_Right;
		}ImGui::SameLine(0, 10);
		if (ImGui::Button("+             ", ImVec2(20, 18)))
		{
			m_tParticleDesc.iFollowingDir = (eFollowingDirID)(m_tParticleDesc.iFollowingDir + 1);
			if (m_tParticleDesc.iFollowingDir >= FollowingDir_End) m_tParticleDesc.iFollowingDir = (eFollowingDirID)(FollowingDir_End - 1);
		}

		ImGui::SameLine(0, 10);		ImGui::Text(Tag_InstancePass(m_tParticleDesc.iFollowingDir));

	}
	else
	{

		TempFloatArr[0] = m_tParticleDesc.vFixedPosition.x;
		TempFloatArr[1] = m_tParticleDesc.vFixedPosition.y;
		TempFloatArr[2] = m_tParticleDesc.vFixedPosition.z;
		ImGui::InputFloat3("SwpanPosition", TempFloatArr);
		m_tParticleDesc.vFixedPosition.x = TempFloatArr[0];
		m_tParticleDesc.vFixedPosition.y = TempFloatArr[1];
		m_tParticleDesc.vFixedPosition.z = TempFloatArr[2];


		ZeroMemory(TempFloatArr, sizeof(_float) * 4);
		TempFloatArr[0] = m_tParticleDesc.vPowerDirection.x;
		TempFloatArr[1] = m_tParticleDesc.vPowerDirection.y;
		TempFloatArr[2] = m_tParticleDesc.vPowerDirection.z;
		ImGui::InputFloat3("Force Direct", TempFloatArr);
		m_tParticleDesc.vPowerDirection.x = TempFloatArr[0];
		m_tParticleDesc.vPowerDirection.y = TempFloatArr[1];
		m_tParticleDesc.vPowerDirection.z = TempFloatArr[2];
	}



	Make_VerticalSpacing(1);

	if (m_tParticleDesc.ePassID >= InstancePass_MaskingNoising && m_tParticleDesc.ePassID <= InstancePass_MaskingNoising_Appear_Bright)
	{
		m_tParticleDesc.vNoisePushingDir;
		
		_float2 tt = m_tParticleDesc.vNoisePushingDir.Get_Nomalize();
		float fArr[2];
		memcpy(fArr, &tt, sizeof(float) * 2);
		ImGui::DragFloat2("Noise Pusing Dir", fArr, 0.0001f, -1, 1);
		memcpy(&tt, fArr, sizeof(float) * 2);
		m_tParticleDesc.vNoisePushingDir = tt.Get_Nomalize();

			
		int tempint = m_tParticleDesc.iNoiseTextureIndex;


		ImGui::InputInt("MoiseTex Index", &tempint);
		m_tParticleDesc.iNoiseTextureIndex = tempint;
		tempint = m_tParticleDesc.iMaskingTextureIndex;
		ImGui::InputInt("MaskingTex Index", &tempint);
		m_tParticleDesc.iMaskingTextureIndex = tempint;

	}
	else if (m_tParticleDesc.ePassID >= InstancePass_AllDistortion && m_tParticleDesc.ePassID <= InstancePass_Distortion_ColorMix_Bright)
	{

		m_tParticleDesc.vNoisePushingDir;

		_float2 tt = m_tParticleDesc.vNoisePushingDir.Get_Nomalize();
		float fArr[2];
		memcpy(fArr, &tt, sizeof(float) * 2);
		ImGui::DragFloat2("Distortion Pusing Dir", fArr, 0.0001f, -1, 1);
		memcpy(&tt, fArr, sizeof(float) * 2);
		m_tParticleDesc.vNoisePushingDir = tt.Get_Nomalize();


		ImGui::DragFloat("Push Power", &m_tParticleDesc.fDistortionNoisingPushPower, 0.001f, -100, 100);



		int tempint = m_tParticleDesc.iNoiseTextureIndex;
		ImGui::InputInt("MoiseTex Index", &tempint);
		m_tParticleDesc.iNoiseTextureIndex = tempint;
		tempint = m_tParticleDesc.iMaskingTextureIndex;
		ImGui::InputInt("MaskingTex Index", &tempint);
		m_tParticleDesc.iMaskingTextureIndex = tempint;




	}

	Make_VerticalSpacing(1);

		static ImGuiTextFilter filter = "Prototype_Texture_TestEffect";
	{

		wstring ta = m_tParticleDesc.szTextureProtoTypeTag;
		string t2;
		strcpy_s(filter.InputBuf, t2.assign(ta.begin(), ta.end()).c_str());
		filter.Draw("Input TextureProtoTypeTag");
	}

	string Temp = string(filter.InputBuf);
	wstring wtemp;
	wtemp.assign(Temp.begin(), Temp.end());
	lstrcpy(m_tParticleDesc.szTextureProtoTypeTag, wtemp.c_str());

	static ImGuiTextFilter filter2 = DefaultParticleNameTag;
	wstring ta = m_tParticleDesc.szTextureLayerTag;
	string t2;

	strcpy_s(filter2.InputBuf, t2.assign(ta.begin(), ta.end()).c_str());
	filter2.Draw("Input TextureLayer");


	string Temp2 = string(filter2.InputBuf);
	wstring wtemp2;
	wtemp2.assign(Temp2.begin(), Temp2.end());


	lstrcpy(m_tParticleDesc.szTextureLayerTag, wtemp2.c_str());


	int tt = m_tParticleDesc.iTextureLayerIndex;
	ImGui::InputInt("TextureLayer Index", &tt);
	m_tParticleDesc.iTextureLayerIndex = tt;


	ImGui::InputInt("FigureCount_In_Tex", &m_tParticleDesc.iFigureCount_In_Texture);


	TempIntArr[1] = (_int)m_tParticleDesc.TextureChageFrequency;
	ImGui::InputInt("TextureChageFrequency", &TempIntArr[1]);
	m_tParticleDesc.TextureChageFrequency = TempIntArr[1];

	ZeroMemory(TempFloatArr, sizeof(_float) * 4);
	TempFloatArr[0] = m_tParticleDesc.vTextureXYNum.x;
	TempFloatArr[1] = m_tParticleDesc.vTextureXYNum.y;
	ImGui::InputFloat2("TextureXYSizeNum", TempFloatArr);
	m_tParticleDesc.vTextureXYNum.x = TempFloatArr[0];
	m_tParticleDesc.vTextureXYNum.y = TempFloatArr[1];




	Make_VerticalSpacing(1);




	TempFloatArr[2] = m_tParticleDesc.TotalParticleTime;
	ImGui::DragFloat("Total PaticleLifeTime", &TempFloatArr[2],0.0035f, (_float)g_fDeltaTime);
	m_tParticleDesc.TotalParticleTime = TempFloatArr[2];


	TempFloatArr[3] = m_tParticleDesc.EachParticleLifeTime;
	ImGui::DragFloat("Each PaticleLifeTime", &TempFloatArr[3], 0.0035f, (_float)g_fDeltaTime);
	m_tParticleDesc.EachParticleLifeTime = TempFloatArr[3];


	Make_VerticalSpacing(1);

	TempIntArr[3] = (_int)m_tParticleDesc.SizeChageFrequency;
	ImGui::InputInt("SizeChageFrequency", &TempIntArr[3]);
	m_tParticleDesc.SizeChageFrequency = TempIntArr[3];


	ZeroMemory(TempFloatArr, sizeof(_float) * 4);
	TempFloatArr[0] = m_tParticleDesc.ParticleSize.x;
	TempFloatArr[1] = m_tParticleDesc.ParticleSize.y;
	TempFloatArr[2] = m_tParticleDesc.ParticleSize.z;
	ImGui::InputFloat3("ParticleSize1", TempFloatArr);
	m_tParticleDesc.ParticleSize.x = TempFloatArr[0];
	m_tParticleDesc.ParticleSize.y = TempFloatArr[1];
	m_tParticleDesc.ParticleSize.z = TempFloatArr[2];

	ZeroMemory(TempFloatArr, sizeof(_float) * 4);
	TempFloatArr[0] = m_tParticleDesc.ParticleSize2.x;
	TempFloatArr[1] = m_tParticleDesc.ParticleSize2.y;
	TempFloatArr[2] = m_tParticleDesc.ParticleSize2.z;
	ImGui::InputFloat3("ParticleSize2", TempFloatArr);
	m_tParticleDesc.ParticleSize2.x = TempFloatArr[0];
	m_tParticleDesc.ParticleSize2.y = TempFloatArr[1];
	m_tParticleDesc.ParticleSize2.z = TempFloatArr[2];

	Make_VerticalSpacing(1);

	ZeroMemory(TempIntArr, sizeof(_int) * 4);
	TempIntArr[0] = _int(m_tParticleDesc.ColorChageFrequency);
	ImGui::InputInt("ColorChageFrequency", &TempIntArr[0]);
	m_tParticleDesc.ColorChageFrequency = TempIntArr[0];

	ZeroMemory(TempFloatArr, sizeof(_float) * 4);
	TempFloatArr[0] = m_tParticleDesc.TargetColor.x;
	TempFloatArr[1] = m_tParticleDesc.TargetColor.y;
	TempFloatArr[2] = m_tParticleDesc.TargetColor.z;
	TempFloatArr[3] = m_tParticleDesc.TargetColor.w;
	ImGui::InputFloat4("TargetColor1", TempFloatArr);
	m_tParticleDesc.TargetColor.x = TempFloatArr[0];
	m_tParticleDesc.TargetColor.y = TempFloatArr[1];
	m_tParticleDesc.TargetColor.z = TempFloatArr[2];
	m_tParticleDesc.TargetColor.w = TempFloatArr[3];

	ZeroMemory(TempFloatArr, sizeof(_float) * 4);
	TempFloatArr[0] = m_tParticleDesc.TargetColor2.x;
	TempFloatArr[1] = m_tParticleDesc.TargetColor2.y;
	TempFloatArr[2] = m_tParticleDesc.TargetColor2.z;
	TempFloatArr[3] = m_tParticleDesc.TargetColor2.w;
	ImGui::InputFloat4("TargetColor2", TempFloatArr);
	m_tParticleDesc.TargetColor2.x = TempFloatArr[0];
	m_tParticleDesc.TargetColor2.y = TempFloatArr[1];
	m_tParticleDesc.TargetColor2.z = TempFloatArr[2];
	m_tParticleDesc.TargetColor2.w = TempFloatArr[3];

	Make_VerticalSpacing(1);

	ZeroMemory(TempFloatArr, sizeof(_float) * 4);
	TempFloatArr[3] = m_tParticleDesc.Particle_Power;
	ImGui::DragFloat("Particle_Power", &TempFloatArr[3], 0.0034f);
	m_tParticleDesc.Particle_Power = TempFloatArr[3];


	TempFloatArr[0] = m_tParticleDesc.PowerRandomRange.x;
	TempFloatArr[1] = m_tParticleDesc.PowerRandomRange.y;
	ImGui::InputFloat2("PowerRandomRange", TempFloatArr);
	m_tParticleDesc.PowerRandomRange.x = TempFloatArr[0];
	m_tParticleDesc.PowerRandomRange.y = TempFloatArr[1];

	TempFloatArr[0] = m_tParticleDesc.SubPowerRandomRange_RUL.x;
	TempFloatArr[1] = m_tParticleDesc.SubPowerRandomRange_RUL.y;
	TempFloatArr[2] = m_tParticleDesc.SubPowerRandomRange_RUL.z;
	ImGui::DragFloat3("Power Weight by RUL", TempFloatArr, 0.03f); 
	m_tParticleDesc.SubPowerRandomRange_RUL.x = TempFloatArr[0];
	m_tParticleDesc.SubPowerRandomRange_RUL.y = TempFloatArr[1];
	m_tParticleDesc.SubPowerRandomRange_RUL.z = TempFloatArr[2];


	ZeroMemory(TempFloatArr, sizeof(_float) * 4);
	TempFloatArr[0] = m_tParticleDesc.vPowerDirection.x;
	TempFloatArr[1] = m_tParticleDesc.vPowerDirection.y;
	TempFloatArr[2] = m_tParticleDesc.vPowerDirection.z;
	ImGui::InputFloat3("Force Direct", TempFloatArr);
	m_tParticleDesc.vPowerDirection.x = TempFloatArr[0];
	m_tParticleDesc.vPowerDirection.y = TempFloatArr[1];
	m_tParticleDesc.vPowerDirection.z = TempFloatArr[2];

	Make_VerticalSpacing(1);


	TempFloatArr[3] = m_tParticleDesc.fMaxBoundaryRadius;
	ImGui::DragFloat("MaxBoundaryRadius", &TempFloatArr[3], 0.0034f);
	m_tParticleDesc.fMaxBoundaryRadius = TempFloatArr[3];



	ZeroMemory(TempFloatArr, sizeof(_float) * 4);
	TempFloatArr[0] = m_tParticleDesc.ParticleStartRandomPosMin.x;
	TempFloatArr[1] = m_tParticleDesc.ParticleStartRandomPosMin.y;
	TempFloatArr[2] = m_tParticleDesc.ParticleStartRandomPosMin.z;
	ImGui::InputFloat3("StartRandomPos  Min", TempFloatArr);
	m_tParticleDesc.ParticleStartRandomPosMin.x = TempFloatArr[0];
	m_tParticleDesc.ParticleStartRandomPosMin.y = TempFloatArr[1];
	m_tParticleDesc.ParticleStartRandomPosMin.z = TempFloatArr[2];


	ZeroMemory(TempFloatArr, sizeof(_float) * 4);
	TempFloatArr[0] = m_tParticleDesc.ParticleStartRandomPosMax.x;
	TempFloatArr[1] = m_tParticleDesc.ParticleStartRandomPosMax.y;
	TempFloatArr[2] = m_tParticleDesc.ParticleStartRandomPosMax.z;
	ImGui::InputFloat3("StartRandomPos  Max", TempFloatArr);
	m_tParticleDesc.ParticleStartRandomPosMax.x = TempFloatArr[0];
	m_tParticleDesc.ParticleStartRandomPosMax.y = TempFloatArr[1];
	m_tParticleDesc.ParticleStartRandomPosMax.z = TempFloatArr[2];

	Make_VerticalSpacing(1);

	ImGui::Checkbox("Billboard", &m_tParticleDesc.bBillboard); ImGui::SameLine();
	ImGui::Checkbox("AlphaBlendON", &m_tParticleDesc.AlphaBlendON); ImGui::SameLine();
	ImGui::Checkbox("Emissive", &m_tParticleDesc.bEmissive);

	if (m_tParticleDesc.bEmissive)
	{
		Make_VerticalSpacing(1);
		memcpy(TempFloatArr, &(m_tParticleDesc.vEmissive_SBB), sizeof(_float) * 3);
		ImGui::Text("Shade /Blur /Bright");

		ImGui::DragFloat3("  ", TempFloatArr, 0.0073f, 0, 1.f);

		memcpy(&(m_tParticleDesc.vEmissive_SBB), TempFloatArr, sizeof(_float) * 3);


	}
	Make_VerticalSpacing(1);

	TempFloatArr[3] = m_tParticleDesc.m_fAlphaTestValue;
	ImGui::DragFloat("AlphaTestValue", &TempFloatArr[3]);
	m_tParticleDesc.m_fAlphaTestValue = TempFloatArr[3];


	ImGui::DragFloat("DepthTargetRender ", &m_tParticleDesc.TempBuffer_0.x);



	Make_VerticalSpacing(1);


	if (ImGui::Button("Play Partlcle", ImVec2(-FLT_MIN, 30)))
	{
		GetSingle(CUtilityMgr)->Create_TextureInstance(SCENE_EDIT, m_tParticleDesc);
	}









	return S_OK;
}

HRESULT CScene_Edit::Widget_ModelParticleDesc(_double fDeltatime)
{

	_float TempFloatArr[4];
	_int TempIntArr[4];
	ZeroMemory(TempIntArr, sizeof(_int) * 4);
	ZeroMemory(TempFloatArr, sizeof(_float) * 4);

	Make_VerticalSpacing(1);

	{

		if (ImGui::Button("-", ImVec2(20, 18)))
		{
			m_tMeshDesc.eParticleTypeID = (eInstanceEffectID)(m_tMeshDesc.eParticleTypeID - 1);
			if (m_tMeshDesc.eParticleTypeID < InstanceEffect_Ball) m_tMeshDesc.eParticleTypeID = InstanceEffect_Ball;
		}ImGui::SameLine(0, 10);
		if (ImGui::Button("+", ImVec2(20, 18)))
		{
			m_tMeshDesc.eParticleTypeID = (eInstanceEffectID)(m_tMeshDesc.eParticleTypeID + 1);
			if (m_tMeshDesc.eParticleTypeID >= InstanceEffect_End) m_tMeshDesc.eParticleTypeID = (eInstanceEffectID)(InstanceEffect_End - 1);
		}
		ImGui::SameLine(0, 10);
		ImGui::Text(TAG_TEXINSTEFFECT(m_tMeshDesc.eParticleTypeID));

		//////////////////////////////////////////////////////////////////////////
		if (ImGui::Button("- ", ImVec2(20, 18)))
		{
			m_tMeshDesc.eInstanceCount = (COMPONENTPROTOTYPEID)(m_tMeshDesc.eInstanceCount - 1);
			if (m_tMeshDesc.eInstanceCount < Prototype_ModelInstance_1) m_tMeshDesc.eInstanceCount = Prototype_ModelInstance_1;
		}ImGui::SameLine(0, 10);
		if (ImGui::Button("+ ", ImVec2(20, 18)))
		{
			m_tMeshDesc.eInstanceCount = (COMPONENTPROTOTYPEID)(m_tMeshDesc.eInstanceCount + 1);
			if (m_tMeshDesc.eInstanceCount > Prototype_ModelInstance_512) m_tMeshDesc.eInstanceCount = (Prototype_ModelInstance_512);
		}
		ImGui::SameLine(0, 10);
		{	wstring tt = TAG_CP(m_tMeshDesc.eInstanceCount);	ImGui::Text(string(tt.begin(), tt.end()).c_str()); }

		//////////////////////////////////////////////////////////////////////////
		if (ImGui::Button("-  ", ImVec2(20, 18)))
		{
			m_tMeshDesc.ePassID = (eMeshInstancePassID)(m_tMeshDesc.ePassID - 1);
			if (m_tMeshDesc.ePassID < MeshPass_OriginColor) m_tMeshDesc.ePassID = MeshPass_OriginColor;
		}ImGui::SameLine(0, 10);
		if (ImGui::Button("+  ", ImVec2(20, 18)))
		{
			m_tMeshDesc.ePassID = (eMeshInstancePassID)(m_tMeshDesc.ePassID + 1);
			if (m_tMeshDesc.ePassID >= MeshPass_End) m_tMeshDesc.ePassID = (eMeshInstancePassID)(MeshPass_End - 1);
		}
		ImGui::SameLine(0, 10);		ImGui::Text(TAG_MESHINSTPASS(m_tMeshDesc.ePassID));
	}
	Make_VerticalSpacing(1);


	static bool	bIsFollowingTransform = false;
	{

		ImGui::Checkbox("FollowingTransform", &bIsFollowingTransform);

		if (bIsFollowingTransform)
		{
			m_tMeshDesc.FollowingTarget = (CTransform*)(m_vecBatchedObject[0].pObject->Get_Component(TAG_COM(Com_Transform)));
		}
		else
		{
			m_tMeshDesc.FollowingTarget = nullptr;
		}

	}

	if (bIsFollowingTransform)
	{
		if (ImGui::Button("-             ", ImVec2(20, 18)))
		{
			m_tMeshDesc.iFollowingDir = (eFollowingDirID)(m_tMeshDesc.iFollowingDir - 1);
			if (m_tMeshDesc.iFollowingDir < FollowingDir_Right) m_tMeshDesc.iFollowingDir = FollowingDir_Right;
		}ImGui::SameLine(0, 10);
		if (ImGui::Button("+             ", ImVec2(20, 18)))
		{
			m_tMeshDesc.iFollowingDir = (eFollowingDirID)(m_tMeshDesc.iFollowingDir + 1);
			if (m_tMeshDesc.iFollowingDir >= FollowingDir_End) m_tMeshDesc.iFollowingDir = (eFollowingDirID)(FollowingDir_End - 1);
		}

		ImGui::SameLine(0, 10);		ImGui::Text(Tag_InstancePass(m_tMeshDesc.iFollowingDir));
		
	}
	else
	{

		TempFloatArr[0] = m_tMeshDesc.vFixedPosition.x;
		TempFloatArr[1] = m_tMeshDesc.vFixedPosition.y;
		TempFloatArr[2] = m_tMeshDesc.vFixedPosition.z;
		ImGui::InputFloat3("SwpanPosition", TempFloatArr);
		m_tMeshDesc.vFixedPosition.x = TempFloatArr[0];
		m_tMeshDesc.vFixedPosition.y = TempFloatArr[1];
		m_tMeshDesc.vFixedPosition.z = TempFloatArr[2];


		ZeroMemory(TempFloatArr, sizeof(_float) * 4);
		TempFloatArr[0] = m_tMeshDesc.vPowerDirection.x;
		TempFloatArr[1] = m_tMeshDesc.vPowerDirection.y;
		TempFloatArr[2] = m_tMeshDesc.vPowerDirection.z;
		ImGui::InputFloat3("Force Direct", TempFloatArr);
		m_tMeshDesc.vPowerDirection.x = TempFloatArr[0];
		m_tMeshDesc.vPowerDirection.y = TempFloatArr[1];
		m_tMeshDesc.vPowerDirection.z = TempFloatArr[2];
	}


	Make_VerticalSpacing(1);

	if (m_tMeshDesc.ePassID >= MeshPass_MaskingNoising && m_tMeshDesc.ePassID <= MeshPass_MaskingNoising_Appear_Bright)
	{
		m_tMeshDesc.vNoisePushingDir;

		_float2 tt = m_tMeshDesc.vNoisePushingDir.Get_Nomalize();
		float fArr[2];
		memcpy(fArr, &tt, sizeof(float) * 2);
		ImGui::DragFloat2("Noise Pusing Dir", fArr, 0.0001f, -1, 1);
		memcpy(&tt, fArr, sizeof(float) * 2);
		m_tMeshDesc.vNoisePushingDir = tt.Get_Nomalize();


		int tempint = m_tMeshDesc.iNoiseTextureIndex;


		ImGui::InputInt("MoiseTex Index", &tempint);
		m_tMeshDesc.iNoiseTextureIndex = tempint;


		tempint = m_tMeshDesc.iMaskingTextureIndex;
		ImGui::InputInt("MaskingTex Index", &tempint);
		m_tMeshDesc.iMaskingTextureIndex = tempint;


		if (m_tMeshDesc.ePassID == MeshPass_MaskingNoising_Appear || m_tMeshDesc.ePassID == MeshPass_MaskingNoising_Appear_Bright)
		{
			ImGui::DragFloat("AppearTime", &m_tMeshDesc.fAppearTimer, 0.001f, 0, m_tMeshDesc.EachParticleLifeTime);
			m_tMeshDesc.fAppearTimer = min(max(m_tMeshDesc.fAppearTimer, 0.000001f), m_tMeshDesc.EachParticleLifeTime);
		}
	}
	else if (m_tMeshDesc.ePassID >= MeshPass_AllDistortion && m_tMeshDesc.ePassID <= MeshPass_Distortion_ColorMix_Bright)
	{


		int tempint = m_tMeshDesc.iNoiseTextureIndex;
		ImGui::InputInt("MoiseTex Index", &tempint);
		m_tMeshDesc.iNoiseTextureIndex = tempint;


		m_tMeshDesc.vNoisePushingDir;

		_float2 tt = m_tMeshDesc.vNoisePushingDir.Get_Nomalize();
		float fArr[2];
		memcpy(fArr, &tt, sizeof(float) * 2);
		ImGui::DragFloat2("Distortion Pusing Dir", fArr, 0.0001f, -1, 1);
		memcpy(&tt, fArr, sizeof(float) * 2);
		m_tMeshDesc.vNoisePushingDir = tt.Get_Nomalize();




		ImGui::DragFloat("Push Power", &m_tMeshDesc.fDistortionNoisingPushPower,0.001f, -100, 100);
	
	}

	Make_VerticalSpacing(1);


	static ImGuiTextFilter filter = "Prototype_Mesh_AlgaeRock_Ledge";
	{
		wstring ta = m_tMeshDesc.szModelMeshProtoTypeTag;
		string t2;
		strcpy_s(filter.InputBuf, t2.assign(ta.begin(), ta.end()).c_str());
	}

	filter.Draw("Input MeshProtoTypeTag");

	string Temp = string(filter.InputBuf);
	wstring wtemp;
	wtemp.assign(Temp.begin(), Temp.end());

	lstrcpy(m_tMeshDesc.szModelMeshProtoTypeTag, wtemp.c_str());

	Make_VerticalSpacing(1);


	TempFloatArr[2] = m_tMeshDesc.TotalParticleTime;
	ImGui::DragFloat("Total PaticleLifeTime", &TempFloatArr[2], 0.0035f, (_float)g_fDeltaTime);
	m_tMeshDesc.TotalParticleTime = TempFloatArr[2];


	TempFloatArr[3] = m_tMeshDesc.EachParticleLifeTime;
	ImGui::DragFloat("Each PaticleLifeTime", &TempFloatArr[3], 0.0035f, (_float)g_fDeltaTime);
	m_tMeshDesc.EachParticleLifeTime = TempFloatArr[3];

	Make_VerticalSpacing(1);


	TempIntArr[3] = (_int)m_tMeshDesc.SizeChageFrequency;
	ImGui::InputInt("SizeChageFrequency", &TempIntArr[3]);
	m_tMeshDesc.SizeChageFrequency = TempIntArr[3];


	ZeroMemory(TempFloatArr, sizeof(_float) * 4);
	TempFloatArr[0] = m_tMeshDesc.ParticleSize.x;
	TempFloatArr[1] = m_tMeshDesc.ParticleSize.y;
	TempFloatArr[2] = m_tMeshDesc.ParticleSize.z;
	ImGui::InputFloat3("ParticleSize1", TempFloatArr);
	m_tMeshDesc.ParticleSize.x = TempFloatArr[0];
	m_tMeshDesc.ParticleSize.y = TempFloatArr[1];
	m_tMeshDesc.ParticleSize.z = TempFloatArr[2];

	ZeroMemory(TempFloatArr, sizeof(_float) * 4);
	TempFloatArr[0] = m_tMeshDesc.ParticleSize2.x;
	TempFloatArr[1] = m_tMeshDesc.ParticleSize2.y;
	TempFloatArr[2] = m_tMeshDesc.ParticleSize2.z;
	ImGui::InputFloat3("ParticleSize2", TempFloatArr);
	m_tMeshDesc.ParticleSize2.x = TempFloatArr[0];
	m_tMeshDesc.ParticleSize2.y = TempFloatArr[1];
	m_tMeshDesc.ParticleSize2.z = TempFloatArr[2];

	Make_VerticalSpacing(1);

	ZeroMemory(TempIntArr, sizeof(_int) * 4);
	TempIntArr[0] = _int(m_tMeshDesc.ColorChageFrequency);
	ImGui::InputInt("ColorChageFrequency", &TempIntArr[0]);
	m_tMeshDesc.ColorChageFrequency = TempIntArr[0];

	ZeroMemory(TempFloatArr, sizeof(_float) * 4);
	TempFloatArr[0] = m_tMeshDesc.TargetColor.x;
	TempFloatArr[1] = m_tMeshDesc.TargetColor.y;
	TempFloatArr[2] = m_tMeshDesc.TargetColor.z;
	TempFloatArr[3] = m_tMeshDesc.TargetColor.w;
	ImGui::InputFloat4("TargetColor1", TempFloatArr);
	m_tMeshDesc.TargetColor.x = TempFloatArr[0];
	m_tMeshDesc.TargetColor.y = TempFloatArr[1];
	m_tMeshDesc.TargetColor.z = TempFloatArr[2];
	m_tMeshDesc.TargetColor.w = TempFloatArr[3];

	ZeroMemory(TempFloatArr, sizeof(_float) * 4);
	TempFloatArr[0] = m_tMeshDesc.TargetColor2.x;
	TempFloatArr[1] = m_tMeshDesc.TargetColor2.y;
	TempFloatArr[2] = m_tMeshDesc.TargetColor2.z;
	TempFloatArr[3] = m_tMeshDesc.TargetColor2.w;
	ImGui::InputFloat4("TargetColor2", TempFloatArr);
	m_tMeshDesc.TargetColor2.x = TempFloatArr[0];
	m_tMeshDesc.TargetColor2.y = TempFloatArr[1];
	m_tMeshDesc.TargetColor2.z = TempFloatArr[2];
	m_tMeshDesc.TargetColor2.w = TempFloatArr[3];



	Make_VerticalSpacing(1);



	ZeroMemory(TempFloatArr, sizeof(_float) * 4);
	TempFloatArr[3] = m_tMeshDesc.Particle_Power;
	ImGui::DragFloat("Particle_Power", &TempFloatArr[3], 0.0034f);
	m_tMeshDesc.Particle_Power = TempFloatArr[3];


	TempFloatArr[0] = m_tMeshDesc.PowerRandomRange.x;
	TempFloatArr[1] = m_tMeshDesc.PowerRandomRange.y;
	ImGui::InputFloat2("PowerRandomRange", TempFloatArr);
	m_tMeshDesc.PowerRandomRange.x = TempFloatArr[0];
	m_tMeshDesc.PowerRandomRange.y = TempFloatArr[1];

	TempFloatArr[0] = m_tMeshDesc.SubPowerRandomRange_RUL.x;
	TempFloatArr[1] = m_tMeshDesc.SubPowerRandomRange_RUL.y;
	TempFloatArr[2] = m_tMeshDesc.SubPowerRandomRange_RUL.z;
	ImGui::DragFloat3("Power Weight by RUL", TempFloatArr,0.03f);
	m_tMeshDesc.SubPowerRandomRange_RUL.x = TempFloatArr[0];
	m_tMeshDesc.SubPowerRandomRange_RUL.y = TempFloatArr[1];
	m_tMeshDesc.SubPowerRandomRange_RUL.z = TempFloatArr[2];

	
	Make_VerticalSpacing(1);


	TempFloatArr[3] = m_tMeshDesc.fMaxBoundaryRadius;
	ImGui::DragFloat("MaxBoundaryRadius", &TempFloatArr[3], 0.0034f);
	m_tMeshDesc.fMaxBoundaryRadius = TempFloatArr[3];


	ZeroMemory(TempFloatArr, sizeof(_float) * 4);
	TempFloatArr[0] = m_tMeshDesc.ParticleStartRandomPosMin.x;
	TempFloatArr[1] = m_tMeshDesc.ParticleStartRandomPosMin.y;
	TempFloatArr[2] = m_tMeshDesc.ParticleStartRandomPosMin.z;
	ImGui::InputFloat3("StartRandomPos  Min", TempFloatArr);
	m_tMeshDesc.ParticleStartRandomPosMin.x = TempFloatArr[0];
	m_tMeshDesc.ParticleStartRandomPosMin.y = TempFloatArr[1];
	m_tMeshDesc.ParticleStartRandomPosMin.z = TempFloatArr[2];


	ZeroMemory(TempFloatArr, sizeof(_float) * 4);
	TempFloatArr[0] = m_tMeshDesc.ParticleStartRandomPosMax.x;
	TempFloatArr[1] = m_tMeshDesc.ParticleStartRandomPosMax.y;
	TempFloatArr[2] = m_tMeshDesc.ParticleStartRandomPosMax.z;
	ImGui::InputFloat3("StartRandomPos  Max", TempFloatArr);
	m_tMeshDesc.ParticleStartRandomPosMax.x = TempFloatArr[0];
	m_tMeshDesc.ParticleStartRandomPosMax.y = TempFloatArr[1];
	m_tMeshDesc.ParticleStartRandomPosMax.z = TempFloatArr[2];

	Make_VerticalSpacing(1);


	ImGui::Checkbox("Emissive", &m_tMeshDesc.bEmissive); ImGui::SameLine();
	ImGui::Checkbox("bIsOclusion", &m_tMeshDesc.bIsOclusion); ImGui::SameLine();
	ImGui::Checkbox("bAutoTurn", &m_tMeshDesc.bAutoTurn); 

	if (m_tMeshDesc.bEmissive)
	{
		memcpy(TempFloatArr, &(m_tMeshDesc.vEmissive_SBB), sizeof(_float) * 3);
		ImGui::Text("Shade /Blur /Bright");

		ImGui::DragFloat3("  ", TempFloatArr, 0.0073f, 0, 1.f);

		memcpy(&(m_tMeshDesc.vEmissive_SBB), TempFloatArr, sizeof(_float) * 3);


	}

	if (m_tMeshDesc.bAutoTurn)
	{
		_float Angle = XMConvertToDegrees(m_tMeshDesc.fRotSpeed_Radian);
		ImGui::DragFloat("Rot Speed", &Angle,0.5f,0,FLT_MAX);
		m_tMeshDesc.fRotSpeed_Radian = XMConvertToRadians(max(Angle, 0));
	}


	Make_VerticalSpacing(1);


	//TempFloatArr[3] = m_tMeshDesc.m_fAlphaTestValue;
	//ImGui::DragFloat("AlphaTestValue", &TempFloatArr[3]);
	//m_tMeshDesc.m_fAlphaTestValue = TempFloatArr[3];



		if (ImGui::Button("Play Partlcle", ImVec2(-FLT_MIN, 30)))
	{
		GetSingle(CUtilityMgr)->Create_MeshInstance(SCENE_EDIT, m_tMeshDesc);
	}







	return S_OK;
}

HRESULT CScene_Edit::Widget_SaveLoadTextureParticle(_double fDeltatime)
{
	Make_VerticalSpacing(10);


	if (ImGui::Button("Clear TexParticle"))
		ImGui::OpenPopup("Clear TexParticle");
	ImGui::SameLine();
	if (ImGui::Button("Save TexParticle"))
		ImGui::OpenPopup("Save TexParticle");
	ImGui::SameLine();
	if (ImGui::Button("Laod TexParticle"))
		ImGui::OpenPopup("Laod TexParticle");



	ImVec2 center = ImGui::GetMainViewport()->GetCenter();

	// Always center this window when appearing
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("Clear TexParticle", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("!!!!!!!!!!!!!!!!Waring!!!!!!!!!!!!!!!!\n\n Delete Batched Object Without Save!!!\n\n	Please Check Save One more\n\n\n");
		ImGui::Separator();

		//static int unused_i = 0;
		//ImGui::Combo("Combo", &unused_i, "Delete\0Delete harder\0");

		//static bool dont_ask_me_next_time = false;
		//ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
		//ImGui::Checkbox("Don't ask me next time", &dont_ask_me_next_time);
		//ImGui::PopStyleVar();

		if (ImGui::Button("OK", ImVec2(130, 0))) {


			m_tParticleDesc = INSTPARTICLEDESC();


			ImGui::CloseCurrentPopup();
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(130, 0))) { ImGui::CloseCurrentPopup(); }
		ImGui::EndPopup();
	}


	//////////////////////////////////////////////////////////////////////////


	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("Save TexParticle", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{

		if (m_FilePathList.size() == 0)
		{
			m_FilePathList.clear();
			_tfinddata64_t fd;
			__int64 handle = _tfindfirst64(TEXT("../bin/Resources/Data/ParicleData/TextureParticle/*.*"), &fd);
			if (handle == -1 || handle == 0)
				return E_FAIL;

			_int iResult = 0;

			//char szCurPath[128] = "../bin/Resources/Data/Map/";
			//char szFullPath[128] = "";
			char szFilename[MAX_PATH];

			while (iResult != -1)
			{
				if (!lstrcmp(fd.name, L".") || !lstrcmp(fd.name, L".."))
				{
					iResult = _tfindnext64(handle, &fd);
					continue;
				}


				WideCharToMultiByte(CP_UTF8, 0, fd.name, -1, szFilename, sizeof(szFilename), NULL, NULL);
				//strcpy_s(szFullPath, szCurPath);
				//strcat_s(szFullPath, szFilename);
				m_FilePathList.push_back({ szFilename });


				iResult = _tfindnext64(handle, &fd);
			}


			_findclose(handle);

		}

		ImGui::Text("Save TexParticle!\n\nExist TexParticle DataList");

		static ImGuiTextFilter filter;

		char	szCheckforSameFileName[256] = "";

		if (ImGui::BeginListBox(" "))
		{
			auto iter = m_FilePathList.begin();


			for (; iter != m_FilePathList.end(); iter++)
			{
				const bool is_selected = false;

				if (filter.PassFilter(iter->c_str()))
				{
					if (ImGui::Selectable(iter->c_str(), is_selected))
					{
						strcpy_s(filter.InputBuf, iter->c_str());
					}

					if (!strcmp(iter->c_str(), filter.InputBuf))
						strcpy_s(szCheckforSameFileName, filter.InputBuf);
				}
			}
			ImGui::EndListBox();

		}

		filter.Draw("Input FileName");


		ImGui::Separator();
		if (ImGui::Button("Save", ImVec2(120, 0)))
		{

			if (strcmp(filter.InputBuf, ""))
			{

				if (!strcmp(szCheckforSameFileName, filter.InputBuf))
				{
					ImGui::OpenPopup("One More Check");
				}
				else
				{
					//실제 저장

					FAILED_CHECK(Sava_Data(filter.InputBuf, Data_Particle_Texture));

					ImGui::CloseCurrentPopup();
					m_FilePathList.clear();


				}
			}



		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			ImGui::CloseCurrentPopup();
			m_FilePathList.clear();
		}


		//서브 팝업
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		if (ImGui::BeginPopupModal("One More Check", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{

			ImGui::Text("Mapdata Already Exist\nDo you want to Override on it?");

			if (ImGui::Button("Ok", ImVec2(130, 0)))
			{

				//실제 저장
				Sava_Data(filter.InputBuf, Data_Particle_Texture);

				ImGui::CloseCurrentPopup();
				m_FilePathList.clear();
			}

			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(130, 0)))
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}


		ImGui::EndPopup();
	}


	//////////////////////////////////////////////////////////////////////////

	// Always center this window when appearing
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("Laod TexParticle", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Laod TexParticle!\n\n");
		ImGui::Separator();


		if (m_FilePathList.size() == 0)
		{
			m_FilePathList.clear();
			_tfinddata64_t fd;
			__int64 handle = _tfindfirst64(TEXT("../bin/Resources/Data/ParicleData/TextureParticle/*.*"), &fd);
			if (handle == -1 || handle == 0)
				return E_FAIL;

			_int iResult = 0;

			//char szCurPath[128] = "../bin/Resources/Data/Map/";
			//char szFullPath[128] = "";
			char szFilename[MAX_PATH];

			while (iResult != -1)
			{
				if (!lstrcmp(fd.name, L".") || !lstrcmp(fd.name, L".."))
				{
					iResult = _tfindnext64(handle, &fd);
					continue;
				}


				WideCharToMultiByte(CP_UTF8, 0, fd.name, -1, szFilename, sizeof(szFilename), NULL, NULL);
				//strcpy_s(szFullPath, szCurPath);
				//strcat_s(szFullPath, szFilename);
				m_FilePathList.push_back({ szFilename });


				iResult = _tfindnext64(handle, &fd);
			}


			_findclose(handle);

		}



		static ImGuiTextFilter filter;

		char	szCheckforSameFileName[256] = "";

		if (ImGui::BeginListBox(" "))
		{
			auto iter = m_FilePathList.begin();


			for (; iter != m_FilePathList.end(); iter++)
			{
				const bool is_selected = false;

				if (filter.PassFilter(iter->c_str()))
				{
					if (ImGui::Selectable(iter->c_str(), is_selected))
					{
						strcpy_s(filter.InputBuf, iter->c_str());
					}

					if (!strcmp(iter->c_str(), filter.InputBuf))
						strcpy_s(szCheckforSameFileName, filter.InputBuf);
				}
			}
			ImGui::EndListBox();

		}

		filter.Draw("Input FileName");




		if (ImGui::Button("OK", ImVec2(120, 0)))
		{

			if (strcmp(filter.InputBuf, ""))
			{
				if (!strcmp(szCheckforSameFileName, filter.InputBuf))
				{
					Load_Data(filter.InputBuf, Data_Particle_Texture);
					m_FilePathList.clear();
					ImGui::CloseCurrentPopup();
				}
			}
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0))) {
			m_FilePathList.clear();
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}






	return S_OK;
}

HRESULT CScene_Edit::Widget_SaveLoadMeshParticle(_double fDeltatime)
{


	Make_VerticalSpacing(10);


	if (ImGui::Button("Clear MeshParticle"))
		ImGui::OpenPopup("Clear MeshParticle");
	ImGui::SameLine();
	if (ImGui::Button("Save MeshParticle"))
		ImGui::OpenPopup("Save MeshParticle");
	ImGui::SameLine();
	if (ImGui::Button("Laod MeshParticle"))
		ImGui::OpenPopup("Laod MeshParticle");
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();

	// Always center this window when appearing
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("Clear MeshParticle", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("!!!!!!!!!!!!!!!!Waring!!!!!!!!!!!!!!!!\n\n Delete Batched Object Without Save!!!\n\n	Please Check Save One more\n\n\n");
		ImGui::Separator();

		//static int unused_i = 0;
		//ImGui::Combo("Combo", &unused_i, "Delete\0Delete harder\0");

		//static bool dont_ask_me_next_time = false;
		//ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
		//ImGui::Checkbox("Don't ask me next time", &dont_ask_me_next_time);
		//ImGui::PopStyleVar();

		if (ImGui::Button("OK", ImVec2(130, 0))) {


			m_tMeshDesc = INSTMESHDESC();


			ImGui::CloseCurrentPopup();
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(130, 0))) { ImGui::CloseCurrentPopup(); }
		ImGui::EndPopup();
	}


	//////////////////////////////////////////////////////////////////////////


	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("Save MeshParticle", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{

		if (m_FilePathList.size() == 0)
		{
			m_FilePathList.clear();
			_tfinddata64_t fd;
			__int64 handle = _tfindfirst64(TEXT("../bin/Resources/Data/ParicleData/MeshParticle/*.*"), &fd);
			if (handle == -1 || handle == 0)
				return E_FAIL;

			_int iResult = 0;

			//char szCurPath[128] = "../bin/Resources/Data/Map/";
			//char szFullPath[128] = "";
			char szFilename[MAX_PATH];

			while (iResult != -1)
			{
				if (!lstrcmp(fd.name, L".") || !lstrcmp(fd.name, L".."))
				{
					iResult = _tfindnext64(handle, &fd);
					continue;
				}


				WideCharToMultiByte(CP_UTF8, 0, fd.name, -1, szFilename, sizeof(szFilename), NULL, NULL);
				//strcpy_s(szFullPath, szCurPath);
				//strcat_s(szFullPath, szFilename);
				m_FilePathList.push_back({ szFilename });


				iResult = _tfindnext64(handle, &fd);
			}


			_findclose(handle);

		}

		ImGui::Text("Save MeshParticle!\n\nExist MeshParticleDataList");

		static ImGuiTextFilter filter;

		char	szCheckforSameFileName[256] = "";

		if (ImGui::BeginListBox(" "))
		{
			auto iter = m_FilePathList.begin();


			for (; iter != m_FilePathList.end(); iter++)
			{
				const bool is_selected = false;

				if (filter.PassFilter(iter->c_str()))
				{
					if (ImGui::Selectable(iter->c_str(), is_selected))
					{
						strcpy_s(filter.InputBuf, iter->c_str());
					}

					if (!strcmp(iter->c_str(), filter.InputBuf))
						strcpy_s(szCheckforSameFileName, filter.InputBuf);
				}
			}
			ImGui::EndListBox();

		}

		filter.Draw("Input FileName");


		ImGui::Separator();
		if (ImGui::Button("Save", ImVec2(120, 0)))
		{

			if (strcmp(filter.InputBuf, ""))
			{

				if (!strcmp(szCheckforSameFileName, filter.InputBuf))
				{
					ImGui::OpenPopup("One More Check");
				}
				else
				{
					//실제 저장

					FAILED_CHECK(Sava_Data(filter.InputBuf, Data_Particle_Mesh));

					ImGui::CloseCurrentPopup();
					m_FilePathList.clear();


				}
			}



		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			ImGui::CloseCurrentPopup();
			m_FilePathList.clear();
		}


		//서브 팝업
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		if (ImGui::BeginPopupModal("One More Check", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{

			ImGui::Text("Mapdata Already Exist\nDo you want to Override on it?");

			if (ImGui::Button("Ok", ImVec2(130, 0)))
			{

				//실제 저장
				Sava_Data(filter.InputBuf, Data_Particle_Mesh);

				ImGui::CloseCurrentPopup();
				m_FilePathList.clear();
			}

			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(130, 0)))
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}


		ImGui::EndPopup();
	}


	//////////////////////////////////////////////////////////////////////////

	// Always center this window when appearing
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("Laod MeshParticle", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Laod MeshParticle!\n\n");
		ImGui::Separator();


		if (m_FilePathList.size() == 0)
		{
			m_FilePathList.clear();
			_tfinddata64_t fd;
			__int64 handle = _tfindfirst64(TEXT("../bin/Resources/Data/ParicleData/MeshParticle/*.*"), &fd);
			if (handle == -1 || handle == 0)
				return E_FAIL;

			_int iResult = 0;

			//char szCurPath[128] = "../bin/Resources/Data/Map/";
			//char szFullPath[128] = "";
			char szFilename[MAX_PATH];

			while (iResult != -1)
			{
				if (!lstrcmp(fd.name, L".") || !lstrcmp(fd.name, L".."))
				{
					iResult = _tfindnext64(handle, &fd);
					continue;
				}


				WideCharToMultiByte(CP_UTF8, 0, fd.name, -1, szFilename, sizeof(szFilename), NULL, NULL);
				//strcpy_s(szFullPath, szCurPath);
				//strcat_s(szFullPath, szFilename);
				m_FilePathList.push_back({ szFilename });


				iResult = _tfindnext64(handle, &fd);
			}


			_findclose(handle);

		}



		static ImGuiTextFilter filter;

		char	szCheckforSameFileName[256] = "";

		if (ImGui::BeginListBox(" "))
		{
			auto iter = m_FilePathList.begin();


			for (; iter != m_FilePathList.end(); iter++)
			{
				const bool is_selected = false;

				if (filter.PassFilter(iter->c_str()))
				{
					if (ImGui::Selectable(iter->c_str(), is_selected))
					{
						strcpy_s(filter.InputBuf, iter->c_str());
					}

					if (!strcmp(iter->c_str(), filter.InputBuf))
						strcpy_s(szCheckforSameFileName, filter.InputBuf);
				}
			}
			ImGui::EndListBox();

		}

		filter.Draw("Input FileName");




		if (ImGui::Button("OK", ImVec2(120, 0)))
		{

			if (strcmp(filter.InputBuf, ""))
			{
				if (!strcmp(szCheckforSameFileName, filter.InputBuf))
				{
					Load_Data(filter.InputBuf, Data_Particle_Mesh);
					m_FilePathList.clear();
					ImGui::CloseCurrentPopup();
				}
			}
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0))) {
			m_FilePathList.clear();
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}









	return S_OK;
}



HRESULT CScene_Edit::Update_CameraActionTab(_double fDeltatime)
{


	FAILED_CHECK(Widget_CursorBatch(fDeltatime));

	FAILED_CHECK(Widget_CreatedCamPosListBox(fDeltatime));

	FAILED_CHECK(Widget_Play(fDeltatime));
	FAILED_CHECK(Widget_SaveLoadCamAction(fDeltatime));

	
	return S_OK;
}

HRESULT CScene_Edit::Widget_CursorBatch(_double fDeltatime)
{
	Make_VerticalSpacing(3);



	ImGui::RadioButton("CursorEdit", &iPickKinds, 0); ImGui::SameLine();
	if (ImGui::RadioButton("CamPosEdit", &iPickKinds, 1))
	{
		if (m_vecCamPosBatchedObj.size() <= 0)
			iPickKinds = 0;

		else
		{
			if (iCamPosIndex >= m_vecCamPosBatchedObj.size())
				iCamPosIndex = _uint(m_vecCamPosBatchedObj.size() - 1);


			CamDesc = m_vecCamPositions[iCamPosIndex];

		}
	}; ImGui::SameLine();

	if(ImGui::RadioButton("LookEdit", &iPickKinds, 2))
	{

		if (m_vecLookPostions.size() <= 0)
			iPickKinds = 0;

		else
		{
			if (iCamLookIndex >= m_vecLookPostions.size())
				iCamLookIndex = _uint(m_vecLookPostions.size() - 1);

			CamDesc = m_vecLookPostions[iCamLookIndex];

		}


	}




	Make_VerticalSpacing(3);

	static float TempFloat3Arr[3];
	switch (iPickKinds)
	{
	case 0:

		ImGui::DragFloat("Duration", &CamDesc.fDuration, 0.01f, 0);


		TempFloat3Arr[0] = CamDesc.vPosition.x;
		TempFloat3Arr[1] = CamDesc.vPosition.y;
		TempFloat3Arr[2] = CamDesc.vPosition.z;
		
		ImGui::DragFloat3("Position", TempFloat3Arr, 0.1f);
		

		CamDesc.vPosition.x = TempFloat3Arr[0];
		CamDesc.vPosition.y = TempFloat3Arr[1];
		CamDesc.vPosition.z = TempFloat3Arr[2];

		m_pCamCursor->Set_Position(CamDesc.vPosition);


		if (ImGui::Button("Create CamPosition", ImVec2(200, 30)))
		{
			CESCursor* pCursor = nullptr;
			FAILED_CHECK(m_pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&pCursor), SCENE_EDIT, TAG_OP(Prototype_EditorCursor)));
			NULL_CHECK_RETURN(pCursor, E_FAIL);
			pCursor->Set_Position(CamDesc.vPosition);
			pCursor->Set_Color({ 0, 0.5f, 0, 1 });

			m_vecCamPosBatchedObj.push_back(pCursor);

			CAMACTDESC tDesc = { 0 };

			tDesc = CamDesc;
			m_vecCamPositions.push_back(tDesc);


		} ImGui::SameLine();

		if (ImGui::Button("Create LookPosition", ImVec2(200, 30)))
		{
			CESCursor* pCursor = nullptr;
			FAILED_CHECK(m_pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&pCursor), SCENE_EDIT, TAG_OP(Prototype_EditorCursor)));
			NULL_CHECK_RETURN(pCursor, E_FAIL);
			pCursor->Set_Position(CamDesc.vPosition);
			pCursor->Set_Color({ 0, 0, 0.25f, 1 });
			m_vecLookBatchedObj.push_back(pCursor);

			CAMACTDESC tDesc = { 0 };

			tDesc = CamDesc;
			m_vecLookPostions.push_back(tDesc);



		}



		break;
	case 1: // CamPositions

	{
		{
			char buf[MAX_PATH];
			sprintf_s(buf, "CamPos Index : %d", iCamPosIndex);
			ImGui::Text(buf);
		}

		ImGui::DragFloat("Duration", &CamDesc.fDuration, 0.01f, 0);


		TempFloat3Arr[0] = CamDesc.vPosition.x;
		TempFloat3Arr[1] = CamDesc.vPosition.y;
		TempFloat3Arr[2] = CamDesc.vPosition.z;

		ImGui::DragFloat3("Position", TempFloat3Arr, 0.1f);


		CamDesc.vPosition.x = TempFloat3Arr[0];
		CamDesc.vPosition.y = TempFloat3Arr[1];
		CamDesc.vPosition.z = TempFloat3Arr[2];

		m_vecCamPosBatchedObj[iCamPosIndex]->Set_Position(CamDesc.vPosition);
		m_vecCamPositions[iCamPosIndex].vPosition = CamDesc.vPosition;
		m_vecCamPositions[iCamPosIndex].fDuration = CamDesc.fDuration;

		if (ImGui::Button("Delete CamPostion", ImVec2(-FLT_MIN, 30)))
		{

			auto ObjIter = m_vecCamPosBatchedObj.begin();
			ObjIter += iCamPosIndex;

			Safe_Release(*ObjIter);
			m_vecCamPosBatchedObj.erase(ObjIter);


			auto DescIter = m_vecCamPositions.begin();
			DescIter += iCamPosIndex;
			m_vecCamPositions.erase(DescIter);

			iCamPosIndex = 0;

			if (m_vecCamPositions.size() <= 0 || m_vecCamPosBatchedObj.size() <= 0)
			{
				iPickKinds = 0;
			}

		}


	}
		break;
	case 2: // CamLook
	{
		{
			char buf[MAX_PATH];
			sprintf_s(buf, "Look Index : %d", iCamLookIndex);
			ImGui::Text(buf);
		}

		ImGui::DragFloat("Duration", &CamDesc.fDuration, 0.01f, 0);


		TempFloat3Arr[0] = CamDesc.vPosition.x;
		TempFloat3Arr[1] = CamDesc.vPosition.y;
		TempFloat3Arr[2] = CamDesc.vPosition.z;

		ImGui::DragFloat3("Position", TempFloat3Arr, 0.1f);


		CamDesc.vPosition.x = TempFloat3Arr[0];
		CamDesc.vPosition.y = TempFloat3Arr[1];
		CamDesc.vPosition.z = TempFloat3Arr[2];


		m_vecLookBatchedObj[iCamLookIndex]->Set_Position(CamDesc.vPosition);
		m_vecLookPostions[iCamLookIndex].vPosition = CamDesc.vPosition;
		m_vecLookPostions[iCamLookIndex].fDuration = CamDesc.fDuration;

		if (ImGui::Button("Delete LookPosition", ImVec2(-FLT_MIN, 30)))
		{
			auto ObjIter = m_vecLookBatchedObj.begin();
			ObjIter += iCamLookIndex;

			Safe_Release(*ObjIter);
			m_vecLookBatchedObj.erase(ObjIter);


			auto DescIter = m_vecLookPostions.begin();
			DescIter += iCamLookIndex;
			m_vecLookPostions.erase(DescIter);

			iCamLookIndex = 0;

			if (m_vecLookPostions.size() <= 0 || m_vecLookBatchedObj.size() <= 0)
			{
				iPickKinds = 0;
			}
		}

	}
		break;


	default:
		break;
	}

	return S_OK;
}

HRESULT CScene_Edit::Widget_CreatedCamPosListBox(_double fDeltatime)
{

	Make_VerticalSpacing(5);

	_float AllTotalTime = 0;
	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_None | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_HorizontalScrollbar;
		{

			ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
			ImGui::BeginChild("ChildL", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, 130), true, window_flags);


			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("CamPosList"))
				{
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}

			if (ImGui::BeginTable("split", 1, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings))
			{
				_float TotalTime = 0;

				for (int i = 0; i < m_vecCamPositions.size(); i++)
				{

					char buf[MAX_PATH];
					TotalTime += m_vecCamPositions[i].fDuration;
					sprintf_s(buf, "%d : %3f(%3f)", i, m_vecCamPositions[i].fDuration, TotalTime);
					ImGui::TableNextColumn();

					if (ImGui::Button(buf, ImVec2(-FLT_MIN, 0.0f)))
					{
						iCamPosIndex = i;
						if(iPickKinds == 1)
							CamDesc = m_vecCamPositions[iCamPosIndex];
					}

					if (ImGui::IsItemHovered())
					{
						//m_iBatchedVecIndex = 0;
						//m_bIsModelMove = 0;
						//m_SelectedObjectSRT = &(m_vecBatchedObject[m_iBatchedVecIndex].matSRT);
						//ZeroMemory(m_iSelectedObjectNMesh, sizeof(_uint) * 2);
						//m_iSelectedObjectNMesh[0] = Prototype_StaticMapObject;
						//m_iSelectedObjectNMesh[1] = Prototype_Mesh_None;

					}
				}

				if (AllTotalTime < TotalTime)AllTotalTime = TotalTime;
				ImGui::EndTable();
			}
			ImGui::EndChild();
			ImGui::PopStyleVar();
		}

		ImGui::SameLine();

		{

			ImGui::BeginChild("ChildR", ImVec2(0, 130), true, window_flags);

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("CamLook List"))
				{
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();

				//char buf[128];
				//sprintf_s(buf, "%ws\n", TAG_MESH(MESHTYPEID(m_iSelectedObjectNMesh[1])));
				//ImGui::Text(buf);
			}

			if (ImGui::BeginTable("split", 1, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings))
			{
				_float TotalTime = 0;

				for (int i = 0; i < m_vecLookPostions.size(); i++)
				{

					char buf[MAX_PATH];
					TotalTime += m_vecLookPostions[i].fDuration;
					sprintf_s(buf, "%d : %.3f(%.3f)", i, m_vecLookPostions[i].fDuration, TotalTime);
					ImGui::TableNextColumn();

					if (ImGui::Button(buf, ImVec2(-FLT_MIN, 0.0f)))
					{
						iCamLookIndex = i;
						if (iPickKinds == 2)
							CamDesc = m_vecLookPostions[iCamLookIndex];
					}

					if (ImGui::IsItemHovered())
					{
						//m_iBatchedVecIndex = 0; m_bIsModelMove = 0;
						//m_SelectedObjectSRT = &(m_vecBatchedObject[m_iBatchedVecIndex].matSRT);
						//ZeroMemory(m_iSelectedObjectNMesh, sizeof(_uint) * 2);
						//m_iSelectedObjectNMesh[0] = Prototype_StaticMapObject;
						//m_iSelectedObjectNMesh[1] = Prototype_Mesh_None;

					}
				}
				if (AllTotalTime < TotalTime)AllTotalTime = TotalTime;
				ImGui::EndTable();
			}

			Make_VerticalSpacing(1);

			ImGui::EndChild();
		}
	}

	{
		char buf[MAX_PATH];
		sprintf_s(buf, "Total CamAction Time : %f", AllTotalTime);
		ImGui::Text(buf);
		Make_VerticalSpacing(2);
	}



	return S_OK;
}

HRESULT CScene_Edit::Widget_Play(_double fDeltatime)
{
	Make_VerticalSpacing(5);

	if (ImGui::Button("Play", ImVec2(200, 60.f)))
	{
		CAMERAACTION tDesc;

		tDesc.vecCamPos = m_vecCamPositions;
		tDesc.vecLookAt = m_vecLookPostions;

		m_pEditorCam->CamActionStart(tDesc);
	}


	return S_OK;
}

HRESULT CScene_Edit::Widget_SaveLoadCamAction(_double fDeltatime)
{
	Make_VerticalSpacing(10);


	if (ImGui::Button("Clear Action"))
		ImGui::OpenPopup("Clear Action");
	ImGui::SameLine();
	if (ImGui::Button("Save Action"))
		ImGui::OpenPopup("Save Action");
	ImGui::SameLine();
	if (ImGui::Button("Laod Action"))
		ImGui::OpenPopup("Laod Action");
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();

	// Always center this window when appearing
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("Clear Action", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("!!!!!!!!!!!!!!!!Waring!!!!!!!!!!!!!!!!\n\n Delete Batched Object Without Save!!!\n\n	Please Check Save One more\n\n\n");
		ImGui::Separator();

		//static int unused_i = 0;
		//ImGui::Combo("Combo", &unused_i, "Delete\0Delete harder\0");

		//static bool dont_ask_me_next_time = false;
		//ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
		//ImGui::Checkbox("Don't ask me next time", &dont_ask_me_next_time);
		//ImGui::PopStyleVar();

		if (ImGui::Button("OK", ImVec2(130, 0))) {


			for (auto& iter : m_vecCamPosBatchedObj)
				Safe_Release(iter);
			m_vecCamPosBatchedObj.clear();

			for (auto& iter : m_vecLookBatchedObj)
				Safe_Release(iter);
			m_vecLookBatchedObj.clear();

			m_vecCamPositions.clear();
			m_vecLookPostions.clear();

			iPickKinds = 0;


			ImGui::CloseCurrentPopup();
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(130, 0))) { ImGui::CloseCurrentPopup(); }
		ImGui::EndPopup();
	}


	//////////////////////////////////////////////////////////////////////////


	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("Save Action", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{

		if (m_FilePathList.size() == 0)
		{
			m_FilePathList.clear();
			_tfinddata64_t fd;
			__int64 handle = _tfindfirst64(TEXT("../bin/Resources/Data/CameraAction/*.*"), &fd);
			if (handle == -1 || handle == 0)
				return E_FAIL;

			_int iResult = 0;

			//char szCurPath[128] = "../bin/Resources/Data/Map/";
			//char szFullPath[128] = "";
			char szFilename[MAX_PATH];

			while (iResult != -1)
			{
				if (!lstrcmp(fd.name, L".") || !lstrcmp(fd.name, L".."))
				{
					iResult = _tfindnext64(handle, &fd);
					continue;
				}


				WideCharToMultiByte(CP_UTF8, 0, fd.name, -1, szFilename, sizeof(szFilename), NULL, NULL);
				//strcpy_s(szFullPath, szCurPath);
				//strcat_s(szFullPath, szFilename);
				m_FilePathList.push_back({ szFilename });


				iResult = _tfindnext64(handle, &fd);
			}


			_findclose(handle);

		}

		ImGui::Text("Save Action!\n\nExist ActionDataList");

		static ImGuiTextFilter filter;

		char	szCheckforSameFileName[256] = "";

		if (ImGui::BeginListBox(" "))
		{
			auto iter = m_FilePathList.begin();


			for (; iter != m_FilePathList.end(); iter++)
			{
				const bool is_selected = false;

				if (filter.PassFilter(iter->c_str()))
				{
					if (ImGui::Selectable(iter->c_str(), is_selected))
					{
						strcpy_s(filter.InputBuf, iter->c_str());
					}

					if (!strcmp(iter->c_str(), filter.InputBuf))
						strcpy_s(szCheckforSameFileName, filter.InputBuf);
				}
			}
			ImGui::EndListBox();

		}

		filter.Draw("Input FileName");


		ImGui::Separator();
		if (ImGui::Button("Save", ImVec2(120, 0)))
		{

			if (strcmp(filter.InputBuf, ""))
			{

				if (!strcmp(szCheckforSameFileName, filter.InputBuf))
				{
					ImGui::OpenPopup("One More Check");
				}
				else
				{
					//실제 저장

					Sava_Data(filter.InputBuf, Data_CameraAction);

					ImGui::CloseCurrentPopup();
					m_FilePathList.clear();


				}
			}



		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			ImGui::CloseCurrentPopup();
			m_FilePathList.clear();
		}


		//서브 팝업
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		if (ImGui::BeginPopupModal("One More Check", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{

			ImGui::Text("Mapdata Already Exist\nDo you want to Override on it?");

			if (ImGui::Button("Ok", ImVec2(130, 0)))
			{

				//실제 저장
				Sava_Data(filter.InputBuf, Data_CameraAction);

				ImGui::CloseCurrentPopup();
				m_FilePathList.clear();
			}

			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(130, 0)))
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}


		ImGui::EndPopup();
	}


	//////////////////////////////////////////////////////////////////////////

	// Always center this window when appearing
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("Laod Action", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Laod Action!\n\n");
		ImGui::Separator();


		if (m_FilePathList.size() == 0)
		{
			m_FilePathList.clear();
			_tfinddata64_t fd;
			__int64 handle = _tfindfirst64(TEXT("../bin/Resources/Data/CameraAction/*.*"), &fd);
			if (handle == -1 || handle == 0)
				return E_FAIL;

			_int iResult = 0;

			//char szCurPath[128] = "../bin/Resources/Data/Map/";
			//char szFullPath[128] = "";
			char szFilename[MAX_PATH];

			while (iResult != -1)
			{
				if (!lstrcmp(fd.name, L".") || !lstrcmp(fd.name, L".."))
				{
					iResult = _tfindnext64(handle, &fd);
					continue;
				}


				WideCharToMultiByte(CP_UTF8, 0, fd.name, -1, szFilename, sizeof(szFilename), NULL, NULL);
				//strcpy_s(szFullPath, szCurPath);
				//strcat_s(szFullPath, szFilename);
				m_FilePathList.push_back({ szFilename });


				iResult = _tfindnext64(handle, &fd);
			}


			_findclose(handle);

		}



		static ImGuiTextFilter filter;

		char	szCheckforSameFileName[256] = "";

		if (ImGui::BeginListBox(" "))
		{
			auto iter = m_FilePathList.begin();


			for (; iter != m_FilePathList.end(); iter++)
			{
				const bool is_selected = false;

				if (filter.PassFilter(iter->c_str()))
				{
					if (ImGui::Selectable(iter->c_str(), is_selected))
					{
						strcpy_s(filter.InputBuf, iter->c_str());
					}

					if (!strcmp(iter->c_str(), filter.InputBuf))
						strcpy_s(szCheckforSameFileName, filter.InputBuf);
				}
			}
			ImGui::EndListBox();

		}

		filter.Draw("Input FileName");




		if (ImGui::Button("OK", ImVec2(120, 0)))
		{

			if (strcmp(filter.InputBuf, ""))
			{
				if (!strcmp(szCheckforSameFileName, filter.InputBuf))
				{
					Load_Data(filter.InputBuf, Data_CameraAction);
					m_FilePathList.clear();
					ImGui::CloseCurrentPopup();
				}
			}
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0))) {
			m_FilePathList.clear();
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}






	return S_OK;
}


HRESULT CScene_Edit::Update_HeightMap(_double fDeltatime)
{



	FAILED_CHECK(Widget_CreateDeleteHeightMap(fDeltatime));
	
	Make_VerticalSpacing(5);


	return S_OK;
}

HRESULT CScene_Edit::Widget_CreateDeleteHeightMap(_double fDeltatime)
{
	if (m_pCreatedTerrain == nullptr)
	{
		if (ImGui::TreeNode("Create HeightMap"))
		{

			ImGui::InputInt("Input X Size", &(m_iMapSize[0]));
			ImGui::InputInt("Input Z Size", &(m_iMapSize[1]));

			if (m_iMapSize[0] < 1) m_iMapSize[0] = 1;
			if (m_iMapSize[1] < 1) m_iMapSize[1] = 1;


			char buf[MAX_PATH];
			sprintf_s(buf, "X Size : %d\nZ Size : %d", _int(pow(2, m_iMapSize[0])) + 1, _int(pow(2, m_iMapSize[1])) + 1);
			ImGui::Text(buf);

			if (ImGui::Button("Create Height Map", ImVec2(-FLT_MIN, 20.0f)))
			{
				FAILED_CHECK(m_pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&m_pCreatedTerrain), SCENE_EDIT, TAG_OP(Prototype_EditorTerrain)));


				FAILED_CHECK(m_pCreatedTerrain->Change_Component_by_Parameter(CVIBuffer_EditorTerrain::Create(m_pDevice, m_pDeviceContext, _int(pow(2, m_iMapSize[0])) + 1, _int(pow(2, m_iMapSize[1])) + 1), TAG_COM(Com_VIBuffer)));
			
			}



			Make_VerticalSpacing(10);



			if (ImGui::Button("Laod Hight Map", ImVec2(-FLT_MIN, 20.0f)))
				ImGui::OpenPopup("Laod Hight Map");

			ImVec2 center = ImGui::GetMainViewport()->GetCenter();
			ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

			if (ImGui::BeginPopupModal("Laod Hight Map", NULL, ImGuiWindowFlags_AlwaysAutoResize))
			{
				ImGui::Text("Laod Hight Map!\n\n");
				ImGui::Separator();


				if (m_FilePathList.size() == 0)
				{
					m_FilePathList.clear();
					_tfinddata64_t fd;
					__int64 handle = _tfindfirst64(TEXT("../bin/Resources/Textures/HeightMap/*.*"), &fd);
					if (handle == -1 || handle == 0)
						return E_FAIL;

					_int iResult = 0;

					//char szCurPath[128] = "../bin/Resources/Data/Map/";
					//char szFullPath[128] = "";
					char szFilename[MAX_PATH];

					while (iResult != -1)
					{
						if (!lstrcmp(fd.name, L".") || !lstrcmp(fd.name, L".."))
						{
							iResult = _tfindnext64(handle, &fd);
							continue;
						}


						WideCharToMultiByte(CP_UTF8, 0, fd.name, -1, szFilename, sizeof(szFilename), NULL, NULL);
						//strcpy_s(szFullPath, szCurPath);
						//strcat_s(szFullPath, szFilename);
						m_FilePathList.push_back({ szFilename });


						iResult = _tfindnext64(handle, &fd);
					}


					_findclose(handle);

				}



				static ImGuiTextFilter filter;

				char	szCheckforSameFileName[256] = "";

				if (ImGui::BeginListBox(" "))
				{
					auto iter = m_FilePathList.begin();


					for (; iter != m_FilePathList.end(); iter++)
					{
						const bool is_selected = false;

						if (filter.PassFilter(iter->c_str()))
						{
							if (ImGui::Selectable(iter->c_str(), is_selected))
							{
								strcpy_s(filter.InputBuf, iter->c_str());
							}

							if (!strcmp(iter->c_str(), filter.InputBuf))
								strcpy_s(szCheckforSameFileName, filter.InputBuf);
						}
					}
					ImGui::EndListBox();

				}

				filter.Draw("Input FileName");




				if (ImGui::Button("OK", ImVec2(120, 0)))
				{

					if (strcmp(filter.InputBuf, ""))
					{
						if (!strcmp(szCheckforSameFileName, filter.InputBuf))
						{

							Load_Data(filter.InputBuf, Data_HeightMap);
							m_FilePathList.clear();
							ImGui::CloseCurrentPopup();
						}
					}
				}
				ImGui::SetItemDefaultFocus();
				ImGui::SameLine();
				if (ImGui::Button("Cancel", ImVec2(120, 0))) {
					m_FilePathList.clear();
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}

			ImGui::TreePop();
		}
	}
	else 
	{
		/////////////////////////////////////////////////////////
		CGameInstance* pInstance = g_pGameInstance;

		if (pInstance->Get_DIMouseButtonState(CInput_Device::MBS_LBUTTON) & DIS_DoubleDown)
		{
			POINT ptMouse;
			GetCursorPos(&ptMouse);
			ScreenToClient(g_hWnd, &ptMouse);



			_Vector vCursorPos = XMVectorSet(
				(_float(ptMouse.x) / (g_iWinCX * 0.5f)) - 1.f,
				(_float(ptMouse.y) / -(g_iWinCY * 0.5f)) + 1.f,
				0, 1.f);
			//_Vector vCursorPos = XMVectorSet(	(_float(ptMouse.x) / g_iWinCX * 0.5f) - 1.f, 	(_float(ptMouse.y) / g_iWinCY * 0.5f) + 1.f ,		0, 1.f);

			_Matrix InvProjMat = XMMatrixInverse(nullptr, pInstance->Get_Transform_Matrix(PLM_PROJ));

			_Vector vRayDir = XMVector4Transform(vCursorPos, InvProjMat) - XMVectorSet(0, 0, 0, 1);

			_Matrix InvViewMat = XMMatrixInverse(nullptr, pInstance->Get_Transform_Matrix(PLM_VIEW));
			vRayDir = XMVector3TransformNormal(vRayDir, InvViewMat);

			//vRayDir = XMVector3TransformNormal(vRayDir, m_pCreatedTerrain->Get_Component(TAG_COM(COm_C))->Get_Camera_Transform()->Get_InverseWorldMatrix());
			

			if (m_pCreatedTerrain)
			{
				_Vector vCamPos = m_pEditorCam->Get_Camera_Transform()->Get_MatrixState(CTransform::STATE_POS);

				_Vector vOldPos = vCamPos;
				_Vector vNewPos;
				_float3 vResult;
				_bool IsPicked = false;


				for (_uint i = 0; i < 200; i++)
				{
					vNewPos = vOldPos + vRayDir;

					vResult = m_pCreatedTerrain->Pick_OnTerrain(&IsPicked, vNewPos, vOldPos);

					if (IsPicked)
					{

						//wstring ResultString = L"X : " + to_wstring(vResult.x) + L"	Y : " + to_wstring(vResult.y) + L"	Z : " + to_wstring(vResult.z) + L"\n";
						//OutputDebugStringW(ResultString.c_str());
						m_fPickingedPosition[0] = vResult.x;
						m_fPickingedPosition[1] = vResult.y;
						m_fPickingedPosition[2] = vResult.z;
						break;
					}

					vOldPos = vNewPos;
				}

				if (IsPicked && !m_iBatchedVecIndex)
				{
					memcpy(m_vecBatchedObject[0].matSRT.m[2], &vResult, sizeof(_float3));

					RenewElenmetTransform(&m_vecBatchedObject[0]);
				}
			}
			else
			{
				_Vector vCamPos = m_pEditorCam->Get_Camera_Transform()->Get_MatrixState(CTransform::STATE_POS);

				if (XMVectorGetY(vCamPos) * XMVectorGetY(vRayDir) < 0)
				{
					_float Scale = XMVectorGetY(vCamPos) / -XMVectorGetY(vRayDir);

					_float3 vTargetPos = vCamPos + Scale * vRayDir;

					m_fPickingedPosition[0] = vTargetPos.x;
					m_fPickingedPosition[1] = vTargetPos.y;
					m_fPickingedPosition[2] = vTargetPos.z;

					if (!m_iBatchedVecIndex)
					{

						memcpy(m_vecBatchedObject[0].matSRT.m[2], &vTargetPos, sizeof(_float3));

						RenewElenmetTransform(&m_vecBatchedObject[0]);
					}

				}
			}
		}
		//////////////////////////////////////////////

		Make_VerticalSpacing(3);

		char PickingPos[128] = "";
		if (m_pCreatedTerrain)
		{
			sprintf_s(PickingPos, "Picked Position : ( %f   , %f   , %f   )\n",
				m_fPickingedPosition[0], m_fPickingedPosition[1], m_fPickingedPosition[2]);
		}
		else
		{
			sprintf_s(PickingPos, "Picked Position : ( %f   , %f   , %f   )\n", m_fPickingedPosition[0], m_fPickingedPosition[1], m_fPickingedPosition[2]);
		}
		ImGui::Text(PickingPos);

		Make_VerticalSpacing(3);

		string VertexIndexText = "VertexIndex : " + to_string(m_iVertexIndex) + "\n";

		//static char str0[128] = "Hello, world!";
		ImGui::Text((VertexIndexText.c_str()));

		Make_VerticalSpacing(3);

		ibClickChecker = 0;
		ImGui::RadioButton("Terrain_Position", &m_iRadioNumMoving, 0); ImGui::SameLine();
		ibClickChecker += _uint(ImGui::IsItemClicked());
		ImGui::RadioButton("Terrain_Rotation", &m_iRadioNumMoving, 1); ImGui::SameLine();
		ibClickChecker += _uint(ImGui::IsItemClicked());

		if (ImGui::Button("VertexCountReset"))
		{
			m_iVertexIndex = 0;
			ZeroMemory(m_vVertexts, sizeof(_float3) * 3);
		}

		if (ibClickChecker)
		{
			ZeroMemory(m_TerrainArrBuffer, sizeof(_float) * 4);
			m_TerrainArrBuffer[3] = 0.1f;
		}

		Make_VerticalSpacing(2);

		switch (m_iRadioNumMoving)
		{
		case 0://Terrain Pos
		{
			CTransform* Transform = static_cast<CTransform*>(m_pCreatedTerrain->Get_Component(TEXT("Com_Transform")));


			memcpy(m_TerrainArrBuffer, &(m_TerrainObjectSRT.m[2]), sizeof(_float) * 3);
			ImGui::DragFloat3("Terrain X Y Z", m_TerrainArrBuffer, m_TerrainArrBuffer[3], -FLT_MAX, FLT_MAX);

			memcpy(&(m_TerrainObjectSRT.m[2]), m_TerrainArrBuffer, sizeof(_float) * 3);

			_float Temp[4] = { 1.f, 1.f, 1.f, 0.f };
			memcpy(&(m_TerrainObjectSRT.m[0]), Temp, sizeof(_float) * 3);

			_Matrix Trans = XMMatrixTranslation(m_TerrainObjectSRT.m[2][0], m_TerrainObjectSRT.m[2][1], m_TerrainObjectSRT.m[2][2]);
			_Matrix RotX = XMMatrixRotationX(XMConvertToRadians(m_TerrainObjectSRT.m[1][0]));
			_Matrix RotY = XMMatrixRotationY(XMConvertToRadians(m_TerrainObjectSRT.m[1][1]));
			_Matrix RotZ = XMMatrixRotationZ(XMConvertToRadians(m_TerrainObjectSRT.m[1][2]));
			_Matrix Scale = XMMatrixScaling((m_TerrainObjectSRT.m[0][0]), (m_TerrainObjectSRT.m[0][1]), (m_TerrainObjectSRT.m[0][2]));


			_float4x4 Matrix = Scale* RotX *RotY* RotZ* Trans;

			Transform->Set_Matrix(Matrix);

			break;
			/*CTransform* Transform = static_cast<CTransform*>(m_pCreatedTerrain->Get_Component(TEXT("Com_Transform")));

			m_vTerrainPos = Transform->Get_MatrixState(CTransform::STATE_POS);
			ImGui::DragFloat3(" Terrain X Y Z", m_TerrainArrBuffer, 0.1f, -FLT_MAX, FLT_MAX);

			Transform->Set_MatrixState(CTransform::STATE_POS, _float3(m_TerrainArrBuffer[0], m_TerrainArrBuffer[1], m_TerrainArrBuffer[2]));
			break;*/
		}
		case 1://Terrain Rotation
		{
			CTransform* Transform = static_cast<CTransform*>(m_pCreatedTerrain->Get_Component(TEXT("Com_Transform")));

			memcpy(m_TerrainArrBuffer, &(m_TerrainObjectSRT.m[1]), sizeof(_float) * 3);
			ImGui::DragFloat3("Terrain X Y Z", m_TerrainArrBuffer, m_TerrainArrBuffer[3], -FLT_MAX, FLT_MAX);

			memcpy(&(m_TerrainObjectSRT.m[1]), m_TerrainArrBuffer, sizeof(_float) * 3);

			_float Temp[4] = { 1.f, 1.f, 1.f, 0.f };
			memcpy(&(m_TerrainObjectSRT.m[0]), Temp, sizeof(_float) * 3);

			_Matrix Trans = XMMatrixTranslation(m_TerrainObjectSRT.m[2][0], m_TerrainObjectSRT.m[2][1], m_TerrainObjectSRT.m[2][2]);
			_Matrix RotX = XMMatrixRotationX(XMConvertToRadians(m_TerrainObjectSRT.m[1][0]));
			_Matrix RotY = XMMatrixRotationY(XMConvertToRadians(m_TerrainObjectSRT.m[1][1]));
			_Matrix RotZ = XMMatrixRotationZ(XMConvertToRadians(m_TerrainObjectSRT.m[1][2]));
			_Matrix Scale = XMMatrixScaling((m_TerrainObjectSRT.m[0][0]), (m_TerrainObjectSRT.m[0][1]), (m_TerrainObjectSRT.m[0][2]));


			_float4x4 Matrix = Scale* RotX *RotY* RotZ* Trans;

			Transform->Set_Matrix(Matrix);
			break;
		}
		}

		Make_VerticalSpacing(1);

		ImGui::RadioButton("Navi_Picking", &m_iPickingModeNumber, 0); ImGui::SameLine();
		ImGui::RadioButton("Navi_Modify", &m_iPickingModeNumber, 1); ImGui::SameLine();

		if (m_iPickingModeNumber == 0)
		{
			if (m_iVertexIndex == 3)
			{
				//시계방향으로 바꾸기.
				_int iCCW = CCWClockSort();

				if (iCCW > 0)
				{
					_float3 Temp = m_vVertexts[0];

					m_vVertexts[0] = m_vVertexts[2];
					m_vVertexts[2] = Temp;
				}

				CCell*		pCell = CCell::Create(m_pDevice, m_pDeviceContext, m_vVertexts, (_uint)m_Cells.size());
				if (nullptr != pCell)
				{
					for (_int i = 0; i < m_Points.size(); ++i)
					{
						CTransform* PointTransform = (CTransform*)m_Points[i]->Get_Component(TAG_COM(Com_Transform));
						_float3 Pos = PointTransform->Get_MatrixState(CTransform::STATE_POS);

						for (_int j = 0; j < 3; ++j)
						{
							if (Pos == m_vVertexts[j])
							{
								m_Points[i]->Push_Cells(pCell);
								break;
							}
						}
					}
				}

				ZeroMemory(m_vVertexts, sizeof(_float3) * 3);

				m_Cells.push_back(pCell);
				m_iVertexIndex = 0;

				++m_iCellCount;
				char sIntStr[5];
				_itoa_s(m_iCellCount, sIntStr, 10);
				char str[10] = "Cell";
				strcat_s(str, sIntStr);

				char* sCellNum = NEW char[10];

				strcpy_s(sCellNum, 10, str);

				m_vCellNames.push_back(sCellNum);
			}

			Add_Vertex();
		}
		else if (m_iPickingModeNumber == 1)
		{
			Make_VerticalSpacing(2);

			ImGui::RadioButton("Vertex", &m_iVertexAndCell, 0); ImGui::SameLine();
			ImGui::RadioButton("Cell", &m_iVertexAndCell, 1); ImGui::SameLine();

			if (m_iVertexAndCell == 0)
			{
				if (g_pGameInstance->Get_DIMouseButtonState(CInput_Device::MBS_RBUTTON) & DIS_Down)
				{
					for (auto& Point : m_Points)
					{
						CCollisionMgr::EDITPOINTCOLLIDER TypeColl;

						TypeColl.vCollider = Point->Get_Collider();
						TypeColl.GameObject = Point;
						TypeColl.vCollider->Get_Edit_ColliderBuffer()->Set_IsConflicted(false);
						g_pGameInstance->Add_NaviPointCollider(TypeColl);
					}

					POINT ptMouse;
					GetCursorPos(&ptMouse);
					ScreenToClient(g_hWnd, &ptMouse);

					_Vector vCursorPos = XMVectorSet(
						(_float(ptMouse.x) / (g_iWinCX * 0.5f)) - 1.f,
						(_float(ptMouse.y) / -(g_iWinCY * 0.5f)) + 1.f,
						0, 1.f);

					_Matrix InvProjMat = XMMatrixInverse(nullptr, g_pGameInstance->Get_Transform_Matrix(PLM_PROJ));

					_Vector vRayDir = XMVector4Transform(vCursorPos, InvProjMat) - XMVectorSet(0, 0, 0, 1);
					_float3 m_vRayPos = _float3(0.f, 0.f, 0.f);

					_Matrix InvViewMat = XMMatrixInverse(nullptr, g_pGameInstance->Get_Transform_Matrix(PLM_VIEW));
					vRayDir = XMVector3TransformNormal(vRayDir, InvViewMat);
					XMStoreFloat3(&m_vRayPos, XMVector3TransformCoord(XMLoadFloat3(&m_vRayPos), InvViewMat));

					m_pPointObj = static_cast<CNaviPoint*>(g_pGameInstance->NaviPointCollision(XMLoadFloat3(&m_vRayPos), vRayDir));
				}
				if (nullptr != m_pPointObj)
				{
					CCollider* PointCollider = m_pPointObj->Get_Collider();
					CColliderBuffer* ColliderBuf = PointCollider->Get_Edit_ColliderBuffer();
					ColliderBuf->Set_IsConflicted(true);
					Make_VerticalSpacing(2);
					CTransform* Transform = (CTransform*)m_pPointObj->Get_Component(TAG_COM(Com_Transform));
					_float3 Pos = Transform->Get_MatrixState(CTransform::STATE_POS);
					memcpy(m_VertexArrBuffer, &Pos, sizeof(_float) * 3);
					ImGui::DragFloat3("Vertex  X Y Z", m_VertexArrBuffer, 0.1f, -FLT_MAX, FLT_MAX);
					Transform->Set_MatrixState(CTransform::STATE_POS, _float3(m_VertexArrBuffer[0], m_VertexArrBuffer[1], m_VertexArrBuffer[2]));

					for (_int i = 0; i < m_Points.size(); ++i)
					{

						if (m_Points[i] == m_pPointObj)
						{
							m_iVertexListCount = i;
							m_Points[i]->ReLocationCell(Pos, Transform->Get_MatrixState(CTransform::STATE_POS));
						}
					}
				}
			}
			else if (m_iVertexAndCell == 1)
			{
				if (g_pGameInstance->Get_DIMouseButtonState(CInput_Device::MBS_RBUTTON) & DIS_Down)
				{
					POINT ptMouse;
					GetCursorPos(&ptMouse);
					ScreenToClient(g_hWnd, &ptMouse);

					_Vector vCursorPos = XMVectorSet(
						(_float(ptMouse.x) / (g_iWinCX * 0.5f)) - 1.f,
						(_float(ptMouse.y) / -(g_iWinCY * 0.5f)) + 1.f,
						0, 1.f);

					_Matrix InvProjMat = XMMatrixInverse(nullptr, pInstance->Get_Transform_Matrix(PLM_PROJ));

					_Vector vRayDir = XMVector4Transform(vCursorPos, InvProjMat) - XMVectorSet(0, 0, 0, 1);

					_Matrix InvViewMat = XMMatrixInverse(nullptr, pInstance->Get_Transform_Matrix(PLM_VIEW));
					vRayDir = XMVector3TransformNormal(vRayDir, InvViewMat);

					if (m_pCreatedTerrain)
					{
						_Vector vCamPos = m_pEditorCam->Get_Camera_Transform()->Get_MatrixState(CTransform::STATE_POS);

						_Vector vOldPos = vCamPos;
						_Vector vNewPos;
						_float3 vResult;
						_bool IsPicked = false;


						for (_uint i = 0; i < 200; i++)
						{
							vNewPos = vOldPos + vRayDir;

							vResult = m_pCreatedTerrain->Pick_OnTerrain(&IsPicked, vNewPos, vOldPos);

							if (IsPicked)
							{
								_Vector Pos = XMLoadFloat3(&vResult);
								Pos = XMVectorSetW(Pos, 1.f);
								for (_int j = 0; j < m_Cells.size(); ++j)
								{
									if (nullptr != m_Cells[j])
									{
										_int	NeighborIndex = -1;
										_int	Line[3] = { 1, 1, 1 };
										if (m_Cells[j]->isIn(Pos, &NeighborIndex, Line))
										{
											m_bIsCellOption = true;
											m_OptionNumber = m_Cells[j]->Get_CellOption();
											m_iCellListCount = j;
										}
									}
								}
								break;
							}

							vOldPos = vNewPos;
						}
					}
				}
				
			}

			
		}

		Make_VerticalSpacing(2);

		ImGui::Text("VertexList"); ImGui::SameLine(200.f, 1.f); ImGui::Text("CellList");


		if (ImGui::BeginListBox("", ImVec2(150.f, 70.f)))
		{
			for (int n = 0; n < m_vVertexs.size(); n++)
			{
				const bool is_selected = (m_iVertexListCount == n);
				if (ImGui::Selectable(m_vVertexs[n], is_selected))
				{
					m_iVertexListCount = n;
				}

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndListBox();
		}

		ImGui::SameLine(200.f, 1.f);

		if (ImGui::BeginListBox(" ", ImVec2(150.f, 70.f)))
		{
			for (int n = 0; n < m_vCellNames.size(); n++)
			{
				const bool is_selected = (m_iCellListCount == n);
				if (ImGui::Selectable(m_vCellNames[n], is_selected))
				{
					m_iCellListCount = n;

				}
				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
				{
					ImGui::SetItemDefaultFocus();
					m_bIsCellListClick = is_selected;
				}
			}
			ImGui::EndListBox();
		}

		Make_VerticalSpacing(2);
		if (ImGui::Button("Point Delete"))
		{
			if (m_vVertexs.size() > 0)
			{
				Safe_Delete(m_vVertexs[m_iVertexListCount]);
				m_vVertexs.erase(m_vVertexs.begin() + m_iVertexListCount);

				m_Points[m_iVertexListCount]->Set_IsDead();
				m_Points.erase(m_Points.begin() + m_iVertexListCount);

				m_iVertexListCount = 0;
			}
		}

		ImGui::SameLine(200.f, 1.f);

		if (ImGui::Button("Cell Delete"))
		{
			if (m_vCellNames.size() > 0)
			{
				Safe_Delete(m_vCellNames[m_iCellListCount]);
				m_vCellNames.erase(m_vCellNames.begin() + m_iCellListCount);


				Safe_Release(m_Cells[m_iCellListCount]);
				vector<CCell*>::iterator  itr =  m_Cells.erase(m_Cells.begin() + m_iCellListCount);

				for (itr; itr != m_Cells.end(); ++itr)
				{
					
					(*itr)->Set_Index((*itr)->Get_Index() - 1);
				}
				
				m_iCellListCount = 0;
			}
		}

		Make_VerticalSpacing(2);

		const char* items[] = { "CELL_NOMAL", "CELL_DROP","CELL_JUMPZONE","CELL_BLOCKZONE" };
		static int item_current = 0;
		ImGui::Combo("Cell Option", &item_current, items, IM_ARRAYSIZE(items));
		if (m_bIsCellListClick)
		{
			if (m_bIsCellOption)
			{
				item_current = m_OptionNumber;
				m_bIsCellOption = false;
			}
			if (m_Cells.size() > 0)
			{
				if (item_current == 0)
				{
					m_Cells[m_iCellListCount]->Set_CellOption(CCell::CELL_NOMAL);
				}
				else if (item_current == 1)
					m_Cells[m_iCellListCount]->Set_CellOption(CCell::CELL_DROP);
				else if (item_current == 2)
					m_Cells[m_iCellListCount]->Set_CellOption(CCell::CELL_JUMPZONE);
				else if (item_current == 3)
					m_Cells[m_iCellListCount]->Set_CellOption(CCell::CELL_BLOCKZONE);
			}
		}

		Make_VerticalSpacing(10);

		if (ImGui::TreeNode("Navigation Save & Load"))
		{
			if (ImGui::Button("Save Navigation", ImVec2(-FLT_MIN, 30.f)))
				ImGui::OpenPopup("Save Navigation");

			if (ImGui::Button("Load Navigation", ImVec2(-FLT_MIN, 30.f)))
				ImGui::OpenPopup("Load Navigation");

			ImVec2 center = ImGui::GetMainViewport()->GetCenter();
			ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

			if (ImGui::BeginPopupModal("Save Navigation", NULL, ImGuiWindowFlags_AlwaysAutoResize))
			{
				if (m_FilePathList.size() == 0)
				{
					m_FilePathList.clear();
					_tfinddata64_t fd;
					__int64 handle = _tfindfirst64(TEXT("../bin/Resources/Data/NaviMesh/*.*"), &fd);
					if (handle == -1 || handle == 0)
						return E_FAIL;

					_int iResult = 0;

					char szFilename[MAX_PATH];

					while (iResult != -1)
					{
						if (!lstrcmp(fd.name, L".") || !lstrcmp(fd.name, L".."))
						{
							iResult = _tfindnext64(handle, &fd);
							continue;
						}

						WideCharToMultiByte(CP_UTF8, 0, fd.name, -1, szFilename, sizeof(szFilename), NULL, NULL);
						m_FilePathList.push_back({ szFilename });

						iResult = _tfindnext64(handle, &fd);
					}

					_findclose(handle);
				}

				ImGui::Text("Save Navigation!\n\nExist NaviGationDataFiles");

				static ImGuiTextFilter filter;

				char	szCheckforSameFileName[256] = "";

				if (ImGui::BeginListBox(" "))
				{
					auto iter = m_FilePathList.begin();


					for (; iter != m_FilePathList.end(); iter++)
					{
						const bool is_selected = false;

						if (filter.PassFilter(iter->c_str()))
						{
							if (ImGui::Selectable(iter->c_str(), is_selected))
							{
								strcpy_s(filter.InputBuf, iter->c_str());
							}

							if (!strcmp(iter->c_str(), filter.InputBuf))
								strcpy_s(szCheckforSameFileName, filter.InputBuf);
						}
					}
					ImGui::EndListBox();
				}

				filter.Draw("Input FileName");

				ImGui::Separator();
				if (ImGui::Button("Save", ImVec2(120, 0)))
				{

					if (strcmp(filter.InputBuf, ""))
					{

						if (!strcmp(szCheckforSameFileName, filter.InputBuf))
						{
							ImGui::OpenPopup("Navi One More Check");
						}
						else
						{
							//실제 저장

							Sava_Data(filter.InputBuf, Data_Navigation);

							ImGui::CloseCurrentPopup();
							m_FilePathList.clear();
						}
					}



				}
				ImGui::SetItemDefaultFocus();
				ImGui::SameLine();
				if (ImGui::Button("Cancel", ImVec2(120, 0)))
				{
					ImGui::CloseCurrentPopup();
					m_FilePathList.clear();
				}


				//서브 팝업
				ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
				if (ImGui::BeginPopupModal("Navi One More Check", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{

					ImGui::Text("NaviGationData Already Exist\nDo you want to Override on it?");

					if (ImGui::Button("Ok", ImVec2(130, 0)))
					{

						//실제 저장
						Sava_Data(filter.InputBuf, Data_Navigation);

						ImGui::CloseCurrentPopup();
						m_FilePathList.clear();
					}

					ImGui::SameLine();
					if (ImGui::Button("Cancel", ImVec2(130, 0)))
					{
						ImGui::CloseCurrentPopup();
					}
					ImGui::EndPopup();
				}

				ImGui::EndPopup();
			}

			if (ImGui::BeginPopupModal("Load Navigation", NULL, ImGuiWindowFlags_AlwaysAutoResize))
			{
				ImGui::Text("Laod Navigation!\n\n");
				ImGui::Separator();


				if (m_FilePathList.size() == 0)
				{
					m_FilePathList.clear();
					_tfinddata64_t fd;
					__int64 handle = _tfindfirst64(TEXT("../bin/Resources/Data/NaviMesh/*.*"), &fd);
					if (handle == -1 || handle == 0)
						return E_FAIL;

					_int iResult = 0;

					char szFilename[MAX_PATH];

					while (iResult != -1)
					{
						if (!lstrcmp(fd.name, L".") || !lstrcmp(fd.name, L".."))
						{
							iResult = _tfindnext64(handle, &fd);
							continue;
						}


						WideCharToMultiByte(CP_UTF8, 0, fd.name, -1, szFilename, sizeof(szFilename), NULL, NULL);
						//strcpy_s(szFullPath, szCurPath);
						//strcat_s(szFullPath, szFilename);
						m_FilePathList.push_back({ szFilename });


						iResult = _tfindnext64(handle, &fd);
					}
					_findclose(handle);
				}

				static ImGuiTextFilter filter;

				char	szCheckforSameFileName[256] = "";

				if (ImGui::BeginListBox(" "))
				{
					auto iter = m_FilePathList.begin();


					for (; iter != m_FilePathList.end(); iter++)
					{
						const bool is_selected = false;

						if (filter.PassFilter(iter->c_str()))
						{
							if (ImGui::Selectable(iter->c_str(), is_selected))
							{
								strcpy_s(filter.InputBuf, iter->c_str());
							}

							if (!strcmp(iter->c_str(), filter.InputBuf))
								strcpy_s(szCheckforSameFileName, filter.InputBuf);
						}
					}
					ImGui::EndListBox();

				}

				filter.Draw("Input FileName");




				if (ImGui::Button("OK", ImVec2(120, 0)))
				{

					if (strcmp(filter.InputBuf, ""))
					{
						if (!strcmp(szCheckforSameFileName, filter.InputBuf))
						{
							Load_Data(filter.InputBuf, Data_Navigation);
							m_FilePathList.clear();
							ImGui::CloseCurrentPopup();
						}
					}
				}
				ImGui::SetItemDefaultFocus();
				ImGui::SameLine();
				if (ImGui::Button("Cancel", ImVec2(120, 0))) {
					m_FilePathList.clear();
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
			ImGui::TreePop();
		}

		Make_VerticalSpacing(10);

		if (ImGui::TreeNode("Save & Delete HeightMap"))
		{


			if (ImGui::Button("Save HeightMap", ImVec2(-FLT_MIN, 30.f)))
				ImGui::OpenPopup("Save HeightMap");

			if (ImGui::Button("Delete HeightMap", ImVec2(-FLT_MIN, 30.f)))
				ImGui::OpenPopup("Delete HeightMap");


			ImVec2 center = ImGui::GetMainViewport()->GetCenter();
			ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

			if (ImGui::BeginPopupModal("Save HeightMap", NULL, ImGuiWindowFlags_AlwaysAutoResize))
			{

				if (m_FilePathList.size() == 0)
				{
					m_FilePathList.clear();
					_tfinddata64_t fd;
					__int64 handle = _tfindfirst64(TEXT("../bin/Resources/Textures/HeightMap/*.*"), &fd);
					if (handle == -1 || handle == 0)
						return E_FAIL;

					_int iResult = 0;

					//char szCurPath[128] = "../bin/Resources/Data/Map/";
					//char szFullPath[128] = "";
					char szFilename[MAX_PATH];

					while (iResult != -1)
					{
						if (!lstrcmp(fd.name, L".") || !lstrcmp(fd.name, L".."))
						{
							iResult = _tfindnext64(handle, &fd);
							continue;
						}


						WideCharToMultiByte(CP_UTF8, 0, fd.name, -1, szFilename, sizeof(szFilename), NULL, NULL);
						//strcpy_s(szFullPath, szCurPath);
						//strcat_s(szFullPath, szFilename);
						m_FilePathList.push_back({ szFilename });


						iResult = _tfindnext64(handle, &fd);
					}


					_findclose(handle);

				}

				ImGui::Text("Save Map!\n\nExist MapDataFiles");

				static ImGuiTextFilter filter;

				char	szCheckforSameFileName[256] = "";

				if (ImGui::BeginListBox(" "))
				{
					auto iter = m_FilePathList.begin();


					for (; iter != m_FilePathList.end(); iter++)
					{
						const bool is_selected = false;

						if (filter.PassFilter(iter->c_str()))
						{
							if (ImGui::Selectable(iter->c_str(), is_selected))
							{
								strcpy_s(filter.InputBuf, iter->c_str());
							}

							if (!strcmp(iter->c_str(), filter.InputBuf))
								strcpy_s(szCheckforSameFileName, filter.InputBuf);
						}
					}
					ImGui::EndListBox();

				}

				filter.Draw("Input FileName");


				ImGui::Separator();
				if (ImGui::Button("Save", ImVec2(120, 0)))
				{

					if (strcmp(filter.InputBuf, ""))
					{

						if (!strcmp(szCheckforSameFileName, filter.InputBuf))
						{
							ImGui::OpenPopup("One More Check");
						}
						else
						{
							//실제 저장

							Sava_Data(filter.InputBuf, Data_HeightMap);

							ImGui::CloseCurrentPopup();
							m_FilePathList.clear();


						}
					}



				}
				ImGui::SetItemDefaultFocus();
				ImGui::SameLine();
				if (ImGui::Button("Cancel", ImVec2(120, 0)))
				{
					ImGui::CloseCurrentPopup();
					m_FilePathList.clear();
				}


				//서브 팝업
				ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
				if (ImGui::BeginPopupModal("One More Check", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{

					ImGui::Text("Mapdata Already Exist\nDo you want to Override on it?");

					if (ImGui::Button("Ok", ImVec2(130, 0)))
					{

						//실제 저장
						Sava_Data(filter.InputBuf, Data_HeightMap);

						ImGui::CloseCurrentPopup();
						m_FilePathList.clear();
					}

					ImGui::SameLine();
					if (ImGui::Button("Cancel", ImVec2(130, 0)))
					{
						ImGui::CloseCurrentPopup();
					}
					ImGui::EndPopup();
				}


				ImGui::EndPopup();
			}

			ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

			if (ImGui::BeginPopupModal("Delete HeightMap", NULL, ImGuiWindowFlags_AlwaysAutoResize))
			{
				ImGui::Text("!!!!!!!!!!!!!!!!Waring!!!!!!!!!!!!!!!!\n\n Delete Batched Filter Without Save!!!\n\n	Please Check Save One more\n\n\n");
				ImGui::Separator();

				//static int unused_i = 0;
				//ImGui::Combo("Combo", &unused_i, "Delete\0Delete harder\0");

				//static bool dont_ask_me_next_time = false;
				//ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
				//ImGui::Checkbox("Don't ask me next time", &dont_ask_me_next_time);
				//ImGui::PopStyleVar();

				if (ImGui::Button("OK!", ImVec2(130, 0)))
				{
					Safe_Release(m_pCreatedTerrain);
					ImGui::CloseCurrentPopup();
				}


				ImGui::SetItemDefaultFocus();
				ImGui::SameLine();
				if (ImGui::Button("Cancel!", ImVec2(130, 0))) { ImGui::CloseCurrentPopup(); }
				ImGui::EndPopup();
			}



			ImGui::TreePop();
		}
	}

	return S_OK;
}

void CScene_Edit::Add_Vertex()
{
	if (g_pGameInstance->Get_DIMouseButtonState(CInput_Device::MBS_RBUTTON) & DIS_Down)
	{
		for (auto& Point : m_Points)
		{
			CCollisionMgr::EDITPOINTCOLLIDER TypeColl;

			TypeColl.vCollider = Point->Get_Collider();
			TypeColl.GameObject = Point;
			g_pGameInstance->Add_NaviPointCollider(TypeColl);
		}

		POINT ptMouse;
		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);

		_Vector vCursorPos = XMVectorSet(
			(_float(ptMouse.x) / (g_iWinCX * 0.5f)) - 1.f,
			(_float(ptMouse.y) / -(g_iWinCY * 0.5f)) + 1.f,
			0, 1.f);

		_Matrix InvProjMat = XMMatrixInverse(nullptr, g_pGameInstance->Get_Transform_Matrix(PLM_PROJ));

		_Vector vRayDir = XMVector4Transform(vCursorPos, InvProjMat) - XMVectorSet(0, 0, 0, 1);
		_float3 m_vRayPos = _float3(0.f, 0.f, 0.f);

		_Matrix InvViewMat = XMMatrixInverse(nullptr, g_pGameInstance->Get_Transform_Matrix(PLM_VIEW));
		vRayDir = XMVector3TransformNormal(vRayDir, InvViewMat);
		XMStoreFloat3(&m_vRayPos, XMVector3TransformCoord(XMLoadFloat3(&m_vRayPos), InvViewMat));

		CGameObject* Obj = g_pGameInstance->NaviPointCollision(XMLoadFloat3(&m_vRayPos), vRayDir);

		if (nullptr == Obj)
		{
			POINT ptMouse;
			GetCursorPos(&ptMouse);
			ScreenToClient(g_hWnd, &ptMouse);



			_Vector vCursorPos = XMVectorSet(
				(_float(ptMouse.x) / (g_iWinCX * 0.5f)) - 1.f,
				(_float(ptMouse.y) / -(g_iWinCY * 0.5f)) + 1.f,
				0, 1.f);

			_Matrix InvProjMat = XMMatrixInverse(nullptr, g_pGameInstance->Get_Transform_Matrix(PLM_PROJ));

			_Vector vRayDir = XMVector4Transform(vCursorPos, InvProjMat) - XMVectorSet(0, 0, 0, 1);

			_Matrix InvViewMat = XMMatrixInverse(nullptr, g_pGameInstance->Get_Transform_Matrix(PLM_VIEW));
			vRayDir = XMVector3TransformNormal(vRayDir, InvViewMat);


			if (m_pCreatedTerrain)
			{
				_Vector vCamPos = m_pEditorCam->Get_Camera_Transform()->Get_MatrixState(CTransform::STATE_POS);

				_Vector vOldPos = vCamPos;
				_Vector vNewPos;
				_float3 vResult;
				_bool IsPicked = false;


				for (_uint i = 0; i < 200; i++)
				{
					vNewPos = vOldPos + vRayDir;

					vResult = m_pCreatedTerrain->Pick_OnTerrain(&IsPicked, vNewPos, vOldPos);

					if (IsPicked)
					{
						Ready_Layer_NaviPoint(vResult);
						break;
					}

					vOldPos = vNewPos;
				}
			}

			++m_iVertexCount;
			char sIntStr[5];
			_itoa_s(m_iVertexCount, sIntStr, 10);
			char str[10] = "Vertex";
			char* sVertexNum = NEW char[10];
			strcat_s(str, sIntStr);
			strcpy_s(sVertexNum, 10, str);

			m_vVertexs.push_back(sVertexNum);
		}
		else
		{
			if (m_iVertexIndex < 3)
			{
				CTransform* ObjTransform = (CTransform*)Obj->Get_Component(TAG_COM(Com_Transform));
				XMStoreFloat3(&m_vVertexts[m_iVertexIndex], ObjTransform->Get_MatrixState(CTransform::STATE_POS));
				++m_iVertexIndex;
			}
		}
	}
}

_int CScene_Edit::CCWClockSort()
{
	//x1 * y2 + x2 * y3 + x3 * y1
	_int a =_int( m_vVertexts[0].x * m_vVertexts[1].z + m_vVertexts[1].x * m_vVertexts[2].z + m_vVertexts[2].x * m_vVertexts[0].z);
	//y1 * x2 + y2 * x3 + y3 * x1
	_int b = _int( m_vVertexts[0].z * m_vVertexts[1].x + m_vVertexts[1].z * m_vVertexts[2].x + m_vVertexts[2].z * m_vVertexts[0].x);


	return a - b;
}

HRESULT CScene_Edit::Ready_Layer_NaviPoint(_float3 Pos)
{
	CNaviPoint* pNaviPoint = nullptr;
	FAILED_CHECK(m_pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&pNaviPoint), SCENE_EDIT, TAG_OP(Prototype_NaviPoint)));
	NULL_CHECK_RETURN(pNaviPoint, E_FAIL);
	CTransform* Transform =  (CTransform*)pNaviPoint->Get_Component(TAG_COM(Com_Transform));

	Transform->Set_MatrixState(CTransform::STATE_POS, Pos);

	if (m_iVertexIndex < 3)
	{
		XMStoreFloat3(&m_vVertexts[m_iVertexIndex], Transform->Get_MatrixState(CTransform::STATE_POS));
		++m_iVertexIndex;
	}

	m_Points.push_back(pNaviPoint);

	return S_OK;
}

HRESULT CScene_Edit::Ready_Layer_LoadNaviPoint(_float3 Pos)
{
	CNaviPoint* pNaviPoint = nullptr;
	FAILED_CHECK(m_pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&pNaviPoint), SCENE_EDIT, TAG_OP(Prototype_NaviPoint)));
	NULL_CHECK_RETURN(pNaviPoint, E_FAIL);
	CTransform* Transform = (CTransform*)pNaviPoint->Get_Component(TAG_COM(Com_Transform));

	Transform->Set_MatrixState(CTransform::STATE_POS, Pos);
	m_Points.push_back(pNaviPoint);
	return S_OK;
}

#pragma endregion CamTab



#endif // USE_IMGUI
HRESULT CScene_Edit::Ready_Layer_MainCamera(const _tchar * pLayerTag)
{

	CCamera::CAMERADESC CameraDesc;
	CameraDesc.vWorldRotAxis = _float3(0, 0, 0);
	CameraDesc.vEye = _float3(0, 5.f, -10.f);
	CameraDesc.vAt = _float3(0, 0.f, 0);
	CameraDesc.vAxisY = _float3(0, 1, 0);

	CameraDesc.fFovy = XMConvertToRadians(60.f);
	CameraDesc.fAspect = _float(g_iWinCX) / g_iWinCY;
	CameraDesc.fNear = 0.2f;
	CameraDesc.fFar = CAMERAFAR;

	CameraDesc.iWinCX = g_iWinCX;
	CameraDesc.iWinCY = g_iWinCY;

	CameraDesc.TransformDesc.fMovePerSec = 5.f;
	CameraDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(4.0f);
	CameraDesc.TransformDesc.fScalingPerSec = 1.f;


	CGameInstance* pInstance = GetSingle(CGameInstance);

	list<CGameObject*>* MainCameraList = pInstance->Get_ObjectList_from_Layer(SCENE_STATIC, TAG_LAY(Layer_Camera_Main));

	if (MainCameraList != nullptr)
	{
		auto iter = MainCameraList->begin();
		for (; iter != MainCameraList->end();)
		{
			Safe_Release(*(iter));
			iter = MainCameraList->erase(iter);
		}
		MainCameraList->clear();
	}

	m_pEditorCam = (CCamera_Editor*)(pInstance->Get_GameObject_By_LayerIndex(SCENE_EDIT, TAG_LAY(Layer_Camera_Main)));

	if (m_pEditorCam == nullptr)
	{
		FAILED_CHECK(pInstance->Add_GameObject_To_Layer(SCENEID::SCENE_EDIT, pLayerTag, TAG_OP(Prototype_Camera_Editor), &CameraDesc));

		m_pEditorCam = (CCamera_Editor*)(pInstance->Get_GameObject_By_LayerIndex(SCENE_EDIT, TAG_LAY(Layer_Camera_Main)));

		NULL_CHECK_RETURN(m_pEditorCam, E_FAIL);

		// 초기 새팅
		//m_pEditorCam

		//
	}
	else
	{
		//리셋

		m_pEditorCam->Set_NowSceneNum(SCENE_EDIT);

	}



	return S_OK;
}

HRESULT CScene_Edit::Ready_Layer_Player(const _tchar * pLayerTag)
{

	CGameInstance* pInstance = GetSingle(CGameInstance);

	list<CGameObject*>* PlayerList= pInstance->Get_ObjectList_from_Layer(SCENE_STATIC, TAG_LAY(Layer_Player));

	if (PlayerList != nullptr)
	{
		auto iter = PlayerList->begin();
		for (; iter != PlayerList->end();)
		{
			Safe_Release(*(iter));
			iter = PlayerList->erase(iter);
		}
		PlayerList->clear();
	}

	//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENE_EDIT, pLayerTag, TAG_OP(Prototype_SkyBox)));


	return S_OK;
}
#ifdef _DEBUG



HRESULT CScene_Edit::Ready_CamActionCursor(const _tchar * pLayerTag)
{

	FAILED_CHECK(m_pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&m_pCamCursor), SCENE_EDIT, TAG_OP(Prototype_EditorCursor)));
	NULL_CHECK_RETURN(m_pCamCursor, E_FAIL);
	m_pCamCursor->Set_Color({1, 0, 0, 1});

	CamDesc.fDuration = 1.f;

	return S_OK;
}

HRESULT CScene_Edit::Ready_ParticleDesc()
{

	m_tParticleDesc.eParticleTypeID = InstanceEffect_Spread;
	m_tParticleDesc.eInstanceCount = Prototype_VIBuffer_Point_Instance_128;
	m_tParticleDesc.ePassID = InstancePass_MaskingNoising;

	m_tParticleDesc.bBillboard = false;

	//둘중 하나만 써야함	
	m_tParticleDesc.vFixedPosition = _float3(0);
	m_tParticleDesc.vPowerDirection = _float3(0, 1, 1);
	//팔로잉 타겟을 쓰면 해당 타겟의 룩을 기준으로 파워 디렉션이 잡힘
	m_tParticleDesc.FollowingTarget = nullptr;
	m_tMeshDesc.iFollowingDir = FollowingDir_Right;

	lstrcpy(m_tParticleDesc.szTextureProtoTypeTag, TAG_CP(Prototype_Texture_TestEffect));
	lstrcpy(m_tParticleDesc.szTextureLayerTag, L"dds");

	m_tParticleDesc.iTextureLayerIndex = 0;

	m_tParticleDesc.iNoiseTextureIndex = 263;
	m_tParticleDesc.iMaskingTextureIndex = 0;

	m_tParticleDesc.TextureChageFrequency = 0;
	//텍스쳐 안에 그림의 가로 세로 개수
	m_tParticleDesc.vTextureXYNum = _float2(1, 1);
	//텍스쳐 안에 형태가 몇개 있는지 -1이면 간격 기준으로 꽉차있는거
	//m_tParticleDesc.iFigureCount_In_Texture = -1;
	m_tParticleDesc.iFigureCount_In_Texture = -1;


	m_tParticleDesc.TotalParticleTime = 3;
	m_tParticleDesc.EachParticleLifeTime = 0.35f;

	m_tParticleDesc.SizeChageFrequency = 0;
	m_tParticleDesc.ParticleSize = _float3(1.f, 1.f, 1.f);
	m_tParticleDesc.ParticleSize2 = _float3(0.1f);


	m_tParticleDesc.ColorChageFrequency = 0;
	m_tParticleDesc.TargetColor = _float4(1.f, 1.f, 1.f, 1.f);
	m_tParticleDesc.TargetColor2 = _float4(1.f, 1.f, 1.f, 1.f);

	m_tParticleDesc.fMaxBoundaryRadius = 999.f;

	m_tParticleDesc.Particle_Power = 10.f;
	m_tParticleDesc.PowerRandomRange = _float2(0.8f, 1.2f);
	//콘 형태나 파운틴 형태일때 라업룩 파워 조절 용
	m_tParticleDesc.SubPowerRandomRange_RUL = _float3(1.f, 1.f, 1.f);

	m_tParticleDesc.ParticleStartRandomPosMin = _float3(0);
	m_tParticleDesc.ParticleStartRandomPosMax = _float3(0);

	m_tParticleDesc.AlphaBlendON = true;
	m_tParticleDesc.bEmissive = false;
	m_tParticleDesc.vEmissive_SBB = _float3(0);

	m_tParticleDesc.m_fAlphaTestValue = 0.1f;











	m_tMeshDesc.eParticleTypeID = InstanceEffect_Cone;
	m_tMeshDesc.eInstanceCount = Prototype_ModelInstance_1;
	m_tMeshDesc.ePassID = MeshPass_MaskingNoising;

	m_tMeshDesc.vFixedPosition = _float3(0);
	m_tMeshDesc.vPowerDirection = _float3(0, 1, 0);

	m_tMeshDesc.FollowingTarget = nullptr;
	m_tMeshDesc.iFollowingDir = FollowingDir_Up;

	lstrcpy(m_tMeshDesc.szModelMeshProtoTypeTag, TAG_CP(Prototype_Mesh_AlgaeRock_Ledge));

	m_tMeshDesc.iNoiseTextureIndex = 263;
	m_tMeshDesc.iMaskingTextureIndex = 0;
	m_tMeshDesc.vNoisePushingDir = _float2(0, 1);



	m_tMeshDesc.TotalParticleTime = 5.f;
	m_tMeshDesc.EachParticleLifeTime = 1.f;

	m_tMeshDesc.SizeChageFrequency = 0;
	m_tMeshDesc.ParticleSize = _float3(1.f);
	m_tMeshDesc.ParticleSize2 = _float3(0, 0, 0);
	m_tMeshDesc.ColorChageFrequency = 5;
	m_tMeshDesc.TargetColor = _float4(1.f, 1.f, 1.f, 1.f);
	m_tMeshDesc.TargetColor2 = _float4(0.f, 1.f, .0f, .2f);
	m_tMeshDesc.fMaxBoundaryRadius = 999999.f;
	m_tMeshDesc.Particle_Power = 1.f;
	m_tMeshDesc.PowerRandomRange = _float2(0.5f, 1.5f);
	m_tMeshDesc.SubPowerRandomRange_RUL = _float3(1.f, 1.f, 1.f);
	m_tMeshDesc.ParticleStartRandomPosMin = _float3(0, 0, 0);
	m_tMeshDesc.ParticleStartRandomPosMax = _float3(0, 0, 0);

	m_tMeshDesc.bEmissive = false;
	m_tMeshDesc.bAutoTurn = false;
	m_tMeshDesc.bIsOclusion = true;

	m_tMeshDesc.vEmissive_SBB = _float3(0);
	//m_tMeshDesc.m_fAlphaTestValue = 0.1f;


	return S_OK;
}


#endif // USE_IMGUI



CScene_Edit * CScene_Edit::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CScene_Edit* pTempGame = NEW CScene_Edit(pDevice, pDeviceContext);

	if (FAILED(pTempGame->Initialize()))
	{
		Safe_Release(pTempGame);
		MSGBOX("Failed to Creating Scene_Stage1");
		return nullptr;
	}

	return pTempGame;
}

void CScene_Edit::Free()
{
	__super::Free();

#ifdef _DEBUG
	for (auto& iter : m_vecBatchedObject)
	{
		//객채 없애기 죽이기 비워주기
		Safe_Release(iter.pObject);
	}
	m_vecBatchedObject.clear();

	for (auto& EditedUI : m_vecBatchedUI)
	{
		Safe_Release(EditedUI.SRV);
	}
	
	m_vecBatchedObject.clear();


	Safe_Release(m_pCamCursor);
	for (auto& iter : m_vecCamPosBatchedObj)
		Safe_Release(iter);
	m_vecCamPosBatchedObj.clear();
	for (auto& iter : m_vecLookBatchedObj)
		Safe_Release(iter);
	m_vecLookBatchedObj.clear();

	for (auto& Point : m_Points)
		Point->Free();
	m_Points.clear();

	for (auto& Cell : m_Cells)
		Cell->Free();
	m_Cells.clear();

	for (auto& Vertex : m_vVertexs)
		Safe_Delete(Vertex);
	m_vVertexs.clear();

	for (auto& CellName : m_vCellNames)
		Safe_Delete(CellName);
	m_vCellNames.clear();


	for (auto& pTrigger : m_vecTriggerObject)
		Safe_Release(pTrigger);
	m_vecTriggerObject.clear();

	Safe_Release(m_pCreatedTerrain);
	Safe_Release(m_pGameInstance);
	Safe_Release(m_TargetSRV);
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pShader);
	//Safe_Release(m_pRendererEditUI);
#endif
}
