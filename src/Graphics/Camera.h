#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>




namespace Graphics {




    enum Direction 
    {
        LEFT,
        RIGHT,
        FORWARDS,
        BACKWARDS,
        UP,
        DOWN
    };




    class Camera 
    {
        public:
            Camera(int width, int height);
            void processMousePosition(double xoffset, double yoffset, bool constrainPitch = true);
	 
            float Zoom = 45.0f; 

            float getLastX();
            float getLastY(); 
	    void setLastX(double X);
            void setLastY(double Y);
            bool isFirstMouse();
            void startMouse();
            void updateDirection();
	    glm::vec3 getDirection();
            glm::mat4 getViewMatrix(); 
	    glm::mat4 getProjectionMatrix();
	    void setCameraPos(glm::vec3 position);
	    void processKeyboard(Graphics::Direction keyPressed, float deltaTime);
            void processMouseScroll(float yoffset);
	    void disableCamera();
	    void enableCamera(); 
	    bool getCameraStatus();
	    void resetPosition();

        private:
	    bool CAMERA_ENABLED = false;
	    float MouseSensitivity = 0.1f; 
            float MovementSpeed = 2.5f;
            float Velocity;
            float ScreenWidth; 
            float ScreenHeight; 
            float pitch = 0; 
            float yaw = 0; 
            int lastX; 
            int lastY; 
            bool firstMouse = true; 
            glm::vec3 Direction; 
            glm::vec3 CameraFront;
            GLFWwindow *window;
            glm::vec3 Position = glm::vec3(0.0f, 0.0f, 3.0f);
            glm::vec3 CameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
            glm::vec3 CameraDirection = glm::normalize(Position - CameraTarget); 
            glm::mat4 Projection = glm::mat4(1.0f);
            glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
            glm::vec3 CameraRight = glm::normalize(glm::cross(WorldUp, CameraDirection));
            glm::vec3 CameraUp = glm::cross(CameraRight, CameraFront);

            void setCameraFront(glm::vec3 direction);
    };

}

#endif // !CAMERA_H



