//
// Created by magami@wincs.cs.bgu.ac.il on 25/11/2019.
//new

#ifndef REALSPLFLIX_ACTION_H
#define REALSPLFLIX_ACTION_H
#include <string>
#include <iostream>

class Session;

enum ActionStatus{
    PENDING, COMPLETED, ERROR
};
class BaseAction{
public:
    BaseAction(); //EMPTY CONSTRUCTOR
    BaseAction(std::string &firstWord,std::string &secWord);//CONSTRUCTOR
    virtual ~BaseAction(); //DESTRUCTOR
    virtual BaseAction* clone()=0;

    ActionStatus getStatus() const;
    std::string getFirstWord();
    std::string getSecWord();
    virtual void act(Session&)=0;
    virtual std::string toString() const=0;


protected:
    void complete();
    void error(const std::string& _errorMsg);
    std::string getErrorMsg() const;
private:
    ActionStatus status;
    std::string errorMsg;
    std::string firstWord;
    std::string secWord;

};

class CreateUser  : public BaseAction {
public:
    CreateUser(std::string &firstWord,std::string &secWord);
    virtual void act(Session&);
    virtual std::string toString() const;
    virtual CreateUser* clone();
};

class ChangeActiveUser : public BaseAction {
public:
    ChangeActiveUser(std::string &firstWord,std::string &secWord);
    virtual void act(Session& sess);
    virtual std::string toString() const;
    virtual ChangeActiveUser* clone();
};

class DeleteUser : public BaseAction {
public:
    DeleteUser(std::string &firstWord,std::string &secWord);
    virtual void act(Session & sess);
    virtual std::string toString() const;
    virtual DeleteUser* clone();
};


class DuplicateUser : public BaseAction {
public:
    DuplicateUser(std::string &firstWord,std::string &secWord);
    virtual void act(Session & sess);
    virtual std::string toString() const;
    virtual DuplicateUser* clone();
};

class PrintContentList : public BaseAction {
public:
    PrintContentList(std::string &firstWord,std::string &secWord);
    virtual void act (Session& sess);
    virtual std::string toString() const;
    virtual PrintContentList* clone();
};

class PrintWatchHistory : public BaseAction {
public:
    PrintWatchHistory(std::string &firstWord,std::string &secWord);
    virtual void act (Session& sess);
    virtual std::string toString() const;
    virtual PrintWatchHistory* clone();
};


class Watch : public BaseAction {
public:
    Watch(int id,std::string &_firstWord,std::string &_secWord);
    virtual void act (Session& sess);
    virtual std::string toString() const;
    virtual Watch* clone();
private:
    int id;
};


class PrintActionsLog : public BaseAction {
public:
    PrintActionsLog(std::string &firstWord,std::string &secWord);
    virtual void act(Session& sess);
    virtual std::string toString() const;
    virtual PrintActionsLog* clone();
};

class Exit : public BaseAction {
public:
    Exit(std::string &firstWord,std::string &secWord);
    virtual void act(Session& sess);
    virtual std::string toString() const;
    virtual Exit* clone();
};
#endif //REALSPLFLIX_ACTION_H