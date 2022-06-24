#include "..\public\Collider.h"
#include "ColliderBuffer.h"

CCollider::CCollider(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CComponent(pDevice, pDeviceContext)
{
}

CCollider::CCollider(const CCollider & rhs)
	: CComponent(rhs),
	m_vecPrototypeCollideBuffer(rhs.m_vecPrototypeCollideBuffer)
{
	for (auto& pColliderProto : m_vecPrototypeCollideBuffer)
		Safe_AddRef(pColliderProto);
	

}

HRESULT CCollider::Initialize_Prototype()
{
	m_vecPrototypeCollideBuffer.reserve(COLLIDERTYPE_END);


	for (_uint i = 0; i < COLLIDERTYPE_END; i++)
	{
		CColliderBuffer* pColliderBuffer = CColliderBuffer::Create(m_pDevice, m_pDeviceContext, COLLIDERTYPE(i));
		NULL_CHECK_RETURN(pColliderBuffer, E_FAIL);
		m_vecPrototypeCollideBuffer.push_back(pColliderBuffer);

	}


	return S_OK;
}

HRESULT CCollider::Initialize_Clone(void * pArg)
{



	return S_OK;
}

void CCollider::Update_ConflictPassedTime(_double fDeltaTime)
{
	if (m_bIsConflicted)
	{
		m_ConflictedPassedTime -= fDeltaTime;
		if (m_ConflictedPassedTime < 0)
		{
			m_ConflictedPassedTime = 0;
			m_bIsConflicted = false;
		}
	}
}

void CCollider::Update_Transform(_uint iIndex, _fMatrix& Transform)
{
	if (iIndex >= m_vecColliderBuffer.size())
	{
		__debugbreak();
		return;
	}

	m_vecColliderBuffer[iIndex]->Update_Transform(Transform);

}

_bool CCollider::Inspect_Collision(CCollider* pTargetCollider, _uint iBufferIndex, _uint iTargetIndex, _uint2* pOutIndex)
{
	if (m_bIsConflicted || pTargetCollider->m_bIsConflicted) return false;

	if (iBufferIndex >= m_vecColliderBuffer.size() || iTargetIndex >= pTargetCollider->m_vecColliderBuffer.size())
	{
		__debugbreak();
		return false;
	}

	return Inspect_ChildBuffer(iBufferIndex, pTargetCollider, iTargetIndex, pOutIndex);
}

HRESULT CCollider::Set_ParantBuffer(_uint iParantIndex, _int iIndex)
{
	if (iIndex == -1) iIndex = _int(_int(m_vecColliderBuffer.size()) - 1);

	if (iIndex >= m_vecColliderBuffer.size() || iParantIndex >= m_vecColliderBuffer.size())
	{
		__debugbreak();
		return E_FAIL;
	}

	m_vecColliderBuffer[iParantIndex]->Add_ChildBufferIndex(iIndex);

	return S_OK;
}




_int CCollider::Add_ColliderBuffer(COLLIDERTYPE eColliderType, COLLIDERDESC * pColliderDesc)
{

	CColliderBuffer* pColliderBuffer = m_vecPrototypeCollideBuffer[eColliderType]->Clone(pColliderDesc);
	
	NULL_CHECK_RETURN(pColliderBuffer, -1);

	m_vecColliderBuffer.push_back(pColliderBuffer);

	return _int(m_vecColliderBuffer.size() - 1);
}

void CCollider::Set_Conflicted(_double TotalTime)
{
	if (!m_bIsConflicted)
	{
		m_bIsConflicted = true;
		m_ConflictedPassedTime = TotalTime;
	}
}

_float3 CCollider::Get_ColliderPosition(_uint iIndex)
{
	if (iIndex >= m_vecColliderBuffer.size()) return _float3(0);

	return m_vecColliderBuffer[iIndex]->Get_ColliderPosition();
}

#ifdef _DEBUG

HRESULT CCollider::Render()
{
	for (auto& pColliderBuffer : m_vecColliderBuffer)
	{
		if (pColliderBuffer->Render() < 0)
		{
			__debugbreak();
			return -1;
		}
	}


	return 0;
}
#endif // _DEBUG

_bool CCollider::Inspect_ChildBuffer(_uint iBufferIndex, CCollider* pTargetCollider, _uint iTargetIndex, _uint2* pOutIndex)
{
	_bool IsFinalNode = (m_vecColliderBuffer[iBufferIndex]->Get_NumChildBuffer() + pTargetCollider->m_vecColliderBuffer[iTargetIndex]->Get_NumChildBuffer())?false:true;
	if (m_vecColliderBuffer[iBufferIndex]->Collision_All(pTargetCollider->m_vecColliderBuffer[iTargetIndex], IsFinalNode))
	{
		if (m_vecColliderBuffer[iBufferIndex]->Get_NumChildBuffer())
		{
			list<_uint>* ChildList = m_vecColliderBuffer[iBufferIndex]->Get_ChildIndexList();

			for (auto iChildIndex : *ChildList)
			{
				if (Inspect_ChildBuffer(iChildIndex, pTargetCollider, iTargetIndex, pOutIndex))
					return true;
			}

		}
		else
		{
			if (pTargetCollider->m_vecColliderBuffer[iTargetIndex]->Get_NumChildBuffer())
			{
				list<_uint>* TargetChildList = pTargetCollider->m_vecColliderBuffer[iTargetIndex]->Get_ChildIndexList();


				for (auto iTargetChildIndex : *TargetChildList)
				{
					if (Inspect_ChildBuffer(iBufferIndex, pTargetCollider, iTargetChildIndex, pOutIndex))
						return true;
				}

			}
			else
			{
				if (pOutIndex != nullptr)
				{
					*pOutIndex = _uint2(iBufferIndex, iTargetIndex);
					pTargetCollider->m_vecColliderBuffer[iTargetIndex]->Set_IsConflicted(true);
				}
				return true;
			}

		}

	}
	else
		return false;

	return  false;
}

CCollider * CCollider::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CCollider*	pInstance = new CCollider(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSGBOX("Failed to Creating CCollider");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CCollider::Clone(void * pArg)
{
	CCollider*	pInstance = new CCollider(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Creating CCollider");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCollider::Free()
{
	__super::Free();


	for (auto& pColliderBuffer : m_vecColliderBuffer)
		Safe_Release(pColliderBuffer);
	m_vecColliderBuffer.clear();



	for (auto& pColliderProto : m_vecPrototypeCollideBuffer)
		Safe_Release(pColliderProto);

	m_vecPrototypeCollideBuffer.clear();
}
