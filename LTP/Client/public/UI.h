#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
class CTransform;
END

BEGIN(Client)

class CUI : public CGameObject
{
public:
	enum UITYPE {UI_2D, UI_3D, UI_END};
public:
	enum UIID
	{
		UIID_JB, UIID_JY_Lobby, UIID_MINIGAME_TAIKO, UIID_MINIGAME_TAIKODRUM, UIID_EFFECT, UIID_END
	};

	typedef struct tagSettingUI
	{
		_uint		iLevelIndex = 0;
		//_tchar*		pPrototypeTag = nullptr;
		//_tchar*		pComponentTag = nullptr;
		_tchar*		pUI_Name = nullptr;
		_int		iTextureIndex = 0;

		_float		m_fX, m_fY, m_fSizeX, m_fSizeY;
		_float		fAngle = 0.f;

		_bool		bColl = false;
		_bool		bMove = false;
		_bool		bDraw = false;
		_bool		bClick = false;
		
		_uint		eUIKindsID = UIID_JB;
		_uint		eUIType = UI_2D;
		_bool		bSettingOtherTexture = false;
		_float		fAlpha = 1.f;
		_uint		iNoiseTextureIndex = 0;
		_uint		iMaskTextureIndex = 0;

		//3D
		_float3		v3DUIPosition = _float3(0);
		_float3		v3DUIScaled = _float3(0);


	}SETTING_UI;

private:
	CUI(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CUI(const CUI& rhs);
	virtual ~CUI() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	virtual _int Update(_double fDeltaTime)override;
	virtual _int LateUpdate(_double fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

public:
	_tchar*			Get_UIName() { return m_pUIName; }
	_int			Get_TextureIndex() { return m_iTextureIndex; }
	_float			Get_PosX() { return m_fX; }
	_float			Get_PosY() { return m_fY; }
	_float2			Get_Size() { return _float2(m_fSizeX, m_fSizeY); }
	_bool			Get_IsDraw() { return m_bDraw; }
	_bool			Get_FadeState() { return m_bFadeState; }
	_float			Get_Alpha() { return m_SettingUIDesc.fAlpha; }
	_float4			Get_Color() { return m_vColor; }

	_bool			Get_IsRevolutionCW() {
		return m_bIsRevolutionCW;
	}

	_bool			Get_IsRevolutionCCW() {
		return m_bIsRevolutionCCW;
	}

public:
	void			Set_PassIndex(_int Index) { m_iPassIndex = Index; }
	void			Set_UIPosition(_float fX, _float fY) { m_fX = fX, m_fY = fY; }
	void			Set_TextureIndex(_int Number) { m_iTextureIndex = Number; }
	void			Set_Size(_float SizeX, _float SizeY) { m_fSizeX = SizeX, m_fSizeY = SizeY; }
	void			Set_Angle(_float fAngle);
	void			Set_Alpha(_float fAlpha) { m_SettingUIDesc.fAlpha = fAlpha; }

	void			Set_IsDraw(_bool Draw) { m_bDraw = Draw; }
	void			Set_IsPushed(_bool Pushed) { m_bPushed = Pushed; }

	void			Set_UV_Y(_float fY);

	void			Set_Color(_float4 Color) { m_vColor = Color; }

	void			Set_IsRevolutionCCW(_bool State) { m_bIsRevolutionCCW = State; }
	void			Set_IsRevolutionCW(_bool State) { m_bIsRevolutionCW = State; }

public:
	_float3			Get_3DPos() { return  m_SettingUIDesc.v3DUIPosition; }
	_float3			Get_3DScaled() { return  m_SettingUIDesc.v3DUIScaled; }

	void			Set_3DScaled(_float3 Scaled) { m_SettingUIDesc.v3DUIScaled = Scaled; }
	void			Set_3DPosition(_float3 Pos) { m_SettingUIDesc.v3DUIPosition = Pos; }

	HRESULT			Set_ChangeTextureLayer(_tchar* LayerName);

	void			Set_RenderSortValue(_float Valuew) {
		m_fRenderSortValue = Valuew;
	}

	void			Set_RevolutionStartEndAngle(_float StartAngle, _float GoalAngel)
	{
		m_fRevolutionStartAngle = StartAngle;
		m_fRevolutionGoalAngle = GoalAngel; 
	}

	void			UI_RevolutionCCWInitialization(_float Angle);
	void			UI_RevolutionCWInitialization(_float Angle);

	void			TimeDrawStart(_float Time)
	{
		m_bIsDrawTimeOn = true;
		m_fDrawTime = Time;
	}
	
	void			TimeDraw(_double TimeDelta);

private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;

private:
	_float4x4	m_ProjMatrix;
	_float		m_fX, m_fY, m_fSizeX, m_fSizeY;

	SETTING_UI	m_SettingUIDesc;

	_tchar*		m_pUIName = nullptr;		// UI 이름
	_uint		m_iLevelIndex = 0;
	//_tchar*		m_pPrototypeTag = nullptr;
	//_tchar*		m_pComponentTag = nullptr;

	_bool		m_bMove;					// 이동 가능 여부. 이게 켜져야 이동가능함. UI 위치 배치용.
	_bool		m_bDraw;					// 렌더 여부
	_bool		m_bClick;					// 클릭 기능이 있는 UI만 true로 설정
	_bool		m_bColl = false;			//마우스 충돌 체크 여부
	_bool		m_bPushed = false;			//클릭이 되면 true로 바꾼다.

	_int		m_iTextureIndex = 0;
	_int		m_iPassIndex = 1;

	_float		m_fFadeTime = 0.f;
	_bool		m_bFadeState = false;
	_float		m_fAngle = 0.f;


	_bool		m_bIsCahngeUV_Y = false;
	_float		m_fY_EsingTime = 0.f;
	_float		m_fGoalUV_Y = 0;
	_float		m_fUV_Y = 1.f;
	_float4		m_vColor = _float4(1.f, 1.f, 1.f, 1.f);

	_float		m_PassedTimer = 0;	

	//시간지나면 그리지않기위한 변수
	_bool				m_bIsDrawTimeOn = false;
	_float				m_fDrawTime = 0.f;

	//디졸브
	_float				m_fTotalTargetTime = 7.f;
	_float				m_fApearTime = 0.35f;
	_float				m_fDisApearTime = 1.5f;

	//공전
	_bool				m_bIsRevolutionCCW = false;
	_bool				m_bIsRevolutionCW = false;
	_float				m_fRevolutionStartAngle = 0.f;
	_float				m_fRevolutionGoalAngle = 0.f;

private:
	HRESULT SetUp_Components();
	void	UI_RevolutionCCW();
	void	UI_RevolutionCW();

private:
	void SetUp_UIInfo(SETTING_UI& pStruct);			// 얻어온 구조체 값으로 멤버변수 채우기
	void Update_Rect();								// 위치 바꿨을 때 렉트 위치 갱신하기 위해 호출
	void Update_Rect3D();
	HRESULT SettingTexture();							//마스크,노이즈 텍스처
	void EsingUV_Y(_double fDeltaTime);

public:
	static CUI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END