#include "gtest/gtest.h"
#include <gtest/gtest_prod.h>
#include "sge.h"
#include "TestEngine.h"

/**
 * Test basic funcionallity of the mesh builder.
 *
 */
TEST(TestMeshBuilder, builder)
{
	TestEngine testEngine;
	testEngine.startEngine();

	float positions[]{1.f,1.f,1.f};

	// Test a valid build
	{
		Mesh* mesh = MeshBuilder::builder()
			.setPositions(positions, 1)
			.build();

		EXPECT_TRUE(mesh != nullptr);

		// release resources
		delete mesh;
	}

	// Test an invalid build
	{
		Mesh* mesh = MeshBuilder::builder()
			.build();

		EXPECT_TRUE(mesh == nullptr);
	}

	testEngine.stopEngine();
}

/**
 * Test enable attribute basic logic
 *
 */
TEST(TestMeshBuilder, enableDisableAttribute)
{
	TestEngine testEngine;
	testEngine.startEngine();

	float positions[]{ 1.f,1.f,1.f };

	// Test basic enable attribute
	{
		auto mesh = MeshBuilder::builder()
			.setPositions(positions, 1)
			.enableAttribute(LayoutAttribute::Colors)
			.build();

		ASSERT_TRUE(mesh != nullptr);

		VertexLayout layout = mesh->getVertexLayout();

		EXPECT_NE(std::find(layout.attribs.begin(), layout.attribs.end(), LayoutAttribute::Colors), layout.attribs.end());

		// release resources
		delete mesh;
	}

	// Test basic disable attribute
	{
		auto mesh = MeshBuilder::builder()
			.setPositions(positions, 1)
			.disableAttribute(LayoutAttribute::Texcoords)
			.build();

		ASSERT_TRUE(mesh != nullptr);

		VertexLayout layout = mesh->getVertexLayout();

		EXPECT_EQ(std::find(layout.attribs.begin(), layout.attribs.end(), LayoutAttribute::Texcoords), layout.attribs.end());

		// release resources
		delete mesh;
	}

	testEngine.stopEngine();
}

/**
 * Test basic assignemnt of positions data.
 *
 */
TEST(TestMeshBuilder, setPositions)
{
	TestEngine testEngine;
	testEngine.startEngine();

	// Valdiate setPositions using raw array
	{
		float positions[]{ 1.f,0.f,-1.f };

		Mesh* mesh = MeshBuilder::builder()
			.setPositions(positions, 1)
			.build();

		ASSERT_TRUE(mesh != nullptr);

		auto actualPositions = mesh->getPositions();
		
		// Validate position data
		EXPECT_EQ(actualPositions->at(0).x, positions[0]);
		EXPECT_EQ(actualPositions->at(0).y, positions[1]);
		EXPECT_EQ(actualPositions->at(0).z, positions[2]);

		// release resources
		delete mesh;
	}

	// Valdiate setPositions a vector - no copy
	{
		std::vector<glm::vec3>* positions = new std::vector<glm::vec3>{ {1.f,0.f,-1.f} };

		Mesh* mesh = MeshBuilder::builder()
			.setPositions(*positions)
			.build();

		ASSERT_TRUE(mesh != nullptr);

		auto actualPositions = mesh->getPositions();

		// Validate position data
		EXPECT_EQ(actualPositions->at(0).x, positions->at(0).x);
		EXPECT_EQ(actualPositions->at(0).y, positions->at(0).y);
		EXPECT_EQ(actualPositions->at(0).z, positions->at(0).z);

		// release resources
		delete mesh;
	}

	// Valdiate setPositions a vector - with copy
	{
		float x = 1.f;
		float y = 1.f;
		float z = 1.f;
		std::vector<glm::vec3>* positions = new std::vector<glm::vec3>{ {x,y,z} };

		Mesh* mesh = MeshBuilder::builder()
			.setPositions(*positions, true)
			.build();

		ASSERT_TRUE(mesh != nullptr);

		// We clean the original position ptr to test if the mesh still contains valdi positions
		delete positions;

		auto actualPositions = mesh->getPositions();

		// Validate position data
		EXPECT_EQ(actualPositions->at(0).x, x);
		EXPECT_EQ(actualPositions->at(0).y, y);
		EXPECT_EQ(actualPositions->at(0).z, z);

		// release resources
		delete mesh;
	}

	testEngine.stopEngine();
}

/**
 * Test basic assignemnt of normals data.
 *
 */
TEST(TestMeshBuilder, setNormals)
{
	TestEngine testEngine;
	testEngine.startEngine();

	float positions[]{ .5f,1.f,-.6f };

	// Valdiate setPositions using raw array
	{
		float normals[]{ 1.f,0.f,-1.f };

		Mesh* mesh = MeshBuilder::builder()
			.setPositions(positions, 1)
			.setNormals(normals, 1)
			.build();

		ASSERT_TRUE(mesh != nullptr);

		auto actualNormals = mesh->getNormals();

		// Validate normals data
		EXPECT_EQ(actualNormals->at(0).x, normals[0]);
		EXPECT_EQ(actualNormals->at(0).y, normals[1]);
		EXPECT_EQ(actualNormals->at(0).z, normals[2]);

		// release resources
		delete mesh;
	}

	// Valdiate setNormals a vector - no copy
	{
		std::vector<glm::vec3>* normals = new std::vector<glm::vec3>{ {1.f,0.f,-1.f} };

		Mesh* mesh = MeshBuilder::builder()
			.setPositions(positions, 1)
			.setNormals(*normals)
			.build();

		ASSERT_TRUE(mesh != nullptr);

		auto actualNormals = mesh->getNormals();

		// Validate normals data
		EXPECT_EQ(actualNormals->at(0).x, normals->at(0).x);
		EXPECT_EQ(actualNormals->at(0).y, normals->at(0).y);
		EXPECT_EQ(actualNormals->at(0).z, normals->at(0).z);

		// release resources
		delete mesh;
	}

	// Valdiate setNormals a vector - with copy
	{
		float x = 1.f;
		float y = 1.f;
		float z = 1.f;
		std::vector<glm::vec3>* normals = new std::vector<glm::vec3>{ {x,y,z} };

		Mesh* mesh = MeshBuilder::builder()
			.setPositions(positions, 1)
			.setNormals(*normals, true)
			.build();

		ASSERT_TRUE(mesh != nullptr);

		// We clean the original normals ptr to test if the mesh still contains valid normals
		delete normals;

		auto actualNormals = mesh->getNormals();

		// Validate normals data
		EXPECT_EQ(actualNormals->at(0).x, x);
		EXPECT_EQ(actualNormals->at(0).y, y);
		EXPECT_EQ(actualNormals->at(0).z, z);

		// release resources
		delete mesh;
	}

	testEngine.stopEngine();
}

/**
 * Test basic funcionallity of addTexture and addTextures.
 *
 */
TEST(TestMeshBuilder, addTexture)
{
	TestEngine testEngine;
	testEngine.startEngine();

	float positions[]{ 1.f,1.f,1.f };

	// Test a addTexture call
	{
		Texture* tex = Texture::createEmptyTexture(100, 100);

		int expectedID = tex->getID();

		Mesh* mesh = MeshBuilder::builder()
			.setPositions(positions, 1)
			.addTexture(tex)
			.build();

		EXPECT_TRUE(mesh != nullptr);

		auto textures = mesh->getTextures();
		int actualID = textures[0]->getID();

		EXPECT_EQ(expectedID, actualID);

		// release resources
		delete mesh;
	}

	testEngine.stopEngine();
}