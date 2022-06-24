#include "..\Public\FrustumMgr.h"
#include "PipeLineMgr.h"

IMPLEMENT_SINGLETON(CFrustumMgr);


CFrustumMgr::CFrustumMgr()
{
	
	vDefaultProjectFrustumCubePos[0] = _float3(-1, 1, 0);
	vDefaultProjectFrustumCubePos[1] = _float3(1, 1, 0);
	vDefaultProjectFrustumCubePos[2] = _float3(1, -1, 0);
	vDefaultProjectFrustumCubePos[3] = _float3(-1, -1, 0);

	vDefaultProjectFrustumCubePos[4] = _float3(-1, 1, 1);
	vDefaultProjectFrustumCubePos[5] = _float3(1, 1, 1);
	vDefaultProjectFrustumCubePos[6] = _float3(1, -1, 1);
	vDefaultProjectFrustumCubePos[7] = _float3(-1, -1, 1);

}

HRESULT CFrustumMgr::Initialize_FrustumMgr(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	if (pDevice == nullptr || pDeviceContext == nullptr)
	{
		__debugbreak();
		return E_FAIL;
	}

	m_pDevice = pDevice;
	m_pDeviceContext = pDeviceContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);

	return S_OK;
}

HRESULT CFrustumMgr::SetUp_WorldFrustumPlane()
{
	if (m_pDevice == nullptr || m_pDeviceContext == nullptr)
	{
		__debugbreak();
		return E_FAIL;
	}

	CPipeLineMgr* pPipeline = GetSingle(CPipeLineMgr);


	_Matrix matVeiwInverse = XMMatrixInverse(nullptr, pPipeline->Get_Transform_Matrix(PLM_VIEW));
	_Matrix matProjectInverse = XMMatrixInverse(nullptr, pPipeline->Get_Transform_Matrix(PLM_PROJ));

//#ifdef _DEBUG
//	_Vector pDeterminate;
//
//	_Matrix matVeiwInverse = XMMatrixInverse(&pDeterminate, pPipeline->Get_Transform_Matrix(PLM_VIEW));
//	if (!XMVectorGetX(pDeterminate)) {
//		__debugbreak();
//		MessageBox(0, TEXT("Failed to Make Inverse Matrix"), TEXT("System Message"), MB_OK);
//	}
//
//	_Matrix matProjectInverse = XMMatrixInverse(&pDeterminate, pPipeline->Get_Transform_Matrix(PLM_PROJ));
//
//	if (!XMVectorGetX(pDeterminate))
//	{
//		__debugbreak();
//		MessageBox(0, TEXT("Failed to Make Inverse Matrix"), TEXT("System Message"), MB_OK);
//	}
//
//
//#else
//
//	_Matrix matVeiwInverse = XMMatrixInverse(nullptr, pPipeline->Get_Transform_Matrix(PLM_VIEW));
//	_Matrix matProjectInverse = XMMatrixInverse(nullptr, pPipeline->Get_Transform_Matrix(PLM_PROJ));
//
//#endif




	////투영 -> 뷰 -> 월드
	_Vector vWorldFrustumCubePos[8] = {};

	for (_uint i = 0; i < 8; i++)
	{
		//기본 투영 111, -1-1-1 큐브를 받아옴
		vWorldFrustumCubePos[i] = vDefaultProjectFrustumCubePos[i].XMVector();
		//투영 역행렬 곱해줌
		vWorldFrustumCubePos[i] = XMVector3TransformCoord(vWorldFrustumCubePos[i], matProjectInverse);
		//뷰 역행렬 곱해줌
		vWorldFrustumCubePos[i] = XMVector3TransformCoord(vWorldFrustumCubePos[i], matVeiwInverse);
	}


	//6평면 구하기
	ZeroMemory(tWorldFrustumPlane, sizeof(_float4) * 6);

	////평면 순서 좌 우 상 하 앞 뒤   => 앞에 있는 애들이 빨리 걸러지는게 좋을거 같아서 생각해보면 이순서대로가 제일 빠르게 걸러지지 않을까

	////좌
	tWorldFrustumPlane[0] = XMPlaneFromPoints(vWorldFrustumCubePos[0], vWorldFrustumCubePos[3], vWorldFrustumCubePos[4]);
	////우
	tWorldFrustumPlane[1] = XMPlaneFromPoints(vWorldFrustumCubePos[2], vWorldFrustumCubePos[1], vWorldFrustumCubePos[6]);

	////상
	tWorldFrustumPlane[2] = XMPlaneFromPoints(vWorldFrustumCubePos[1], vWorldFrustumCubePos[0], vWorldFrustumCubePos[5]);
	////하
	tWorldFrustumPlane[3] = XMPlaneFromPoints(vWorldFrustumCubePos[3], vWorldFrustumCubePos[2], vWorldFrustumCubePos[7]);

	////앞
	tWorldFrustumPlane[4] = XMPlaneFromPoints(vWorldFrustumCubePos[0], vWorldFrustumCubePos[1], vWorldFrustumCubePos[3]);
	////뒤
	tWorldFrustumPlane[5] = XMPlaneFromPoints(vWorldFrustumCubePos[5], vWorldFrustumCubePos[4], vWorldFrustumCubePos[6]);



	return S_OK;
}

_bool CFrustumMgr::IsNeedToRender(_float3 vWorldPosition, _float fLenth)
{

	for (_uint i = 0; i < 6; i++)
	{
		if (tWorldFrustumPlane[i].x * vWorldPosition.x + tWorldFrustumPlane[i].y * vWorldPosition.y +
			tWorldFrustumPlane[i].z * vWorldPosition.z + tWorldFrustumPlane[i].w > fLenth)
			return false;
	}

	return true;
}


void CFrustumMgr::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);
}
