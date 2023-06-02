#pragma once

#include "OpenGLWindow.h"
#include <ShaderProgram.h>
#include <memory>
#include <AssetManager.h>
#include "Transform.h"

class Scene
{
public:
	Scene(OpenGLWindow* window);
	~Scene();

	bool init();
	void shutdown();
	void render(float dt);
	void update(float dt);
	OpenGLWindow* getWindow();

	void onKey(Key key, Action action, Modifier modifier);
	void onMouseMove(MousePosition mouseposition);
	void onMouseButton(MouseButton button, Action action, Modifier modifier);
	void onMouseScroll(double xscroll, double yscroll);
	void onFrameBufferResize(int width, int height);

private:
	OpenGLWindow* m_window;
	AssetManager m_assets;
    ShaderProgram* m_shader;
    GLuint vaoID, vboID;

    int indSize;

    float totalTime;
    float colorTime;
    float colorR, colorG, colorB;
    bool reverseAnim;

    Transform *world;
    Transform *head;
    Transform *body;
    Transform *leftUpperArm;
    Transform *rightUpperArm;
    Transform *leftLowerArm;
    Transform *rightLowerArm;
    Transform *leftLeg;
    Transform *rightLeg;
    Transform *leftArm;
    Transform *rightArm;

    Transform *view;
    Transform *perspective;

    void initRobot();
};

