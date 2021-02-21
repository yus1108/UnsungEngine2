#pragma once

namespace UEngine
{
	class DXGeometryFigurePrefab
	{
	public:
		static class DXRenderObject* CreatePoint();
		static class DXRenderObject* CreateLine();
		static class DXRenderObject* CreateTriangle();
		static class DXRenderObject* CreateRectangle();
		static class DXRenderObject* CreateCircle(unsigned slice = 3600);
	};
}


