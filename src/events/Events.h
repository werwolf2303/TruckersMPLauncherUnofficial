//
// Created by werwolf2303 on 7/26/24.
//

#ifndef EVENTS_H
#define EVENTS_H
#include <functional>
#include <any>

class Events {
public:
    enum EventsList {
        onError,
        onProgressUpdate
    };
    class Subscriber {
    public:
        int subscriberID;
        std::function<void(std::any)> subscriberRaw;
    };
    class Event {
    public:
        EventsList event;
        std::vector<Subscriber> subscribers;
    };
    static void registerEvent(EventsList event);
    static void unregisterEvent(EventsList event);
    static int subscribe(EventsList event, std::function<void(std::any)> runOnEvent);
    static void unsubscribe(EventsList event, int subscriberID);
    static void trigger(EventsList event, std::any data);
private:
    static std::vector<Event> events;
};



#endif //EVENTS_H
