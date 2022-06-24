#pragma once

#include "Shader.h"
#include "VIBuffer_Rect.h"

BEGIN(Engine)

class CLight final : public CBase
{
private:
	CLight(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CLight() = default;

public:
	LIGHTDESC* Get_LightDesc() {return &m_LightDesc;}

public:
	HRESULT Initilize_Protoype(const LIGHTDESC& LightDesc);


public:
	HRESULT Render(CShader* pShader, CVIBuffer_Rect* pVIBuffer, _double fDeltaTime);


private:
	ID3D11Device*				m_pDevice = nullptr;
	ID3D11DeviceContext*		m_pDeviceContext = nullptr;
	LIGHTDESC					m_LightDesc;

	_float						m_PassedTime = 0;
	_float						m_TargetTime = 0;

public:
	static CLight* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const LIGHTDESC& LightDesc);
	virtual void Free() override;
};


END