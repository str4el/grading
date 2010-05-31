/*
 *
 *  Copyright (C) 2010 Wolfgang Forstmeier <wolfgang.forstmeier@gmail.com>
 *
 *  This file is part of grading
 *
 *  grading program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#pragma once

#include <QObject>
#include <QMap>
#include <QSharedPointer>

#include "boost/shared_ptr.hpp"

#include "GradingVariableContainer.h"

class GradingSave
{

public:
        enum TYPE { BINARY_FILE = 0 };

private:
        QMap<QString, boost::shared_ptr<GradingVariableContainer> > valueMap;
        TYPE streamType;
        QString filename;
        const int fileVersion;

public:    
        // Constructors
        GradingSave() : fileVersion(0xEFAA) {};
        GradingSave(TYPE myStreamType) : fileVersion(0xEFAA) { this->streamType = myStreamType; };

        // Destructors
        ~GradingSave() {};

        bool save(void);
        bool save(TYPE myStreamType);

        bool load(void);
        bool load(TYPE myStreamType);

        bool saveBinaryFile(void);
        bool loadBinaryFile(void);

        // Setter Methods
        template <class T>
                        bool registerVariable(QString varName, T varValue);

        void setType(TYPE myStreamType) { this->streamType = myStreamType; };
        void setFilename(QString myFilename) { this->filename = myFilename; };

        // Getter Methods
        QVariant getValue(QString varName);
};

template <class T>
                bool GradingSave::registerVariable(QString varName, T varValue)
{
        boost::shared_ptr<GradingVariableContainer> tmp(new GradingVariableContainer(varName, varValue));

        this->valueMap[varName] = tmp;

        if( !this->valueMap.contains(varName) )
        {
                return false;
        }

        return true;
};
