#pragma once
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

namespace ge {
	class Camera2D
	{
	public:
		Camera2D();
		~Camera2D();
		// must be called before window.create();
		void init(int screenWidth, int screenHeight);

		void update();

		glm::vec2 covertScreenToWorld(glm::vec2 screenCoords);

		bool isInView(glm::vec2& Pos, const glm::vec2& Dim);

		// setters
		void setPosition(glm::vec2& newPos) { pos = newPos; needsMatrixUpdate = true; }

		void setScale(float newScale) { scale = newScale; needsMatrixUpdate = true; }

		// getters
		glm::vec2 getPosition() { return pos; }
		float getScale() { return scale; }
		
		glm::mat4 getCameraMatrix() { return cameraMatrix; }


	private:

		glm::vec2 pos;
		glm::mat4 cameraMatrix; // 4x4 matrix
		glm::mat4 orthoMatrix;

		float scale;
		bool needsMatrixUpdate;
		float scrW, scrH;
	};


}