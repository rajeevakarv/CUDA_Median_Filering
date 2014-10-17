/************************************************************************/
// The purpose of this program is to perform Median Filtering on the GPU
// and CPU.
//
// Author: Rajeev Verma
// Course: 0306-724 - High Performance Architectures
//
// File: Bitmap.h
//
// This file was provided with the lab exercise.
/************************************************************************/

#ifndef BITMAP_H
#define BITMAP_H

#include <iostream>
#include <fstream>

typedef __int32 int32_t;
typedef unsigned __int32 uint32_t;
typedef unsigned __int16 uint16_t;

class Bitmap {
public:
	Bitmap() : image( NULL ) {}
	
	Bitmap( int w, int h ) {
		image = new char[ w * h ];
		header.filesz = sizeof( bmpHeader ) + sizeof( bmpInfo ) + ( w * h ) + 2 + 1024;
		header.bmp_offset = sizeof( bmpHeader ) + sizeof( bmpInfo ) + 2 + 1024;
		info.header_sz = sizeof( bmpInfo );
		info.width = w;
		info.height = h;
		info.nplanes = 1;
		info.bitspp = 8;
		info.compress_type = 0;
		info.bmp_bytesz = w * h;
		info.hres = 2835;
		info.vres = 2835;
		info.ncolors = 0;
		info.nimpcolors = 0;
	}
	
	bool Save( const char* filename ) {
		if( image == NULL ) {
			std::cerr << "Image unitialized" << std::endl;
			return false;
		}
		
		std::ofstream file( filename, std::ios::out | std::ios::binary );
		
		file.write( "BM", 2 );
		file.write( (char*)( &header ), sizeof( bmpHeader ) );
		file.write( (char*)( &info ), sizeof( bmpInfo ) );
		
		char rgba[ 4 ];
		for( int i = 0; i < 256; ++i ) {
			rgba[ 0 ] = i;
			rgba[ 1 ] = i;
			rgba[ 2 ] = i;
			rgba[ 3 ] = 0;
			
			file.write( rgba, 4 );
		}
		
		file.write( image, Width() * Height() );
		
		file.close();
		
		return true;
	}
	
	bool Load(const char* filename) {
		if( image != NULL ) {
			delete[] image;
		}
		
		std::ifstream file( filename, std::ios::in | std::ios::binary );

		if( !file.is_open() ) {
			std::cerr << "Cannot open " << filename << std::endl;
			return false;
		}
	
		// Bitmaps have two magic characters at the beginning
		char BM[ 2 ];
		file.read( (char*)( BM ), 2 );
	
		// Followed by the header
		file.read( (char*)( &header ), sizeof( bmpHeader ) );

		// Followed by the info about this image.
		file.read( (char*)( &info ), sizeof( bmpInfo ) );

		// We only support BI_RGB
		if( info.compress_type != 0 ) {
			std::cerr << "Unsupported compression type " << info.compress_type << std::endl;
			return false;
		}
	
		// And 8bpp (grayscale)
		if( info.bitspp != 8 ) {
			std::cerr << "Unsupported bit depth " << info.bitspp << std::endl;
			return false;
		}	
	
		// Don't care about the palette, seek to the data
		file.seekg( header.bmp_offset, std::ios::beg );

		// And read it all in.
		image = new char[ info.width * info.height ];
	
		//Read the number of bytes based on the size of the image.
		//info.bmp_bytesz may not always be the same is the size of the image
		//array, resulting in a VS debug error when executing.
		file.read(image, info.width * info.height);
	
		file.close();

		return true;
	}

	~Bitmap() {
		if( image != NULL ) {
			delete [] image;
		}
	}
	
	int Width() {
		return info.width;
	}
	
	int Height() {
		return info.height;
	}
	
	char GetPixel( int x, int y ) {
		return image[ y * info.width + x ];
	}
	
	void SetPixel( int x, int y, char color ) {
		image[ y * info.width + x ] = color;
	}
	
	char* image;
	
private:
	struct  bmpHeader {
		uint32_t filesz;
		uint16_t creator1;
		uint16_t creator2;
		uint32_t bmp_offset;
	};
	
	struct bmpInfo {
		uint32_t header_sz;
		int32_t width;
		int32_t height;
		uint16_t nplanes;
		uint16_t bitspp;
		uint32_t compress_type;
		uint32_t bmp_bytesz;
		int32_t hres;
		int32_t vres;
		uint32_t ncolors;
		uint32_t nimpcolors;
	};
	
	bmpHeader header;
	bmpInfo info;
	
};

#endif
