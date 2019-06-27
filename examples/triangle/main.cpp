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
	shaderProgram2.loadAndSetup("shaders/default.frag", "shaders/default.vert");
	// shaderProgram2.loadAndAttach("shaders/default.vert");//, "shaders/default.frag");
	// shaderProgram2.loadAndAttach("shaders/default.frag");//, "shaders/default.frag");
	// program.load(gl::Shader::Vertex, "shaders/default.vert", gl::Shader::Fragment, "shaders/default.frag");
	// shaderProgram2.setup();
	shaderProgram2.defineUniform("customColor");

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
		shaderProgram.defineUniform("customColor");
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
    vao.applyShader(shaderProgram);
    vao.setup();


	gl::Engine engine;

	engine.setFunctions(
		[&window](){},
		[&window](double deltaTime){},
		[&](){

			// shaderProgram.setUniform("customColor", 1.f, 2.f, 3.f);
			// shaderProgram.setUniform("customColor", {1.f, 2.f, 3.f});
			// shaderProgram.setUniform("customColor", glm::vec3(1.f, 2.f, 3.f));
			// shaderProgram.setUniform("time", 1.f);

			vao.program().setUniform("customColor", glm::vec3(0.1f, 0.f, 0.7f));
			vao.draw();
		}
	);

	engine.setup(window);

	// gl::Logger.print();

	return 0;
}