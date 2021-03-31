#include "world.h"
#include <iostream>

#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin.h"

World::World() {
	// prepare RENDER_DISTANCE * RENDER_DISTANCE chunks
	chunks_to_render = new glm::vec3[RENDER_DISTANCE * RENDER_DISTANCE];
	for (int x = 0; x < RENDER_DISTANCE; x++) {
		for (int z = 0; z < RENDER_DISTANCE; z++) {
			// perlin noise to generate height
			float height = stb_perlin_noise3(x, 0, z, 0, 0, 0);
 
			// for now only add chunks horizontally
			// todo: add vertical chunking
			glm::vec3 position = glm::vec3(x * CHUNK_DIM, 0.0f, z * CHUNK_DIM);
			auto chunk = std::make_shared<Chunk>(position);
			chunks.insert({ {position, chunk} });
			chunks_to_render[x + RENDER_DISTANCE * z] = position;

			// only need to generate meshes once since we don't change the mesh at all for now...
			chunk->generateMesh();
		}
	}
}

void World::render(Frustum frustum) {
	for (int i = 0; i < RENDER_DISTANCE * RENDER_DISTANCE; i++) {
		glm::vec3 chunk_coord = chunks_to_render[i];
		
		// find chunk in the unordered_map
		std::unordered_map<glm::vec3, std::shared_ptr<Chunk>>::const_iterator map_item = chunks.find(chunk_coord);

		if (map_item == chunks.end()) {
			std::cout << "Error: (World) chunk not found" << std::endl;
		} else {
			// check if chunk is inside frustum
			std::vector<glm::vec3> vertex_coords = map_item->second->getVertexCoords();
			if (frustum.cubeIntersection(vertex_coords)) {
				map_item->second->render();
			}
		}
	}
}