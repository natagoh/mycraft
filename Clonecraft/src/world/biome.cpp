#include "FastNoiseLite.h"
#include "world/chunk.h"
#include "world/biome.h"

#include <iostream>

GLshort* Biome::generateHeightMap(glm::vec3 position) {
	// simplex noise [-1, 1]
	FastNoiseLite noise;
	noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	float amplitude[] = {
		1.0f, 0.5f, 0.25f,
	};

	// get height from noise function
	int height_limit = 64; // can be different from world limit
	GLshort* height_map = new GLshort[CHUNK_DIM * CHUNK_DIM];
	for (int x = 0; x < CHUNK_DIM; x++) {
		for (int z = 0; z < CHUNK_DIM; z++) {
			int world_x = x + position.x;
			int world_z = z + position.z;

			float height = 0;
			float amplitude_sum = 0;
			for (int i = 0; i < sizeof(amplitude) / sizeof(amplitude[0]); i++) {
				noise.SetSeed(i + 666);
				height += amplitude[i] * (noise.GetNoise((float)world_x * 1.0f / amplitude[i], (float)world_z * 1.0f / amplitude[i]) + 1.0f) / 2.0f;
				amplitude_sum += amplitude[i];
			}
			height /= amplitude_sum;
			height_map[x + CHUNK_DIM * z] = height * (float) height_limit;
		}
	}

	return height_map;
}

GLshort* Biome::generateMoistureMap(glm::vec3 position) {
	// simplex noise [-1, 1]
	FastNoiseLite noise;
	noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	float amplitude[] = {
		1.0f, 0.5f, 0.25f, 0.125,
	};

	// get moisture from noise function
	int moisture_limit = 64; // can be different from world limit
	GLshort* moisture_map = new GLshort[CHUNK_DIM * CHUNK_DIM];
	//std::cout << "====================" << std::endl; 
	for (int x = 0; x < CHUNK_DIM; x++) {
		for (int z = 0; z < CHUNK_DIM; z++) {
			int world_x = x + position.x;
			int world_z = z + position.z;

			float moisture = 0;
			float amplitude_sum = 0;
			for (int i = 0; i < sizeof(amplitude) / sizeof(amplitude[0]); i++) {
				noise.SetSeed(i + 42);
				moisture += amplitude[i] * (noise.GetNoise((float)world_x * 1.0f / amplitude[i], (float)world_z * 1.0f / amplitude[i]) + 1.0f) / 2.0f;
				amplitude_sum += amplitude[i];
			}
			moisture /= amplitude_sum;
			moisture_map[x + CHUNK_DIM * z] = moisture * (float)moisture_limit;
		}
	}

	return moisture_map;
}

// elevation is world y coord
// height is from height map
// moisture is from moisture map
BlockType Biome::getBlockType(int elevation, int height, int moisture) {
	int height_limit = 64; // todo: make this biome specific
	int moisture_limit = 64;
	int water_level = (float)height_limit * 0.15;

	if (elevation > height) {
		if (elevation <= water_level) {
			return BlockType::WATER;
		} else {
			return BlockType::AIR;
		}
	} else if (elevation <= (float)height_limit * 0.2) {
		return BlockType::SAND;
	} else if (elevation <= (float)height_limit * 0.5) {
		return BlockType::GRASS;
	} else if (elevation <= (float)height_limit * 0.7) {
		if (moisture < (float)moisture_limit * 0.16) {
			return BlockType::SAND;
		} else if (moisture < (float)moisture_limit * 0.50) {
			return BlockType::STONE;
		} else {
			return BlockType::GRASS;
		}
	} else {
		return BlockType::SNOW;
	}
}
