	#include "post.h"

//default post constructor
//Pre: none
//Post: creates an empty post object
Post::Post() : id_(std::size_t()), message_(std::string()), likes_(int()) {}

//3 argument post constructor
//Pre: The user has provided relevant parameters to create a post object
//Post: A post object with all relevant information is created
Post::Post(std::size_t inputID, std::string inputMessage, int inputLikes) : id_(inputID), message_(inputMessage), likes_(inputLikes) {}

//displaypost function
//Pre: none
//Post: A string with the post's details is returned
std::string Post::displayPost(){ 
	return message_ + "\nLiked by " + std::to_string(likes_) + " people."; 
} 

//getID function
//Pre: none
//Post: The ID of the post in question is returned
std::size_t Post::getID(){ return id_; }

//getMessage function
//Pre: none
//Post: The message body of the post in question is returned
std::string Post::getMessage(){ return message_; }

//getLikes function
//Pre: none
//Post: The amount of likes on the post in question is returned
int Post::getLikes(){ return likes_; }

//default incomingPost constructor
//Pre: None.
//Post: creates an empty incomingPost object
incomingPost::incomingPost() : Post(){
	isPublic_ = bool();
	author_ = std::size_t();
}

//5 argument incomingPost constructor
//Pre: The user has provided relevant parameters to create an incomingPost object
//Post: An incomingPost object with all relevant information is created
incomingPost::incomingPost(std::size_t inputID, std::string inputMessage, int inputLikes, bool inputIsPublic, std::string inputAuthor) : Post(inputID, ("from " + inputAuthor + ": \n" + inputMessage), inputLikes){
	isPublic_ = inputIsPublic;
}

//displaypost function
//Pre: none
//Post: A string containing the public/private status and the message body of the post in question is returned
std::string incomingPost::displayPost(){
	if(isPublic_){
		return "Public message " + getMessage() + "\nLiked by " + std::to_string(getLikes()) + " people.";
	}
	else{
		return "Private message " + getMessage() + "\n";
	}
}