#include "read_write_interface.h"
#include "vs_char_set.h"

#include <QTextStream>
#include <QDataStream>
#include <QtDebug>
#include <QFile>
#include <QMap>

#define WITH_SELF_WRITE

static QString g_strConfigFilePath  = "config.ini";
static int g_currentGroupIndex      = 0;
QList<QString> g_listGroup;
QList<QStringList> g_listKeys;
QList<QStringList> g_listValues;

#ifdef __cplusplus
extern "C" {

void setConfigFilePath(const QString &strFilePath)
{
    g_strConfigFilePath = strFilePath;
}

QString variantToString(const QVariant &v)
{
    QString result;
    switch (v.type()) {
    case QVariant::String:
    case QVariant::LongLong:
    case QVariant::ULongLong:
    case QVariant::Int:
    case QVariant::UInt:
    case QVariant::Bool:
    case QVariant::Double:
    case QVariant::KeySequence: {
        result = v.toString();
        if (result.startsWith(QChar('@')))
            result.prepend(QChar('@'));
        break;
    }
    default: {
        QByteArray a;
        {
            QDataStream s(&a, QIODevice::WriteOnly);
            s.setVersion(QDataStream::Qt_4_0);
            s << v;
        }

        result = QString("@Variant(");
        result += QString::fromLatin1(a.constData(), a.size());
        result += QChar(')');
        break;
    }
    }

    return result;
}

QVariant stringToVariant(const QString &s)
{
    if (s.startsWith(QChar('@'))) {
        if (s.endsWith(QChar(')'))) {
            if (s.startsWith(QString("@Variant("))) {
                QByteArray a(s.toUtf8().mid(9));
                QDataStream stream(&a, QIODevice::ReadOnly);
                stream.setVersion(QDataStream::Qt_4_0);
                QVariant result;
                stream >> result;
                return result;
            }
        }
        if (s.startsWith(QString("@@")))
            return QVariant(s.mid(1));
    }
    return QVariant(s);
}

QByteArray escapedString(const QString &src)
{
    bool needsQuotes = false;
    bool escapeNextIfDigit = false;
    int i;
    QByteArray result;
    result.reserve(src.size() * 3 / 2);
    for (i = 0; i < src.size(); ++i) {
        uint ch = src.at(i).unicode();
        if (ch == ';' || ch == ',' || ch == '=') {
            needsQuotes = true;
        }
        if (escapeNextIfDigit && ((ch >= '0' && ch <= '9')
                                  || (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F'))) {
            result += "\\x";
            result += QByteArray::number(ch, 16);
            continue;
        }

        escapeNextIfDigit = false;

        switch (ch) {
        case '\0':
            result += "\\0";
            escapeNextIfDigit = true;
            break;
        case '\n':
            result += "\\n";
            break;
        case '\r':
            result += "\\r";
            break;
        case '\t':
            result += "\\t";
            break;
        case '"':
        case '\\':
            result += '\\';
            result += (char)ch;
            break;
        default:
            if (ch <= 0x1F) {
                result += "\\x";
                result += QByteArray::number(ch, 16);
                escapeNextIfDigit = true;
            } else{
                result += QString(src[i]).toUtf8();
            }
        }
    }
    if (result.size()>0 && (result.at(0)==' ' || result.at(result.size() - 1) == ' ')) {
        needsQuotes = true;
    }
    if (needsQuotes) {
        result.prepend('"');
        result.append('"');
    }
    return result;
}

const char hexDigits[] = "0123456789ABCDEF";

QString unescapedString(const QString &src)
{
    static const char escapeCodes[][2] =
    {
        { 'a', '\a' },
        { 'b', '\b' },
        { 'f', '\f' },
        { 'n', '\n' },
        { 'r', '\r' },
        { 't', '\t' },
        { 'v', '\v' },
        { '"', '"' },
        { '?', '?' },
        { '\'', '\'' },
        { '\\', '\\' }
    };
    static const int numEscapeCodes = sizeof(escapeCodes) / sizeof(escapeCodes[0]);

    QString stringResult;
    int escapeVal = 0;
    QChar ch;
    int i = 0;
normal:
    while (i < src.size()) {
        ch = src.at(i);
        if (ch == QChar('\\')) {
            ++i;
            if (i >= src.size()) {
                break;
            }
            ch = src.at(i++);
            for (int j = 0; j < numEscapeCodes; ++j) {
                if (ch == escapeCodes[j][0]) {
                    stringResult += QChar(escapeCodes[j][1]);
                    goto normal;
                }
            }
            if (ch == 'x') {
                escapeVal = 0;
                if (i >= src.size())
                    break;
                ch = src.at(i);
                if ((ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'F') || (ch >= 'a' && ch <= 'f'))
                    goto hexEscape;
            } else if (ch >= '0' && ch <= '7') {
                escapeVal = ch.unicode() - '0';
                goto octEscape;
            } else {
                //skip
            }
        } else {
            stringResult += ch;
        }
        i++;
    }
    goto end;

hexEscape:
    if (i >= src.size()) {
        stringResult += QChar(escapeVal);
        goto end;
    }

    ch = src.at(i);
    if (ch >= 'a')
        ch = ch.unicode() - ('a' - 'A');
    if ((ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'F')) {
        escapeVal <<= 4;
        escapeVal += strchr(hexDigits, ch.toLatin1()) - hexDigits;
        ++i;
        goto hexEscape;
    } else {
        stringResult += QChar(escapeVal);
        goto normal;
    }

octEscape:
    if (i >= src.size()) {
        stringResult += QChar(escapeVal);
        goto end;
    }

    ch = src.at(i);
    if (ch >= '0' && ch <= '7') {
        escapeVal <<= 3;
        escapeVal += ch.toLatin1() - '0';
        ++i;
        goto octEscape;
    } else {
        stringResult += QChar(escapeVal);
        goto normal;
    }

end:
    return stringResult;
}

bool IniWriteFunc(QIODevice &device, const QSettings::SettingsMap &settingsMap)
{
#ifdef Q_OS_WIN
    const char * const eol = "\r\n";
#else
    const char eol = '\n';
#endif
    bool bWriteError = false;

#ifdef WITH_SELF_WRITE
    Q_UNUSED(device);
    qDebug() << "Prepare to write data by our way.";
    return writeDataToConfigFile();
#endif

    int length = 0;

    QString lastSection;
    QMapIterator<QString,QVariant> it(settingsMap);
    while(it.hasNext() && !bWriteError) {
        it.next();
        QString key = it.key();
        QString section;
        qDebug()<<"key: "<<key;
        int idx = key.lastIndexOf(QChar('/'));
        if (idx == -1) {
            section = QString("[General]");
        } else {
            section = key.left(idx);
            key = key.mid(idx+1);
            if (section.compare(QString("General"), Qt::CaseSensitive) == 0) {
                section = QString("[%General]");
            } else {
                section.prepend(QChar('['));
                section.append(QChar(']'));
            }
        }
        if (section.compare(lastSection, Qt::CaseSensitive))
        {
            if (!lastSection.isEmpty()) {
                device.write(eol);
            }
            lastSection = section;
            if ((length = device.write(section.toUtf8() + eol)) == -1) {
                bWriteError = true;
                qDebug() << "write error: " << device.errorString();
            }
        }
        QByteArray block = key.toUtf8();
        block += "=";
        if (it.value().type() == QVariant::StringList) {
            foreach (QString s, it.value().toStringList()) {
                block += escapedString(s);
                block += ", ";
            }
            if (block.endsWith(", ")) {
                block.chop(2);
            }
        } else if (it.value().type() == QVariant::List) {
            foreach (QVariant v, it.value().toList()) {
                block += escapedString(variantToString(v));
                block += ", ";
            }
            if (block.endsWith(", ")) {
                block.chop(2);
            }
        } else {
            block += escapedString(variantToString(it.value()));
        }
        block += eol;
        if ((length = device.write(block)) == -1) {
            bWriteError = true;
            qDebug() << "write error: " << device.errorString();
        }
    }

    return bWriteError;
}

bool IniReadFunc(QIODevice &device, QSettings::SettingsMap &settingsMap)
{
    g_listGroup.clear();
    g_listKeys.clear();

    QString currentSection;
    QString strCurrentGroup = "";
    QStringList strListCurrentKeys;
    QStringList strListCurrentValues;
    QTextStream stream(&device);
    stream.setCodec("UTF-8");
    QString data;
    bool ok = true;

    while (!stream.atEnd()) {
        data = stream.readLine();
        if (data.trimmed().isEmpty()) {
            continue;
        }
        if (data[0] == QChar('[')) {
#ifdef WITH_SELF_WRITE
            /* 将上一组解析完成的结果放在list当中，文件处理结束的时候也需要设置一次 */
            if (!strCurrentGroup.isEmpty()) {
                g_listKeys.append(strListCurrentKeys);
                g_listValues.append(strListCurrentValues);

                strListCurrentKeys.clear();
                strListCurrentValues.clear();
            }
#endif
            QString iniSection;
            int inx = data.indexOf(QChar(']'));
            if (inx == -1){
                ok = false;
                iniSection = data.mid(1);
            } else {
                iniSection = data.mid(1, inx - 1);
            }

            iniSection = iniSection.trimmed();
            if (iniSection.compare(QString("general"), Qt::CaseSensitive) == 0) {
                currentSection.clear();
                strCurrentGroup.clear();
            } else {
                if (iniSection.compare(QString("%general"), Qt::CaseSensitive) == 0) {
                    currentSection = QString("general");
                } else {
                    currentSection = iniSection;
                }
#ifdef WITH_SELF_WRITE
                strCurrentGroup = currentSection;
#endif
                currentSection += QChar('/');
            }
#ifdef WITH_SELF_WRITE
            /* 由于把QStringList放入QMap中，也会导致QStringList重新排序，所以这里不能用QMap，这里并不考虑key在根部的情况，
             * 即无组名的情况 */
            g_listGroup.append(strCurrentGroup);
#endif

        } else {
            bool inQuotes = false;
            int equalsPos = -1;
            QList<int> commaPos;
            int i = 0;
            while (i < data.size())
            {
                QChar ch = data.at(i);
                if (ch == QChar('=')) {
                    if (!inQuotes && equalsPos == -1) {
                        equalsPos = i;
                    }
                } else if (ch == QChar('"')) {
                    inQuotes = !inQuotes;
                } else if (ch == QChar(',')) {
                    if (!inQuotes && equalsPos != -1) {
                        commaPos.append(i);
                    }
                } else if (ch == QChar(';')) {
                    if (!inQuotes) {
                        data.resize(i);
                        break;
                    }
                } else if (ch == QChar('\\')) {
                    if (++i < data.size()) {
                    } else {
                        ok = false;
                        break;
                    }
                }
                i++;
            }
            if (equalsPos == -1) {
                break;
            } else {
                QString key = data.mid(0, equalsPos).trimmed();
                if (key.isEmpty()) {
                    break;
                } else {
#ifdef WITH_SELF_WRITE
                    // 将当前组的key放入对应列表
                    if (!strCurrentGroup.isEmpty()) {
                        strListCurrentKeys.append(key);
//                        qDebug() << "key: " << key;
                    }
#endif
                    key = currentSection + key;
                }
                if (commaPos.isEmpty()) { //value
                    QString v = data.mid(equalsPos+1).trimmed();
                    if (v.startsWith("\"") && v.endsWith("\"") && v.length()>1) {
                        v = v.mid(1, v.length()-2);                     }
                    settingsMap[key] = stringToVariant(unescapedString(v));
#ifdef WITH_SELF_WRITE
                    strListCurrentValues.append(unescapedString(v));
#endif
                } else { //value list
                    commaPos.prepend(equalsPos);
                    commaPos.append(-1);
                    QVariantList vals;
                    for (int i=1; i<commaPos.size(); ++i) {
                        QString d = data.mid(commaPos.at(i-1)+1, commaPos.at(i)-commaPos.at(i-1)-1);
                        QString v = d.trimmed();
                        if (v.startsWith("\"") && v.endsWith("\"") && v.length()>1) {
                            v = v.mid(1, v.length()-2);                         }
                        vals.append(stringToVariant(unescapedString(v)));
                    }
                    settingsMap[key] = vals;
#ifdef WITH_SELF_WRITE
                    // 不考虑List这种情况，所以这里只取了第0个数据
                    strListCurrentValues.append(vals[0].toString());
#endif
                }
            }
        }
    }
#ifdef WITH_SELF_WRITE
    /* 文件读取完毕的时候也需要添加一次 */
    if (!strCurrentGroup.isEmpty()) {
        g_listKeys.append(strListCurrentKeys);
        g_listValues.append(strListCurrentValues);
    }
#endif

#if 0
    for (int i = 0; i < g_listGroup.count(); ++i) {
        qDebug() << "##########################";
        qDebug() << "Group: " << g_listGroup[i];
        for (int j = 0; j < g_listKeys[i].count(); ++j) {
            qDebug() << g_listKeys[i][j];
        }
    }
    qDebug() << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@";
#endif
    return ok;
}

QStringList getGroupKeys(const QString &strGroupName)
{
    QStringList strList;
    strList.clear();
    if (g_listGroup.contains(strGroupName)) {
        int index = g_listGroup.indexOf(strGroupName);
        if (index < g_listKeys.count()) {
            strList = g_listKeys[index];
        }
    }
    return strList;
}

bool setCurrentGroup(const QString &strGroupName)
{
    if (g_listGroup.contains(strGroupName)) {
        g_currentGroupIndex = g_listGroup.indexOf(strGroupName);
        return true;
    }
    qWarning() << "Wrong group name.";
    return false;
}

/* 不通过QSttring而通过自己的方式去修改内存中的值 */
bool setValueNotBySettings(const QString &strKey, const QString &strValue)
{
    int index = g_listKeys[g_currentGroupIndex].indexOf(strKey);
    if (-1 != index) {
        g_listValues[g_currentGroupIndex][index] = strValue;
    } else {
        g_listKeys[g_currentGroupIndex].append(strKey);
        g_listValues[g_currentGroupIndex].append(strValue);
    }
    return true;
}

bool changeGroupName(const QString &strOldName, const QString &strNewName)
{
    int index = g_listGroup.indexOf(strOldName);
    if (-1 != index) {
        g_listGroup[index] = strNewName;
        return true;
    }
    qWarning() << "Wrong group name or index.";
    return false;
}

bool writeDataToConfigFile()
{
#ifdef Q_OS_WIN
    const char * const eol = "\r\n";
#else
    const char eol = '\n';
#endif

    QFile device(g_strConfigFilePath);
    if (!device.open(QIODevice::WriteOnly)) {
        qWarning() << "Open file failed!";
        return false;
    } else {
        qDebug() << "Open file sucess: " << g_strConfigFilePath;
    }

    bool bWriteError = false;
    int length = 0;
    /* 需要保证g_listGroup/g_listKeys/g_listValues的大小等相对应 */
    for (int i = 0; i < g_listGroup.count() && !bWriteError; ++i) {
        QString section = g_listGroup[i];
        qDebug()<<"section: " << section;

        section.prepend(QChar('['));
        section.append(QChar(']'));

        if ((length = device.write(section.toUtf8() + eol)) == -1) {
            bWriteError = true;
            qDebug() << "write error: " << device.errorString();
        }

        for (int j = 0; j < g_listKeys[i].count() && !bWriteError; ++j) {
            QByteArray block = g_listKeys[i][j].toUtf8();
            block += "=";
            block += g_listValues[i][j].toUtf8();
            block += eol;
            if ((length = device.write(block)) == -1) {
                bWriteError = true;
                qDebug() << "write error: " << device.errorString();
            }
        }
    }

    device.close();
    return true;

}

}
#endif

