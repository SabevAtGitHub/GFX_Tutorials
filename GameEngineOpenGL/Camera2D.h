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
		void setPosition(glm::vec2& newPos) { m_pos = newPos; m_needsMatrixUpdate = true; }

		void setScale(float newScale) { m_scale = newScale; m_needsMatrixUpdate = true; }

		// getters
		glm::vec2 getPosition() { return m_pos; }
		float getScale() { return m_scale; }
		
		glm::mat4 getCameraMatrix() { return m_cameraMatrix; }


	private:

		glm::vec2 m_pos;
		glm::mat4 m_cameraMatrix; // 4x4 matrix
		glm::mat4 m_orthoMatrix;

		float m_scale;
		bool m_needsMatrixUpdate;
		float m_scrW, m_scrH;
	};


}