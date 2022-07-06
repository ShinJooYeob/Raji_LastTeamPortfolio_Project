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

	HRESULT Render(class CShader* pShader, _uint iPassIndex, vector<CTransform*>* pvecWorldMatrixs, _float fFrustumsize = 0);

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