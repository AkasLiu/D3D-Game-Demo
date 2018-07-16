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
	LPDIRECT3DDEVICE9				_device;			//D3D设备
	LPDIRECT3DTEXTURE9				_tex;				//纹理
	LPDIRECT3DINDEXBUFFER9			_ib;			//顶点缓存
	LPDIRECT3DVERTEXBUFFER9			_vb;		//索引缓存

	int _numVertsPerRow;		// 每行的顶点数
	int _numVertsPerCol;		// 每行的顶点数
	int _cellSpacing;			// 单元格的间距
	int _numCellsPerRow;		// 每行的单元格数
	int _numCellsPerCol;		// 每列的单元格数
	int _width;					// 地形的宽度
	int _depth;					// 地形的深度
	int _numVertices;			// 顶点总数
	int _numTriangles;			//三角形总数

	float _heightScale;			// 高度缩放系数

	std::vector<int> _heightmap;		// 用于存放高度信息

										//定义一个地形的FVF顶点格式
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
