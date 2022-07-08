#pragma once

#include "GameObject.h"


BEGIN(Client)
typedef struct tagInstanceMeshAttribute
{
	_float4x4		_LocalMatirx;
	_float3			_size = _float3(1, 1, 1);

	_float			_lifeTime;
	_float			_age;

	_float3			_velocity;
	_float			_force;
	_float			_Targetforce;

	_float4			_color = _float4(0);

	_float3			_randRotAxis = _float3(0, 1, 0);
	_float			_randRotSpeed = XMConvertToRadians(720.f);

	_bool			_isAlive;
	_float3			_TargetParentPosition = _float3(0);
}INSTMESHATT;


class CMeshEffect abstract : public CGameObject
{
protected:
	CMeshEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CMeshEffect(const CMeshEffect& rhs);
	virtual ~CMeshEffect() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize_Clone(void* pArg);

	virtual HRESULT Initialize_Child_Clone()PURE;

	virtual _int Update(_double TimeDelta)override;
	virtual _int LateUpdate(_double TimeDelta)override;
	virtual _int Render() override;
	virtual _int LateRender() override;



	virtual void ResetParticle(INSTMESHATT* attribute);
	virtual void Reset_Velocity(_float3& fAttVlocity)PURE;

	virtual void Update_ParticleAttribute(_double fDeltaTime);
	virtual void Update_SizeChange(INSTMESHATT* tParticleAtt, _double fTimeDelta, CGameInstance* pInstance);
	virtual void Update_ColorChange(INSTMESHATT* tParticleAtt, _double fTimeDelta, CGameInstance* pInstance);
	virtual void Update_Position_by_Velocity(INSTMESHATT* tParticleAtt, _double fTimeDelta)PURE;



protected:
	CShader*					m_pShaderCom = nullptr;
	CRenderer*					m_pRendererCom = nullptr;

	CModel*						m_pModel = nullptr;
	CModelInstance*				m_pModelInstance = nullptr;
	vector<_float4x4>			m_vecWorld;
	vector<_float4>				m_vecLimLight;
	vector<_float4>				m_vecEmisive;

protected:
	INSTMESHDESC				m_tInstanceDesc;
	vector<INSTMESHATT>			m_vecParticleAttribute;

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


class CMeshEffect_Ball final : public CMeshEffect
{
private:
	explicit CMeshEffect_Ball(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CMeshEffect_Ball(const CMeshEffect_Ball& rhs);
	virtual ~CMeshEffect_Ball() = default;

private:

	virtual void Reset_Velocity(_float3& fAttVlocity)override;
	virtual void Update_Position_by_Velocity(INSTMESHATT* tParticleAtt, _double fTimeDelta)override;


	virtual HRESULT Initialize_Child_Clone() override;
	//	virtual void ResetParticle(PARTICLEATT* attribute);

	virtual _int Update(_double fTimeDelta)override;
	virtual _int LateUpdate(_double fTimeDelta)override;
	// 랜더는 부모 것 사용


public:

	static CMeshEffect_Ball* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject * Clone(void * pArg) override;
};
END

#pragma endregion


///////////직선 형태 파티클///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region StraightParticle

BEGIN(Client)


class CMeshEffect_Straight final : public CMeshEffect
{
private:
	explicit CMeshEffect_Straight(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CMeshEffect_Straight(const CMeshEffect_Straight& rhs);
	virtual ~CMeshEffect_Straight() = default;

private:

	virtual void Reset_Velocity(_float3& fAttVlocity)override;
	virtual void Update_Position_by_Velocity(INSTMESHATT* tParticleAtt, _double fTimeDelta)override;


	virtual HRESULT Initialize_Child_Clone() override;
	//	virtual void ResetParticle(PARTICLEATT* attribute);

	virtual _int Update(_double fTimeDelta)override;
	virtual _int LateUpdate(_double fTimeDelta)override;
	// 랜더는 부모 것 사용


public:
	static CMeshEffect_Straight* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject * Clone(void * pArg) override;

};
END

#pragma endregion




///////////원뿔 형태 파티클///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region ConeParticle

BEGIN(Client)


class CMeshEffect_Cone final : public CMeshEffect
{
private:
	explicit CMeshEffect_Cone(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CMeshEffect_Cone(const CMeshEffect_Cone& rhs);
	virtual ~CMeshEffect_Cone() = default;

private:

	virtual void Reset_Velocity(_float3& fAttVlocity)override;
	virtual void Update_Position_by_Velocity(INSTMESHATT* tParticleAtt, _double fTimeDelta)override;


	virtual HRESULT Initialize_Child_Clone() override;
	//	virtual void ResetParticle(PARTICLEATT* attribute);

	virtual _int Update(_double fTimeDelta)override;
	virtual _int LateUpdate(_double fTimeDelta)override;
	// 랜더는 부모 것 사용


public:

	static CMeshEffect_Cone* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject * Clone(void * pArg) override;
};
END

#pragma endregion
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////퍼지는 형태 파티클///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region SpreadParticle
BEGIN(Client)

class CMeshEffect_Spread final : public CMeshEffect
{
private:
	explicit CMeshEffect_Spread(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CMeshEffect_Spread(const CMeshEffect_Spread& rhs);
	virtual ~CMeshEffect_Spread() = default;

private:

	virtual void Reset_Velocity(_float3& fAttVlocity)override;
	virtual void Update_Position_by_Velocity(INSTMESHATT* tParticleAtt, _double fTimeDelta)override;


	virtual HRESULT Initialize_Child_Clone() override;
	//	virtual void ResetParticle(PARTICLEATT* attribute);

	virtual _int Update(_double fTimeDelta)override;
	virtual _int LateUpdate(_double fTimeDelta)override;
	// 랜더는 부모 것 사용


public:

	static CMeshEffect_Spread* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject * Clone(void * pArg) override;
};
END

#pragma endregion
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////분수 형태 파티클///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region FountainParticle
BEGIN(Client)
class CMeshEffect_Fountain final : public CMeshEffect
{
private:
	explicit CMeshEffect_Fountain(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CMeshEffect_Fountain(const CMeshEffect_Fountain& rhs);
	virtual ~CMeshEffect_Fountain() = default;

private:

	virtual void Reset_Velocity(_float3& fAttVlocity)override;
	virtual void Update_Position_by_Velocity(INSTMESHATT* tParticleAtt, _double fTimeDelta)override;


	virtual HRESULT Initialize_Child_Clone() override;
	//	virtual void ResetParticle(PARTICLEATT* attribute);

	virtual _int Update(_double fTimeDelta)override;
	virtual _int LateUpdate(_double fTimeDelta)override;
	// 랜더는 부모 것 사용


public:

	static CMeshEffect_Fountain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject * Clone(void * pArg) override;
};
END

#pragma endregion
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////빨아들이는 형태 파티클///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region SuckParticle

BEGIN(Client)


class CMeshEffect_Suck final : public CMeshEffect
{
private:
	explicit CMeshEffect_Suck(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CMeshEffect_Suck(const CMeshEffect_Suck& rhs);
	virtual ~CMeshEffect_Suck() = default;

private:
	virtual void Reset_Velocity(_float3& fAttVlocity)override;
	virtual void Update_Position_by_Velocity(INSTMESHATT* tParticleAtt, _double fTimeDelta)override;


	virtual HRESULT Initialize_Child_Clone() override;
	//	virtual void ResetParticle(PARTICLEATT* attribute);

	virtual _int Update(_double fTimeDelta)override;
	virtual _int LateUpdate(_double fTimeDelta)override;
	// 랜더는 부모 것 사용

public:
	static CMeshEffect_Suck* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject * Clone(void * pArg) override;
};
END

#pragma endregion