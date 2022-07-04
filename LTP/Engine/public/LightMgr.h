#pragma once


#include "Base.h"
BEGIN(Engine)

class CLightMgr final : public CBase
{
	DECLARE_SINGLETON(CLightMgr);

private:
	explicit CLightMgr();
	virtual ~CLightMgr() = default;


public:
	HRESULT Initialize_LightMgr(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);

public:
	HRESULT Render(class CShader* pShader, class CVIBuffer_Rect* pViBuffer, MATRIXWVP* pWVPMat, _double fDeltaTime);
public:
	LIGHTDESC* Get_LightDesc(LIGHTDESC::TYPE eLightType, _uint iIndex);
	HRESULT EasingDiffuseLightDesc(LIGHTDESC::TYPE eLightType, _uint iIndex, _fVector vTargetDiffuse, _float MixRate = 1.f);
	HRESULT Relocate_LightDesc(LIGHTDESC::TYPE eLightType, _uint iIndex, _fVector vRelocatePosition, _float fRange = 8.f);
	HRESULT Add_Light(const LIGHTDESC& LightDesc, LIGHTDESC** pOutDesc);


	_float3 Get_SunAtPoint() { return m_vSunAtPoint; };
	void Set_SunAtPoint(_float3 vVector) { m_vSunAtPoint = vVector; };

	HRESULT Clear_PointLightList();

private:
	list<class CLight*>			m_ArrLightList[LIGHTDESC::TYPE_END];
	typedef list<class CLight*>	LIGHTS;

	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pDeviceContext = nullptr;


	_float3						m_vSunAtPoint = _float3(10, -10, 10);

public:
	virtual void Free() override;
};

END