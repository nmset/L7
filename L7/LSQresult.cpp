/* 
 * File:   LSQresult.cpp
 * Author: SET - nmset@yandex.com
 * License : LGPL version 2.1
 * Copyright SET, M. D. - © 2014
 * 
 * Created on 31 mai 2014, 18:57
 */
#ifdef USE_LIBSQ

#include "stddef.h"
#include "LSQresult.h"
using namespace SQ;

SQresult::SQresult()
{
    m_nbRows = 0;
    m_nbCols = 0;
    m_data = NULL;
}

SQresult::~SQresult()
{
}

#endif