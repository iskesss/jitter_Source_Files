#ifndef NETWORK_H
#define NETWORK_H

#include "user.h"
#include <iostream>

class Network{
public:
	Network(); //CONVERT ALL ACCESSORS TO CONST!!

	void addUser(User * userIQ); //................................................................................................DONE!
	int addConnection(std::string s1, std::string s2); //..........................................................................DONE!
	int removeConnection(std::string s1, std::string s2); //.......................................................................DONE!
	std::size_t getID(std::string name); //........................................................................................DONE!
	User * getUser(std::size_t ID); //.............................................................................................DONE!
	int readUsers(std::string fname); //...........................................................................................DONE!
	int writeUsers(std::string fname); //..........................................................................................DONE!
	std::size_t numUsers(); //.....................................................................................................DONE!
	std::vector<std::size_t> shortestPath(std::size_t startID, std::size_t endID); //..............................................DONE!
	std::vector<std::size_t> distanceUser(std::size_t startID, std::size_t& endID, std::size_t distance); //.......................DONE!
	std::vector<std::size_t> suggestFriends(std::size_t who, std::size_t& score); //...............................................DONE!
	std::vector<std::vector<std::size_t> > groups(); //............................................................................DONE!
	void addPost(std::size_t ownerID, std::string message, int likes, bool isIncoming, std::size_t authorID, bool isPrivate); //...DONE!
	std::size_t numPosts(); //.....................................................................................................DONE!
	std::string displayPosts(std::string name, std::size_t howMany, bool showOnlyPublic); //.......................................DONE!
	int readPosts(std::string fname); //...........................................................................................DONE!

private:
	std::vector<User*> users_;
};


#endif
 