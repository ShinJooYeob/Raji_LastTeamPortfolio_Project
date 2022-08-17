#pragma once
#include "GameObject.h"

BEGIN(Client)

class CUI_Texture_Universal final : public CGameObject
{
public:
	enum UI_TextureType{
		UI_SKILL_FIREBALL,
		UI_SKILL_BARRIERBULLET,
		UI_SKILL_BLACKHOLE,
		UI_Key,
		UI_SKILL_COOLTIME,
		UI_TEXT,
		UI_END
	};
public:
	typedef struct tagUI_Texture_UniversalDesc
	{
		_uint		iUI_TextureType;
		_uint		iTextureIndex;

		_float		fSizeX;
		_float		fSizeY;
		_float		fX;
		_float		fY;
		_float		fDepth; //숫자가 큰 게 앞으로
		_double		dDuration;
		void*		pGameObject = nullptr;
	}UI_TEXTURE_UNIVERSALDESC;
private:
	explicit CUI_Texture_Universal(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CUI_Texture_Universal(const CUI_Texture_Universal& rhs);
	virtual ~CUI_Texture_Universal() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	virtual _int Update(_double dDeltaTime)override;
	virtual _int LateUpdate(_double dDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

public:
	_float	Get_SizeX() { return m_fSizeX; }

private:
	HRESULT				SetUp_Components();
	HRESULT				Set_UI_Transform();

private:
	HRESULT				Play_UI(_double dDeltaTime);	

private:
	HRESULT				Frame_Speed(_double dDeltaTime);

private:
	HRESULT				UI_Skill_CoolTime(_double dDeltaTime);
	HRESULT				UI_Text(_double dDeltaTime);

private:
	CShader*			m_pShaderCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;

private:
	UI_TEXTURE_UNIVERSALDESC	m_UI_UniversalDesc;

	_float4x4			m_ProjMatrix;
	_float				m_fX, m_fY, m_fSizeX, m_fSizeY;

	//사이즈의 맥스
	_float				m_fMaxSizeX;
	_float				m_fMaxSizeY;


	_double				m_dDurationTime = 0;

	//Easing
	_double				m_dEasingTime = 0;

	_double				m_AccumulationTime = 0;


public:
	static CUI_Texture_Universal* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END