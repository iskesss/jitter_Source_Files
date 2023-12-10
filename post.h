#ifndef POST_H
#define POST_H

#include <iostream>

class Post{
public:
	Post();
	Post(std::size_t inputID, std::string inputMessage, int inputLikes);
	std::string virtual displayPost();
	std::size_t getID();
	std::string getMessage();
	int getLikes();

private:
	std::size_t id_;
	std::string message_;
	int likes_;

};

class incomingPost : public Post {
public:
	incomingPost();
	incomingPost(std::size_t inputID, std::string inputMessage, int inputLikes, bool inputIsPublic, std::string inputAuthor);
	std::string displayPost();

private:
	bool isPublic_;
	std::size_t author_;

};

#endif

