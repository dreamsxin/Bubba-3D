//
// Created by simon on 2016-02-21.
//

#include <float3.h>
#include <float4.h>
#include <Texture.h>
#include <Dimension.h>
#include "HUDGraphic.h"
#include <iostream>
#include <sstream>

using namespace chag;

HUDGraphic::HUDGraphic(Texture* texture) : HUDGraphic(texture,Dimension(),Dimension()) {}

HUDGraphic::HUDGraphic(Texture *texture, Dimension offsetX, Dimension offsetY)
        : texture(texture), offsetX(offsetX), offsetY(offsetY), textureElseColor(true){ }

HUDGraphic::HUDGraphic(Color color) : color(color.getColor()) , textureElseColor(false){ }

float3 HUDGraphic::getCenterOffset(float width, float height) {
    return make_vector(offsetX.getSize(width),offsetY.getSize(height),0.0f);
}

Texture* HUDGraphic::getTexture() {
    return texture;
}

bool HUDGraphic::isTextureElseColor() {
    return textureElseColor;
}

float4 HUDGraphic::getColor() {
    return color;
}

HUDGraphic::Color::Color(int red, int green, int blue, float opacity)
        : Color(make_vector((float)red/255.0f,(float)green/255.0f,(float)blue/255.0f,opacity)){}

HUDGraphic::Color::Color(float4 rawColor) : color(rawColor) { }

HUDGraphic::Color::Color(int red, int green, int blue)  : Color(red,green,blue,1.0f){}

HUDGraphic::Color::Color(string* hexString) : Color(hexString,1.0f) { }

HUDGraphic::Color::Color(string* hexString, float opacity) {
    if(hexString->length() == 0)
        throw invalid_argument("The color hexString can't be empty in HUDGraphic::Color::Color().");

    if((*hexString)[0] == '#')
        *hexString = hexString->substr(1,hexString->length()-1);

    if (hexString->find_first_not_of("0123456789abcdefABCDEF", 0) != std::string::npos) {
        throw invalid_argument("The color hexString can only contain: 0-9, a-f or A-F HUDGraphic::Color::Color()->");
    }

    float divBy;
    unsigned int valLen;
    if(hexString->length() == 3){
        divBy = 15.0f;
        valLen = 1;
    } else if(hexString->length() == 6){
        divBy = 255.0f;
        valLen = 2;
    }else
        throw invalid_argument("The color hexString has to be of length 3 or 6 in HUDGraphic::Color::Color().");

    hexStringToFloat(hexString,0,valLen,&color.x);
    hexStringToFloat(hexString,valLen*1,valLen,&color.y);
    hexStringToFloat(hexString,valLen*2,valLen,&color.z);
    color /= divBy;

    color.w = opacity;
}

void HUDGraphic::Color::hexStringToFloat(string* hexString,unsigned int pos, unsigned int len, float* target){
    int temp = 0;
    stringstream(hexString->substr(pos,len)) >> hex >> temp;
    *target = (float)temp;
}

float4 HUDGraphic::Color::getColor() {
    return color;
}