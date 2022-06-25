#include "stdafx.h"
#include "..\Public\Scene_Edit.h"
#include "Scene_Loading.h"
#include "EditorTerrain.h"
#include "Camera_Editor.h"
#include "ESCursor.h"
#include "MapObject.h"

CScene_Edit::CScene_Edit(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CScene(pDevice, pDeviceContext)
{
}

HRESULT CScene_Edit::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

#ifdef USE_IMGUI
	m_pGameInstance = GetSingle(CGameInstance);
	Safe_AddRef(m_pGameInstance);

	FAILED_CHECK(Ready_Layer_Player(TAG_LAY(Layer_Player)));
	FAILED_CHECK(Ready_Layer_MainCamera(TAG_LAY(Layer_Camera_Main)));
	FAILED_CHECK(Ready_Layer_RendererEditUI(TAG_LAY(Layer_UI_IMG)));
	FAILED_CHECK(Ready_CamActionCursor(L"CamCursor"));

	


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







#endif // USE_IMGUI
	return S_OK;
}

_int CScene_Edit::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)
		return -1;

	if (m_bIsNeedToSceneChange)
		return Change_to_NextScene();

#ifdef USE_IMGUI


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


#ifdef USE_IMGUI


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
	SetWindowText(g_hWnd, TEXT("MapEditScene"));
#endif // _DEBUG


	return 0;
}

_int CScene_Edit::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;

#ifdef USE_IMGUI
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

#ifdef USE_IMGUI

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
			//FAILED_CHECK(Update_ParticleTab(fDeltatime));
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
			return E_FAIL;

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
	case Client::CScene_Edit::Data_Particle:
	{

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
			return E_FAIL;

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
				((CMapObject*)tData.pObject)->Set_PassIndex(tData.PassIndex);
			}

			//트렌스폼
			CTransform* pTrans = (CTransform*)(tData.pObject->Get_Component(TAG_COM(Com_Transform)));
			NULL_CHECK_RETURN(pTrans, E_FAIL);
			pTrans->Set_Matrix(tData.matTransform);



			m_vecBatchedObject.push_back(tData);
		}


		m_iBatchedVecIndex = 0;
		m_SelectedObjectSRT = &(m_vecBatchedObject[m_iBatchedVecIndex].matSRT);
		m_fFrustumRange = (m_vecBatchedObject[m_iBatchedVecIndex].FrustumRange);
		m_iPassIndex = (m_vecBatchedObject[m_iBatchedVecIndex].PassIndex);

		CloseHandle(hFile);
	

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
	case Client::CScene_Edit::Data_Particle:
	{

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
	case Client::CScene_Edit::Data_FilterMap:
	{



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

	FAILED_CHECK(Widget_CreateDeleteHeightMap(fDeltatime));
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
			m_fFrustumRange = m_vecBatchedObject[m_iBatchedVecIndex].FrustumRange;
			m_iPassIndex = m_vecBatchedObject[m_iBatchedVecIndex].PassIndex;


			m_iBatchedVecIndex = _uint(m_vecBatchedObject.size() - 1);
			m_SelectedObjectSRT = &(m_vecBatchedObject[m_iBatchedVecIndex].matSRT);
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
				ImGui::BeginChild("ChildL", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, 130), true, window_flags);


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

				ImGui::BeginChild("ChildR", ImVec2(0, 130), true, window_flags);


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
							m_fFrustumRange = m_vecBatchedObject[m_iBatchedVecIndex].FrustumRange;
							m_iPassIndex = m_vecBatchedObject[m_iBatchedVecIndex].PassIndex;
							ZeroMemory(m_iSelectedObjectNMesh, sizeof(_uint) * 2);
							m_iSelectedObjectNMesh[0] = Prototype_StaticMapObject;
							m_iSelectedObjectNMesh[1] = Prototype_Mesh_None;

						}
					}
					ImGui::EndTable();
				}

				Make_VerticalSpacing(1);

				ImGui::EndChild();
				Make_VerticalSpacing(1);
			}
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
			}

			m_vecBatchedObject.push_back(ObjElement);

			m_iBatchedVecIndex = _uint(m_vecBatchedObject.size() - 1);
			m_SelectedObjectSRT = &(m_vecBatchedObject[m_iBatchedVecIndex].matSRT);
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
	pObjElement->PassIndex = m_iPassIndex;

	if (pObjElement->pObject != nullptr)
	{
		CTransform* pTrans = (CTransform*)(pObjElement->pObject->Get_Component(TAG_COM(Com_Transform)));
		pTrans->Set_Matrix(pObjElement->matTransform);

		if (m_iBatchedVecIndex)
		{
			((CMapObject*)pObjElement->pObject)->Set_FrustumSize(m_fFrustumRange);
			((CMapObject*)pObjElement->pObject)->Set_PassIndex(m_iPassIndex);
		}
	}

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
	else {

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
	CameraDesc.fFar = 300.f;

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

	return S_OK;
}
#ifdef USE_IMGUI



HRESULT CScene_Edit::Ready_CamActionCursor(const _tchar * pLayerTag)
{

	FAILED_CHECK(m_pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&m_pCamCursor), SCENE_EDIT, TAG_OP(Prototype_EditorCursor)));
	NULL_CHECK_RETURN(m_pCamCursor, E_FAIL);
	m_pCamCursor->Set_Color({1, 0, 0, 1});

	CamDesc.fDuration = 1.f;

	return S_OK;
}


#endif // USE_IMGUI



CScene_Edit * CScene_Edit::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CScene_Edit* pTempGame = new CScene_Edit(pDevice, pDeviceContext);

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

#ifdef USE_IMGUI
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



	Safe_Release(m_pCreatedTerrain);
	Safe_Release(m_pGameInstance);
	Safe_Release(m_TargetSRV);
	//Safe_Release(m_pRendererEditUI);
#endif
}
