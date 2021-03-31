#ifndef CHUNK_HPP
#define CHUNK_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "render/mesh.h"
#include "render/texture.h"
#include "world/block.h"

#define CHUNK_DIM 16
#define NUM_BLOCKS CHUNK_DIM * CHUNK_DIM * CHUNK_DIM

class Chunk {
private:
	Block blocks[NUM_BLOCKS];
	glm::vec3 position = {};

	std::vector<GLfloat> vertices = {};
	std::vector<GLuint> indices = {};
	std::vector<GLfloat> uvs = {};

	Mesh mesh;

    static const GLfloat base_vertices[]; 
	static const GLushort base_indices[];

	void addBlockToMesh(int x, int y, int z);

public:
	Chunk(glm::vec3 position);
	//~Chunk();

	Block getBlock(int x, int y, int z);
	void setBlock(int x, int y, int z, Block block);

	glm::vec3 getPosition();
	void setPosition(glm::vec3 position);

	// get world space coords of chunk mesh vertices
	std::vector<glm::vec3> getVertexCoords();

	bool blockIsVisibleAt(int x, int y, int z);

	void render();
	void generateMesh();
};

#endif