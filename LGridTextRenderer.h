/* 
 * File:   LGridTextRenderer.h
 * Author: Saleem Edah-Tally - nmset@yandex.com
 * License : LGPL version 2.1
 * Copyright Saleem Edah-Tally, M. D. - © 2014
 * 
 * Created on 9 juin 2014, 19:00
 */

#ifndef LGRIDTEXTRENDERER_H
#define LGRIDTEXTRENDERER_H

#include "wx/grid.h"

class LGridTextRenderer : public wxGridCellAutoWrapStringRenderer
{
public:
    LGridTextRenderer();
    virtual ~LGridTextRenderer();
private:

};

#endif /* LGRIDTEXTRENDERER_H */

