/* 
 * File:   LGridDateRenderer.cpp
 * Author: Saleem Edah-Tally - nmset@yandex.com
 * License : LGPL version 2.1
 * Copyright Saleem Edah-Tally, M. D. - © 2014
 * 
 * Created on 12 juin 2014, 18:02
 */

#include "LGridDateRenderer.h"

LGridDateRenderer::LGridDateRenderer(const wxString& outFormat, const wxString& inFormat)
: wxGridCellDateTimeRenderer(outFormat, inFormat)
{
}

LGridDateRenderer::~LGridDateRenderer()
{
}

