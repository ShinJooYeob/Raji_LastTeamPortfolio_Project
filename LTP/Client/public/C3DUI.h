#pragma once
#include "GameObject.h"

class C3DUI final : public CGameObject
{
public:
	typedef struct tagSettingUI
	{
		_uint		iLevelIndex = 0;

		_tchar*		pUI_Name = nullptr;
		_int		iTextureIndex = 0;

		_float3		Position;
		_float3		Scaled;
		_float		fAngle = 0.f;

		_bool		bColl = false;
		_bool		bMove = false;
		_bool		bDraw = false;
		_bool		bClick = false;

		_bool		bSettingOtherTexture = false;
		_float		fAlpha = 1.f;
		_uint		iNoiseTextureIndex = 0;
		_uint		iMaskTextureIndex = 0;
	}SETTING_UI;

private:
	C3DUI(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	C3DUI(const C3DUI& rhs);
	virtual ~C3DUI() = default;


public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	virtual _int Update(_double fDeltaTime)override;
	virtual _int LateUpdate(_double fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

public:
	_tchar*			Get_UIName();
	_int			Get_TextureIndex();
	_float3			Get_Pos();
	_float3			Get_Scaled();
	_bool			Get_IsDraw();
	_float			Get_Alpha();
	_float4			Get_Color();


	void			Set_PassIndex(_int Index) { m_iPassIndex = Index; }
	void			Set_UIPosition(_float3 Position) { m_SettingUIDesc.Position = Position; }
	void			Set_TextureIndex(_int Number) { m_iTextureIndex = Number; }
	void			Set_Scaled(_float3 Scaled) { m_SettingUIDesc.Scaled = Scaled; }
	void			Set_Alpha(_float fAlpha) { m_SettingUIDesc.fAlpha = fAlpha; }
	void			Set_IsDraw(_bool Draw) { m_SettingUIDesc.bDraw = Draw; }
	void			Set_IsPushed(_bool Pushed) { m_SettingUIDesc.bClick = Pushed; }

	void			Set_UV_Y(_float fY);
	void			Set_Angle(_float3 vAxis, _float fAngle);

	void			Set_Color(_float4 Color) { m_vColor = Color; }

private:
	CShader*						m_pShaderCom = nullptr;
	CRenderer*						m_pRendererCom = nullptr;
	CTransform*						m_pTransformCom = nullptr;
	CTexture*						m_pTextureCom = nullptr;
	CVIBuffer_Rect*					m_pVIBufferCom = nullptr;

private:
	_int		m_iTextureIndex = 0;
	_int		m_iPassIndex = 1;

	_bool		m_bIsCahngeUV_Y = false;
	_float		m_fY_EsingTime = 0.f;
	_float		m_fGoalUV_Y = 0;
	_float		m_fUV_Y = 1.f;
	_float4		m_vColor = _float4(1.f, 1.f, 1.f, 1.f);

	_float		m_PassedTimer = 0;

	//디졸브
	_float				m_fTotalTargetTime = 7.f;
	_float				m_fApearTime = 0.35f;
	_float				m_fDisApearTime = 1.5f;

private:
	SETTING_UI						m_SettingUIDesc;

private:
	HRESULT SetUp_Components();

private:
	HRESULT SettingTexture();							//마스크,노이즈 텍스처
	void EsingUV_Y(_double fDeltaTime);

public:
	static C3DUI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

