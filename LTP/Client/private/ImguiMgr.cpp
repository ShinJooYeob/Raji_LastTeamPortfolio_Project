#include "stdafx.h"
#include "..\public\ImguiMgr.h"
#include "Scene_Loading.h"
#include "GameObject.h"
#include "Camera_Main.h"
#include "TestObject_PhysX.h"
#include "TestObject_BaseCollider.h"

#include "PhysX/Collider_PhysX_Static.h"
#include "PhysX/Collider_PhysX_Dynamic.h"
#include "PhysX/Collider_PhysX_Joint.h"

#include "Monster_Mahinasura_Minion.h"
#include "Monster_Mahinasura_Leader.h"

#include "Monster_Gadasura_Black.h"
#include "Monster_Gadasura_Rage.h"

#ifdef _DEBUG


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

	if (g_pGameInstance->Get_DIKeyState(DIK_F1) & DIS_Down) m_bOnDebugWnd = !m_bOnDebugWnd;
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
		ImGui::TableNextColumn(); ImGui::Checkbox("Block Move", &bArrWindowFlag[1]);


		CCamera_Main* pCam = (CCamera_Main*)pGameInstance->Get_GameObject_By_LayerIndex(g_pGameInstance->Get_NowSceneNum(), TAG_LAY(Layer_Camera_Main));
		if (pCam != nullptr)
		{
			ImGui::SameLine();

			//enum ECameraMode
			//{
			//	CAM_MODE_FREE, CAM_MODE_NOMAL
			//};
			_bool	CamModeIsFree = (pCam->Get_CameraMode() == ECameraMode::CAM_MODE_FREE);
			ImGui::TableNextColumn(); ImGui::Checkbox("Cam Free", &CamModeIsFree);
			pCam->Set_CameraMode((CamModeIsFree) ? ECameraMode::CAM_MODE_FREE : ECameraMode::CAM_MODE_NOMAL);

			ImGui::SameLine();
		}
		else
		{
			ImGui::SameLine(0.f, 50.f);
		}

		if (ImGui::Button("Close All")) { open_action = 0; }
	}
	ImGui::Separator();

	if (open_action != -1)	ImGui::SetNextItemOpen(open_action != 0);
	if (ImGui::TreeNode("Scene Change"))
	{
		static int SelectedSceneIndex = 0;

		if (ImGui::BeginListBox("", ImVec2(150.f, (SCENE_END - 3.f) *17.5f)))
		{
			for (int n = 0; n < SCENE_END; n++)
			{
				const bool is_selected = (SelectedSceneIndex == n);
				if (pGameInstance->Get_NowSceneNum() == n || n == SCENE_LOADING || n == SCENE_STATIC)continue;
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

			ImGui::SameLine(200, 10);

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
		if (g_pGameInstance->Get_NowSceneNum() == SCENE_LOADING)open_ObjectController = -1;
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

			if (ImGui::BeginListBox("", ImVec2(150.f, min((iPrototypeNum) *18.f, 250.f))))
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
			_int iNowSceneLayerNum = (_int)pGameInstance->Get_SceneLayerSize(iNowSceneEnum);

			static int SelectedLayerObjectIndex = 0;
			static int SelectedLayerIndex = 0;
			static int SelectedAnimIndex = 0;

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
							SelectedLayerObjectIndex = 0;
							SelectedAnimIndex = 0;
						}
					}
					else
					{
						wstring wTag = g_pGameInstance->Get_SceneLayerTag(iNowSceneEnum, n - iStaticLayerNum);
						string sTag;

						if (ImGui::Selectable(sTag.assign(wTag.begin(), wTag.end()).c_str(), is_selected))
						{
							SelectedLayerIndex = n;
							SelectedLayerObjectIndex = 0;
							SelectedAnimIndex = 0;
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
				CGameObject*	pObject = (*iter);
				CTransform* pObjectTransform = (CTransform*)(*iter)->Get_Component(TAG_COM(Com_Transform));

				{
					_bool	bIsOcllusion = pObject->Get_IsOcllusion();
					ImGui::Checkbox("Ocllusion Object", &bIsOcllusion);
					pObject->Set_IsOcllusion(bIsOcllusion);


					Make_VerticalSpacing(2);
				}

				if (pObjectTransform != nullptr)
				{
					if (ImGui::TreeNode("Object Controller"))
					{


						Make_VerticalSpacing(2);
						ImGui::Text("Position");
						static _float DragSpeed = 0.01f;
						ImGui::DragFloat("Drag Speed", &DragSpeed, 0.013f, 0.00001f, 100.f);
						DragSpeed = min(max(DragSpeed, 0.00001f), 100.f);

						float ObjectPosition[3] = { 0,0,0 };
						_float3 vPosition = pObjectTransform->Get_MatrixState_Float3(CTransform::STATE_POS);
						memcpy(ObjectPosition, &vPosition, sizeof(_float) * 3);
						ImGui::DragFloat3("   ", ObjectPosition, DragSpeed, -FLT_MAX, FLT_MAX);

						memcpy(&vPosition, ObjectPosition, sizeof(_float) * 3);

						pObjectTransform->Set_MatrixState(CTransform::STATE_POS, vPosition);


						Make_VerticalSpacing(2);
						ImGui::Text("Rotation");
						ImGui::SameLine(250);	ImGui::Text("Scaling");

						static _float RotSpeed = 60.;
						ImGui::DragFloat("Rot Speed", &RotSpeed, 0.1f, 0.001f, 360);
						RotSpeed = min(max(RotSpeed, 0), 360);


						ImGui::Button("-", ImVec2(20, 18));
						if (ImGui::IsItemHovered())
						{
							_float OldTurnSpeed = pObjectTransform->Get_TurnSpeed();
							pObjectTransform->Set_TurnSpeed(XMConvertToRadians(RotSpeed));
							pObjectTransform->Turn_CW(XMVectorSet(1, 0, 0, 0), fDeltaTime);
							pObjectTransform->Set_TurnSpeed(OldTurnSpeed);

						}
						ImGui::SameLine(0, 10);		ImGui::Text("X");			ImGui::SameLine(0, 10);

						ImGui::Button("+", ImVec2(20, 18));
						if (ImGui::IsItemHovered())
						{
							_float OldTurnSpeed = pObjectTransform->Get_TurnSpeed();
							pObjectTransform->Set_TurnSpeed(XMConvertToRadians(RotSpeed));
							pObjectTransform->Turn_CCW(XMVectorSet(1, 0, 0, 0), fDeltaTime);
							pObjectTransform->Set_TurnSpeed(OldTurnSpeed);
						}


						ImGui::SameLine(250);
						_float3 OldScaled = pObjectTransform->Get_Scale();
						_float Scaled = OldScaled.x;
						ImGui::DragFloat("X ", &Scaled, 0.001f, 0.001f, FLT_MAX);
						OldScaled.x = Scaled;



						ImGui::Button("- ", ImVec2(20, 18));
						if (ImGui::IsItemHovered())
						{
							_float OldTurnSpeed = pObjectTransform->Get_TurnSpeed();
							pObjectTransform->Set_TurnSpeed(XMConvertToRadians(RotSpeed));
							pObjectTransform->Turn_CW(XMVectorSet(0, 1, 0, 0), fDeltaTime);
							pObjectTransform->Set_TurnSpeed(OldTurnSpeed);
						}
						ImGui::SameLine(0, 10);		ImGui::Text("Y");			ImGui::SameLine(0, 10);
						ImGui::Button("+ ", ImVec2(20, 18));

						if (ImGui::IsItemHovered())
						{
							_float OldTurnSpeed = pObjectTransform->Get_TurnSpeed();
							pObjectTransform->Set_TurnSpeed(XMConvertToRadians(RotSpeed));
							pObjectTransform->Turn_CCW(XMVectorSet(0, 1, 0, 0), fDeltaTime);
							pObjectTransform->Set_TurnSpeed(OldTurnSpeed);

						}

						ImGui::SameLine(250);
						Scaled = OldScaled.y;
						ImGui::DragFloat("Y ", &Scaled, 0.001f, 0.001f, FLT_MAX);
						OldScaled.y = Scaled;
						ImGui::Button("-  ", ImVec2(20, 18));
						if (ImGui::IsItemHovered())
						{
							_float OldTurnSpeed = pObjectTransform->Get_TurnSpeed();
							pObjectTransform->Set_TurnSpeed(XMConvertToRadians(RotSpeed));
							pObjectTransform->Turn_CW(XMVectorSet(0, 0, 1, 0), fDeltaTime);
							pObjectTransform->Set_TurnSpeed(OldTurnSpeed);

						}
						ImGui::SameLine(0, 10);		ImGui::Text("Z");			ImGui::SameLine(0, 10);
						ImGui::Button("+  ", ImVec2(20, 18));
						if (ImGui::IsItemHovered())
						{
							_float OldTurnSpeed = pObjectTransform->Get_TurnSpeed();
							pObjectTransform->Set_TurnSpeed(XMConvertToRadians(RotSpeed));
							pObjectTransform->Turn_CCW(XMVectorSet(0, 0, 1, 0), fDeltaTime);
							pObjectTransform->Set_TurnSpeed(OldTurnSpeed);
						}

						ImGui::SameLine(250);
						Scaled = OldScaled.z;
						ImGui::DragFloat("Z ", &Scaled, 0.001f, 0.001f, FLT_MAX);
						OldScaled.z = Scaled;

						OldScaled.x = max(OldScaled.x, 0.001f);
						OldScaled.y = max(OldScaled.y, 0.001f);
						OldScaled.z = max(OldScaled.z, 0.001f);


						pObjectTransform->Scaled_All(OldScaled);


						Make_VerticalSpacing(1);
						ImGui::SameLine(50);
						if (ImGui::Button("ReSet Rot", ImVec2(80, 25)))
						{
							pObjectTransform->Rotation_CW(XMVectorSet(0, 1, 0, 0), 0);
						}

						ImGui::SameLine(250);
						if (ImGui::Button("ReSet Scale", ImVec2(100, 25)))
						{
							pObjectTransform->Scaled_All(_float3(1, 1, 1));
						}

						Make_VerticalSpacing(3);

						_float fNowHP = pObject->Get_NowHP();
						_float fMaxHP = pObject->Get_MaxHP();

						_float HpRate = fNowHP / fMaxHP;

						ImGui::DragFloat("Hp", &HpRate, 0.001f, 0.f, 1.f);
						HpRate = min(max(HpRate, 0), 1.f);

						HpRate *= fMaxHP;
						pObject->Set_NowHP(HpRate);

						Make_VerticalSpacing(3);
						ImGui::TreePop();
					}

					CModel* pModel = (CModel*)(*iter)->Get_Component(TAG_COM(Com_Model));

					if (pModel != nullptr)
					{
						_int NumAnim = _int(pModel->Get_HaveAnimNum());
						if (NumAnim > 0)
						{
							if (ImGui::TreeNode("Animation Control"))
							{

								_float fSpeed = pModel->Get_DebugAnimSpeed();

								ImGui::DragFloat("Anim Speed", &fSpeed, 0.01f, 0.f, 20.f);
								fSpeed = min(max(fSpeed, 0), 20.f);

								pModel->Set_DebugAnimSpeed(fSpeed);

								Make_VerticalSpacing(3);

								static _float ExitTimeValue = 0.15f;

								ImGui::DragFloat("ExitTime", &ExitTimeValue, 0.001f, 0.f, 10.f);
								ExitTimeValue = min(max(ExitTimeValue, 0), 10.f);


								Make_VerticalSpacing(3);

								static _bool bWantToStopAnimCertainTiming = false;
								static _bool PressedPlayedAnim = false;
								static _float fTargetAnimRate = 0.7f;

								ImGui::Checkbox("Stop Anim Certain Rate", &bWantToStopAnimCertainTiming);

								if (bWantToStopAnimCertainTiming)
								{

									ImGui::DragFloat("TargetAnimRate", &fTargetAnimRate, 0.001f, 0.0001f, 1.f);
									fTargetAnimRate = min(max(fTargetAnimRate, 0.0001f), 1.f);


									if (PressedPlayedAnim)
									{
										if (pModel->Get_PlayRate() > fTargetAnimRate)
										{
											pModel->Set_DebugAnimSpeed(0);
											PressedPlayedAnim = false;
										}
									}
								}



								if (ImGui::BeginListBox("", ImVec2(150.f, min((NumAnim) *18.f, 250.f))))
								{
									for (int n = 0; n < NumAnim; n++)
									{
										const bool is_selected = (SelectedAnimIndex == n);


										if (ImGui::Selectable(to_string(n).c_str(), is_selected))
										{
											SelectedAnimIndex = n;
										}

										// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
										if (is_selected)
										{
											ImGui::SetItemDefaultFocus();
										}
									}
									ImGui::EndListBox();
								}
								ImGui::SameLine(0, 20);
								if (ImGui::Button("Play Animation"))
								{
									pModel->Change_AnimIndex_ReturnTo_Must(SelectedAnimIndex, 0, ExitTimeValue);
									if (bWantToStopAnimCertainTiming)
									{
										PressedPlayedAnim = true;
									}
								}


								Make_VerticalSpacing(3);

								ImGui::TreePop();
							}

						}
					}
					if (ImGui::TreeNode("LimLight & Emisive Setting"))
					{


						static ImVec4 color = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);
						static ImVec4 ref_color_v(1.0f, 0.0f, 1.0f, 0.5f);
						static bool alpha_preview = true;
						static bool alpha_half_preview = false;
						static bool drag_and_drop = true;
						static bool options_menu = false;
						//static bool options_menu = true;
						static bool hdr = false;
						static bool ref_color = false;


						_float4 ObjectLimLight = pObject->Get_LimLightValue();
						_float4 ObjectEmissive = pObject->Get_EmissiveValue();


						color = ImVec4(ObjectLimLight.x, ObjectLimLight.y, ObjectLimLight.z, ObjectLimLight.w);

						ImGuiColorEditFlags misc_flags = (hdr ? ImGuiColorEditFlags_HDR : 0) | (drag_and_drop ? 0 : ImGuiColorEditFlags_NoDragDrop) | (alpha_half_preview ? ImGuiColorEditFlags_AlphaPreviewHalf : (alpha_preview ? ImGuiColorEditFlags_AlphaPreview : 0)) | (options_menu ? 0 : ImGuiColorEditFlags_NoOptions);
						ImGuiColorEditFlags flags = misc_flags;
						/*if (!alpha)           flags |= ImGuiColorEditFlags_NoAlpha;        // This is by default if you call ColorPicker3() instead of ColorPicker4()	  */
						/*if (alpha_bar)        flags |= ImGuiColorEditFlags_AlphaBar;																					  */
						/*if (!side_preview)    flags |= ImGuiColorEditFlags_NoSidePreview;																				  */
						/*if (picker_mode == 1) flags |= ImGuiColorEditFlags_PickerHueBar;																				  */
						/*if (picker_mode == 2) flags |= ImGuiColorEditFlags_PickerHueWheel;																			  */
						/*if (display_mode == 1)flags |= ImGuiColorEditFlags_NoInputs;       // Disable all RGB/HSV/Hex displays										  */
						/*if (display_mode == 2)flags |= ImGuiColorEditFlags_DisplayRGB;     // Override display mode													  */
						/*if (display_mode == 3)flags |= ImGuiColorEditFlags_DisplayHSV;																				  */
						/*if (display_mode == 4)flags |= ImGuiColorEditFlags_DisplayHex;																				  */

						ImGui::ColorPicker4("MyColor##444", (float*)&color, flags, ref_color ? &ref_color_v.x : NULL);



						Make_VerticalSpacing(4);

						float ObjectEmissiveArr[3] = { 0,0,0 };
						ImGui::Text("Shade / Blur / Bright");
						memcpy(ObjectEmissiveArr, &ObjectEmissive, sizeof(_float) * 3);

						ImGui::DragFloat3(" ", ObjectEmissiveArr, 0.0073f, 0, 1.f);

						memcpy(&ObjectEmissive, ObjectEmissiveArr, sizeof(_float) * 3);

					

						pObject->Set_LimLight_N_Emissive(_float4(color.x, color.y, color.z, color.w), ObjectEmissive);


						ImGui::TreePop();
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
		ImGui::Separator();
		CUtilityMgr* pUtil = GetSingle(CUtilityMgr);

		if (ImGui::TreeNode("Main Light Controller"))
		{
			LIGHTDESC* pLightDesc = g_pGameInstance->Get_LightDesc(tagLightDesc::TYPE_DIRECTIONAL, 0);

			if (pLightDesc == nullptr)
			{
				LIGHTDESC LightDesc;

				LightDesc.eLightType = tagLightDesc::TYPE_DIRECTIONAL;
				LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
				LightDesc.vAmbient = _float4(1.0f);
				LightDesc.vSpecular = _float4(1);
				LightDesc.vVector = _float4(-10, 10, -10, 0);
				g_pGameInstance->Add_Light(LightDesc);
				pLightDesc = g_pGameInstance->Get_LightDesc(tagLightDesc::TYPE_DIRECTIONAL, 0);
			}


			_float3 vSunPos = pLightDesc->vVector;
			float arrSunPos[3];
			memcpy(arrSunPos, &vSunPos, sizeof(float) * 3);


			ImGui::DragFloat3("Sun Pos", arrSunPos, 0.134f, -64, 320.f);

			vSunPos.x = max(min(arrSunPos[0], 1024.f), -64.f);
			vSunPos.y = max(min(arrSunPos[1], 128.f), 10.f);
			vSunPos.z = max(min(arrSunPos[2], 1024.f), -64.f);

			g_pGameInstance->Relocate_LightDesc(tagLightDesc::TYPE_DIRECTIONAL, 0, _float4(vSunPos, 0).XMVector());


			Make_VerticalSpacing(1);



			_float3 vSunLookAt = pUtil->Get_Renderer()->Get_SunAtPoint();
			memcpy(arrSunPos, &vSunLookAt, sizeof(float) * 3);

			ImGui::DragFloat3("Sun LookAt ", arrSunPos, 0.134f, -64, 320.f);

			vSunLookAt.x = max(min(arrSunPos[0], 320.f), -64.f);
			vSunLookAt.y = max(min(arrSunPos[1], -10.f), -128.f);
			vSunLookAt.z = max(min(arrSunPos[2], 320.f), -64.f);

			pUtil->Get_Renderer()->Set_SunAtPoint(vSunLookAt);


			ImGui::TreePop();
		}
		ImGui::Separator();


		_bool bBool = pUtil->Get_Renderer()->Get_IsOnPostPorcessing(POSTPROCESSING_DEBUGCOLLIDER);
		ImGui::Checkbox("Show Collider", &bBool);
		pUtil->Get_Renderer()->OnOff_PostPorcessing_byParameter(POSTPROCESSING_DEBUGCOLLIDER, bBool);

		ImGui::Separator();

		bBool = pUtil->Get_Renderer()->Get_IsOnPostPorcessing(POSTPROCESSING_DEBUGTARGET);
		ImGui::Checkbox("Debug RenderTarget", &bBool);
		pUtil->Get_Renderer()->OnOff_PostPorcessing_byParameter(POSTPROCESSING_DEBUGTARGET, bBool);

		if (bBool)
		{

			static int SelectedTargetIndex = 0;
			_int iTargetNum = _int(pUtil->Get_Renderer()->Get_DebugRenderTargetSize());

			if (ImGui::BeginListBox("", ImVec2(150.f, (iTargetNum) *18.f)))
			{
				for (int n = 0; n < iTargetNum; n++)
				{
					const bool is_selected = (SelectedTargetIndex == n);

					wstring wTargetTag = pUtil->Get_Renderer()->Get_DebugRenderTargetTag(n);
					string sTargetTag = "";

					if (ImGui::Selectable(sTargetTag.assign(wTargetTag.begin(), wTargetTag.end()).c_str(), is_selected))
					{
						SelectedTargetIndex = n;
					}

					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (is_selected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndListBox();





				ImGui::SameLine(0, 70);


				if (ImGui::Button("Show Bigger"))
					ImGui::OpenPopup("Show Bigger");


				ImVec2 center = ImGui::GetMainViewport()->GetCenter();
				ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
				ImGui::SetNextWindowSize(ImVec2(center.x * 1.79f, center.y * 1.93f), ImGuiCond_None);

				if (ImGui::BeginPopupModal("Show Bigger", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					if (ImGui::Button("Close", ImVec2(center.x* 1.76f, 0)))
					{
						ImGui::CloseCurrentPopup();
					}
					ImGui::SetItemDefaultFocus();

					ID3D11ShaderResourceView* TargetSRV = g_pGameInstance->Get_SRV(pUtil->Get_Renderer()->Get_DebugRenderTargetTag(SelectedTargetIndex));

					if (TargetSRV != nullptr)
					{

						ImTextureID my_tex_id = TargetSRV;
						float my_tex_w = center.x* 1.76f;
						float my_tex_h = center.y * 1.76f;
						{
							ImGuiIO& io = ImGui::GetIO();
							ImVec2 pos = ImGui::GetCursorScreenPos();
							ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
							ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
							ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
							ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white
							ImGui::Image(my_tex_id, ImVec2(my_tex_w, my_tex_h), uv_min, uv_max, tint_col, border_col);
							if (ImGui::IsItemHovered() && pGameInstance->Get_DIMouseButtonState(CInput_Device::MBS_WHEEL) & DIS_Press)
							{
								ImGui::BeginTooltip();
								float region_sz = 100.0f;
								float region_x = io.MousePos.x - pos.x - region_sz * 0.5f;
								float region_y = io.MousePos.y - pos.y - region_sz * 0.5f;
								float zoom = 4.0f;
								if (region_x < 0.0f) { region_x = 0.0f; }
								else if (region_x > my_tex_w - region_sz) { region_x = my_tex_w - region_sz; }
								if (region_y < 0.0f) { region_y = 0.0f; }
								else if (region_y > my_tex_h - region_sz) { region_y = my_tex_h - region_sz; }
								ImVec2 uv0 = ImVec2((region_x) / my_tex_w, (region_y) / my_tex_h);
								ImVec2 uv1 = ImVec2((region_x + region_sz) / my_tex_w, (region_y + region_sz) / my_tex_h);
								ImGui::Image(my_tex_id, ImVec2(region_sz * zoom, region_sz * zoom), uv0, uv1, tint_col, border_col);
								ImGui::EndTooltip();
							}
						}
					}


					ImGui::EndPopup();
				}



			}














		}


		ImGui::Separator();
		//if (!bBool)
		//	ImGui::Separator();

		bBool = pUtil->Get_Renderer()->Get_IsOnPostPorcessing(POSTPROCESSING_SHADOW);
		ImGui::Checkbox("Shadow", &bBool);
		pUtil->Get_Renderer()->OnOff_PostPorcessing_byParameter(POSTPROCESSING_SHADOW, bBool);

		if (bBool)
		{
			_float Value = pUtil->Get_Renderer()->Get_ShadowIntensive();
			ImGui::DragFloat("ShadowIntensive", &Value, 0.001f, 0.001f, 1.f);
			Value = max(min(Value, 1.f), 0.0001f);
			pUtil->Get_Renderer()->Set_ShadowIntensive(Value);

			Make_VerticalSpacing(1);

			ImGui::Separator();
			_bool bGodRayBool = pUtil->Get_Renderer()->Get_IsOnPostPorcessing(POSTPROCESSING_GODRAY);
			ImGui::Checkbox("GodRay", &bGodRayBool);
			pUtil->Get_Renderer()->OnOff_PostPorcessing_byParameter(POSTPROCESSING_GODRAY, bGodRayBool);

			if (bGodRayBool)
			{
				if (ImGui::TreeNode("GodRay Color"))
				{
					static ImVec4 color = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);
					static ImVec4 ref_color_v(1.0f, 0.0f, 1.0f, 0.5f);
					static bool alpha_preview = true;
					static bool alpha_half_preview = false;
					static bool drag_and_drop = true;
					static bool options_menu = false;
					//static bool options_menu = true;
					static bool hdr = false;
					static bool ref_color = false;

					//Get_GodRayColor() { return m_vGodRayColor; }
					//Set_SunAtPoint(_float3 vVector) { m_vGodRayColor = vVector; }
					_float3 ColorValue = pUtil->Get_Renderer()->Get_GodRayColor();

					color = ImVec4(ColorValue.x, ColorValue.y, ColorValue.z, 1.f);

					ImGuiColorEditFlags misc_flags = (hdr ? ImGuiColorEditFlags_HDR : 0) | (drag_and_drop ? 0 : ImGuiColorEditFlags_NoDragDrop) | (alpha_half_preview ? ImGuiColorEditFlags_AlphaPreviewHalf : (alpha_preview ? ImGuiColorEditFlags_AlphaPreview : 0)) | (options_menu ? 0 : ImGuiColorEditFlags_NoOptions);
					ImGuiColorEditFlags flags = misc_flags;

					ImGui::ColorPicker4("FogColor##444", (float*)&color, flags, ref_color ? &ref_color_v.x : NULL);

					ColorValue.x = max(min(color.x, 1.f), 0.f);
					ColorValue.y = max(min(color.y, 1.f), 0.f);
					ColorValue.z = max(min(color.z, 1.f), 0.f);

					pUtil->Get_Renderer()->Set_GodRayColor(ColorValue);
					ImGui::TreePop();
				}



				Value = pUtil->Get_Renderer()->Get_SunSize() * 0.05f;
				ImGui::DragFloat("GodraySunSize", &Value, 0.06f, 0.01f, 1.f);
				Value = max(min(Value, 1.f), 0.06f);
				pUtil->Get_Renderer()->Set_SunSize(Value* 20.f);



				Value = pUtil->Get_Renderer()->Get_GodrayLength();
				ImGui::DragFloat("GodrayLength", &Value, 1.f, 1.f, 64.f);
				Value = _float(_int(max(min(Value, 64.f), 1.f)));
				pUtil->Get_Renderer()->Set_GodrayLength(Value);

				Value = pUtil->Get_Renderer()->Get_GodrayIntensity();
				ImGui::DragFloat("GodrayIntensity", &Value, 0.001f, 0.001f, 1.f);
				Value = max(min(Value, 1.f), 0.001f);
				pUtil->Get_Renderer()->Set_GodrayIntensity(Value);

				Value = pUtil->Get_Renderer()->Get_StartDecay();
				ImGui::DragFloat("StartDecay", &Value, 0.001f, 0.001f, 1.f);
				Value = max(min(Value, 1.f), 0.001f);
				pUtil->Get_Renderer()->Set_StartDecay(Value);


				Value = pUtil->Get_Renderer()->Get_DistDecay();
				ImGui::DragFloat("DistDecay", &Value, 0.001f, 0.001f, 30.f);
				Value = max(min(Value, 30.f), 0.001f);
				pUtil->Get_Renderer()->Set_DistDecay(Value);


				Value = pUtil->Get_Renderer()->Get_MaxDeltaLen();
				ImGui::DragFloat("MaxDeltaLen", &Value, 0.0001f, 0.001f, 1.f);
				Value = max(min(Value, 1.f), 0.0001f);
				pUtil->Get_Renderer()->Set_MaxDeltaLen(Value);

			}

			ImGui::Separator();
			bGodRayBool = pUtil->Get_Renderer()->Get_IsOnPostPorcessing(POSTPROCESSING_LENSEFLARE);
			ImGui::Checkbox("LenseFlare", &bGodRayBool);
			pUtil->Get_Renderer()->OnOff_PostPorcessing_byParameter(POSTPROCESSING_LENSEFLARE, bGodRayBool);

			if (bGodRayBool)
			{
				 
				//Value = (128 - pUtil->Get_Renderer()->Get_LensfalreSupportSunSize()) / 128.f;

				Value =pUtil->Get_Renderer()->Get_LensfalreSupportSunSize();
				Value = 1 - ((Value - 16) / 344.f);
				ImGui::DragFloat("Support Sun Flare", &Value, 0.01f, 0.01f, 1.f);
				Value = max(min(Value, 1.f), 0.f);
				Value = (1 - Value) * 344.f + 16.f;

				pUtil->Get_Renderer()->Set_LensfalreSupportSunSize(Value);


				_int iValue = _int(pUtil->Get_Renderer()->Get_LensefalreNoiseTexIndex());
				ImGui::InputInt("Sun Noise", &iValue);
				iValue = _int(max(min(iValue, 360), 0));
				pUtil->Get_Renderer()->Set_LensefalreNoiseTexIndex(_uint(iValue));

			}






		}

		ImGui::Separator();

		bBool = pUtil->Get_Renderer()->Get_IsOnPostPorcessing(POSTPROCESSING_BLOOM);
		ImGui::Checkbox("Bloom", &bBool);
		pUtil->Get_Renderer()->OnOff_PostPorcessing_byParameter(POSTPROCESSING_BLOOM, bBool);

		if (bBool)
		{
			_float Value = pUtil->Get_Renderer()->Get_BloomOverLuminceValue();
			ImGui::DragFloat("BloomOverLumince", &Value, 0.001f, 0.001f, 1.f);
			Value = max(min(Value, 1.f), 0.01f);
			pUtil->Get_Renderer()->Set_BloomOverLuminceValue(Value);


			Value = pUtil->Get_Renderer()->Get_BloomBrightnessMul();
			ImGui::DragFloat("Brightness Mul", &Value, 0.001f, 0.001f, 3.f);
			Value = max(min(Value, 3.f), 0.001f);
			pUtil->Get_Renderer()->Set_BloomBrightnessMul(Value);


		}


		ImGui::Separator();


		bBool = pUtil->Get_Renderer()->Get_IsOnPostPorcessing(POSTPROCESSING_DOF);
		ImGui::Checkbox("Depth Of Feild", &bBool);
		pUtil->Get_Renderer()->OnOff_PostPorcessing_byParameter(POSTPROCESSING_DOF, bBool);

		if (bBool)
		{
			_float Value = pUtil->Get_Renderer()->Get_DofLength();
			ImGui::DragFloat("Dof Length", &Value, 0.1f, 0.001f, 100.f);
			Value = max(min(Value, 100.f), 0.01f);
			pUtil->Get_Renderer()->Set_DofLength(Value);
		}




		ImGui::Separator();

		bBool = pUtil->Get_Renderer()->Get_IsOnPostPorcessing(POSTPROCESSING_DDFOG);
		ImGui::Checkbox("DDFog", &bBool);
		pUtil->Get_Renderer()->OnOff_PostPorcessing_byParameter(POSTPROCESSING_DDFOG, bBool);

		if (bBool)
		{

			if (ImGui::TreeNode("FogColor"))
			{
				static ImVec4 color = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);
				static ImVec4 ref_color_v(1.0f, 0.0f, 1.0f, 0.5f);
				static bool alpha_preview = true;
				static bool alpha_half_preview = false;
				static bool drag_and_drop = true;
				static bool options_menu = false;
				//static bool options_menu = true;
				static bool hdr = false;
				static bool ref_color = false;

				_float3 ColorValue = pUtil->Get_Renderer()->Get_FogColor();

				color = ImVec4(ColorValue.x, ColorValue.y, ColorValue.z, 1.f);

				ImGuiColorEditFlags misc_flags = (hdr ? ImGuiColorEditFlags_HDR : 0) | (drag_and_drop ? 0 : ImGuiColorEditFlags_NoDragDrop) | (alpha_half_preview ? ImGuiColorEditFlags_AlphaPreviewHalf : (alpha_preview ? ImGuiColorEditFlags_AlphaPreview : 0)) | (options_menu ? 0 : ImGuiColorEditFlags_NoOptions);
				ImGuiColorEditFlags flags = misc_flags;

				ImGui::ColorPicker4("FogColor##444", (float*)&color, flags, ref_color ? &ref_color_v.x : NULL);

				ColorValue.x = max(min(color.x, 1.f), 0.f);
				ColorValue.y = max(min(color.y, 1.f), 0.f);
				ColorValue.z = max(min(color.z, 1.f), 0.f);

				pUtil->Get_Renderer()->Set_FogColor(ColorValue);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("FogHighlightColor"))
			{
				static ImVec4 color = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);
				static ImVec4 ref_color_v(1.0f, 0.0f, 1.0f, 0.5f);
				static bool alpha_preview = true;
				static bool alpha_half_preview = false;
				static bool drag_and_drop = true;
				static bool options_menu = false;
				//static bool options_menu = true;
				static bool hdr = false;
				static bool ref_color = false;

				_float3 ColorValue = pUtil->Get_Renderer()->Get_FogHighlightColor();

				color = ImVec4(ColorValue.x, ColorValue.y, ColorValue.z, 1.f);

				ImGuiColorEditFlags misc_flags = (hdr ? ImGuiColorEditFlags_HDR : 0) | (drag_and_drop ? 0 : ImGuiColorEditFlags_NoDragDrop) | (alpha_half_preview ? ImGuiColorEditFlags_AlphaPreviewHalf : (alpha_preview ? ImGuiColorEditFlags_AlphaPreview : 0)) | (options_menu ? 0 : ImGuiColorEditFlags_NoOptions);
				ImGuiColorEditFlags flags = misc_flags;

				ImGui::ColorPicker4("FogHighlightColor##444", (float*)&color, flags, ref_color ? &ref_color_v.x : NULL);

				ColorValue.x = max(min(color.x, 1.f), 0.f);
				ColorValue.y = max(min(color.y, 1.f), 0.f);
				ColorValue.z = max(min(color.z, 1.f), 0.f);

				pUtil->Get_Renderer()->Set_FogHighlightColor(ColorValue);
				ImGui::TreePop();

			}

			_float Value = pUtil->Get_Renderer()->Get_FogStartDist();
			ImGui::DragFloat("Fog StartDist", &Value, 0.01f, 0.0001f, 100.f);
			Value = max(min(Value, 100.f), 0.0001f);
			pUtil->Get_Renderer()->Set_FogStartDist(Value);

			Value = pUtil->Get_Renderer()->Get_FogGlobalDensity();
			ImGui::DragFloat("Fog Density", &Value, 0.01f, 0.0001f, 20.f);
			Value = max(min(Value, 20.f), 0.0001f);
			pUtil->Get_Renderer()->Set_FogGlobalDensity(Value);


			Value = pUtil->Get_Renderer()->Get_FogHeightFalloff();
			ImGui::DragFloat("Fog HeightFalloff", &Value, 0.001f, 0.0001f, 1.f);
			Value = max(min(Value, 1.f), 0.0001f);
			pUtil->Get_Renderer()->Set_FogHeightFalloff(Value);

			//_float Get_FogGlobalDensity() { return m_fFogGlobalDensity; };
			//void  Set_FogGlobalDensity(_float fValue) { m_fFogGlobalDensity = fValue; };
			//_float Get_FogHeightFalloff() { return m_fFogHeightFalloff; };
			//void  Set_FogHeightFalloff(_float fValue) { m_fFogHeightFalloff = fValue; };

		}





		ImGui::TreePop();
	}

	ImGui::Separator();

	if (GetSingle(CGameInstance)->Get_NowSceneNum() == SCENE_STAGE6)
	{
		IMGUITREE("PhysX Controller")
		{
			Update_DebugWnd_PhysX(fDeltaTime);
			IMGUITREE_END
		}
	}
	ImGui::Separator();
	if (GetSingle(CGameInstance)->Get_NowSceneNum() == SCENE_STAGE6)
	{
		IMGUITREE("Sound_3D_PlayerTest")
		{

			Update_DebugWnd_Sound3D(fDeltaTime);

			IMGUITREE_END
		}

	}
	ImGui::Separator();

	if (GetSingle(CGameInstance)->Get_NowSceneNum() == SCENE_STAGE6)
	{
		IMGUITREE("EFFECT_TEST")
		{

			Update_DebugWnd_EffectTest(fDeltaTime);

			IMGUITREE_END
		}

	}

	ImGui::Separator();

	End_Update_Frame();
	return _int();
}


_int CImguiMgr::Update_DebugWnd_PhysX(_double fDeltaTime)
{
	
	static bool bTestObject = false;
	
	// static
	static _float3 Position = _float3::Zero();
	ImGui::DragFloat3("POS:", (float*)&Position, 0.1f, -100, 100);
	static  _float3 Scale = _float3(1.0f, 1.0f, 1.0f);
	ImGui::DragFloat3("SCALE:", (float*)&Scale, 0.1f, 0.1f, 100);
	static  _float3 Force = _float3(1.0f, 1.0f, 1.0f);
	ImGui::DragFloat3("FORCE:", (float*)&Force, 0.1f, -100, 100);

	static  _float3 MainActorScale = _float3(1.0f,1,1);
	ImGui::DragFloat3("MainActorScale:", (float*)&MainActorScale, 0.1f, -100, 100);

	CCollider_PhysX_Base::PHYSXDESC_STATIC createStatic;
	CCollider_PhysX_Base::PHYSXDESC_DYNAMIC createDynamic;

	static const wchar_t* layerStatic = TAG_LAY(Layer_ColStatic);
	static const wchar_t* layerDynamic = TAG_LAY(Layer_ColDynamic);
	static const wchar_t* layerTrigger = TAG_LAY(Layer_ColTrigger);

	static CTestObject_PhysX* PhysX_Testobj = nullptr;

	//if (bTestObject == false)
	//{
	//	bTestObject = true;
	//	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer
	//	(g_pGameInstance->Get_NowSceneNum(), TAG_LAY(Layer_ColBase), TAG_OP(Prototype_Object_Static_PhysX)));

	//	PhysX_Testobj =
	//		static_cast<CTestObject_PhysX*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(g_pGameInstance->Get_NowSceneNum(), TAG_LAY(Layer_ColBase)));
	//	PhysX_Testobj->Set_ColSetID(E_PHYTYPE_STATIC);
	//	PhysX_Testobj->Set_ModelSetting(CTestObject_PhysX::MODEL_EMPTY);

	//	CCollider_PhysX_Static* colCom = (CCollider_PhysX_Static*)PhysX_Testobj->Get_ComCollider();
	//	CTransform* objTrans = (CTransform*)PhysX_Testobj->Get_Component(TAG_COM(Com_Transform));
	//	NULL_CHECK_BREAK(objTrans);

	//	objTrans->Set_MatrixState(CTransform::STATE_POS, Position);
	//	objTrans->Scaled_All(Scale);

	//	createStatic.bTrigger = false;
	//	createStatic.eShapeType = E_GEOMAT_BOX;
	//	createStatic.mTrnasform = objTrans;
	//	createStatic.mGameObect = PhysX_Testobj;

	//	colCom->Set_ColiiderBufferDesc(createStatic);
	//}

	//{
	//	if (PhysX_Testobj == nullptr)
	//		return 0;

	//	PhysX_Testobj->Set_Postition(Position);
	//	PhysX_Testobj->Set_Scale(Scale);

	//}

#pragma region PHYSX UI


	//IMGUITREE("PHYSX_STATIC")
	//{

	//	if (ImGui::Button("Static_Box"))
	//	{

	//		FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer
	//		(g_pGameInstance->Get_NowSceneNum(), layerStatic, TAG_OP(Prototype_Object_Static_PhysX)));
	//		CTestObject_PhysX* obj =
	//			static_cast<CTestObject_PhysX*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(g_pGameInstance->Get_NowSceneNum(), layerStatic));
	//		obj->Set_ColSetID(E_PHYTYPE_STATIC);
	//		obj->Set_ModelSetting(CTestObject_PhysX::MODEL_GEMETRY);

	//		CTransform* objTrans = (CTransform*)obj->Get_Component(TAG_COM(Com_Transform));
	//		CCollider_PhysX_Static* colStatic = (CCollider_PhysX_Static*)obj->Get_Component(TAG_COM(Com_Collider_PhysX));
	//		objTrans->Set_MatrixState(CTransform::STATE_POS, Position);
	//		objTrans->Scaled_All(Scale);

	//		createStatic.bTrigger = false;
	//		createStatic.eShapeType = E_GEOMAT_BOX;
	//		createStatic.mTrnasform = objTrans;
	//		createStatic.mGameObect = obj;
	//		NULL_CHECK_BREAK(createStatic.mTrnasform);
	//		colStatic->Set_ColiiderDesc(createStatic);

	//	}

	//	ImGui::SameLine();
	//	if (ImGui::Button("Static_Sphere"))
	//	{

	//		FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer
	//		(g_pGameInstance->Get_NowSceneNum(), layerStatic, TAG_OP(Prototype_Object_Static_PhysX)));
	//		CTestObject_PhysX* obj =
	//			static_cast<CTestObject_PhysX*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(g_pGameInstance->Get_NowSceneNum(), layerStatic));
	//		obj->Set_ColSetID(E_PHYTYPE_STATIC);
	//		obj->Set_ModelSetting(CTestObject_PhysX::MODEL_GEMETRY);
	//		CCollider_PhysX_Static* colStatic = (CCollider_PhysX_Static*)obj->Get_Component(TAG_COM(Com_Collider_PhysX));


	//		CTransform* objTrans = (CTransform*)obj->Get_Component(TAG_COM(Com_Transform));
	//		objTrans->Set_MatrixState(CTransform::STATE_POS, Position);
	//		objTrans->Scaled_All(Scale);

	//		createStatic.bTrigger = false;
	//		createStatic.eShapeType = E_GEOMAT_SPEHE;
	//		createStatic.mTrnasform = objTrans;
	//		createStatic.mGameObect = obj;
	//		NULL_CHECK_BREAK(createStatic.mTrnasform);
	//		colStatic->Set_ColiiderDesc(createStatic);

	//	}
	//	ImGui::SameLine();

	//	if (ImGui::Button("Static_Capsule"))
	//	{
	//		FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer
	//		(g_pGameInstance->Get_NowSceneNum(), layerStatic, TAG_OP(Prototype_Object_Static_PhysX)));
	//		CTestObject_PhysX* obj =
	//			static_cast<CTestObject_PhysX*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(g_pGameInstance->Get_NowSceneNum(), layerStatic));
	//		obj->Set_ColSetID(E_PHYTYPE_STATIC);
	//		obj->Set_ModelSetting(CTestObject_PhysX::MODEL_GEMETRY);

	//		CCollider_PhysX_Static* colStatic = (CCollider_PhysX_Static*)obj->Get_Component(TAG_COM(Com_Collider_PhysX));

	//		CTransform* objTrans = (CTransform*)obj->Get_Component(TAG_COM(Com_Transform));
	//		objTrans->Set_MatrixState(CTransform::STATE_POS, Position);
	//		objTrans->Scaled_All(Scale);

	//		createStatic.bTrigger = false;
	//		createStatic.eShapeType = E_GEOMAT_CAPSULE;
	//		createStatic.mTrnasform = objTrans;
	//		createStatic.mGameObect = obj;
	//		NULL_CHECK_BREAK(createStatic.mTrnasform);
	//		colStatic->Set_ColiiderDesc(createStatic);
	//	}		

	//	IMGUITREE_END
	//}



	//ImGui::Separator();


	//IMGUITREE("PHYSX_DYMAMIC")
	//{
	//	// dynamic 오브젝트 배치
	//	if (ImGui::Button("Dynamic_Box"))
	//	{

	//		FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer
	//		(g_pGameInstance->Get_NowSceneNum(), layerDynamic, TAG_OP(Prototype_Object_Dynamic_PhysX)));
	//		CTestObject_PhysX* obj =
	//			static_cast<CTestObject_PhysX*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(g_pGameInstance->Get_NowSceneNum(), layerDynamic));

	//		obj->Set_ColSetID(E_PHYTYPE_DYNAMIC);
	//		obj->Set_ModelSetting(CTestObject_PhysX::MODEL_GEMETRY);
	//		CCollider_PhysX_Dynamic* coldynamic = (CCollider_PhysX_Dynamic*)obj->Get_Component(TAG_COM(Com_Collider_PhysX));


	//		CTransform* objTrans = (CTransform*)obj->Get_Component(TAG_COM(Com_Transform));
	//		objTrans->Set_MatrixState(CTransform::STATE_POS, Position);
	//		objTrans->Scaled_All(Scale);

	//		createDynamic.eShapeType = E_GEOMAT_BOX;
	//		createDynamic.mTrnasform = objTrans;
	//		createDynamic.mGameObect = obj;
	//		NULL_CHECK_BREAK(createDynamic.mTrnasform);
	//		createDynamic.mVelocity = Force;
	//		coldynamic->Set_ColiiderDesc(createDynamic);
	//	}

	//	ImGui::SameLine();

		//if (ImGui::Button("Dynamic_Sphere"))
		//{
		//	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer
		//	(g_pGameInstance->Get_NowSceneNum(), layerDynamic, TAG_OP(Prototype_Object_Dynamic_PhysX)));
		//	CTestObject_PhysX* obj =
		//		static_cast<CTestObject_PhysX*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(g_pGameInstance->Get_NowSceneNum(), layerDynamic));

		//	obj->Set_ColSetID(E_PHYTYPE_DYNAMIC);
		//	obj->Set_ModelSetting(CTestObject_PhysX::MODEL_GEMETRY);
		//	CCollider_PhysX_Dynamic* coldynamic = (CCollider_PhysX_Dynamic*)obj->Get_Component(TAG_COM(Com_Collider_PhysX));


		//	CTransform* objTrans = (CTransform*)obj->Get_Component(TAG_COM(Com_Transform));
		//	objTrans->Set_MatrixState(CTransform::STATE_POS, Position);
		//	objTrans->Scaled_All(Scale);

		//	createDynamic.eShapeType = E_GEOMAT_SPEHE;
		//	createDynamic.mTrnasform = objTrans;
		//	createDynamic.mGameObect = obj;
		//	NULL_CHECK_BREAK(createDynamic.mTrnasform);
		//	createDynamic.mVelocity = Force;
		//	coldynamic->Set_ColiiderDesc(createDynamic);
		//}


	//	
	//	IMGUITREE_END
	//}


	//IMGUITREE("PHYSX_DYMAMIC_SET")
	//{
	//	static CTestObject_PhysX* playerDynamic = nullptr;
	//	static CCollider_PhysX_Dynamic* playerDynamicCol = nullptr;


	//	if (ImGui::Button("Dynamic_Player"))
	//	{
	//		if (playerDynamic == nullptr)
	//		{
	//			FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer
	//			(g_pGameInstance->Get_NowSceneNum(), layerDynamic, TAG_OP(Prototype_Object_Dynamic_PhysX)));
	//			playerDynamic =
	//				static_cast<CTestObject_PhysX*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(g_pGameInstance->Get_NowSceneNum(), layerDynamic));

	//			playerDynamic->Set_ColSetID(E_PHYTYPE_DYNAMIC);
	//			playerDynamic->Set_ModelSetting(CTestObject_PhysX::MODEL_PLAYER);
	//			playerDynamicCol = (CCollider_PhysX_Dynamic*)playerDynamic->Get_Component(TAG_COM(Com_Collider_PhysX));

	//			CTransform* objTrans = (CTransform*)playerDynamic->Get_Component(TAG_COM(Com_Transform));
	//			objTrans->Set_MatrixState(CTransform::STATE_POS, Position);
	//			objTrans->Scaled_All(Scale);

	//			createDynamic.eShapeType = E_GEOMAT_CAPSULE;
	//			createDynamic.mTrnasform = objTrans;
	//			createDynamic.mGameObect = playerDynamic;
	//			NULL_CHECK_BREAK(createDynamic.mTrnasform);
	//			createDynamic.mVelocity = Force;

	//			playerDynamicCol->Set_ColiiderDesc(createDynamic);
	//			playerDynamicCol->Set_RigidbodyFlag(PxRigidBodyFlag::Enum::eKINEMATIC, true);
	//			playerDynamicCol->Set_DynamicLock(PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, true);
	//			playerDynamicCol->Set_DynamicLock(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, true);
	//			playerDynamicCol->Set_DynamicLock(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, true);

	//			playerDynamicCol->Set_RigidbodyFlag(PxRigidBodyFlag::Enum::eUSE_KINEMATIC_TARGET_FOR_SCENE_QUERIES, true);
	//			playerDynamicCol->Set_RigidbodyFlag(PxRigidBodyFlag::Enum::eFORCE_STATIC_KINE_NOTIFICATIONS, true);
	//		}
	//	}
	//	IMGUITREE_END
	//}

#pragma endregion PHYSX UI

	ImGui::Separator();

	IMGUITREE("PHYSX_JOINT")
	{

		// Joint Test
		if (ImGui::Button("Joint_HairBoneSphere"))
		{

			 CCollider_PhysX_Base::PHYSXDESC_JOINT  createJoint;
			//	skd_hair01 skd_hair02 skd_hair03 skd_hair04 skd_hair05 skd_hair06 skd_hair07 skd_hairEnd
			FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer
			(g_pGameInstance->Get_NowSceneNum(), layerDynamic, TAG_OP(Prototype_Object_Joint_PhysX)));
			CTestObject_PhysX* obj =
				static_cast<CTestObject_PhysX*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(g_pGameInstance->Get_NowSceneNum(), layerDynamic));

			obj->Set_ColSetID(E_PHYTYPE_JOINT);
			obj->Set_ModelSetting(CTestObject_PhysX::MODEL_PLAYER);

			const _uint length = 9;
			string mBoneNames[length] =
			{
				"skd_head",
				"skd_hair01",
				"skd_hair02", "skd_hair03",
				"skd_hair04","skd_hair05", 
				"skd_hair06",
				"skd_hair07", "skd_hairEnd"
			};

			
			CCollider_PhysX_Joint* coljoint = (CCollider_PhysX_Joint*)obj->Get_Component(TAG_COM(Com_Collider_PhysX));
			CTransform* objTrans = (CTransform*)obj->Get_Component(TAG_COM(Com_Transform));
			CModel* objModel = (CModel*)obj->Get_Component(TAG_COM(Com_Model));
			objTrans->Set_MatrixState(CTransform::STATE_POS, Position);

			createJoint.mBoneNames = mBoneNames;
			createJoint.mLength = length;
			createJoint.mGameObject = obj;
			createJoint.eShapeType = E_GEOMAT_SPEHE;
			createJoint.mMainScale = MainActorScale;
			createJoint.mActorScale = Scale;
			createJoint.mSeparation = Force.x;
			createJoint.mAttachModel = objModel;

		//	coljoint->Set_ColiiderDesc2(createJoint);
			coljoint->Set_ColiderDesc_Hair(createJoint);

		}

		ImGui::SameLine();

		//if (ImGui::Button("Joint_Demo"))
		//{
		//	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer
		//	(g_pGameInstance->Get_NowSceneNum(), layerDynamic, TAG_OP(Prototype_Object_Joint_PhysX)));
		//	CTestObject_PhysX* obj =
		//		static_cast<CTestObject_PhysX*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(g_pGameInstance->Get_NowSceneNum(), layerDynamic));
		//
		//	obj->Set_ColSetID(E_PHYTYPE_JOINT);
		//	obj->Set_ModelSetting(CTestObject_PhysX::MODEL_GEMETRY);
		//
		//	CCollider_PhysX_Joint* coljoint = (CCollider_PhysX_Joint*)obj->Get_Component(TAG_COM(Com_Collider_PhysX));
		//	CTransform* objTrans = (CTransform*)obj->Get_Component(TAG_COM(Com_Transform));
		//	CModel* objModel = (CModel*)obj->Get_Component(TAG_COM(Com_Model));
		//	objTrans->Set_MatrixState(CTransform::STATE_POS, Position);
		//	objTrans->Scaled_All(Scale);
		//	coljoint->Set_ColiderDesc_(PxTransform(Position.x, Position.y, Position.z),
		//		PxVec3(Scale.x,Scale.y,Scale.z), Force.x,obj);
		//}
		//
		//ImGui::SameLine();
		//
		//if (ImGui::Button("Joint_Demo_Default"))
		//{
		//	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer
		//	(g_pGameInstance->Get_NowSceneNum(), layerDynamic, TAG_OP(Prototype_Object_Joint_PhysX)));
		//	CTestObject_PhysX* obj =
		//		static_cast<CTestObject_PhysX*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(g_pGameInstance->Get_NowSceneNum(), layerDynamic));
		//
		//	obj->Set_ColSetID(E_PHYTYPE_JOINT);
		//	obj->Set_ModelSetting(CTestObject_PhysX::MODEL_GEMETRY);
		//
		//	CCollider_PhysX_Joint* coljoint = (CCollider_PhysX_Joint*)obj->Get_Component(TAG_COM(Com_Collider_PhysX));
		//	CTransform* objTrans = (CTransform*)obj->Get_Component(TAG_COM(Com_Transform));
		//	CModel* objModel = (CModel*)obj->Get_Component(TAG_COM(Com_Model));
		//	objTrans->Set_MatrixState(CTransform::STATE_POS, Position);
		//	objTrans->Scaled_All(Scale);
		//	coljoint->Set_ColiderDesc_(PxTransform(Position.x, Position.y, Position.z),
		//		PxVec3(2.0f, 0.5f, 0.5f), 1.0f, obj);
		//}

		IMGUITREE_END
	}
	


	ImGui::Separator();

	//IMGUITREE("BaseCollision")
	//{
	//	static CTestObject_BaseCollider* COllision_TestObj = nullptr;
	//	if (ImGui::Button("CollisionTest"))
	//	{
	//		//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer
	//		//(g_pGameInstance->Get_NowSceneNum(), layerDynamic, TAG_OP(Prototype_Object_BaseCollision)));
	//		//CTestObject_PhysX* obj =
	//		//	static_cast<CTestObject_PhysX*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(g_pGameInstance->Get_NowSceneNum(), layerDynamic));
	//	}
	//	IMGUITREE_END
	//}

	if (ImGui::Button("Delete_Static"))
	{
		auto vecDelete = g_pGameInstance->Get_ObjectList_from_Layer(g_pGameInstance->Get_NowSceneNum(), layerStatic);
		for (auto obj : *vecDelete)
		{
			obj->Set_IsDead();
		}
	}
	ImGui::SameLine();

	if (ImGui::Button("Delete_Dynamic"))
	{
		auto vecDelete = g_pGameInstance->Get_ObjectList_from_Layer(g_pGameInstance->Get_NowSceneNum(), layerDynamic);
		for (auto obj : *vecDelete)
		{
			obj->Set_IsDead();
		}
	}
	ImGui::Separator();

	ImGui::DragFloat3("DebugValue1:", (float*)&GetSingle(CPhysXMgr)->gDebugValue1, 0.1f, -1000, 1000);
	ImGui::DragFloat3("DebugValue2:", (float*)&GetSingle(CPhysXMgr)->gDebugValue2, 0.1f, -1000, 1000);
	ImGui::DragFloat3("DebugValue3:", (float*)&GetSingle(CPhysXMgr)->gDebugValue3, 0.1f, -1000, 1000);
	ImGui::DragFloat3("DebugValue4:", (float*)&GetSingle(CPhysXMgr)->gDebugValue4, 0.1f, -1000, 1000);

	return _int();
}

_int CImguiMgr::Update_DebugWnd_Sound3D(_double fDeltaTime)
{
	static _float3 PlayPostiton1 = _float3::Zero();
	ImGui::DragFloat3("PlayPostiton1:", (float*)&PlayPostiton1, 0.1f, -100, 100);
	static  _float3 PlayPostiton2 = _float3::Zero();
	ImGui::DragFloat3("PlayPostiton2:", (float*)&PlayPostiton2, 0.1f, -100, 100);

	static TCHAR* str_DemoSoundFile = L"3DTestSound2.wav";

	static const wchar_t* layerStatic = TAG_LAY(Layer_ColStatic);
	static const wchar_t* layerDynamic = TAG_LAY(Layer_ColDynamic);
	static const wchar_t* layerTrigger = TAG_LAY(Layer_ColTrigger);

	static CTestObject_PhysX* PlayerSoundObject = nullptr;

	if (ImGui::Button("Create_SoundPlayer"))
	{
		FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer
		(g_pGameInstance->Get_NowSceneNum(), layerStatic, TAG_OP(Prototype_Object_Static_PhysX)));
		PlayerSoundObject =
			static_cast<CTestObject_PhysX*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(g_pGameInstance->Get_NowSceneNum(), layerStatic));
		PlayerSoundObject->Set_ModelSetting(CTestObject_PhysX::MODEL_PLAYER);
		PlayerSoundObject->Set_TYPE(CTestObject_PhysX::TESTTYPE_SOUND);
		CTransform* objTrans = (CTransform*)PlayerSoundObject->Get_Component(TAG_COM(Com_Transform));
		objTrans->Set_MatrixState(CTransform::STATE_POS, PlayPostiton1);
		NULL_CHECK_BREAK(PlayerSoundObject);
		NULL_CHECK_BREAK(objTrans);


	}

	if (PlayerSoundObject)
	{
		static FMOD_SYSTEM* SoundSystem = GetSingle(CGameInstance)->Get_SOUNDSYSTEM();

		// 3D Sound
		int iListener=0;
		float fDopperScale, fDistance, fRollScale;

		FMOD_System_Get3DSettings(SoundSystem, &fDopperScale, &fDistance,&fRollScale);
		FMOD_System_Get3DNumListeners(SoundSystem, &iListener);
	
		ImGui::Text("3DSetting:(%.2f,%.2f,%.2f,)", fDopperScale, fDistance, fRollScale);
		ImGui::Text("3DListener:(%d)", iListener);

		// Scale 
		_float MinusValue = fRollScale;
		ImGui::DragFloat("DopperScale Distance RollScale:", &MinusValue, 0.1f, 0.01f, 1000);

		GetSingle(CGameInstance)->Set_3DSound_DistanceMinValue(MinusValue);

		// #3DSOUND

		if (ImGui::Button("StartSound"))
		{
			GetSingle(CGameInstance)->PlaySound(str_DemoSoundFile, CHANNEL_EFFECT, 1.0f);

		}

		if (ImGui::Button("Start3DSound"))
		{
			GetSingle(CGameInstance)->Play3D_Sound(str_DemoSoundFile,_float3(0,0,0), CHANNEL_EFFECT, 1.0f);
		}
		if (ImGui::Button("Start3DSound_Move"))
		{

			/*
			velx = (posx-lastposx) * 1000 / timedelta;
			velz = (posy-lastposy) * 1000 / timedelta;
			velz = (posz-lastposz) * 1000 / timedelta;

			*/
			GetSingle(CGameInstance)->Play3D_Sound(str_DemoSoundFile, _float3(0, 0, 0), CHANNEL_EFFECT, 1.0f,_float3(1,0,0),1000);
		}

	}
	return _int();
}

_int CImguiMgr::Update_DebugWnd_EffectTest(_double fDeltaTime)
{
	
	static _float3 CreatePos = _float3(0, 0, 0);
	ImGui::DragFloat3("CreatePos:", (float*)&CreatePos, 0.1f, -100, 100);
	static const wchar_t* layer_Monster = TAG_LAY(Layer_Monster);
	static const wchar_t* layer_Boss = TAG_LAY(Layer_Boss);
	

	if (ImGui::Button("Create_Prototype_Object_Monster_Mahinasura_Minion"))
	{		
		CGameObject* monsterobj =  (g_pGameInstance->Add_GameObject_GetObject
		(g_pGameInstance->Get_NowSceneNum(), layer_Monster, TAG_OP(Prototype_Object_Monster_Mahinasura_Minion),&CreatePos));

		CTransform* transform = (CTransform*)static_cast<CMonster_Mahinasura_Minion*>(monsterobj)->Get_Component(TAG_COM(Com_Transform));
		transform->Set_MatrixState(CTransform::STATE_POS, _float3(CreatePos));
	}

	if (ImGui::Button("Create_Prototype_Object_Monster_Mahinasura_Leader"))
	{
		CGameObject* monsterobj = (g_pGameInstance->Add_GameObject_GetObject
		(g_pGameInstance->Get_NowSceneNum(), layer_Monster, TAG_OP(Prototype_Object_Monster_Mahinasura_Leader), &CreatePos));

		CTransform* transform = (CTransform*)static_cast<CMonster_Mahinasura_Minion*>(monsterobj)->Get_Component(TAG_COM(Com_Transform));
		transform->Set_MatrixState(CTransform::STATE_POS, _float3(CreatePos));
	}

	//if (ImGui::Button("Create_Prototype_Object_Monster_Vayusura_Minion"))
	//{
	//	CGameObject* monsterobj = (g_pGameInstance->Add_GameObject_GetObject
	//	(g_pGameInstance->Get_NowSceneNum(), layer_Monster, TAG_OP(Prototype_Object_Monster_Vayusura_Minion), &CreatePos));

	//	CTransform* transform = (CTransform*)static_cast<CMonster_Mahinasura_Minion*>(monsterobj)->Get_Component(TAG_COM(Com_Transform));
	//	transform->Set_MatrixState(CTransform::STATE_POS, _float3(CreatePos));
	//}


	//if (ImGui::Button("Create_Prototype_Object_Monster_Vayusura_Leader"))
	//{
	//	CGameObject* monsterobj = (g_pGameInstance->Add_GameObject_GetObject
	//	(g_pGameInstance->Get_NowSceneNum(), layer_Monster, TAG_OP(Prototype_Object_Monster_Vayusura_Leader), &CreatePos));

	//	CTransform* transform = (CTransform*)static_cast<CMonster_Mahinasura_Minion*>(monsterobj)->Get_Component(TAG_COM(Com_Transform));
	//	transform->Set_MatrixState(CTransform::STATE_POS, _float3(CreatePos));
	//}


	//if (ImGui::Button("Create_Prototype_Object_Monster_Ninjasura_Minion"))
	//{
	//	CGameObject* monsterobj = (g_pGameInstance->Add_GameObject_GetObject
	//	(g_pGameInstance->Get_NowSceneNum(), layer_Monster, TAG_OP(Prototype_Object_Monster_Ninjasura_Minion), &CreatePos));

	//	CTransform* transform = (CTransform*)static_cast<CMonster_Mahinasura_Minion*>(monsterobj)->Get_Component(TAG_COM(Com_Transform));
	//	transform->Set_MatrixState(CTransform::STATE_POS, _float3(CreatePos));
	//}
	//if (ImGui::Button("Create_Prototype_Object_Monster_Ninjasura"))
	//{
	//	CGameObject* monsterobj = (g_pGameInstance->Add_GameObject_GetObject
	//	(g_pGameInstance->Get_NowSceneNum(), layer_Monster, TAG_OP(Prototype_Object_Monster_Ninjasura), &CreatePos));

	//	CTransform* transform = (CTransform*)static_cast<CMonster_Mahinasura_Minion*>(monsterobj)->Get_Component(TAG_COM(Com_Transform));
	//	transform->Set_MatrixState(CTransform::STATE_POS, _float3(CreatePos));
	//}


	if (ImGui::Button("Create_Prototype_Object_Monster_Gadasura_Black"))
	{
		CGameObject* monsterobj = (g_pGameInstance->Add_GameObject_GetObject
		(g_pGameInstance->Get_NowSceneNum(), layer_Monster, TAG_OP(Prototype_Object_Monster_Gadasura_Black), &CreatePos));

		CTransform* transform = (CTransform*)static_cast<CMonster_Gadasura_Black*>(monsterobj)->Get_Component(TAG_COM(Com_Transform));
		transform->Set_MatrixState(CTransform::STATE_POS, _float3(CreatePos));
	}


	if (ImGui::Button("Create_Prototype_Object_Monster_Gadasura_Rage"))
	{
		CGameObject* monsterobj = (g_pGameInstance->Add_GameObject_GetObject
		(g_pGameInstance->Get_NowSceneNum(), layer_Monster, TAG_OP(Prototype_Object_Monster_Gadasura_Rage), &CreatePos));

		CTransform* transform = (CTransform*)static_cast<CMonster_Gadasura_Rage*>(monsterobj)->Get_Component(TAG_COM(Com_Transform));
		transform->Set_MatrixState(CTransform::STATE_POS, _float3(CreatePos));
	}



	// BOSS


	/*
	Prototype_Object_Boss_Rangda,
	Prototype_Object_Boss_Rangda_Finger,
	Prototype_Object_Boss_Chiedtian,
	Prototype_Object_Boss_ChiedtianWeapon,
	Prototype_Object_Boss_Snake,
	Prototype_Object_Boss_Mahabalasura,
	Prototype_Object_Boss_MahabalasuraArms,
	Prototype_Object_Boss_MahabalasuraAttackArms,
	Prototype_Object_Boss_MahabalasuraWeapon,
	Prototype_Object_Boss_MahabalasuraCopy,
	*/

	if (ImGui::Button("Create_BOSS_Snake"))
	{
		CGameObject* monsterobj = (g_pGameInstance->Add_GameObject_GetObject
		(g_pGameInstance->Get_NowSceneNum(), layer_Boss, TAG_OP(Prototype_Object_Boss_Snake), &CreatePos));

		CTransform* transform = (CTransform*)static_cast<CMonster_Mahinasura_Minion*>(monsterobj)->Get_Component(TAG_COM(Com_Transform));
		transform->Set_MatrixState(CTransform::STATE_POS, _float3(CreatePos));

	}

	if (ImGui::Button("Create_BOSS_Prototype_Object_Boss_Mahabalasura"))
	{
		CGameObject* monsterobj = (g_pGameInstance->Add_GameObject_GetObject
		(g_pGameInstance->Get_NowSceneNum(), layer_Boss, TAG_OP(Prototype_Object_Boss_Mahabalasura), &CreatePos));

		CTransform* transform = (CTransform*)static_cast<CMonster_Mahinasura_Minion*>(monsterobj)->Get_Component(TAG_COM(Com_Transform));
		transform->Set_MatrixState(CTransform::STATE_POS, _float3(CreatePos));
	}


	if (ImGui::Button("Create_BOSS_Prototype_Object_Boss_Rngda"))
	{
		CGameObject* monsterobj = (g_pGameInstance->Add_GameObject_GetObject
		(g_pGameInstance->Get_NowSceneNum(), layer_Boss, TAG_OP(Prototype_Object_Boss_Rangda), &CreatePos));

		CTransform* transform = (CTransform*)static_cast<CMonster_Mahinasura_Minion*>(monsterobj)->Get_Component(TAG_COM(Com_Transform));
		transform->Set_MatrixState(CTransform::STATE_POS, _float3(CreatePos));
	}



	//if (ImGui::Button("Create_BOSS_Chiedtian"))
	//{
	//	CGameObject* monsterobj = (g_pGameInstance->Add_GameObject_GetObject
	//	(g_pGameInstance->Get_NowSceneNum(), layer_Boss, TAG_OP(Prototype_Object_Boss_Chiedtian), &CreatePos));

	//	CTransform* transform = (CTransform*)static_cast<CMonster_Mahinasura_Minion*>(monsterobj)->Get_Component(TAG_COM(Com_Transform));
	//	transform->Set_MatrixState(CTransform::STATE_POS, _float3(CreatePos));
	//}


	//if (ImGui::Button("Create_BOSS_Rangda"))
	//{
	//	CGameObject* monsterobj = (g_pGameInstance->Add_GameObject_GetObject
	//	(g_pGameInstance->Get_NowSceneNum(), layer_Boss, TAG_OP(Prototype_Object_Boss_Rangda), &CreatePos));

	//	CTransform* transform = (CTransform*)static_cast<CMonster_Mahinasura_Minion*>(monsterobj)->Get_Component(TAG_COM(Com_Transform));
	//	transform->Set_MatrixState(CTransform::STATE_POS, _float3(CreatePos));
	//}

	

	if (ImGui::Button("Delete_Monster"))
	{
		auto vecDelete_Mon = g_pGameInstance->Get_ObjectList_from_Layer(g_pGameInstance->Get_NowSceneNum(), layer_Monster);
		
		for (auto obj : *vecDelete_Mon)
		{
			obj->Set_IsDead();
		}
	}

	ImGui::SameLine();
	if (ImGui::Button("Delete_Boss"))
	{
		auto vecDelete_Bos = g_pGameInstance->Get_ObjectList_from_Layer(g_pGameInstance->Get_NowSceneNum(), layer_Boss);

		for (auto obj : *vecDelete_Bos)
		{
			obj->Set_IsDead();
		}

	}

	

	return S_OK;
	
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