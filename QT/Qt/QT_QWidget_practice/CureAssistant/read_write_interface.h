#ifndef READWRITEINTERFACE_H
#define READWRITEINTERFACE_H

#include <QIODevice>
#include <QSettings>
#include <QStringList>

#ifdef __cplusplus
extern "C" {
    void setConfigFilePath(const QString &strFilePath);
    bool setCurrentGroup(const QString &strGroupName);
    bool setValueNotBySettings(const QString &strKey, const QString &strValue);
    bool changeGroupName(const QString &strOldName, const QString &strNewName);
    QStringList getGroupKeys(const QString &strGroupName);
    bool IniWriteFunc(QIODevice &device, const QSettings::SettingsMap &settingsMap);
    bool IniReadFunc(QIODevice &device, QSettings::SettingsMap &settingsMap);
    bool writeDataToConfigFile();
}
#endif

#endif // READWRITEINTERFACE_H
