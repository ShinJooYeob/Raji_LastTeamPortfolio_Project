#include "stdafx.h"
#include "..\public\MainApp.h"
#include "Scene_Loading.h"
#include "Camera_Main.h"
#include "Model.h"
#include "UtilityMgr.h"
#include "FIleIO/AssimpMgr.h"
#include "ImguiMgr.h"

CMainApp::CMainApp()
	:m_pGameInstance(GetSingle(CGameInstance))
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApp::Initialize()
{
	if (m_pGameInstance == nullptr)
		return E_FAIL;

	CGraphic_Device::GRAPHICDESC		GraphicDesc;
	ZeroMemory(&GraphicDesc, sizeof(CGraphic_Device::GRAPHICDESC));

	GraphicDesc.hWnd = g_hWnd;
	GraphicDesc.eWinMode = CGraphic_Device::MODE_WIN;
	GraphicDesc.iWinCX = g_iWinCX;
	GraphicDesc.iWinCY = g_iWinCY;

	//ShowCursor(false);


	FAILED_CHECK(m_pGameInstance->Initialize_Engine(g_hInst, GraphicDesc, SCENEID::SCENE_END, &m_pDevice, &m_pDeviceContext,&m_pBackBufferRTV,&m_pDepthStencilView,&m_pSwapChain));

	FAILED_CHECK(Ready_Static_Component_Prototype());

	FAILED_CHECK(Ready_Static_GameObject_Prototype());

	FAILED_CHECK(Ready_SingletonMgr());

	FAILED_CHECK(Scene_Change(SCENEID::SCENE_LOBY));




	FAILED_CHECK(g_pGameInstance->Add_Font(L"VinerFonts", L"../bin/Resources/Fonts/Reenie.spritefont"));

	return S_OK;
}

_int CMainApp::Update(_double fDeltaTime)
{
	if (m_pGameInstance == nullptr)
		return -1;

	if (m_bIsSlowed)
		Update_SlowMotion(fDeltaTime);


	g_fDeltaTime = fDeltaTime * m_SlowTimes;



	if (FAILED(m_pGameInstance->Update_Engine(fDeltaTime * m_SlowTimes)))
	{
		__debugbreak();
		MSGBOX("Failed to Update_Engine ");
		return E_FAIL;
	}

	if (FAILED(m_pGameInstance->LateUpdate_Engine(fDeltaTime * m_SlowTimes)))
	{
		__debugbreak();
		MSGBOX("Failed to LateUpdate_Engine ");
		return E_FAIL;

	}

	return 0;
}

_double CMainApp::Update_SlowMotion(_double fDeltaTime)
{
	m_fPassedTime += _float(fDeltaTime);

#define SLOWMOTIONTURNINGPOINT 0.0f

	if (m_fPassedTime < m_fTargetTime * SLOWMOTIONTURNINGPOINT)
		m_SlowTimes = m_pGameInstance->Easing(TYPE_QuarticOut, 1, m_fTargetSpeed, m_fPassedTime, m_fTargetTime *SLOWMOTIONTURNINGPOINT);
	else
		m_SlowTimes = m_pGameInstance->Easing(TYPE_QuarticIn, m_fTargetSpeed, 1, m_fPassedTime - m_fTargetTime *SLOWMOTIONTURNINGPOINT, m_fTargetTime * (1 - SLOWMOTIONTURNINGPOINT));

	if (m_fPassedTime > m_fTargetTime)
	{
		m_bIsSlowed = false;
		m_SlowTimes = 1;
	}

	return m_SlowTimes;
}

HRESULT CMainApp::Render()
{
	if (m_pGameInstance == nullptr)
	{
		DEBUGBREAK;
		return E_FAIL;
	}



	FAILED_CHECK(m_pGameInstance->Clear_BackBuffer_View(_float4(0.5f, 0.5f, 0.5f, 1.f)));
	FAILED_CHECK(m_pGameInstance->Clear_DepthStencil_View());

	FAILED_CHECK(m_pComRenderer->Render_RenderGroup(g_fDeltaTime));
	FAILED_CHECK(m_pGameInstance->Render_Scene());

	FAILED_CHECK(m_pGameInstance->Present());


#ifdef _DEBUG
	if (m_pGameInstance->Get_DIKeyState(DIK_F7)& DIS_Down) g_bIsShowFPS = !g_bIsShowFPS;
	if (g_bIsShowFPS)
	{
		++m_dwNumRender;
		m_dTimerAcc += g_fDeltaTime;

		if (m_dTimerAcc >= 1.f)
		{
			wsprintf(m_szFPS, TEXT("FPS : %d"), m_dwNumRender);
			SetWindowText(g_hWnd, m_szFPS);

			m_dwNumRender = 0;
			m_dTimerAcc = 0.f;
		}
	}
#endif // _DEBUG


	return S_OK;

}

void CMainApp::SlowMotionStart(_float fTargetTime , _float TargetSpeed )
{
	if (m_bIsSlowed)
		return;

	m_fTargetSpeed = TargetSpeed;
	m_fTargetTime = fTargetTime;
	m_fPassedTime = 0;

	m_bIsSlowed = true;

}



HRESULT CMainApp::Scene_Change(SCENEID eSceneID)
{
	if (m_pGameInstance == nullptr)
		return E_FAIL;



	switch (eSceneID)
	{
	case SCENEID::SCENE_LOBY:
	case SCENEID::SCENE_STAGESELECT:
	case SCENEID::SCENE_STAGE1:
	case SCENEID::SCENE_STAGE2:
	case SCENEID::SCENE_STAGE3:
	case SCENEID::SCENE_STAGE4:
	case SCENEID::SCENE_STAGE5:
	case SCENEID::SCENE_STAGE6:
	case SCENEID::SCENE_STAGE7:
	case SCENEID::SCENE_BOSS:
	case SCENEID::SCENE_EDIT:

		FAILED_CHECK(GetSingle(CUtilityMgr)->Clear_RenderGroup_forSceneChange());
		m_pGameInstance->Scene_Change(CScene_Loading::Create(m_pDevice, m_pDeviceContext, eSceneID), SCENEID::SCENE_LOADING);

		break;

	default:
		return E_FAIL;
		break;
	}

	return S_OK;
}


HRESULT CMainApp::Ready_SingletonMgr()
{

#ifdef _DEBUG
	FAILED_CHECK(GetSingle(CImguiMgr)->Initialize_ImguiMgr(m_pDevice, m_pDeviceContext, m_pBackBufferRTV, m_pDepthStencilView, m_pSwapChain));
#endif // _DEBUG

//	FAILED_CHECK(GetSingle(CUtilityMgr)->Initialize_UtilityMgr());
	FAILED_CHECK(GetSingle(CAssimpMgr)->Initialize(m_pDevice,m_pDeviceContext));

	
	return S_OK;
}

HRESULT CMainApp::Free_SingletonMgr()
{

#ifdef _DEBUG
	if (0 != GetSingle(CImguiMgr)->DestroyInstance())
	{
		MSGBOX("Failed to Release CImguiMgr");
		return E_FAIL;
	}

#endif // USE_IMGUI
	if (0 != GetSingle(CUtilityMgr)->DestroyInstance())
	{
		MSGBOX("Failed to Release CUtilityMgr");
		return E_FAIL;
	}
	if (0 != GetSingle(CAssimpMgr)->DestroyInstance())
	{
		MSGBOX("Failed to Release CUtilityMgr");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CMainApp::Ready_Static_Component_Prototype()
{
	if (m_pGameInstance == nullptr)
		return E_FAIL;
	

	//렌더러 컴객체 프로토타입 생성
	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Renderer), 
		m_pComRenderer = CRenderer::Create(m_pDevice,m_pDeviceContext)));
	Safe_AddRef(m_pComRenderer);
	GetSingle(CUtilityMgr)->Set_Renderer(m_pComRenderer);

	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Transform),
		CTransform::Create(m_pDevice, m_pDeviceContext)));

	////버퍼인덱스 프로토타입 생성
	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Rect),
		CVIBuffer_Rect::Create(m_pDevice, m_pDeviceContext)));

	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Cube),
		CVIBuffer_Cube::Create(m_pDevice, m_pDeviceContext)));

	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Point_Instance),
		CVIBuffer_Point_Instance::Create(m_pDevice, m_pDeviceContext,15)));

	////쉐이더
	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Shader_VT),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("Shader_VtxTex.hlsl"), VTXTEX_DECLARATION::Elements, VTXTEX_DECLARATION::iNumElements)));

	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Shader_VCT),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("Shader_VtxCubeTex.hlsl"), VTXCUBETEX_DECLARATION::Elements, VTXCUBETEX_DECLARATION::iNumElements)));

	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Shader_VNT),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("Shader_VtxNorTex.hlsl"), VTXNORMTEX_DECLARATION::Elements, VTXNORMTEX_DECLARATION::iNumElements)));

	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Shader_VNAM),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("Shader_NonAnimModel.hlsl"), VTXMODEL_DECLARATION::Elements, VTXMODEL_DECLARATION::iNumElements)));

	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Shader_VAM),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("Shader_AnimModel.hlsl"), VTXANIMMODEL_DECLARATION::Elements, VTXANIMMODEL_DECLARATION::iNumElements)));

	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Shader_VTXPOINTINST),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("Shader_VtxPointInstance.hlsl"), VTXPOINT_INSTMAT_DECLARATION::Elements, VTXPOINT_INSTMAT_DECLARATION::iNumElements)));


	////텍스처 프로토타입 생성
	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Texture_DefaultUI),
		CTexture::Create(m_pDevice, m_pDeviceContext, L"UI_Default.txt" )));


	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Collider),
		CCollider::Create(m_pDevice, m_pDeviceContext)));


	return S_OK;
}

HRESULT CMainApp::Ready_Static_GameObject_Prototype()
{
	///////////////////Camera_Main 프로토타입 생성
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
	CameraDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(60.0f);
	CameraDesc.TransformDesc.fScalingPerSec = 1.f;


	// Assimp의 메인입니다.
	FAILED_CHECK(m_pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_Camera_Main), CCamera_Main::Create(m_pDevice, m_pDeviceContext, &CameraDesc)));
//	FAILED_CHECK(m_pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_SkyBox), CSkyBox::Create(m_pDevice, m_pDeviceContext)));
	

	return S_OK;
}

CMainApp * CMainApp::Create()
{
	CMainApp* pInstance = NEW CMainApp;

	if (FAILED(pInstance->Initialize()))
	{
		MSGBOX("Failed to Creating CMainApp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainApp::Free()
{
	m_pGameInstance->Get_NowScene()->Free();

	if (FAILED(Free_SingletonMgr()))
	{
		__debugbreak();
		MSGBOX("Failed to Free Singleton");
	}

	Safe_Release(m_pComRenderer);

	Safe_Release(m_pSwapChain);
	Safe_Release(m_pDepthStencilView);
	Safe_Release(m_pBackBufferRTV);
	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
	Safe_Release(m_pGameInstance);
}

