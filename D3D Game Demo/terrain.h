#pragma once

#include "d3dUtility.h"
#include <vector>

class Terrain
{
public:
	Terrain::Terrain(IDirect3DDevice9* device,
		wchar_t* heightmapFileName,
		int numVertsPerRow,
		int numVertsPerCol,
		int cellSpacing,
		float heightScale);
	~Terrain();

public:
	int  getHeightmapEntry(int row, int col);
	void setHeightmapEntry(int row, int col, int value);
	float getHeight(float x, float z);
	bool  loadTexture(std::string fileName);
	bool  genTexture(D3DXVECTOR3* directionToLight);
	bool  draw(D3DXMATRIX* world, bool drawTris);


private:
	LPDIRECT3DDEVICE9				_device;			//D3D�豸
	LPDIRECT3DTEXTURE9				_tex;				//����
	LPDIRECT3DINDEXBUFFER9			_ib;			//���㻺��
	LPDIRECT3DVERTEXBUFFER9			_vb;		//��������

	int _numVertsPerRow;		// ÿ�еĶ�����
	int _numVertsPerCol;		// ÿ�еĶ�����
	int _cellSpacing;			// ��Ԫ��ļ��
	int _numCellsPerRow;		// ÿ�еĵ�Ԫ����
	int _numCellsPerCol;		// ÿ�еĵ�Ԫ����
	int _width;					// ���εĿ��
	int _depth;					// ���ε����
	int _numVertices;			// ��������
	int _numTriangles;			//����������

	float _heightScale;			// �߶�����ϵ��

	std::vector<int> _heightmap;		// ���ڴ�Ÿ߶���Ϣ

										//����һ�����ε�FVF�����ʽ
	struct TerrainVertex
	{
		TerrainVertex() {}
		TerrainVertex(float x, float y, float z, float u, float v)
		{
			_x = x; _y = y; _z = z; _u = u; _v = v;
		}
		float _x, _y, _z;
		float _u, _v;

		static const DWORD FVF;
	};

	// helper methods
	bool  readRawFile(wchar_t *pRawFileName);
	bool  computeVertices();
	bool  computeIndices();
	bool  lightTerrain(D3DXVECTOR3* directionToLight);
	float computeShade(int cellRow, int cellCol, D3DXVECTOR3* directionToLight);

};
