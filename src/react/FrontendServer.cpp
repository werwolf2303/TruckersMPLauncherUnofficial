//
// Created by werwolf2303 on 6/30/24.
//

#include "FrontendServer.h"
#ifdef DEBUG_HTML
#include "../utils/FileUtils.h"
#endif
#include "FrontendServer.h"

#include <QMessageBox>
#include <nlohmann/json.hpp>
#include "../base64/decode.h"
#include "../api/TruckersMPAPI.h"
#include <QSslKey>

bool FrontendServer::start() {
    try {
#ifdef DEBUG_HTML
        if(!FileUtils::pathExists("html")) {
            std::cout << "HTML folder doesn't exist in current directory! Please create it or compile the binary without -DDEBUG_HTML" << std::endl;
            exit(-1);
        }
#endif
        server.listen(QHostAddress("127.0.0.1"), 11232);
        return true;
    }catch (...) {
        return false;
    }
}

bool FrontendServer::init() {
    try {
        server.route("/events", [] (const QHttpServerRequest &request) {
            if(request.localAddress().toString() != "127.0.0.1") {
                QHttpServerResponse response(QHttpServerResponder::StatusCode::Unauthorized);
                return response;
            }
#ifdef DEBUG_HTML
            std::string eventsHTML = FileUtils::file_contents("html/events.html");
#else
            std::string eventsHTML =
            #include "../../resources/html/events.html"
                ;
#endif
            return QHttpServerResponse(QByteArray(eventsHTML.c_str()));
        });
        server.route("/settings", [] (const QHttpServerRequest &request) {
            if(request.localAddress().toString() != "127.0.0.1") {
                QHttpServerResponse response(QHttpServerResponder::StatusCode::Unauthorized);
                return response;
            }
#ifdef DEBUG_HTML
            std::string eventsHTML = FileUtils::file_contents("html/settings.html");
#else
            std::string eventsHTML =
            #include "../../resources/html/settings.html"
                ;
#endif
            return QHttpServerResponse(QByteArray(eventsHTML.c_str()));
        });
        server.route("/community", [] (const QHttpServerRequest &request) {
                    if(request.localAddress().toString() != "127.0.0.1") {
                        QHttpServerResponse response(QHttpServerResponder::StatusCode::Unauthorized);
                        return response;
                    }
#ifdef DEBUG_HTML
            std::string communityHTML = FileUtils::file_contents("html/community.html");
#else
            std::string communityHTML =
            #include "../../resources/html/community.html"
                ;
#endif
            return QHttpServerResponse(QByteArray(communityHTML.c_str()));
        });
        server.route("/serverstatus", [] (const QHttpServerRequest &request) {
            if(request.localAddress().toString() != "127.0.0.1") {
                QHttpServerResponse response(QHttpServerResponder::StatusCode::Unauthorized);
                return response;
            }
#ifdef DEBUG_HTML
            std::string serverstatusHTML = FileUtils::file_contents("html/serverStatus.html");
#else
            std::string serverstatusHTML =
            #include "../../resources/html/serverStatus.html"
                ;
#endif
            return QHttpServerResponse(QByteArray(serverstatusHTML.c_str()));
        });
        server.route("/versions", [] (const QHttpServerRequest &request) {
            if(request.localAddress().toString() != "127.0.0.1") {
                QHttpServerResponse response(QHttpServerResponder::StatusCode::Unauthorized);
                return response;
            }
#ifdef DEBUG_HTML
            std::string versionsHTML = FileUtils::file_contents("html/versions.html");
#else
            std::string versionsHTML =
            #include "../../resources/html/versions.html"
                ;
#endif
            return QHttpServerResponse(QByteArray(versionsHTML.c_str()));
        });
#ifdef NO_RELEASE
        server.route("/", [] (const QHttpServerRequest &request) {
#ifdef DEBUG_HTML
            std::string rootHTML = FileUtils::file_contents("html/root.html");
#else
                std::string rootHTML =
            #include "../../resources/html/root.html"
                ;
#endif
            return QHttpServerResponse(QByteArray(rootHTML.c_str()));
        });
#endif
        server.route("/serverStatus.js", [] (const QHttpServerRequest &request) {
            if(request.localAddress().toString() != "127.0.0.1") {
                QHttpServerResponse response(QHttpServerResponder::StatusCode::Unauthorized);
                return response;
            }
#ifdef DEBUG_HTML
            std::string serverStatusJS = FileUtils::file_contents("html/serverStatus.js");
#else
            std::string serverStatusJS =
            #include "../../resources/html/serverStatus.js"
                ;
#endif
            return QHttpServerResponse(QByteArray(serverStatusJS.c_str()));
        });
        server.route("/versions.js", [] (const QHttpServerRequest&request) {
            if(request.localAddress().toString() != "127.0.0.1") {
                QHttpServerResponse response(QHttpServerResponder::StatusCode::Unauthorized);
                return response;
            }
#ifdef DEBUG_HTML
            std::string versionsJS = FileUtils::file_contents("html/versions.js");
#else
            std::string versionsJS =
            #include "../../resources/html/versions.js"
                ;
#endif
            return QHttpServerResponse(QByteArray(versionsJS.c_str()));
        });
        server.route("/events.js", [] (const QHttpServerRequest &request) {
            if(request.localAddress().toString() != "127.0.0.1") {
                QHttpServerResponse response(QHttpServerResponder::StatusCode::Unauthorized);
                return response;
            }
#ifdef DEBUG_HTML
            std::string eventsJS = FileUtils::file_contents("html/events.js");
#else
            std::string eventsJS =
            #include "../../resources/html/events.js"
                ;
#endif
           return QHttpServerResponse(QByteArray(eventsJS.c_str()));
        });
        server.route("/community.js", [] (const QHttpServerRequest &request) {
            if(request.localAddress().toString() != "127.0.0.1") {
                QHttpServerResponse response(QHttpServerResponder::StatusCode::Unauthorized);
                return response;
            }
#ifdef DEBUG_HTML
            std::string communityJS = FileUtils::file_contents("html/community.js");
#else
            std::string communityJS =
            #include "../../resources/html/community.js"
                ;
#endif
            return QHttpServerResponse(QByteArray(communityJS.c_str()));
        });
        server.route("/bridge/getServers", [] (const QHttpServerRequest &request) {
            if(request.localAddress().toString() != "127.0.0.1") {
                QHttpServerResponse response(QHttpServerResponder::StatusCode::Unauthorized);
                return response;
            }
            QHttpServerResponse response(QByteArray(TruckersMPAPI::Servers::getAllServersRaw().c_str()));
            response.addHeader("Content-Type", "application/json");
            return response;
        });
        server.route("/settings.js", [] (const QHttpServerRequest &request) {
            if(request.localAddress().toString() != "127.0.0.1") {
                QHttpServerResponse response(QHttpServerResponder::StatusCode::Unauthorized);
                return response;
            }
#ifdef DEBUG_HTML
            std::string settingsJS = FileUtils::file_contents("html/settings.js");
#else
            std::string settingsJS =
            #include "../../resources/html/settings.js"
                ;
#endif
            return QHttpServerResponse(QByteArray(settingsJS.c_str()));
        });
        return true;
    }catch (...) {
        return false;
    }
}

std::vector<std::string> FrontendServer::split(std::string str, char delim) {
    std::vector<std::string> result;
    auto left = str.begin();
    for (auto it = left; it != str.end(); ++it)
    {
        if (*it == delim)
        {
            result.emplace_back(&*left, it - left);
            left = it + 1;
        }
    }
    if (left != str.end())
        result.emplace_back(&*left, str.end() - left);
    return result;
}



bool FrontendServer::stop() {
    try {
        server.deleteLater();
        return true;
    }catch (...) {
        return false;
    }
}