/* 
 * File:   LGridComboRenderer.h
 * Author: Saleem Edah-Tally - nmset@yandex.com
 * License : LGPL version 2.1
 * Copyright Saleem Edah-Tally, M. D. - © 2014
 * 
 * Created on 13 juin 2014, 18:03
 */

#ifndef LGRIDCOMBORENDERER_H
#define LGRIDCOMBORENDERER_H

#include "wx/grid.h"

class LGridComboRenderer : public wxGridCellAutoWrapStringRenderer
{
public:
    LGridComboRenderer();
    virtual ~LGridComboRenderer();
private:

};

#endif /* LGRIDCOMBORENDERER_H */

