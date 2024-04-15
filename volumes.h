#ifndef VOLUMES_H
#define VOLUMES_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>

class Volumes : public QObject {
    Q_OBJECT

public:
    explicit Volumes(QObject *parent = nullptr);

    Q_INVOKABLE QStringList getVolumeNames();
    Q_INVOKABLE QJsonObject getVolumeInfo(QString volumeName);

signals:
};

#endif // VOLUMES_H
