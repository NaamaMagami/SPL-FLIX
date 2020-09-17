//new
#include "../include/User.h"
#include <iostream>
#include "../include/Watchable.h"
#include "../include/Session.h"
#include <cmath>
#include <map>
#include <iterator>
#include <algorithm>

User::User (const std::string &_name) :history({}),name(_name),type("len"), averageLen(0), tagsMap({}){}

User::~User() =default;

void User::deleteUser() {
    history={};
    averageLen=0;
    tagsMap={};

}

std::string User::getName() const {
    return name;
}

void User::setName(std::string s) {
    name=s;
}

void User::setType(std::string t){
    type=t;
}

long User::getAverage() { return averageLen;}

void User::printHistory () {
    int i=1;
    for (Watchable* &w : history){
        std::cout << i<<". " <<w->toString() << std::endl;
        ++i;
    }
}

std::vector<Watchable*> User::get_history() const {
    return history;
}

void User::addToHistory(Watchable* w){
    //ADDING TO HISTORY
    history.push_back(w);
    int newLen=w->getLen();
    if (history.size()>1){
        int oldSum = averageLen*(history.size());
        averageLen=(oldSum+newLen)/(history.size()+1);
    }
    else{
        averageLen=newLen;
    }
    //ADDING TO TAGS HISTORY
    if(type=="gen") {
        std::map<int, std::string>::iterator it;
        for (std::string &tag : w->getTags()){
            tagsMap[tag]++;
        }
    }
}

void User::setHistory(std::vector<Watchable*> OtherHistory) {
    history = std::move(OtherHistory);
}

bool User::isInHistory(std::string s) {
    for (Watchable* w : history){
        std::string name = w->getName();
        if (name==s){
            return true;
        }
    }
    return false;
}

std::string User::getMaxTag(std::map<std::string,int> illigalTags) {
    int max = 0;
    std::string maxTag;
    for (auto tag: tagsMap){
        if ((tag.second>max) & (illigalTags.find(tag.first)==illigalTags.end())){//IF NOT IN ILLIGAL TAGS
            max=tag.second;
            maxTag=tag.first;
        }
    }

    return maxTag;
}

int User::tagsLen() {
    return tagsMap.size();
}



//BY LENGTH
LengthRecommenderUser::LengthRecommenderUser( const std::string &name) : User(name) {}

Watchable* LengthRecommenderUser::getRecommendation(Session& s){
    Watchable* curr;
    Watchable* output= nullptr;
    curr = history[history.size() - 1];
    if(curr->isEpisode()) {
        if (!curr->checkIsLast()) {
            return s.getFromContent(curr->getId() + 1);
        }
    }
    int min=INT32_MAX;

    for (Watchable* &cont: s.getContent()){
        if (isInHistory(cont->getName())==false){
            if ( std::abs(cont->getLen()- s.getActiveUser()->getAverage() ) < min) {
                min = abs(cont->getLen() - s.getActiveUser()->getAverage());
                output=cont;
            }
        }
    }
    return output;
}

LengthRecommenderUser* LengthRecommenderUser::clone() const {
    return new LengthRecommenderUser(*this);
}

//RE-RUN
RerunRecommenderUser::RerunRecommenderUser( const std::string &name) : User(name) ,lastRec(-1){
    setType("rer");
}

Watchable* RerunRecommenderUser::getRecommendation(Session& s){
    Watchable* curr;
    curr = history[history.size() - 1];
    if(curr->isEpisode() & (!curr->checkIsLast())) {
        return s.getFromContent(curr->getId() + 1);
    }

    lastRec=(lastRec+1)%(history.size());
    return history.at(lastRec);

}

RerunRecommenderUser* RerunRecommenderUser::clone() const{
    return new RerunRecommenderUser(*this);
}


//BY GENERE
GenreRecommenderUser::GenreRecommenderUser( const std::string &name) :User (name){
    setType("gen");
}
Watchable* GenreRecommenderUser::getRecommendation(Session& s) {
    Watchable *curr;
    curr = history[history.size() - 1];
    if (curr->isEpisode()) {
        if (!curr->checkIsLast()) {
            return s.getFromContent(curr->getId() + 1);
        }
    }
    int numOfTagsChecked=0;
    std::vector<std::string> availableContent;
    std::map <std::string, int> illigalTags;
    while (numOfTagsChecked<tagsLen()) {
        ++numOfTagsChecked;
        std::string popularT = getMaxTag(illigalTags);
        for (auto &w : s.getContent()) { //CREATING A VECTOR OF WATCHABLE CONTENT
            if (!isInHistory(w->getName())) {
                for (auto &tag : w->getTags()) {
                    if (tag == popularT) //ADDING ONLY CONTENT WITH THE POPULAR TAG
                        availableContent.push_back(w->getName());
                }
            }
        }
        if (availableContent.size() > 0) { // IF THERE IS A CONTENT TO WATCH WITH POPULAR TAG
            std::sort(availableContent.begin(), availableContent.end());
            return s.getFromContent(availableContent.at(0));
        }
        else{
            illigalTags.insert({popularT,1});
        }
    }
    return nullptr;
}

GenreRecommenderUser* GenreRecommenderUser::clone() const{
    return new GenreRecommenderUser(*this);
}