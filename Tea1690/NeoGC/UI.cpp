#include "ui.h"
#include "Font.h"

void placeholder()
{
    std::cout << "This button has been clicked." << std::endl;
}

#define screenPixelUnitXHalf 0.00390625

#define screenPixelUnitYHalf 0.00390625

#define screenPixelUnitX 0.001953125

#define screenPixelUnitY 0.001953125

inline double screenToPixelCoordinateX(int X)
{
    return (X * screenPixelUnitXHalf / (Vwidth * screenPixelUnitX)) - 1;
}

inline double screenToPixelCoordinateY(int Y)
{
    return (Y * -screenPixelUnitYHalf / (Vheight * screenPixelUnitY)) + 1;
}

inline double offsetFromEdgeX(int X)
{
    return 1 - (-X * screenPixelUnitXHalf / (Vwidth * screenPixelUnitX));
}

inline double offsetFromEdgeY(int Y)
{
    return -1 + (Y * -screenPixelUnitYHalf / (Vheight * screenPixelUnitY));
}

inline double screenToLocalPixelCoordinateX(int X)
{
    return (X * screenPixelUnitXHalf) - 1;
}

inline double screenToLocalPixelCoordinateY(int Y)
{
    return (Y * screenPixelUnitYHalf) + 1;
}

void drawPane(int width, int height, int X, int Y, unsigned char flags, bool isClicked, unsigned char outlineThickness, void (*functionOnClick)(), unsigned short ID)       //note that horizontal stretch/vertical stretch overrides anchor options
{
    if (ID > 0 && ID != PSEdropDownID)
        return;
    float rectX1;
    float rectX2;
    float rectY1;
    float rectY2;

    float rectX3;
    float rectX4;
    float rectY3;
    float rectY4;

    if ((flags & anchorHorizontal))
    {
        rectX1 = offsetFromEdgeX(X - 720);
        rectX2 = offsetFromEdgeX((X - 720) + width);
        rectX3 = offsetFromEdgeX(X - 720 + outlineThickness);
        rectX4 = offsetFromEdgeX((X - 720) + width - outlineThickness);
    }
    else
    {
        rectX1 = screenToPixelCoordinateX(X);
        rectX2 = screenToPixelCoordinateX(X + width);
        rectX3 = screenToPixelCoordinateX(X + outlineThickness);
        rectX4 = screenToPixelCoordinateX(X + width - outlineThickness);
    }
    if ((flags & anchorVertical))
    {
        rectY2 = offsetFromEdgeY(Y - 480);
        rectY1 = offsetFromEdgeY((Y - 480) + height);
        rectY4 = offsetFromEdgeY(Y - 480 + outlineThickness);
        rectY3 = offsetFromEdgeY((Y - 480) + height - outlineThickness);
    }
    else
    {
        rectY2 = screenToPixelCoordinateY(Y);
        rectY1 = screenToPixelCoordinateY(Y + height);
        rectY4 = screenToPixelCoordinateY(Y + outlineThickness);
        rectY3 = screenToPixelCoordinateY(Y + height - outlineThickness);
    }

    if ((flags & stretchHorizontal))
    {
        rectX1 = -1;
        rectX2 = 1;
        rectX3 = screenToPixelCoordinateX(outlineThickness);
        rectX4 = offsetFromEdgeX(-outlineThickness);
    }

    if ((flags & stretchVertical))
    {
        rectY1 = -1;
        rectY2 = 1;
        rectY3 = screenToPixelCoordinateY(outlineThickness);
        rectY4 = offsetFromEdgeY(-outlineThickness);
    }

    if ((flags & stretchHorizontal))
    {
        rectX1 = -1;
        rectX2 = 1;
        rectX3 = screenToPixelCoordinateX(outlineThickness);
        rectX4 = offsetFromEdgeX(-outlineThickness);
    }

    if ((flags & maintainLocalHorizontal))
    {
        rectX1 = screenToLocalPixelCoordinateX(X);
        rectX2 = screenToLocalPixelCoordinateX(X) + screenToPixelCoordinateX(width) + 1;
        rectX3 = screenToLocalPixelCoordinateX(X) + screenToPixelCoordinateX(outlineThickness) + 1;
        rectX4 = screenToLocalPixelCoordinateX(X) + screenToPixelCoordinateX(width - outlineThickness) + 1;
    }

    if ((flags & maintainLocalVertical))
    {
        rectY1 = screenToLocalPixelCoordinateY(Y);
        rectY2 = screenToLocalPixelCoordinateY(Y) + screenToPixelCoordinateY(height) + 1;
        rectY3 = screenToLocalPixelCoordinateY(Y) + screenToPixelCoordinateY(outlineThickness) + 1;
        rectY4 = screenToLocalPixelCoordinateY(Y) + screenToPixelCoordinateY(height - outlineThickness) + 1;
    }

    //glColor3f(0, 0, 0);

    glBegin(GL_QUADS);

    glVertex2d(0, 0);
    glTexCoord2d(0, 0);
    glVertex2d(0, 0);
    glTexCoord2d(0, 0);
    glVertex2d(0, 0);
    glTexCoord2d(0, 0);
    glVertex2d(0, 0);
    glTexCoord2d(0, 1);
    //bottom left
    glVertex2d(rectX1, rectY1 - screenToPixelCoordinateY(outlineThickness) + 1);
    glTexCoord2d(1, 1);
    glVertex2d(rectX1 + screenToPixelCoordinateX(outlineThickness) + 1, rectY1 - screenToPixelCoordinateY(outlineThickness) + 1);
    glTexCoord2d(1, 0);
    glVertex2d(rectX1 + screenToPixelCoordinateX(outlineThickness) + 1, rectY1);
    glTexCoord2d(0, 0);
    glVertex2d(rectX1, rectY1);
    glTexCoord2d(0, 1);

    //top left
    glVertex2d(rectX1, rectY2 + screenToPixelCoordinateY(outlineThickness) - 1);
    glTexCoord2d(1, 1);
    glVertex2d(rectX1 + screenToPixelCoordinateX(outlineThickness) + 1, rectY2 + screenToPixelCoordinateY(outlineThickness) - 1);
    glTexCoord2d(1, 0);
    glVertex2d(rectX1 + screenToPixelCoordinateX(outlineThickness) + 1, rectY2);
    glTexCoord2d(0, 0);
    glVertex2d(rectX1, rectY2);
    glTexCoord2d(0, 1);

    //top right
    glVertex2d(rectX2, rectY2 + screenToPixelCoordinateY(outlineThickness) - 1);
    glTexCoord2d(1, 1);
    glVertex2d(rectX2 - screenToPixelCoordinateX(outlineThickness) - 1, rectY2 + screenToPixelCoordinateY(outlineThickness) - 1);
    glTexCoord2d(1, 0);
    glVertex2d(rectX2 - screenToPixelCoordinateX(outlineThickness) - 1, rectY2);
    glTexCoord2d(0, 0);
    glVertex2d(rectX2, rectY2);
    glTexCoord2d(0, 1);

    //bottom right
    glVertex2d(rectX2, rectY1 - screenToPixelCoordinateY(outlineThickness) + 1);
    glTexCoord2d(1, 1);
    glVertex2d(rectX2 - screenToPixelCoordinateX(outlineThickness) - 1, rectY1 - screenToPixelCoordinateY(outlineThickness) + 1);
    glTexCoord2d(1, 0);
    glVertex2d(rectX2 - screenToPixelCoordinateX(outlineThickness) - 1, rectY1);
    glTexCoord2d(0, 0);
    glVertex2d(rectX2, rectY1);
    glTexCoord2d(0, 1);

    //left column
    glVertex2d(rectX1, rectY1 - screenToPixelCoordinateY(outlineThickness) + 1);
    glTexCoord2d(1, 1);
    glVertex2d(rectX1 + screenToPixelCoordinateX(outlineThickness) + 1, rectY1 - screenToPixelCoordinateY(outlineThickness) + 1);
    glTexCoord2d(1, 1);
    glVertex2d(rectX1 + screenToPixelCoordinateX(outlineThickness) + 1, rectY2 + screenToPixelCoordinateY(outlineThickness) - 1);
    glTexCoord2d(0, 1);
    glVertex2d(rectX1, rectY2 + screenToPixelCoordinateY(outlineThickness) - 1);
    glTexCoord2d(0, 1);

    //right column
    glVertex2d(rectX2, rectY1 - screenToPixelCoordinateY(outlineThickness) + 1);
    glTexCoord2d(1, 1);
    glVertex2d(rectX2 - screenToPixelCoordinateX(outlineThickness) - 1, rectY1 - screenToPixelCoordinateY(outlineThickness) + 1);
    glTexCoord2d(1, 1);
    glVertex2d(rectX2 - screenToPixelCoordinateX(outlineThickness) - 1, rectY2 + screenToPixelCoordinateY(outlineThickness) - 1);
    glTexCoord2d(0, 1);
    glVertex2d(rectX2, rectY2 + screenToPixelCoordinateY(outlineThickness) - 1);
    glTexCoord2d(0, 1);

    //top row
    glVertex2d(rectX1 + screenToPixelCoordinateX(outlineThickness) + 1, rectY2 + screenToPixelCoordinateY(outlineThickness) - 1);
    glTexCoord2d(200, 1);
    glVertex2d(rectX2 - screenToPixelCoordinateX(outlineThickness) - 1, rectY2 + screenToPixelCoordinateY(outlineThickness) - 1);
    glTexCoord2d(200, 0);
    glVertex2d(rectX2 - screenToPixelCoordinateX(outlineThickness) - 1, rectY2);
    glTexCoord2d(200, 0);
    glVertex2d(rectX1 + screenToPixelCoordinateX(outlineThickness) + 1, rectY2);
    glTexCoord2d(200, 1);

    //bottom row
    glVertex2d(rectX1 + screenToPixelCoordinateX(outlineThickness) + 1, rectY1 - screenToPixelCoordinateY(outlineThickness) + 1);
    glTexCoord2d(200, 1);
    glVertex2d(rectX2 - screenToPixelCoordinateX(outlineThickness) - 1, rectY1 - screenToPixelCoordinateY(outlineThickness) + 1);
    glTexCoord2d(200, 0);
    glVertex2d(rectX2 - screenToPixelCoordinateX(outlineThickness) - 1, rectY1);
    glTexCoord2d(200, 0);
    glVertex2d(rectX1 + screenToPixelCoordinateX(outlineThickness) + 1, rectY1);
    glTexCoord2d(200, 1);

    //middle
    glVertex2d(rectX1 + screenToPixelCoordinateX(outlineThickness) + 1, rectY1 - screenToPixelCoordinateY(outlineThickness) + 1);
    glTexCoord2d(200, 200);
    glVertex2d(rectX2 - screenToPixelCoordinateX(outlineThickness) - 1, rectY1 - screenToPixelCoordinateY(outlineThickness) + 1);
    glTexCoord2d(200, 200);
    glVertex2d(rectX2 - screenToPixelCoordinateX(outlineThickness) - 1, rectY2 + screenToPixelCoordinateY(outlineThickness) - 1);
    glTexCoord2d(200, 200);
    glVertex2d(rectX1 + screenToPixelCoordinateX(outlineThickness) + 1, rectY2 + screenToPixelCoordinateY(outlineThickness) - 1);
    glTexCoord2d(200, 200);
    glEnd();

    double mouseX, mouseY;
    glfwGetCursorPos(debugWindow, &mouseX, &mouseY);
    //std::cout << "mouse X: " << mouseX << std::endl;
    //std::cout << "mouse Y: " << mouseY << std::endl;
    mouseX = ((mouseX / Vwidth) * 2) - 1;
    mouseY =  1 - ((mouseY / Vheight) * 2);
    //std::cout << "X1: " << rectX1 << std::endl;
    //std::cout << "X2: " << rectX2 << std::endl;
    //std::cout << "Y1: " << rectY1 << std::endl;
    //std::cout << "Y2: " << rectY2 << std::endl;
    //std::cout << "normalized mouse X: " << mouseX << std::endl;
    //std::cout << "normalized mouse Y: " << mouseY << std::endl;
    if (isClicked == true)
    {
        if (mouseX > -1 && mouseX < 1 && mouseY > -1 && mouseY < 1)
        {
            if (mouseX > rectX1 && mouseX < rectX2 && mouseY > rectY1 && mouseY < rectY2)
            {
                functionOnClick();
            }
        }
    }
}

#define fontTexUnit 0.00390625

inline float coordinateToTexturePos(int Pos)
{
    return (Pos * fontTexUnit);
}

void drawText(float X, float Y, unsigned short lineHeight, unsigned short tabWidth, float size, unsigned short charWrap, const char* string, unsigned short ID, unsigned short characterSpacing)
{
    if (ID > 0 && ID != PSEdropDownID)
        return;
    int textStringOffset = 0;
    int texPixelX = ((X + 1) * Vwidth) / 2;
    int texPixelY = ((Y + 1) * Vheight) / 2;
    int texPixelX2 = X;
    int texPixelY2 = Y;
    int charWCount = 0;
    while (string[textStringOffset] != 0)
    {
        if (string[textStringOffset] < 32 && string[textStringOffset] >= 0)
            switch (string[textStringOffset])
            {
            case 13:
                texPixelX = ((X + 1) * Vwidth) / 2;
                charWCount = 0;
                break;
            case 10:
                texPixelY += lineHeight;
                texPixelX = ((X + 1) * Vwidth) / 2;
                charWCount = 0;
                break;
            case 9:
                texPixelX += tabWidth * size;
                charWCount += tabWidth * size;
                break;
            }
        else
        {
            char getCharID = 95;
            if (string[textStringOffset] > 0)
                getCharID = string[textStringOffset] - 32;
            double fontTexH = coordinateToTexturePos(glyphLists[getCharID].x);
            double fontTexH2 = coordinateToTexturePos(glyphLists[getCharID].x + glyphLists[getCharID].width);
            double fontTexV = coordinateToTexturePos(glyphLists[getCharID].y);
            double fontTexV2 = coordinateToTexturePos(glyphLists[getCharID].y + glyphLists[getCharID].height);
            texPixelX2 = texPixelX + glyphLists[getCharID].width * size;
            texPixelY2 = texPixelY + glyphLists[getCharID].height * size;
            double texPixelOFX2 = glyphLists[getCharID].xoffset * size;
            double texPixelOFY2 = glyphLists[getCharID].yoffset * size;
            glBegin(GL_QUADS);
            glTexCoord2d(fontTexH, fontTexV2), glVertex2d(screenToPixelCoordinateX(texPixelX + texPixelOFX2), screenToPixelCoordinateY(texPixelY2 + texPixelOFY2));
            glTexCoord2d(fontTexH2, fontTexV2), glVertex2d(screenToPixelCoordinateX(texPixelX2 + texPixelOFX2), screenToPixelCoordinateY(texPixelY2 + texPixelOFY2));
            glTexCoord2d(fontTexH2, fontTexV), glVertex2d(screenToPixelCoordinateX(texPixelX2 + texPixelOFX2), screenToPixelCoordinateY(texPixelY + texPixelOFY2));
            glTexCoord2d(fontTexH, fontTexV), glVertex2d(screenToPixelCoordinateX(texPixelX + texPixelOFX2), screenToPixelCoordinateY(texPixelY + texPixelOFY2));
            glEnd();
            if (characterSpacing == 0)
            {
                texPixelX += glyphLists[getCharID].xadvance * size;
                charWCount += glyphLists[getCharID].xadvance * size;
            }
            else
            {
                texPixelX += characterSpacing * size;
                charWCount += characterSpacing * size;
            }
        }
        textStringOffset++;
        if (charWCount > charWrap)
        {
            charWCount = 0;
            texPixelX = ((X + 1) * Vwidth) / 2;
            texPixelY += lineHeight * size;
        }
    }
}

unsigned char paneTexture[512] = {
    // Offset 0x00000000 to 0x000001FF
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x02, 0x03, 0x04, 0x02, 0x06,
    0x02, 0x06, 0x02, 0x06, 0x02, 0x06, 0x02, 0x06, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x02, 0x03, 0x04,
    0x02, 0x07, 0x01, 0x14, 0x00, 0x31, 0x00, 0x3F, 0x00, 0x40, 0x00, 0x40,
    0x00, 0x40, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x08, 0x02, 0x02, 0x06, 0x01, 0x14, 0x00, 0x31, 0x00, 0x41, 0x00, 0x60,
    0x00, 0x9F, 0x00, 0xBC, 0x01, 0xBF, 0x01, 0xBF, 0x02, 0xBF, 0x02, 0xBF,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x03, 0x01, 0x13, 0x00, 0x34,
    0x00, 0x5E, 0x00, 0x9F, 0x00, 0xBE, 0x00, 0xCE, 0x01, 0xEB, 0x03, 0xF8,
    0x12, 0xF9, 0x24, 0xF9, 0x29, 0xF9, 0x2A, 0xF9, 0x00, 0x00, 0x00, 0x00,
    0x08, 0x02, 0x01, 0x13, 0x00, 0x50, 0x00, 0x9F, 0x00, 0xCB, 0x00, 0xEB,
    0x03, 0xF8, 0x12, 0xFB, 0x23, 0xFD, 0x2E, 0xFF, 0x50, 0xFF, 0x7A, 0xFF,
    0x88, 0xFF, 0x89, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x02, 0x06, 0x00, 0x34,
    0x00, 0x9F, 0x00, 0xE7, 0x02, 0xF9, 0x12, 0xFD, 0x29, 0xFF, 0x4F, 0xFF,
    0x7A, 0xFF, 0x8B, 0xFF, 0xA5, 0xFF, 0xCF, 0xFF, 0xDF, 0xFF, 0xE0, 0xFF,
    0x00, 0x00, 0x08, 0x02, 0x01, 0x14, 0x00, 0x5E, 0x00, 0xCB, 0x02, 0xF9,
    0x1B, 0xFF, 0x4D, 0xFF, 0x7D, 0xFF, 0xA4, 0xFF, 0xCF, 0xFF, 0xE0, 0xFF,
    0xE8, 0xFF, 0xF6, 0xFF, 0xFC, 0xFF, 0xFC, 0xFF, 0x00, 0x00, 0x03, 0x04,
    0x00, 0x31, 0x00, 0x9F, 0x00, 0xEB, 0x12, 0xFD, 0x4D, 0xFF, 0x98, 0xFF,
    0xCF, 0xFF, 0xE7, 0xFF, 0xF6, 0xFF, 0xFC, 0xFF, 0xFD, 0xFF, 0xFE, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x02, 0x07, 0x00, 0x41, 0x00, 0xBE,
    0x03, 0xF8, 0x29, 0xFF, 0x7D, 0xFF, 0xCF, 0xFF, 0xF4, 0xFF, 0xFC, 0xFF,
    0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0x08, 0x02, 0x01, 0x14, 0x00, 0x60, 0x00, 0xCE, 0x12, 0xFB, 0x4F, 0xFF,
    0xA4, 0xFF, 0xE7, 0xFF, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x03, 0x04, 0x00, 0x31,
    0x00, 0x9F, 0x01, 0xEB, 0x23, 0xFD, 0x7A, 0xFF, 0xCF, 0xFF, 0xF6, 0xFF,
    0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0x02, 0x06, 0x00, 0x3F, 0x00, 0xBC, 0x03, 0xF8,
    0x2E, 0xFF, 0x8B, 0xFF, 0xE0, 0xFF, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0x02, 0x06, 0x00, 0x40, 0x01, 0xBF, 0x12, 0xF9, 0x50, 0xFF, 0xA5, 0xFF,
    0xE8, 0xFF, 0xFD, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x02, 0x06, 0x00, 0x40,
    0x01, 0xBF, 0x24, 0xF9, 0x7A, 0xFF, 0xCF, 0xFF, 0xF6, 0xFF, 0xFE, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0x02, 0x06, 0x00, 0x40, 0x02, 0xBF, 0x29, 0xF9,
    0x88, 0xFF, 0xDF, 0xFF, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0x02, 0x06, 0x00, 0x40, 0x02, 0xBF, 0x2A, 0xF9, 0x89, 0xFF, 0xE0, 0xFF,
    0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};