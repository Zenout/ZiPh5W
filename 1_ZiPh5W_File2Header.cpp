//-----1_ZiPh5W_File2Header.cpp-----//

/*
 * ZiPh5W - Zenouts iPhone 5 Workaround (1: converter)
 * (C)2012 Jez 'Zenout' Hammond
 * www.jezhammond.com | www.zenout.co.uk
 */

// !! note only the header file created in your data-ram folder should be used in your project !!

//----------//
#include "s3e.h"
#include "stdio.h"

//----------//
const char*						g_inputFilename = "originalFile.png";
const char*						g_outputFilename = "convertedData.h";

#define							MAX_TEXT_BUFFER					255
char							g_textBuffer[MAX_TEXT_BUFFER+1];

enum { no_error, fileDoesntExist_error, lowMemory_error, readInputFile_error, writeOutputFile_error, };

//----------//
int main(void)
{
	// Check original file exists.
	if( !s3eFileCheckExists(g_inputFilename) )
	{
		// Error, not found.
		sprintf(g_textBuffer, "Error: File ''%s'' doesn't exist.", g_inputFilename);
		s3eDebugErrorShow(S3E_MESSAGE_CONTINUE, g_textBuffer);
		return fileDoesntExist_error;
	}
	// Open the input file for reading.
	s3eFile* inputFileHandle = s3eFileOpen(g_inputFilename, "rb");
	// Get input file's size.
	const int inputFileSize = s3eFileGetSize(inputFileHandle);

	// Allocate memory for the input data.
	char* data = new char[inputFileSize];
	if(!data)
	{
		// Error, not enough memory.
		sprintf(g_textBuffer, "Error: Not enough memory (%d requested).", inputFileSize);
		s3eDebugErrorShow(S3E_MESSAGE_CONTINUE, g_textBuffer);
		s3eFileClose(inputFileHandle);
		return lowMemory_error;
	}

	// Read all of the input data to the buffer.
	if( s3eFileRead(data, inputFileSize, 1, inputFileHandle) != 1 )
	{
		//
		s3eFileGetError();
		s3eDebugErrorShow( S3E_MESSAGE_CONTINUE, s3eFileGetErrorString() );
		s3eFileClose(inputFileHandle);
		delete [] data;
		return readInputFile_error;
	}
	// Close the input file.
	s3eFileClose(inputFileHandle);

	// Open the output file for writing. [note that the output file (.h) will save to your data-ram folder in simulator, for use in *your* project]
	s3eFile* outputFileHandle = s3eFileOpen(g_outputFilename, "wb");

	// Loop through the data. Converting to 8bit (x2 = 16bit) ascii hex values.
	const char* hex[] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F" };
	char* pData = data;
	for(int i=0; i<inputFileSize; ++i)
	{
		// As the initial conversion is ran offline just the once, a slow byte-by-byte method is sufficient.
		const char c = *pData++;
		g_textBuffer[0] = *hex[c>>4];
		g_textBuffer[1] = *hex[c&0x0F];

		if( s3eFileWrite(g_textBuffer, 2, 1, outputFileHandle) != 1 )
		{
			// Error writing data.
			s3eFileGetError();
			s3eDebugErrorShow( S3E_MESSAGE_CONTINUE, s3eFileGetErrorString() );
			s3eFileClose(outputFileHandle);
			delete [] data;
			return writeOutputFile_error;
		}
	}

	// Delete the input data.
	delete [] data;

	// Close the output file.
	s3eFileClose(outputFileHandle);

	//
	sprintf(g_textBuffer, "Done OK.\n\nCreated header file ''data-ram/%s'', have a nice day!", g_outputFilename);
	s3eDebugErrorShow(S3E_MESSAGE_CONTINUE, g_textBuffer);

	//
	return 0;
}

//----------//
