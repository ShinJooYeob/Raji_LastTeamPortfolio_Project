#include "stdafx.h"
#include "..\public\ImguiMgr.h"
#include "Scene_Loading.h"
#include "GameObject.h"

#ifdef USE_IMGUI


IMPLEMENT_SINGLETON(CImguiMgr)

CImguiMgr::CImguiMgr()
{
}



HRESULT CImguiMgr::Initialize_ImguiMgr(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, ID3D11RenderTargetView* pBackBufferRTV, ID3D11DepthStencilView* pDepthStencilView, IDXGISwapChain* pSwapChain)
{

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	m_pDevice = pDevice;
	m_pDeviceContext = pDeviceContext;
	m_pBackBufferRTV = pBackBufferRTV;
	m_pDepthStencilView = pDepthStencilView;
	m_pSwapChain = pSwapChain;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
	Safe_AddRef(m_pBackBufferRTV);
	Safe_AddRef(m_pDepthStencilView);
	Safe_AddRef(m_pSwapChain);

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(g_hWnd);
	ImGui_ImplDX11_Init(m_pDevice, m_pDeviceContext);


	//// Our state
	//show_demo_window = true;
	//show_another_window = false;
	//clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


	return S_OK;
}

_int CImguiMgr::Update_ImguiMgr(_double fDeltatime)
{
	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	////////1. Show the big demo window(Most of the sample code is in ImGui::ShowDemoWindow()!You can browse its code to learn more about Dear ImGui!).
	//if (show_demo_window)
	//	ImGui::ShowDemoWindow(&show_demo_window);

	//// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	//{
	//	static float f = 0.0f;
	//	static int counter = 0;

	//	ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

	//	ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
	//	ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
	//	ImGui::Checkbox("Another Window", &show_another_window);

	//	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	//	ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

	//	if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
	//		counter++;
	//	ImGui::SameLine();
	//	ImGui::Text("counter = %d", counter);

	//	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	//	ImGui::End();
	//}

	//// 3. Show another simple window.
	//if (show_another_window)
	//{
	//	ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
	//	ImGui::Text("Hello from another window!");
	//	if (ImGui::Button("Close Me"))
	//		show_another_window = false;
	//	ImGui::End();
	//}


	return _int();
}

_int CImguiMgr::Begin_Update_Frame(_double fDeltatime, const char * szManuBarName,_uint iFlag)
{
	_bool _bOpen = true;
	ImGui::Begin(szManuBarName,&_bOpen, iFlag);

	return _int();
}

_int CImguiMgr::End_Update_Frame()
{
	ImGui::End();
	return _int();
}

HRESULT CImguiMgr::Render_ImguiMgr()
{

	// Rendering
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());


	return S_OK;
}

_int CImguiMgr::Update_DebugWnd(_double fDeltaTime)
{
	CGameInstance* pGameInstance = g_pGameInstance;

	if(g_pGameInstance->Get_DIKeyState(DIK_F1) & DIS_Down) m_bOnDebugWnd = !m_bOnDebugWnd;
	if (!m_bOnDebugWnd) return S_FALSE;

	if (g_pGameInstance->Get_NowSceneNum() != SCENE_EDIT)
		Update_ImguiMgr(fDeltaTime);
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
		ImGui::SetNextWindowSize(ImVec2(center.x * 0.667f, center.y), ImGuiCond_None);
		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_None, ImVec2(0.0f, 0.0f));
		bFirstSetting = true;
	}


	Begin_Update_Frame(fDeltaTime, "DebugMode", window_flags);
	int open_action = -1;

	{
		++m_dwNumRender;
		m_dTimerAcc += g_fDeltaTime;
		static int Frame = 0;
		if (m_dTimerAcc >= 1.f)
		{
			Frame = m_dwNumRender;
			m_dwNumRender = 0;
			m_dTimerAcc = 0.f;
		}
		ImGui::Text("FPS : %d ", Frame); ImGui::SameLine(100);
		ImGui::TableNextColumn(); ImGui::Checkbox("BG X", &bArrWindowFlag[0]); ImGui::SameLine();
		ImGui::TableNextColumn(); ImGui::Checkbox("Block Move", &bArrWindowFlag[1]); ImGui::SameLine(0.f ,50.f);
		if (ImGui::Button("Close All Tree"))	{	open_action = 0;	}
	}
	ImGui::Separator();

	if (open_action != -1)	ImGui::SetNextItemOpen(open_action != 0);
	if (ImGui::TreeNode("Scene Change"))
	{
		static int SelectedSceneIndex = 0;

		if (ImGui::BeginListBox("", ImVec2(150.f, (SCENE_END-3.f) *17.5f)))
		{
			for (int n = 0; n < SCENE_END; n++)
			{ 
				const bool is_selected = (SelectedSceneIndex == n);
				if(pGameInstance->Get_NowSceneNum() == n || n == SCENE_LOADING || n == SCENE_STATIC)continue;
				if (ImGui::Selectable(TAG_SCENE((SCENEID)n), is_selected))
				{
					SelectedSceneIndex = n;
				}

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
				{
					ImGui::SetItemDefaultFocus(); 
				}
			}
			ImGui::EndListBox();

			ImGui::SameLine(200,10);

			if (pGameInstance->Get_NowSceneNum() == SCENE_LOADING)	ImGui::BeginDisabled(true);

			if (ImGui::Button("Go Scene"))
			{
				ImGui::TreePop();
				FAILED_CHECK(GetSingle(CUtilityMgr)->Clear_RenderGroup_forSceneChange());
				pGameInstance->Scene_Change(CScene_Loading::Create(m_pDevice, m_pDeviceContext, (SCENEID)SelectedSceneIndex), SCENEID::SCENE_LOADING);
				End_Update_Frame();
				Render_DebugWnd();
				return 0;
			}			

			if (pGameInstance->Get_NowSceneNum() == SCENE_LOADING)	ImGui::EndDisabled();

		}

		ImGui::TreePop();
	}
	ImGui::Separator();
	static int open_ObjectController = -1;

	if (open_action != -1)	ImGui::SetNextItemOpen(open_action != 0);

	if (ImGui::TreeNode("Object Controller"))
	{
		_bool IsAllClosed = false;

		if (open_ObjectController != -1)	ImGui::SetNextItemOpen(open_ObjectController == 1);
		if (ImGui::TreeNode("New Object Create"))
		{
			IsAllClosed = false;
			open_ObjectController = 1;
			_uint iPrototypeNum = pGameInstance->Get_ObjectPrototypeNum();
			static wstring SelectedPrototypeTag = L"";
			static char InputLayerTag[MAX_PATH] = "";
			static float Position[3] = { 0,0,0 };

			if (ImGui::BeginListBox("", ImVec2(150.f, min((iPrototypeNum) *18.f,250.f))))
			{
				static int	SelectedPrototypeIndex = 0;
			

				for (int n = 0; n < (int)iPrototypeNum; n++)
				{
					const bool is_selected = (SelectedPrototypeIndex == n);

					wstring wtag = (pGameInstance->Get_PrototypeTag(n));
					string stag = "";
					if (ImGui::Selectable(stag.assign(wtag.begin(), wtag.end()).c_str(), is_selected))
					{
						SelectedPrototypeIndex = n;
						SelectedPrototypeTag = wtag.c_str();
					}

					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (is_selected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndListBox();
			}

			{
				ImGui::SameLine(0.f, 50.f);
				static bool BtndisAbler = false;

				if (!strcmp(InputLayerTag, "")) BtndisAbler = true;
				else BtndisAbler = false;
				if (BtndisAbler) ImGui::BeginDisabled(true);


				if (ImGui::Button("Create Object"))
				{
					if (lstrcmp(SelectedPrototypeTag.c_str(), L"") || strcmp(InputLayerTag, ""))
					{
						string sLayer = string("Layer_") + string(InputLayerTag);
						wstring wLayer = L"";
						wLayer.assign(sLayer.begin(), sLayer.end());
						pGameInstance->Add_GameObject_To_Layer(pGameInstance->Get_NowSceneNum(),
							wLayer.c_str(), SelectedPrototypeTag.c_str());

						((CTransform*)(pGameInstance->Get_GameObject_By_LayerLastIndex(pGameInstance->Get_NowSceneNum(),
							wLayer.c_str())->Get_Component(TAG_COM(Com_Transform))))
						->Set_MatrixState(CTransform::STATE_POS, _float3(Position[0], Position[1], Position[2]));
			

						strcpy_s(InputLayerTag, "");

					}
				}

				if (BtndisAbler)	ImGui::EndDisabled();

			}


			{
				ImGui::InputText(" ", InputLayerTag, MAX_PATH);
			}

			Make_VerticalSpacing(3);
			{

				ImGui::DragFloat3("Position", Position, 0.1f, -FLT_MAX, FLT_MAX);

			}
			Make_VerticalSpacing(3);


			string stag = "";
			string Texted = "Selected Object Tag : " + stag.assign(SelectedPrototypeTag.begin(), SelectedPrototypeTag.end());
			ImGui::Text(Texted.c_str());

			if (strcmp(InputLayerTag, ""))
				Texted = "Input Layer Tag : " + string("Layer_") + string(InputLayerTag);
			else
				Texted = "Input Layer Tag : ";

			ImGui::Text(Texted.c_str());

			ImGui::TreePop();
		}
		else IsAllClosed = true;


		if (open_ObjectController != -1)	ImGui::SetNextItemOpen(open_ObjectController == 2);
		if (ImGui::TreeNode("Control Created Object"))
		{
			IsAllClosed = false;
			open_ObjectController = 2;
			_uint iNowSceneEnum = pGameInstance->Get_NowSceneNum();

			_int iStaticLayerNum = _int(pGameInstance->Get_SceneLayerSize(SCENE_STATIC));
			_int iNowSceneLayerNum =(_int) pGameInstance->Get_SceneLayerSize(iNowSceneEnum);

			static int SelectedLayerIndex = 0;

			if (ImGui::BeginListBox("", ImVec2(150.f, min((iStaticLayerNum + iNowSceneLayerNum) *18.f, 250.f))))
			{
				for (int n = 0; n < iStaticLayerNum + iNowSceneLayerNum; n++)
				{
					const bool is_selected = (SelectedLayerIndex == n);

					if (iStaticLayerNum > n)
					{
						wstring wTag = g_pGameInstance->Get_SceneLayerTag(SCENE_STATIC, n);
						string sTag;

						if (ImGui::Selectable(sTag.assign(wTag.begin(), wTag.end()).c_str(), is_selected))
						{
							SelectedLayerIndex = n;
						}
					}
					else
					{
						wstring wTag = g_pGameInstance->Get_SceneLayerTag(iNowSceneEnum, n - iStaticLayerNum);
						string sTag;

						if (ImGui::Selectable(sTag.assign(wTag.begin(), wTag.end()).c_str(), is_selected))
						{
							SelectedLayerIndex = n;
						}

					}

					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (is_selected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndListBox();
			}
			ImGui::SameLine(0.f, 20);


			static int SelectedLayerObjectIndex = 0;
			list<CGameObject*>* pObjectList = nullptr;

			if (SelectedLayerIndex < iStaticLayerNum)
				pObjectList = pGameInstance->Get_ObjectList_from_Layer(iNowSceneEnum, pGameInstance->Get_SceneLayerTag(SCENE_STATIC, SelectedLayerIndex));
			else
				pObjectList = pGameInstance->Get_ObjectList_from_Layer(iNowSceneEnum, pGameInstance->Get_SceneLayerTag(iNowSceneEnum, SelectedLayerIndex - iStaticLayerNum));


			if (ImGui::BeginListBox("  ", ImVec2(150.f, min((pObjectList->size()) *18.f, 250.f))))
			{
				for (int n = 0; n < pObjectList->size(); n++)
				{
					const bool is_selected = (SelectedLayerObjectIndex == n);


					if (ImGui::Selectable(to_string(n).c_str(), is_selected))
					{
						SelectedLayerObjectIndex = n;
					}


					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (is_selected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndListBox();
			}

			Make_VerticalSpacing(3);


			{
				auto& iter = pObjectList->begin();
				for (_int i = 0; i < SelectedLayerObjectIndex; i++)
					iter++;
				CTransform* pObjectTransform = (CTransform*)(*iter)->Get_Component(TAG_COM(Com_Transform));

				if (pObjectTransform != nullptr)
				{
					ImGui::Text("Object Controller");



					Make_VerticalSpacing(2);
					ImGui::Text("Position");
					float ObjectPosition[3] = { 0,0,0 };
					_float3 vPosition = pObjectTransform->Get_MatrixState_Float3(CTransform::STATE_POS);
					memcpy(ObjectPosition, &vPosition, sizeof(_float) * 3);
					ImGui::DragFloat3("   ", ObjectPosition, 0.1f, -FLT_MAX, FLT_MAX);

					memcpy(&vPosition, ObjectPosition, sizeof(_float) * 3);

					pObjectTransform->Set_MatrixState(CTransform::STATE_POS, vPosition);


					Make_VerticalSpacing(2);
					ImGui::Text("Rotation");

					if (ImGui::Button("-", ImVec2(20, 18)))
					{
						//pObjectTransform->Get_MoveSpeed()
						pObjectTransform->Turn_CW(XMVectorSet(1, 0, 0, 0), fDeltaTime);
					}
					ImGui::SameLine(0, 10);		ImGui::Text("X");			ImGui::SameLine(0, 10);
					if (ImGui::Button("+", ImVec2(20, 18)))
					{
						pObjectTransform->Turn_CCW(XMVectorSet(1, 0, 0, 0), fDeltaTime);
					}


					if (ImGui::Button("- ", ImVec2(20, 18)))
					{
						pObjectTransform->Turn_CW(XMVectorSet(0, 1, 0, 0), fDeltaTime);
					}
					ImGui::SameLine(0, 10);		ImGui::Text("Y");			ImGui::SameLine(0, 10);
					if (ImGui::Button("+ ", ImVec2(20, 18)))
					{
						pObjectTransform->Turn_CCW(XMVectorSet(0, 1, 0, 0), fDeltaTime);

					}


					if (ImGui::Button("-  ", ImVec2(20, 18)))
					{
						pObjectTransform->Turn_CW(XMVectorSet(0, 0, 1, 0), fDeltaTime);

					}
					ImGui::SameLine(0, 10);		ImGui::Text("Z");			ImGui::SameLine(0, 10);
					if (ImGui::Button("+  ", ImVec2(20, 18)))
					{
						pObjectTransform->Turn_CCW(XMVectorSet(0, 0, 1, 0), fDeltaTime);
					}
				}


			}

			ImGui::TreePop();
		}
		else if (IsAllClosed)
		{
			open_ObjectController = -1;
		}

		ImGui::TreePop();
	}
	ImGui::Separator();

	if (open_action != -1)	ImGui::SetNextItemOpen(open_action != 0);
	if (ImGui::TreeNode("Post Processing Controller"))
	{
		ImGui::TreePop();
	}
	ImGui::Separator();




	End_Update_Frame();
	return _int();
}

_int CImguiMgr::LateUpdate_DebugWnd(_double fDeltaTime)
{
	if (!m_bOnDebugWnd) return S_FALSE;

	return _int();
}

_int CImguiMgr::Render_DebugWnd()
{
	if (!m_bOnDebugWnd) return S_FALSE;
	Render_ImguiMgr();
	return _int();
}

void CImguiMgr::Make_VerticalSpacing(_uint count)
{
	for (_uint i = 0; i < count; i++)
	{
		ImGui::Spacing();
	}
}

void CImguiMgr::Free()
{
	// Cleanup
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pBackBufferRTV);
	Safe_Release(m_pDepthStencilView);
	Safe_Release(m_pSwapChain);
}

#endif // USE_IMGUI