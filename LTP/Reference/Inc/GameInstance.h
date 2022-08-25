#pragma once

#include "Graphic_Device.h"
#include "ComponentMgr.h"
#include "Input_Device.h"
#include "EasingMgr.h"
#include "CollisionMgr.h"

BEGIN(Engine)

class CTimeMgr;
class CSceneMgr;
class CScene;
class CThreadMgr; 
class CObjectMgr;
class CGameObject;
class CLayer;
class CFrustumMgr;
class CPipeLineMgr;
class CLightMgr;
//class CCollisionMgr;
class CRenderTargetMgr;
class CExternFontMgr;
class CSoundMgr;
class CFileInfoMgr;
class CPhysXMgr;

class ENGINE_DLL CGameInstance final :public CBase
{
	DECLARE_SINGLETON(CGameInstance)

private:
	explicit CGameInstance();
	virtual ~CGameInstance() = default;

	 

public:/*엔진이 돌아갈 때 전반적으로 처리해줘야할 것들*/
	HRESULT		Initialize_Engine(HINSTANCE hInst, const CGraphic_Device::GRAPHICDESC& GraphicDesc,_uint iMaxSceneNum , ID3D11Device** ppDeviceOut, 
		ID3D11DeviceContext** ppDeviceContextOut,  ID3D11RenderTargetView** ppBackBufferRTV, ID3D11DepthStencilView** ppDepthStencilView, IDXGISwapChain**	ppSwapChain, _double fDoubleInterver = 0.3);
	_int		Update_Engine(_double fDeltaTime);
	_int		LateUpdate_Engine(_double fDeltaTime);

	HRESULT		Clear_Scene_Resource(_uint eSceneNum);

public:  /*For Graphic Device*/
	HRESULT Clear_BackBuffer_View(_float4 vClearColor);
	HRESULT Clear_DepthStencil_View();
	HRESULT Present();


public:/* For.ObjectMgr */
	HRESULT			Add_GameObject_Prototype(const _tchar* tagPrototype, CGameObject* pPrototype);
	HRESULT			Add_GameObject_To_Layer(_uint eSceneNum, const _tchar* tagLayer, const _tchar* tagPrototype, void* pArg = nullptr);
	HRESULT			Add_GameObject_To_Layer_By_Parameter(_uint eSceneNum, const _tchar* tagLayer, CGameObject* pGameObject);
	HRESULT			Add_GameObject_Out_of_Manager(CGameObject** ppOutGameObj, _uint eSceneNum, const _tchar* tagPrototype, void* pArg = nullptr);
	CGameObject*	Add_GameObject_GetObject(_uint eSceneNum, const _tchar* tagLayer, const _tchar* tagPrototype, void* pArg = nullptr);
	CComponent*		Get_Commponent_By_LayerIndex(_uint eSceneNum, const _tchar* tagLayer, const _tchar* tagComponet, _uint iLayerIndex = 0);
	CGameObject*	Get_GameObject_By_LayerIndex(_uint eSceneNum, const _tchar* tagLayer, _uint iLayerIndex = 0);
	CGameObject*	Get_GameObject_By_LayerLastIndex(_uint eSceneNum, const _tchar* tagLayer);
	list<CGameObject*>*	Get_ObjectList_from_Layer(_uint eSceneNum, const _tchar* tagLayer);
	HRESULT			Delete_GameObject_To_Layer_Index(_uint eSceneNum, const _tchar* tagLayer, _uint index);
	HRESULT			Delete_GameObject_To_Layer_Object(_uint eSceneNum, const _tchar* tagLayer, CGameObject* obj);

public:/* For.PipeLineMgr */
	HRESULT Set_Transform(TRANSFORMSTATETYPE eStateType, const _fMatrix& TransformMatrix);
	HRESULT Set_Transform(TRANSFORMSTATETYPE eStateType, const _float4x4& TransformMatrix);

	_fMatrix Get_Transform_Matrix(TRANSFORMSTATETYPE eStateType);
	_float4x4 Get_Transform_Float4x4(TRANSFORMSTATETYPE eStateType);
	_fMatrix Get_Transform_Matrix_TP(TRANSFORMSTATETYPE eStateType);
	_float4x4 Get_Transform_Float4x4_TP(TRANSFORMSTATETYPE eStateType);

	HRESULT Set_TargetPostion(TARGETPOSITIONTYPE eStateType, const _fVector& TargetPos);
	HRESULT Set_TargetPostion(TARGETPOSITIONTYPE eStateType, const _float4& TargetPos);

	_fVector Get_TargetPostion_Vector(TARGETPOSITIONTYPE eStateType);
	_float4  Get_TargetPostion_float4(TARGETPOSITIONTYPE eStateType);

public: /*For.ComMgr*/
	HRESULT			Add_Component_Prototype(_uint eSceneIdx, const _tchar* tagPrototypeComponent, CComponent* pComponenet);
	CComponent*		Clone_Component(_uint eSceneIdx, const _tchar* tagPrototypeComponent, void* pArg = nullptr);

public: /*For. SoundMgr*/
	_int  Set_ChannelVolume(CHANNELID eID, _float _vol);
	_int  Pause_ChannelSound(CHANNELID eID);

	HRESULT PlaySound(TCHAR* pSoundKey, CHANNELID eID, _float fLouderMultiple = 1.f);
	HRESULT PlayBGM(TCHAR* pSoundKey, _uint iBGMIndex = 0, _float fLouderMultiple = 1.f);
	HRESULT Play3D_Sound(TCHAR* pSoundKey, _float3 Pos, CHANNELID eID, _float fLouderMultiple = 1.f, _float3 dir = _float3::Zero(), _float speed = 1.f);
	HRESULT Play3D_Sound_IsPlay(TCHAR* pSoundKey, _float3 Pos, CHANNELID eID, _float fLouderMultiple = 1.f, _float3 dir = _float3::Zero(), _float speed = 1.f,_bool* Playing = nullptr);

	HRESULT Set_3DSound_DistanceMinValue(_float rolloffscale);

	void Stop_ChannelSound(CHANNELID eID);
	void Stop_AllChannel();

	_float  Get_Channel_Volume(CHANNELID eID);
	_bool  Get_Channel_IsPaused(CHANNELID eID);
	
	FMOD_SYSTEM*	Get_SOUNDSYSTEM();


public: /* For.TimerMgr */
	_double	Get_DeltaTime(const _tchar* tagTimer);
	HRESULT Add_Timer(const _tchar* tagTimer);


public: /*For SceneMgr*/
	HRESULT		Scene_Change( CScene* pScene, _int iNextSceneIdx);
	_int		Render_Scene();

	_uint		Get_NowSceneNum();
	_uint		Get_TargetSceneNum();
	void		Set_TargetSceneNum(_uint iSceneNum);

	CScene*		Get_NowScene();
	HRESULT		Set_SceneChanging_to_Scene(_uint _INextScene);

public: /*For TreadMgr*/
	HRESULT		PlayThread(void* _ThreadFunc, void* _pArg, void*  _pDesc = nullptr);

public: /* For.Input_Device */
	_byte Get_DIKeyState(_ubyte eKeyID);
	_long Get_DIMouseMoveState(CInput_Device::MOUSEMOVESTATE eMouseMoveState);
	_byte Get_DIMouseButtonState(CInput_Device::MOUSEBUTTONSTATE eMouseButtonState);

public:
	_float Easing(_uint eEasingType, _float fStartPoint, _float fTargetPoint, _float fPassedTime, _float fTotalTime = 1.0f);
	_float3	Easing_Vector(_uint eEasingType, _float3 fStartPoint, _float3 fTargetPoint, _float fPassedTime, _float fTotalTime = 1.0f);
	_float	Easing_Return(_uint eGoEasingType, _uint eBackEasingType, _float fStartPoint, _float fTargetPoint, _float fPassedTime, _float fTotalTime = 1.f);
	_float3	Easing_Return_Vector(_uint eGoEasingType, _uint eBackEasingType, _float3 fStartPoint, _float3 fTargetPoint, _float fPassedTime, _float fTotalTime = 1.f);


	//FrustumMgr
public:
	HRESULT SetUp_WorldFrustumPlane();
	HRESULT SetUp_LightFrustumPlane();
											//이 함수는 매 프레임 당 한번만 실행 어차피 매 프레임 같은 투영, 뷰 행렬일테니까 
	_bool IsNeedToRender(_float3 vWorldPosition, _float fLenth = 1.4142135623f);		//이 함수를 이제 객체별로 전달해서 그려야할지 여부를 판단
	_bool IsNeedToLightRender(_float3 vWorldPosition, _float fLenth = 1.4142135623f);		//이 함수를 이제 객체별로 전달해서 그려야할지 여부를 판단




public:// LightMgr
	LIGHTDESC* Get_LightDesc(LIGHTDESC::TYPE eLightType, _uint iIndex);
	HRESULT Add_Light(const LIGHTDESC& LightDesc, LIGHTDESC** pOutDesc = nullptr);
	HRESULT EasingDiffuseLightDesc(LIGHTDESC::TYPE eLightType, _uint iIndex, _fVector vTargetDiffuse, _float MixRate = 1.f);
	HRESULT Relocate_LightDesc(LIGHTDESC::TYPE eLightType, _uint iIndex, _fVector vRelocatePosition, _float fRange = 8.f);

public: //CollisionMgr
	HRESULT Add_CollisionGroup(CollisionTypeID eType, class CGameObject* pCollisionObject, class CCollider* pCollider);
	HRESULT Add_RepelGroup(class CTransform* pTransform, _float fRadious = 0.5f, class CNavigation* pNavigation = nullptr, _bool IsKinect = false);
	void	Clear_CollisionGroup();

	//Edit
	HRESULT		 Add_NaviPointCollider(CCollisionMgr::EDITPOINTCOLLIDER Collider);
	CGameObject* NaviPointCollision(_Vector pos, _Vector dir);


public: /* For.FontMgr */
	HRESULT Add_Font(const _tchar* pFontTag, const _tchar* pFontFilePath);
	HRESULT Render_Font(const _tchar* pFontTag, const _tchar* pText, _float2 vPosition, _float4 vColor,_float fAngle = 0,_float2 vScale = _float2(1));

public:
	ID3D11ShaderResourceView* Get_SRV(const _tchar* pTargetTag) const;
	HRESULT Begin_MTV(const _tchar* pMRTTag);
	HRESULT Begin_MTV(const _tchar* pMRTTag, ID3D11DepthStencilView* DepthStencil);
	HRESULT Begin_MTV_WithBackBuffer(const _tchar* pMRTTag);
	HRESULT End_MTV(const _tchar* pMRTTag);

public: /* CFileInfoMgr */
	HRESULT FolderFinder(const wstring& FileFolder);
	void SaveVectorToDat(wstring savetxtName, wstring ExtensionName = L"");
	void SaveVectorToDat_Particle(wstring savetxtName, wstring ExtensionName = L"");

	list<MYFILEPATH*> Load_ExtensionList(wstring txtfilepath, string exe, bool bFlag = true);
	wstring Get_PathData(wstring Fullpath);



public:	/*For DebugWnd*/
	_uint			Get_ObjectPrototypeNum() ;
	const _tchar*	Get_PrototypeTag(_uint iIndex);
	_uint			Get_SceneLayerSize(_uint iSceneNum);
	const _tchar*	Get_SceneLayerTag(_uint iSceneNum, _uint iIndex);


private:
	CGraphic_Device*	m_pGraphicDevice = nullptr;
	CTimeMgr*			m_pTimerMgr = nullptr;
	CSceneMgr*			m_pSceneMgr = nullptr;
	CThreadMgr*			m_pThreadMgr = nullptr;
	CObjectMgr*			m_pObjectMgr = nullptr;
	CComponentMgr*		m_pComponenetMgr = nullptr;
	CInput_Device*		m_pInputDevice = nullptr;
	CEasingMgr*			m_pEasingMgr = nullptr;
	CFrustumMgr*		m_pFrustumMgr = nullptr;
	CPipeLineMgr*		m_pPipeLineMgr = nullptr;
	CLightMgr*			m_pLightMgr = nullptr;
	CCollisionMgr*		m_pCollisionMgr = nullptr;
	CRenderTargetMgr*	m_pRenderTargetMgr = nullptr;
	CExternFontMgr*		m_pExternFontMgr	= nullptr;
	CSoundMgr*			m_pSoundMgr = nullptr;
	CFileInfoMgr*		m_pFileIoMgr = nullptr;

public:
	static void Release_Engine();
	virtual void Free() override;
};

END