//new

#include "../include/Watchable.h"
#include "../include/Session.h"
#include "../include/User.h"
#include <iostream>
#include <string>

Watchable::Watchable(long _id, int _length, const std::vector<std::string>& _tags):id(_id),length(_length),isLast(false),tags(_tags){}


Watchable::~Watchable() = default ;

long Watchable::getId () const {
    return id;
}

int Watchable::getLen() const {
    return length;
}

std::vector<std::string> Watchable::getTags() const{
    return tags;
}

std::string Watchable::TagsToString (){
    std::string s = "[";
    int tagsSize = tags.size()-1;
    for (int i=0; i <tagsSize; ++i){
        s = s+tags.at(i)+", ";
    }
    s = s+tags.at(tagsSize)+"]";
    return s;
}

void Watchable::setAsLast(){
    isLast=true;
}

bool Watchable::checkIsLast(){
    return isLast;
}


//  MOVIE

Movie::Movie(long _id, const std::string& _name, int _length, const std::vector<std::string>& _tags): Watchable(_id, _length, _tags),name(_name),MovieTags(_tags){}

Movie *Movie::clone() const {
    return new Movie(*this);
}

std::string Movie::toString() const {
    return name;
}
std::string Movie::getName() const {
    return name;
}

Watchable* Movie::getNextWatchable(Session& s) const{
    return (s.getActiveUser()->getRecommendation(s));
}
bool Movie::isEpisode() const {
    return false;
}

int Movie::getSeason() const { return 0;}
int Movie::getEpisode() const { return 0;}


// EPISODE

Episode::Episode(long _id, const std::string& _seriesName,int _length, int _season, int _episode ,const std::vector<std::string>& _tags):Watchable(_id,_length,_tags),seriesName(_seriesName),season(_season),episode(_episode){}

Episode *Episode::clone() const {
    return new Episode(*this);
}

std::string Episode::toString() const {
    std::string s = seriesName+" S"+ std::to_string(season) +"E"+std::to_string(episode);
    return s;
}

std::string Episode::getName() const{
    return seriesName;
}

Watchable* Episode::getNextWatchable(Session& s) const{
    return (s.getActiveUser()->getRecommendation(s));
}

bool Episode::isEpisode() const {
    return true;
}

int Episode::getSeason() const{ return season;}
int Episode::getEpisode() const { return episode;}


