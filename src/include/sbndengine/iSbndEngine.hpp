/*
 * Copyright 2010 Martin Schreiber
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * this header file is intended to be the only included one by
 * the application developer
 */

#ifndef __I_SBND_ENGINE_HPP__
#define __I_SBND_ENGINE_HPP__

#include "iApplication.hpp"
#include "iBase.hpp"
#include "iRef.hpp"
#include "iTime.hpp"
#include "iWindow.hpp"
#include "iText.hpp"
#include "camera/iCamera.hpp"
#include "camera/cCameraLookAt.hpp"
#include "camera/cCamera1stPerson.hpp"
#include "camera/cCamera3rdPerson.hpp"
#include "graphics/iDraw3D.hpp"
#include "engine/iEngine.hpp"
#include "engine/iObject.hpp"
#include "engine/iObjectFactory.hpp"
#include "engine/cObjectFactoryBox.hpp"
#include "engine/cObjectFactoryPlane.hpp"
#include "engine/cObjectFactorySphere.hpp"
#include "engine/iObjectRayIntersection.hpp"
#include "graphics/iGraphics.hpp"
#include "graphics/iTexture.hpp"
#include "graphics/iGraphicsObject.hpp"
#include "graphics/iGraphicsObjectConnector.hpp"
#include "graphics/cGraphicsObjectConnectorCenter.hpp"
#include "graphics/cGraphicsObjectConnectorAngular.hpp"
#include "graphics/iGraphicsMaterial.hpp"
#include "physics/iPhysics.hpp"
#include "physics/iPhysicsObject.hpp"
#include "physics/iPhysicsSoftConstraint.hpp"
#include "physics/cPhysicsSoftConstraintSpring.hpp"
#include "physics/cPhysicsSoftConstraintSpringAngular.hpp"
#include "physics/iPhysicsHardConstraint.hpp"
#include "physics/cPhysicsHardConstraintRope.hpp"
#include "physics/cPhysicsHardConstraintRopeAngular.hpp"

#endif // __I_SBND_ENGINE_HPP__
