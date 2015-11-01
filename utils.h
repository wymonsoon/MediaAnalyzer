#ifndef UTILS_H
#define UTILS_H

#include <QChar>
#include <QString>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QFile>
#include <QIODevice>
#include <QJsonObject>

class JsonParser {
public:
    static QJsonDocument loads(QString path, QJsonParseError& error) {
        QJsonDocument doc = QJsonDocument();
        if (QFile::exists(path)) {
            QFile jFile(path);
            jFile.open(QIODevice::ReadOnly);
            QByteArray jBuffer = jFile.readAll();
            jFile.close();
            doc = QJsonDocument::fromJson(jBuffer, &error);
        }
        return doc;
    }

    static QString getValueStr(QJsonObject obj, const char* key) {
        return obj.value(QString(key)).toString();
    }
};

class FileUtils {
public:
    static QString basename(QString path) {
        int index = path.lastIndexOf('.');
        if (index == -1) {
            return path;
        } else {
            return path.left(index);
        }
    }

    static QString extname(QString path) {
        int index = path.lastIndexOf('.');
        if (index == -1) {
            return path;
        } else {
            int len = path.length();
            return path.right(len - index);
        }
    }

    static QString urldir(QString path) {
        int index = path.lastIndexOf('/');
        if (index == -1) {
            return path;
        } else {
            return path.left(index);
        }
    }

    static QString dirname(QString path) {
    #ifdef WIN32
        QChar sep('\\');
    #else
        QChar sep('/');
    #endif
        int index = path.lastIndexOf(sep);
        if (index == -1) {
            return path;
        } else {
            return path.left(index);
        }
    }
};

#endif // UTILS_H

