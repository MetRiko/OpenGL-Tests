#include <iostream>
#include <array>

#include <functional>
#include <Core/window.hpp>
#include <Core/buffers.hpp>
#include <Core/shaders.hpp>
#include <Core/logger.hpp>
#include <Core/engine.hpp>

namespace gl {

}

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

	gl::ShaderProgram shaderProgram;
	{
		gl::Shader vertex;
		// vertex.source(gl::Shader::Vertex);
		vertex.load("shaders/default.vert");
		vertex.setup();

		gl::Shader fragment;
		// fragment.source(gl::Shader::Fragment);
		fragment.load("shaders/default.frag");
		fragment.setup();

		shaderProgram.attach(vertex, fragment);
		shaderProgram.setup();
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
    vao.applyShader(shaderProgram);
    vao.setup();


	gl::Engine engine;

	engine.setFunctions(
		[&window](){},
		[&window](double deltaTime){},
		[&](){
			vao.draw();
		}
	);

	engine.setup(window);

	// gl::Logger.print();

	return 0;
}