#pragma once
#ifndef _ENGINECORE_H
#define _ENGINECORE_H

/// All inlcudes for the CORE of the engine

#include "Engine.h"

#include "managers\LogManager.h"
#include "managers\ResourceManager.h"
#include "managers\SceneManager.h"
#include "managers\SystemManager.h"

#include "systems\Clock.h"
#include "systems\Graphics.h"
#include "systems\Logic.h"
#include "systems\Window.h"

#include <core\resources\MeshDataResource.h>
#include <core\resources\ShaderResource.h>

#include "AbstractGame.h"

#endif // !_ENGINECORE_H
