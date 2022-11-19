/* 
 * File:   LGridDateRenderer.h
 * Author: Saleem Edah-Tally - nmset@yandex.com
 * License : LGPL version 2.1
 * Copyright Saleem Edah-Tally, M. D. - © 2014
 * 
 * Created on 12 juin 2014, 18:01
 */

#ifndef LGRIDDATERENDERER_H
#define LGRIDDATERENDERER_H

#include <wx/grid.h>

class LGridDateRenderer : public wxGridCellDateTimeRenderer
{
public:
    LGridDateRenderer(const wxString& outFormat = _T("%a %d %b %Y"), const wxString& inFormat = _T("%Y-%m-%d"));
    virtual ~LGridDateRenderer();
private:

};

#endif /* LGRIDDATERENDERER_H */

