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
		vertex.source(gl::Shader::Vertex);
		vertex.setup();

		gl::Shader fragment;
		fragment.source(gl::Shader::Fragment);
		fragment.setup();

		shaderProgram.attach(vertex, fragment);
		shaderProgram.setup();
	}

	gl::VAO vao; 
    vao.defineVBO({
		 0.0f,  0.5f,  0.0f,
		 0.5f, -0.5f,  0.0f,
		-0.5f, -0.5f,  0.0f
    });
    vao.applyShader(shaderProgram);
    vao.setup();

    // vao.defineEBO({

    // });

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