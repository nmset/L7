/* 
 * File:   JsonHelper.h
 * Author: Saleem Edah-Tally - nmset@yandex.com
 * License : LGPL version 2.1
 * Copyright Saleem Edah-Tally, M. D. - © 2014
 *
 * Created on December 16, 2019, 8:25 PM
 */

#ifndef JSONHELPER_H
#define JSONHELPER_H

#include <wx/wx.h>

/**
 * Application helper to quickly get the Intent value.
 * @param jsonData
 * @return 
 */
class JsonHelper
{
public:
    
    /**
     * Returns a valid Intent value or an empty string.
     * @param jsonData
     * @return 
     */
    static wxString GetIntent(const wxString& jsonData);
private:
    JsonHelper();
    virtual ~JsonHelper();
};

#endif /* JSONHELPER_H */

