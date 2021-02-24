#pragma once

#include <list>
#include <queue>
#include <map>

#include "../Utility/UtilityDefinitions.h"
#include "../Utility/UMath.h"
#include "../Utility/UTime.h"
#include "../Utility/UThreadPool.h"
#include "../WinApplication/uengine_winapp.h"
#include "../DXRenderer/dxrframework.h"


#include "UEngineDefinitions.h"
#include "Pool\RenderObjectPool.h"
#include "Pool\ConstantBufferPool.h"
#include "Game\GameScene.h"
#include "Game\GameState.h"
#include "Object\Component\IComponent.h"
#include "Object\Component\Camera.h"
#include "Object\Component\Material.h"
#include "Object\Component\RenderComponent.h"
#include "Object\Component\Transform.h"
#include "Object\GameObject.h"

