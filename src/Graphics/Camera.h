#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

namespace Graphics 
{
    // In progress, for now, delegates keyboard direction control 
    enum Direction 
    {
        LEFT,
        RIGHT,
        FORWARDS,
        BACKWARDS,
        UP,
        DOWN
    };




    /**
     * This class provides camera functionality, 
     * allowing for accurate rendering of a given area. 
     *  
     */
    class Camera 
    {
        public:
            Camera(std::pair<float, float>& resolution); 
	    
	    void setZoom(float zoom);
	    float getZoom();  

            void processMousePosition(double xoffset, double yoffset, bool constrainPitch = true);
            void updateDirection();
	    void resetPosition();
	    
	    bool isFirstMouse();
            void startMouse();
	 
	    glm::vec3 getDirection();
            glm::mat4 getViewMatrix(); 
	    glm::mat4 getProjectionMatrix();
	    
	    void setCameraPos(glm::vec3 position);

	    void processKeyboard(Graphics::Direction keyPressed, float deltaTime);
            void processMouseScroll(float yoffset);
	   
	    void disableCamera();
	    void enableCamera(); 
	    bool getCameraStatus();
	  
	    void setAspectRatio(float ratio);

        private:
	    bool CAMERA_ENABLED = false;
            bool firstMouse = true; 

	    float MouseSensitivity = 0.1f; 
            float MovementSpeed = 2.5f;

	    // Direction render box data
	    float pitch = 0; 
            float yaw = 0; 
	    float zoom = 45.0f; 

	    // Vector data
	    glm::vec3 Position = glm::vec3(0.0f, 1.0f, 3.0f);
            glm::vec3 CameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
            glm::vec3 CameraDirection = glm::normalize(Position - CameraTarget); 
            glm::mat4 Projection = glm::mat4(1.0f);
            glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
            glm::vec3 CameraRight = glm::normalize(glm::cross(WorldUp, CameraDirection));
            glm::vec3 CameraUp = glm::cross(CameraRight, CameraFront);
	     
	    glm::vec3 Direction; 
            glm::vec3 CameraFront;

	    float Velocity;
            float AspectRatio; 

            void setCameraFront(glm::vec3 direction);
    };
}




#endif 
