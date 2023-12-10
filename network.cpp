#include "network.h"
#include <queue>
#include <stack>
#include <sstream>
#include <fstream>

//Zero-arg network constructor
//Pre: None
//Post: An empty network object is created
Network::Network(){}

//addUser function
//Pre: A user object exists
//Post: A user object has been added to the network's private vector
void Network::addUser(User * userIQ){
	users_.push_back(userIQ);
}

//numUsers function
//Pre: none
//Post: The amount of users present in the network's private vector is returned
std::size_t Network::numUsers(){
	return users_.size();
}

// getID function
//Pre: A user object exists and is present in the network's private vector, and we have their full name
//Post: The ID of the user in question is returned
std::size_t Network::getID(std::string name){
	for(auto currUser : users_){
		if(currUser->getName() == name){
			return currUser->getID();
		}
	}
	std::cout << "Network error: user not found!" << std::endl;
	return -1;
}

// getID function
//Pre: A user object exists and is present in the network's private vector, and we have their ID number
//Post: A pointer to the user in question is returned
User * Network::getUser(std::size_t ID){
	for(auto currUser : users_){
		if(currUser->getID() == ID){
			return currUser;
		}
	}
	std::cout << "Network error: user not found!" << std::endl;
	return nullptr;
}

// addConnection function
//Pre: At least two user objects exist and are present in the network's private vector, and they aren't already friends
//Post: The two users in question have mutually been added to each other's private friends lists
int Network::addConnection(std::string s1, std::string s2){
	std::size_t ID1, ID2;
	User *U1, *U2;
	ID1 = getID(s1);
	ID2 = getID(s2);
	U1 = getUser(ID1);
	U2 = getUser(ID2);
	if(!( U1 == nullptr || U2 == nullptr)){
		U1->addFriend(ID2);
		U2->addFriend(ID1);
		return 0;
	}
	return -1;
}

//removeConnection function
//Pre: At least two user objects exist and are present in the network's private vector, and they're already friends
//Post: The two users in question have mutually been removed from each other's private friends lists
int Network::removeConnection(std::string s1, std::string s2){
	std::size_t ID1, ID2;
	User *U1, *U2;
	ID1 = getID(s1);
	ID2 = getID(s2);
	U1 = getUser(ID1);
	U2 = getUser(ID2);
	if(!( U1 == nullptr || U2 == nullptr)){
		U1->deleteFriend(ID2);
		U2->deleteFriend(ID1);
		return 0;
	}
	return -1;
}

int Network::readUsers(std::string fname){
	std::ifstream file;
	file.open(fname);
	if (!file.is_open()) {
		return -1; // file not open or not found
	}

	int n;
	file >> n;
	for (int i = 0; i < n; i ++){
		std::size_t id;
		std::string first_name, last_name, name;
		int year;
		int zip;
		std::set<std::size_t> friends;
		friends.clear();

		file >> id;
		file >> first_name >> last_name;
		file >> year;
		file >> zip;
		name = first_name + " " + last_name;

		std::string line;
		std::getline(file, line); // read in the \n
		std::getline(file, line);
		std::stringstream ss(line);

		std::size_t id2;
		while(ss >> id2){
		friends.insert(id2);
		}

		User* u = new User(id, name, year, zip, friends);
		addUser(u);
	}
	return 0;
}

//writeUsers function
//Pre: None.
//Post: A local file has been populated with network and user information in this assignment's proprietary format.
int Network::writeUsers(std::string fname){
	std::ofstream outFile(fname);
	if(!outFile.is_open()){
		std::cout << "Network error: writeUsers function could not open file." << std::endl;
		return -1;
	}
	outFile << numUsers() << std::endl;
	for(auto currUser : users_){
		outFile << currUser->getID() << std::endl;
		outFile << "\t" << currUser->getName() << std::endl;
		outFile << "\t" << currUser->getYear() << std::endl;
		if(std::to_string(currUser->getZip()).length() < 5){
			outFile << "\t" << "0" << currUser->getZip() << std::endl;
		}
		else{
			outFile << "\t" << currUser->getZip() << std::endl;
		}
		outFile << "\t";
		std::set<std::size_t> tempFriends = currUser->getFriends();
		for(auto e : tempFriends){
			outFile << e << " ";
		}
		outFile << std::endl;
	}
	outFile.close();
	return 0;
}

//shortestPath function
//Pre: None.
//Post: A vector containing the shortest path between startID and endID is returned.
std::vector<std::size_t> Network::shortestPath(std::size_t startID, std::size_t endID){
	std::vector<std::size_t> outVec;
	std::vector<std::pair<std::size_t, std::size_t> > backtrackVec;
	std::queue<std::size_t> q;

	bool visited[numUsers()]; //using an array to store a record of whether nodes have been visited or not, as there's no need for dynamic allocation
	for(std::size_t i = 0; i < numUsers(); ++i){
		visited[i] = false;
	}

	q.push(startID);

	while(q.size() > 0){
		size_t currID = q.front();
		q.pop();

		if(visited[currID]){ continue; }
		visited[currID] = true;

		if(currID == endID){ // if a match is found, we must backtrack from each node to its parent back to the root to find the true shortest path. this will be stored in outVec and returned
			size_t currChildID = endID;
			while (currChildID != startID){
				outVec.push_back(currChildID);
				for (auto backtrack : backtrackVec){ // find the previous node in the path by searching the backtrackVec!
					if (backtrack.second == currChildID){
						currChildID = backtrack.first;
						break;
					}
				}
			}
			outVec.push_back(startID); // add the startID to the traced path. cherry on the cake!
			std::reverse(outVec.begin(), outVec.end()); // reverse the path so that it starts from startID and ends at endID.
			return outVec; 
		}

		User * currUserPtr = getUser(currID);
		std::set<std::size_t> currFriendsList = currUserPtr->getFriends();

		for(auto currFriendID : currFriendsList){
			q.push(currFriendID);
			backtrackVec.push_back(std::make_pair(currID,currFriendID));
		}
		
	}
	return outVec;
}

//distanceUser function
//Pre: None.
//Post: Assuming some friend at the specified radius exists, the path to them is returned as a vector.
std::vector<std::size_t> Network::distanceUser(std::size_t startID, std::size_t& endID, std::size_t distance){
	std::vector<std::size_t> outVec;
	std::vector<std::pair<std::size_t, std::size_t> > backtrackVec;
	std::stack<std::size_t> stak;

	bool visited[numUsers()]; //using an array to store a record of whether nodes have been visited or not, as there's no need for dynamic allocation
	for(std::size_t i = 0; i < numUsers(); ++i){
		visited[i] = false;
	}

	stak.push(startID);

	while(stak.size() > 0){
		size_t currID = stak.top();
		stak.pop();

		if(visited[currID]){ continue; }
		visited[currID] = true;

		size_t currChildID = currID;
		size_t counter = 0;
		while(currChildID != startID){
			for (auto backtrack : backtrackVec){ // retrace our steps to determine whether the current currID is far away enough from startID
				if (backtrack.second == currChildID) {
					currChildID = backtrack.first;
					++counter;
					break;
				}
			}
		}
		if(counter == distance){ // we've officially found some currID the proper distance from startID! Now we need to retrace our steps again, this time actually storing the path in outVec.
			currChildID = currID;
			while(currChildID != startID){
				outVec.push_back(currChildID);
				for (auto backtrack : backtrackVec){
					if (backtrack.second == currChildID) {
						currChildID = backtrack.first;
						break;
					}
				}
			}
			outVec.push_back(startID);
			std::reverse(outVec.begin(), outVec.end()); // reverse the path so that it starts from startID and ends at endID.
			endID = currID; // we can update this value in a broader scope because it was passed by reference
			return outVec;
		}

		User * currUserPtr = getUser(currID);
		std::set<std::size_t> currFriendsList = currUserPtr->getFriends();

		for(auto currFriendID : currFriendsList){
			stak.push(currFriendID);
			backtrackVec.push_back(std::make_pair(currID,currFriendID));
		}

	}
	return outVec;
}

//suggestFriends function
//Pre: None.
//Post: A vector of suggested friends is returned. All have the same score.
std::vector<std::size_t> Network::suggestFriends(std::size_t who, std::size_t& score){
	size_t userScores[numUsers()]; //using an array to store each user's score (index indicates ID num), as there's no need for dynamic allocation
	for(std::size_t i = 0; i < numUsers(); ++i){
		userScores[i] = 0;
	}

	std::set<size_t> whoFriends = getUser(who)->getFriends();
	for(auto e : whoFriends){
		userScores[e] = -1;
	}

	for(auto e : whoFriends){
		std::set<size_t> whoFriendsFriends = getUser(e)->getFriends();
		for(auto c : whoFriendsFriends){
			if((userScores[c] == 0 && c != who)){
				std::set<size_t> whoFriendsFriendsFriends = getUser(c)->getFriends();
				for(auto d : whoFriendsFriendsFriends){
					if(userScores[d] == -1){
						++userScores[c];
					}
				}
			}
		}
	}

	std::size_t allTimeHighestScore = 0;

	for(std::size_t i = 0; i < numUsers(); i++){
		if((userScores[i] > allTimeHighestScore) && (userScores[i] != -1)){
			allTimeHighestScore = userScores[i];
		}
	}

	std::vector<size_t> highestScoringCandidates;
	if(allTimeHighestScore == 0){
		score = 0;
		return highestScoringCandidates;
	}
	score = allTimeHighestScore;
	
	
	for(std::size_t i = 0; i < numUsers(); i++){
		if(userScores[i] == allTimeHighestScore){
			highestScoringCandidates.push_back(i);
		}
	}
	return highestScoringCandidates;
}

//groups function
//Pre: None
//Post: Returns a vector of vectors of users representing a list of discrete subnetworks within the main network.
std::vector<std::vector<std::size_t> > Network::groups(){
	std::vector<std::vector<std::size_t> > outMatrix;
	bool allUsersVisited = false;
	bool visited[numUsers()]; //using an array to store a record of which users have been visited as there's no need for dynamic allocation
	for(std::size_t i = 0; i < numUsers(); ++i){
		visited[i] = false;
	}

	while(!allUsersVisited){
		std::size_t newNetworkRoot = 0; //this will iterate over the visited array, assuming the value of every userID at some point.
		while((visited[newNetworkRoot] == true) && (newNetworkRoot < numUsers())){ //newNetworkRoot will stop at the first user that hasn't been visited yet. i.e. the root of a new network
			++newNetworkRoot;
		}
		if(newNetworkRoot == numUsers()){ // then the function has successfully visited every user in the network!
            allUsersVisited = true;
			return outMatrix;
		}
		visited[newNetworkRoot] = true;
		//std::cout << "newNetworkRoot updated to " << newNetworkRoot << std::endl;

		std::vector<std::size_t> currNetwork;
		std::stack<std::size_t> stak;

		stak.push(newNetworkRoot);
		currNetwork.push_back(newNetworkRoot);
		while(!stak.empty()){
			std::size_t currUserID = stak.top();
			stak.pop();
			visited[currUserID] = true;
			std::set<std::size_t> currFriendsList = getUser(currUserID)->getFriends();
			for(auto e : currFriendsList){
				if(!visited[e]){
					visited[e] = true;
					stak.push(e);
					currNetwork.push_back(e);
				}
			}
		}
		outMatrix.push_back(currNetwork);
	}
	return outMatrix;
}

std::size_t Network::numPosts(){
	std::ifstream inFile("Posts.txt");
	std::size_t numPosts;
	inFile >> numPosts;

	inFile.close();
	return numPosts;
}


//addPost function
//Pre: A network with at least one user exists
//Post: The user in question now possesses a post.
void Network::addPost(std::size_t ownerID, std::string message, int likes, bool isIncoming, std::size_t authorID, bool isPrivate){
	if(isIncoming){
		Post * currPost = new incomingPost(numPosts(),message,likes,isPrivate,getUser(authorID)->getName());
		getUser(ownerID)->addPost(currPost);
	}
	else{
		Post * currPost = new Post(numPosts(),message,likes);
		getUser(ownerID)->addPost(currPost);
	}	
}

//displayPosts function
//Pre: at least one network with at least one user with at least one post exists
//Post: 'howMany' posts of the user in question are returned as one string
std::string Network::displayPosts(std::string name, std::size_t howMany, bool showOnlyPublic){
	return getUser(getID(name))->displayPosts(howMany, showOnlyPublic);
}


//readPosts function
//Pre: None
//Post: All posts from the input file have been assigned to the messages_ vectors of their corresponding users
int Network::readPosts(std::string fname){
	std::ifstream inFile(fname);
	if (!inFile.is_open()) {
        std::cout << "Network error: readPosts function could not open file." << std::endl;
        return -1;
    }

	std::size_t numPosts;
	inFile >> numPosts;
	for(std::size_t i = 0; i < numPosts; ++i){
	    std::size_t messageID = 0;
	    std::string messageText = "";
	    std::size_t ownerUserID = 0;
	    int numLikes = 0;
	    bool isPublic = true;
		std::string incomingAuthorName = "";

		inFile >> messageID;
		std::string currLine;
		std::getline(inFile,currLine); // reads in the \n following messageID
		std::getline(inFile,currLine); // reads in the post's actual message
		messageText = currLine.substr(1,currLine.length());
		inFile >> ownerUserID;
		inFile >> numLikes;
		std::getline(inFile,currLine); // reads in the \n following numLikes
		std::getline(inFile,currLine); // reads in whether the post is public or private. if it's neither, then we know the post isn't an incoming post
		if(currLine != "\tpublic" && currLine != "\tprivate"){ //must be a regular post
			Post * currPost = new Post(messageID,messageText,numLikes);
			getUser(ownerUserID)->addPost(currPost);
		}
		else{ //must be an incoming post
			std::string firstName, lastName;
			inFile >> firstName >> lastName;
			if(currLine == "\tprivate"){ isPublic = false; } // because by default it was true
			Post * currPost = new incomingPost(messageID,messageText,numLikes,isPublic,firstName + " " + lastName);
			getUser(ownerUserID)->addPost(currPost);
		}
	}
    inFile.close();
	return 0;
}


