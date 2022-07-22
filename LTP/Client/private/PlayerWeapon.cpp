#include "stdafx.h"
#include "..\public\PlayerWeapon.h"


CPlayerWeapon::CPlayerWeapon(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CPlayerWeapon::CPlayerWeapon(const CPlayerWeapon & rhs)
	: CGameObject(rhs)
{
}

HRESULT CPlayerWeapon::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CPlayerWeapon::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	ZeroMemory(&m_tPlayerWeaponDesc, sizeof(PlayerWeaponDesc));
	if (nullptr != pArg)
	{
		memcpy(&m_tPlayerWeaponDesc, pArg, sizeof(PlayerWeaponDesc));
	}
	return S_OK;
}

_int CPlayerWeapon::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0) return -1;

	return _int();
}

_int CPlayerWeapon::LateUpdate(_double fDeltaTimer)
{
	if (__super::LateUpdate(fDeltaTimer) < 0)return -1;

	return _int();
}

_int CPlayerWeapon::Render()
{
	if (__super::Render() < 0)		return -1;

	return _int();
}

_int CPlayerWeapon::LateRender()
{
	return _int();
}

void CPlayerWeapon::Dissolve_In(_double fTargetTime)
{
	
}

void CPlayerWeapon::Dissolve_Out(_double fTargetTime)
{
	
}

void CPlayerWeapon::Active_Collision()
{
	m_bActiveCollision = true;
}

void CPlayerWeapon::DeActive_Collision()
{
	m_bActiveCollision = false;
}

void CPlayerWeapon::Active_Collision_1()
{
	m_bActiveCollision_1 = true;
}

void CPlayerWeapon::DeActive_Collision_1()
{
	m_bActiveCollision_1 = false;
}

void CPlayerWeapon::Active_Collision_2()
{
	m_bActiveCollision_2 = true;
}

void CPlayerWeapon::DeActive_Collision_2()
{
	m_bActiveCollision_2 = false;
}

void CPlayerWeapon::Active_Collision_3()
{
	m_bActiveCollision_3 = true;
}

void CPlayerWeapon::DeActive_Collision_3()
{
	m_bActiveCollision_3 = false;
}

void CPlayerWeapon::Active_Collision_4()
{
	m_bActiveCollision_4 = true;
}

void CPlayerWeapon::DeActive_Collision_4()
{
	m_bActiveCollision_4 = false;
}

void CPlayerWeapon::Active_Trail(_bool bActivate)
{
	m_bActiveTrail = bActivate;
}

void CPlayerWeapon::Set_BlockUpdate(_bool bBlock)
{
	m_bBlockUpdate = bBlock;
}

HRESULT CPlayerWeapon::Ready_ParticleDesc()
{
	m_vecMeshParticleDesc.clear();

	return S_OK;
}

HRESULT CPlayerWeapon::Set_Play_Particle(_uint ParticleIndex, _fVector FixVec,_float3 offset, _float Timer)
{
	if (PARTILCECOUNT <= ParticleIndex)
	{
		DEBUGBREAK;
		return E_FAIL;
	}

	if (m_vecTextureParticleDesc.size() <= ParticleIndex)
	{
		DEBUGBREAK;
		return E_FAIL;
	}
	bool isCreate = false;

	if (m_fPlayParticleTimer[ParticleIndex] <= 0.0f)
	{
		isCreate = true;

		if (Timer == -1)
		{
			m_fPlayParticleTimer[ParticleIndex] = m_vecTextureParticleDesc[ParticleIndex].TotalParticleTime;
		}
		else
			m_fPlayParticleTimer[ParticleIndex] = Timer;
	}

	// 위치
	if (isCreate)
	{
		_Vector CreatePos;

		if (m_vecTextureParticleDesc[ParticleIndex].FollowingTarget)
		{
			if (offset.Get_Lenth() != 0)
			{
				_Matrix  TargetMat = m_vecTextureParticleDesc[ParticleIndex].FollowingTarget->Get_WorldMatrix();

				TargetMat.r[0] = XMVector3Normalize(TargetMat.r[0]);
				TargetMat.r[1] = XMVector3Normalize(TargetMat.r[1]);
				TargetMat.r[2] = XMVector3Normalize(TargetMat.r[2]);

				_Vector WorldPos = TargetMat.r[3];
				CreatePos = WorldPos + TargetMat.r[0] * offset.x + TargetMat.r[1] * offset.y + TargetMat.r[2] * offset.z;
				CTransform* cashtrans = m_vecTextureParticleDesc[ParticleIndex].FollowingTarget;
				m_vecTextureParticleDesc[ParticleIndex].FollowingTarget = nullptr;
				m_vecTextureParticleDesc[ParticleIndex].vFixedPosition = CreatePos;
				GETPARTICLE->Create_Texture_Effect_Desc(m_vecTextureParticleDesc[ParticleIndex], m_eNowSceneNum);
				m_vecTextureParticleDesc[ParticleIndex].FollowingTarget = cashtrans;

			}
			else
				GETPARTICLE->Create_Texture_Effect_Desc(m_vecTextureParticleDesc[ParticleIndex], m_eNowSceneNum);

		}
		else
		{
			// if (defaultTrans == nullptr)
			// 	return S_OK;
			// 
			// _Matrix WeaponMat = defaultTrans->Get_WorldMatrix()  * m_tPlayerWeaponDesc.eAttachedDesc.Caculate_AttachedBoneMatrix();
			// 
			// WeaponMat.r[0] = XMVector3Normalize(WeaponMat.r[0]);
			// WeaponMat.r[1] = XMVector3Normalize(WeaponMat.r[1]);
			// WeaponMat.r[2] = XMVector3Normalize(WeaponMat.r[2]);
			// 
			// _Vector WorldPos = WeaponMat.r[3];
			// CreatePos = WorldPos + WeaponMat.r[0] * offset.z + WeaponMat.r[1] * offset.z + WeaponMat.r[2] * offset.z;


			m_vecTextureParticleDesc[ParticleIndex].vFixedPosition = FixVec;
			GETPARTICLE->Create_Texture_Effect_Desc(m_vecTextureParticleDesc[ParticleIndex], m_eNowSceneNum);

		}
	}

	return S_OK;
}

HRESULT CPlayerWeapon::Set_Play_Particle_Must(_uint ParticleIndex, _float3 FixPos, _float3 offset, _float Timer)
{
	if (PARTILCECOUNT <= ParticleIndex)
		return E_FAIL;

	if (m_vecTextureParticleDesc.size() <= ParticleIndex)
		return E_FAIL;

	bool isCreate = true;
	m_fPlayParticleTimer[ParticleIndex] = m_vecTextureParticleDesc[ParticleIndex].TotalParticleTime;


	// 위치
	if (isCreate)
	{
		_Vector CreatePos;

		if (m_vecTextureParticleDesc[ParticleIndex].FollowingTarget)
		{

			if (m_vecTextureParticleDesc[ParticleIndex].FollowingTarget->Get_IsOwnerDead() == true)
				(m_vecTextureParticleDesc[ParticleIndex].FollowingTarget->Set_IsOwnerDead(false));

			GETPARTICLE->Create_Texture_Effect_Desc(m_vecTextureParticleDesc[ParticleIndex], m_eNowSceneNum);

		

		}
		else
		{

			CreatePos = FixPos.XMVector() + offset.XMVector();

			m_vecTextureParticleDesc[ParticleIndex].vFixedPosition = CreatePos;
			GETPARTICLE->Create_Texture_Effect_Desc(m_vecTextureParticleDesc[ParticleIndex], m_eNowSceneNum);

		}
	}


	return S_OK;
}


HRESULT CPlayerWeapon::Set_Dead_Transform(_uint ParticleIndex)
{
	
	if (m_vecTextureParticleDesc[ParticleIndex].FollowingTarget)
	{
		m_vecTextureParticleDesc[ParticleIndex].FollowingTarget->Set_IsOwnerDead(true);

	}
	 
	return S_OK;
}

HRESULT CPlayerWeapon::Update_Particle(_double timer)
{
	for (auto& t : m_fPlayParticleTimer)
	{
		t -= (_float)timer;
		if (t <= -100)
			t = -1;
	}

	return S_OK;
}

HRESULT CPlayerWeapon::Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ type, CTransform* trans,  bool * pb)
{
	NULL_CHECK_BREAK(trans);

	if (trans)
	{
		if (trans->Get_IsOwnerDead() == true)
		{
			trans->Set_IsOwnerDead(false);
		}

		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc_Hard(type, trans);
		if (pb)
			*pb = true;
	}

	return S_OK;
}

INSTPARTICLEDESC & CPlayerWeapon::Get_VecParticle(_uint index)
{
	if (m_vecTextureParticleDesc.size() < index)
	{
		
		return INSTPARTICLEDESC();
	}

	return m_vecTextureParticleDesc[index];
}
void CPlayerWeapon::Set_VecParticle(_uint index, INSTPARTICLEDESC & d)
{
	if (m_vecTextureParticleDesc.size() < index)
		return;

	m_vecTextureParticleDesc[index] = d;


}


_bool CPlayerWeapon::AbleToChangeWeapon()
{
	return _bool();
}

_fVector CPlayerWeapon::Get_BonePos(const char * pBoneName)
{
	return _fVector();
}

void CPlayerWeapon::Update_AttachCamPos()
{
}

void CPlayerWeapon::Free()
{
	__super::Free();

}

