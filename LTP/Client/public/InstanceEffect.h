#pragma once

#include "GameObject.h"


BEGIN(Client)
typedef struct tagInstanceParticleAttribute
{
	_float4x4		_LocalMatirx;
	_float4			_color = _float4(1.f, 1.f, 1.f, 1.f);
	_float3			_size = _float3(1, 1, 1);
	_float2			_TextureUV = _float2(1, 1);

	_float			_lifeTime;
	_float			_age;

	_float3			_velocity;
	_float			_force;
	_float			_Targetforce;

	_float			_CamDist = 0;

	_bool			_isAlive = true;
	
	_float3			_TargetParentPosition = _float3(0);
	_uint			_iIndex = 0;
}INSTANCEATT;


class CInstanceEffect abstract : public CGameObject
{
protected:
	CInstanceEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CInstanceEffect(const CInstanceEffect& rhs);
	virtual ~CInstanceEffect() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize_Clone(void* pArg);

	virtual HRESULT Initialize_Child_Clone()PURE;

	virtual _int Update(_double TimeDelta)override;
	virtual _int LateUpdate(_double TimeDelta)override;
	virtual _int Render() override;
	virtual _int LateRender() override;

	

	virtual void ResetParticle(INSTANCEATT* attribute);
	virtual void Reset_Velocity(_float3& fAttVlocity)PURE;

	virtual void Update_ParticleAttribute(_double fDeltaTime);
	virtual void Update_SizeChange(INSTANCEATT* tParticleAtt, _double fTimeDelta, CGameInstance* pInstance);
	virtual void Update_TextureChange(INSTANCEATT* tParticleAtt, _double fTimeDelta, CGameInstance* pInstance);
	virtual void Update_ColorChange(INSTANCEATT* tParticleAtt, _double fTimeDelta, CGameInstance* pInstance);
	virtual void Update_Position_by_Velocity(INSTANCEATT* tParticleAtt, _double fTimeDelta)PURE;


	HRESULT Set_AsMapParticle(vector<_float3>& vecMapParticlePosition);
	void Erase_Particle() { m_tInstanceDesc.TotalParticleTime = m_PassedTime; };


	void	Set_GonnabeDie() { m_tInstanceDesc.TotalParticleTime = m_PassedTime; };

protected:
	CShader*					m_pShaderCom = nullptr;
	CRenderer*					m_pRendererCom = nullptr;
	CTexture*					m_pTextureCom = nullptr;
	CVIBuffer_Point_Instance*	m_pVIBufferCom = nullptr;
	CTransform*					m_pTransformCom = nullptr;

protected:
	INSTPARTICLEDESC			m_tInstanceDesc;
	vector<INSTANCEATT>			m_vecParticleAttribute;
	_bool						m_bIsMapParitcle = false;
	vector<_float3>				m_vecMapParticlePosition;

	_uint						m_iNumInstance = 0;
	_float						m_PassedTime = 0;
	_float3						m_vRight = _float3(1, 0, 0);
	_float3						m_vLook = _float3(0, 0, 1);
	_float3						m_vUp = _float3(0, 1, 0);


protected:
	HRESULT SetUp_Components();
	HRESULT Ready_InstanceDesc();
	HRESULT SetUp_ConstantTable();


public:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free() override;
};


END










///////////구 형태 파티클///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma region BallParticle

BEGIN(Client)


class CInstanceEffect_Ball final : public CInstanceEffect
{
private:
	explicit CInstanceEffect_Ball(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CInstanceEffect_Ball(const CInstanceEffect_Ball& rhs);
	virtual ~CInstanceEffect_Ball() = default;

private:

	virtual void Reset_Velocity(_float3& fAttVlocity)override;
	virtual void Update_Position_by_Velocity(INSTANCEATT* tParticleAtt, _double fTimeDelta)override;


	virtual HRESULT Initialize_Child_Clone() override;
	//	virtual void ResetParticle(PARTICLEATT* attribute);

	virtual _int Update(_double fTimeDelta)override;
	virtual _int LateUpdate(_double fTimeDelta)override;
	// 랜더는 부모 것 사용


public:

	static CInstanceEffect_Ball* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject * Clone(void * pArg) override;
};
END

#pragma endregion


///////////직선 형태 파티클///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region StraightParticle

BEGIN(Client)


class CInstanceEffect_Straight final : public CInstanceEffect
{
private:
	explicit CInstanceEffect_Straight(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CInstanceEffect_Straight(const CInstanceEffect_Straight& rhs);
	virtual ~CInstanceEffect_Straight() = default;

private:

	virtual void Reset_Velocity(_float3& fAttVlocity)override;
	virtual void Update_Position_by_Velocity(INSTANCEATT* tParticleAtt, _double fTimeDelta)override;


	virtual HRESULT Initialize_Child_Clone() override;
	//	virtual void ResetParticle(PARTICLEATT* attribute);

	virtual _int Update(_double fTimeDelta)override;
	virtual _int LateUpdate(_double fTimeDelta)override;
	// 랜더는 부모 것 사용


public:
	static CInstanceEffect_Straight* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject * Clone(void * pArg) override;

};
END

#pragma endregion




///////////원뿔 형태 파티클///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region ConeParticle

BEGIN(Client)


class CInstanceEffect_Cone final : public CInstanceEffect
{
private:
	explicit CInstanceEffect_Cone(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CInstanceEffect_Cone(const CInstanceEffect_Cone& rhs);
	virtual ~CInstanceEffect_Cone() = default;

private:

	virtual void Reset_Velocity(_float3& fAttVlocity)override;
	virtual void Update_Position_by_Velocity(INSTANCEATT* tParticleAtt, _double fTimeDelta)override;


	virtual HRESULT Initialize_Child_Clone() override;
	//	virtual void ResetParticle(PARTICLEATT* attribute);

	virtual _int Update(_double fTimeDelta)override;
	virtual _int LateUpdate(_double fTimeDelta)override;
	// 랜더는 부모 것 사용


public:

	static CInstanceEffect_Cone* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject * Clone(void * pArg) override;
};
END

#pragma endregion
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////퍼지는 형태 파티클///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region SpreadParticle
BEGIN(Client)

class CInstanceEffect_Spread final : public CInstanceEffect
{
private:
	explicit CInstanceEffect_Spread(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CInstanceEffect_Spread(const CInstanceEffect_Spread& rhs);
	virtual ~CInstanceEffect_Spread() = default;

private:

	virtual void Reset_Velocity(_float3& fAttVlocity)override;
	virtual void Update_Position_by_Velocity(INSTANCEATT* tParticleAtt, _double fTimeDelta)override;


	virtual HRESULT Initialize_Child_Clone() override;
	//	virtual void ResetParticle(PARTICLEATT* attribute);

	virtual _int Update(_double fTimeDelta)override;
	virtual _int LateUpdate(_double fTimeDelta)override;
	// 랜더는 부모 것 사용


public:

	static CInstanceEffect_Spread* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject * Clone(void * pArg) override;
};
END

#pragma endregion
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////분수 형태 파티클///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region FountainParticle
BEGIN(Client)
class CInstanceEffect_Fountain final : public CInstanceEffect
{
private:
	explicit CInstanceEffect_Fountain(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CInstanceEffect_Fountain(const CInstanceEffect_Fountain& rhs);
	virtual ~CInstanceEffect_Fountain() = default;

private:

	virtual void Reset_Velocity(_float3& fAttVlocity)override;
	virtual void Update_Position_by_Velocity(INSTANCEATT* tParticleAtt, _double fTimeDelta)override;


	virtual HRESULT Initialize_Child_Clone() override;
	//	virtual void ResetParticle(PARTICLEATT* attribute);

	virtual _int Update(_double fTimeDelta)override;
	virtual _int LateUpdate(_double fTimeDelta)override;
	// 랜더는 부모 것 사용


public:

	static CInstanceEffect_Fountain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject * Clone(void * pArg) override;
};
END

#pragma endregion
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////빨아들이는 형태 파티클///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region SuckParticle

BEGIN(Client)


class CInstanceEffect_Suck final : public CInstanceEffect
{
private:
	explicit CInstanceEffect_Suck(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CInstanceEffect_Suck(const CInstanceEffect_Suck& rhs);
	virtual ~CInstanceEffect_Suck() = default;

private:
	virtual void Reset_Velocity(_float3& fAttVlocity)override;
	virtual void Update_Position_by_Velocity(INSTANCEATT* tParticleAtt, _double fTimeDelta)override;


	virtual HRESULT Initialize_Child_Clone() override;
	//	virtual void ResetParticle(PARTICLEATT* attribute);

	virtual _int Update(_double fTimeDelta)override;
	virtual _int LateUpdate(_double fTimeDelta)override;
	// 랜더는 부모 것 사용

public:
	static CInstanceEffect_Suck* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject * Clone(void * pArg) override;
};
END

#pragma endregion