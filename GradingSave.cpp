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

#include "GradingSave.h"

#include <QFile>
#include <QIODevice>
#include <QDataStream>

#include "GradingVersion.h"

QVariant GradingSave::getValue(QString varName)
{
        if( !this->valueMap.contains(varName) )
        {
                return false;
        }

        return this->valueMap.value(varName)->getValue();
};

bool GradingSave::save( void )
{
        return this->save( this->streamType );
}

bool GradingSave::save( TYPE myStreamType )
{
        switch(myStreamType) {

        case BINARY_FILE:
                return this->saveBinaryFile();
                break;

        default:
                return this->saveBinaryFile();
                break;

        }
}

bool GradingSave::saveBinaryFile( void )
{
        QFile file(this->filename);

        if (!file.open(QIODevice::WriteOnly))
        {
                return false;
        }

        QDataStream out(&file);

        out.setVersion(QDataStream::Qt_4_6);

        out << this->fileVersion << GradingVersion::getVersion();

        foreach( QString key, this->valueMap.keys() )
        {
                out << key;
                QVariant x = this->valueMap.value(key)->getValue();
                out << x;
        }

        file.close();

        // Clear up map.
        this->valueMap.clear();

        return true;
}

bool GradingSave::load(void)
{
        return this->load( this->streamType );
}

bool GradingSave::load(TYPE myStreamType)
{
        switch( myStreamType ) {

        case BINARY_FILE:
                return this->loadBinaryFile();
                break;

        default:
                return false;
                break;

        }
}

bool GradingSave::loadBinaryFile()
{
        QString versionNumber;
        int fileFormat;
        QString key;
        QVariant value;

        QFile file(this->filename);
        if (!file.open(QIODevice::ReadOnly)) {
                return false;
        }

        QDataStream in(&file);
        in.setVersion(QDataStream::Qt_4_6);

        in >> fileFormat;
        if (fileFormat != this->fileVersion) {
                return false;
        }

        in >> versionNumber;
        if (versionNumber > GradingVersion::getVersion() ) {
                return false;
        }

        while( in.status() == QDataStream::Ok)
        {
                in >> key;
                in >> value;
                this->registerVariable(key,value);
        }

        file.close();

        return true;
}
