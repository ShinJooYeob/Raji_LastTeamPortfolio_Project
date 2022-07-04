#include "MotionTrail.h"
#include "MotionTrailBuffer.h"

CMotionTrail::CMotionTrail(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CComponent(pDevice, pDeviceContext)
{
}

CMotionTrail::CMotionTrail(const CMotionTrail & rhs)
	: CComponent(rhs)
{


}

HRESULT CMotionTrail::Initialize_Prototype()
{



	return S_OK;
}

HRESULT CMotionTrail::Initialize_Clone(void * pArg)
{

	memcpy(&m_tTrailDesc, pArg, sizeof(MOTIONTRAILDESC));

	m_tTrailDesc.iNumTrailCount = max(m_tTrailDesc.iNumTrailCount, 1);
	NULL_CHECK_RETURN(m_tTrailDesc.pModel, E_FAIL);
	NULL_CHECK_RETURN(m_tTrailDesc.pShader, E_FAIL);
	
	FAILED_CHECK(Ready_TrailBuffer());

	return S_OK;
}



void CMotionTrail::Add_MotionBuffer(_float4x4 & WorldMatrix, _double TargetTime)
{
	auto& iter = m_TrailBufferList.begin();

	for (_uint i = 0; i<  m_TrailBufferList.size();i++)
	{
		if(!(*iter)->IsTrailing()) break;
		iter++;
	}

	if (iter == m_TrailBufferList.end())
	{
		(*m_TrailBufferList.begin())->Start_MotionTrail(WorldMatrix, TargetTime);

	}
	else
	{
		(*iter)->Start_MotionTrail(WorldMatrix, TargetTime);
	}

	m_TrailBufferList.sort([](CMotionTrailBuffer* pSour, CMotionTrailBuffer* pDest)->_bool
	{
		return pSour->Get_PassedTime() > pDest->Get_PassedTime();
	});

}

void CMotionTrail::Update_MotionTrail(_double fDeltaTime)
{
	for (auto& pBuffer : m_TrailBufferList)
	{
		if (pBuffer->IsTrailing())
			pBuffer->Update_MotionBuffer(fDeltaTime);
		
	}

}

HRESULT CMotionTrail::Render()
{
	for (auto& pBuffer : m_TrailBufferList)
	{
		if (pBuffer->IsTrailing())
			pBuffer->Render(m_tTrailDesc.pShader,m_tTrailDesc.iPassIndex, m_tTrailDesc.vLimLight, "g_BoneMatrices");

	}

	return 0;
}



HRESULT CMotionTrail::Ready_TrailBuffer()
{

	for (_uint i = 0; i< m_tTrailDesc.iNumTrailCount;i++)
	{
		CMotionTrailBuffer* pBuffer = CMotionTrailBuffer::Create(m_tTrailDesc.pModel);

		NULL_CHECK_RETURN(pBuffer, E_FAIL);

		m_TrailBufferList.push_back(pBuffer);

	}



	return S_OK;
}

CMotionTrail * CMotionTrail::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CMotionTrail*	pInstance = new CMotionTrail(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSGBOX("Failed to Creating CMotionTrail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CMotionTrail::Clone(void * pArg)
{
	CMotionTrail*	pInstance = new CMotionTrail(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Creating CMotionTrail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMotionTrail::Free()
{
	__super::Free();


	for (auto& pTrailBuffer : m_TrailBufferList)
		Safe_Release(pTrailBuffer);

	m_TrailBufferList.clear();

}
