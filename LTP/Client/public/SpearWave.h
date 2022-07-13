#pragma once

#include "GameObject.h"

BEGIN(Client)

class CSpearWave final : public CGameObject
{
public:
	typedef struct tagSpearWaveDesc
	{
		_float3				fStartPos;
		_float3				fLookDir;
	}SPEARWAVEDESC;

private:
	CSpearWave(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CSpearWave(const CSpearWave& rhs);
	virtual ~CSpearWave() = default;

public:
	virtual HRESULT		Initialize_Prototype(void* pArg) override;
	virtual HRESULT		Initialize_Clone(void* pArg) override;

public:
	virtual _int		Update(_double fDeltaTime) override;
	virtual _int		LateUpdate(_double fDeltaTimer) override;
	virtual _int		Render() override;
	virtual _int		LateRender() override;

private:
	HRESULT				SetUp_Components();

private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CModel*				m_pModel = nullptr;
	CTransform*			m_pTransformCom = nullptr;

public:
	static CSpearWave*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject*			Clone(void* pArg);
	virtual void					Free() override;
};

END
