/*
 * FBReader -- electronic book reader
 * Copyright (C) 2005 Nikolay Pultsin <geometer@mawhrin.net>
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
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef __PARAGRAPHCURSOR_H__
#define __PARAGRAPHCURSOR_H__

#include <vector>
#include <map>
#include <string>

#include <abstract/shared_ptr.h>
#include <abstract/allocator.h>

#include "../model/TextModel.h"
#include "Word.h"

class TextElement;
class Paragraph;

class TextElementVector : public std::vector<TextElement*> {

public:
	TextElementVector() VIEW_SECTION;
	~TextElementVector() VIEW_SECTION;
};

class TextElementPool {

public:
	static TextElementPool Pool;

public:
	TextElementPool() VIEW_SECTION;
	~TextElementPool() VIEW_SECTION;
	
public:
	TextElement *HSpaceElement;
	TextElement *BeforeParagraphElement;
	TextElement *AfterParagraphElement;
	TextElement *IndentElement;
	TextElement *EmptyLineElement;

	Word *getWord(const char *data, unsigned short length, size_t paragraphOffset) VIEW_SECTION;
	void storeWord(Word *word) VIEW_SECTION;
	ControlElement *getControlElement(const ControlEntry &entry) VIEW_SECTION;
	void storeControlElement(ControlElement *element) VIEW_SECTION;

private:
	Allocator<sizeof(Word),64> myWordAllocator;
	Allocator<sizeof(ControlElement),32> myControlAllocator;
};

class WordCursor {

private:
	WordCursor() VIEW_SECTION;
	WordCursor(const shared_ptr<TextElementVector> elements, const TextElementVector::const_iterator &wordIterator) VIEW_SECTION;

public:
	~WordCursor() VIEW_SECTION;
	bool sameElementAs(const WordCursor &cursor) const VIEW_SECTION;
	TextElement &element() const VIEW_SECTION;
	void nextWord() VIEW_SECTION;
	void previousWord() VIEW_SECTION;
	void setCharNumber(int charNumber) VIEW_SECTION;

	int wordNumber() const VIEW_SECTION;
	int charNumber() const VIEW_SECTION;

	bool isStartOfParagraph() const VIEW_SECTION;
	bool isEndOfParagraph() const VIEW_SECTION;

private:
	shared_ptr<TextElementVector> myElements;
	TextElementVector::const_iterator myWordIterator;
	int myCharNumber;

friend class ParagraphCursor;
friend class FullCursor;
};

class ParagraphCursor {

public:
	class Cache {

	public:
		Cache() VIEW_SECTION;
		~Cache() VIEW_SECTION;

	private:
		static std::map<Paragraph*,shared_ptr<TextElementVector> > ourCache;
		static int ourCacheCounter;

	friend class ParagraphCursor;
	};
	
private:
	class ParagraphProcessor {

	public:
		ParagraphProcessor(const Paragraph &paragraph, const std::vector<TextMark> &marks, int paragraphNumber, const shared_ptr<TextElementVector> elements) VIEW_SECTION;
		~ParagraphProcessor() VIEW_SECTION;

		void fill() VIEW_SECTION;

	private:
		void beforeAddWord() VIEW_SECTION;
		void addWord(const char *ptr, int offset, int len) VIEW_SECTION;

	private:
		const Paragraph &myParagraph;
		shared_ptr<TextElementVector> myElements;

		std::vector<TextMark>::const_iterator myFirstMark;
		std::vector<TextMark>::const_iterator myLastMark;
		int myWordCounter;
		int myOffset;
	};

protected:
	ParagraphCursor(const TextModel &model) VIEW_SECTION;
	ParagraphCursor(const ParagraphCursor &cursor) VIEW_SECTION;

public:
	static ParagraphCursor *createCursor(const TextModel &model) VIEW_SECTION;
	virtual ParagraphCursor *createCopy() const VIEW_SECTION = 0;
	virtual ~ParagraphCursor() VIEW_SECTION;

	virtual bool previous() VIEW_SECTION = 0;
	virtual bool next() VIEW_SECTION = 0;

	bool isFirst() const VIEW_SECTION;
	virtual bool isLast() const VIEW_SECTION = 0;
	bool isEndOfSection() const VIEW_SECTION;

	int paragraphLength() const VIEW_SECTION;
	int paragraphNumber() const VIEW_SECTION;
	TextMark position(const WordCursor &word) const VIEW_SECTION;
	void moveTo(int paragraphNumber) VIEW_SECTION;

	WordCursor wordCursor() const VIEW_SECTION;
	WordCursor wordCursor(int wordNumber, int charNumber) const VIEW_SECTION;
	void setWordCursor(const WordCursor cursor) VIEW_SECTION;
	const WordCursor begin() const VIEW_SECTION;
	const WordCursor end() const VIEW_SECTION;

	void rebuild() VIEW_SECTION;

private:
	void processControlParagraph(const Paragraph &paragraph) VIEW_SECTION;

protected:
	void fill() VIEW_SECTION;
	void clear() VIEW_SECTION;

private:
	/* assignment is disabled */
	ParagraphCursor &operator = (const ParagraphCursor &) VIEW_SECTION;
	
protected:
	const TextModel &myModel;
	std::vector<Paragraph*>::const_iterator myParagraphIterator;
	shared_ptr<TextElementVector> myElements;
};

class FullCursor {

public:
	FullCursor() VIEW_SECTION;
	~FullCursor() VIEW_SECTION;

	const FullCursor &operator = (ParagraphCursor *paragraphCursor) VIEW_SECTION;

	void moveTo(int paragraphNumber) VIEW_SECTION;
	void moveToParagraphStart() VIEW_SECTION;
	void moveToParagraphEnd() VIEW_SECTION;
	void moveWordCursorTo(int wordNumber, int charNumber) VIEW_SECTION;
	void setWordCursor(const WordCursor &word) VIEW_SECTION;
	void rebuild() VIEW_SECTION;
	bool next() VIEW_SECTION;
	bool previous() VIEW_SECTION;

	bool isNull() const VIEW_SECTION;
	const ParagraphCursor &paragraphCursor() const VIEW_SECTION;
	const WordCursor &wordCursor() const VIEW_SECTION;
	TextMark position() const VIEW_SECTION;
	
private:
	shared_ptr<ParagraphCursor> myParagraphCursor;
	WordCursor myWordCursor;
};

class PlainTextParagraphCursor : public ParagraphCursor {

private:
	PlainTextParagraphCursor(const PlainTextParagraphCursor &cursor) VIEW_SECTION;

public:
	PlainTextParagraphCursor(const TextModel &model) VIEW_SECTION;
	~PlainTextParagraphCursor() VIEW_SECTION;
	ParagraphCursor *createCopy() const VIEW_SECTION;

	bool previous() VIEW_SECTION;
	bool next() VIEW_SECTION;
	bool isLast() const VIEW_SECTION;
};

class TreeParagraphCursor : public ParagraphCursor {

private:
	TreeParagraphCursor(const TreeParagraphCursor &cursor) VIEW_SECTION;

public:
	TreeParagraphCursor(const TreeModel &model) VIEW_SECTION;
	~TreeParagraphCursor() VIEW_SECTION;
	ParagraphCursor *createCopy() const VIEW_SECTION;

	bool previous() VIEW_SECTION;
	bool next() VIEW_SECTION;
	bool isLast() const VIEW_SECTION;
};

inline TextElementVector::TextElementVector() {}

inline Word *TextElementPool::getWord(const char *data, unsigned short length, size_t paragraphOffset) {
	return new (myWordAllocator.allocate()) Word(data, length, paragraphOffset);
}
inline void TextElementPool::storeWord(Word *word) {
	word->~Word();
	myWordAllocator.free((void*)word);
}
inline ControlElement *TextElementPool::getControlElement(const ControlEntry &entry) {
	return new (myControlAllocator.allocate()) ControlElement(entry);
}
inline void TextElementPool::storeControlElement(ControlElement *element) {
	element->~ControlElement();
	myControlAllocator.free((void*)element);
}

inline WordCursor::WordCursor() { myCharNumber = 0; }
inline WordCursor::WordCursor(const shared_ptr<TextElementVector> elements, const TextElementVector::const_iterator &wordIterator) : myElements(elements), myWordIterator(wordIterator) { myCharNumber = 0; }
inline WordCursor::~WordCursor() {}
inline bool WordCursor::sameElementAs(const WordCursor &cursor) const {
	return myWordIterator == cursor.myWordIterator;
}
inline TextElement &WordCursor::element() const { return **myWordIterator; }
inline void WordCursor::nextWord() { myWordIterator++; myCharNumber = 0; }
inline void WordCursor::previousWord() { myWordIterator--; myCharNumber = 0; }
inline int WordCursor::wordNumber() const { return myWordIterator - myElements->begin(); }
inline int WordCursor::charNumber() const { return myCharNumber; }
inline bool WordCursor::isStartOfParagraph() const {
	return (myWordIterator == myElements->begin()) && (myCharNumber == 0);
}
inline bool WordCursor::isEndOfParagraph() const { return myWordIterator == myElements->end(); }

inline ParagraphCursor::Cache::Cache() { ourCacheCounter++; }
inline ParagraphCursor::Cache::~Cache() {
	ourCacheCounter--;
	if (ourCacheCounter == 0) {
		ourCache.clear();
	}
}

inline int ParagraphCursor::paragraphLength() const { return myElements->size(); }
inline int ParagraphCursor::paragraphNumber() const { return myParagraphIterator - myModel.paragraphs().begin(); }
inline const WordCursor ParagraphCursor::begin() const { return WordCursor(myElements, myElements->begin()); }
inline const WordCursor ParagraphCursor::end() const { return WordCursor(myElements, myElements->end()); }

inline PlainTextParagraphCursor::PlainTextParagraphCursor(const PlainTextParagraphCursor &cursor) : ParagraphCursor(cursor) {}
inline PlainTextParagraphCursor::PlainTextParagraphCursor(const TextModel &model) : ParagraphCursor(model) {}
inline PlainTextParagraphCursor::~PlainTextParagraphCursor() {}
inline ParagraphCursor *PlainTextParagraphCursor::createCopy() const { return new PlainTextParagraphCursor(*this); }

inline TreeParagraphCursor::TreeParagraphCursor(const TreeParagraphCursor &cursor) : ParagraphCursor(cursor) {}
inline TreeParagraphCursor::TreeParagraphCursor(const TreeModel &model) : ParagraphCursor(model) {}
inline TreeParagraphCursor::~TreeParagraphCursor() {}
inline ParagraphCursor *TreeParagraphCursor::createCopy() const { return new TreeParagraphCursor(*this); }

inline FullCursor::FullCursor() {}
inline FullCursor::~FullCursor() {}

inline void FullCursor::moveToParagraphStart() { if (!isNull()) myWordCursor = myParagraphCursor->begin(); }
inline void FullCursor::moveToParagraphEnd() { if (!isNull()) myWordCursor = myParagraphCursor->end(); }
inline void FullCursor::setWordCursor(const WordCursor &word) { myWordCursor = word; }

inline bool FullCursor::isNull() const { return myParagraphCursor.isNull(); }
inline const ParagraphCursor &FullCursor::paragraphCursor() const { return *myParagraphCursor; }
inline const WordCursor &FullCursor::wordCursor() const { return myWordCursor; }
inline TextMark FullCursor::position() const { return myParagraphCursor->position(myWordCursor); }

#endif /* __PARAGRAPHCURSOR_H__ */
