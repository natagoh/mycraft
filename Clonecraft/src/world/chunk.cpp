#include "world/chunk.h"
#include <iostream>

Chunk::Chunk(void) {
	/*for (int x = 0; x < CHUNK_DIM; x++) {
		for (int y = 0; y < CHUNK_DIM; y++) {
			for (int z = 0; z < CHUNK_DIM; z++) {
				blocks[x + CHUNK_DIM * (y + CHUNK_DIM * z)] = Block(BlockType::GRASS);
			}
		}
	}*/

    for (int i = 0; i < NUM_BLOCKS; i++) {
        blocks[i] = Block(BlockType::GRASS);
    }
}

// get the block at the specified chunk coordinates
Block Chunk::getBlock(int x, int y, int z) {
	return blocks[x + CHUNK_DIM * (y + CHUNK_DIM * z)];
}

// is a block in the chunk visible at this coord?
bool Chunk::blockIsVisibleAt(int x, int y, int z) {
	// check edge of chunk
	bool x_in_range = x > 0 && x < CHUNK_DIM - 1;
	bool y_in_range = y > 0 && y < CHUNK_DIM - 1;
	bool z_in_range = z > 0 && z < CHUNK_DIM - 1;
	if (!x_in_range || !y_in_range || !z_in_range) {
		return true;
	}

	// check if neighboring blocks exist
	if (getBlock(x + 1, y, z).isVisible() && getBlock(x - 1, y, z).isVisible() &&
		getBlock(x, y + 1, z).isVisible() && getBlock(x, y - 1, z).isVisible() &&
		getBlock(x, y, z + 1).isVisible() && getBlock(x, y, z - 1).isVisible()) {
		return false;
	}

	return true;
}


// make sure to call generateMesh at least once before render
void Chunk::render() {
    mesh.render();
}

// todo implement this function
void Chunk::generateMesh() {
	for (int x = 0; x < CHUNK_DIM; x++) {
		for (int y = 0; y < CHUNK_DIM; y++) {
			for (int z = 0; z < CHUNK_DIM; z++) {
				//if (blockIsVisibleAt(x, y, z)) {
                    addBlockToMesh(x, y, z);
                    std::cout << "after iteration size of vertices " << vertices.size() << std::endl;
				//}
			}
		}
	}

    std::cout << "num blocks " << sizeof(blocks) / sizeof(blocks[0]) << std::endl;
    std::cout << "num vertices " << vertices.size() / 3 << std::endl;
    std::cout << "num uvs " << uvs.size() / 2 << std::endl;
    std::cout << "num indices " << indices.size() / 6 << std::endl;
    mesh = Mesh(vertices, uvs, indices);

    //mesh = Block(BlockType::GRASS).generateMesh();

    // clean up buffer vectors once data already pushed to mesh
    vertices.clear();
    uvs.clear();
    indices.clear();
}

// add the block at x, y, z to the chunk's mesh
void Chunk::addBlockToMesh(int x, int y, int z) {
    std::vector<GLfloat> base_vertices {
        // front
        -0.5f, 0.5f, 0.5f,		// V0
        -0.5f, -0.5f, 0.5f,		// V1
        0.5f, -0.5f, 0.5f,		// V1
        0.5f, 0.5f, 0.5f,		// V3
        // back
        -0.5f, 0.5f, -0.5f,		// V4
        0.5f, 0.5f, -0.5f,		// V5
        -0.5f, -0.5f, -0.5f,	// V6
        0.5f, -0.5f, -0.5f,		// V7
        // right
        0.5f, 0.5f, 0.5f,		// V8: V3 repeated
        0.5f, -0.5f, 0.5f,		// V9: V2 repeated
        // left
        -0.5f, 0.5f, -0.5f,		// V10: V4 repeated
        -0.5f, -0.5f, -0.5f,	// V11: V6 repeated
        // top
        -0.5f, 0.5f, -0.5f,		// V12: V4 repeated
        0.5f, 0.5f, -0.5f,		// V13: V5 repeated
        -0.5f, 0.5f, 0.5f,		// V14: V0 repeated
        0.5f, 0.5f, 0.5f,		// V15: V3 repeated
        // bot
        -0.5f, -0.5f, -0.5f,	// V16: V6 repeated
        0.5f, -0.5f, -0.5f,		// V17: V7 repeated
        -0.5f, -0.5f, 0.5f,		// V18: V1 repeated
        0.5f, -0.5f, 0.5f,		// V19: V2 repeated

    };

    std::vector<GLuint> base_indices {
        0, 1, 3, 3, 1, 2,		// front
        4, 7, 6, 5, 7, 4,		// back
        8, 9, 7, 5, 8, 7,	    // right
        10, 11, 0, 0, 11, 1,	// left
        12, 14, 15, 13, 12, 15,	// top
        16, 19, 18, 17, 19, 16, // bot
    };

   /* std::cout << "num base vertices " << base_vertices.size() << std::endl;
    std::cout << "num base indices " << base_indices.size() << std::endl;*/

    GLfloat texture_size = 16.0f;
    GLfloat atlas_size = 256.0f;

    GLfloat block_size = texture_size / atlas_size;

    // sides
    GLfloat u_offset = 2 * texture_size / atlas_size;
    GLfloat v_offset = 0 * texture_size / atlas_size;

    GLfloat top_left_x = 0.0f + u_offset;
    GLfloat top_left_y = 1.0f - v_offset;
    GLfloat top_right_x = block_size + u_offset;
    GLfloat top_right_y = 1.0f - v_offset;
    GLfloat bot_left_x = 0.0f + u_offset;
    GLfloat bot_left_y = 1.0f - block_size - v_offset;
    GLfloat bot_right_x = block_size + u_offset;
    GLfloat bot_right_y = 1.0f - block_size - v_offset;

    // top 
    u_offset = 0 * texture_size / atlas_size;
    v_offset = 0 * texture_size / atlas_size;

    GLfloat ttop_left_x = 0.0f + u_offset;
    GLfloat ttop_left_y = 1.0f - v_offset;
    GLfloat ttop_right_x = block_size + u_offset;
    GLfloat ttop_right_y = 1.0f - v_offset;
    GLfloat tbot_left_x = 0.0f + u_offset;
    GLfloat tbot_left_y = 1.0f - block_size - v_offset;
    GLfloat tbot_right_x = block_size + u_offset;
    GLfloat tbot_right_y = 1.0f - block_size - v_offset;

    // bot 
    u_offset = 1 * texture_size / atlas_size;
    v_offset = 0 * texture_size / atlas_size;

    GLfloat btop_left_x = 0.0f + u_offset;
    GLfloat btop_left_y = 1.0f - v_offset;
    GLfloat btop_right_x = block_size + u_offset;
    GLfloat btop_right_y = 1.0f - v_offset;
    GLfloat bbot_left_x = 0.0f + u_offset;
    GLfloat bbot_left_y = 1.0f - block_size - v_offset;
    GLfloat bbot_right_x = block_size + u_offset;
    GLfloat bbot_right_y = 1.0f - block_size - v_offset;


    std::vector<GLfloat> base_uvs {
        // front
        top_left_x, top_left_y,
        bot_left_x, bot_left_y,
        bot_right_x, bot_right_y,
        top_right_x, top_right_y,
        // back
        top_left_x, top_left_y,
        top_right_x, top_right_y,
        bot_left_x, bot_left_y,
        bot_right_x, bot_right_y,
        // right
        top_left_x, top_left_y,
        bot_left_x, bot_left_y,
        // left
        top_right_x, top_right_y,
        bot_right_x, bot_right_y,
        // top
        ttop_left_x, ttop_left_y,
        ttop_right_x, ttop_right_y,
        tbot_left_x, tbot_left_y,
        tbot_right_x, tbot_right_y,
        // bot
        btop_left_x, btop_left_y,
        btop_right_x, btop_right_y,
        bbot_left_x, bbot_left_y,
        bbot_right_x, bbot_right_y,
    };

    // should be 20 since 1 block has 20 vertices
    unsigned int num_vertices_per_block = base_vertices.size() / 3;

    // index of block we are adding
    unsigned int num_blocks = vertices.size() / 3;
    unsigned int block_index = num_blocks / num_vertices_per_block;
    std::cout << "num vertext coords " << num_vertices_per_block << std::endl;
    std::cout << "block index " << block_index << std::endl;

    // add the position-offset vertices of the newly added block
    for (size_t i = 0; i < num_vertices_per_block; i++) {
        vertices.push_back(base_vertices[i * 3] + position.x + x);
        vertices.push_back(base_vertices[i * 3 + 1] + position.y + y);
        vertices.push_back(base_vertices[i * 3 + 2] + position.z + z);
    }

    // add the indices of the newly added block
    for (size_t i = 0; i < base_indices.size(); i++) {
        indices.push_back(base_indices[i] + num_vertices_per_block * block_index);
    }

    // for now, all blocks have the same texture
    uvs.insert(std::end(uvs), std::begin(base_uvs), std::end(base_uvs));
}

