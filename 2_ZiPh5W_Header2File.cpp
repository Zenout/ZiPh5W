//-----2_ZiPh5W_Header2File.cpp-----//

/*
 * ZiPh5W - Zenouts iPhone 5 Workaround - (2: deconverter)
 * (C)2012 Jez 'Zenout' Hammond
 * www.jezhammond.com | www.zenout.co.uk
 *
 * ********** [Use at your own risk! This design is to bypass iOS6 automated AppStore rejection when app is built with iOS 5 -> aka Marmalade v6.1.x] **********
 *
 * This demonstrates how to convert the header data* back to a file.
 * [data created with "1_ZiPh5W_File2Header.cpp"]
 *
 * This function is very fast and should be ran once.
 * [just check if the file exists first]
 *
 * The purpose of this code is to create an iPhone 5 compatible splash screen from our header data.
 * 'Apple' have refused us to just include the appropriate splashscreen, by automated process...
 * This is my (Jez Hammond) workaround for including/producing the appropriate asset to trigger the iPhone-
 * 5's true potential (640x1136 / 1136x640 pix).
 *
 * I created this code because it is a win-win situation for everyone:
 *   Full screen iPhone 5 experience for customer (once the file is dumped -> do *not* use a path in your filename).
 *   No further delays for updating your apps to support true widescreen on the iPhone 5.
 *   No need to include the 'offending' asset as a file -> else you could just rename it later - but that might not get past Cecil...
 *      *Cecil is my new pet name for the server at the other end of 'Application Loader' (OS X).
 *       if I made Cecil, she would not have a problem with creative stuff that benefits the greater good.
 *
 * ALSO: Yesterday Apple 'deleted' our [individual developers] company names :(
 *       so, today You might search the AppStore for "Zenout" and You get NO results. None.
 *       ...but they can't take our names! ["Company 1234567890.4 has Products 1-3 available: Please select or return to ../../../../../Categories."]
 *       
 * GOOD LUCK submitting your iPhone 5 ready apps. And *remember*, the 1st launch only will still have borders (probably until app is force-exited??)
 *
 * Jez Hammond, on behalf of all iOS bedroom programmers.
 * peace
 */

//----------//
#include "s3e.h"
#include "stdio.h"
#include "string.h"

#include "2_ZiPh5W_formattedData.h"

//----------//
#define				converted_back_filename				"Default-568h@2x.png"

#define				MAX_TEXT_BUFFER					255
char				g_textBuffer[MAX_TEXT_BUFFER+1];

enum { no_error, lowMemory_error, writeFile_error, };

//----------//
int main(void)
{
	// Allocate a chunk of memory for a decode buffer.
	const int decodeBufferSize = 16 * 1024;			// 16KB -> this is the max chunk size per string (compiler allows 8192 chars -> we use 2 per ascii-hex value 'XX')
	char* data = new char[decodeBufferSize];
	if(!data)
	{
		// Error, not enough memory.
		sprintf( g_textBuffer, "Error: Not enough memory (%.2fMB requested).", (float)decodeBufferSize / (1<<20) );
		s3eDebugErrorShow(S3E_MESSAGE_CONTINUE, g_textBuffer);
		return lowMemory_error;
	}

	// Open a new file for the decoded file.
	s3eFile* decodedFileHandle = s3eFileOpen(converted_back_filename, "wb");

	// Loop through the entries.
	for(int i=0; i<num_ascii_strings; ++i)
	{
		const int numAsciiEntriesThisChunk = strlen(g_data[i]) >> 1;

		char* pDataIn = (char*)g_data[i];
		char* pDataOut = data;
		for(int j=0; j<numAsciiEntriesThisChunk; ++j)
		{
			//
			const char in1 = *pDataIn++;
			const char in2 = *pDataIn++;
			*pDataOut++ = (in1 < 'A' ? (in1 - '0') << 4 : (in1 - 'A' + 0x0A) << 4) | (in2 < 'A' ? in2 - '0' : in2 - 'A' + 0x0A);		// :)
		}

		//
		if( s3eFileWrite(data, numAsciiEntriesThisChunk, 1, decodedFileHandle) != 1 )
		{
			// Error writing data.
			s3eFileGetError();
			s3eDebugErrorShow( S3E_MESSAGE_CONTINUE, s3eFileGetErrorString() );
			s3eFileClose(decodedFileHandle);
			delete [] data;
			return writeFile_error;
		}
	}

	//
	s3eFileClose(decodedFileHandle);
	delete [] data;

	//
	s3eDebugErrorShow(S3E_MESSAGE_CONTINUE, "\nYour file has probably been reconstructed.");

	//
	s3eAudioPlay("why.mp3", 1);		// ;)
	//
	s3eDebugErrorShow(S3E_MESSAGE_CONTINUE_STOP_IGNORE, "\n<Ok.>");

	//
	return 0;
}

//----------//
