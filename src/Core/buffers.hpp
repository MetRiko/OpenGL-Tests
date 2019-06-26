#pragma once

#include <array>
#include <iostream>

#include <glm/vec3.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <Core/shaders.hpp>

namespace gl {

	struct VAO {

		std::vector<float> vertices;
		std::vector<unsigned int> indices;

		unsigned int vaoId = 0;
		unsigned int vboId = 0;
		unsigned int eboId = 0;

		ShaderProgram* shaderProgram = nullptr;

		enum {
			Points = GL_POINTS, 
			Lines = GL_LINES, 
			LineStrip = GL_LINE_STRIP, 
			LineLoop = GL_LINE_LOOP, 
			Triangles = GL_TRIANGLES, 
			TriangleStrip = GL_TRIANGLE_STRIP, 
			TriangleFan = GL_TRIANGLE_FAN
		};

		int primitiveType = Triangles;

		unsigned int indicesAmount = 0;

		void createVAO() {
			glGenVertexArrays(1, &vaoId);  
		}

		void createVBO() {
			glGenBuffers(1, &vboId);
		}

		void createEBO() {
			// Later
		}

		void create() {
			createVAO();
			if (not vertices.empty())
				createVBO();
			if (not indices.empty())
				createEBO();
		}

		void primitive(int type) {
			primitiveType = type;
		}

		void bindVAO() {
			glBindVertexArray(vaoId);
		}

		void bindVBO() {
			if (not vertices.empty()) {
				glBindBuffer(GL_ARRAY_BUFFER, vboId);
			}
		}

		void bindEBO() {
			// Later

			// if (not vertices.empty()) {
			// 	glBindBuffer(GL_ARRAY_BUFFER, vboId);
			// 	int size = sizeof(float) * vertices.size();
			// 	glBufferData(GL_ARRAY_BUFFER, size, &vertices.front(), GL_STATIC_DRAW);
			// 	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			// 	glEnableVertexAttribArray(0);
			// }
		}

		void configure() {
			bindVAO();
			bindVBO();
			if (not vertices.empty()) {
				int size = sizeof(float) * vertices.size();
				glBufferData(GL_ARRAY_BUFFER, size, &vertices.front(), GL_STATIC_DRAW);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
				glEnableVertexAttribArray(0);
			}
		}

		void setup() {
			create();
			configure();
		}

		void defineVBO(const std::initializer_list<float> vertices) {
			this->vertices = vertices;
			if (indices.empty()) 
				indicesAmount = vertices.size()/3;
		}

		void defineEBO(const std::initializer_list<unsigned int> indices) {
			this->indices = indices;
			indicesAmount = indices.size();
		}

		void applyShader(ShaderProgram& shaderProgram) {
			this->shaderProgram = &shaderProgram;
		}

		void useShader() {		
			if (shaderProgram)	
				glUseProgram(*shaderProgram);
		}

		void draw() {
			useShader();
			bindVAO();			
			glDrawArrays(primitiveType, 0, indicesAmount);
			glBindVertexArray(0);
		}

	};


// * * * Not ready yet : Work held * * * //

	// template <size_t Size>
	// struct VBO {
	// 	std::array<float, Size> vertices;

	// 	unsigned int vboId;

	// 	VBO() {
	// 	}

	// 	VBO(float const(&src)[Size]) {
	// 		define(src);
	// 	}

	// 	constexpr VBO& operator=(float const(&src)[Size]) {
	// 		std::copy(std::begin(src), std::end(src), std::begin(vertices));
	// 		return *this;
	// 	}

	// 	void define(float const(&src)[Size]) {
	// 		std::copy(std::begin(src), std::end(src), std::begin(vertices));
	// 	}

	// 	void create() {
	// 		glGenBuffers(1, &vboId);
	// 	}

	// 	void bind() {
	// 		glBindBuffer(GL_ARRAY_BUFFER, vboId);
	// 		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 	}

	// 	void addVertexAttribPointer(unsigned int id, unsigned int size) {

	// 		glVertexAttribPointer(id, size, GL_FLOAT, GL_FALSE, size * sizeof(float), (void*)0);
	// 		glEnableVertexAttribArray(id);  
	// 	}

	// 	void setup() {
	// 		create();
	// 		bind();
	// 	}

	// 	void print() {
	// 		for(int i=0; i<Size; ++i) {
	// 			std::cout<<vertices[i]<<' ';
	// 		}
	// 		std::cout<<'\n';
	// 	}
	// };

	// struct VBOD {
	// 	std::vector<glm::vec3> vertices;

	// 	unsigned int vboId;

	// 	void addVertex(glm::vec3 point) {
	// 		vertices.emplace_back(point);
	// 	}

	// 	void create() {
	// 		glGenBuffers(1, &vboId);
	// 	}

	// 	void print() {
	// 		for(auto& vec : vertices) {
	// 			std::cout << vec.x << ' ' << vec.y << ' ' << vec.z << ", ";
	// 		}
	// 		std::cout<<'\n';
	// 	}
	// };

}