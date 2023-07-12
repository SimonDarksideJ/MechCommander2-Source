//***************************************************************************
//
//	Debugging.cpp -- File contains the Game Debugging code
//
//	MechCommander 2
//
//---------------------------------------------------------------------------//
// Copyright (C) Microsoft Corporation. All rights reserved.                 //
//===========================================================================//

#ifndef DEBUGGING_H
#include "debugging.h"
#endif

#include <stdio.h>
#include <string.h>
#include <GameOS\GameOS.hpp>
#include <GameOS\ToolOS.hpp>

HGOSFONT3D GameDebugWindow::font = NULL;
long GameDebugWindow::fontHeight = 0;

//***************************************************************************
//	GAME DEBUG WINDOW class
//***************************************************************************

void GameDebugWindow::setFont (char* fontFile) {

	if (font) {
		gos_DeleteFont(font);
		font = NULL;
	}
	if (fontFile) {
		font = gos_LoadFont(fontFile);
		gos_TextSetAttributes(font, 0xffffffff, 1.0, true, true, false, false);
	}
	DWORD height, width;
	gos_TextStringLength(&width, &height, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
	fontHeight = height;
}

//---------------------------------------------------------------------------

void GameDebugWindow::print (char* s) {
	
	if (numLines < MAX_DEBUG_WINDOW_LINES)
		strcpy(textBuffer[numLines++], s);
	else {
		numLines++;
		strcpy(textBuffer[linePos++], s);
		if (linePos == MAX_DEBUG_WINDOW_LINES)
			linePos = 0;
	}
}

//---------------------------------------------------------------------------

void GameDebugWindow::render (void) {

	if (!display)
		return;

	long curY = pos[1] + 5;
	for (long i = linePos; i < MAX_DEBUG_WINDOW_LINES; i++) {
		gos_TextSetPosition(pos[0] + 5, curY);
		curY += fontHeight;
		gos_TextDraw(textBuffer[i]);
	}
	for (i = 0; i < linePos; i++) {
		gos_TextSetPosition(pos[0] + 5, curY);
		curY += fontHeight;
		gos_TextDraw(textBuffer[i]);
	}
}

//***************************************************************************
