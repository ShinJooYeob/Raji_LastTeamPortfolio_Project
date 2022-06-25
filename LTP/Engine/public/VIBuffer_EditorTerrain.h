#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_EditorTerrain final : public CVIBuffer
{
public:
	CVIBuffer_EditorTerrain(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CVIBuffer_EditorTerrain(const CVIBuffer_EditorTerrain& rhs);
	virtual ~CVIBuffer_EditorTerrain() = default;

public:
	virtual HRESULT Initialize_Prototype(const _tchar* pHeightMap);
	virtual HRESULT Initialize_Prototype(_uint iNumWidthPixelX, _uint iNumWidthPixelY);
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	_Vector Caculate_TerrainY(_bool* pbIsOnTerrain, _float3 PosOnTerrainLocal,_float3 OldPosOnTerrainLocal ,_float3* vLocalPlaneNormVector = nullptr);
	_Vector Caculate_Terrain_Pick_byRay(_bool* pbIsOnTerrain, _float3 PosOnTerrainLocal, _float3 OldPosOnTerrainLocal, _float3* vLocalPlaneNormVector = nullptr);
	_float EquationPlane(_bool* pbIsOnTerrain, _float3 PosOnTerrainLocal, _float* CaculateY = nullptr, _float3* vNormVector = nullptr);
	_Vector Pick_ByRay(_fVector vRayPos, _fVector vRayDir, _float2 vIndex, _bool* bIsPieck);
	_float Get_NowValueY(_float2 vIndex);

	_float2 Get_NumVerticesXY() { return _float2(_float(m_iNumVerticesX), _float(m_iNumVerticesZ)); };
	_float	Get_MinMapSize() { return m_fMinMapSize; };


private:
	_uint			m_iNumVerticesX = 0;
	_uint			m_iNumVerticesZ = 0;

	_float			m_fMinMapSize = 20;

	VTXNORTEX*		m_pKeepVertices = nullptr;
	
	ID3D11ShaderResourceView* m_pHeightMapSRV = nullptr;
public:
	static CVIBuffer_EditorTerrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _tchar* pHeightMap);
	static CVIBuffer_EditorTerrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint iNumWidthPixelX, _uint iNumWidthPixelY = 0);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END