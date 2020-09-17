

    #include "../include/Session.h"
    #include "../include/User.h"
    #include "../include/Watchable.h"
    #include <iostream>
    #include "../include/json.hpp"
    #include <unordered_map>
    #include <fstream>
    #include <sstream>



    using json = nlohmann::json;

    Session::Session(const std::string &configFilePath) :content({}),actionsLog({}),userMap({}),activeUser(nullptr),isOn("true"){

        User* defaultUser= new LengthRecommenderUser ("default");
        userMap["default"]=defaultUser;
        activeUser= defaultUser;

        std::ifstream i(configFilePath);
        json j;
        i >> j;

        json movies = j["movies"];
        int id = 1;
        int moviesSize =movies.size();
        for (int i = 0; i < moviesSize; ++i) {
            json movie = movies[i];
            std::string name = movie["name"];
            Watchable *temp = new Movie(id, movie["name"], movie["length"], movie["tags"]);
            content.push_back(temp);

            ++id;
        }
        json tvShows = j["tv_series"];
        int tvShowsSize = tvShows.size();
        for (int l = 0; l < tvShowsSize; ++l) { //TV-SHOWS LOOP
            json series = tvShows[l];
            int lastSeason = series["seasons"].size()-1;
            int numOfSeasons = series["seasons"].size();
            for (int k = 0; k < numOfSeasons; ++k) { // SEASONS LOOP
                int lasEpisodeInSeason = series["seasons"][k];
                int numOfEpisodes = series["seasons"][k];
                for (int n = 1; n <= numOfEpisodes; ++n) { //EPISODES LOOP
                    Watchable *tempEpisode = new Episode(id, series["name"], series["episode_length"], (k + 1), n,
                                                         series["tags"]);
                    if ((k==lastSeason) & (n==lasEpisodeInSeason))
                        tempEpisode->setAsLast();

                    content.push_back(tempEpisode);
                    ++id;
                }
            }
        }

    }

    //COPY CONSTRUCTOR
        Session::Session(const Session& s) : content({}),actionsLog({}),userMap({}),activeUser(nullptr),isOn(false){
            this->copy(s);
        }

        void Session::copy (const Session& s) {
            this->isOn = s.isOn;

            for (auto *w : s.content) {
                content.push_back(w->clone());
            }
            for (auto *a : s.actionsLog) {
                actionsLog.push_back(a->clone());
            }
            for (std::pair<std::string, User *> hisUser : s.userMap) { //ADDING THE USER ITSELF
                User *newUser = hisUser.second->clone();
                std::string newUserName = hisUser.first;
                userMap[newUserName] = newUser;
                newUser->setHistory({});

                std::vector<Watchable *> hisHistory = hisUser.second->get_history();
                for (Watchable *w : hisHistory) { //DEEP-COPPING THE HISTORY
                    int Wid = w->getId();
                    for (Watchable *newW : content) {
                        if (newW->getId() == Wid)
                            newUser->addToHistory(newW);
                    }
                }

            }
            activeUser = getUserFromMap(s.activeUser->getName());
        }
        Session::~Session() {
            clear();
        }

        void Session::clear() {
            int contSize= content.size();
            for(int i=0 ; i<contSize; ++i){
                delete(content.at(i));
            }
            int actionLogSize = actionsLog.size();
            for(int i=0; i<actionLogSize; ++i ){
                delete(actionsLog.at(i));
            }
            for (std::pair<std::string,User*> user : userMap){
                delete(user.second);
            }
            content.clear();
            actionsLog.clear();
            userMap.clear();
//            for(Watchable *w : content){
//                if(w!=nullptr){
//                    delete(w);
//                    w=nullptr;
//                }
//            }
//            for(BaseAction* action : actionsLog ){
//                if(action!=nullptr){
//                    delete(action);
//                    action=nullptr;
//                }
//            }
//            for (std::pair<std::string,User*> user : userMap){
//                if(user.second!=nullptr){
//                    delete(user.second);
//                    user.second = nullptr;
//                }
//            }
        }

    //COPY ASSIGNMENT OPERATOR (=)
        Session& Session::operator=(const Session& s){
            if (this==&s) //checking for self-assignment
                    return *this;
            //deleting the old resources
            clear();
            //coping the new resources
            this->copy(s);
            return *this;
        }
        void Session::start() {
            printf("SPLFLIX is now on!\n");
            setIsOn(true);
            runSplflix();
        }

    //MOVE CONSTRUCTOR
        Session::Session ( Session&& s): content({}),actionsLog({}),userMap({}),activeUser(nullptr),isOn(false){
            move(s);
        }

    //MOVE-ASSIGNMENT OPERATOR
        Session& Session::operator=(Session&& s){
        if (this!=&s){
            this->clear();
            move(s);
        }
        return *this;
        }

        void Session::move(Session& s){
            this->isOn = s.isOn;
            content = std::move(s.content);
            actionsLog = std::move(s.actionsLog);
            userMap = std::move(s.userMap);
            activeUser = getUserFromMap(s.activeUser->getName());
            return;
            for (auto &w : s.content) {
                content.push_back(w);
                w=nullptr;
            }
            for (auto &a : s.actionsLog){
                actionsLog.push_back(a);
                a=nullptr;
            }
            for (auto u = s.userMap.begin(); u!=s.userMap.end();++u){
                userMap.insert({u->first,u->second});
                u->second= nullptr;
            }
            activeUser= getUserFromMap(s.activeUser->getName());
        }
        void Session::setIsOn(bool b){ isOn=b; }

        void Session::runSplflix() {
            std::string actionToDo;
            if (isOn)
                std::cout << "Ready to go! please enter a command\n";
            while (isOn) {
                getline(std::cin, actionToDo);
                commitAction(actionToDo);
            }
        }
        bool Session::isNum(std::string s) {
            bool output=(s!="");
            int size =  s.length();
            for(int i=0 ; (output) & (i<size); i=i+1){
                if(!std::isdigit(s.at(i))){
                    output= false;
                }
            }
        return output;
    }
        void Session::commitAction(std::string actionToDo){
            std::string command="";
            std::string secondW="";
            std::string thirdW="";
            std::istringstream iss(actionToDo);
            std::getline(iss, command, ' ');
            std::getline(iss, secondW, ' ');
            std::getline(iss, thirdW, ' ');
            if(command=="createuser"){
                BaseAction* tempAc=new CreateUser(secondW,thirdW);
                tempAc->act(*this);
            }
            else if(command=="changeuser"){
                BaseAction* tempAc=new ChangeActiveUser(secondW,thirdW);
                tempAc->act(*this);
            }
            else if(command=="deleteuser"){
                BaseAction* tempAc=new DeleteUser(secondW,thirdW);
                tempAc->act(*this);
            }
            else if(command=="dupuser"){
                BaseAction* tempAc=new DuplicateUser(secondW,thirdW);
                tempAc->act(*this);
            }
            else if(command=="content"){
                BaseAction* tempAc=new PrintContentList(secondW,thirdW);
                tempAc->act(*this);
            }
            else if(command=="watchhist"){
                BaseAction* tempAc=new PrintWatchHistory(secondW,thirdW);
                std::cout << "Watch history of " << activeUser->getName() << std::endl ;
                tempAc->act(*this);
            }
            else if(command=="watch"){
                if(!isNum(secondW))
                    std::cout << "Please insert id  "<< std::endl ;
                else{
                    std::string continueWatch="y";
                    int x=std::stoi(secondW);
                    while(continueWatch=="y"){
                        BaseAction* tempAc=new Watch(x,secondW,thirdW);
                        tempAc->act(*this);
                        Watchable* w = getActiveUser()->getRecommendation(*this);
                        if(w== nullptr){
                            std::cout << "No recommendation available" <<std::endl;
                            continueWatch="n";
                        }
                        else{
                        std::cout << "We recommend Watching " ;
                        std::string s =w->toString().c_str();
                        std::cout <<  s << std::endl;
                        std::cout << "continue watching? [y/n]" << std::endl;
                        std::cin >>  continueWatch;
                        x=w->getId();
                    }}
                }
            }
            else if(command=="log"){
                BaseAction* tempAc= new PrintActionsLog (secondW,thirdW);
                tempAc->act(*this);

            }
            else if(command=="exit"){
                BaseAction* tempAc = new Exit(secondW,thirdW);
                tempAc->act(*this);

            }

            else
            {}
            command="";
            secondW="";
            thirdW="";
        }






    void Session::addUserToMap(User *user){
        std::string s = user->getName();
        userMap[s]=user;
    }

    bool Session::SearchInMap (std::string s) {
        std::unordered_map<std::string, User*>::iterator it;
        bool answer;
        it = userMap.find(s);
        if (it != userMap.end()) {
            answer= true;
        }
        else
        {
            answer = false;
        }
        return answer;

    }
    Watchable* Session::nextWatchableByTag(std::string s){
        Watchable* curr;
        int contSize = content.size();
        for (int i = 0; i < contSize ;i=i+1) {
            curr = content[i];
            if (!activeUser->isInHistory(curr->getName())) {
                int tagsSize = curr->getTags().size();
                for (int j = 0; j < tagsSize; j = j + 1) {
                    if((curr->getTags()[j])==s)
                        return curr;
                }

            }
        }
        return nullptr;
    }


    void Session::addActionToActionLog(BaseAction *action){
        actionsLog.push_back(action);
    }
    User* Session::getUserFromMap(std::string s){
      //  User* us = userMap[s];
        for(auto it =userMap.begin(); it != userMap.end(); ++it ){
            if(it->first == s) {
                User *u = it->second;
                return u;

            }
        }

        std::cout<<"user not found"<<std::endl;
        return nullptr;
    }
    void Session::setActiveUser(User* u){
        activeUser= u;
    }

    void Session::removeUser(std::string name){
//        std::unordered_map<std::string, User*>::iterator it;
//        it = userMap.find(name);
        userMap[name]->deleteUser();
        delete(userMap[name]);
        userMap.erase(name);
    }

    void Session::printCont (){
        int contSize = content.size();
        for(int i=0 ; i< contSize; i=i+1){
            std::cout << i+1<<"." ;
            std::cout << content[i]->toString() << " ";
            std::cout << content[i]->getLen() << " minutes" ;
            std::cout << content[i]->TagsToString() << std::endl;
        }
    }
    User* Session::getUser(std::string name){
        std::unordered_map<std::string, User*>::iterator it;
        it = userMap.find(name);
        User *user= (*it).second;
        return user;
    }

    User* Session::getActiveUser(){
        return activeUser;
    }

    Watchable* Session::getFromContent (int id){
        Watchable* toWatch= nullptr;
        int i=0;
        bool found=false;
        int contSize = content.size();
        while ( (i< contSize) & !found){
            int contIniID = content[i]->getId();
            if (contIniID==id)
                found=true;
            i=i+1;
        }
        if (found) {
            toWatch = content[i - 1];
            return toWatch;
        }
        return nullptr;
    }

    Watchable* Session::getFromContent (std::string s){
        for (Watchable* &w : content){
            if (w->getName() == s)
                return w;
        }
        return nullptr;
    }

    void Session::printActionsLog() {
        int actionLogSize = actionsLog.size()-1;
        for (int i = actionLogSize; i >=0 ; i = i - 1) {
            printf("%s\n", actionsLog[i]->toString().c_str());
        }
    }

    std::vector<Watchable*> Session::getContent(){
        return content;
    }