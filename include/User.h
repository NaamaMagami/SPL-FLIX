//
// Created by magami@wincs.cs.bgu.ac.il on 25/11/2019.
//new

#ifndef REALSPLFLIX_USER_H
#define REALSPLFLIX_USER_H
#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <map>
class Watchable;
class Session;

class User{
public:
    User(const std::string& name);
    virtual ~User();
    virtual User* clone() const=0;
    void deleteUser();
    std::string getName() const;
    void setName(std::string);
    void setType (std::string);
    long getAverage ();
    virtual Watchable* getRecommendation(Session& s) = 0;
    void printHistory();
    std::vector<Watchable*> get_history() const;
    void addToHistory(Watchable*);
    void setHistory(std::vector<Watchable*> );
    bool isInHistory(std::string);
    std::string getMaxTag(std::map<std::string,int>);
    int tagsLen();

protected:
    std::vector<Watchable*> history;
private:
    std::string name;
    std::string type;
    long averageLen;
    std::map <std::string, int>tagsMap;
};


class LengthRecommenderUser : public User {
public:
    LengthRecommenderUser( const std::string &name);
    virtual Watchable* getRecommendation(Session& s);
    virtual LengthRecommenderUser* clone() const;
private:

};

class RerunRecommenderUser : public User {
public:
    RerunRecommenderUser( const std::string& name);

    virtual Watchable* getRecommendation(Session& s);
    virtual RerunRecommenderUser* clone() const;
private:
    int lastRec;
};

class GenreRecommenderUser : public User {
public:
    GenreRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
    virtual GenreRecommenderUser* clone() const;

};


#endif //REALSPLFLIX_USER_H