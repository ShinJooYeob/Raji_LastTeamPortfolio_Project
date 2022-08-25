#pragma once
#include "GameObject.h"

BEGIN(Client)

class CSpeechUI final : public CGameObject
{
public:
	typedef struct tagSpeechFontDesc
	{
		_tchar*		Text = nullptr;
		_float		LlveingTime = 0.f;
		_float2		TextPos;
		_float4		vColor = _float4(1);
		_float2		vFontScale = _float2(1);

	}SPEECHFONTDESC;
private:
	CSpeechUI(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CSpeechUI(const CSpeechUI& rhs);
	virtual ~CSpeechUI() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	virtual _int Update(_double fDeltaTime)override;
	virtual _int LateUpdate(_double fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

public:
	HRESULT Render_Fonts();

private:
	CRenderer*		m_pRendererCom = nullptr;

	SPEECHFONTDESC	m_FonstDesc;

	FONTSDESC		m_SkillNameFont;

private:
	HRESULT SetUp_Components();


public:
	static CSpeechUI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END