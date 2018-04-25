/* 
 * File:   LInformation.cpp
 * Author: SET - nmset@netcourrier.com
 * License : LGPL version 2.1
 * Copyright SET, M. D. - © 2014
 * 
 * Created on 24 mai 2014, 21:23
 */

#include "LInformation.h"

LInformation::LInformation(const wxString& newCode, 
                           const wxString& newMsg)
{
    m_code = newCode;
    m_msg = newMsg;
}

LInformation::~LInformation()
{
}

