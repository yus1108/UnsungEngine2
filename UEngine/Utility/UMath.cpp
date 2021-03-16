#include "UMath.h"

namespace UEngine
{
	namespace Utility
	{
		const float UMath::PI = 3.14159f;

		const float UMath::Convert_DegreeToRad(const float& degree)
		{
			//return degree / 180.0f * 3.14159f;
			return degree * 0.0174532f;
		}
		const double UMath::Convert_DegreeToRad(const double& degree)
		{
			//return degree / 180.0 * 3.14159265359;
			return degree * 0.0174532925199;
		}

		const UEngine::Utility::Coordinate2D UMath::ConvertPixelToNDC(const UEngine::Utility::Coordinate2D& mousePos, RECT clientSize)
		{
			UEngine::Utility::Coordinate2D pos = mousePos;
			pos.x = (pos.x - clientSize.left) / clientSize.right;
			pos.y = (pos.y - clientSize.top) / clientSize.bottom;
			pos.x = pos.x * 2.0f - 1;
			pos.y = pos.y * 2.0f - 1;

			pos.y *= -1.0f;

			return pos;
		}
	}
}