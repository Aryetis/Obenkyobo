#include "Src/GetMy.h"

void GetMy::ClearEnabledVocabSheets()
{
    enabledSheets.clear();
    settingSerializerInstance->setValue("Vocabs/enabledSheets", QVariant::fromValue(enabledSheets));
}

void GetMy::AddEnabledVocabSheet(QString filePath)
{
    enabledSheets.insert(filePath);
    settingSerializerInstance->setValue("Vocabs/enabledSheets", QVariant::fromValue(enabledSheets));
}

void GetMy::AddEnabledVocabSheets(QSet<QString> filePaths)
{
    enabledSheets = enabledSheets.unite(filePaths);
    settingSerializerInstance->setValue("Vocabs/enabledSheets", QVariant::fromValue(enabledSheets));
}

void GetMy::RemoveEnabledVocabSheet(QString filePath)
{
    enabledSheets.remove(filePath);
    settingSerializerInstance->setValue("Vocabs/enabledSheets", QVariant::fromValue(enabledSheets));
}

void GetMy::RemoveEnabledVocabSheets(QSet<QString> filePaths)
{
    enabledSheets = enabledSheets.subtract(filePaths);
    settingSerializerInstance->setValue("Vocabs/enabledSheets", QVariant::fromValue(enabledSheets));
}
