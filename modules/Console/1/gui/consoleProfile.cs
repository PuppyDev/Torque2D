//-----------------------------------------------------------------------------
// Copyright (c) 2013 GarageGames, LLC
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//-----------------------------------------------------------------------------

if(!isObject(GuiConsoleProfile)) new GuiControlProfile (GuiConsoleProfile)
{
    fontType = $platformFontType;
    fontSize = $platformFontSize * 1.1;
    fontColor = White;
    fontColorHL = LightSlateGray;
    fontColorNA = Red;
    fontColors[6] = "100 100 100";
    fontColors[7] = "100 100 0";
    fontColors[8] = "0 0 100";
    fontColors[9] = "0 100 0";
};

//-----------------------------------------------------------------------------

if(!isObject(ConsoleBoxProfile)) new GuiControlProfile(ConsoleBoxProfile)
{
    border = 1;
    bitmap = "^Console/gui/images/scrollBar.png";
    hasBitmapArray = true;    
    opaque = true;
    fillColor = "255 255 255 220";
    border = 3;
    borderThickness = 0;
    borderColor = "0 0 0";
};

//-----------------------------------------------------------------------------

if(!isObject(GuiConsoleTextEditProfile)) new GuiControlProfile (GuiConsoleTextEditProfile)
{
    fontType = $platformFontType;
    fontSize = $platformFontSize * 1.1;
    opaque = false;
    fillColor = "232 240 248 255";
    fillColorHL = "251 170 0 255";
    fillColorNA = "127 127 127 52";
    border = -2;
    bitmap = "^Console/gui/images/textEdit.png";
    borderColor = "40 40 40 10";
    fontColor = "27 59 95 255";
    fontColorHL = "232 240 248 255";
    fontColorNA = "0 0 0 52";
    fontColorSEL = "0 0 0 255";
    textOffset = "5 2";
    autoSizeWidth = false;
    autoSizeHeight = false;
    tab = false;
    canKeyFocus = true;
    returnTab = true;
};

//-----------------------------------------------------------------------------

if(!isObject(ConsoleScrollProfile)) new GuiControlProfile( ConsoleScrollProfile)
{
    opaque = true;
    fillColor = "0 0 0 120";
    border = 3;
    borderThickness = 0;
    borderColor = "0 0 0";
    bitmap = "^Console/gui/images/scrollBar.png";
    hasBitmapArray = true;
};

//-----------------------------------------------------------------------------

if (!isObject(ConsoleButtonProfile)) new GuiControlProfile (ConsoleButtonProfile)
{
    opaque = true;
    border = -1;
    fixedExtent = true;
    justify = "center";
    canKeyFocus = false;
    fontType = $platformFontType;
    fontSize = $platformFontSize;
    fontColor = "255 255 255 255";
    fontColorHL = "255 255 255 255";
    bitmap = "^Console/gui/images/blueButton.png";
};