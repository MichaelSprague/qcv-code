/*
 * Copyright (C) 2012 Hernan Badino <hernan.badino@gmail.com>
 *
 * This file is part of QCV
 *
 * QCV is under the terms of the GNU Lesser General Public License
 * version 2.1. See the GNU LGPL version 2.1 for details.
 * QCV is distributed "AS IS" without ANY WARRANTY, without even the
 * implied warranty of merchantability or fitness for a particular
 * purpose. 
 *
 * In no event shall the authors or contributors be liable
 * for any direct, indirect, incidental, special, exemplary, or
 * consequential damages arising in any way out of the use of this
 * software.
 *
 * By downloading, copying, installing or using the software you agree
 * to this license. Do not download, install, copy or use the
 * software, if you do not agree to this license.
 */


#ifndef __FLOATPARAM_H
#define __FLOATPARAM_H

/**
 *******************************************************************************
 *
 * @file floatParam.h
 *
 * \class CFloatParameter
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief Parameter interface for a float variable.
 *
 * The class implements a parameter interface for a float variable.
 *
 *******************************************************************************/

/* INCLUDES */
#include "parameter.h"

namespace QCV
{

    class CFloatParameter : public CParameter
    {
    /// Constructors/Destructor
    public:
        
        /// Constructors/Destructor
        CFloatParameter (  std::string               f_name_str = "",
                           std::string               f_comment_str = "",
                           float                     f_value_f = 0.0f,
                           CParameterBaseConnector * f_connector_p = NULL );

        virtual ~CFloatParameter ();

    public:
        /// Get and set values from strings.
        virtual std::string     getStringFromValue ( ) const;
        virtual bool            setValueFromString ( std::string f_val_str );
 
        /// Get and set bool value
        virtual float           getValue ( ) const;
        virtual bool            setValue ( float  f_value_b,
                                           bool   f_shouldUpdate_b = true );
        

        /// Get editor.
        virtual QWidget *       createEditor ( );

    /// Protected members
    protected:

        /// Stored value.
        float                  m_value_f;
    };
}


#endif // __FLOATPARAM_H
