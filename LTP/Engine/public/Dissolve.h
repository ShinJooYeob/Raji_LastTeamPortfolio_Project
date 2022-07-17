#pragma once

#include "Component.h"


BEGIN(Engine)
class CModel;
class CShader;
class CTexture;

class ENGINE_DLL CDissolve final : public CComponent
{
public:
	enum DISSOLVETYPE
	{
		DISSOLVE_ANIM, DISSOLVE_ANIM_ATTACHED, DISSOLVE_NONANIM, DISSOLVE_NONANIM_ATTACHED
		,DISSOLVE_END
	};

public:
	typedef struct tagDissolveDesc
	{
		DISSOLVETYPE eDissolveModelType = DISSOLVE_END;
		CModel*		pModel = nullptr;
		CShader*	pShader = nullptr;

		_uint		RampTextureIndex = 1;
	}DISSOLVEDESC;




private:
	CDissolve(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CDissolve(const CDissolve& rhs);
	virtual ~CDissolve() = default;

private:
	virtual HRESULT Initialize_Prototype(const _tchar* szDissolvePrototypeTag);
	virtual HRESULT Initialize_Clone(void * pArg)override;

public:
	HRESULT Update_Dissolving(_double fDeltaTimme);
	_int Render(_uint iPassIndex);
	_int Render_Shadow(_uint iPassIndex);

public:
	HRESULT	Set_DissolveOn(_bool bFadeIn, _double TargetTime);
	_bool	Get_IsDissolving() { return m_bIsDissolving; }
	_double	Get_DissolvingRate() { return m_PassedTime / m_TargetTime; }
	_bool	Get_IsFadeIn() { return m_bIsFadeIn; };


private:
	wstring			m_szTexturePrototypeTag = L"";
	CTexture*		m_pDissolveTexture = nullptr;


	
private:
	_bool			m_bIsDissolving = false;
	_bool			m_bIsFadeIn = false;
	_double			m_TargetTime = 0;
	_double			m_PassedTime = 0;
	_uint			m_iDissolvePassedIndex = 0;
	_uint			m_iShadowPassIndex = 0;

	DISSOLVEDESC	m_tDissolveDesc;


public:
	static CDissolve* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext,const _tchar* szDissolvePrototypeTag);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END






