#pragma once

#include "Component.h"


BEGIN(Engine)
class CMotionTrailBuffer;
class CModel;
class CShader;

class ENGINE_DLL CMotionTrail final : public CComponent
{
public:
	typedef struct tagMotionTrailDesc
	{
		_uint		iNumTrailCount = 0;
		CModel*		pModel = nullptr;
		CShader *	pShader=	nullptr;
		_uint		iPassIndex = 5;
		_float4		vLimLight = _float4(1,0,0,1);

	}MOTIONTRAILDESC;

private:
	CMotionTrail(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CMotionTrail(const CMotionTrail& rhs);
	virtual ~CMotionTrail() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize_Clone(void* pArg)override;

	void Add_MotionBuffer(_float4x4& WorldMatrix, _double TargetTime = 2.5f);
	void Update_MotionTrail(_double fDeltaTime);
	virtual HRESULT Render() override;

private:
	MOTIONTRAILDESC							m_tTrailDesc;

private:
	list<CMotionTrailBuffer*>				m_TrailBufferList;
	typedef list<CMotionTrailBuffer*>		MTRAILBUFFERS;

private:
	HRESULT		Ready_TrailBuffer();	


public:
	static CMotionTrail* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END