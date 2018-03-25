#include "Camera2D.h"

namespace ge {
	Camera2D::Camera2D() :
		pos(0.0f, 0.0f),
		cameraMatrix(1.0f),
		orthoMatrix(1.f),
		scale(1.0f),
		needsMatrixUpdate(true),
		scrW(600.f),
		scrH(480.f)
	{/* empty */ }

	Camera2D::~Camera2D() {/* empty */ }

	void Camera2D::init(int screenWidth, int screenHeight)
	{
		scrW = (float)screenWidth;
		scrH = (float)screenHeight;

		orthoMatrix = glm::ortho(0.0f, scrW, 0.0f, scrH);
	}

	void Camera2D::update()
	{
		if (needsMatrixUpdate) {

			glm::vec3 translator(-pos.x + scrW / 2.f , -pos.y + scrH / 2.f, 0.0f);

			// translating 2D camera position
			cameraMatrix = glm::translate(orthoMatrix, translator);

			// scaling 2D camera
			glm::vec3 scaler(scale, scale, 0.0f);
			cameraMatrix = glm::scale(glm::mat4(1.0f), scaler) * cameraMatrix;

			needsMatrixUpdate = false;
		}
	}

	glm::vec2 Camera2D::covertScreenToWorld(glm::vec2 screenCoords)
	{
		// first inverting y direction
		screenCoords.y = scrH - screenCoords.y;

		// converting to screen centered coordinate system, e.g. pos(0.0f, 0.0f) is screen center
		screenCoords -= glm::vec2(scrW / 2.f, scrH / 2.f);

		// adjusting coordinates with current camerea scale
		screenCoords /= scale;

		// translating with respect to camera position
		screenCoords += pos;

		return screenCoords;
	}

	// AABB test to see if object is inside the view
	bool Camera2D::isInView(glm::vec2& Pos, const glm::vec2& Dim)
	{

		auto scaledScrDims = glm::vec2(scrW, scrH) / scale;

		const float MIN_DISTANCE_X = (Dim.x + scaledScrDims.x) / 2.f;
		const float MIN_DISTANCE_Y = (Dim.y + scaledScrDims.y) / 2.f;

		// finding the distance vector between tile center
		// and the agent center
		auto centerPos = Pos + Dim / 2.f;

		// the distance between the object's center and camera center
		auto distVec = centerPos - this->pos; // camera position is centered

		float xDepth = MIN_DISTANCE_X - abs(distVec.x);
		float yDepth = MIN_DISTANCE_Y - abs(distVec.y);

		/* if collision occured */
		if (xDepth > 0 && yDepth > 0) {
			return true;
		}

		return false;
	}

} //namespace ge