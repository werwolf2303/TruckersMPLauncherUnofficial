//
// Created by werwolf2303 on 6/28/24.
//

#ifndef TruckersMPAPI_H
#define TruckersMPAPI_H

#include <functional>
#include <QByteArray>
#include <string>
#include <vector>

#include "../utils/ErrorHandling.h"

class TruckersMPAPI {
public:
    static std::string api_url() {
        return "https://api.truckersmp.com";
    };
    static std::string download_url() {
        return "https://download-new.ets2mp.com";
    }
    static std::string update_url() {
        return "https://update.ets2mp.com";
    }

    class Players {
    public:
        struct PlayerPatreon {
            bool isPatron;
            bool active;
            std::string color;
            int tierId;
            int currentPledge;
            int lifetimePledge;
            int nextPledge;
            bool hidden;
        };

        struct PlayerPermissions {
            bool isStaff;
            bool isUpperStaff;
            bool isGameAdmin;
            bool showDetailedOnWebMaps;
        };

        struct PlayerVTC {
            int id;
            std::string name;
            std::string tag;
            bool inVTC;
            int memberID;
        };

        struct PlayerVTCHistoryEntry {
            int id;
            std::string name;
            bool verified;
            std::string joinDate;
            std::string leftDate;
        };

        struct Player {
            int id;
            std::string name;
            std::string avatar;
            std::string smallAvatar;
            std::string joinDate;
            int steamID64;
            std::string steamID;
            std::string discordSnowflake;
            bool displayVTCHistory;
            std::string groupName;
            std::string groupColor;
            int groupID;
            bool banned;
            std::string bannedUntil;
            int bansCount;
            bool displayBans;
            PlayerPatreon patreon;
            PlayerPermissions permissions;
            PlayerVTC vtc;
            std::vector<PlayerVTCHistoryEntry> vtcHistory;
        };

        struct PlayerBanEntry {
            std::string expiration;
            std::string timeAdded;
            bool active;
            std::string reason;
            std::string adminName;
            int adminID;
        };

        static Player getPlayer(int id);
        static std::vector<PlayerBanEntry> getPlayerBans(int id);
    };

    class Servers {
    public:
        struct Server {
            int id;
            std::string game;
            std::string ip;
            int port;
            std::string name;
            std::string shortname;
            std::string idprefix;
            bool online;
            int players;
            int queue;
            int maxplayers;
            int mapid;
            int displayorder;
            int speedlimiter;
            bool collisions;
            bool carsforplayers;
            bool policecarsforplayers;
            bool afkenabled;
            bool event;
            bool specialEvent;
            bool promods;
            int syncdelay;
        };

        static std::vector<Server> getAllServers();
        static std::string getAllServersRaw();
        static int getGameTime();
    };

    enum GameTypes {
        ETS2,
        ATS
    };

    class Files {
    public:
        struct File {
            std::string filePath;
            std::string type;
            std::string md5;
        };

        static std::vector<File> getFiles(GameTypes gameType);
    };

    static std::string getSupportedGameVersionFor(GameTypes gameType);
    static std::string getTruckersMPAPIVersion();
    static std::string getTruckersMPAPIStage();
};

#endif //TruckersMPAPI_H
