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
	enum UIID
	{
		UIID_JB, UIID_JY_Lobby, UIID_END
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
		_bool		bSettingOtherTexture = false;
		_float		fAlpha = 1.f;
		_uint		iNoiseTextureIndex = 0;
		_uint		iMaskTextureIndex = 0;
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

public:
	void			Set_PassIndex(_int Index) { m_iPassIndex = Index; }
	void			Set_UIPosition(_float fX, _float fY) { m_fX = fX, m_fY = fY; }
	void			Set_TextureIndex(_int Number) { m_iTextureIndex = Number; }
	void			Set_Size(_float SizeX, _float SizeY) { m_fSizeX = SizeX, m_fSizeY = SizeY; }
	void			Set_Angle(_float fAngle);
	void			Set_Alpha(_float fAlpha) { m_SettingUIDesc.fAlpha = fAlpha; }

	void			Set_IsDraw(_bool Draw) { m_bDraw = Draw; }
	void			Set_IsPushed(_bool Pushed) { m_bPushed = m_bPushed; }

	void			Set_UV_Y(_float fY) { m_fUV_Y = fY; }

	HRESULT			Set_ChangeTextureLayer(_tchar* LayerName);

	void			Set_RenderSortValue(_float Valuew) {
		m_fRenderSortValue = Valuew;
	}

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

	_float		m_fAliveTime = 1.5f;			//데미지UI일때 보여지는 시간
	_float		m_fFadeTime = 0.f;
	_bool		m_bFadeState = false;
	_float		m_fAngle = 0.f;
	_float		m_fUV_Y = 1.f;

	_float		m_PassedTimer = 0;	

private:
	HRESULT SetUp_Components();

private:
	void SetUp_UIInfo(SETTING_UI& pStruct);			// 얻어온 구조체 값으로 멤버변수 채우기
	void Update_Rect();								// 위치 바꿨을 때 렉트 위치 갱신하기 위해 호출
	HRESULT SettingTexture();							//마스크,노이즈 텍스처

public:
	static CUI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END