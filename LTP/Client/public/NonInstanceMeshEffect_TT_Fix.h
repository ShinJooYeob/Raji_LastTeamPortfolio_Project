#pragma once

#include "GameObject.h"
#include "PartilceCreateMgr.h"

BEGIN(Engine)
class CTransform;
END
BEGIN(Client)

// Tex Mesh Effect 참조해서 재생
// 사운드 관련 처리

class CNonInstanceMeshEffect_TT_Fix final : public CGameObject
{

private:
	CNonInstanceMeshEffect_TT_Fix(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CNonInstanceMeshEffect_TT_Fix(const CNonInstanceMeshEffect_TT_Fix& rhs);
	virtual ~CNonInstanceMeshEffect_TT_Fix() = default;

public:
	virtual HRESULT		Initialize_Prototype(void* pArg) override;
	virtual HRESULT		Initialize_Clone(void* pArg) override;

public:
	virtual _int		Update(_double fDeltaTime) override;
	virtual _int		LateUpdate(_double fDeltaTimer) override;
	virtual _int		Render() override;
	virtual _int		LateRender() override;

public:
	CTransform*			Get_Transform();
	void				Set_Position(_float3 pos);
	void				Set_Matrix(_fMatrix pos);
	_float3				Get_Postition();

	

	void				Set_AddDesc(const MESHADDDATA& desc)
	{
		mAddDesc = desc;
		if (mAddDesc.DealyTime <= 0)
		{
			mIsDealy = false;
		}
		else
		{
			mIsDealy = true;
			mDealyTime = mAddDesc.DealyTime;
		}

	}

	void				Set_LightDesc(const MESHLIGHTDATA& desc)
	{
		mLightDesc = desc;
		if (mLightDesc.bEmsiive)
		{
			mEmissive_Origin = mMeshDesc.vEmissive;
		}
		if (mLightDesc.bRim)
		{
			mRim_Origin = mMeshDesc.vLimLight;

		}
	}

	void				Set_DeadMeshparticle();
	void				Set_Particle(INSTPARTICLEDESC particledesc)
	{
		m_DescParticle = particledesc;
		m_bIsParticle = true;
	}

private:
	HRESULT				SetUp_Components();

	bool				IsLife(_double);
	bool				Update_INIT();

	bool				Update_Move(_double Timer);
	bool				Update_Rot(_double Timer);
	bool				Update_Scale(_double Timer);
	bool				Update_Light(_double Timer);


private:
	NONINSTNESHEFTDESC		mMeshDesc;
	MESHADDDATA				mAddDesc;
	MESHLIGHTDATA			mLightDesc;


	_float					m_CurTime_Duration = 0.f;
	_float3					m_vRotAxis = _float3(0, 1, 0);
	_float3					m_vLookAxis = _float3(0, 0, 1);
	_float3					m_vMoveDir = _float3(0, 0, 1);
	_bool					mIsInit = false;

	_float3					mFIXPos = _float3(0, 0, 0);


	_double					mDealyTime = 0;
	_bool					mIsDealy = false;

	_float4					mEmissive_Origin = _float3(0);
	_float4					mRim_Origin = _float3(0);
	_double					mTimer_Rim = 0;
	_double					mTimer_Em = 0;


private:
	CShader*				m_pShaderCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CModel*					m_pModel = nullptr;
	CTransform*				m_pTransformCom = nullptr;

	// Particle
	INSTPARTICLEDESC		m_DescParticle;
	bool					m_bIsParticle = false;
	bool					m_bIsParticle_Create = false;

public:
	static CNonInstanceMeshEffect_TT_Fix*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject*							Clone(void* pArg);
	virtual void									Free() override;
};

END
