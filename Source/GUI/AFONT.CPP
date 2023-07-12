//===========================================================================//
// Copyright (C) Microsoft Corporation. All rights reserved.                 //
//===========================================================================//

#include "afont.h"
#include "mclib.h"
#include <mbstring.h>

aFont::aFont()
{
	gosFont = 0;
	fontName[0] = 0;
	resID = 0;
	size = 1;
}

long aFont::init( const char* newFontName )
{
	destroy();
	gosFont = 0;

	strcpy( fontName, newFontName );
	char path[256];

	char* pStr = strstr( fontName, "," );
	if ( pStr )
	{
		size = atoi( pStr + 1 );
	}


	strcpy( path, "assets\\graphics\\" );
	strcat( path, fontName );
	_strlwr( path );
	gosFont = gos_LoadFont( path );

	if ( gosFont )
		return 0;

	return FONT_NOT_LOADED;

}

long aFont::init( long resourceID )
{
	destroy();
	gosFont = 0;
	gosFont = loadFont( resourceID, size );
	resID = resourceID;

	if ( gosFont )
		return 0;

	return FONT_NOT_LOADED;
}


aFont::~aFont()
{
	destroy();
}



unsigned long aFont::height( ) const
{
	unsigned long width, height;
	gos_TextSetAttributes (gosFont, 0, size, false, true, false, false);
	gos_TextStringLength(&width,&height,"ABCDE");
	return height;
}

void	aFont::getSize( unsigned long& width, unsigned long& height, const char* pText )
{
	gos_TextSetAttributes (gosFont, 0, size, false, true, false, false);
	gos_TextStringLength(&width,&height,pText);

}


unsigned long aFont::height( const char* st, int areaWidth ) const
{
	unsigned long width, height;
	gos_TextSetAttributes (gosFont, 0, size, false, true, false, false);
	gos_TextSetRegion( 0, 0, areaWidth, Environment.screenHeight );
	gos_TextStringLength(&width,&height,st);

	unsigned long lineCount = 1;

	if ( width > areaWidth - 1 )
	{
		unsigned long curLineWidth = 0;

		gosASSERT( strlen( st) < 2048 );

		
		char pLine[2048]; // should be more than adequate

		char* pLastWord = (char*)st;
		char* pTmp = (char*)st;
		char* pTmpLine = (char*)pLine;

		int numberOfWordsPerLine = 0;


		bool bHasSpaces = true;
		if (  !strstr( st, " " ) )
		{
			if ( !strstr( st, "\n" ) )
				bHasSpaces = false;
		}

		while( *pTmp != NULL )
		{
			if ( *pTmp == '\n' )
			{
				lineCount++;
				numberOfWordsPerLine = 0;
				curLineWidth = 0;	
				pTmpLine = pLine;
				pLastWord = pTmp+1;
			}
			else if ( !bHasSpaces )
			{
				if ( pTmp > st )
				{
					char tmp = *(pTmp-1);
					if ( !isleadbyte( tmp ) )
					{
						*(pTmpLine) = NULL;
						gos_TextStringLength( &curLineWidth, &height, pLine );
						if ( curLineWidth > areaWidth )
						{
							lineCount++;
							pTmp--;
							pTmpLine = pLine;
							curLineWidth = 0;
							numberOfWordsPerLine = 0;

						}

					}
					
				
				}
			}

			else if ( isspace( *pTmp ) )
			{
				*(pTmpLine) = NULL;
				gos_TextStringLength( &curLineWidth, &height, pLine );
				if ( curLineWidth > areaWidth )
				{
					gos_TextStringLength( &curLineWidth, &height, pLastWord );
					if ( numberOfWordsPerLine == 0 || curLineWidth > areaWidth )
					{
						static bool firstTime = true;
						if (firstTime)
						{
							Assert( true, 0, "this list box item contains a word of greater "
									" area than the list box, giving up" );
							firstTime = false;
						}
						/*There are times when you just can't guarantee that this won't occur,
						so we have to just continue and deal with it.*/
						//return height;

						pLastWord = pTmp;
					}
					lineCount++;
					pTmpLine = pLine;
					pTmp = pLastWord - 1;
					curLineWidth = 0;
					numberOfWordsPerLine = 0;

				}
		
				pLastWord = pTmp;
				numberOfWordsPerLine++;
			}


			*pTmpLine = *pTmp;
			if ( isleadbyte( *pTmpLine ) )
			{
				*(pTmpLine+1) = *(pTmp+1);
			}

			pTmpLine = (char*)_mbsinc( (unsigned char*)pTmpLine );
			pTmp = (char*)_mbsinc( (unsigned char*)pTmp );
		}

		// one last check
		*pTmpLine = NULL;
		gos_TextStringLength( &curLineWidth, &height, pLine );
		if ( curLineWidth > areaWidth )
		{
			lineCount++;
		}

		if ( *pTmp == NULL )
			lineCount++;	
	}

	gos_TextStringLength( &width, &height, "A" );

	return (height) * lineCount ;
}

unsigned long aFont::width( const char* st ) const
{
	unsigned long width, height;
	gos_TextSetAttributes (gosFont, 0, size, false, true, false, false);
	gos_TextStringLength(&width,&height,st);
	return width;
}

long aFont::load( const char* fontName )
{
	destroy();

	return init( fontName );
}

void aFont::destroy()
{
	if ( gosFont )
		gos_DeleteFont( gosFont );

	gosFont = 0;
	resID = 0;
	fontName[0] = 0;

}

void aFont::render( const char* text, int xPos, int yPos, int areaWidth, int areaHeight, 
				   unsigned long color, bool bBold, int alignment )
{
	gos_TextSetAttributes( gosFont, color, size, true, true, bBold, false, alignment );
	
	if ( areaWidth < 1 )
	{
		if ( alignment == 1 )
		{
			unsigned long width, height;
			gos_TextStringLength( &width, &height, text );
			xPos -= width;
			areaWidth = width + 1;
		}
		else
			areaWidth = Environment.screenWidth;
	}

	if ( areaHeight < 1 )
	{
		if ( alignment == 3  ) // bottom
		{
			unsigned long width, height;
			gos_TextStringLength( &width, &height, text );
			yPos -= height;
			areaHeight = height + 1;
		}
		
		else
			areaHeight = Environment.screenHeight;
	}

	gos_TextSetRegion( xPos, yPos, xPos + areaWidth, yPos + areaHeight );
	
	gos_TextSetPosition( xPos, yPos );
	gos_TextDraw( text );	

}

HGOSFONT3D aFont::loadFont( long resourceID, long& size )
{
	size = 1;
	char buffer[256];
	cLoadString( resourceID, buffer, 255 );

	char* pStr = strstr( buffer, "," );
	if ( pStr )
	{
		size = -atoi( pStr + 1 );
		*pStr = NULL;
	}

	char path[256];
	strcpy( path, "assets\\graphics\\" );
	strcat( path, buffer );
	_strlwr( path );
	HGOSFONT3D retFont = gos_LoadFont( path );

	return retFont;
}

aFont::aFont( const aFont& src )
{
	copyData(src);
}

aFont& aFont::operator=( const aFont& src )
{
	copyData(src);
	return *this;
}

void aFont::copyData( const aFont& src )
{
	if ( &src != this )
	{
		if (src.resID )
		{
			init( src.resID );
		}
		else if ( src.fontName[0] != 0 )
		{
			init( src.fontName );
		}

		size = src.size;
	}
}
