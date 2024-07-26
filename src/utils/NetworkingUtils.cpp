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

#include "FileUtils.h"
#include "../events/Events.h"

QByteArray NetworkingUtils::makeGet(Endpoints server, std::string path) {
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
        request.setRawHeader("User-Agent", "TruckersMP Launcher Unofficial/1.0 (Qt)");
        request.setUrl(craftedURL);
        QNetworkReply *reply = manager.get(request);
        event_loop.exec();
        if (reply->error() == QNetworkReply::NoError) {
            return reply->readAll();
        }
        Events::trigger(Events::onError, std::tuple<ErrorHandling::ErrorType, std::string>(ErrorHandling::DESCRIPTIVE, reply->errorString().toStdString()));
        delete reply;
        return nullptr;
    }catch (...) {
        Events::trigger(Events::onError, std::tuple<ErrorHandling::ErrorType, std::string>(ErrorHandling::GENERIC, "Exception thrown"));
        return nullptr;
    }
}

std::string NetworkingUtils::getLocation(std::string path) {
    try {
        QString craftedURL;
        craftedURL.append(path.c_str());
        QEventLoop event_loop;
        QNetworkAccessManager manager;
        QObject::connect(&manager, SIGNAL(finished(QNetworkReply*)), &event_loop, SLOT(quit()));
        QNetworkRequest request;
        request.setRawHeader("User-Agent", "TruckersMP Launcher Unofficial/1.0 (Qt)");
        request.setUrl(craftedURL);
        QNetworkReply *reply = manager.get(request);
        event_loop.exec();
        if (reply->error() == QNetworkReply::NoError) {
            return reply->rawHeader("location").toStdString();
        }
        Events::trigger(Events::onError, std::tuple<ErrorHandling::ErrorType, std::string>(ErrorHandling::DESCRIPTIVE, reply->errorString().toStdString()));
        delete reply;
        return nullptr;
    }catch (...) {
        Events::trigger(Events::onError, std::tuple<ErrorHandling::ErrorType, std::string>(ErrorHandling::GENERIC, "Exception thrown"));
        return nullptr;
    }
}


bool NetworkingUtils::download(Endpoints server, const std::string &path, std::string downloadTo) {
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
            Events::trigger(Events::onError, std::tuple<ErrorHandling::ErrorType, std::string>(ErrorHandling::DESCRIPTIVE, QString("Failed to open file: ").append(downloadTo.c_str()).toStdString()));
            return false;
        }
        QEventLoop event_loop;
        QNetworkAccessManager manager;
        QObject::connect(&manager, SIGNAL(finished(QNetworkReply*)), &event_loop, SLOT(quit()));
        QNetworkRequest request;
        request.setRawHeader("User-Agent", "TruckersMP Launcher Unofficial/1.0 (Qt)");
        if(server == GITHUB) {
            request.setUrl(QUrl(QString(getLocation(QString(path.c_str()).toStdString()).c_str())));
        }else {
            request.setUrl(craftedURL);
        }
        QNetworkReply *reply = manager.get(request);
        auto startTime = std::chrono::steady_clock::now();
        qint64 totalBytesReceived = 0;
        QObject::connect(reply, &QNetworkReply::readyRead, &event_loop, [file, reply, &totalBytesReceived]() {
            QByteArray data = reply->readAll();
            totalBytesReceived += data.size();
            file->write(data);
        });
        QObject::connect(reply, &QNetworkReply::downloadProgress, &event_loop, [&totalBytesReceived, &startTime, reply](qint64 bytesReceived, qint64 bytesTotal) {
            if (bytesTotal > 0) {
                int progress = static_cast<int>((bytesReceived * 100) / bytesTotal);
                auto currentTime = std::chrono::steady_clock::now();
                std::chrono::duration<double> elapsedSeconds = currentTime - startTime;
                double downloadSpeedMBps = (totalBytesReceived / 1048576.0) / elapsedSeconds.count();
                Events::trigger(Events::onProgressUpdate, std::tuple<int, int, int, std::string, std::string>(100, 0, progress, "Downloading ", std::to_string(downloadSpeedMBps)));
            }
        });
        QObject::connect(reply, &QNetworkReply::finished, &event_loop, [file, reply, &totalBytesReceived, &startTime]() {
            if (reply->error() != QNetworkReply::NoError) {
                Events::trigger(Events::onError, std::tuple<ErrorHandling::ErrorType, std::string>(ErrorHandling::DESCRIPTIVE, reply->errorString().toStdString()));
            } else {
                auto endTime = std::chrono::steady_clock::now();
                std::chrono::duration<double> totalElapsedSeconds = endTime - startTime;
                double finalDownloadSpeedMBps = (totalBytesReceived / 1048576.0) / totalElapsedSeconds.count();
                Events::trigger(Events::onProgressUpdate, std::tuple<int, int, int, std::string, std::string>(100, 0, 100, "Downloading ", std::to_string(finalDownloadSpeedMBps)));
            }
            file->close();
            file->deleteLater();
            reply->deleteLater();
        });
        event_loop.exec();
        if(reply->error() != QNetworkReply::NoError) {
            Events::trigger(Events::onError, std::tuple<ErrorHandling::ErrorType, std::string>(ErrorHandling::DESCRIPTIVE, reply->errorString().toStdString()));
            return false;
        }
        if(FileUtils::pathExists(downloadTo)) {
            if(std::filesystem::file_size(std::filesystem::path(downloadTo)) == totalBytesReceived) {
                return true;
            }
        }
        delete reply;
        return false;
    }catch (...) {
        Events::trigger(Events::onError, std::tuple<ErrorHandling::ErrorType, std::string>(ErrorHandling::GENERIC, "Exception thrown"));
        return false;
    }
}

