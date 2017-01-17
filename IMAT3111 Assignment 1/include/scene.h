#pragma once

#ifndef SCENE_H
#define SCENE_H

#include <glfw3.h>
#include "QuatCamera.h"

namespace imat3111
{

class Scene
{
public:
	Scene() : m_animate(true) { }
	
    /**
      Load textures, initialize shaders, etc.
      */
    virtual void initScene(QuatCamera camera) = 0;

    /**
      This is called prior to every frame.  Use this
      to update your animation.
      */
   // virtual void update(GLFWwindow * window, float t ) = 0;

    /**
      Draw your scene.
      */
    virtual void render(QuatCamera camera) = 0;

    /**
      Called when screen is resized
      */
    virtual void resize(QuatCamera camera,int, int) = 0;
    
    void animate( bool value ) { m_animate = value; }
    bool animating() { return m_animate; }

	void showCubeMap(bool value) { m_showCubeMap = value; };
	bool showingCubeMap() { return m_showCubeMap; };
	void toggleCubeMap() { m_showCubeMap = !m_showCubeMap; };
	void toggleRefract() { m_refract = !m_refract; };
    
protected:
	bool m_animate;
	bool m_showCubeMap;
	bool m_refract;
};

}

#endif // SCENE_H
