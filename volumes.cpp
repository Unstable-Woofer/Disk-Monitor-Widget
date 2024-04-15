#include "volumes.h"
#include "windows.h"

#include <QDebug>

Volumes::Volumes(QObject *parent) : QObject{parent} {}

QStringList Volumes::getVolumeNames() {
    QStringList volumeNames;

    WCHAR volumeName[MAX_PATH];
    DWORD len;
    HANDLE hFindVolume;

    hFindVolume = FindFirstVolumeW(volumeName, ARRAYSIZE(volumeName));
    if (hFindVolume != INVALID_HANDLE_VALUE) {
        do {
            WCHAR volumePath[MAX_PATH];
            DWORD pathLen;

            volumeNames.append(QString::fromWCharArray(volumeName));
        } while (FindNextVolumeW(hFindVolume, volumeName, ARRAYSIZE(volumeName)));
        FindVolumeClose(hFindVolume);
    }

    return volumeNames;
}

QJsonObject Volumes::getVolumeInfo(QString volumeName) {
    WCHAR volumePath[MAX_PATH];
    DWORD len = MAX_PATH + 1;

    QJsonObject volumeInfo;


    qDebug() << volumeName;

    if (GetVolumePathNamesForVolumeName(reinterpret_cast<LPCWSTR>(volumeName.utf16()), volumePath, len, &len)) {
        WCHAR volumeNameBuffer[MAX_PATH + 1] = {0};
        DWORD serialNumber = 0;
        DWORD maxComponentLen = 0;
        DWORD fileSystemFlags = 0;
        WCHAR fileSystemNameBuffer[MAX_PATH + 1] = {0};

        if (GetVolumeInformationW(
                volumePath,
                volumeNameBuffer,
                MAX_PATH + 1,
                &serialNumber,
                &maxComponentLen,
                &fileSystemFlags,
                fileSystemNameBuffer,
                MAX_PATH + 1)) {

            volumeInfo["path"] = QString::fromWCharArray(volumePath);
            volumeInfo["name"] = QString::fromWCharArray(volumeNameBuffer);
            volumeInfo["filesystem"] = QString::fromWCharArray(fileSystemNameBuffer);
        }

        ULARGE_INTEGER totalNumberOfBytes;
        ULARGE_INTEGER totalNumberOfFreeBytes;

        if (GetDiskFreeSpaceExW(volumePath, nullptr, &totalNumberOfBytes, &totalNumberOfFreeBytes)) {
            volumeInfo["capacity"] = QString::number(totalNumberOfBytes.QuadPart);
            volumeInfo["freespace"] = QString::number(totalNumberOfFreeBytes.QuadPart);
        }

        if (volumeInfo.keys().count() != 0)
            volumeInfo["volumeName"] = volumeName;
    }

    return volumeInfo;
}
