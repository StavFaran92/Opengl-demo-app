#pragma once

// STL
#include <vector>
#include <string>
#include <map>

// sge
#include "Core.h"
#include "Object3D.h"
#include "Shader.h"
#include "Transform.h"
#include "Material.h"
#include "Texture.h"

// Forward declerations
struct aiNode;
struct aiScene;
struct aiMesh;
struct aiMaterial;
enum aiTextureType;

struct Vertex;
class IRenderer;
class Mesh;
class Scene;
class ModelImporter;
class MeshBuilder;
class ModelBuilder;

class EngineAPI Model : public Object3D
{
public:
	enum class PrimitiveType 
	{
		Quad,
		Cube,
		Sphere

	};
public:
	// -------------------- Methods -------------------- //
	/** Constructor */
	Model();

	/** Destructor */
	~Model();	

	bool AttachShader(Shader* shader);
	bool AttachShader(std::shared_ptr<Shader> shader);
	bool DetachShader();

	bool UseMaterial(std::shared_ptr<Material> material);

	bool use();

	Shader* GetShader();

	std::vector<const Texture*> GetTextures();

	void flipTexture();

	void SetReflection(bool val);
	void SetRefraction(bool val);

	virtual MeshBuilder* createMeshBuilder();
private:
	friend class Scene;
	friend class ModelImporter;
	friend class ModelBuilder;

	void update(float deltaTime);
	void addMesh(Mesh* mesh);
	
protected:
	// -------------------- Methods -------------------- //
	virtual void Draw(IRenderer& renderer, Shader* shader = nullptr);
	
	// -------------------- Attributes -------------------- //
	std::vector<std::shared_ptr<Mesh>> m_meshes;
	std::shared_ptr<Shader> m_shader = nullptr;
	std::shared_ptr<Material> m_material = nullptr;

	bool m_isReflective = false;
	bool m_isRefractive = false;
};