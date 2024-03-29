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

        float vertices[] = {0.5, -0.5, -0.5, 1, 0, 0,
                            0.5, -0.5, 0.5, 0, 1, 0,
                            -0.5, -0.5, 0.5, 0, 0, 1,
                            -0.5, -0.5, -0.5, 1, 1, 0,
                            0.5, 0.5, -0.5, 1, 0, 1,
                            0.5, 0.5, 0.5, 0, 1, 1,
                            -0.5, 0.5, 0.5, 1, 1, 1,
                            -0.5, 0.5, -0.5, 0.5, 1, 0.5};

        int indices[] = {1, 2, 3,
                         7, 6, 5,
                         4, 5, 1,
                         5, 6, 2,
                         2, 6, 7,
                         0, 3, 7,
                         0, 1, 3,
                         4, 7, 5,
                         0, 4, 1,
                         1, 5, 2,
                         3, 2, 7,
                         4, 0, 7};

        indSize = sizeof(indices);
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
        glVertexAttribPointer(0, 3, GL_FLOAT, false, 6*4, 0); //4 Byte one Float
        glEnableVertexAttribArray(0);
        //VAO Attribute Colors
        glVertexAttribPointer(1, 3, GL_FLOAT, false, 6*4, (void*)12); //8 offset cause color begins at 9th byte
        glEnableVertexAttribArray(1);

        GLuint iboID = 0;
        glGenBuffers(1, &iboID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,iboID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,indSize, indices, GL_STATIC_DRAW);

        //Unbind
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        //Camera
        view = new Transform();
        view->setMatrix(glm::lookAt(glm::vec3(0,0,-2), glm::vec3(0,0,1), glm::vec3(0,1,0)));
        m_shader->setUniform("view", view->getMatrix(),false);

        //Perspective
        perspective = new Transform();
        float ratio = 1280.0f / 720.0f;
        perspective->setMatrix(glm::perspective(45.0f, ratio, 0.5f, 5.0f));
        m_shader->setUniform("projection", perspective->getMatrix(),false);

        initRobot();

        //1.4 Backface culling
        /*
         * if enabled OpenGL only renders visible faces
         */
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);
        glCullFace(GL_BACK);

        glClearColor(0.10f, 0.10f, 0.10f, 1.0f);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS); // Default-Value, könnte auch weggelassen werden
        glClearDepth(1.0); // Default-Value, könnte auch weggelassen werden

        totalTime= 0;
        reverseAnim = true;

        colorR = 0.0f;
        colorG = 0.0f;
        colorB = 0.0f;

        std::cout << "Scene initialization done\n";
        return true;
	}
	catch (std::exception& ex)
	{
	    throw std::logic_error("Scene initialization failed:\n" + std::string(ex.what()) + "\n");
	}
}

void Scene::initRobot() {
    world = new Transform();
    world->rotate(glm::vec3(0.1, 0.4, 0));
    //world->scale(glm::vec3(0.5, 0.5, 0.5));
    //world->translate(glm::vec3(0, 0, 0.5));

    head = new Transform();
    head->translate(glm::vec3(0, 0.5, 0));
    head->scale(glm::vec3(0.3, 0.3, 0.3));

    body = new Transform();
    body->scale(glm::vec3(0.4, 0.6, 0.4));

    //Left Arm
    leftArm = new Transform();
    leftArm->translate(glm::vec3(-0.3, 0.15, 0));
    leftArm->rotate(glm::vec3(-0.80, 0, 0));

    leftUpperArm = new Transform();
    leftUpperArm->scale(glm::vec3(0.1, 0.3, 0.1));

    leftLowerArm = new Transform();
    leftLowerArm->translate(glm::vec3(0, -0.35, 0));
    leftLowerArm->scale(glm::vec3(0.1, 0.3, 0.1));
    leftLowerArm->rotateAroundPoint(glm::vec3(0, -0.15, 0), glm::vec3(0.45, 0, 0));

    //Right Arm
    rightArm = new Transform();
    rightArm->translate(glm::vec3(0.3, 0.15, 0));
    rightArm->rotate(glm::vec3(0.80, 0, 0));

    rightUpperArm = new Transform();
    rightUpperArm->scale(glm::vec3(0.1, 0.3, 0.1));

    rightLowerArm = new Transform();
    rightLowerArm->translate(glm::vec3(0, -0.35, 0));
    rightLowerArm->scale(glm::vec3(0.1, 0.3, 0.1));
    rightLowerArm->rotateAroundPoint(glm::vec3(0, -0.15, 0), glm::vec3(0.45, 0, 0));

    leftLeg = new Transform();
    leftLeg->translate(glm::vec3(-0.1, -0.6, 0));
    leftLeg->scale(glm::vec3(0.1, 0.5, 0.1));
    leftLeg->rotateAroundPoint(glm::vec3(-0.1, -0.12, 0), glm::vec3(-0.80, 0, 0));

    rightLeg = new Transform();
    rightLeg->translate(glm::vec3(0.1, -0.6, 0));
    rightLeg->scale(glm::vec3(0.1, 0.5, 0.1));
    rightLeg->rotateAroundPoint(glm::vec3(0.1, -0.12, 0), glm::vec3(0.80, 0, 0));
}

void Scene::render(float dt)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Clock for animation
    totalTime += dt;
    if(totalTime > 3.5){
        reverseAnim = !reverseAnim;
        totalTime = 0;

    }
    //Calculate new color and set colorAnimation Uniform
    /*colorTime += dt;
    float color = sin(colorTime) * 0.5;
    m_shader->setUniform("colorAnim", color); */

    if (m_window->getInput().getKeyState(Key::R) == KeyState::Pressed)
    {
        colorR= 1;
    }else
    {
        colorR= 0;
    if (m_window->getInput().getKeyState(Key::G) == KeyState::Pressed)
    {
        colorG= 1;
    }else{
        colorG= 0;}
    }
    if (m_window->getInput().getKeyState(Key::B) == KeyState::Pressed)
    {
        colorB= 1;
    }else{
        colorB= 0;
    }

    m_shader->setUniform("colorR", colorR);
    m_shader->setUniform("colorG", colorG);
    m_shader->setUniform("colorB", colorB);

    /*
    * ************
    * Place your code here!
    * ************
    */
    glBindVertexArray(vaoID); //activate VAO and implicit VBO


    if (m_window->getInput().getKeyState(Key::D) == KeyState::Pressed)
    {
        view->rotate(glm::vec3(0,0.6*dt,0));

    }
    if (m_window->getInput().getKeyState(Key::A) == KeyState::Pressed)
    {
        view->rotate(glm::vec3(0,-0.6*dt,0));

    }

    m_shader->setUniform("view", view->getMatrix(), false);
        //world
    //world->rotate(glm::vec3(0,0.3*dt,0));

    //head
    m_shader->setUniform("model", world->getMatrix() * head->getMatrix() , false);
    glDrawElements(GL_TRIANGLES, indSize,GL_UNSIGNED_INT,0);

    //body
    m_shader->setUniform("model", world->getMatrix() * body->getMatrix() , false);
    glDrawElements(GL_TRIANGLES, indSize,GL_UNSIGNED_INT,0);

    //leftUpperArm
    m_shader->setUniform("model", world->getMatrix() * leftArm->getMatrix() * leftUpperArm->getMatrix() , false);
    glDrawElements(GL_TRIANGLES, indSize,GL_UNSIGNED_INT,0);

    //rightUpperArm
    m_shader->setUniform("model", world->getMatrix() * rightArm->getMatrix() * rightUpperArm->getMatrix() , false);
    glDrawElements(GL_TRIANGLES, indSize,GL_UNSIGNED_INT,0);

    //leftLowerArm
    m_shader->setUniform("model", world->getMatrix() * leftArm->getMatrix() * leftLowerArm->getMatrix(), false);
    glDrawElements(GL_TRIANGLES, indSize,GL_UNSIGNED_INT,0);

    //rightLowerArm
    m_shader->setUniform("model", world->getMatrix() * rightArm->getMatrix() * rightLowerArm->getMatrix(), false);
    glDrawElements(GL_TRIANGLES, indSize,GL_UNSIGNED_INT,0);

    //leftLeg
    m_shader->setUniform("model", world->getMatrix() * leftLeg->getMatrix() , false);
    glDrawElements(GL_TRIANGLES, indSize,GL_UNSIGNED_INT,0);

    //rightLeg
    m_shader->setUniform("model", world->getMatrix() * rightLeg->getMatrix() , false);
    glDrawElements(GL_TRIANGLES, indSize,GL_UNSIGNED_INT,0);

    glBindVertexArray(0);

    if(reverseAnim){

        leftArm->rotate(glm::vec3(0.45*dt,0,0));
        rightArm->rotate(glm::vec3(-0.45*dt,0,0));

        leftLeg->rotateAroundPoint(glm::vec3(-0.1,-0.12,0),glm::vec3(0.45 *dt,0,0));
        rightLeg->rotateAroundPoint(glm::vec3(0.1,-0.12,0),glm::vec3(-0.45 *dt,0,0));
    }else{

        leftArm->rotate(glm::vec3(-0.45*dt,0,0));
        rightArm->rotate(glm::vec3(0.45*dt,0,0));

        leftLeg->rotateAroundPoint(glm::vec3(-0.1,-0.12,0),glm::vec3(-0.45 *dt,0,0));
        rightLeg->rotateAroundPoint(glm::vec3(0.1,-0.12,0),glm::vec3(0.45 *dt,0,0));
    }

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
    /*
    auto directioX = mouseposition.X - mouseposition.oldX;
    auto directioY = mouseposition.Y - mouseposition.oldY;
    view->rotate(glm::vec3(0.01*directioY,0.01*directioX,0)); */

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
