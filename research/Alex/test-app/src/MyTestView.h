/*
 * Copyright (C) 2004-2010 Geometer Plus <contact@geometerplus.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#include <ZLTextView.h>
#include <ZLTextPositionIndicatorInfo.h>

#include "FBTextStyle.h"


class MyTestView : public ZLTextView {
public:
	MyTestView(ZLPaintContext &context) 
		: ZLTextView(context)
		, myCaption("test")
		 {}

	virtual ~MyTestView() {}

	virtual shared_ptr<ZLTextPositionIndicatorInfo> indicatorInfo() const { return NULL; }

	virtual int doubleClickDelay() const { return 10; }

	virtual int leftMargin() const { return 10; }
	virtual int rightMargin() const { return 10; }
	virtual int topMargin() const { return 10; }
	virtual int bottomMargin() const { return 10; }

	virtual const std::string &caption() const { return myCaption; } 

	virtual ZLColor backgroundColor() const { return ZLColor(0, 0, 0); }


	virtual shared_ptr<ZLTextStyle> baseStyle() const { return FBTextStyle::InstanceAsPtr(); }
	virtual ZLColor color(const std::string &style = std::string()) const { return ZLColor(255, 255, 255); }
	virtual bool isSelectionEnabled() const { return false; }

private:
	std::string myCaption;
};