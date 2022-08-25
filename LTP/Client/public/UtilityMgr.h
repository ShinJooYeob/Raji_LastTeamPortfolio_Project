#pragma once

#include "Base.h"

BEGIN(Client)

class CUtilityMgr final :public CBase
{
	DECLARE_SINGLETON(CUtilityMgr)
public:
	enum E_DEBUGTIMER
	{
		DEBUGTIMER_1,
		DEBUGTIMER_2,
		DEBUGTIMER_3,
		DEBUGTIMER_END,
	};
	enum UTILTEXTUREID
	{
		UTILTEX_NOISE,
		UTILTEX_MASK,
		UTILTEX_MINIGAMETEX,
		UTILTEX_END,
	};


private:
	typedef struct tagLoadParticleSturct
	{
		wstring szFileName;
		wstring szFilePath;
	}LOADPARTICLE;

private:
	explicit CUtilityMgr();
	virtual ~CUtilityMgr() = default;

public:
	HRESULT Initialize_UtilityMgr(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, class CMainApp*	pMainApp);


	/*For Random*/
	_float	RandomFloat(_float Min, _float Max);
	_float3	RandomFloat3(_float Min, _float Max);
	_float3	RandomFloat3(_float3 Min, _float3 Max);

	/*For MinMax*/
	static _float	MinFloat(_float a, _float b);
	static _float	MaxFloat(_float a, _float b);


	/*For SlowMotion*/
	void SlowMotionStart(_float fTargetTime = 0.5f, _float TargetSpeed = 0.0f, _float TargetCurveRate = 0.5f);
	_float Get_SlowMotionSpeed();
	_bool Get_IsSlowed();


	/*For Debug_Timer*/
	void Start_DebugTimer(E_DEBUGTIMER type);
	void End_DebugTimer(E_DEBUGTIMER type, wstring debugLog = L"");


	/*For DebugMode*/
	CRenderer* Get_Renderer() { return m_pRenderer; }


	/*For Particle*/
	HRESULT Create_TextureInstance(_uint eSceneID, INSTPARTICLEDESC& tParticleDesc);
	HRESULT Create_TextureInstance(_uint eSceneID, INSTPARTICLEDESC& tParticleDesc, vector<_float3>* vPosition);
	HRESULT Create_MeshInstance(_uint eSceneID, INSTMESHDESC& tParticleDesc);

	/*For Utility Texture */
	HRESULT Bind_UtilTex_OnShader(UTILTEXTUREID eID, CShader* pShader, const char* szhlslConstName, _uint iTextureIndex = 0);
	ID3D11ShaderResourceView* Get_UtilTex_SRV(UTILTEXTUREID eID, _uint iTextureIndex = 0);


	/*For Dissolve*/
	HRESULT Bind_DissolveTex_OnShader(CShader* pShader, _uint iRampTextureIndex);


	INSTPARTICLEDESC	Get_TextureParticleDesc(const _tchar* szFileName);
	INSTMESHDESC		Get_MeshParticleDesc(const _tchar* szFileName);


	/*For SCD*/
	HRESULT Copy_LastDeferredToToonShadingTexture(_float fToonRate, _bool bIsSecond = false);
	HRESULT SCD_Rendering();
	HRESULT SCD_Rendering_Rolling(_float RollingStartTime, _float RollingTargetTime, const _tchar* szRenderTargetTag);
	HRESULT SCD_Rendering_FadeOut(_float RollingStartTime, _float RollingTargetTime, const _tchar* szRenderTargetTag);
	//HRESULT SCD_Rendering_Rolling(_float RollingStartTime, _float RollingTargetTime);


	/* For Get World Object */
	class CCamera_Main* Get_MainCamera();


	/*For Radial Blur*/
	void Set_RadialBlurTargetPos_ByWorldPos(_float3 vPos);
	void Set_IsRadialBlurFadeIn(_bool bBool, _float fZoomRadialSize = 0.25f, _float fZoomPower = 0.7f, _float TargetTimer = 1.f);


	void Set_HitEffect(_float Intensive = 0.2f, _float HitTime = 0.5f, _float3 vColor = _float3(1,0,0));
	

public:
	HRESULT Clear_RenderGroup_forSceneChange();
	void Set_Renderer(CRenderer* pRenderer);

public:
	CVIBuffer_Rect*			Get_VIBuffer_Rect() {	return m_pVIBufferRect;	};
	CShader*				Get_RectShader() { return m_pRectShader; };

public:
	_int* Get_FireSkillPointArry() { return m_FireSkillPoint; }
	_int* Get_IceSkillPointArry() { return m_IceSkillPoint; }
	_int* Get_LightningSkillPointArry() { return m_LightningSkillPoint; }
	_int*  Get_SkillPoint() { return &m_iSkillPoint; }
	WeaponType Get_SkillWeaponType() { return m_eSkillWeaponType; }

public:
	void	Set_FireSkillPointArry(_int iArrayNum, _int iCount) { m_FireSkillPoint[iArrayNum] = iCount; }
	void	Set_IceSkillPointArry(_int iArrayNum, _int iCount) { m_FireSkillPoint[iArrayNum] = iCount; }
	void	Set_LightningSkillPointArry(_int iArrayNum, _int iCount) { m_FireSkillPoint[iArrayNum] = iCount; }
	void	Set_SKillPoint(_int PointCount) { m_iSkillPoint = PointCount; }
	void	Set_SkillWeaponType(WeaponType Type) { m_eSkillWeaponType = Type; }

public:
	void	PlusSpearSkillPersent(_float fPersent = 5.f) { m_fSpearSkillPersent += (5.f + (5.f * 0.25f * m_FireSkillPoint[2])); }
	void	PlusBowSkillPersent(_float fPersent = 5.f) { m_fBowSkillPersent += (5.f + (5.f * 0.25f * m_IceSkillPoint[2])); }
	void	PlusSwordshieldSkillPersent(_float fPersent = 5.f) { m_fSwordshieldSkillPersent += (5.f + (5.f * 0.25f * m_LightningSkillPoint[2])); }

	void	ResetSpearPersent() { m_fSpearSkillPersent = 0.f; }
	void	ResetBowPersent() { m_fBowSkillPersent = 0.f; }
	void	ResetSwordshieldPersent() { m_fSwordshieldSkillPersent = 0.f; }

	_float	Get_SpearSkillPersent() { return m_fSpearSkillPersent; }
	_float	Get_BowSkillPersent() { return m_fBowSkillPersent; }
	_float	Get_SwordshieldSkillPersent() { return m_fSwordshieldSkillPersent; }

	HRESULT Ready_SceneChangingData(_uint eSceneID,_uint iLayerIndex);
	PPDDESC Set_SceneChangingData(_uint eSceneID);

private:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;
	class CMainApp*			m_pMainApp = nullptr;
	CRenderer*				m_pRenderer = nullptr;
	CTexture*				m_pTexture = nullptr;
	CTexture*				m_pDissolveTexture = nullptr;

	SCDDESC				m_pLoadingSCD;

	CVIBuffer_Rect*			m_pVIBufferRect = nullptr;
	CShader*				m_pRectShader = nullptr;

	PPDDESC					m_tForSceneChangingData;

	class CRadialBlurUI*		m_pRadialUI = nullptr;
	class CHitEffectUI*			m_pHitEffectUI = nullptr;

	map<wstring, INSTPARTICLEDESC>				m_mapTextureParticles;
	typedef map<wstring, INSTPARTICLEDESC>		TEXPARTICLES;
	map<wstring, INSTMESHDESC>					m_mapMeshParticles;
	typedef map<wstring, INSTMESHDESC>			MESHPARTICLES;

private:
	clock_t m_StartTime[E_DEBUGTIMER::DEBUGTIMER_END];

private:
	_int		m_FireSkillPoint[3] = {0,0,0};
	_int		m_IceSkillPoint[3] = { 0,0,0 };
	_int		m_LightningSkillPoint[3] = { 0,0,0 };

	_int		m_iSkillPoint = 18;

	WeaponType  m_eSkillWeaponType = WEAPON_BOW;

private:
	_float		m_fSpearSkillPersent = 0.f;
	_float		m_fBowSkillPersent = 0.f;
	_float		m_fSwordshieldSkillPersent = 0.f;

public:
	HRESULT Ready_Particles();
private:
	_uint CountDigit(_uint iNum);
	HRESULT Ready_TextureParticles();
	HRESULT Ready_MeshParticles();

public:
	virtual void Free()override;

};

END