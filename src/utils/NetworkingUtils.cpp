//
// Created by werwolf2303 on 6/29/24.
//

#include "NetworkingUtils.h"

#include <iostream>
#include <string>
#include <QCoreApplication>
#include <QNetworkReply>
#include <QFile>
#include "../api/TruckersMPAPI.h"
#include <chrono>

QByteArray NetworkingUtils::makeGet(Endpoints server, const std::string &path,std::function<int(ErrorHandling::ErrorType errorType, std::string)> onError = nullptr) {
    try {
        QString craftedURL;
        switch (server) {
            case API:
                craftedURL.append(TruckersMPAPI::api_url().c_str());
            break;
            case DOWNLOAD:
                craftedURL.append(TruckersMPAPI::download_url().c_str());
            break;
            case UPDATE:
                craftedURL.append(TruckersMPAPI::update_url().c_str());
            break;
        }
        craftedURL.append(path.c_str());
        QEventLoop event_loop;
        QNetworkAccessManager manager;
        QObject::connect(&manager, SIGNAL(finished(QNetworkReply*)), &event_loop, SLOT(quit()));
        QNetworkRequest request;
        request.setUrl(craftedURL);
        QNetworkReply *reply = manager.get(request);
        event_loop.exec();
        if (reply->error() == QNetworkReply::NoError) {
            return reply->readAll();
        }
        if(onError != nullptr) onError(ErrorHandling::DESCRIPTIVE_API, reply->errorString().toStdString());
        return nullptr;
    }catch (...) {
        if(onError != nullptr) onError(ErrorHandling::GENERIC_API, "Exception thrown");
        return nullptr;
    }
}

bool NetworkingUtils::download(Endpoints server, const std::string &path, std::string downloadTo, std::function<int(ErrorHandling::ErrorType errorType, std::string)> onError, std::function<int(int maxProgress, int minProgress, int actualProgress, std::string progressDescription, std::string downloadSpeed)> onProgress) {
    try {
        QString craftedURL;
        switch (server) {
            case API:
                craftedURL.append(TruckersMPAPI::api_url().c_str());
            break;
            case DOWNLOAD:
                craftedURL.append(TruckersMPAPI::download_url().c_str());
            break;
            case UPDATE:
                craftedURL.append(TruckersMPAPI::update_url().c_str());
            break;
        }
        craftedURL.append(path.c_str());
        QFile *file = new QFile(QString(downloadTo.c_str()));
        if(!file->open(QIODevice::WriteOnly)) {
            onError(ErrorHandling::DESCRIPTIVE, "Failed to open file");
            return false;
        }
        QEventLoop event_loop;
        QNetworkAccessManager manager;
        QObject::connect(&manager, SIGNAL(finished(QNetworkReply*)), &event_loop, SLOT(quit()));
        QNetworkRequest request;
        request.setUrl(craftedURL);
        QNetworkReply *reply = manager.get(request);
        auto startTime = std::chrono::steady_clock::now();
        qint64 totalBytesReceived = 0;
        QObject::connect(reply, &QNetworkReply::readyRead, &event_loop, [file, reply, &totalBytesReceived]() {
            QByteArray data = reply->readAll();
            totalBytesReceived += data.size();
            file->write(data);
        });
        QObject::connect(reply, &QNetworkReply::downloadProgress, &event_loop, [onProgress, &totalBytesReceived, &startTime](qint64 bytesReceived, qint64 bytesTotal) {
            if (bytesTotal > 0) {
                int progress = static_cast<int>((bytesReceived * 100) / bytesTotal);
                auto currentTime = std::chrono::steady_clock::now();
                std::chrono::duration<double> elapsedSeconds = currentTime - startTime;
                double downloadSpeedMBps = (totalBytesReceived / 1048576.0) / elapsedSeconds.count();
                onProgress(100, 0, progress, "Downloading", std::to_string(downloadSpeedMBps));
            }
        });
        QObject::connect(reply, &QNetworkReply::finished, &event_loop, [file, reply, onError, onProgress, &totalBytesReceived, &startTime]() {
            if (reply->error() != QNetworkReply::NoError) {
                onError(ErrorHandling::DESCRIPTIVE, reply->errorString().toStdString());
            } else {
                auto endTime = std::chrono::steady_clock::now();
                std::chrono::duration<double> totalElapsedSeconds = endTime - startTime;
                double finalDownloadSpeedMBps = (totalBytesReceived / 1048576.0) / totalElapsedSeconds.count();
                onProgress(100, 0, 100, "Download complete", std::to_string(finalDownloadSpeedMBps));
            }
            file->close();
            file->deleteLater();
            reply->deleteLater();
        });
        event_loop.exec();
        if(reply->error() != QNetworkReply::NoError) onError(ErrorHandling::DESCRIPTIVE_API, reply->errorString().toStdString());
    }catch (...) {
        if(onError != nullptr) onError(ErrorHandling::GENERIC_API, "Exception thrown");
    }
}

