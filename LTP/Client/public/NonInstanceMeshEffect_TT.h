#pragma once

#include "GameObject.h"
#include "PartilceCreateMgr.h"

BEGIN(Engine)
class CTransform;
END
BEGIN(Client)

class CNonInstanceMeshEffect_TT final : public CGameObject
{
	enum E_MOVETYPE
	{
		MOVETYPE_NONE,
		MOVETYPE_EASE,
		MOVETYPE_FIX,
		MOVETYPE_FOLLOW,
		MOVETYPE_END,

	};

private:
	CNonInstanceMeshEffect_TT(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CNonInstanceMeshEffect_TT(const CNonInstanceMeshEffect_TT& rhs);
	virtual ~CNonInstanceMeshEffect_TT() = default;

public:
	virtual HRESULT		Initialize_Prototype(void* pArg) override;
	virtual HRESULT		Initialize_Clone(void* pArg) override;

public:
	virtual _int		Update(_double fDeltaTime) override;
	virtual _int		LateUpdate(_double fDeltaTimer) override;
	virtual _int		Render() override;
	virtual _int		LateRender() override;

	void				Set_ParentTransform(class CTransform* parentTrans);
	void				Set_AddDesc(const MESHADDDATA& desc)
	{
		mAddDesc = desc;
	}

	HRESULT				Set_EasingMoveDesc(const MESHAEASING* desc, _uint count);

private:
	HRESULT				SetUp_Components();

private:
	NONINSTNESHEFTDESC		mMeshDesc;
	_float					m_fCurTime_Duration = 0.f;
	_float3					m_vRotAxis = _float3(0, 1, 0);
	_float3					m_vLookAxis = _float3(0, 0, 1);
	_float3					m_vMoveDir = _float3(0, 0, 1);
	_bool					mIsInit = false;
	_bool					mReScale = false;

	_float3					mFIXPos = _float3(0, 0, 0);

	MESHADDDATA				mAddDesc;

	E_MOVETYPE				me_MoveType = MOVETYPE_NONE;
	MESHAEASING*			mEasingDesc = nullptr;
	MESHAEASING				mCurrentEasingDesc;
	_float3					mStartPoint = _float3::Zero();
	_float3					mEndPoint = _float3::Zero();
	_uint					mEasingCount=0;
	_uint					mEasingCountMAX=0;
	_float					mEasingTimer = 0;
	_bool					mbEasingStart = false;
	_bool					mbEasingEnd = false;


private:
	CShader*				m_pShaderCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CModel*					m_pModel = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CTransform*				m_pParentTranscom = nullptr;


public:
	static CNonInstanceMeshEffect_TT*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject*							Clone(void* pArg);
	virtual void									Free() override;
};

END
