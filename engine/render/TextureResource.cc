#include "config.h"
#include "TextureResource.h"
#include <iostream>

void TextureResource::BindTexture(){
    glBindTexture(GL_TEXTURE_2D, this->texture);
}
void TextureResource::UnbindTexture(){
    glBindTexture(GL_TEXTURE_2D, 0);
}
void TextureResource::LoadFromFile(const char* filename){
    int width, height, channels;

    unsigned char* image = stbi_load(filename, &width, &height, &channels, 0);
    if(image == NULL){
        std::cout << "ERROR: Image load error, check 'TextureResource.cc'" << std::endl;
    }
    glGenTextures(1, &this->texture);
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    if(channels == 3){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    }else if(channels == 4){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    }
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(image);
}

TextureResource::~TextureResource(){
    glDeleteTextures(1, &this->texture);
}
