#pragma once

#include "Scene.h"
#include "GameObject.h"


#ifdef USE_IMGUI
#include "ImguiMgr.h"
#endif // USE_IMGUI


//const char* DataFilePath = "../bin/Resources/Data/";
//const _tchar* MeshFilePath = L"../bin/Resources/Mesh/";


BEGIN(Client)
class CESCursor;
class CScene_Edit final : public CScene
{

#ifdef USE_IMGUI


	enum eDATATYPE
	{
		Data_Map,
		Data_UI,
		Data_Particle_Mesh,
		Data_Particle_Texture,
		Data_CameraAction,
		Data_HeightMap,
		Data_FilterMap,
		Data_Navigation,
		Datatype_End
	};


#endif



private:
	explicit CScene_Edit(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	virtual ~CScene_Edit() = default;

public:
	virtual HRESULT Initialize();
	virtual _int Update(_double fDeltaTime)override;
	virtual _int LateUpdate(_double fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;


	virtual _int Change_to_NextScene() override;

//imgui 세팅 관련


#ifdef USE_IMGUI
private:

#pragma region Total

	HRESULT Update_First_Frame(_double fDeltatime, const char* szFrameBarName);


	void Make_HelpWidget(const char* szString);
	void Make_VerticalSpacing(_uint count);
	HRESULT Sava_Data(const char* szFileName, eDATATYPE iKinds);
	HRESULT Load_Data(const char* szFileName, eDATATYPE iKinds);
	HRESULT Input_KeyBoard(_double fDeltaTime);


private:
	CGameInstance* m_pGameInstance = nullptr;
	_uint m_iNowTab = 0;
	_bool	Prevent_Order = false;
	_bool bArrWindowFlag[10];
	_uint ibClickChecker = 0;
	list<string>		m_FilePathList;
#pragma endregion Total

#pragma region MapTab
	HRESULT Update_MapTab(_double fDeltatime);
	HRESULT Widget_SRT(_double fDeltatime);

	HRESULT Widget_BatchedObjectList(_double fDeltatime);

	HRESULT Widget_CreateDeleteObject(_double fDeltatime);
	HRESULT Widget_SaveLoadMapData(_double fDeltatime);

	HRESULT	RenewElenmetTransform(OBJELEMENT* pObjElement);

private:
	_int m_bIsModelMove = 0;
	_int m_iKindsOfMoving = 0;
	_int m_iSelectedXYZ = 0;
	_int m_iPassIndex = 0;
	_float m_fFrustumRange = 5.f;
	_bool	m_bIsOcllusion = false;
	_float m_ArrBuffer[4];
	_float4x4* m_SelectedObjectSRT = nullptr;

	_uint m_iBatchedVecIndex = 0;
	vector<OBJELEMENT>		m_vecBatchedObject;

	_uint m_iSelectedObjectNMesh[2];
	_float	m_fPickingedPosition[3] = {0};
#pragma endregion MapTab

#pragma region UITab


	HRESULT Ready_Layer_RendererEditUI(const _tchar* pLayerTag);

	HRESULT Update_UITab(_double fDeltatime);

	HRESULT Widget_LoadUISRVTexture(_double fDeltatime);
	HRESULT Widget_CreateNBatchUI(_double fDeltatime);
	HRESULT Widget_DeleteBatchedUI(_double fDeltatime);



	HRESULT LoadTextureByAssimp(const _tchar* FileFullPath);

	ID3D11ShaderResourceView*  m_TargetSRV = nullptr;
	vector<EDITUI>		m_vecBatchedUI;
	_uint					m_iBatchedUIIndex = 0;
	char			m_szTexturPath[MAX_PATH] = "";
	class	CRendererEditSceneUI* m_pRendererEditUI = nullptr;

	UIDESC				m_fUIDesc ;
	FLOATRECT			m_fUIRect ;
	_float				m_fUIAngle = 0;
	_float				m_fDepth = 0;
	_bool				m_bIsRect = false;


	_bool				m_bModifyBatchedUI = false;
	_int				m_iModifyUIIndex= 0;

#pragma endregion UITab

#pragma region ParticleTab
	HRESULT Update_ParticleTab(_double fDeltatime);
	HRESULT Widget_SettingParticleDesc(_double fDeltatime);
	HRESULT Widget_ModelParticleDesc(_double fDeltatime);


	HRESULT Widget_SaveLoadTextureParticle(_double fDeltatime);
	HRESULT Widget_SaveLoadMeshParticle(_double fDeltatime);

	INSTPARTICLEDESC m_tParticleDesc;
	INSTMESHDESC m_tMeshDesc;

#pragma endregion ParticleTab

#pragma region CamTab
	HRESULT Update_CameraActionTab(_double fDeltatime);

	HRESULT Widget_CursorBatch(_double fDeltatime);
	HRESULT Widget_CreatedCamPosListBox(_double fDeltatime);
	HRESULT Widget_Play(_double fDeltatime);

	HRESULT Widget_SaveLoadCamAction(_double fDeltatime);

	
	CAMACTDESC CamDesc = { 0 };

	_int iPickKinds = 0;
	_uint iCamPosIndex = 0;
	_uint iCamLookIndex = 0;
	
	CESCursor*		m_pCamCursor = nullptr;
	vector<CESCursor*> m_vecCamPosBatchedObj;
	vector<CESCursor*> m_vecLookBatchedObj;


	vector<CAMACTDESC>		 m_vecCamPositions;
	vector<CAMACTDESC>		 m_vecLookPostions;


#pragma endregion CamTab


#pragma region HeightMap


	HRESULT Update_HeightMap(_double fDeltatime);
	HRESULT Widget_CreateDeleteHeightMap(_double fDeltatime);

private:
	_int		m_iMapSize[2];
	class CEditorTerrain* m_pCreatedTerrain = nullptr;

////////////////////////////////////////////////////////////////JJB
	void Add_Vertex();
	//시계방향정렬 함수
	_int CCWClockSort();
	HRESULT Ready_Layer_NaviPoint(_float3 Pos);
	HRESULT Ready_Layer_LoadNaviPoint(_float3 Pos);

	_int		m_iRadioNumMoving = 0;
	_int		m_iPickingModeNumber = 0;
	_int		m_iVertexAndCell = 0;
	_float4		m_vTerrainPos;
	_float4		m_vTerrainRotation;
	_float		m_TerrainArrBuffer[4];
	_float4x4	m_TerrainObjectSRT;

	//네비메쉬
	vector<char*>				m_vVertexs;//버틱스리스트
	vector<char*>				m_vCellNames;//셀이름리스트
	vector<class CNaviPoint*>	m_Points;
	vector<class CCell*>		m_Cells;
	CCell*						m_PreveCell = nullptr;

	class CNaviPoint*			m_pPointObj = nullptr;

	_float3						m_vVertexts[3];
	_int						m_iVertexListCount = 0;
	_int						m_iCellListCount = 0;
	_float						m_VertexArrBuffer[4];
	_int						m_iVertexIndex = 0;
	_bool						m_bIsCellListClick = false;

	_int						m_iVertexCount = 0;
	_int						m_iCellCount = 0;

	_bool						m_bIsCellOption = false;
	_int						m_OptionNumber = 0;

#ifdef _DEBUG
private:
	class CVIBuffer_Triangle*		m_pVIBuffer = nullptr;
	class CShader*					m_pShader = nullptr;

#endif // _DEBUG
////////////////////////////////////////////////////////////////

#pragma endregion HeightMap


#endif // USE_IMGUI


private:
	class CCamera_Editor*				m_pEditorCam = nullptr;

private:
	HRESULT Ready_Layer_MainCamera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Player(const _tchar* pLayerTag);
	HRESULT Ready_CamActionCursor(const _tchar* pLayerTag);
	HRESULT Ready_ParticleDesc();


public:
	static CScene_Edit* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	virtual void Free() override;
};
END
