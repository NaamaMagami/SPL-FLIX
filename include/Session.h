//
// Created by magami@wincs.cs.bgu.ac.il on 25/11/2019.
//new

#ifndef REALSPLFLIX_SESSION_H
#define REALSPLFLIX_SESSION_H

#include <vector>
#include <unordered_map>
#include <string>
#include "Action.h"

class User;
class Watchable;

class Session{
public:
    Session(const std::string &configFilePath); //CONSTRUCTOR
    Session(const Session&); //COPY CONSTRUCTOR
    Session& operator=(const Session&); //COPY ASS. OPERATOR
    Session( Session&&); //MOVE CONSTRUCTOR
    Session& operator=( Session&&);  //MOVE ASS. OPERATOR
    ~Session(); //DESSTRUCTOR
    void copy(const Session& s);
    void move(Session&);
    void clear();
    void start();
    //running fuctions
    void runSplflix();
    void commitAction(std::string);
    //user-related functions
    void addUserToMap(User *user);
    bool SearchInMap(std::string s);
    User* getUser(std::string name);
    User* getActiveUser();
    void setActiveUser(User*);
    void removeUser (std::string name);
    User* getUserFromMap(std::string);
    //actions log functions
    void addActionToActionLog (BaseAction *action);
    void printActionsLog();
    //content functions
    void printCont ();
    Watchable* getFromContent (int id);
    Watchable* getFromContent (std::string);
    std::vector<Watchable*> getContent();
    Watchable* nextWatchableByTag(std::string);
    //other functions
    void setIsOn(bool);
    bool isNum(std::string);
private:
    std::vector<Watchable*> content;
    std::vector<BaseAction*> actionsLog;
    std::unordered_map<std::string,User*> userMap;
    User* activeUser;
    bool isOn;

};
#endif //REALSPLFLIX_SESSION_H