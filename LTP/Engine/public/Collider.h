#pragma once

#include "Component.h"


BEGIN(Engine)
class CColliderBuffer;

class ENGINE_DLL CCollider final : public CComponent
{
private:
	CCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CCollider(const CCollider& rhs);
	virtual ~CCollider() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize_Clone(void* pArg)override;
	void Update_Transform(_uint iIndex, _fMatrix& Transform);
	void Update_ConflictPassedTime(_double fDeltaTime);

	_bool Inspect_Collision( CCollider* pTargetCollider, _uint iBufferIndex = 0, _uint iTargetIndex = 0, _uint2* pOutIndex = nullptr);

	HRESULT Set_ParantBuffer(_uint iParantIndex = 0, _int iIndex = -1);

	_uint Get_NumColliderBuffer() { return _uint(m_vecColliderBuffer.size()); };
	_int Add_ColliderBuffer(COLLIDERTYPE eColliderType, COLLIDERDESC* pColliderDesc = nullptr);
	void Set_Conflicted(_double TotalTime = 0.5);
	_bool Get_Conflicted() {return m_bIsConflicted;}

	_float3 Get_ColliderPosition(_uint iIndex = 0);

#ifdef _DEBUG
public:
	virtual HRESULT Render() override;
#endif // _DEBUG

private:
	vector<CColliderBuffer*>    m_vecColliderBuffer;
	vector<CColliderBuffer*>	m_vecPrototypeCollideBuffer;

private:
	_bool						m_bIsConflicted = false;
	_double						m_ConflictedPassedTime = 0;

private:
	_bool Inspect_ChildBuffer(_uint iBufferIndex, CCollider* pTargetCollider, _uint iTargetIndex,_uint2* pOutIndex);

public:
	static CCollider* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END