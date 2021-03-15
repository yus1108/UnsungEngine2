#pragma once

#include <list>
#include <queue>
#include <map>
#include <array>

#include "../Utility/UtilityDefinitions.h"
#include "../Utility/UMath.h"
#include "../Utility/UTime.h"
#include "../Utility/UThreadPool.h"
#include "../WinApplication/uengine_winapp.h"
#include "../DXRenderer/dxrframework.h"

#include "../XMLSerializer/stdafx.h"

#include "UEngineDefinitions.h"
#include "DebugRenderer\DebugRenderer.h"

#include "Game Architecture\Physics2D\Component\Collider.h"
#include "Game Architecture\Physics2D\Component\CircleCollider.h"
#include "Game Architecture\Physics2D\Component\RectCollider.h"
#include "Math\Math.h"

#include "Game Architecture\Scene\GameView.h"
#include "Game Architecture\Scene\GameScene.h"
#include "Game Architecture\GameState.h"
#include "Game Architecture\Object\GameObject.h"
#include "Game Architecture\Object\Component\Component.h"
#include "Game Architecture\Object\Component\Transform.h"
#include "Game Architecture\Object\Component\RenderComponent.h"
#include "Game Architecture\Object\Component\Material.h"
#include "Game Architecture\Object\Component\Camera.h"
#include "Game Architecture\Object\Component\EditorScript.h"

