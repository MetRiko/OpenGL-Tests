#include <iostream>
#include <array>

#include <functional>
#include <Core/window.hpp>
#include <Core/buffers.hpp>
#include <Core/shaders.hpp>
#include <Core/logger.hpp>
#include <Core/engine.hpp>

int main()
{
	// gl::VBO a({3.f, 2.f, 1.f, 9.f});

	// gl::VBO<4> b;
	// b.define({6,5,4,3});

	// gl::VBO<4> c;
	// c = {1,2,3,4};

	// a.print();
	// b.print();
	// c.print();

	gl::Window window;
	window.setScreen({800, 600}, "Hello OpenGL!");
	window.setVersion(4, 6);
	window.setup();

	// gl::ShaderProgram program("shaders/default.vert", "shaders/default.frag");

	gl::ShaderProgram shaderProgram2;
	// shaderProgram2.loadAndAttach("shaders/default.vert", "shaders/default.frag");
	// shaderProgram2.loadAttach("shaders/default.frag", "shaders/default.vert");
	shaderProgram2.loadAndSetup("shaders/default.vert", "shaders/default.frag");
	// shaderProgram2.loadAndAttach("shaders/default.vert");//, "shaders/default.frag");
	// shaderProgram2.loadAndAttach("shaders/default.frag");//, "shaders/default.frag");
	// program.load(gl::Shader::Vertex, "shaders/default.vert", gl::Shader::Fragment, "shaders/default.frag");
	// shaderProgram2.setup();
	shaderProgram2.defineUniforms("customColor");

	// program.defineUniform("customColor");
	// program.defineUniform("time");
	// program.defineUniforms("customColor", "time");

	gl::ShaderProgram shaderProgram;
	{
		gl::Shader vertex;
		// vertex.source(gl::Shader::Vertex);
		vertex.load("shaders/default.vert");
		vertex.setup();

		gl::Shader fragment;
		// fragment.source(gl::Shader::Fragment);
		// fragment.loadAndSetup("shaders/default.frag");
		// fragment.setup();

		// shaderProgram.attach(vertex, fragment);
		shaderProgram.attach(vertex, gl::Shader("shaders/default.frag"));

		// shaderProgram.defineUniform("time");

		// ...

		// Render ...
		// shaderProgram.setUniform("customColor", 1.f, 2.f, 3.f);
		// shaderProgram.setUniform("customColor", {1.f, 2.f, 3.f});
		// shaderProgram.setUniform("time", 1.f);
		// shaderProgram.setUniform("customColor", glm::vec3(1.f, 2.f, 3.f));

		shaderProgram.setup();
		shaderProgram.defineUniforms("customColor");
	}

	gl::VAO vao; 
    vao.defineVBO({
	     0.5f,  0.5f, 0.0f,  // top right
	     0.5f, -0.5f, 0.0f,  // bottom right
	    -0.5f, -0.5f, 0.0f,  // bottom left
	    -0.5f,  0.5f, 0.0f   // top left 
    });
    vao.defineEBO({    	
    	0, 1, 3,   // first triangle
    	1, 2, 3    // second triangle
    });
    
    vao.primitive(gl::VAO::Triangles);
    // vao.outline(true);
    // vao.applyShader(shaderProgram);
    vao.applyShader(shaderProgram2);
    vao.setup();


	gl::Engine engine;

	engine.setFunctions(
		[&window](){},
		[&window](double deltaTime){},
		[&](){

			gl::Logger.log("\nargs... | Type: float\n");
			vao.uniform("myColor", 0.1f);
			vao.uniform("myColor", 0.1f, 0.2f);
			vao.uniform("myColor", 0.1f, 0.2f, 0.3f);
			vao.uniform("myColor", 0.1f, 0.2f, 0.3f, 0.4f);
			gl::Logger.log("\nargs... | Type: double\n");
			vao.uniform("myColor", 0.1);
			vao.uniform("myColor", 0.1, 0.2);
			vao.uniform("myColor", 0.1, 0.2, 0.3);
			vao.uniform("myColor", 0.1, 0.2, 0.3, 0.4);
			gl::Logger.log("\nargs... | Type: unsigned int\n");
			vao.uniform("myColor", 1u);
			vao.uniform("myColor", 1u, 2u);
			vao.uniform("myColor", 1u, 2u, 3u);
			vao.uniform("myColor", 1u, 2u, 3u, 4u);
			gl::Logger.log("\nargs... | Type: int\n");
			vao.uniform("myColor", 1);
			vao.uniform("myColor", 1, 2);
			vao.uniform("myColor", 1, 2, 3);
			vao.uniform("myColor", 1, 2, 3, 4);

			gl::Logger.log("\nglm::vec | Type: float\n");
			vao.uniform("myColor", glm::vec2(0.1f, 0.2f));
			vao.uniform("myColor", glm::vec3(0.1f, 0.2f, 0.3f));
			vao.uniform("myColor", glm::vec4(0.1f, 0.2f, 0.3f, 0.4f));
			gl::Logger.log("\nglm::vec | Type: double\n");
			vao.uniform("myColor", glm::vec2(0.1, 0.2));
			vao.uniform("myColor", glm::vec3(0.1, 0.2, 0.3));
			vao.uniform("myColor", glm::vec4(0.1, 0.2, 0.3, 0.4));
			gl::Logger.log("\nglm::vec | Type: unsigned int\n");
			vao.uniform("myColor", glm::uvec2(1u, 2u));
			vao.uniform("myColor", glm::uvec3(1u, 2u, 3u));
			vao.uniform("myColor", glm::uvec4(1u, 2u, 3u, 4u));
			gl::Logger.log("\nglm::vec | Type: int\n");
			vao.uniform("myColor", glm::ivec2(1, 2));
			vao.uniform("myColor", glm::ivec3(1, 2, 3));
			vao.uniform("myColor", glm::ivec4(1, 2, 3, 4));

			gl::Logger.log("\n{...} | Type: float\n");
			vao.uniform("myColor", {0.1f});
			vao.uniform("myColor", {0.1f, 0.2f});
			vao.uniform("myColor", {0.1f, 0.2f, 0.3f});
			vao.uniform("myColor", {0.1f, 0.2f, 0.3f, 0.4f});
			gl::Logger.log("\n{...} | Type: double\n");
			vao.uniform("myColor", {0.1});
			vao.uniform("myColor", {0.1, 0.2});
			vao.uniform("myColor", {0.1, 0.2, 0.3});
			vao.uniform("myColor", {0.1, 0.2, 0.3, 0.4});
			gl::Logger.log("\n{...} | Type: unsigned int\n");
			vao.uniform("myColor", {1u});
			vao.uniform("myColor", {1u, 2u});
			vao.uniform("myColor", {1u, 2u, 3u});
			vao.uniform("myColor", {1u, 2u, 3u, 4u});
			gl::Logger.log("\n{...} | Type: int\n");
			vao.uniform("myColor", {1});
			vao.uniform("myColor", {1, 2});
			vao.uniform("myColor", {1, 2, 3});
			vao.uniform("myColor", {1, 2, 3, 4});

			vao.draw();
			engine.quit();
		}
	);

	engine.setup(window);

	// gl::Logger.print();

	return 0;
}