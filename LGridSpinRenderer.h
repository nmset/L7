/* 
 * File:   LGridSpinRenderer.h
 * Author: Saleem Edah-Tally - nmset@yandex.com
 * License : LGPL version 2.1
 * Copyright Saleem Edah-Tally, M. D. - © 2014
 * 
 * Created on 25 septembre 2014, 21:19
 */

#ifndef LGRIDSPINRENDERER_H
#define LGRIDSPINRENDERER_H

#include <wx/grid.h>

class LGridSpinRenderer : public wxGridCellNumberRenderer
{
public:
    LGridSpinRenderer();
    virtual ~LGridSpinRenderer();
private:

};

#endif /* LGRIDSPINRENDERER_H */

