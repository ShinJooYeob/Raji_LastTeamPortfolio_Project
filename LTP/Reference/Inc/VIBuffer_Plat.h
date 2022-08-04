#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Plat final : public CVIBuffer
{
public:
	CVIBuffer_Plat(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CVIBuffer_Plat(const CVIBuffer_Plat& rhs);
	virtual ~CVIBuffer_Plat() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	_float Get_OnPlatHeight(_fVector vPos);

public:
	static CVIBuffer_Plat* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END