#ifndef USER_H
#define USER_H

#include <iostream>
#include <vector>
#include <set>

#include "post.h"

class User{
public:
	User();
	User(std::size_t inputID, std::string inputName, std::size_t inputYear, std::size_t inputZip, std::set<std::size_t> inputFriends);

	void addFriend(std::size_t otherID); //.....................................DONE!
	void deleteFriend(std::size_t otherID); //..................................DONE!
	std::size_t getID(); //.....................................................DONE!
	std::string getName(); //...................................................DONE!
	std::size_t getYear(); //...................................................DONE!
	std::size_t getZip(); //....................................................DONE!
	std::set<std::size_t> getFriends(); //......................................DONE!
	void addPost(Post* inputPostPtr); //........................................DONE!
	std::string displayPosts(std::size_t howMany, bool showOnlyPublic); //......DONE!

private:
	std::size_t id_;
	std::string name_;
	std::size_t year_;
	std::size_t zip_;
	std::set<std::size_t> friends_;
	std::vector<Post*> messages_;
};


#endif
 