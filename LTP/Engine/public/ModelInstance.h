#pragma once

#include "Component.h"


BEGIN(Engine)
class CModel;
class CVIBuffer_Model_Instance;
class CTransform;

class ENGINE_DLL CModelInstance final : public CComponent
{
public:
	typedef struct tagModelInstDesc
	{
		CModel*		m_pTargetModel = nullptr;

	}MODELINSTDESC;

private:
	CModelInstance(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CModelInstance(const CModelInstance& rhs);
	virtual ~CModelInstance() = default;

public:
	virtual HRESULT Initialize_Prototype(_uint LayerSceneNum, _uint iNumInstance);
	virtual HRESULT Initialize_Clone(void* pArg)override;

	HRESULT Render(class CShader* pShader, _uint iPassIndex, vector<CTransform*>* pvecWorldMatrixs, _float fFrustumsize = 0, 
		vector<_float4>*  pvecLimLight = nullptr, vector<_float4>*  pvecEmissive = nullptr, vector<_float4>*  pvecTimmer = nullptr,
	_bool bPrimitiveCullingOn = false);

	HRESULT Render_By_float4x4(class CShader* pShader, _uint iPassIndex, vector<_float4x4>* pvecWorldMatrixs,
		_float fFrustumsize = 0, vector<_float4>*  pvecLimLight = nullptr, vector<_float4>*  pvecEmissive = nullptr, vector<_float4>*  pvecTiemer = nullptr,
		_bool bPrimitiveCullingOn = false);
private:
	wstring									m_szInstancePrototypeTag = L"";
	_uint									m_ePrototypeLayerIndex = 0;

	CVIBuffer_Model_Instance*				m_pInstanceBuffer = nullptr;
	MODELINSTDESC							m_tDesc;

private:




public:
	static CModelInstance* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint LayerSceneNum, _uint iNumInstance);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END