//
// Created by werwolf2303 on 7/26/24.
//

#include "Events.h"

#include <algorithm>
#include<any>

std::vector<Events::Event> Events::events;

int Events::subscribe(EventsList eventType, std::function<void(std::any)> runOnEvent) {
   int subscriberID = -1;
   for(Event& event :events) {
      if(event.event == eventType) {
         Subscriber subscriber;
         subscriber.subscriberID = static_cast<int>(event.subscribers.size()) + 1;
         subscriber.subscriberRaw = runOnEvent;
         subscriberID = subscriber.subscriberID;
         event.subscribers.push_back(subscriber);
      }
   }
   return subscriberID;
}

void Events::registerEvent(EventsList event) {
   Event evt;
   evt.event = event;
   events.push_back(evt);
}

void Events::unregisterEvent(EventsList event) {
   auto it = std::remove_if(events.begin(), events.end(),[event](const Event& e) { return e.event == event; });
   events.erase(it, events.end());
}

void Events::trigger(EventsList event, std::any data) {
   for(Event evt : events) {
      if(evt.event == event) {
         for(Subscriber subscriber : evt.subscribers) {
            subscriber.subscriberRaw(data);
         }
         break;
      }
   }
}

void Events::unsubscribe(EventsList event, int subscriberID) {
   for (Event& evt : events) {
      if (evt.event == event) {
         auto it = std::remove_if(evt.subscribers.begin(), evt.subscribers.end(),[subscriberID](const Subscriber& s) { return s.subscriberID == subscriberID; });
         evt.subscribers.erase(it, evt.subscribers.end());
         break;
      }
   }
}




