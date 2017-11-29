#include "ModelService.h"
#include "ScreenService.h"
#include "Vertex.h"
#include <DXTK\DirectXHelpers.h>
#include "ObjLoader.h"
#include <vector>
using namespace cst;
using namespace std;
using namespace DirectX;
using namespace Microsoft::WRL;
void createSimpleQuadModel(vector<Vertex> &dst) {
	XMFLOAT3 normal(0, 1, 0);
	XMFLOAT4 color(1, 0, 1, 1);
	XMFLOAT3 pins[2*2];
	XMFLOAT2 texs[2*2];
	dst.push_back({ {-1.f,1.f,1.f},{0,1,0},{1,1,0,1},{0,0} });
	dst.push_back({ { 1.f,-1.f,1.f },{ 0,1,0 },{ 1,1,0,1 },{ 1,1 } });
	dst.push_back({ { -1.f,-1.f,1.f },{ 0,1,0 },{ 1,1,0,1 },{ 0,1 } });
	
}
void loadTeapotModel(vector<Vertex>& dst) {
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> mats;
	string error;
	if (!tinyobj::LoadObj(&attrib, &shapes, &mats, &error, "teapot.605.obj")) {
		printf("%s\n", error.c_str());
		throw exception();
	}
	for (auto shape : shapes) {
		printf("loading %s", shape.name.c_str());
		size_t index_offset = 0;
		for (auto vcount : shape.mesh.num_face_vertices) {
			for (size_t vertex_offset = 0; vertex_offset < vcount; vertex_offset++) {
				tinyobj::index_t idx = shape.mesh.indices[index_offset + vertex_offset];
				tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
				tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
				tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];
				tinyobj::real_t nx = attrib.normals[3 * idx.normal_index + 0];
				tinyobj::real_t ny = attrib.normals[3 * idx.normal_index + 1];
				tinyobj::real_t nz = attrib.normals[3 * idx.normal_index + 2];
				tinyobj::real_t tx = attrib.texcoords[2 * idx.texcoord_index + 0];
				tinyobj::real_t ty = attrib.texcoords[2 * idx.texcoord_index + 1];
				dst.push_back({ { vx,vy,vz }, { nx,ny,nz }, { 1,1,0,1 }, { tx,ty } });
			}
			index_offset += vcount;
		}
	}
}
void loadDragonModel(vector<Vertex>& dst) {
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> mats;
	string error;
	if (!tinyobj::LoadObj(&attrib, &shapes, &mats, &error, "dragon.obj")) {
		printf("%s\n", error.c_str());
		throw exception();
	}
	for (auto shape : shapes) {
		printf("loading %s", shape.name.c_str());
		size_t index_offset = 0;
		for (auto vcount : shape.mesh.num_face_vertices) {
			for (size_t vertex_offset = 0; vertex_offset < vcount; vertex_offset++) {
				tinyobj::index_t idx = shape.mesh.indices[index_offset + vertex_offset];
				tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
				tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
				tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];
				tinyobj::real_t nx = attrib.normals[3 * idx.normal_index + 0];
				tinyobj::real_t ny = attrib.normals[3 * idx.normal_index + 1];
				tinyobj::real_t nz = attrib.normals[3 * idx.normal_index + 2];
				tinyobj::real_t tx = attrib.texcoords[2 * idx.texcoord_index + 0];
				tinyobj::real_t ty = attrib.texcoords[2 * idx.texcoord_index + 1];
				dst.push_back({ { vx,vy,vz },{ nx,ny,nz },{ 1,1,0,1 },{ tx,ty } });
			}
			index_offset += vcount;
		}
	}
}
void createQuadModel(vector<Vertex>& dst) {
	XMFLOAT3 normal(0, 1, 0);
	XMFLOAT4 color(1, 0, 1, 1);
	XMFLOAT3 pins[100 * 100];
	XMFLOAT2 texs[100 * 100];

	for (int y = 0; y < 100; y++) {
		for (int x = 0; x < 100; x++) {
			texs[y * 100 + x] = { (float)x / 100.0f,(float)y / 100.0f };
			pins[y * 100 + x] = { (float)x / 25.0f,0.f,(float)(y / 25.0f),};
		}
	}

	for (int y = 0; y < 100-1; y++) {
		for (int x = 0; x < 100-1; x++) {
			auto tl = y * 100 + x;
			auto tr = y * 100 + (x + 1);
			auto br = (y + 1) * 100 + (x + 1);
			auto bl = (y + 1) * 100 + x;
			
			dst.push_back({ pins[tl],normal,color,texs[tl] });
			dst.push_back({ pins[br],normal,color,texs[br] });
			dst.push_back({ pins[bl],normal,color,texs[bl] });

			dst.push_back({ pins[tl],normal,color,texs[tl] });
			dst.push_back({ pins[tr],normal,color,texs[tr] });
			dst.push_back({ pins[br],normal,color,texs[br] });
		}
	}
}
cst::ModelService::ModelService()
{
	auto &screenService = ScreenService::current();
	screenService.with([this](ComPtr<ID3D11Device> device) {
		vector<Vertex> quadModelVerts;
		loadTeapotModel(quadModelVerts);

		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.ByteWidth = sizeof(Vertex)*quadModelVerts.size();

		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = quadModelVerts.data();
		
		ComPtr<ID3D11Buffer> buffer;
		ThrowIfFailed(device->CreateBuffer(&bd, &sd, &buffer));
		_models.insert({ "XZQuad",{buffer,quadModelVerts.size()} });

	});
}
ModelService & cst::ModelService::current()
{
	static ModelService current;
	return current;
	// TODO: insert return statement here
}

void cst::ModelService::with(std::string modelName, std::function<void(ModelInfo)> execute)
{
	execute(_models.at(modelName));
}
