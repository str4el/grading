#ifndef FILECONVERTER_H
#define FILECONVERTER_H

#include <QString>
#include <QSettings>

class FileConverter
{

private:
        QSettings file;

        void from2to3 (void);
        void from3to4 (void);

public:
        enum VersionType {
                Newer,
                ExactMatch,
                Convertible,
                ToOld
        };

        FileConverter(QString fileName);
        bool convert (void);

        static enum VersionType isConvertible (const int version);
};

#endif // FILECONVERTER_H
