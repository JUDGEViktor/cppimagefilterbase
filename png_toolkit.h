#ifndef PNG_TOOLKIT_H
#define PNG_TOOLKIT_H

#include "Libraries.h"
#include "stb_image.h"

struct image_data{
    stbi_uc* pixels;
    int w, h;
    int compPerPixel;
};

class png_toolkit{

private:    

    image_data imgData;

public:
    enum class Error {
        WrongSize,
        WrongFormat,
        Ok
    };

    png_toolkit();
    ~png_toolkit();
    bool load( std::string const &pictureName );
    bool save( std::string const &pictureName );
    image_data getPixelData( void ) const;
    void freePixels();

};

#endif // PNG_TOOLKIT_H
