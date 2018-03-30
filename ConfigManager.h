#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QString>
#include <QVariant>
#include <QVector>
#include <QList>
#include <QMap>
#include <QJsonObject>
#include <QJsonArray>

Q_DECLARE_METATYPE(int*)
Q_DECLARE_METATYPE(double*)
Q_DECLARE_METATYPE(QString*)

class ConfigManager
{
public:
    ConfigManager();

    void registerItem(const QString &name, ConfigManager param);

    template<typename T>
    void registerItem(const QString &name, T *param, T defaultVal)
    {
        paramMap[name].setValue(param);
        defaultMap[name].setValue(defaultVal);

        const QString type = paramMap[name].typeName();
        if (!variantFromJson.contains(type))
        {
            variantFromJson[type] = [](QVariant &to, bool contains, const QVariant &from, const QVariant &defaultVal)
            {
                *to.value<T*>() = contains? from.value<T>() : defaultVal.value<T>();
            };
        }
        if (!variantToJson.contains(type))
        {
            variantToJson[type] = [](QJsonObject &json, const QString &key, const QVariant &val)
            {
                json[key] = *val.value<T*>();
            };
        }
    }
    template <typename T, typename _>
    void registerItem(const QString &name, T* param, _ defaultVal)
    {
        registerItem<T>(name, param, (T)defaultVal);
    }


    template<typename T>
    void registerItem(const QString &name, QList<T*> param, QList<T> defaultVal)
    {
        paramMap[name].setValue(param);
        defaultMap[name].setValue(defaultVal);

        const QString type = paramMap[name].typeName();
        if (!variantFromJson.contains(type))
        {
            variantFromJson[type] = [](QVariant &to, bool contains, const QVariant &from, const QVariant &defaultVal)
            {
                auto t = to.value<QList<T*>>();
                auto f = from.toList();
                auto d = defaultVal.value<QList<T>>();
                for(int i=0; i<t.size(); i++)
                {
                    *t[i] = contains? f[i].value<T>() : d[i];
                }
            };
        }
        if (!variantToJson.contains(type))
        {
            variantToJson[type] = [](QJsonObject &json, const QString &key, const QVariant &val)
            {
                QJsonArray array;
                for(const auto &v: val.value<QList<T*>>())
                    array.append(*v);
                json[key] = array;
            };
        }
    }


    template<typename T>
    void registerItem(const QString &name, QVector<T*> param, QVector<T> defaultVal)
    {
        paramMap[name].setValue(param);
        defaultMap[name].setValue(defaultVal);

        const QString type = paramMap[name].typeName();
        if (!variantFromJson.contains(type))
        {
            variantFromJson[type] = [](QVariant &to, bool contains, const QVariant &from, const QVariant &defaultVal)
            {
                auto t = to.value<QVector<T*>>();
                auto f = from.toList();
                auto d = defaultVal.value<QVector<T>>();
                for(int i=0; i<t.size(); i++)
                {
                    *t[i] = contains? f[i].value<T>() : d[i];
                }
            };
        }
        if (!variantToJson.contains(type))
        {
            variantToJson[type] = [](QJsonObject &json, const QString &key, const QVariant &val)
            {
                QJsonArray array;
                for(const auto &v: val.value<QVector<T*>>())
                    array.append(*v);
                json[key] = array;
            };
        }
    }

    void fromJson(const QJsonObject &json);
    QJsonObject toJson();

private:
    void _fromJson(QVariantMap map, const QJsonObject &json, const QVariantMap &dMap);
    QJsonObject _toJson(const QVariantMap &map);

private:
    QVariantMap paramMap;
    QVariantMap defaultMap;

    static QMap<QString, void (*)(QVariant &to, bool contains, const QVariant &from, const QVariant &defaultVal)> variantFromJson;
    static QMap<QString, void (*)(QJsonObject &json, const QString &key, const QVariant &val)> variantToJson;
};

#endif // CONFIGMANAGER_H
