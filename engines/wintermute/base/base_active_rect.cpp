/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/*
 * This file is based on WME Lite.
 * http://dead-code.org/redir.php?target=wmelite
 * Copyright (c) 2011 Jan Nedoma
 */

#include "engines/wintermute/base/base_active_rect.h"
#include "engines/wintermute/base/base_game.h"
#include "engines/wintermute/base/base_engine.h"
#include "engines/wintermute/base/base_region.h"
#include "engines/wintermute/base/gfx/base_renderer.h"
#include "engines/wintermute/platform_osystem.h"

namespace Wintermute {

//////////////////////////////////////////////////////////////////////
BaseActiveRect::BaseActiveRect(BaseGame *inGame) : BaseClass(inGame) {
	_rect.setEmpty();
	_owner = nullptr;
	_frame = nullptr;
#ifdef ENABLE_WME3D
	_modelX = nullptr;
#endif
	_region = nullptr;
	_zoomX = 100;
	_zoomY = 100;
	_offsetX = _offsetY = 0;
	clipRect();
}


//////////////////////////////////////////////////////////////////////
BaseActiveRect::BaseActiveRect(BaseGame *inGame, BaseObject *owner, BaseSubFrame *frame, int x, int y, int width, int height, float zoomX, float zoomY, bool precise) : BaseClass(inGame) {
	_owner = owner;
	_frame = frame;
	_rect.setRect(x, y, x + width, y + height);
	_zoomX = zoomX;
	_zoomY = zoomY;
	_precise = precise;
#ifdef ENABLE_WME3D
	_modelX = nullptr;
#endif
	_region = nullptr;
	_offsetX = _offsetY = 0;
	clipRect();
}

//////////////////////////////////////////////////////////////////////
#ifdef ENABLE_WME3D
BaseActiveRect::BaseActiveRect(BaseGame *inGame, BaseObject *owner, ModelX *model, int x, int y, int width, int height, bool precise) : BaseClass(inGame) {
	_owner = owner;
	_modelX = model;
	_rect.setRect(x, y, x + width, y + height);
	_zoomX = 100;
	_zoomY = 100;
	_precise = precise;
	_frame = nullptr;
	_region = nullptr;
	_offsetX = _offsetY = 0;
	clipRect();
}
#endif

//////////////////////////////////////////////////////////////////////
BaseActiveRect::BaseActiveRect(BaseGame *inGame, BaseObject *owner, BaseRegion *region, int offsetX, int offsetY) : BaseClass(inGame) {
	_owner = owner;
	_region = region;
	BasePlatform::copyRect(&_rect, &region->_rect);
	_rect.offsetRect(-offsetX, -offsetY);
	_zoomX = 100;
	_zoomY = 100;
	_precise = true;
	_frame = nullptr;
#ifdef ENABLE_WME3D
	_modelX = nullptr;
#endif
	clipRect();
	_offsetX = offsetX;
	_offsetY = offsetY;
}

//////////////////////////////////////////////////////////////////////
BaseActiveRect::~BaseActiveRect() {
	_owner = nullptr;
	_frame = nullptr;
#ifdef ENABLE_WME3D
	_modelX = nullptr;
#endif
	_region = nullptr;
}


//////////////////////////////////////////////////////////////////////////
void BaseActiveRect::clipRect() {
	Rect32 rc;
	bool customViewport;
	_gameRef->getCurrentViewportRect(&rc, &customViewport);
	BaseRenderer *Rend = BaseEngine::getRenderer();

	if (!customViewport) {
		rc.left -= Rend->_drawOffsetX;
		rc.right -= Rend->_drawOffsetX;
		rc.top -= Rend->_drawOffsetY;
		rc.bottom -= Rend->_drawOffsetY;
	}

	if (rc.left > _rect.left) {
		_offsetX = rc.left - _rect.left;
	}
	if (rc.top  > _rect.top) {
		_offsetY = rc.top  - _rect.top;
	}

	BasePlatform::intersectRect(&_rect, &_rect, &rc);
}

} // End of namespace Wintermute
