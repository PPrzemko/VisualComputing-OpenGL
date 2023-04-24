#include "Scene.h"
#include <AssetManager.h>

Scene::Scene(OpenGLWindow * window) :
	m_window(window)
{
	assert(window != nullptr);
}

Scene::~Scene()
{}

bool Scene::init()
{
	try
	{
		//Load shader
		m_assets.addShaderProgram("shader", AssetManager::createShaderProgram("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl"));
		m_shader = m_assets.getShaderProgram("shader");
        m_shader->use();

        /* Original
        float vertices[] = {-0.5, -0.5, 0.0, 0.0, 1.0,
                            0.5, -0.5, 0.0, 0.0, 1.0,
                            0.5, 0.5, 0.0, 1.0, 0.0,
                            0.0, 1.0, 1.0, 0.0, 0.0,
                            -0.5, 0.5, 0.0, 1.0, 0.0}; */
        //
        float vertices[] = {-1 + 0.5, 1 - 0.5 , 1.0, 1.0, 1.0, //1
                            -1 +0.5, 0 - 0.5, 1.0, 1.0, 1.0, //2
                            -0.33 +0.5,0 - 0.5, 1.0, 1.0, 1.0, //3
                            -0.33 +0.5, 0.33 - 0.5, 1.0, 1.0, 1.0, //4
                            -0.66 +0.5, 0.33 - 0.5, 1.0, 1.0, 1.0, //5
                            -0.66 +0.5, 1 - 0.5, 1.0, 1.0, 1.0}; //6

        /* Original
        int indices[] = {0, 1, 2,
                         0, 2, 4,
                         4, 2, 3}; */

        int indices[] = {5, 1, 0, //1
                         5, 4, 0, //2
                         4, 2, 1, //3
                         4, 3, 2}; //4
		/*
		 * ************
		 * Place your code here!
		 * ************
		 */


        glGenBuffers(1, &vboID); //generate VBO
        glBindBuffer(GL_ARRAY_BUFFER,vboID); //Activate Buffer
        glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),&vertices,GL_STATIC_DRAW); //upload Data to vram

        glGenVertexArrays(1,&vaoID);
        glBindVertexArray(vaoID);

        //VAO Attribute Verticies
        glVertexAttribPointer(0, 2, GL_FLOAT, false, 5*4, 0); //4 Byte one Float
        glEnableVertexAttribArray(0);
        //VAO Attribute Colors
        glVertexAttribPointer(1, 3, GL_FLOAT, false, 5*4, (void*)8); //8 offset cause color begins at 9th byte
        glEnableVertexAttribArray(1);

        GLuint iboID = 0;
        glGenBuffers(1, &iboID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,iboID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices), indices, GL_STATIC_DRAW);

        //Unbind
        /*
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); */

        std::cout << "Scene initialization done\n";
        return true;
	}
	catch (std::exception& ex)
	{
	    throw std::logic_error("Scene initialization failed:\n" + std::string(ex.what()) + "\n");
	}
}

void Scene::render(float dt)
{


    /*
    * ************
    * Place your code here!
    * ************
    */
    glBindVertexArray(vaoID); //activate VAO and implicit VBO
    glDrawElements(GL_TRIANGLES, 9,GL_UNSIGNED_INT,0);
    glBindVertexArray(0);



}

void Scene::update(float dt)
{

}

OpenGLWindow * Scene::getWindow()
{
	return m_window;
}

void Scene::onKey(Key key, Action action, Modifier modifier)
{

}

void Scene::onMouseMove(MousePosition mouseposition)
{

}

void Scene::onMouseButton(MouseButton button, Action action, Modifier modifier)
{

}

void Scene::onMouseScroll(double xscroll, double yscroll)
{

}

void Scene::onFrameBufferResize(int width, int height)
{

}
void Scene::shutdown()
{

}
