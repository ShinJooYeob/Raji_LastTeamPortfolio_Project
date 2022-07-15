#include "stdafx.h"
#include "..\public\MainApp.h"
#include "Scene_Loading.h"
#include "Camera_Main.h"
#include "Model.h"
#include "UtilityMgr.h"
#include "AssimpCreateMgr.h"
#include "PhysX/PhysXMgr.h"
#include "TestObject_PhysX.h"
#include "InstanceEffect.h"
#include "MeshEffect.h"

#include "PhysX/Collider_PhysX_Static.h"
#include "PhysX/Collider_PhysX_Dynamic.h"
#include "PhysX/Collider_PhysX_Joint.h"
#include "HpUI.h"
//#include "LoadingUI.h"

#ifdef _DEBUG
#include "ImguiMgr.h"
#endif // USE_IMGUI
//#include "UtilityMgr.h"

 
#include "SkyBox.h"


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


	// 업데이트 -> 충돌 시물레이션 -> 레이트 업데이트
	if (FAILED(m_pGameInstance->Update_Engine(fDeltaTime * m_SlowTimes)))
	{
		__debugbreak();
		MSGBOX("Failed to Update_Engine ");
		return E_FAIL;
	}

#pragma region PhysX_Update
	if (FAILED(GetSingle(CPhysXMgr)->Update_PhysX(fDeltaTime * m_SlowTimes)))
	{
		__debugbreak();
		MSGBOX("Failed to Update_PhysX ");
		return E_FAIL;
	}

	if (FAILED(GetSingle(CPhysXMgr)->LateUpdate_PhysX(fDeltaTime * m_SlowTimes)))
	{
		__debugbreak();
		MSGBOX("Failed to LateUpdate_PhysX ");
		return E_FAIL;
	}
#pragma endregion PhysX_Update


#ifdef _DEBUG
	FAILED_CHECK(m_pImguiMgr->Update_DebugWnd(fDeltaTime));
#endif // _DEBUG
	if (FAILED(m_pGameInstance->LateUpdate_Engine(fDeltaTime * m_SlowTimes)))
	{
		__debugbreak();
		MSGBOX("Failed to LateUpdate_Engine ");
		return E_FAIL;

	}
#ifdef _DEBUG
	FAILED_CHECK(m_pImguiMgr->LateUpdate_DebugWnd(fDeltaTime));
#endif // _DEBUG


	return 0;
}

_double CMainApp::Update_SlowMotion(_double fDeltaTime)
{
	m_fPassedTime += _float(fDeltaTime);


	if (m_fPassedTime < m_fTargetTime * m_fTargetCurveRate)
		m_SlowTimes = m_pGameInstance->Easing(TYPE_QuarticOut, 1, m_fTargetSpeed, m_fPassedTime, m_fTargetTime *m_fTargetCurveRate);
	else
		m_SlowTimes = m_pGameInstance->Easing(TYPE_QuarticIn, m_fTargetSpeed, 1, m_fPassedTime - m_fTargetTime *m_fTargetCurveRate, m_fTargetTime * (1 - m_fTargetCurveRate));

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
		__debugbreak();
		return E_FAIL;
	}

	FAILED_CHECK(m_pGameInstance->Clear_BackBuffer_View(_float4(0.5f, 0.5f, 0.5f, 1.f)));
	FAILED_CHECK(m_pGameInstance->Clear_DepthStencil_View());

	FAILED_CHECK(m_pComRenderer->Render_RenderGroup(g_fDeltaTime));
	FAILED_CHECK(m_pGameInstance->Render_Scene());
#ifdef _DEBUG
	FAILED_CHECK(m_pImguiMgr->Render_DebugWnd());
#endif // _DEBUG

	FAILED_CHECK(m_pGameInstance->Present());
	return S_OK;

}

void CMainApp::SlowMotionStart(_float fTargetTime , _float TargetSpeed, _float TargetCurveRate)
{
	if (m_bIsSlowed)
		return;

	m_fTargetSpeed = TargetSpeed;
	m_fTargetTime = fTargetTime;
	m_fTargetCurveRate = TargetCurveRate;
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

	FAILED_CHECK(GETIMGUI->Initialize_ImguiMgr(m_pDevice, m_pDeviceContext, m_pBackBufferRTV, m_pDepthStencilView, m_pSwapChain));
	m_pImguiMgr = GETIMGUI;
	NULL_CHECK_RETURN(m_pImguiMgr,E_FAIL);
	Safe_AddRef(m_pImguiMgr);

#endif // USE_IMGUI

	FAILED_CHECK(GetSingle(CUtilityMgr)->Initialize_UtilityMgr(m_pDevice, m_pDeviceContext, this));
	FAILED_CHECK(GetSingle(CAssimpCreateMgr)->Initalize(m_pDevice, m_pDeviceContext));
	FAILED_CHECK(GetSingle(CPhysXMgr)->Initialize_PhysX(m_pDevice, m_pDeviceContext));

	
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
	if (0 != GetSingle(CAssimpCreateMgr)->DestroyInstance())
	{
		MSGBOX("Failed to Release CAssimpCreateMgr");
		return E_FAIL;
	}

	if (0 != GetSingle(CPhysXMgr)->DestroyInstance())
	{
		MSGBOX("Failed to Release CPhysXMgr");
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
		m_pComRenderer = CRenderer::Create(m_pDevice, m_pDeviceContext)));
	Safe_AddRef(m_pComRenderer);
	GetSingle(CUtilityMgr)->Set_Renderer(m_pComRenderer);

	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Transform),
		CTransform::Create(m_pDevice, m_pDeviceContext)));

	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_MotionTrail),
		CMotionTrail::Create(m_pDevice, m_pDeviceContext)));
	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_SwordTrail),
		CSwordTrail::Create(m_pDevice, m_pDeviceContext, L"SwordTrail.txt", 20)));

	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Dissolve),
		CDissolve::Create(m_pDevice, m_pDeviceContext, L"Prototype_Texture_Dissolve")));



	////버퍼인덱스 프로토타입 생성
	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Rect),
		CVIBuffer_Rect::Create(m_pDevice, m_pDeviceContext)));

	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Cube),
		CVIBuffer_Cube::Create(m_pDevice, m_pDeviceContext)));


	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Point_Instance_1),
		CVIBuffer_Point_Instance::Create(m_pDevice, m_pDeviceContext, 1)));
	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Point_Instance_2),
		CVIBuffer_Point_Instance::Create(m_pDevice, m_pDeviceContext, 2)));
	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Point_Instance_4),
		CVIBuffer_Point_Instance::Create(m_pDevice, m_pDeviceContext, 4)));
	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Point_Instance_8),
		CVIBuffer_Point_Instance::Create(m_pDevice, m_pDeviceContext, 8)));
	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Point_Instance_16),
		CVIBuffer_Point_Instance::Create(m_pDevice, m_pDeviceContext, 16)));
	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Point_Instance_32),
		CVIBuffer_Point_Instance::Create(m_pDevice, m_pDeviceContext, 32)));
	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Point_Instance_64),
		CVIBuffer_Point_Instance::Create(m_pDevice, m_pDeviceContext, 64)));
	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Point_Instance_128),
		CVIBuffer_Point_Instance::Create(m_pDevice, m_pDeviceContext, 128)));
	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Point_Instance_256),
		CVIBuffer_Point_Instance::Create(m_pDevice, m_pDeviceContext, 256)));
	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Point_Instance_512),
		CVIBuffer_Point_Instance::Create(m_pDevice, m_pDeviceContext, 512)));



	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_ModelInstance_1),
		CModelInstance::Create(m_pDevice, m_pDeviceContext, SCENEID::SCENE_STATIC, 1)));
	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_ModelInstance_2),
		CModelInstance::Create(m_pDevice, m_pDeviceContext, SCENEID::SCENE_STATIC, 2)));
	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_ModelInstance_4),
		CModelInstance::Create(m_pDevice, m_pDeviceContext, SCENEID::SCENE_STATIC, 4)));
	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_ModelInstance_8),
		CModelInstance::Create(m_pDevice, m_pDeviceContext, SCENEID::SCENE_STATIC, 8)));
	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_ModelInstance_16),
		CModelInstance::Create(m_pDevice, m_pDeviceContext, SCENEID::SCENE_STATIC, 16)));
	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_ModelInstance_32),
		CModelInstance::Create(m_pDevice, m_pDeviceContext, SCENEID::SCENE_STATIC, 32)));
	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_ModelInstance_64),
		CModelInstance::Create(m_pDevice, m_pDeviceContext, SCENEID::SCENE_STATIC, 64)));
	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_ModelInstance_128),
		CModelInstance::Create(m_pDevice, m_pDeviceContext, SCENEID::SCENE_STATIC, 128)));
	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_ModelInstance_256),
		CModelInstance::Create(m_pDevice, m_pDeviceContext, SCENEID::SCENE_STATIC, 256)));
	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_ModelInstance_512),
		CModelInstance::Create(m_pDevice, m_pDeviceContext, SCENEID::SCENE_STATIC, 512)));




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


	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Shader_VTXNONANIMINST),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("Shader_VtxNonAnimMeshInstance.hlsl"), VTXNONANIM_INSTMAT_DECLARATION::Elements, VTXNONANIM_INSTMAT_DECLARATION::iNumElements)));
	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Shader_VTXANIMINST),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("Shader_VtxAnimMeshInstance.hlsl"), VTXANIM_INSTMAT_DECLARATION::Elements, VTXANIM_INSTMAT_DECLARATION::iNumElements)));


	////텍스처 프로토타입 생성
	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Texture_DefaultUI),
		CTexture::Create(m_pDevice, m_pDeviceContext, L"UI_Default.txt")));

	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Texture_HPUI),
		CTexture::Create(m_pDevice, m_pDeviceContext, L"HPUI.txt")));

	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Texture_TestEffect),
		CTexture::Create(m_pDevice, m_pDeviceContext, L"Instance_Effect.txt")));
	
	
	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, L"Prototype_Texture_Dissolve",
		CTexture::Create(m_pDevice, m_pDeviceContext, L"Dissolve.txt")));


	//TestEffect.txt;

	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Collider),
		CCollider::Create(m_pDevice, m_pDeviceContext)));

	//////////FAILED_CHECK(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Collider_PhysX),
	//////////	CCollider_PhysX_Base::Create(m_pDevice, m_pDeviceContext)));
	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Collider_PhysX_Static),
	CCollider_PhysX_Static::Create(m_pDevice, m_pDeviceContext)));
	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Collider_PhysX_Dynamic),
		CCollider_PhysX_Dynamic::Create(m_pDevice, m_pDeviceContext)));
	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Collider_PhysX_Joint),
		CCollider_PhysX_Joint::Create(m_pDevice, m_pDeviceContext)));


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


	FAILED_CHECK(m_pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_Camera_Main), CCamera_Main::Create(m_pDevice, m_pDeviceContext, &CameraDesc)));
	FAILED_CHECK(m_pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_SkyBox), CSkyBox::Create(m_pDevice, m_pDeviceContext)));

	//FAILED_CHECK(m_pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_UIImage), CUIImage::Create(m_pDevice, m_pDeviceContext)));
	//FAILED_CHECK(m_pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_UILoading), CLoadingUI::Create(m_pDevice, m_pDeviceContext)));

	
	FAILED_CHECK(m_pGameInstance->Add_GameObject_Prototype(TEXT("ProtoType_GameObjectObject_InstanceEffect_Ball"), CInstanceEffect_Ball::Create(m_pDevice, m_pDeviceContext)));
	FAILED_CHECK(m_pGameInstance->Add_GameObject_Prototype(TEXT("ProtoType_GameObjectObject_InstanceEffect_Straight"), CInstanceEffect_Straight::Create(m_pDevice, m_pDeviceContext)));
	FAILED_CHECK(m_pGameInstance->Add_GameObject_Prototype(TEXT("ProtoType_GameObjectObject_InstanceEffect_Cone"), CInstanceEffect_Cone::Create(m_pDevice, m_pDeviceContext)));
	FAILED_CHECK(m_pGameInstance->Add_GameObject_Prototype(TEXT("ProtoType_GameObjectObject_InstanceEffect_Spread"), CInstanceEffect_Spread::Create(m_pDevice, m_pDeviceContext)));
	FAILED_CHECK(m_pGameInstance->Add_GameObject_Prototype(TEXT("ProtoType_GameObjectObject_InstanceEffect_Fountain"), CInstanceEffect_Fountain::Create(m_pDevice, m_pDeviceContext)));
	FAILED_CHECK(m_pGameInstance->Add_GameObject_Prototype(TEXT("ProtoType_GameObjectObject_InstanceEffect_Suck"), CInstanceEffect_Suck::Create(m_pDevice, m_pDeviceContext)));


	FAILED_CHECK(m_pGameInstance->Add_GameObject_Prototype(TEXT("ProtoType_GameObjectObject_MeshEffect_Ball"),		CMeshEffect_Ball::Create(m_pDevice, m_pDeviceContext)));
	FAILED_CHECK(m_pGameInstance->Add_GameObject_Prototype(TEXT("ProtoType_GameObjectObject_MeshEffect_Straight"),	CMeshEffect_Straight::Create(m_pDevice, m_pDeviceContext)));
	FAILED_CHECK(m_pGameInstance->Add_GameObject_Prototype(TEXT("ProtoType_GameObjectObject_MeshEffect_Cone"),		CMeshEffect_Cone::Create(m_pDevice, m_pDeviceContext)));
	FAILED_CHECK(m_pGameInstance->Add_GameObject_Prototype(TEXT("ProtoType_GameObjectObject_MeshEffect_Spread"),	CMeshEffect_Spread::Create(m_pDevice, m_pDeviceContext)));
	FAILED_CHECK(m_pGameInstance->Add_GameObject_Prototype(TEXT("ProtoType_GameObjectObject_MeshEffect_Fountain"),	CMeshEffect_Fountain::Create(m_pDevice, m_pDeviceContext)));
	FAILED_CHECK(m_pGameInstance->Add_GameObject_Prototype(TEXT("ProtoType_GameObjectObject_MeshEffect_Suck"),		CMeshEffect_Suck::Create(m_pDevice, m_pDeviceContext)));
	
	//JJB
	FAILED_CHECK(m_pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_Object_UI_HpUI),		CHpUI::Create(m_pDevice, m_pDeviceContext)));


	//

	//
	//FAILED_CHECK(m_pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_UILoading), CLoadingUI::Create(m_pDevice, m_pDeviceContext)));
	//FAILED_CHECK(m_pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_UILoading), CLoadingUI::Create(m_pDevice, m_pDeviceContext)));
	//FAILED_CHECK(m_pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_UILoading), CLoadingUI::Create(m_pDevice, m_pDeviceContext)));




	//FAILED_CHECK(m_pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_Terrain), CSkyBox::Create(m_pDevice, m_pDeviceContext)));


	//if (FAILED(m_pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_UI_Common), CUI_Common::Create(m_pGraphicDevice, _float4(0, 0, 0, 0)))))
	//	return E_FAIL;
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
	GetSingle(CUtilityMgr)->Get_Renderer()->Clear_RenderGroup_forSceneChaging();

	m_pGameInstance->Get_NowScene()->Free();

#ifdef _DEBUG
	Safe_Release(m_pImguiMgr);
#endif // _DEBUG

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

