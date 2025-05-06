#pragma once

#include <glm/glm.hpp>

class Scene
{
protected:
    glm::mat4 model;        
    glm::mat4 view;         
    glm::mat4 projection;

public:
    int width;
    int height;

	Scene() : m_animate(true), width(1280), height(720) { }
	virtual ~Scene() {}

	void setDimensions( int w, int h ) {
	    width = w;
	    height = h;
	}
	
    /**
      Load textures, initialize shaders, etc.
      */
    virtual void initScene() = 0;

    /**
      This is called prior to every frame.  Use this
      to update your animation.
      */
    virtual void update( float t ) = 0;

    /**
      Draw your scene.
      */
    virtual void render() = 0;

    /**
      Called when screen is resized
      */
    virtual void resize(int, int) = 0;

    virtual void handleInput(int key, int action) {}

    
    virtual void handleMouse(double xpos, double ypos) {}
    
    void animate( bool value ) { m_animate = value; }
    bool animating() { return m_animate; }
    
protected:
	bool m_animate;
};
