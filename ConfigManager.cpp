#include "configmanager.h"

decltype(ConfigManager::variantFromJson) ConfigManager::variantFromJson;
decltype(ConfigManager::variantToJson) ConfigManager::variantToJson;

ConfigManager::ConfigManager()
{

}


void ConfigManager::registerItem(const QString &name, ConfigManager param)
{
    paramMap.insert(name, param.paramMap);
    defaultMap.insert(name, param.defaultMap);
}


void ConfigManager::fromJson(const QJsonObject &json)
{
    _fromJson(paramMap, json, defaultMap);
}

QJsonObject ConfigManager::toJson()
{
    return _toJson(paramMap);
}

void ConfigManager::_fromJson(QVariantMap map, const QJsonObject &json, const QVariantMap &dMap)
{
    for(auto it=map.begin(); it!=map.end(); ++it)
    {
        const QString key = it.key();
        const QString type = it.value().typeName();

        if (type == QMetaType::typeName(QVariant::Map))
        {
             _fromJson(it.value().toMap(), json[key].toObject(), dMap[key].toMap());
        }
        else if (variantFromJson.contains(type))
        {
            variantFromJson[type](it.value(), json.contains(key), json[key].toVariant(), dMap[key]);
        }
    }
}

QJsonObject ConfigManager::_toJson(const QVariantMap &map)
{
    QJsonObject json;

    for(auto it=map.begin(); it!=map.end(); ++it)
    {
        const QString key = it.key();
        const QString type = it.value().typeName();

        if (type == QMetaType::typeName(QVariant::Map))
        {
             json[key] = _toJson(it.value().toMap());
        }
        else if (variantToJson.contains(type))
        {
            variantToJson[type](json, key, it.value());
        }
    }

    return json;
}
