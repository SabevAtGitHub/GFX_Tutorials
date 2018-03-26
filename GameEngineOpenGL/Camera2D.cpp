#include "Camera2D.h"

namespace ge {
	Camera2D::Camera2D() :
		m_pos(0.0f, 0.0f),
		m_cameraMatrix(1.0f),
		m_orthoMatrix(1.f),
		m_scale(1.0f),
		m_needsMatrixUpdate(true),
		m_scrW(600.f),
		m_scrH(480.f)
	{/* empty */ }

	Camera2D::~Camera2D() {/* empty */ }

	void Camera2D::init(int screenWidth, int screenHeight)
	{
		m_scrW = (float)screenWidth;
		m_scrH = (float)screenHeight;

		m_orthoMatrix = glm::ortho(0.0f, m_scrW, 0.0f, m_scrH);
	}

	void Camera2D::update()
	{
		if (m_needsMatrixUpdate) {

			glm::vec3 translator(-m_pos.x + m_scrW / 2.f , -m_pos.y + m_scrH / 2.f, 0.0f);

			// translating 2D camera position
			m_cameraMatrix = glm::translate(m_orthoMatrix, translator);

			// scaling 2D camera
			glm::vec3 scaler(m_scale, m_scale, 0.0f);
			m_cameraMatrix = glm::scale(glm::mat4(1.0f), scaler) * m_cameraMatrix;

			m_needsMatrixUpdate = false;
		}
	}

	glm::vec2 Camera2D::covertScreenToWorld(glm::vec2 screenCoords)
	{
		// first inverting y direction
		screenCoords.y = m_scrH - screenCoords.y;

		// converting to screen centered coordinate system, e.g. m_pos(0.0f, 0.0f) is screen center
		screenCoords -= glm::vec2(m_scrW / 2.f, m_scrH / 2.f);

		// adjusting coordinates with current camerea scale
		screenCoords /= m_scale;

		// translating with respect to camera position
		screenCoords += m_pos;

		return screenCoords;
	}

	// AABB test to see if object is inside the view
	bool Camera2D::isInView(glm::vec2& Pos, const glm::vec2& Dim)
	{

		auto scaledScrDims = glm::vec2(m_scrW, m_scrH) / m_scale;

		const float MIN_DISTANCE_X = (Dim.x + scaledScrDims.x) / 2.f;
		const float MIN_DISTANCE_Y = (Dim.y + scaledScrDims.y) / 2.f;

		// finding the distance vector between tile center
		// and the agent center
		auto centerPos = Pos + Dim / 2.f;

		// the distance between the object's center and camera center
		auto distVec = centerPos - this->m_pos; // camera position is centered

		float xDepth = MIN_DISTANCE_X - abs(distVec.x);
		float yDepth = MIN_DISTANCE_Y - abs(distVec.y);

		/* if collision occured */
		if (xDepth > 0 && yDepth > 0) {
			return true;
		}

		return false;
	}

} //namespace ge