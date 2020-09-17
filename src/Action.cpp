//
// Created by naama on 21/11/2019.
//new

#include "../include/Action.h"
#include "../include/Session.h"
#include <iostream>
#include "../include/User.h"
#include "../include/Watchable.h"
#include <string>

class Session;
class Watchable;

BaseAction::BaseAction() :status (PENDING),errorMsg(""),firstWord("default"),secWord("") {}

BaseAction::BaseAction(std::string &_firstWord,std::string &_secWord):status (PENDING),errorMsg(""),firstWord("default"),secWord("") {
    firstWord = _firstWord;
    secWord= _secWord;
}

ActionStatus BaseAction::getStatus() const {
    return status;
}

std::string BaseAction::getFirstWord() {
    return firstWord;
}
std::string BaseAction::getSecWord() {
    return secWord;
}

void BaseAction::complete(){
    status=COMPLETED;
}
void BaseAction::error(const std::string& _errorMsg){
    status=ERROR;
    errorMsg=_errorMsg;
    std::cout << "ERROR - " << getErrorMsg() << std::endl ;
}
std::string BaseAction::getErrorMsg() const{
    return errorMsg;
}

BaseAction::~BaseAction() = default;

//CREATE USER

CreateUser::CreateUser(std::string &_firstWord,std::string &_secWord): BaseAction(_firstWord,_secWord){}

void CreateUser::act(Session &sess) {
    if (sess.SearchInMap(getFirstWord())){
        error("User name is already in use");
        sess.addActionToActionLog(this);
    }
    else if (this->getSecWord() == "len") {
        User* newUser = new LengthRecommenderUser(getFirstWord());
        sess.addUserToMap(newUser);
        complete();
        sess.addActionToActionLog(this);
    }
    else if (this->getSecWord()=="rer"){
        User* newUser = new RerunRecommenderUser(getFirstWord());
        sess.addUserToMap(newUser);
        complete();
        sess.addActionToActionLog(this);
    }
    else if (this->getSecWord()=="gen"){
        User* newUser = new GenreRecommenderUser(getFirstWord());
        sess.addUserToMap(newUser);
        complete();
        sess.addActionToActionLog(this);
    }
    else {
        error("recommendation algorithm can not be found");
        sess.addActionToActionLog(this);
    }
}
std::string CreateUser::toString() const{
    ActionStatus stat = getStatus();
    std::string ans;
    if (stat==ERROR){
        ans = "ERROR";
        return ("CreateUser "+ans+": "+getErrorMsg());
    }
    if (stat==COMPLETED)
        ans = "COMPLETED";
    if (stat==PENDING)
        ans = "PENDING";
    return ("Create user "+ans);

}

CreateUser* CreateUser::clone() {
    return new CreateUser(*this);
}

//CHANGE ACTIVE USER

ChangeActiveUser::ChangeActiveUser(std::string &_firstWord,std::string &_secWord): BaseAction(_firstWord,_secWord){}

void ChangeActiveUser::act(Session &sess) {
    std::string s = getFirstWord();
    bool isEx = sess.SearchInMap(s);
    if (isEx) {
        sess.setActiveUser(sess.getUserFromMap(s));
        complete();
        sess.addActionToActionLog(this);
    } else {
        error("User does not exist");
        sess.addActionToActionLog(this);
    }
}
std::string ChangeActiveUser::toString() const{
    ActionStatus stat = getStatus();
    std::string ans;
    if (stat==ERROR){
        ans = "ERROR";
        return ("ChangeActiveUser "+ans+": "+getErrorMsg());
    }
    if (stat==COMPLETED)
        ans = "COMPLETED";
    if (stat==PENDING)
        ans = "PENDING";
    return ("Change active user "+ans);
}

ChangeActiveUser* ChangeActiveUser::clone() {
    return new ChangeActiveUser(*this);
}
//DELETE USER

DeleteUser::DeleteUser(std::string &_firstWord,std::string &_secWord): BaseAction(_firstWord,_secWord){}

void DeleteUser::act(Session &sess) {
    std::string s = getFirstWord();
    bool isEx = sess.SearchInMap(s);
    if (isEx) {
        sess.removeUser(s);

        complete();
        sess.addActionToActionLog(this);
    } else {
        error("User does not exist");
        sess.addActionToActionLog(this);
    }
}

std::string DeleteUser::toString() const{
    ActionStatus stat = getStatus();
    std::string ans;
    if (stat==ERROR){
        ans = "ERROR";
        return ("DeleteUser "+ans+": "+getErrorMsg());
    }
    if (stat==COMPLETED)
        ans = "COMPLETED";
    if (stat==PENDING)
        ans = "PENDING";
    return ("Delete user "+ans);
}

DeleteUser* DeleteUser::clone() {
    return new DeleteUser(*this);
}
//DUPLICATE USER

DuplicateUser::DuplicateUser(std::string &_firstWord,std::string &_secWord): BaseAction(_firstWord,_secWord){}

void DuplicateUser::act(Session &sess) {
    std::string s = getFirstWord();
    bool isEx = sess.SearchInMap(s);
    if ((isEx) ){
        User* oldUser = sess.getUser(s);
        if (sess.SearchInMap(getSecWord())){
            error("Name is already in use");
            sess.addActionToActionLog(this);
        }else {
            User* newUser = (oldUser)->clone();
            newUser->setName(getSecWord());
            newUser->setHistory (oldUser->get_history());
            sess.addUserToMap(newUser);
            complete();
            sess.addActionToActionLog(this);
        }
    } else {
        error("User does not exist");
        sess.addActionToActionLog(this);
    }
}

std::string DuplicateUser::toString() const{
    ActionStatus stat = getStatus();
    std::string ans;
    if (stat==ERROR){
        ans = "ERROR";
        return ("DuplicateUser "+ans+": "+getErrorMsg());
    }
    if (stat==COMPLETED)
        ans = "COMPLETED";
    if (stat==PENDING)
        ans = "PENDING";
    return ("Duplicate user "+ans);
}

DuplicateUser* DuplicateUser::clone() {
    return new DuplicateUser(*this);
}
//PRINT CONTENT LIST

PrintContentList::PrintContentList(std::string &_firstWord,std::string &_secWord): BaseAction(_firstWord,_secWord){}

void PrintContentList::act(Session &sess) {
    sess.printCont();
    complete();
    sess.addActionToActionLog(this);
}

std::string PrintContentList::toString() const{
    ActionStatus stat = getStatus();
    std::string ans;
    if (stat==ERROR){
        ans = "ERROR";
        return ("PrintContentList "+ans+": "+getErrorMsg());
    }
    if (stat==COMPLETED)
        ans = "COMPLETED";
    if (stat==PENDING)
        ans = "PENDING";
    return ("Print content list "+ans);
}

PrintContentList* PrintContentList::clone() {
    return new PrintContentList(*this);
}

//PRINT WATCH HISTORY

PrintWatchHistory::PrintWatchHistory(std::string &_firstWord,std::string &_secWord): BaseAction(_firstWord,_secWord){}

void PrintWatchHistory::act(Session &sess) {

    User* active = sess.getActiveUser();
    active->printHistory();
    complete();
    sess.addActionToActionLog(this);
}

std::string PrintWatchHistory::toString() const{
    ActionStatus stat = getStatus();
    std::string ans;
    if (stat==ERROR){
        ans = "ERROR";
        return ("Print Watch History "+ans+": "+getErrorMsg());
    }
    if (stat==COMPLETED)
        ans = "COMPLETED";
    if (stat==PENDING)
        ans = "PENDING";
    return ("Print watch history "+ans);
}
PrintWatchHistory* PrintWatchHistory::clone() {
    return new PrintWatchHistory(*this);
}

//WATCH

Watch::Watch (int _id,std::string &_firstWord,std::string &_secWord) : BaseAction( _firstWord, _secWord) ,id(_id){}

void Watch::act(Session &sess) {
    Watchable *toWatch = sess.getFromContent(id);
    if (toWatch==nullptr) {
        error("Content does not exist\n");
        sess.addActionToActionLog(this);

    }else {
        printf("Watching %s \n", toWatch->toString().c_str());
        User* xx=sess.getActiveUser();
        xx->addToHistory(toWatch);
        complete();
        sess.addActionToActionLog(this);
    }
}

std::string Watch::toString() const{
    ActionStatus stat = getStatus();
    std::string ans;
    if (stat==ERROR) {
        ans = "ERROR";
        return ("Watch "+ans+":"+getErrorMsg());
    }
    if (stat==COMPLETED)
        ans = "COMPLETED";
    if (stat==PENDING)
        ans = "PENDING";
    return ("Watch "+ans);
}
Watch* Watch::clone() {
    return new Watch(*this);
}
//PRINT ACTIONS LOG

PrintActionsLog::PrintActionsLog(std::string &_firstWord,std::string &_secWord): BaseAction(_firstWord,_secWord){}

void PrintActionsLog::act(Session &sess) {
    sess.printActionsLog();
    complete();
    sess.addActionToActionLog(this);
}

std::string PrintActionsLog::toString() const{
    ActionStatus stat = getStatus();
    std::string ans;
    if (stat==ERROR) {
        ans = "ERROR";
        return ("PrintActionLog "+ans+":"+getErrorMsg());
    }
    if (stat==COMPLETED)
        ans = "COMPLETED";
    if (stat==PENDING)
        ans = "PENDING";
    return ("Print action log "+ans);
}
PrintActionsLog* PrintActionsLog::clone() {
    return new PrintActionsLog(*this);
}

//EXIT
Exit::Exit(std::string &_firstWord,std::string &_secWord): BaseAction(_firstWord,_secWord){}

void Exit::act(Session& sess){
    sess.setIsOn(false);
    complete();
    sess.addActionToActionLog(this);
}

std::string Exit::toString() const{
    ActionStatus stat = getStatus();
    std::string ans;
    if (stat==ERROR) {
        ans = "ERROR";
        return ("Exit "+ans+":"+getErrorMsg());
    }
    if (stat==COMPLETED)
        ans = "COMPLETED";
    if (stat==PENDING)
        ans = "PENDING";
    return ("Exit "+ans);
}
Exit* Exit::clone() {
    return new Exit(*this);
}