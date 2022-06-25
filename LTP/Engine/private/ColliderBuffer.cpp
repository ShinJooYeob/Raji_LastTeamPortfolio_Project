#include "..\public\ColliderBuffer.h"
#include "DebugDraw.h"
#include "PipeLineMgr.h"





CColliderBuffer::CColliderBuffer(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:m_pDevice(pDevice),
	m_pDeviceContext(pDeviceContext),
	m_bIsClone(false)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
}

CColliderBuffer::CColliderBuffer(const CColliderBuffer & rhs)
	:m_pDevice(rhs.m_pDevice),
#ifdef _DEBUG
	m_pBasicEffect(rhs.m_pBasicEffect),
	m_pBatch(rhs.m_pBatch),
	m_pInputLayout(rhs.m_pInputLayout),
#endif // _DEBUG
	m_pDeviceContext(rhs.m_pDeviceContext),
	m_eColliderType(rhs.m_eColliderType),
	m_bIsClone(true),
	m_bIsConflicted(false)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
#ifdef _DEBUG
	Safe_AddRef(m_pInputLayout);
#endif // _DEBUG

	for (_uint i = 0; i < BOUNDING_END; ++i)
	{
		switch (m_eColliderType)
		{
		case Engine::COLLIDER_AABB:
			if (nullptr != rhs.m_pAABB[BOUNDING_ORIGINAL])
				m_pAABB[i] = new BoundingBox(*rhs.m_pAABB[BOUNDING_ORIGINAL]);
			break;
		case Engine::COLLIDER_OBB:
			if (nullptr != rhs.m_pOBB[BOUNDING_ORIGINAL])
				m_pOBB[i] = new BoundingOrientedBox(*rhs.m_pOBB[BOUNDING_ORIGINAL]);
			break;
		case Engine::COLLIDER_SPHERE:
			if (nullptr != rhs.m_pSphere[BOUNDING_ORIGINAL])
				m_pSphere[i] = new BoundingSphere(*rhs.m_pSphere[BOUNDING_ORIGINAL]);
			break;
		default:
			break;
		}

	}

}

HRESULT CColliderBuffer::Initialize_Prototype(COLLIDERTYPE eType)
{
	m_eColliderType = eType;

	switch (m_eColliderType)
	{
	case COLLIDER_AABB:
		m_pAABB[BOUNDING_ORIGINAL] = new BoundingBox(_float3(0.f, 0.f, 0.f), _float3(0.5f, 0.5f, 0.5f));
		break;
	case COLLIDER_OBB:
		m_pOBB[BOUNDING_ORIGINAL] = new BoundingOrientedBox(_float3(0.f, 0.f, 0.f), _float3(0.5f, 0.5f, 0.5f), _float4(0.f, 0.f, 0.f, 1.f));
		break;
	case COLLIDER_SPHERE:
		m_pSphere[BOUNDING_ORIGINAL] = new BoundingSphere(_float3(0.f, 0.f, 0.f), 0.5f);
		break;
	}


#ifdef _DEBUG
	m_pBasicEffect = new BasicEffect(m_pDevice);
	m_pBasicEffect->SetVertexColorEnabled(true);

	const void* pShaderByteCode = nullptr;
	size_t		iShaderByteCodeLength = 0;

	m_pBasicEffect->GetVertexShaderBytecode(&pShaderByteCode, &iShaderByteCodeLength);

	FAILED_CHECK(m_pDevice->CreateInputLayout(DirectX::VertexPositionColor::InputElements, DirectX::VertexPositionColor::InputElementCount,
		pShaderByteCode, iShaderByteCodeLength, &m_pInputLayout));

	NULL_CHECK_RETURN(m_pInputLayout, E_FAIL);

	m_pBatch = new PrimitiveBatch<DirectX::VertexPositionColor>(m_pDeviceContext);
	NULL_CHECK_RETURN(m_pBatch, E_FAIL);

#endif // _DEBUG



	return S_OK;
}

HRESULT CColliderBuffer::Initialize_Clone(COLLIDERDESC * pArg)
{
	_Matrix		TransformMatrix;

	if (nullptr != pArg)
	{
		memcpy(&m_ColliderDesc, pArg, sizeof(COLLIDERDESC));
		TransformMatrix = XMMatrixAffineTransformation(XMLoadFloat3(&m_ColliderDesc.vScale), XMVectorSet(0.f, 0.f, 0.f, 1.f), 
			XMLoadFloat4(&m_ColliderDesc.vRotation), XMLoadFloat4(&m_ColliderDesc.vPosition));
	}
	else
		TransformMatrix = XMMatrixIdentity();

	switch (m_eColliderType)
	{
	case Engine::COLLIDER_AABB:
		m_pAABB[BOUNDING_ORIGINAL]->Transform(*m_pAABB[BOUNDING_ORIGINAL], TransformMatrix);
		break;
	case Engine::COLLIDER_OBB:
	{
		//_float3 vExtents = _float3(XMLoadFloat3(&m_pOBB[BOUNDING_ORIGINAL]->Extents) * m_ColliderDesc.vScale.XMVector());
		//m_pOBB[BOUNDING_ORIGINAL]->Transform(*m_pOBB[BOUNDING_ORIGINAL], TransformMatrix);
		//m_pOBB[BOUNDING_ORIGINAL]->Extents = vExtents;

		m_pOBB[BOUNDING_ORIGINAL]->Transform(*m_pOBB[BOUNDING_ORIGINAL], TransformMatrix);
	}
		break;
	case Engine::COLLIDER_SPHERE:
		m_pSphere[BOUNDING_ORIGINAL]->Transform(*m_pSphere[BOUNDING_ORIGINAL], TransformMatrix);
		break;
	default:
		break;
	}

	return S_OK;
}

void CColliderBuffer::Update_Transform(_fMatrix& TransformMatrix)
{
	switch (m_eColliderType)
	{
	case Engine::COLLIDER_AABB:
		m_pAABB[BOUNDING_ORIGINAL]->Transform(*m_pAABB[BOUNDING_TRANSFORM], Remove_Rotation(TransformMatrix));
		break;
	case Engine::COLLIDER_OBB:
		m_pOBB[BOUNDING_ORIGINAL]->Transform(*m_pOBB[BOUNDING_TRANSFORM], TransformMatrix);
		break;
	case Engine::COLLIDER_SPHERE:
		m_pSphere[BOUNDING_ORIGINAL]->Transform(*m_pSphere[BOUNDING_TRANSFORM], TransformMatrix);
		break;
	default:
		break;
	}
	m_bIsConflicted = false;
}

_bool CColliderBuffer::Collision_AABB(CColliderBuffer * pTargetColliderBuffer, _bool IsFinialBuffer)
{
	if (m_pAABB[BOUNDING_TRANSFORM]  == nullptr)
	{
		__debugbreak();
		return false;
	}
	_bool bBool = false;
	switch (pTargetColliderBuffer->m_eColliderType)
	{
	case Engine::COLLIDER_AABB:
		bBool = m_pAABB[BOUNDING_TRANSFORM]->Intersects(*pTargetColliderBuffer->m_pAABB[BOUNDING_TRANSFORM]);
		break;
	case Engine::COLLIDER_OBB:
		bBool = m_pAABB[BOUNDING_TRANSFORM]->Intersects(*pTargetColliderBuffer->m_pOBB[BOUNDING_TRANSFORM]);
		break;
	case Engine::COLLIDER_SPHERE:
		bBool = m_pAABB[BOUNDING_TRANSFORM]->Intersects(*pTargetColliderBuffer->m_pSphere[BOUNDING_TRANSFORM]);
		break;
	default:
		__debugbreak();
		break;
	}

	if (IsFinialBuffer && bBool)m_bIsConflicted = true;

	return bBool;
}

_bool CColliderBuffer::Collision_OBB(CColliderBuffer * pTargetColliderBuffer, _bool IsFinialBuffer)
{
	if (m_pOBB[BOUNDING_TRANSFORM] == nullptr)
	{
		__debugbreak();
		return false;
	}
	_bool bBool = false;


	switch (pTargetColliderBuffer->m_eColliderType)
	{
	case Engine::COLLIDER_AABB:
		bBool = m_pOBB[BOUNDING_TRANSFORM]->Intersects(*pTargetColliderBuffer->m_pAABB[BOUNDING_TRANSFORM]);
		break;
	case Engine::COLLIDER_OBB:
		bBool = m_pOBB[BOUNDING_TRANSFORM]->Intersects(*pTargetColliderBuffer->m_pOBB[BOUNDING_TRANSFORM]);
		break;
	case Engine::COLLIDER_SPHERE:
		bBool = m_pOBB[BOUNDING_TRANSFORM]->Intersects(*pTargetColliderBuffer->m_pSphere[BOUNDING_TRANSFORM]);
		break;
	default:
		__debugbreak();
		break;
	}
	if (IsFinialBuffer && bBool)m_bIsConflicted = true;

	return bBool;
}

_bool CColliderBuffer::Collision_Sphere(CColliderBuffer * pTargetColliderBuffer, _bool IsFinialBuffer)
{
	if (m_pSphere[BOUNDING_TRANSFORM] == nullptr)
	{
		__debugbreak();
		return false;
	}
	_bool bBool = false;

	switch (pTargetColliderBuffer->m_eColliderType)
	{
	case Engine::COLLIDER_AABB:
		bBool = m_pSphere[BOUNDING_TRANSFORM]->Intersects(*pTargetColliderBuffer->m_pAABB[BOUNDING_TRANSFORM]);
		break;
	case Engine::COLLIDER_OBB:
		bBool = m_pSphere[BOUNDING_TRANSFORM]->Intersects(*pTargetColliderBuffer->m_pOBB[BOUNDING_TRANSFORM]);
		break;
	case Engine::COLLIDER_SPHERE:
		bBool = m_pSphere[BOUNDING_TRANSFORM]->Intersects(*pTargetColliderBuffer->m_pSphere[BOUNDING_TRANSFORM]);
		break;
	default:
		__debugbreak();
		break;
	}
	if (IsFinialBuffer && bBool)m_bIsConflicted = true;

	return bBool;
}

_bool CColliderBuffer::Collision_All(CColliderBuffer * pTargetColliderBuffer, _bool IsFinialBuffer)
{

	switch (m_eColliderType)
	{
	case Engine::COLLIDER_AABB:
		 return Collision_AABB(pTargetColliderBuffer, IsFinialBuffer);

	case Engine::COLLIDER_OBB:
		return Collision_OBB(pTargetColliderBuffer, IsFinialBuffer);
		
	case Engine::COLLIDER_SPHERE:
		return Collision_Sphere(pTargetColliderBuffer, IsFinialBuffer);

	default:
		__debugbreak();
		break;
	}

	return false;
}

void CColliderBuffer::Add_ChildBufferIndex(_uint iIndex)
{
	m_ChildNodeIndexList.push_back(iIndex);
}

_float3 CColliderBuffer::Get_ColliderPosition()
{

	switch (m_eColliderType)
	{
	case Engine::COLLIDER_AABB:
		return m_pAABB[BOUNDING_TRANSFORM]->Center;
	case Engine::COLLIDER_OBB:
		return m_pOBB[BOUNDING_TRANSFORM]->Center;
	case Engine::COLLIDER_SPHERE:
		return m_pSphere[BOUNDING_TRANSFORM]->Center;
	default:
		break;
	}



	return _float3();
}

#ifdef _DEBUG

_int CColliderBuffer::Render()
{
	//m_pDeviceContext->GSSetShader(nullptr, nullptr, 0);

	m_pDeviceContext->IASetInputLayout(m_pInputLayout);

	m_pBasicEffect->SetWorld(XMMatrixIdentity());

	CPipeLineMgr*		pPipeLine = GetSingle(CPipeLineMgr);

	m_pBasicEffect->SetView(pPipeLine->Get_Transform_Matrix(PLM_VIEW));
	m_pBasicEffect->SetProjection(pPipeLine->Get_Transform_Matrix(PLM_PROJ));

	m_pBasicEffect->Apply(m_pDeviceContext);


	_Vector		vColliderColor = (m_bIsConflicted) ? XMVectorSet(1.f, 0.f, 0.f, 1.f) : XMVectorSet(0.f, 1.f, 0.f, 1.f);
	if(m_ChildNodeIndexList.size()) vColliderColor =  XMVectorSet(1.f, 1.f, 1.f, 1.f);

	m_pBatch->Begin();

	switch (m_eColliderType)
	{
	case Engine::COLLIDER_AABB:
		DX::Draw(m_pBatch, *m_pAABB[BOUNDING_TRANSFORM], vColliderColor);
		break;
	case Engine::COLLIDER_OBB:
		DX::Draw(m_pBatch, *m_pOBB[BOUNDING_TRANSFORM], vColliderColor);
		break;
	case Engine::COLLIDER_SPHERE:
		DX::Draw(m_pBatch, *m_pSphere[BOUNDING_TRANSFORM], vColliderColor);
		break;
	default:
		break;
	}

	m_pBatch->End();

	return 0;
}
#endif // _DEBUG

_Matrix CColliderBuffer::Remove_Rotation(_fMatrix& TransformMatrix)
{
	_Matrix		Matrix = TransformMatrix;

	Matrix.r[0] = XMVectorSet(1.f, 0.f, 0.f, 0.f) * (XMVector3Length(TransformMatrix.r[0]));
	Matrix.r[1] = XMVectorSet(0.f, 1.f, 0.f, 0.f) * (XMVector3Length(TransformMatrix.r[1]));
	Matrix.r[2] = XMVectorSet(0.f, 0.f, 1.f, 0.f) * (XMVector3Length(TransformMatrix.r[2]));

	return Matrix;
}

CColliderBuffer * CColliderBuffer::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, COLLIDERTYPE eType)
{
	CColliderBuffer*	pInstance = new CColliderBuffer(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(eType)))
	{
		MSGBOX("Failed to Creating CColliderBuffer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CColliderBuffer * CColliderBuffer::Clone(COLLIDERDESC * pArg)
{
	CColliderBuffer*	pInstance = new CColliderBuffer(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Creating CColliderBuffer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CColliderBuffer::Free()
{

	for (_uint i = 0; i < BOUNDING_END; ++i)
	{
		Safe_Delete(m_pAABB[i]);
		Safe_Delete(m_pOBB[i]);
		Safe_Delete(m_pSphere[i]);
	}

#ifdef _DEBUG
	Safe_Release(m_pInputLayout);
	if (!m_bIsClone)
	{
		Safe_Delete(m_pBasicEffect);
		Safe_Delete(m_pBatch);
	}
#endif // _DEBUG
	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);

}
