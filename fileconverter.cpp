#include "fileconverter.h"

#include <QSettings>
#include <QVector>
#include "presets.h"

FileConverter::FileConverter(QString fileName) :
        file(fileName, QSettings::IniFormat)
{

}




enum FileConverter::VersionType FileConverter::isConvertible(const int version)
{
        if (version == Presets::instance().fileVersion()) {
                return ExactMatch;
        } else if (version < Presets::instance().fileVersion()) {
                if (version > 1) {
                        return Convertible;
                } else {
                        return ToOld;
                }
        } else {
                return Newer;
        }
}




bool FileConverter::convert()
{
        switch (file.value("file/version", 2).toInt()) {
        case 2:
                from2to3();
                break;

        default:
                return false;
        }

        return true;
}




void FileConverter::from2to3()
{
        QVector<QString> grades(5);
        grades[0] = "VeryGood";
        grades[1] = "Good";
        grades[2] = "Normal";
        grades[3] = "Bad";
        grades[4] = "VeryBad";

        file.beginGroup("grades");
        foreach (QString name, QString("Knowledge Skills Safety Reliability Activity ProperHandling Teamwork Responsibility").split(' ')) {
                bool ok;
                int val = file.value(name == "ProperHandling" ? "proper_handling" : name.toLower()).toInt(&ok);
                if (ok && val >= 0 && val < grades.size()) {
                        file.setValue(name, grades[val]);
                }
        }
        file.endGroup();

        file.setValue("file/version", 3);
}
