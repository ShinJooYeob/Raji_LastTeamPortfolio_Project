#include "..\Public\GameInstance.h"
#include "TimeMgr.h"
#include "SceneMgr.h"
#include "ThreadMgr.h"
#include "ObjectMgr.h"
#include "GameObject.h"
#include "EasingMgr.h"
#include "FrustumMgr.h"
#include "PipeLineMgr.h"
#include "LightMgr.h"
#include "CollisionMgr.h"
#include "RenderTargetMgr.h"
#include "ExternFontMgr.h"
#include "SoundMgr.h"

IMPLEMENT_SINGLETON(CGameInstance);


CGameInstance::CGameInstance()
	:m_pThreadMgr(GetSingle(CThreadMgr)), 
	m_pTimerMgr(GetSingle(CTimeMgr)),
	m_pGraphicDevice(GetSingle(CGraphic_Device)),
	m_pSceneMgr(GetSingle(CSceneMgr)),
	m_pObjectMgr(GetSingle(CObjectMgr)),
	m_pComponenetMgr(GetSingle(CComponentMgr)),
	m_pInputDevice(GetSingle(CInput_Device)),
	m_pEasingMgr(GetSingle(CEasingMgr)),
	m_pFrustumMgr(GetSingle(CFrustumMgr)),
	m_pPipeLineMgr(GetSingle(CPipeLineMgr)),
	m_pLightMgr(GetSingle(CLightMgr)),
	m_pCollisionMgr(GetSingle(CCollisionMgr)),
	m_pRenderTargetMgr(GetSingle(CRenderTargetMgr)),
	m_pExternFontMgr(GetSingle(CExternFontMgr)),
	m_pSoundMgr(GetSingle(CSoundMgr))

{

	Safe_AddRef(m_pThreadMgr);
	Safe_AddRef(m_pTimerMgr);
	Safe_AddRef(m_pGraphicDevice);
	Safe_AddRef(m_pSceneMgr);
	Safe_AddRef(m_pObjectMgr);
	Safe_AddRef(m_pComponenetMgr);
	Safe_AddRef(m_pInputDevice);
	Safe_AddRef(m_pEasingMgr);
	Safe_AddRef(m_pFrustumMgr);
	Safe_AddRef(m_pPipeLineMgr);
	Safe_AddRef(m_pLightMgr);
	Safe_AddRef(m_pCollisionMgr);
	Safe_AddRef(m_pRenderTargetMgr);
	Safe_AddRef(m_pExternFontMgr);
	Safe_AddRef(m_pSoundMgr);
}


HRESULT CGameInstance::Initialize_Engine(HINSTANCE hInst, const CGraphic_Device::GRAPHICDESC & GraphicDesc, _uint iMaxSceneNum, ID3D11Device** ppDeviceOut, ID3D11DeviceContext** ppDeviceContextOut,
	ID3D11RenderTargetView** ppBackBufferRTV, ID3D11DepthStencilView** ppDepthStencilView, IDXGISwapChain**	ppSwapChain, _double fDoubleInterver)
{
	if (m_pGraphicDevice == nullptr || m_pObjectMgr == nullptr || m_pComponenetMgr == nullptr ||m_pSceneMgr == nullptr || m_pFrustumMgr == nullptr ||  
		m_pLightMgr==nullptr || m_pCollisionMgr == nullptr || m_pExternFontMgr == nullptr || m_pSoundMgr == nullptr)
	{
		__debugbreak();
		return E_FAIL;
	}

	//if (FAILED(m_pSeverMgr->ConnectSever()))
	//{
	//	MSGBOX("Failed to Connecting Sever");
	//}

	FAILED_CHECK(m_pGraphicDevice->Initialize_Graphic_Device(GraphicDesc.hWnd, GraphicDesc.eWinMode, GraphicDesc.iWinCX, GraphicDesc.iWinCY, ppDeviceOut, ppDeviceContextOut, ppBackBufferRTV, ppDepthStencilView, ppSwapChain));

	FAILED_CHECK(m_pObjectMgr->Reserve_Container(iMaxSceneNum));

	FAILED_CHECK(m_pInputDevice->Ready_Input_Device(hInst, GraphicDesc.hWnd, fDoubleInterver));

	FAILED_CHECK(m_pComponenetMgr->Reserve_Container(iMaxSceneNum));


	//메니저 초기화
	FAILED_CHECK(m_pSoundMgr->Initialize_FMOD());

	FAILED_CHECK(m_pFrustumMgr->Initialize_FrustumMgr(*ppDeviceOut, *ppDeviceContextOut));

	FAILED_CHECK(m_pLightMgr->Initialize_LightMgr(*ppDeviceOut, *ppDeviceContextOut));

	FAILED_CHECK(m_pCollisionMgr->Initialize_CollisionMgr(*ppDeviceOut, *ppDeviceContextOut));

	FAILED_CHECK(m_pRenderTargetMgr->Initialize_RenderTargetMgr(*ppDeviceOut, *ppDeviceContextOut));

	FAILED_CHECK(m_pExternFontMgr->Initialize_FontMgr(*ppDeviceOut, *ppDeviceContextOut));

	return S_OK;
}


_int CGameInstance::Update_Engine(_double fDeltaTime)
{
	if (m_pSceneMgr == nullptr || m_pObjectMgr == nullptr || m_pSoundMgr == nullptr)
		return -1;


	FAILED_CHECK(m_pInputDevice->SetUp_InputDeviceState(fDeltaTime));

	FAILED_CHECK(m_pSoundMgr->Update_FMOD((_float)fDeltaTime));


	if (m_pObjectMgr->Update(fDeltaTime) < 0)
	{
		__debugbreak();
		return -1;
	}

	if (m_pSceneMgr->Update(fDeltaTime) < 0)
	{
		__debugbreak();
		return -1;
	}

	FAILED_CHECK(SetUp_WorldFrustumPlane());


	return 0;
}

_int CGameInstance::LateUpdate_Engine(_double fDeltaTime)
{
	FAILED_CHECK(m_pCollisionMgr->Inspect_Collision());

	if (m_pObjectMgr->LateUpdate(fDeltaTime) < 0)
	{
		__debugbreak();
		return -1;
	}
	if (m_pSceneMgr->LateUpdate(fDeltaTime) < 0)
	{
		__debugbreak();
		return -1;
	}

	return 0;
}


HRESULT CGameInstance::Clear_Scene_Resource(_uint eSceneNum)
{
	if (m_pObjectMgr == nullptr || m_pComponenetMgr == nullptr)
	{
		__debugbreak();
		return E_FAIL;
	}

	FAILED_CHECK(m_pObjectMgr->Clear_Scene_GameObjects(eSceneNum));

	FAILED_CHECK(m_pComponenetMgr->Clear_Scene_Componenets(eSceneNum));


	return S_OK;
}

HRESULT CGameInstance::Clear_BackBuffer_View(_float4 vClearColor)
{
	NULL_CHECK_BREAK(m_pGraphicDevice);


	return m_pGraphicDevice->Clear_BackBuffer_View(vClearColor);
}

HRESULT CGameInstance::Clear_DepthStencil_View()
{
	NULL_CHECK_BREAK(m_pGraphicDevice);


	return m_pGraphicDevice->Clear_DepthStencil_View();
}

HRESULT CGameInstance::Present()
{
	NULL_CHECK_BREAK(m_pGraphicDevice);


	return m_pGraphicDevice->Present();
}

HRESULT CGameInstance::Add_GameObject_Prototype(const _tchar * tagPrototype, CGameObject * pPrototype)
{
	if (m_pObjectMgr == nullptr)
	{
		__debugbreak();
		return E_FAIL;
	}

	return	m_pObjectMgr->Add_GameObject_Prototype(tagPrototype, pPrototype);
}

HRESULT CGameInstance::Add_GameObject_To_Layer(_uint eSceneNum, const _tchar * tagLayer, const _tchar * tagPrototype, void * pArg)
{
	NULL_CHECK_BREAK(m_pObjectMgr);



	return	m_pObjectMgr->Add_GameObject_To_Layer(eSceneNum, tagLayer, tagPrototype, pArg);
}

HRESULT CGameInstance::Add_GameObject_To_Layer_By_Parameter(_uint eSceneNum, const _tchar * tagLayer, CGameObject * pGameObject)
{
	NULL_CHECK_BREAK(m_pObjectMgr);

	return m_pObjectMgr->Add_GameObject_To_Layer_By_Parameter(eSceneNum,tagLayer,pGameObject);
}

HRESULT CGameInstance::Add_GameObject_Out_of_Manager(CGameObject ** ppOutGameObj, _uint eSceneNum, const _tchar * tagPrototype, void * pArg)
{
	NULL_CHECK_BREAK(m_pObjectMgr);

	return	m_pObjectMgr->Add_GameObject_Out_of_Manager(ppOutGameObj,eSceneNum, tagPrototype, pArg);
}

CComponent* CGameInstance::Get_Commponent_By_LayerIndex(_uint eSceneNum, const _tchar * tagLayer, const _tchar* tagComponet, _uint iLayerIndex)
{
	if (tagComponet == nullptr || tagLayer == nullptr || m_pObjectMgr == nullptr)
	{
		__debugbreak();
		return nullptr;
	}

	return m_pObjectMgr->Get_Commponent_By_LayerIndex(eSceneNum, tagLayer, tagComponet,iLayerIndex);
}

CGameObject * CGameInstance::Get_GameObject_By_LayerIndex(_uint eSceneNum, const _tchar * tagLayer, _uint iLayerIndex)
{
	if (tagLayer == nullptr || m_pObjectMgr == nullptr)
	{
		__debugbreak();
		return nullptr;
	}

	return m_pObjectMgr->Get_GameObject_By_LayerIndex(eSceneNum, tagLayer,iLayerIndex);
}

CGameObject * CGameInstance::Get_GameObject_By_LayerLastIndex(_uint eSceneNum, const _tchar * tagLayer)
{
	if (tagLayer == nullptr || m_pObjectMgr == nullptr)
	{
		__debugbreak();
		return nullptr;
	}

	return m_pObjectMgr->Get_GameObject_By_LayerLastIndex(eSceneNum, tagLayer);

}

list<CGameObject*>* CGameInstance::Get_ObjectList_from_Layer(_uint eSceneNum, const _tchar * tagLayer)
{
	if (tagLayer == nullptr || m_pObjectMgr == nullptr)
	{
		__debugbreak();
		return nullptr;
	}

	return m_pObjectMgr->Get_ObjectList_from_Layer(eSceneNum, tagLayer);
}


HRESULT CGameInstance::Delete_GameObject_To_Layer_Index(_uint eSceneNum, const _tchar * tagLayer, _uint index)
{
	if (tagLayer == nullptr || m_pObjectMgr == nullptr)
	{

		__debugbreak();
		return E_FAIL;
	}
	

	return Delete_GameObject_To_Layer_Index(eSceneNum, tagLayer, index);
}

HRESULT CGameInstance::Delete_GameObject_To_Layer_Object(_uint eSceneNum, const _tchar * tagLayer, CGameObject * obj)
{
	if (tagLayer == nullptr || m_pObjectMgr == nullptr)
	{
		__debugbreak();
		return E_FAIL;
	}

	return Delete_GameObject_To_Layer_Object(eSceneNum, tagLayer, obj);
}

HRESULT CGameInstance::Set_Transform(TRANSFORMSTATETYPE eStateType, const _fMatrix& TransformMatrix)
{
	NULL_CHECK_RETURN(m_pPipeLineMgr, E_FAIL);

	return m_pPipeLineMgr->Set_Transform(eStateType,TransformMatrix);
}

HRESULT CGameInstance::Set_Transform(TRANSFORMSTATETYPE eStateType, const _float4x4 & TransformMatrix)
{
	NULL_CHECK_RETURN(m_pPipeLineMgr, E_FAIL);

	return m_pPipeLineMgr->Set_Transform(eStateType, TransformMatrix);
}

_fMatrix CGameInstance::Get_Transform_Matrix(TRANSFORMSTATETYPE eStateType)
{
	NULL_CHECK_BREAK(m_pPipeLineMgr);

	return m_pPipeLineMgr->Get_Transform_Matrix(eStateType);
}

_float4x4 CGameInstance::Get_Transform_Float4x4(TRANSFORMSTATETYPE eStateType)
{
	NULL_CHECK_BREAK(m_pPipeLineMgr);

	return m_pPipeLineMgr->Get_Transform_Float4x4(eStateType);
}

_fMatrix CGameInstance::Get_Transform_Matrix_TP(TRANSFORMSTATETYPE eStateType)
{
	NULL_CHECK_BREAK(m_pPipeLineMgr);

	return m_pPipeLineMgr->Get_Transform_Matrix_TP(eStateType);
}

_float4x4 CGameInstance::Get_Transform_Float4x4_TP(TRANSFORMSTATETYPE eStateType)
{
	NULL_CHECK_BREAK(m_pPipeLineMgr);

	return m_pPipeLineMgr->Get_Transform_Float4x4_TP(eStateType);
}

HRESULT CGameInstance::Set_TargetPostion(TARGETPOSITIONTYPE eStateType, const _fVector & TargetPos)
{
	NULL_CHECK_RETURN(m_pPipeLineMgr,E_FAIL);

	return m_pPipeLineMgr->Set_TargetPostion(eStateType,TargetPos);
}

HRESULT CGameInstance::Set_TargetPostion(TARGETPOSITIONTYPE eStateType, const _float4 & TargetPos)
{
	NULL_CHECK_RETURN(m_pPipeLineMgr, E_FAIL);

	return m_pPipeLineMgr->Set_TargetPostion(eStateType, TargetPos);
}

_fVector CGameInstance::Get_TargetPostion_Vector(TARGETPOSITIONTYPE eStateType)
{
	NULL_CHECK_BREAK(m_pPipeLineMgr);

	return m_pPipeLineMgr->Get_TargetPostion_Vector(eStateType);
}

_float4 CGameInstance::Get_TargetPostion_float4(TARGETPOSITIONTYPE eStateType)
{
	NULL_CHECK_BREAK(m_pPipeLineMgr);

	return m_pPipeLineMgr->Get_TargetPostion_float4(eStateType);
}



_double CGameInstance::Get_DeltaTime(const _tchar * tagTimer)
{
	NULL_CHECK_BREAK(m_pTimerMgr);


	return m_pTimerMgr->Get_DeltaTime(tagTimer);
}

HRESULT CGameInstance::Add_Timer(const _tchar * tagTimer)
{
	NULL_CHECK_BREAK(m_pTimerMgr);

	return m_pTimerMgr->Add_Timer(tagTimer);
}



HRESULT CGameInstance::Scene_Change( CScene * pScene, _int iNextSceneIdx)
{

	if (m_pSceneMgr == nullptr || m_pObjectMgr == nullptr)
	{
		__debugbreak();
		return E_FAIL;
	}

	FAILED_CHECK(m_pObjectMgr->Reset_StaticObject_NowSceneNum(iNextSceneIdx));

	FAILED_CHECK(m_pSceneMgr->Scene_Chage(pScene, iNextSceneIdx));



	return S_OK;
}

_int CGameInstance::Render_Scene()
{
	NULL_CHECK_BREAK(m_pSceneMgr);

	if (m_pSceneMgr->Render() < 0)
	{
		__debugbreak();
		return -1;
	}

	if (m_pSceneMgr->LateRender() < 0)
	{
		__debugbreak();
		return -1;
	}

	return 0;
}

_uint CGameInstance::Get_NowSceneNum()
{
	NULL_CHECK_BREAK(m_pSceneMgr);

	return m_pSceneMgr->Get_NowSceneNum();
}

_uint CGameInstance::Get_TargetSceneNum()
{
	NULL_CHECK_BREAK(m_pSceneMgr);

	return m_pSceneMgr->Get_TargetSceneNum();
}

void CGameInstance::Set_TargetSceneNum(_uint iSceneNum)
{
	NULL_CHECK_BREAK(m_pSceneMgr);

	m_pSceneMgr->Set_TargetSceneNum(iSceneNum);
	return;
}


CScene * CGameInstance::Get_NowScene()
{
	NULL_CHECK_BREAK(m_pSceneMgr);

	return m_pSceneMgr->Get_NowScene();
}

HRESULT CGameInstance::Set_SceneChanging_to_Scene( _uint _INextScene)
{
	NULL_CHECK_BREAK(m_pSceneMgr);
	return m_pSceneMgr->Set_SceneChanging_to_Scene(_INextScene);
}

HRESULT CGameInstance::Add_Component_Prototype(_uint eSceneIdx, const _tchar * tagPrototypeComponent, CComponent * pComponenet)
{
	NULL_CHECK_BREAK(m_pComponenetMgr);


	return m_pComponenetMgr->Add_Component_Prototype(eSceneIdx, tagPrototypeComponent,pComponenet);
}

CComponent * CGameInstance::Clone_Component(_uint eSceneIdx, const _tchar * tagPrototypeComponent, void * pArg)
{
	NULL_CHECK_BREAK(m_pComponenetMgr);


	return m_pComponenetMgr->Clone_Component(eSceneIdx, tagPrototypeComponent, pArg);
}

_int CGameInstance::Set_ChannelVolume(CHANNELID eID, _float _vol)
{
	NULL_CHECK_BREAK(m_pSoundMgr);
	return m_pSoundMgr->Set_ChannelVolume(eID, _vol);
}

_int CGameInstance::Pause_ChannelSound(CHANNELID eID)
{
	NULL_CHECK_BREAK(m_pSoundMgr);
	return m_pSoundMgr->Pause_ChannelSound(eID);
}

HRESULT CGameInstance::PlaySound(TCHAR * pSoundKey, CHANNELID eID, _float fLouderMultiple)
{
	NULL_CHECK_BREAK(m_pSoundMgr);
	return m_pSoundMgr->PlaySound(pSoundKey, eID, fLouderMultiple);
}

HRESULT CGameInstance::PlayBGM(TCHAR * pSoundKey, _uint iBGMIndex, _float fLouderMultiple)
{
	NULL_CHECK_BREAK(m_pSoundMgr);
	return m_pSoundMgr->PlayBGM(pSoundKey, iBGMIndex, fLouderMultiple);
}

void CGameInstance::Stop_ChannelSound(CHANNELID eID)
{
	NULL_CHECK_BREAK(m_pSoundMgr);
	return m_pSoundMgr->Stop_ChannelSound(eID);
}

void CGameInstance::Stop_AllChannel()
{
	NULL_CHECK_BREAK(m_pSoundMgr);
	m_pSoundMgr->Stop_AllChannel();
}

_float CGameInstance::Get_Channel_Volume(CHANNELID eID)
{
	NULL_CHECK_BREAK(m_pSoundMgr);
	return m_pSoundMgr->Get_Channel_Volume(eID);
}

_bool CGameInstance::Get_Channel_IsPaused(CHANNELID eID)
{
	NULL_CHECK_BREAK(m_pSoundMgr);
	return m_pSoundMgr->Get_Channel_IsPaused(eID);
}

HRESULT CGameInstance::PlayThread(void * _ThreadFunc, void * _pArg)
{
	NULL_CHECK_BREAK(m_pThreadMgr);

	return m_pThreadMgr->PlayThread(_ThreadFunc, _pArg);
}

_byte CGameInstance::Get_DIKeyState(_ubyte eKeyID)
{
	NULL_CHECK_BREAK(m_pInputDevice);

	return m_pInputDevice->Get_DIKeyState(eKeyID);
}

_long CGameInstance::Get_DIMouseMoveState(CInput_Device::MOUSEMOVESTATE eMouseMoveState)
{
	NULL_CHECK_BREAK(m_pInputDevice);


	return m_pInputDevice->Get_DIMouseMoveState(eMouseMoveState);
}

_byte CGameInstance::Get_DIMouseButtonState(CInput_Device::MOUSEBUTTONSTATE eMouseButtonState)
{
	NULL_CHECK_BREAK(m_pInputDevice);

	return m_pInputDevice->Get_DIMouseButtonState(eMouseButtonState);
}

_float CGameInstance::Easing(_uint eEasingType, _float fStartPoint, _float fTargetPoint, _float fPassedTime, _float fTotalTime)
{
	NULL_CHECK_BREAK(m_pEasingMgr);

	return m_pEasingMgr->Easing(eEasingType, fStartPoint, fTargetPoint, fPassedTime, fTotalTime);
}

_float3 CGameInstance::Easing_Vector(_uint eEasingType, _float3 fStartPoint, _float3 fTargetPoint, _float fPassedTime, _float fTotalTime)
{

	NULL_CHECK_BREAK(m_pEasingMgr);

	return m_pEasingMgr->Easing_Vector(eEasingType, fStartPoint, fTargetPoint, fPassedTime, fTotalTime);
}

_float CGameInstance::Easing_Return(_uint eGoEasingType, _uint eBackEasingType, _float fStartPoint, _float fTargetPoint, _float fPassedTime, _float fTotalTime)
{
	NULL_CHECK_BREAK(m_pEasingMgr);

	return m_pEasingMgr->Easing_Return(eGoEasingType, eBackEasingType, fStartPoint, fTargetPoint, fPassedTime, fTotalTime);
}

_float3 CGameInstance::Easing_Return_Vector(_uint eGoEasingType, _uint eBackEasingType, _float3 fStartPoint, _float3 fTargetPoint, _float fPassedTime, _float fTotalTime)
{
	NULL_CHECK_BREAK(m_pEasingMgr);

	return m_pEasingMgr->Easing_Return_Vector(eGoEasingType, eBackEasingType, fStartPoint, fTargetPoint, fPassedTime, fTotalTime);
}

HRESULT CGameInstance::SetUp_WorldFrustumPlane()
{
	NULL_CHECK_BREAK(m_pFrustumMgr);

	return m_pFrustumMgr->SetUp_WorldFrustumPlane();
}

_bool CGameInstance::IsNeedToRender(_float3 vWorldPosition, _float fLenth)
{
	NULL_CHECK_MSG(m_pFrustumMgr,L"Not Have FrustumMgr");

	return m_pFrustumMgr->IsNeedToRender(vWorldPosition, fLenth);
}


LIGHTDESC * CGameInstance::Get_LightDesc(LIGHTDESC::TYPE eLightType, _uint iIndex)
{
	NULL_CHECK_BREAK(m_pLightMgr);

	return m_pLightMgr->Get_LightDesc(eLightType,iIndex);
}

HRESULT CGameInstance::Add_Light(const LIGHTDESC & LightDesc, LIGHTDESC** pOutDesc)
{
	NULL_CHECK_BREAK(m_pLightMgr);

	return m_pLightMgr->Add_Light(LightDesc, pOutDesc);
}

HRESULT CGameInstance::EasingDiffuseLightDesc(LIGHTDESC::TYPE eLightType, _uint iIndex, _fVector vTargetDiffuse, _float MixRate)
{
	NULL_CHECK_BREAK(m_pLightMgr);

	return m_pLightMgr->EasingDiffuseLightDesc(eLightType, iIndex, vTargetDiffuse, MixRate);
}

HRESULT CGameInstance::Relocate_LightDesc(LIGHTDESC::TYPE eLightType, _uint iIndex, _fVector vRelocatePosition, _float fRange)
{
	NULL_CHECK_BREAK(m_pLightMgr);

	return m_pLightMgr->Relocate_LightDesc(eLightType, iIndex, vRelocatePosition, fRange);;
}

HRESULT CGameInstance::Add_CollisionGroup(CollisionTypeID eType, CGameObject * pCollisionObject, CCollider * pCollider)
{
	NULL_CHECK_RETURN(m_pCollisionMgr,E_FAIL);


	return m_pCollisionMgr->Add_CollisionGroup(eType,pCollisionObject,pCollider);
}

void CGameInstance::Clear_CollisionGroup()
{
	NULL_CHECK_BREAK(m_pCollisionMgr);
	m_pCollisionMgr->Clear_CollisionGroup();
}

HRESULT CGameInstance::Add_Font(const _tchar * pFontTag, const _tchar * pFontFilePath)
{
	NULL_CHECK_BREAK(m_pExternFontMgr);

	return m_pExternFontMgr->Add_Font(pFontTag,pFontFilePath);
}

HRESULT CGameInstance::Render_Font(const _tchar * pFontTag, const _tchar * pText, _float2 vPosition, _float4 vColor,_float fAngle, _float2 vScale)
{
	NULL_CHECK_BREAK(m_pExternFontMgr);

	return m_pExternFontMgr->Render_Font(pFontTag, pText, vPosition, vColor.XMVector(), fAngle, vScale);
}

ID3D11ShaderResourceView * CGameInstance::Get_SRV(const _tchar * pTargetTag) const
{
	NULL_CHECK_BREAK(m_pRenderTargetMgr);

	return m_pRenderTargetMgr->Get_SRV(pTargetTag);
}

HRESULT CGameInstance::Begin_MTV(const _tchar * pMRTTag)
{
	NULL_CHECK_BREAK(m_pRenderTargetMgr);

	return m_pRenderTargetMgr->Begin(pMRTTag);
}

HRESULT CGameInstance::Begin_MTV_WithBackBuffer(const _tchar * pMRTTag)
{
	NULL_CHECK_BREAK(m_pRenderTargetMgr);

	return m_pRenderTargetMgr->Begin_WithBackBuffer(pMRTTag);
}

HRESULT CGameInstance::Begin_MTV(const _tchar * pMRTTag, ID3D11DepthStencilView * DepthStencil)
{
	NULL_CHECK_BREAK(m_pRenderTargetMgr);

	return m_pRenderTargetMgr->Begin(pMRTTag, DepthStencil);
}

HRESULT CGameInstance::End_MTV(const _tchar * pMRTTag)
{
	NULL_CHECK_BREAK(m_pRenderTargetMgr);

	return m_pRenderTargetMgr->End(pMRTTag);
}




void CGameInstance::Release_Engine()
{


	if (0 != GetSingle(CGameInstance)->DestroyInstance())
		MSGBOX("Failed to Release Com GameInstance");

	if (0 != GetSingle(CThreadMgr)->DestroyInstance())
		MSGBOX("Failed to Release Com ThreadMgr ");

	if (0 != GetSingle(CSoundMgr)->DestroyInstance())
		MSGBOX("Failed to Release Com CSoundMgr ");

	if (0 != GetSingle(CEasingMgr)->DestroyInstance())
		MSGBOX("Failed to Release Com EasingMgr ");

	if (0 != GetSingle(CObjectMgr)->DestroyInstance())
		MSGBOX("Failed to Release Com ObjectMgr ");

	if (0 != GetSingle(CSceneMgr)->DestroyInstance())
		MSGBOX("Failed to Release Com SceneMgr ");

	if (0 != GetSingle(CComponentMgr)->DestroyInstance())
		MSGBOX("Failed to Release Com ComponentMgr");

	if (0 != GetSingle(CFrustumMgr)->DestroyInstance())
		MSGBOX("Failed to Release Com FrustumMgr ");

	if (0 != GetSingle(CInput_Device)->DestroyInstance())
		MSGBOX("Failed to Release Com CInput_Device ");

	if (0 != GetSingle(CTimeMgr)->DestroyInstance())
		MSGBOX("Failed to Release Com TimeMgr ");

	if (0 != GetSingle(CPipeLineMgr)->DestroyInstance())
		MSGBOX("Failed to Release CPipeLineMgr ");

	if (0 != GetSingle(CLightMgr)->DestroyInstance())
		MSGBOX("Failed to Release CLightMgr ");

	if (0 != GetSingle(CCollisionMgr)->DestroyInstance())
		MSGBOX("Failed to Release CCollisionMgr ");

	if (0 != GetSingle(CExternFontMgr)->DestroyInstance())
		MSGBOX("Failed to Release Com CExternFontMgr");

	if (0 != GetSingle(CRenderTargetMgr)->DestroyInstance())
		MSGBOX("Failed to Release Com CRenderTargetMgr ");

	if (0 != GetSingle(CGraphic_Device)->DestroyInstance())
		MSGBOX("Failed to Release Com Graphic_Device ");

}

void CGameInstance::Free()
{
	Safe_Release(m_pSoundMgr);
	Safe_Release(m_pThreadMgr);
	Safe_Release(m_pObjectMgr);
	Safe_Release(m_pSceneMgr);
	Safe_Release(m_pComponenetMgr);
	Safe_Release(m_pGraphicDevice);
	Safe_Release(m_pInputDevice);
	Safe_Release(m_pTimerMgr);
	Safe_Release(m_pEasingMgr);
	Safe_Release(m_pFrustumMgr);
	Safe_Release(m_pPipeLineMgr);
	Safe_Release(m_pLightMgr);
	Safe_Release(m_pCollisionMgr);
	Safe_Release(m_pRenderTargetMgr);
	Safe_Release(m_pExternFontMgr);
	
	
}
