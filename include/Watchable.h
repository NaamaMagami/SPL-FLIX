//
// Created by magami@wincs.cs.bgu.ac.il on 25/11/2019.
//new

#ifndef REALSPLFLIX_WATCHABLE_H
#define REALSPLFLIX_WATCHABLE_H
#include <string>
#include <vector>


class Session;

class Watchable{
public:
    Watchable(long id, int length, const std::vector<std::string>& tags);
    virtual ~Watchable();
    virtual Watchable* clone() const = 0;
    virtual std::string toString() const = 0;
    virtual Watchable* getNextWatchable(Session&) const = 0;
    long getId () const ;
    int getLen() const;
    virtual bool isEpisode () const =0;
    std::vector<std::string> getTags() const ;
    std::string TagsToString ();
    virtual std::string getName() const=0;
    virtual int getSeason() const=0;
    virtual int getEpisode() const=0;
    void setAsLast();
    bool checkIsLast();
private:
    const long id;
    int length;
    bool isLast;
    std::vector<std::string> tags;

};

class Movie : public Watchable{
public:
    Movie(long id, const std::string& name, int length, const std::vector<std::string>& tags);
    virtual Movie* clone() const;
    virtual std::string toString() const;
    virtual std::string getName() const;
    virtual Watchable* getNextWatchable(Session& sess) const;
    virtual bool isEpisode () const;
    virtual int getSeason() const ;
    virtual int getEpisode() const ;
private:
    std::string name;
    std::vector<std::string> MovieTags;

};


class Episode: public Watchable{
public:
    Episode(long id, const std::string& seriesName,int length, int season, int episode ,const std::vector<std::string>& tags);
    virtual Episode* clone() const ;
    virtual std::string toString() const;
    virtual std::string getName() const;
    virtual Watchable* getNextWatchable(Session&) const;
    virtual bool isEpisode () const;
    virtual int getSeason() const ;
    virtual int getEpisode() const ;
private:
    std::string seriesName;
    int season;
    int episode;

};
#endif //REALSPLFLIX_WATCHABLE_H

