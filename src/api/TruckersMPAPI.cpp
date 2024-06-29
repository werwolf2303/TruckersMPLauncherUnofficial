//
// Created by werwolf2303 on 6/28/24.
//

#include "TruckersMPAPI.h"
#include <iostream>
#include <string>
#include <vector>
#include <QCoreApplication>
#include <QNetworkReply>
#include <QUrl>
#include <nlohmann/json.hpp>


std::vector<TruckersMPAPI::Servers::Server> TruckersMPAPI::Servers::getAllServers(std::function<int(ErrorType errorType, std::string)> onError = nullptr) {
    try {
        nlohmann::json json = nlohmann::json::parse(QString(makeGet(API, "/servers", onError)).toStdString());
        std::vector<TruckersMPAPI::Servers::Server> servers;
        for (auto &el: json["response"].items()) {
            nlohmann::json serverJson = el.value();
            Server server;
            if (serverJson.count("id") != 0) server.id = serverJson["id"].get<int>();
            if (serverJson.count("game") != 0) server.game = serverJson["game"].get<std::string>();
            if (serverJson.count("ip") != 0) server.ip = serverJson["ip"].get<std::string>();
            if (serverJson.count("port") != 0) server.port = serverJson["port"].get<int>();
            if (serverJson.count("name") != 0) server.name = serverJson["name"].get<std::string>();
            if (serverJson.count("shortname") != 0) server.shortname = serverJson["shortname"].get<std::string>();
            if (serverJson.count("idprefix") != 0) server.idprefix = serverJson["idprefix"].get<std::string>();
            if (serverJson.count("online") != 0) server.online = serverJson["online"].get<bool>();
            if (serverJson.count("players") != 0) server.players = serverJson["players"].get<int>();
            if (serverJson.count("queue") != 0) server.queue = serverJson["queue"].get<int>();
            if (serverJson.count("maxplayers") != 0) server.maxplayers = serverJson["maxplayers"].get<int>();
            if (serverJson.count("mapid") != 0) server.mapid = serverJson["mapid"].get<int>();
            if (serverJson.count("displayorder") != 0) server.displayorder = serverJson["displayorder"].get<int>();
            if (serverJson.count("speedlimiter") != 0) server.speedlimiter = serverJson["speedlimiter"].get<int>();
            if (serverJson.count("collisions") != 0) server.collisions = serverJson["collisions"].get<bool>();
            if (serverJson.count("carsforplayers") != 0) server.carsforplayers = serverJson["carsforplayers"].get<bool>();
            if (serverJson.count("policecarsforplayers") != 0) server.policecarsforplayers = serverJson["policecarsforplayers"].get<bool>();
            if (serverJson.count("afkenabled") != 0) server.afkenabled = serverJson["afkenabled"].get<bool>();
            if (serverJson.count("event") != 0) server.event = serverJson["event"].get<bool>();
            if (serverJson.count("specialEvent") != 0) server.specialEvent = serverJson["specialEvent"].get<bool>();
            if (serverJson.count("promods") != 0) server.promods = serverJson["promods"].get<bool>();
            servers.push_back(server);
        }
        return servers;
    }catch (...) {
        onError(GENERIC, "Exception thrown");
        return std::vector<TruckersMPAPI::Servers::Server>();
    }
}

int TruckersMPAPI::Servers::getGameTime(std::function<int(ErrorType errorType, std::string)> onError = nullptr) {
    try {
        nlohmann::json json = nlohmann::json::parse(QString(makeGet(API, "/game_time", onError)).toStdString());
        if (json.count("game_time") != 0) {
            return json["game_time"].get<int>();
        }
        return -1;
    }catch (...) {
        onError(GENERIC, "Exception thrown");
        return -1;
    }
}

TruckersMPAPI::Players::Player TruckersMPAPI::Players::getPlayer(int id, std::function<int(ErrorType errorType, std::string)> onError = nullptr) {
    try {
        nlohmann::json json = nlohmann::json::parse(QString(makeGet(API, "/player/" + id, onError)).toStdString());
        nlohmann::json serverJSON = json["response"];
        Player player;
        if (serverJSON.count("id") != 0) player.id = serverJSON["id"].get<int>();
        if (serverJSON.count("name") != 0) player.name = serverJSON["name"].get<std::string>();
        if (serverJSON.count("avatar") != 0) player.avatar = serverJSON["avatar"].get<std::string>();
        if (serverJSON.count("smallAvatar") != 0) player.smallAvatar = serverJSON["smallAvatar"].get<std::string>();
        if (serverJSON.count("joinDate") != 0) player.joinDate = serverJSON["joinDate"].get<std::string>();
        if (serverJSON.count("steamID64") != 0) player.steamID64 = serverJSON["steamID64"].get<int>();
        if (serverJSON.count("steamID") != 0) player.steamID = serverJSON["steamID"].get<std::string>();
        if (serverJSON.count("discordSnowflake") != 0) player.discordSnowflake = serverJSON["discordSnowflake"].get<std::string>();
        if (serverJSON.count("displayVTCHistory") != 0) player.displayVTCHistory = serverJSON["displayVTCHistory"].get<bool>();
        if (serverJSON.count("groupName") != 0) player.groupName = serverJSON["groupName"].get<std::string>();
        if (serverJSON.count("groupColor") != 0) player.groupColor = serverJSON["groupColor"].get<std::string>();
        if (serverJSON.count("groupID") != 0) player.groupID = serverJSON["groupID"].get<int>();
        if (serverJSON.count("banned") != 0) player.banned = serverJSON["banned"].get<bool>();
        if (serverJSON.count("bannedUntil") != 0) player.bannedUntil = serverJSON["bannedUntil"].get<std::string>();
        if (serverJSON.count("bansCount") != 0) player.bansCount = serverJSON["bansCount"].get<int>();
        if (serverJSON.count("displayBans") != 0) player.displayBans = serverJSON["displayBans"].get<bool>();
        PlayerPatreon patreon;
        if (serverJSON.count("patreon") != 0) {
            if (serverJSON["patreon"].count("isPatron") != 0) patreon.isPatron = serverJSON["patreon"]["isPatron"].get<bool>();
            if (serverJSON["patreon"].count("active") != 0) patreon.active = serverJSON["patreon"]["active"].get<bool>();
            if (serverJSON["patreon"].count("color") != 0) patreon.color = serverJSON["patreon"]["color"].get<std::string>();
            if (serverJSON["patreon"].count("tierId") != 0) patreon.tierId = serverJSON["patreon"]["tierId"].get<int>();
            if (serverJSON["patreon"].count("currentPledge") != 0) patreon.currentPledge = serverJSON["patreon"]["currentPledge"].get<int>();
            if (serverJSON["patreon"].count("lifetimePledge") != 0) patreon.lifetimePledge = serverJSON["patreon"]["lifetimePledge"].get<int>();
            if (serverJSON["patreon"].count("nextPledge") != 0) patreon.nextPledge = serverJSON["patreon"]["nextPledge"].get<int>();
            if (serverJSON["patreon"].count("hidden") != 0) patreon.hidden = serverJSON["patreon"]["hidden"].get<bool>();
        }
        player.patreon = patreon;
        PlayerPermissions permissions;
        if (serverJSON.count("permissions") != 0) {
            if (serverJSON["permissions"].count("isStaff") != 0) permissions.isStaff = serverJSON["permissions"]["isStaff"].get<bool>();
            if (serverJSON["permissions"].count("isUpperStaff") != 0) permissions.isUpperStaff = serverJSON["permissions"]["isUpperStaff"].get<bool>();
            if (serverJSON["permissions"].count("isGameAdmin") != 0) permissions.isGameAdmin = serverJSON["permissions"]["isGameAdmin"].get<bool>();
            if (serverJSON["permissions"].count("showDetailedOnWebMaps") != 0) permissions.showDetailedOnWebMaps = serverJSON["permissions"]["showDetailedOnWebMaps"].get<bool>();
        }
        player.permissions = permissions;
        PlayerVTC vtc;
        if (serverJSON.count("vtc") != 0) {
            if (serverJSON["vtc"].count("id") != 0) vtc.id = serverJSON["vtc"]["id"].get<int>();
            if (serverJSON["vtc"].count("name") != 0) vtc.name = serverJSON["vtc"]["name"].get<std::string>();
            if (serverJSON["vtc"].count("tag") != 0) vtc.tag = serverJSON["vtc"]["tag"].get<std::string>();
            if (serverJSON["vtc"].count("inVTC") != 0) vtc.inVTC = serverJSON["vtc"]["inVTC"].get<bool>();
            if (serverJSON["vtc"].count("memberID") != 0) vtc.memberID = serverJSON["vtc"]["memberID"].get<int>();
        }
        player.vtc = vtc;
        if (serverJSON.count("vtcHistory") != 0) {
            std::vector<PlayerVTCHistoryEntry> vtcHistoryEntries;
            for(auto &vtchele : serverJSON["vtcHistory"].items()) {
                nlohmann::json vtcEntryJSON = vtchele.value();
                PlayerVTCHistoryEntry entry;
                if (vtcEntryJSON.count("id") != 0) entry.id = vtcEntryJSON["id"].get<int>();
                if (vtcEntryJSON.count("name") != 0) entry.name = vtcEntryJSON["name"].get<std::string>();
                if (vtcEntryJSON.count("verified") != 0) entry.verified = vtcEntryJSON["verified"].get<bool>();
                if (vtcEntryJSON.count("joinDate") != 0) entry.joinDate = vtcEntryJSON["joinDate"].get<std::string>();
                if (vtcEntryJSON.count("leftDate") != 0) entry.leftDate = vtcEntryJSON["leftDate"].get<std::string>();
                vtcHistoryEntries.push_back(entry);
            }
            player.vtcHistory = vtcHistoryEntries;
        }
        return player;
    }catch (...) {
        onError(GENERIC, "Exception thrown");
        return Player();
    }
}

std::vector<TruckersMPAPI::Players::PlayerBanEntry> TruckersMPAPI::Players::getPlayerBans(int id, std::function<int(ErrorType errorType, std::string)> onError = nullptr) {
    try {
        nlohmann::json json = nlohmann::json::parse(QString(makeGet(API, "/bans/" + id, onError)).toStdString());
        std::vector<TruckersMPAPI::Players::PlayerBanEntry> banEntries;
        for(auto &bel : json["response"].items()) {
            nlohmann::json banEntryJSON = bel.value();
            PlayerBanEntry banEntry;
            if (banEntryJSON.count("expiration") != 0) banEntry.expiration = banEntryJSON["expiration"].get<std::string>();
            if (banEntryJSON.count("timeAdded") != 0) banEntry.timeAdded = banEntryJSON["timeAdded"].get<std::string>();
            if (banEntryJSON.count("active") != 0) banEntry.active = banEntryJSON["active"].get<bool>();
            if (banEntryJSON.count("reason") != 0) banEntry.reason = banEntryJSON["reason"].get<std::string>();
            if (banEntryJSON.count("adminName") != 0) banEntry.adminName = banEntryJSON["adminName"].get<std::string>();
            if (banEntryJSON.count("adminID") != 0) banEntry.adminID = banEntryJSON["adminID"].get<int>();
            banEntries.push_back(banEntry);
        }
        return banEntries;
    }catch(...) {
        onError(GENERIC, "Exception thrown");
        return std::vector<PlayerBanEntry>();
    }
}

std::string TruckersMPAPI::getSupportedGameVersionFor(GameTypes gameType, std::function<int(ErrorType errorType, std::string)> onError = nullptr) {
    try {
        nlohmann::json json = nlohmann::json::parse(QString(makeGet(API, "/v2/version", onError)).toStdString());
        if(gameType == ETS2) {
            return json["supported_game_version"].get<std::string>();
        }
        return json["supported_ats_game_version"].get<std::string>();
    }catch (...) {
        if(onError != nullptr) onError(GENERIC, "Exception thrown");
        return nullptr;
    }
}

std::string TruckersMPAPI::getTruckersMPAPIVersion(std::function<int(ErrorType errorType, std::string)> onError) {
    try {
        nlohmann::json json = nlohmann::json::parse(QString(makeGet(API, "/v2/version", onError)).toStdString());
        return json["name"].get<std::string>();
    }catch (...) {
        onError(GENERIC, "Exception thrown");
        return nullptr;
    }
}

std::string TruckersMPAPI::getTruckersMPAPIStage(std::function<int(ErrorType errorType, std::string)> onError) {
    try {
        nlohmann::json json = nlohmann::json::parse(QString(makeGet(API, "/v2/version", onError)).toStdString());
        return json["stage"].get<std::string>();
    }catch (...) {
        onError(GENERIC, "Exception thrown");
        return nullptr;
    }
}


QByteArray TruckersMPAPI::makeGet(Endpoints server, const std::string &path, std::function<int(ErrorType errorType, std::string)> onError = nullptr) {
    try {
        QString craftedURL;
        switch (server) {
            case API:
                craftedURL.append(api_url().c_str());
            break;
            case DOWNLOAD:
                craftedURL.append(download_url().c_str());
            break;
            case UPDATE:
                craftedURL.append(update_url().c_str());
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
        onError(DESCRIPTIVE_API, reply->errorString().toStdString());
        return nullptr;
    }catch (...) {
        onError(GENERIC_API, "Exception thrown");
        return nullptr;
    }
}

std::vector<TruckersMPAPI::Files::File> TruckersMPAPI::Files::getFiles(GameTypes gameType, std::function<int(ErrorType errorType, std::string)> onError = nullptr) {
    try {
        std::vector<File> files;
        nlohmann::json json = nlohmann::json::parse(QString(makeGet(UPDATE, "/files.json", onError)).toStdString());
        for(auto fileJSON : json["Files"].items()) {
            File file;
            if(fileJSON.value()["Type"] != "system") {
                if(gameType == ETS2) {
                    if(fileJSON.value()["Type"] != "ets2")
                        continue;
                }
                if(gameType == ATS) {
                    if(fileJSON.value()["Type"] != "ats")
                        continue;
                }
            }
            file.filePath = fileJSON.value()["FilePath"];
            file.type = fileJSON.value()["Type"];
            file.md5 = fileJSON.value()["Md5"];
            files.push_back(file);
        }
        return files;
    }catch (...) {
        onError(GENERIC, "Exception thrown");
        return std::vector<File>();
    }
}

