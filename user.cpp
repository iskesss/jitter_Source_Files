#include "user.h"
#include <vector>

//default constructor
//pre: none
//post: creates an empty user object
User::User() : id_(0), name_("null"), year_(0), zip_(0) {}

//5 argument constructor 
//pre: The user has provided relevant parameters to create a User object
//post: A user object with all relevant information is created
User::User(std::size_t inputID, std::string inputName, std::size_t inputYear, std::size_t inputZip, std::set<std::size_t> inputFriends) : id_(inputID), name_(inputName), year_(inputYear), zip_(inputZip), friends_(inputFriends.begin(),inputFriends.end()) {}

//addFriend function 
//pre: Another user exists
//post: The other user has been added to the calling user's friends list (in this case a set)
void User::addFriend(std::size_t otherID){
	if(id_ == otherID){
		std::cout << "Error: User '" << id_ << "' cannot add themselves as a friend. Props for self love though!" << std::endl; 
		return;
	}
	std::pair<std::set<std::size_t>::iterator,bool> insertionResults = friends_.insert(otherID); // The set class' .insert() function returns a pair which describes whether the insertion was successful. We need to store the pair's second element (a boolean value) as below it'll let us know if we need to inform the user that the insertion was unsuccessful.
	if(!insertionResults.second){
		std::cout << "Error: Friend request between user '" << id_ << "' and '" << otherID << "' failed! Were they already friends?" << std::endl;
	}
}

//deleteFriend function 
//pre: Another user exists and has been friended by the calling user
//post: The other user has been removed from the calling user's friends list (in this case a set)
void User::deleteFriend(std::size_t otherID){
	if(id_ == otherID){
		std::cout << "Error: User '" << id_ << "' cannot remove themselves as a friend because they were never friends with themself." << std::endl; 
		return;
	}
	if(!friends_.erase(otherID)){
		std::cout << "Error: Unfriend request between user '" << id_ << "' and '" << otherID << "' failed! Were they friends to begin with?" << std::endl;
	}
}

//getID function
//Pre: A user exists
//Post: The ID of the calling user is returned
std::size_t User::getID(){ return id_; }

//getName function
//Pre: A user exists
//Post: The full name of the calling user is returned
std::string User::getName(){ return name_; }

//getYear function
//Pre: A user exists
//Post: The year of the calling user is returned
std::size_t User::getYear(){ return year_; }

//getZip function
//Pre: A user exists
//Post: The zip code of the calling user is returned
std::size_t User::getZip(){ return zip_; }


//getFriends function
//Pre: A user exists (who presumably has at least one friend added)
//Post: A copied set of the calling user's friends is returned 
std::set<std::size_t> User::getFriends(){
	std::set<std::size_t> tempFriends = friends_;
	return tempFriends;
}

//addPost function
//Pre: At least one user exists
//Post: The user in question now possesses a post.
void User::addPost(Post* inputPostPtr){
	messages_.push_back(inputPostPtr);
}

//displayPosts function
//Pre: at least one user with at least one post exists
//Post: 'howMany' posts of the user in question are returned as one string
std::string User::displayPosts(std::size_t howMany, bool showOnlyPublic){
	std::string output;

	//std::cout << getName() << " has " << messages_.size() << " posts!" << std::endl;

	if(howMany > messages_.size()){ // if howMany exceeds the number of posts the user in question actually has, then simply print all of their posts using a range-based for loop (to prevent segfaults)
		for(auto currPost : messages_){
			if(!(showOnlyPublic && currPost->displayPost().substr(0,7) == "Private")){
				output += currPost->displayPost() + "\n\n";
			}
		}
		return output;
	}

	for(std::size_t i = 0; i < howMany; ++i){
		if(!(showOnlyPublic && messages_[i]->displayPost().substr(0,7) == "Private")){
			output += messages_[i]->displayPost() + "\n\n";
		}
	}
	return output;
}

