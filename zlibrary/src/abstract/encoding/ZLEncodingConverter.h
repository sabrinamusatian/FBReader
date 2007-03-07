/*
 * Copyright (C) 2004-2007 Nikolay Pultsin <geometer@mawhrin.net>
 * Copyright (C) 2005 Mikhail Sobolev <mss@mawhrin.net>
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

#ifndef __ZLENCODINGCONVERTER_H__
#define __ZLENCODINGCONVERTER_H__

#include <string>
#include <vector>
#include <map>

#include <shared_ptr.h>

class ZLEncodingConverter {

protected:
	ZLEncodingConverter();

public:
	virtual ~ZLEncodingConverter();
	virtual void convert(std::string &dst, const char *srcStart, const char *srcEnd) = 0;
	void convert(std::string &dst, const std::string &src);
	virtual void reset();
	virtual bool fillTable(int *map) = 0;

private:
	ZLEncodingConverter(const ZLEncodingConverter&);
	ZLEncodingConverter &operator=(const ZLEncodingConverter&);
};

class ZLEncodingConverterInfo {

public:
	ZLEncodingConverterInfo(const std::string &name, const std::string &region);
	void addAlias(const std::string &alias);

	const std::string &name() const;
	const std::string &visibleName() const;
	shared_ptr<ZLEncodingConverter> createConverter() const;
	bool canCreateConverter() const;

private:
	const std::string myName;
	const std::string myVisibleName;
	std::vector<std::string> myAliases;

private:
	ZLEncodingConverterInfo(const ZLEncodingConverterInfo&);
	ZLEncodingConverterInfo &operator=(const ZLEncodingConverterInfo&);
};

typedef shared_ptr<ZLEncodingConverterInfo> ZLEncodingConverterInfoPtr;

class ZLEncodingSet {

public:
	ZLEncodingSet(const std::string &name);
	void addInfo(ZLEncodingConverterInfoPtr info);

	const std::string &name() const;
	const std::vector<ZLEncodingConverterInfoPtr> &infos() const;

private:
	const std::string myName;
	std::vector<ZLEncodingConverterInfoPtr> myInfos;

private:
	ZLEncodingSet(const ZLEncodingSet&);
	ZLEncodingSet &operator=(const ZLEncodingSet&);
};

class ZLEncodingCollection {

public:
	static const std::vector<shared_ptr<ZLEncodingSet> > &sets();
	static ZLEncodingConverterInfoPtr info(const std::string &name);
	static ZLEncodingConverterInfoPtr info(int code);
	static ZLEncodingConverterInfoPtr defaultInfo();

private:
	static void init();
	static std::string encodingDescriptionPath();
	static void addInfo(ZLEncodingConverterInfoPtr info);

private:
	static std::vector<shared_ptr<ZLEncodingSet> > ourSets;
	static std::map<std::string,ZLEncodingConverterInfoPtr> ourInfosByName;

private:
	ZLEncodingCollection();

friend class ZLEncodingConverterInfo;
friend class EncodingReader;
friend class EncodingCollectionReader;
};

#endif /* __ZLENCODINGCONVERTER_H__ */
